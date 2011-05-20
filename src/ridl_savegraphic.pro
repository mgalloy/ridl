; docformat = 'rst'

;+
; Save a graphic to a PNG filename.
;
; :Params:
;    filename : in, required, type=string
;       filename of PNG file to save output from graphic
;
; :Keywords:
;    new_graphic : in, optional, type=boolean
;       set to indictate to save the current new graphics window
;    itool : in, optional, type=boolean
;       set to indicate to save the current iTools graphic window
;    direct_graphic : in, optional, type=boolean
;       set to indicate to save the current direct graphics window
;-
pro ridl_savegraphic, filename, $
                      new_graphic=newGraphic, $
                      itool=itool, $
                      direct_graphic=directGraphic
  compile_opt strictarr
  on_error, 2
  
  case 1 of
    keyword_set(newGraphic): begin
        windows = getWindows()
        g = windows[-1L]
        if (~obj_valid(g)) then return   ; do nothing if no new graphic
        dims = g.window.dimensions
        g->save, filename, width=dims[0], height=dims[1]
      end
    keyword_set(itool): begin
        id = iGetCurrent(tool=tool)
        if (~obj_valid(tool)) then return   ; do nothing if no current iTool
        
        ss = get_screen_size(resolution=resolution)
        iSave, filename, $
               tool_identifier=tool->getFullIdentifier(), $
               resolution=2.54 / resolution[0]
      end
    keyword_set(directGraphic): begin
        if (!d.window eq -1L) then return   ; do nothing if no current direct graphic
        
        im = tvrd(true=1)
        write_png, filename, im
      end
    else: begin
        ; graphics type not set, try to guess
        
        windows = getWindows()
        if (obj_valid(windows[0])) then begin
          ridl_savegraphic, filename, /new_graphic
          return
        endif
        
        id = iGetCurrent(tool=tool)
        if (obj_valid(tool)) then begin
          ridl_savegraphic, filename, /itool
          return
        endif
        
        if (!d.window ne -1L) then begin
          ridl_savegraphic, filename, /direct_graphic
          return
        endif
      end
  endcase
end
