TEMPLATE = subdirs
SUBDIRS = sub_src sub_unittest

CONFIG += x86 x86_64
sub_src.subdir = src
sub_unittest.subdir = tests
sub_unittest.depends = sub_src
