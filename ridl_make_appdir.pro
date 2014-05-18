!quiet = 1
.compile src/ridl_preflocation
loc = ridl_preflocation()
cmake_source_dir = command_line_args()
; will fail (silently because !quiet is on) if prefs.ini is already present
if (~file_test(loc)) then file_copy, filepath('example_prefs.ini', subdir='src', root=cmake_source_dir), loc
exit
