; docformat = 'rst'

;+
; Retrieve events; needs to be called periodically when command line is
; waiting for a new command.
;-
pro ridl_getevents
  compile_opt strictarr, hidden
  
  common managed, ids, names, outermodal
  
  if (n_elements(ids) gt 0L) then dummy = widget_event(/nowait)
end