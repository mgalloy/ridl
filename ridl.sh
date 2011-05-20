PREFER_32=0
OS=`uname -s | tr [A-Z] [a-z]`

for arg
do
  if [ "$arg" = "-32" ]; then
    PREFER_32=1
  fi
done

export IDL_DIR=/Applications/itt/idl/idl

UNAME=`uname`
case $UNAME in
  "Darwin")
    LIBRARY_PATH=DYLD_LIBRARY_PATH
    ;;
  *)
    LIBRARY_PATH=LD_LIBRARY_PATH
    ;;
esac

if [ $PREFER_32 = 1 ]; then 
  export $LIBRARY_PATH=$IDL_DIR/bin/bin.$OS.i386:$DYLD_LIBRARY_PATH
  $RIDL_DIR/bin/bin.$OS.i386/ridl $@
else
  export $LIBRARY_PATH=$IDL_DIR/bin/bin.$OS.x86_64:$DYLD_LIBRARY_PATH
  $RIDL_DIR/bin/bin.$OS.x86_64/ridl $@
fi
