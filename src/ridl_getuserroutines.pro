; docformat = 'rst'

;+
; Returns the user-defined routine names that would be automatically found
; with the current path, i.e., they have the same name as a filename with the
; `.pro` extension in the current `!path.`
;
; :Returns:
;    strarr
;-
function ridl_getuserroutines
  compile_opt strictarr, hidden
  
  dirs = strsplit(!path, ':', /extract, count=ndirs)
  
  ; add the current directory
  if (ndirs eq 0L) then begin
    dirs = '.'
  endif else begin
    dirs = ['.', dirs]
  endelse
  
  files = file_search(dirs, '*.pro', /test_read, count=nfiles)
  if (nfiles eq 0L) then return, ['']
  
  routines = file_basename(files, '.pro')

  return, routines
end
