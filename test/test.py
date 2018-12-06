#! /usr/bin/env python3
# -*- coding: utf-8 -*-

from io import StringIO
import sys
import subprocess
from subprocess import Popen, PIPE
import os
from unittest import TestCase
import unittest as ut
import random

from util import bc, Base
import util as st

class Nm(Base):
	def setUp(self):
		super().setUp()
		self.prgm = st.nm_path
		self.ftname = st.name_nm
		self.nxname = st.nxname_nm
		self.flags = []
		self.allowedFlags = ['u', 'U', 'r']

class Easy(Nm):
	def setUp(self):
		super().setUp()
		self.basenames = self.getBasenames()
		self.files = self.getPaths(self.basenames)
		self.valids = ["test_facile", "test_moins_facile"]
		self.invalids = ["test_half_obj", "test_wrong_lc_command_size"]

	def test_all(self):
		""" All the easy ones."""
		self.compare(self.files)

	def test_2(self):
		""" compare easy pairs """
		self.compare_multi_random(self.getPaths(self.valids), k=2, n=len(self.files))

	def test_easy_corrupted(self):
		self.check_corrupted(self.getPaths(self.invalids))

	def test_bad_flags_t(self):
		self.check_bad_flags(self.files, flags=["-t"])

	def test_bad_flags_twohyphen(self):
		self.check_bad_flags(self.files, flags=["--t"])

	def test_flags(self):
		allowedFlags = self.getValidFlagCombinations()
		for d in allowedFlags:
			with self.subTest(d=d):
				self.compare_one(self.getPaths(self.valids), flags=[d])

class T32(Nm):
	def setUp(self):
		super().setUp()
		self.test_path = os.path.join(st.dir_ut, "32")
		self.basenames = self.getBasenames()
		self.files = self.getPaths(self.basenames)

	def test_2(self):
		""" compare 32-bit pairs """
		self.compare_multi_random(self.files, k=2, n=len(self.files))

	def test_all(self):
		""" All the 32-bit """
		self.compare(self.files)

	def test_flags(self):
		allowedFlags = self.getValidFlagCombinations()
		for d in allowedFlags:
			with self.subTest(d=d):
				self.compare_one(random.sample(self.files, 1), flags=[d])

class T64(Nm):
	def setUp(self):
		super().setUp()
		self.test_path = os.path.join(st.dir_ut, "64")
		self.basenames = self.getBasenames()
		self.files = self.getPaths(self.basenames)

	def test_2(self):
		""" compare 32-bit pairs """
		self.compare_multi_random(self.files, k=2, n=len(self.files))

	def test_all(self):
		""" All the 32-bit """
		self.compare(self.files)

	def test_flags(self):
		allowedFlags = self.getValidFlagCombinations()
		for d in allowedFlags:
			with self.subTest(d=d):
				self.compare_one(random.sample(self.files, 1), flags=[d])

class Fat(Nm):
	def setUp(self):
		super().setUp()
		self.test_path = os.path.join(st.dir_ut, "fat")
		self.basenames = self.getBasenames()
		self.files = self.getPaths(self.basenames)

	def test_2(self):
		""" compare fat pairs """
		self.compare_multi_random(self.files, k=2, n=len(self.files))

	def test_3(self):
		""" compare 3 fat """
		self.compare_multi_random(self.files, k=3, n=len(self.files))

	def test_all(self):
		""" All the fat """
		self.compare(self.files)

	def test_flags(self):
		allowedFlags = self.getValidFlagCombinations()
		for d in allowedFlags:
			with self.subTest(d=d):
				self.compare_one(random.sample(self.files, 1), flags=[d])
	
class Statlib(Nm):
	def setUp(self):
		super().setUp()
		self.test_path = os.path.join(st.dir_ut, "lib_stat")
		self.basenames = self.getBasenames()
		self.files = self.getPaths(self.basenames)

	def test_2(self):
		""" compare statlib pairs """
		self.compare_multi_random(self.files, k=2, n=len(self.files))

	def test_3(self):
		""" compare 3 statlib """
		self.compare_multi_random(self.files, k=3, n=len(self.files))

	def test_flags(self):
		allowedFlags = self.getValidFlagCombinations()
		for d in allowedFlags:
			with self.subTest(d=d):
				self.compare_one(random.sample(self.files, 1), flags=[d])

	def test_all(self):
		""" All the statlib """
		self.compare(self.files)

class Corrupt(Nm):
	def setUp(self):
		super().setUp()
		self.test_path = os.path.join(st.dir_ut, "corrupt")
		self.basenames = self.getBasenames()
		self.files = self.getPaths(self.basenames)

	def test_all_corrupt(self):
		""" All the corrupt """
		self.check_corrupted(self.files)

class Dylib(Nm):
	def setUp(self):
		super().setUp()
		self.test_path = os.path.join(st.dir_ut, "fat_lib")
		self.basenames = self.getBasenames()
		self.files = self.getPaths(self.basenames)

	def test_2(self):
		""" compare 1 random fatlib pairs """
		self.compare_multi_random(self.files, k=2, n=1)

	def test_all(self):
		""" All some fatlib """
		self.compare(self.files)

if __name__ == '__main__':
	subprocess.run(st.make_re)
	ut.main()