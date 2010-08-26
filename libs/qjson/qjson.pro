TEMPLATE = subdirs
SUBDIRS = sub_src sub_unittest

config += x86
sub_src.subdir = src
sub_unittest.subdir = tests
sub_unittest.depends = sub_src
