!quiet = 1
.compile src/ridl_preflocation
loc = ridl_preflocation()
; will fail (silently because !quiet is on) if prefs.ini is already present
file_copy, filepath('example_prefs.ini', subdir='src', root='.'), loc
exit
