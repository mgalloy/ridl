RIDL=ridl
IDLDIR=/Applications/itt/idl
ARCH=x86_64
#ARCH=i386
ARCHFLAGS= -arch $(ARCH)
BINDIR=$(IDLDIR)/bin/bin.darwin.$(ARCH)
LDFLAGS= -no-cpp-precomp -dynamic -fno-common \
	-L$(BINDIR) -L/usr/X11/lib \
	-lidl -lXm -lMesaGL6_2 -lMesaGLU6_2 -lOSMesa6_2 -lreadline -lfreetype \
	-multiply_defined suppress

.PHONY: clean install

$(RIDL) : $(RIDL).o
	cc $(RIDL).o -o $(RIDL) $(ARCHFLAGS) $(LDFLAGS) 

$(RIDL).o : $(RIDL).c
	cc $(ARCHFLAGS) -I$(IDLDIR)/external/include -I/usr/local/include -c $(RIDL).c -o $(RIDL).o

install:
	mkdir -p bin.darwin.$(ARCH)
	mv $(RIDL) bin.darwin.$(ARCH)
		
clean:
	rm -f $(RIDL).o
	rm -f $(RIDL)
	