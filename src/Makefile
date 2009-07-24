RIDL=ridl
IDLDIR=/Applications/itt/idl
#BINDIR=$(IDLDIR)/bin/bin.darwin.x86_64
#ARCH= -arch x86_64
BINDIR=$(IDLDIR)/bin/bin.darwin.i386
ARCH= 
LDFLAGS= -no-cpp-precomp -dynamic -fno-common \
	-L$(BINDIR) -L/usr/X11/lib \
	-lidl -lXm -lMesaGL6_2 -lMesaGLU6_2 -lOSMesa6_2 -lreadline -lfreetype \
	-multiply_defined suppress

.PHONY: clean
	
ridl : $(RIDL).o
	cc $(RIDL).o -o $(RIDL) $(ARCH) $(LDFLAGS) 
	
ridl.o : $(RIDL).c
	cc $(ARCH) -I$(IDLDIR)/external/include -I/usr/local/include -c $(RIDL).c -o $(RIDL).o
	
clean:
	rm -f $(RIDL).o
	rm -f $(RIDL)
	