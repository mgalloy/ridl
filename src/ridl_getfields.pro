function ridl_getfields, varname
  compile_opt strictarr, hidden
  
  catch, error
  if (error ne 0) then begin
    catch, /cancel
    return, ['']
  endif

  return, strlowcase(tag_names(varname))
end
