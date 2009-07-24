PREFER_32=0
for arg
do
  if [ "$arg" = "-32" ]; then
    PREFER_32=1
  fi
done

DYLD_LIBRARY_PATH=/Applications/itt/idl/bin/bin.darwin.i386
bin.darwin.i386/ridl

DYLD_LIBRARY_PATH=/Applications/itt/idl/bin/bin.darwin.x86_64
bin.darwin.x86_64/ridl
