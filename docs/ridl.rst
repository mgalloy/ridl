====
rIDL
====

--------------------------------------
enhanced command line interface to IDL
--------------------------------------

:Author: rIDL was written by Michael Galloy (``mgalloy@idldev.com``)
:Date:   2011-05-26
:Copyright: BSD
:Version: 0.1
:Manual section: 1
:Manual group: scientific analysis/visualization


SYNOPSIS
--------

ridl [options] [filename]

  
DESCRIPTION
-----------

A filename, with or without the *.pro* extension, may be passed to ``ridl`` to
be executed as a batch file.

The following options are available:

:-32:
  start in 32-bit mode
:-arg *value*:
  saves a single command line argument
:-args *value1* *value2* ...:
  saves multiple command line arguments; must be the last option on the line
:-demo:
  start in 7-minute demo mode
:-e *command*:
  executes command and exit
:-em=\ *FILENAME*:
  execute the given *.sav* file containing an embedded license
:-h:
  display this help message
:-IDL_PREF *value*:
  set the IDL_PREF preference for this session
:-novm:
  use 7-minute demo mode instead of virtual machine with ``--rt``
:-pref=\ *FILENAME*:
  load the given preference file
:-queue:
  wait for a license when none are available
:-quiet:
  suppress printing information about IDL on startup
:-rt=\ *FILENAME*:
  start with a runtime license on the given *.sav* file
:-student:
  start in student mode
:-ulicense:
  start IDL with a unique license
:-v:
  display version information
:-vm=\ *FILENAME*:
  start the virtual machine with the given *.sav* file
     


EXAMPLES
--------

To run the batch file at *my_batch_file.pro*::

  $ ridl my_batch_file

To execute a *.sav* file with the virtual machine::

  $ ridl -vm=my_savfile.sav


DIAGNOSTICS
-----------

rIDL returns ``0`` on normal completion or the exit status given by the
``EXIT`` command in IDL.


ENVIRONMENT
-----------

The ``.edit`` executive command will use the editor specified by the
``RIDL_EDITOR`` environment variable, if specified. If not, it will check
``EDITOR`` before quitting.

    
BUGS
----

See the rIDL GitHub site at ``github.com/mgalloy/ridl`` for current information
about bugs in rIDL.
