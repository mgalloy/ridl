; docformat = 'rst'

;+
; Finds .pro files in the !path or current directory.
;
; :Params:
;    basename : in, required, type=string
;       basename of .pro file to find in !path or current directory
;-
pro ridl_launcheditor, basename
  compile_opt strictarr, hidden

  fullpath = file_which(basename + '.pro', /include_current_dir)
  if (fullpath eq '') then fullpath = basename + '.pro'
    
  editor = getenv('RIDL_EDITOR')
  if (editor eq '') then editor = getenv('EDITOR')
  if (editor eq '') then begin
    print, 'No editor set: specify $RIDL_EDITOR or $EDITOR'
    return
  endif
  
  spawn, editor + ' ' + fullpath
end
