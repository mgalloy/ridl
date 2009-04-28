IDLDIR=/Applications/itt/idl
BINDIR=$(IDLDIR)/bin/bin.darwin.x86_64
CALLABLE_TEST=callable_test

LDFLAGS= -arch x86_64 -no-cpp-precomp -dynamic -fno-common \
	-L$(BINDIR) -L/usr/X11/lib \
	-lidl -lXm -lMesaGL6_2 -lMesaGLU6_2 -lOSMesa6_2 -lfreetype \
	-multiply_defined suppress


# Build the C language test program
test : $(CALLABLE_TEST).o
	cc $(CALLABLE_TEST).o -o $(CALLABLE_TEST) $(LDFLAGS) 
	
callable_test.o : $(CALLABLE_TEST).c
	cc -arch x86_64 -I$(IDLDIR)/external/include -c $(CALLABLE_TEST).c 

clean:
	rm -f $(CALLABLE_TEST).o
	rm -f $(CALLABLE_TEST)