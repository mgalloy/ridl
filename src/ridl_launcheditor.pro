; docformat = 'rst'

;+
; Finds `.pro` files in the `!path` or current directory.
;
; :Params:
;    basename : in, required, type=string
;       basename of `.pro` file to find in `!path` or current directory; may
;       already have `.pro` extension
;-
pro ridl_launcheditor, basename
  compile_opt strictarr, hidden

  ; add .pro extension if not already present
  _basename = strlowcase(strmid(basename, 3, /reverse_offset)) eq '.pro' $
                ? basename $
                : (basename + '.pro')
                  
  ; suppress "Compiled module: FILE_WHICH" message
  old_quiet = !quiet
  !quiet = 1
  if (file_test(_basename)) then fullpath = _basename else begin
    fullpath = file_which(_basename, /include_current_dir)
    if (fullpath eq '') then fullpath = _basename
  endelse
  !quiet = old_quiet

  ; check for editor specified by environment variables: RIDL_EDITOR, EDITOR
  editor = getenv('RIDL_EDITOR')
  if (editor eq '') then editor = getenv('EDITOR')
  if (editor eq '') then begin
    message, 'No editor: set RIDL_EDITOR or EDITOR environment variable', $
             /informational, /noname
    return
  endif
  
  ; spawn the editor
  spawn, editor + ' ' + fullpath
end
