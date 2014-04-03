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
function ridl_getfields_class_addprops, props1, props2
  compile_opt strictarr, hidden

  if (props1[0] eq '') then begin
    if (props2[0] eq '') then begin
      props = ['']
    endif else begin
      props = props2
    endelse
  endif else begin
    if (props2[0] eq '') then begin
      props = props1
    endif else begin
      props = [props1, props2]
    endelse
  endelse 

  return, props 
end


;+
; Find parameter information about the `getProperty` method of the given
; class.
;
; :Returns:
;    structure or -1L, if `getProperty` method of given class not found
;
; :Private:
;
; :Params:
;    classname : in, required, type=string
;       classname of class to find parameter information about
;
; :Keywords:
;    found : out, optional, type=boolean
;       set to a named variable to return whether the parameter information
;       about the `getProperty` method of the class was found
;-
function ridl_getfields_class_check, classname, found=found
  compile_opt strictarr, hidden

  catch, error
  if (error ne 0L) then begin
    catch, /cancel
    found = 0B
    return, -1L
  endif
    
  param_info = routine_info(classname + '::GETPROPERTY', /parameters)

  found = 1B

  return, param_info
end


;+
; Returns the properties of the given class, and its superclasses. Returns a
; 1-element array with an empty string if no properties found.
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
function ridl_getfields_class, classname
  compile_opt strictarr, hidden

  param_info = ridl_getfields_class_check(classname, found=found)
  if (~found) then return, ['']
  
  ind = where(param_info.kw_args eq '_REF_EXTRA', count, $
              complement=cind, ncomplement=ccount)
  props = ccount eq 0L ? [''] : param_info.kw_args[cind]
  
  if (count gt 0L) then begin
    superclasses = obj_class(classname, /superclass, count=nsuperclasses)
    for s = 0L, nsuperclasses - 1L do begin
      super_props = ridl_getfields_class(superclasses[s])
      props = ridl_getfields_class_addprops(props, super_props)
    endfor
  endif
  
  return, props
end


;+
; Returns the properties of an object, in alphabetical order. Returns a
; 1-element array with an empty string if no properties found.
;
; :Private:
;
; :Returns:
;    string array
;
; :Bugs:
;    This routine will check for keywords to the `::getProperty` methods of
;    superclasses of the object and assume they are properties of the object.
;    This is not correct in all cases, though is quite common. A class does
;    not necessarily query its superclass for properties and a class may query
;    other member objects that are not superclasses.
;
; :Params:
;    var : in, required, type=objref
;       object to check for properties
;-
function ridl_getfields_object, var
  compile_opt strictarr, hidden

  if (obj_hasmethod(var, 'GETPROPERTY')) then begin
    props = ridl_getfields_class(obj_class(var))
    return, obj_isa(var, 'IDL_OBJECT') ? props[sort(props)] : ['']
  endif else return, ['']
end


;+
; Get the field names of a structure.
;
; :Returns:
;    string array
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
  
  case size(var, /type) of
    8: return, strlowcase(tag_names(var))
    11: return, ridl_getfields_object(var)
    else: return, ['']
  endcase
end
