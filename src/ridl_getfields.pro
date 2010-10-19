; docformat = 'rst'

;+
; Get the field names of a structure. 
;
; :Returns:
;    `strarr`
;
; :Params:
;    var : in, required, type=structure
;       structure to obtain the field names of
;-
function ridl_getfields, var
  compile_opt strictarr, hidden
  
  catch, error
  if (error ne 0) then begin
    catch, /cancel
    return, ['']
  endif

  return, strlowcase(tag_names(var))
end
