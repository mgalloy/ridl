; docformat = 'rst'

;+
; Returns the names of the currently defined routines.
;
; :Returns:
;    strarr
;-
function ridl_getroutines
  compile_opt strictarr, hidden
  
  systemProcedures = routine_info(/system)
  systemFunctions = routine_info(/system, /functions)
  userProcedures = routine_info()
  userFunctions = routine_info(/functions)

  return, strlowcase([systemProcedures, $
                      systemFunctions, $
                      userProcedures, $
                      userFunctions])
end
