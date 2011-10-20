; docformat = 'rst'

;+
; Returns the user-defined routine names that would be automatically found
; with the current path, i.e., they have the same name as a filename with the
; `.pro` extension in the current `!path.`
;
; :Returns:
;    strarr
;
; :Keywords:
;    current_directory : in, optional, type=boolean
;       set to find the routines in files in the current directory instead of
;       in `!path`
;-
function ridl_getuserroutines, current_directory=current_dir, n_routines=nroutines
  compile_opt strictarr, hidden
  
  if (keyword_set(current_dir)) then begin
    files = file_search('*.pro', /test_read, count=nroutines)
  endif else begin
    dirs = strsplit(!path, ':', /extract, count=nroutines)
    if (nroutines eq 0L) then return, ['']
    
    files = file_search(dirs, '*.pro', /test_read, count=nroutines)
  endelse

  if (nroutines eq 0L) then return, ['']
  
  ; assumes the routines are named correctly according to their filenames
  routines = file_basename(files, '.pro')

  return, routines
end
