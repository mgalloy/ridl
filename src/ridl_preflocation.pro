; docformat = 'rst'

;+
; Return location of rIDL preference file.
;
; :Returns:
;    string
;-
function ridl_preflocation
  compile_opt strictarr

  readmeVersion = 1
  
  _authorDescription = 'rIDL'
  _appDescription = 'Really Interactive Data Language'
  
  readmeText = ['This is the user configuration directory for ' + _appDescription, $
                'by ' + _authorDescription + '.']
                  
  configDir = app_user_dir(authorName, _authorDescription, $
                           appName, _appDescription, $
                           readmeText, readmeVersion)
  
  pref_loc = filepath('prefs.ini', root=configDir)
  
  return, pref_loc
end
