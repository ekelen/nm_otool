#! /usr/bin/env python3

from io import StringIO
import subprocess
from subprocess import Popen, PIPE
import os
from unittest import TestCase
import unittest as ut

class bcolors:
	HEADER = '\033[95m'
	OKBLUE = '\033[94m'
	OKGREEN = '\033[92m'
	WARNING = '\033[93m'
	FAIL = '\033[91m'
	ENDC = '\033[0m'
	BOLD = '\033[1m'
	UNDERLINE = '\033[4m'

dir_path = os.path.dirname(os.path.realpath(__file__))
name_otool = "ft_otool"
dir_otool = os.path.join(dir_path, "..")
otool_path = os.path.join(dir_path, "..", name_otool)

# /usr/local/mysql/lib

class Base(TestCase):
	def setUp(self):
		self.maxDiff = 10
		self.test_path = os.path.join(dir_path, "unit_test_files")

	def compare(self, test_files, flags=[]):
		for f in test_files:
			with self.subTest(f=f):
				try:
					ftot = subprocess.check_output([otool_path, os.path.join(self.test_path, f)], stderr=subprocess.DEVNULL)
					nxot = subprocess.check_output(["otool", "-t", os.path.join(self.test_path, f)], stderr=subprocess.DEVNULL)
					self.assertEqual(ftot, nxot, msg=f'{f} does not match.')
				except subprocess.CalledProcessError:
					ftot = subprocess.check_output([otool_path, os.path.join(self.test_path, f)], stderr=subprocess.STDOUT)
					self.assertTrue(ftot, msg=f'{f} not causing error.')

	def compare_multiple_valid(self, test_files, flags=[]):
		for f_pair in list(zip(test_files[:-1], test_files[1:])):
			with self.subTest(f_pair=f_pair):
				ftot = subprocess.check_output([otool_path, os.path.join(self.test_path, f_pair[0]), os.path.join(self.test_path, f_pair[1])], stderr=subprocess.DEVNULL)
				nxot = subprocess.check_output(["otool", "-t", os.path.join(self.test_path, f_pair[0]), os.path.join(self.test_path, f_pair[1])], stderr=subprocess.DEVNULL)
				self.assertEqual(ftot, nxot, msg=f'{f_pair} does not match.')

	def check_corrupted(self, test_files):
		for f in test_files:
			with self.subTest(f=f):
				try:
					ftot = subprocess.check_output([otool_path, os.path.join(self.test_path, f)], stderr=subprocess.DEVNULL)
					self.assertFalse(ftot, msg=f'{f} not causing error.')
				except subprocess.CalledProcessError as e:
					self.assertEqual(e.returncode, 1)
			
class Easy(Base):
	def setUp(self):
		super().setUp()
		self.files = ["test_facile", "test_moins_facile", "test_half_obj", "test_wrong_lc_command_size"]
		self.valids = ["test_facile", "test_moins_facile"]
		self.invalids = ["test_half_obj", "test_wrong_lc_command_size"]

	def test_easy(self):
		""" All the easy ones."""
		self.compare(self.files)

	def test_multiple_valid(self):
		""" compare vlaid pairs """
		self.compare_multiple_valid(self.valids)

	def test_easy_corrupted(self):
		self.check_corrupted(self.invalids)

class T32(Base):
	def setUp(self):
		self.test_path = os.path.join(dir_path, "unit_test_files", "32")
		self.files = os.listdir(self.test_path)

	def test_ls_32(self):
		self.compare(["MachO-OSX-x86-ls"])

	def test_32_exe_hard(self):
		self.compare(["32_exe_hard"])

class T32_Hard(T32):
	def test_Helloworld_32(self):
		self.compare(["MachO-iOS-armv7s-Helloworld"])

	def test_32_openssl(self):
		self.compare(["MachO-OSX-ppc-openssl-1.0.1h"])

	def test_32_arm117(self):
		self.compare(["MachO-iOS-arm1176JZFS-bash"])

class T64(Base):
	def setUp(self):
		self.test_path = os.path.join(dir_path, "unit_test_files", "64")
		self.files = os.listdir(self.test_path)

	def test_all_64_otool(self):
		""" All the 64 ones."""
		self.compare(self.files)

	def test_all_64_otool_multiple(self):
		""" Multiple 64 at once. """
		self.compare_multiple_valid(self.files)

class Fat(Base):

	def setUp(self):
		self.test_path = os.path.join(dir_path, "unit_test_files", "fat")

	def test_audiodevice_32_multi_arm(self):
		self.compare(["audiodevice"])

	def test_appsleep_64_native_CIGAM(self):
		self.compare(["appsleepd"])

	def test_arms_32_hellowworld(self):
		self.compare(["MachO-iOS-armv7-armv7s-arm64-Helloworld"])

	# def test_fat_hard(self):
	# 	self.compare(["fat_hard"])

	def test_32_ppc_i386_CIGAM(self):
		self.compare(["MachO-OSX-ppc-and-i386-bash"])

	def test_all_fat(self):
		""" All the fat ones."""
		files = os.listdir(self.test_path)
		self.compare(files)
	
class Statlib(Base):
	def setUp(self):
		self.test_path = os.path.join(dir_path, "unit_test_files", "lib_stat")

	def test_libmalloc(self):
		self.compare(["libmalloc_test.a"])

	def test_libft32(self):
		self.compare(["libft32.a"])

	def test_libftprintf32(self):
		self.compare(["libftprintf32.a"])

	def test_libftprintf64(self):
		self.compare(["libftprintf.a"])

	def test_all_archive(self):
		""" All the archives"""
		files = os.listdir(self.test_path)
		self.compare(files)

class Corrupt(Base):

	def setUp(self):
		self.test_path = os.path.join(dir_path, "unit_test_files", "corrupt")

	def test_hard_32(self):
		self.compare(["32_exe_hard_corupted"])

	def test_fat_not_fail_except_one_32(self):
		self.compare(["fat_not_fail_except_one"])

	def test_mega_bad_string(self):
		self.check_corrupted(["mega_bad_string"])

	def test_all_corrupt(self):
		""" All the corrupt """
		files = os.listdir(self.test_path)
		self.check_corrupted(files)

class Dylib(Base):

	def setUp(self):
		self.test_path = os.path.join(dir_path, "unit_test_files", "fat_lib")

	def test_libAccountPolicyTranslation_dylib(self):
		self.compare(["libAccountPolicyTranslation.dylib"])

	def test_libpmenergy_dylib(self):
		self.compare(["libpmenergy.dylib"])

	def test_libpmsample_dylib(self):
		self.compare(["libpmsample.dylib"])

	def test_all_fat_lib(self):
		""" All the fat libs ones."""
		files = os.listdir(self.test_path)[:10]
		self.compare(files)

if __name__ == '__main__':
	cmd = ["make", "-C", dir_otool, "re"]
	subprocess.run(cmd)
	ut.main()