; docformat = 'rst'

;+
; Prints the current line of source code (used if execution is stopped at a
; breakpoint).
;-
pro ridl_printsource
  compile_opt strictarr, hidden
  
  _window = 1
  
  s = scope_traceback(/structure)
  level = n_elements(s) - 2L
  
  info = s[level]
  
  nlines = file_lines(info.filename)
  startIndex = (info.line - 1L - _window / 2L) > 0
  endIndex = (info.line - 1L + _window / 2L) < (nlines - 1L)
  
  data = strarr(endIndex + 1L)
  openr, lun, info.filename, /get_lun
  readf, lun, data
  free_lun, lun
  
  indent = strarr(endIndex - startIndex + 1L) + '   '
  indent[info.line - 1L - startIndex] = '-> '
  print, indent + data[startIndex:endIndex], format='(A)'
end
