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

  ; suppress "Compiled module: FILE_WHICH" message
  old_quiet = !quiet
  !quiet = 1
  fullpath = file_which(basename + '.pro', /include_current_dir)
  if (fullpath eq '') then fullpath = file_which(basename, /include_current_dir)
  if (fullpath eq '') then fullpath = basename
  !quiet = old_quiet

  editor = getenv('RIDL_EDITOR')
  if (editor eq '') then editor = getenv('EDITOR')
  if (editor eq '') then begin
    message, 'No editor set: specify $RIDL_EDITOR or $EDITOR', $
             /informational, /noname
    return
  endif
  
  spawn, editor + ' ' + fullpath
end
