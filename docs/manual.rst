rIDL 0.1 User Manual
====================

:Author: Michael Galloy


What is rIDL?
-------------

rIDL is an enhanced command line interface to IDL. 

Technically, rIDL is a C program that uses the Callable IDL API to pass your commands to IDL, while adding a few extra features in the translation.


Magic commands
--------------

Magic commands are extra commands not provided by IDL to perform actions
related to your rIDL session like saving the input and/or output of your
session to a file, provide help/version information about rIDL, lookup syntax
for an IDL routine, etc.

The available rIDL commands are listed in the following table.

========================= ==================================================
Magic command             Description
========================= ==================================================
``:colors``               toggle whether color is used
``:doc routine``          show calling syntax and comment header for the
                          routine
``:help [verbose]``       show this help message; 'verbose' option shows 
                          longer version
``:history [n] [nonum]``  show last n commands (defaults to 10); 'nonum' 
                          option hides command numbers
``:histedit n filename``  send last n commands to filename and launch editor
``:log filename``         start logging all commands and output to filename
``:unlog``                stop logging commands and output
``:notebook filename``    start sending all commands and output to an HTML 
                          notebook filename
``:unnotebook``           stop logging commands and output to notebook
``:save_graphic``         save current new graphic, iTool, or direct graphic 
                          window to notebook
``:tee filename``         start logging output to filename
``:untee``                stop logging output
``:version``              print version information
========================= ==================================================


Completion
----------

Command line completion is a handy convenience to minimize typing when you
have already typed enough to identify something uniquely. Even if you haven't
typed enough to uniquely indentify something, the currently available options
can be displayed.

rIDL completes on:

  1. IDL library routines
  2. system variables
  3. local variables (including fields of structures)
  4. reserved words
  5. executive commands
  6. magic commands
  7. filenames
  
Use the tab key to indicate that rIDL should attempt a completion on the
current command line. For example, if you have already entered::

   IDL> a = find

then hit the tab key, rIDL should complete this to::

   IDL> a = findgen

You can then complete the line as required. If you have not typed enough to
uniquely identify an item, then the first press of the tab key will not do
anything, but a second press will print all the items that currently match.

rIDL uses the popular GNU Readline library to do command completion, so it
supports the normal extensions and modifications of a Readline program. An
example ``inputrc`` file is provided in the ``src`` directory which defines a
few rIDL specific key bindings. It can be called from a ``.inputrc`` file.


Other features
--------------

TODO: continued lines merged into a single line

TODO: ``.edit`` works (uses ``RIDL_EDITOR`` or ``EDITOR`` environment variables)

TODO: : ``-h`` and ``-v`` options to the binary::

   $ ridl -v
   rIDL 0.1.r166: Really Interactive Data Language. [Build: Nov 04 2010]
   IDL Version 8.0.1, Mac OS X (darwin x86_64 m64). [Build: Oct  5 2010]

   $ ridl -h
   rIDL 0.1.r166: Really Interactive Data Language. [Build: Nov 04 2010]
   IDL Version 8.0.1, Mac OS X (darwin x86_64 m64). [Build: Oct  5 2010]

   usage: ridl [options] [batch_filename]

   options:
     -32                       start in 32-bit mode
     -arg value                saves a single command line argument
     -args value1 value2 ...   saves multiple command line arguments
     -demo                     start in 7-minute demo mode
     -e command                executes command and exit
     -em=FILENAME              execute the given .sav file containing an embedded license
     -h                        display this help message
     -IDL_PREF value           set the IDL_PREF preference for this session
     -novm                     use 7-minute demo mode instead of virtual machine with -rt
     -pref=FILENAME            load the given preference file
     -queue                    wait for a license when none are available
     -quiet                    suppress printing information about IDL on startup
     -rt=FILENAME              start with a runtime license on the given .sav file
     -student                  start in student mode
     -ulicense                 start IDL with a unique license
     -v                        display version information
     -vm=FILENAME              start the virtual machine with the given .sav file

TODO: prompt: ``%wdir`` and ``cmdnum`` variables can be used in the prompt


Where to go for more help?
--------------------------

Mailing list?

rIDL Trac site `ridl.idldev.com <http://ridl.idldev.com>`_

Contact me at ``mgalloy at idldev dot com``.
