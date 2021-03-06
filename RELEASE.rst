rIDL 0.1
--------

  * Append "?" to the end of an expression to retrieve metadata about the
    expression.
    
  * The .edit executive command uses RIDL_EDITOR or EDITOR environment 
    variables to launch the specified editor.
    
  * Hitting tab will do completion on filenames, system variable names, local
    variable names (including structure field names and object properties),
    reserved words, executive commands, and magic commands.
  
  * The :doc magic command displays header information about a routine.
  
  * Use the :history magic commands to retrieve the command line history or 
    :histedit to send the commands to a file and launch your editor.
    
  * Use :log/:unlog magic commands to get a log of the session, containing 
    both commands and output::
    
       rIDL> :log filename

  * Use the :notebook/:unnotebook magic commands to log the commands entered 	     
    and both the output log and graphics output into a notebook. Use the 
    :save_graphic command to place the current new graphic, iTool, or direct
    graphics window into the notebook.

  * Use the :help magic command to get information about all the currently
    available magic commands.

  * Keybindings are supported. Currently step into, step over, and step return
    can be mapped via a Readline resource file file.
  
  * Readline history commands are available with :!. For example, to replace a 
    string::
    
       rIDL> a = '/path/to/datafile'
       rIDL> :!!:s/a/b/
       rIDL> b = '/path/to/datafile'
  
    Here, the first two lines are typed and the third line is created by the 
    second line. Search for help on using Readline history for more   
    information about commands available (just put them after a :).    
      
  * Command line switches -v to print the rIDL/IDL versions and -h to print
    help on the available command line options are added to the standard IDL
    command line switches.
    
  * Commands are saved in the history file as they are entered so that an IDL 
    crash does not lose all the history from that session.
  
  * The prompt can contain variables such as %wdir or %cmdnum which will be
    expanded for display.
    
  * Multi-line commands, when entered at the interactive command line, are 
    combined into a single line in the command history.
 
  * Online-help can be accessed with :help magic command.
  
  * rIDL preferences can be set or viewed via :pref magic command or prefs.ini
    preference configuration file in app user directory.

  
IDL functionality not in rIDL
-----------------------------
  
  * Setting preferences except for IDL_PROMPT via a command line switch when
    starting is not supported.

  * Command line args specified via a command line switch when starting and
    retrieved with the COMMAND_LINE_ARGS routine are not supported.
    
  * .step'ing into an OBJ_NEW or OBJ_DESTROY call will leave rIDL's command 
    line reading and into IDL's default mechanism until scope returns to the
    level of the original OBJ_NEW or OBJ_DESTROY.

  * You can not use .compile or .run without a filename argument to enter
    a program at the command line.
