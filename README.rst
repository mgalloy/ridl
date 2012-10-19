Building
========

The development version of the GNU Readline library is required to be
installed before building rIDL. Most UNIX-based system will only have the
Readline binaries installed by default.

rIDL uses CMake for its build system. CMake can be installed via your system
package management system or downloaded from `the CMake website
<http://cmake.org>`_.

In most cases, the only steps for building will be::

  cmake .
  make
  make install

If you want to install somewhere besides the default location, then the
``cmake`` command would look like::

  cmake -DCMAKE_INSTALL_PREFIX=install .

CMake allows the locations of the libraries it needs to be specified also. For
example, to specify a different location for Readline::

  cmake -DReadline_INCLUDE_DIR=/usr/local/readline/6.1/include \
        -DReadline_LIBRARY=/usr/local/readline/6.1/lib/libreadline.dylib \
        -DCMAKE_INSTALL_PREFIX=install \
        .

In any case, `make install` will locate all the files rIDL will need at
runtime in the proper relative locations.


Installation
============

After rIDL is built, a few aspects of your environment need to be set for rIDL
to work correctly.

The `share/idl` directory of the installation should in your `IDL_PATH`. This
can also be set via the IDL Workbench preferences or the `PREF_SET` command,
at the user's preference.

Set the `RIDL_EDITOR` or the `EDITOR` environment variable to specify the
editor to use with the ``.edit command``. For example, under the Bash shell
this would be::

  export RIDL_EDITOR="emacs -nw"


Optional Makefile targets
=========================

There are several optional Makefile targets that can be built from the source.

More useful for end users, the `docs` target produces a PDF manual and man
page for rIDL. If you want the system `man` command to find your man pages,
make sure to add the `$INSTALL_DIR/man` to the `MANPATH` environment variable.

Also, the `onlinehelp` target parses the IDL online help to produce text-based
help system from within rIDL. To produce the documentation, use::

  make onlinehelp 
  make install
  
To test the online help, from the rIDL prompt, try checking the help for an
IDL routine with the `:help` magic command::

  rIDL> :help congrid

The `apidocs` and `capidocs` targets are primarily for developers of rIDL to
create API documentation for the rIDL source code. The `apidocs` target
requires IDLdoc to be installed and in IDL's path; the `capidocs` target
requires Doxygen to be installed.


More Information
================

See the `rIDL Trac site <http://ridl.idldev.com/>`_ for more information about
rIDL.
