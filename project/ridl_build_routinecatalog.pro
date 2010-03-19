pro ridl_routinecatalog::startElement, uri, local, qName, attname, attvalue
  compile_opt strictarr
  
  if (local eq 'ROUTINE') then begin
    ind = where(attname eq 'name', count)
    if (count gt 0L) then begin
      routine_name = attvalue[ind[0]]
      if (idl_validname(routine_name) ne '') then begin
        printf, self.lun, strlowcase(routine_name)
      endif else begin
        routine_names = strsplit(routine_name, '/', count=count, /extract)
        if (count gt 1) then begin
          printf, self.lun, strlowcase(transpose(routine_names))
        endif else begin
          print, routine_name, format='(%"Excluding %s...")'
        endelse
      endelse
    endif
  endif
end


pro ridl_routinecatalog::clean
  compile_opt strictarr
  
  free_lun, self.lun
end


function ridl_routinecatalog::init, filename=filename
  compile_opt strictarr
  
  openw, lun, filename, /get_lun
  self.lun = lun
  
  return, 1
end


pro ridl_routinecatalog__define
  compile_opt strictarr
  
  define = { rIDL_RoutineCatalog, inherits IDLffXMLSAX, lun: 0L }

end


; main-level routine to build simpler catalog

xmlfile = filepath('idl_catalog.xml', subdir=['help'])
cat = obj_new('rIDL_RoutineCatalog', filename='idl_routines.txt')
cat->parseFile, xmlfile
obj_destroy, cat

end
