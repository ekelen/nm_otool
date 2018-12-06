#! /usr/bin/env python3
# -*- coding: utf-8 -*-

from io import StringIO
import subprocess
from subprocess import Popen, PIPE
import os
from unittest import TestCase
import unittest as ut
import random

from util import bc, Base
import util as st

class Otool(Base):
	def setUp(self):
		super().setUp()
		self.prgm = st.otool_path
		self.ftname = st.name_otool
		self.nxname = st.nxname_otool
		self.flags = ["-t"]
		self.allowedFlags = ['t']
			
class Easy(Otool):
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
		self.check_bad_flags(self.files, flags=["-O"])

	def test_bad_flags_twohyphen(self):
		self.check_bad_flags(self.files, flags=["-t-t"])

class T32(Otool):
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

class T64(Otool):
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

class Fat(Otool):
	def setUp(self):
		super().setUp()
		self.test_path = os.path.join(st.dir_ut, "fat")
		self.basenames = self.getBasenames()
		self.files = self.getPaths(self.basenames)

	def test_2(self):
		""" compare fat pairs """
		self.compare_multi_random(self.files, k=2, n=len(self.files)//2)

	def test_3(self):
		""" compare 3 fat """
		self.compare_multi_random(self.files, k=3, n=len(self.files)//2)

	def test_all(self):
		""" All the fat """
		self.compare(self.files)
	
class Statlib(Otool):
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

	def test_all(self):
		""" All the statlib """
		self.compare(self.files)

class Corrupt(Otool):
	def setUp(self):
		super().setUp()
		self.test_path = os.path.join(st.dir_ut, "corrupt")
		self.basenames = self.getBasenames()
		self.files = self.getPaths(self.basenames)

	def test_all_corrupt(self):
		""" All the corrupt """
		self.check_corrupted(self.files)

class Dylib(Otool):
	def setUp(self):
		super().setUp()
		self.test_path = os.path.join(st.dir_ut, "fat_lib")
		self.basenames = self.getBasenames()
		self.files = self.getPaths(self.basenames)

	def test_2(self):
		""" compare 1 random fatlib pairs """
		self.compare_multi_random(self.files, k=2, n=1)

	def test_all_part1(self):
		""" All some fatlib """
		self.compare(self.files, israndom=True, k=2)

if __name__ == '__main__':
	subprocess.run([*st.make, 'fclean'])
	subprocess.run([*st.make, st.name_otool])
	ut.main()