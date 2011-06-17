; docformat = 'rst'

;+
; Return location of rIDL preference file.
;
; :Returns:
;    string
;-
function ridl_preflocation
  compile_opt strictarr, hidden

  readmeVersion = 1
  
  authorName = 'mgalloy'
  authorDescription = 'michaelgalloy.com'
  appName = 'rIDL'
  appDescription = 'Really Interactive Data Language'
  
  readmeText = ['This is the user configuration directory for ' + appName, $
                'by ' + authorName + '.']
                  
  configDir = app_user_dir(authorName, authorDescription, $
                           appName, appDescription, $
                           readmeText, readmeVersion)
  
  pref_loc = filepath('prefs.ini', root=configDir)
  
  return, pref_loc
end
