#!/bin/sh

cleanconf.sh

case `uname` in
  Linux)
    source linux_configure.sh
    ;;
  Darwin)
    source homebrew_configure.sh
    ;;
  *)
    ;;
esac

make
make onlinehelp
make install

LN_CMD="ln -s -f -v $PREFIX/ $PREFIX_ROOT/ridl"

# delete $PREFIX_ROOT/ridl if it already exists
if [ -e "$PREFIX_ROOT/ridl" ]; then
  rm $PREFIX_ROOT/ridl
fi

echo $LN_CMD
$LN_CMD
