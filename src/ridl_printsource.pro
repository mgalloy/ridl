; docformat = 'rst'

;+
; Prints the current line of source code (used if execution is stopped at a 
; breakpoint).
;-
pro ridl_printsource
  compile_opt strictarr, hidden
  
  s = scope_traceback(/structure)
  level = n_elements(s) - 2L
  
  info = s[level]
  
  data = strarr(info.line)
  openr, lun, info.filename, /get_lun
  readf, lun, data
  free_lun, lun
  
  print, data[info.line - 1L], format='(%"-> %s")'
end
