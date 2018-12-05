#! /usr/bin/env python3

from io import StringIO
import subprocess
from subprocess import Popen, PIPE
import os
from unittest import TestCase
import unittest as ut

class bc:
	HEADER = '\033[95m'
	OKBLUE = '\033[94m'
	OKGREEN = '\033[92m'
	WARNING = '\033[93m'
	FAIL = '\033[91m'
	ENDC = '\033[0m'
	BOLD = '\033[1m'
	UNDERLINE = '\033[4m'

dir_path = os.path.dirname(os.path.realpath(__file__))
name_nm = "ft_nm"
dir_nm = os.path.join(dir_path, "..")
nm_path = os.path.join(dir_path, "..", name_nm)

class Base(TestCase):
	def setUp(self):
		self.maxDiff = 10
		self.test_path = os.path.join(dir_path, "unit_test_files")

	def check_bad_flags(self, test_files, flags=[]):
		for f in test_files:
			with self.subTest(f=f):
				ftnm = subprocess.check_output([nm_path, os.path.join(self.test_path, f)] + flags, stderr=subprocess.STDOUT)
				print(f'{bc.WARNING}ftnm : {ftnm}{bc.ENDC}')
				self.assertTrue(ftnm, msg=f'{ftnm}: {f} with flags {flags} not causing error.')

	def compare(self, test_files, flags=[]):
		for f in test_files:
			with self.subTest(f=f):
				try:
					ftnm = subprocess.check_output([nm_path, os.path.join(self.test_path, f)] + flags, stderr=subprocess.DEVNULL)
					nxnm = subprocess.check_output(["nm", os.path.join(self.test_path, f)] + flags, stderr=subprocess.DEVNULL)
					self.assertEqual(ftnm, nxnm, msg=f'{f} with flags {flags} does not match.')
				except subprocess.CalledProcessError:
					ftnm = subprocess.check_output([nm_path, os.path.join(self.test_path, f)] + flags, stderr=subprocess.STDOUT)
					self.assertTrue(ftnm, msg=f'{f} with flags {flags} not causing error.')
					# raise RuntimeError("command '{}' return with error (code {}): {}".format(e.cmd, e.returncode, e.output))

	def compare_multiple_valid(self, test_files, flags=[]):
		for f_pair in list(zip(test_files[:-1], test_files[1:])):
			with self.subTest(f_pair=f_pair):
				ftnm = subprocess.check_output([nm_path, os.path.join(self.test_path, f_pair[0]), os.path.join(self.test_path, f_pair[1])] + flags, stderr=subprocess.DEVNULL)
				nxnm = subprocess.check_output(["nm", os.path.join(self.test_path, f_pair[0]), os.path.join(self.test_path, f_pair[1])] + flags, stderr=subprocess.DEVNULL)
				self.assertEqual(ftnm, nxnm, msg=f'{f_pair} does not match.')

	def ensure_no_stdout_for_corrupted(self, test_files):
		for f in test_files:
			with self.subTest(f=f):
				try:
					ftnm = subprocess.check_output([nm_path, os.path.join(self.test_path, f)], stderr=subprocess.DEVNULL)
					if ftnm:
						print(f'{bc.FAIL}ftnm : {ftnm.decode()}{bc.ENDC}')
					self.assertFalse(ftnm, msg=f'{f} returning message {bc.FAIL}{ftnm}{bc.ENDC}')
				except subprocess.CalledProcessError as e:
					self.assertEqual(e.returncode, 1)

	def ensure_has_at_least_stderr_output(self, test_files):
		""" check has output on stderr """
		for f in test_files:
			with self.subTest(f=f):
				try:
					ftnm = subprocess.check_output([nm_path, os.path.join(self.test_path, f)], stderr=subprocess.STDOUT)
					print(f'{bc.WARNING}ftnm : {ftnm.decode()}{bc.ENDC}')
					self.assertTrue(ftnm, msg=f'{f} has no error output.')
				except subprocess.CalledProcessError as e:
					self.assertEqual(e.returncode, 1)

	def check_corrupted(self, test_files):
		self.ensure_has_at_least_stderr_output(test_files)
		self.ensure_no_stdout_for_corrupted(test_files)


class Easy(Base):
	def setUp(self):
		super().setUp()
		self.files = ["test_facile", "test_moins_facile", "test_half_obj", "test_wrong_lc_command_size"]
		self.valids = ["test_facile", "test_moins_facile"]
		self.invalids = ["test_half_obj", "test_wrong_lc_command_size"]

	def test_multiple_valid(self):
		""" compare valid pairs """
		self.compare_multiple_valid(self.valids)

	def test_corrupt(self):
		""" compare invalid files """
		self.check_corrupted(self.invalids)

class T32(Base):
	def setUp(self):
		self.test_path = os.path.join(dir_path, "unit_test_files", "32")

	def test_all_32(self):
		""" All the 32 ones."""
		files = os.listdir(self.test_path)
		self.compare(files)

class T64(Base):

	def setUp(self):
		self.test_path = os.path.join(dir_path, "unit_test_files", "64")

	def test_all_64(self):
		""" All the 64 ones."""
		files = os.listdir(self.test_path)
		self.compare(files)

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
		self.ensure_no_stdout_for_corrupted(["mega_bad_string"])

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
		files = os.listdir(self.test_path)
		self.compare(files)

class Easyflags(Base):
	def setUp(self):
		super().setUp()
		self.files = ["test_facile", "test_moins_facile", "test_half_obj", "test_wrong_lc_command_size"]

	def test_easy_only_undef(self):
		""" Only undefined."""
		self.compare(self.files, flags=["-u"])

	def test_easy_no_undef(self):
		""" No undefined."""
		self.compare(self.files, flags=["-U"])

	def test_easy_all(self):
		""" All."""
		self.compare(self.files, flags=["-a"])

	def test_easy_reverse_undef(self):
		""" No undefined."""
		self.compare(self.files, flags=["-ru"])

	def test_bad_flags(self):
		""" No undefined."""
		self.check_bad_flags(self.files, flags=["-ruG"])

	def test_reverse(self):
		""" Reverse."""
		self.compare(self.files, flags=["-r"])

class T32flags(Base):
	def setUp(self):
		self.test_path = os.path.join(dir_path, "unit_test_files", "32")
		self.files = os.listdir(self.test_path)

	def test_only_undef(self):
		""" Only undefined."""
		self.compare(self.files, flags=["-u"])

	def test_no_undef(self):
		""" No undefined."""
		self.compare(self.files, flags=["-U"])

	def test_reverse(self):
		""" Reverse."""
		self.compare(self.files, flags=["-r"])

	def test_reverse_only_undef(self):
		""" Reverse."""
		self.compare(self.files, flags=["-ru"])

class T64flags(Base):
	def setUp(self):
		self.test_path = os.path.join(dir_path, "unit_test_files", "64")
		self.files = os.listdir(self.test_path)

	def test_only_undef(self):
		""" Only undefined."""
		self.compare(self.files, flags=["-u"])

	def test_no_undef(self):
		""" No undefined."""
		self.compare(self.files, flags=["-U"])

	def test_reverse(self):
		""" Reverse."""
		self.compare(self.files, flags=["-r"])

class Dylibflags(Base):
	def setUp(self):
		self.test_path = os.path.join(dir_path, "unit_test_files", "fat_lib")
		self.files = os.listdir(self.test_path)

	def test_no_undef(self):
		""" No undefined."""
		self.compare(self.files, flags=["-U"])

if __name__ == '__main__':
	cmd = ["make", "-C", dir_nm, "re"]
	subprocess.run(cmd)
	ut.main()