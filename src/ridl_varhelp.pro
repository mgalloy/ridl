; docformat = 'rst'

function ridl_varhelp_typename, var
  compile_opt strictarr
  
  case size(var, /type) of
    0: return, '<undefined>'
    1: return, 'byte'
    2: return, 'int'
    3: return, 'long'
    4: return, 'float'
    5: return, 'double'
    6: return, 'complex'
    7: return, 'string'
    8: return, 'structure'
    9: return, 'double complex'
    10: return, 'pointer'
    11: return, 'object'
    12: return, 'unsigned integer'
    13: return, 'unsigned long'
    14: return, '64-bit integer'
    15: return, 'unsigned 64-bit integer'
    else:
  endcase
end


;+
; Display introspection information about a variable.
;
; :Params:
;    var : in, required, type=any
;       variable to display information about
;-
pro ridl_varhelp, var
  compile_opt strictarr, hidden
  
  ; TODO: need to do more here, depending on variable type (also variable
  ; name is wrong, it is always "var" since that is the local name)
  
  varname = scope_varname(var, level=-1)
  if (varname eq '') then varname = '<expression>'
  
  print, varname, format='(%"Name: %s")'
  print, ridl_varhelp_typename(var), format='(%"Type: %s")'
  
  case size(var, /type) of
    11: begin
        if (obj_isa(var, 'idl_object')) then print, var->_overloadHelp(varname)
        print, 'Class hierarchy:'
        ridl_class_hierarchy, var, hierarchy=hierarchy, /no_print
        ridl_class_hierarchy_print, hierarchy, indent='  '
        ridl_class_hierarchy_cleanup, hierarchy
      end
    else:
  endcase
end
