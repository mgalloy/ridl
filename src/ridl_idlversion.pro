; docformat = 'rst'

;+
; Returns the IDL version number as a string or a boolean indicating whether
; a required version is met. 
;
; :Examples:
;    For example, try::
;
;       IDL> print, ridl_idlversion()
;       8.0.1
;       IDL> print, ridl_idlversion(require='8.0.1')
;          1
;
;    Also, try the main-level program at the end of this file::
;
;       IDL> .run ridl_idlversion
;       Version: 8.0.1
;       Can require 6.4: yes
;       Can require 7.1: yes
;       Can require 7.2: yes
;
; :Returns:
;    string version number or boolean
;
; :Keywords:
;    require : in, optional, type=string
;       IDL version required; if set, `RIDL_IDLVERSION` returns a boolean of 
;       whether the version requirement is met
;-
function ridl_idlversion, require=require
  compile_opt strictarr

  version = (strsplit(!version.release, /extract))[0]
  
  if (n_elements(require) gt 0L) then begin  
    return, ridl_cmp_version(version, require) ge 0B
  endif else begin
    return, version
  endelse
end

; main-level example

print, ridl_idlversion(), format='(%"Version: %s")'
print, ridl_idlversion(require='6.4') ? 'yes' : 'no', $
       format='(%"Can require 6.4: %s")'
print, ridl_idlversion(require='7.1') ? 'yes' : 'no', $
       format='(%"Can require 7.1: %s")'
print, ridl_idlversion(require='7.2') ? 'yes' : 'no', $
       format='(%"Can require 7.2: %s")'

end
