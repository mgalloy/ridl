; docformat = 'rst'

;+
; Add two list of property names with 1-element array containing an empty 
; string representing an empty list of names.
;
; :Private:
;
; :Returns:
;    string array
;
; :Params:
;    props1 : in, required, type=`strarr`
;      first list of property names
;    props2 : in, required, type=`strarr`
;      second list of property names
;-
function ridl_getmethods_class_addmethods, methods1, methods2
  compile_opt strictarr

  if (methods1[0] eq '') then begin
    if (methods2[0] eq '') then begin
      methods = ['']
    endif else begin
      methods = methods2
    endelse
  endif else begin
    if (methods2[0] eq '') then begin
      methods = methods1
    endif else begin
      methods = [methods1, methods2]
      ind = uniq(methods, sort(methods))
      methods = methods[ind]
    endelse
  endelse 

  return, methods 
end


;+
; Returns the method names of the given class, and its superclasses. Returns a 
; 1-element array with an empty string if no methods are found.
;
; :Private:
;
; :Returns:
;    string array
;
; :Params:
;    classname : in, required, type=string
;       classname to find the properties for
;-
function ridl_getmethods_class, classname
  compile_opt strictarr
  
  ; get methods of class
  help, /source, output=output
  re = string(classname, format='(%"^%s::([^[:space:]]+)")')
  methods = stregex(output, re, /extract, /subexpr)
  ind = where(methods[0, *] ne '', count)
  methodnames = count eq 0L ? [''] : reform(methods[1, ind])
  
  ; get methods of superclasses
  superclasses = obj_class(classname, /superclass, count=nsuperclasses)
  for s = 0L, nsuperclasses - 1L do begin
    super_methodnames = ridl_getmethods_class(superclasses[s])
    methodnames = ridl_getmethods_class_addmethods(methodnames, super_methodnames)
  endfor
  
  return, methodnames
end


;+
; Get the method names of an object. 
;
; :Returns:
;    string array
;
; :Params:
;    var : in, required, type=structure
;       object to obtain the method names of
;-
function ridl_getmethods, var
  compile_opt strictarr, hidden

  return, strlowcase(ridl_getmethods_class(obj_class(var)))
end
