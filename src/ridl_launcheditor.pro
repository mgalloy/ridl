; docformat = 'rst'

;+
; 
;-
pro ridl_launcheditor, basename
  compile_opt strictarr

  fullpath = file_which(basename + '.pro', /include_current_dir)
  if (fullpath eq '') then fullpath = basename + '.pro'
    
  editor = getenv('RIDL_EDITOR')
  if (editor eq '') then editor = getenv('EDITOR')
  if (editor eq '') then begin
    print, 'No editor set: specify $EDITOR or $RIDL_EDITOR'
    return
  endif
  
  spawn, editor + ' ' + fullpath
end
