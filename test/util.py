#! /usr/bin/env python3
# -*- coding: utf-8 -*-

import sys
from io import StringIO
import subprocess
from subprocess import Popen, PIPE
import os
from unittest import TestCase
import unittest as ut
import random
from itertools import combinations

class bc:
	HEADER = '\033[95m'
	OKBLUE = '\033[94m'
	OK = '\033[92m'
	WARNING = '\033[93m'
	FAIL = '\033[91m'
	ENDC = '\033[0m'
	BOLD = '\033[1m'
	UNDERLINE = '\033[4m'

dir_path = os.path.dirname(os.path.realpath(__file__))
root_path = os.path.join(dir_path, "..")
dir_ut = os.path.join(dir_path, "unit_test_files")
name_otool = "ft_otool"
name_nm = "ft_nm"
nxname_nm = "nm"
nxname_otool = "otool"
nm_path = os.path.join(root_path, name_nm)
otool_path = os.path.join(root_path, name_otool)
make = ["make", "-C", root_path]

class Base(TestCase):
	def setUp(self):
		self.test_path = dir_ut
		self.maxDiff = 10
		self.prgm = ''
		self.ftname = ''
		self.nxname = ''
		self.flags = []
		self.allowedFlags = []
		self.verbose = bool('-v' in sys.argv)

	def getValidFlagCombinations(self):
		flagCombinations = set('-' + x for x in self.allowedFlags)
		for i in range(2, len(self.allowedFlags) + 1):
			for p in (set(combinations(self.allowedFlags, i))):
				pf = '-' + ''.join(p)
				flagCombinations.add(pf)
		return flagCombinations

	def getBasenames(self):
		return os.listdir(self.test_path)

	def getPaths(self, test_files):
		return [os.path.join(self.test_path, f) for f in test_files]

	def compare_one(self, paths=[], flags=[]):
		if not flags:
			flags = self.flags
		ft_out = subprocess.check_output([self.prgm, *flags, *paths], stderr=subprocess.DEVNULL)
		nx_out = subprocess.check_output([self.nxname, *flags, *paths], stderr=subprocess.DEVNULL)
		self.assertEqual(ft_out, nx_out, msg=f'{bc.FAIL}{paths} does not match with {flags}.{bc.ENDC}')
		if self.verbose:
			basenames = [os.path.split(p)[1] for p in paths]
			print(f'{bc.OK}{basenames} {flags} .. OK{bc.ENDC}')

	def compare(self, paths, israndom=False, k=1, flags=[]):
		if not flags:
			flags = self.flags
		if israndom and k > 0:
			paths = random.sample(paths, k)
		for f in paths:
			with self.subTest(f=f):
				try:
					nx_out = subprocess.check_output([self.nxname, *flags, f], stderr=subprocess.DEVNULL)
				except subprocess.CalledProcessError:
					return
				ft_out = subprocess.check_output([self.prgm, *flags, f], stderr=subprocess.DEVNULL)
				nx_out = subprocess.check_output([self.nxname, *flags, f], stderr=subprocess.DEVNULL)
				self.assertEqual(ft_out, nx_out, msg=f'{bc.FAIL}{f} does not match with {flags}.{bc.ENDC}')
				if self.verbose:
					print(f'{bc.OK}{[self.prgm, *flags, f]} OK {bc.ENDC}')

	def compare_multi_random(self, test_files, flags=[], k=2, n=4):
		if not flags:
			flags = self.flags
		av = list()
		for _ in range(n):
			av.append(random.sample(test_files, k))
		for f in av:
			with self.subTest(f=f):
				ft_out = subprocess.check_output([self.prgm, *flags, *f], stderr=subprocess.DEVNULL)
				nx_out = subprocess.check_output([self.nxname, *flags, *f], stderr=subprocess.DEVNULL)
				self.assertEqual(ft_out, nx_out, msg=f'\n{bc.FAIL}{self.ftname}: {f} does not match.{bc.ENDC}')

	def ensure_no_stdout_for_corrupted(self, test_files, flags=[]):
		for f in test_files:
			with self.subTest(f=f):
				ft_out = subprocess.check_output([self.prgm, *flags, f], stderr=subprocess.DEVNULL)
				self.assertFalse(ft_out, msg=f'{bc.FAIL}{self.ftname} : {ft_out.decode()}{bc.ENDC}')

	def ensure_has_at_least_stderr_output(self, test_files, flags=[]):
		for f in test_files:
			with self.subTest(f=f):
				ft_out = subprocess.check_output([self.prgm, *flags, f], stderr=subprocess.STDOUT)
				self.assertTrue(ft_out, msg=f'{bc.FAIL}{f} has no error output.{bc.ENDC}')
				if self.verbose:
					print(f'{bc.OK}{self.ftname} : {ft_out.decode()}{bc.ENDC}')

	def check_corrupted(self, test_files, flags=[]):
		if not flags:
			flags = self.flags
		self.ensure_has_at_least_stderr_output(test_files, flags)
		self.ensure_no_stdout_for_corrupted(test_files, flags)

	def check_bad_flags(self, test_files, flags=[]):
		for f in test_files:
			with self.subTest(f=f):
				ft_out = subprocess.check_output([self.prgm, *flags, f], stderr=subprocess.STDOUT)
				
				self.assertTrue(ft_out, msg=f'{bc.FAIL}{self.ftname}: {f} with flags {flags} has no error output.{bc.ENDC}')
				if self.verbose:
					print(f'{bc.OK}{self.ftname} : {ft_out.decode()}{bc.ENDC}')

	