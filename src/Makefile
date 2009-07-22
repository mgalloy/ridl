IDLDIR=/Applications/itt/idl
BINDIR=$(IDLDIR)/bin/bin.darwin.x86_64
RIDL=ridl

LDFLAGS= -arch x86_64 -no-cpp-precomp -dynamic -fno-common \
	-L$(BINDIR) -L/usr/X11/lib \
	-lidl -lXm -lMesaGL6_2 -lMesaGLU6_2 -lOSMesa6_2 -lreadline -lfreetype \
	-multiply_defined suppress

.PHONY: clean
	
ridl : $(RIDL).o
	cc $(RIDL).o -o $(RIDL) $(LDFLAGS) 
	
ridl.o : $(RIDL).c
	cc -arch x86_64 -I$(IDLDIR)/external/include -I/usr/local/include -c $(RIDL).c 
	
clean:
	rm -f $(RIDL).o
	rm -f $(RIDL)	