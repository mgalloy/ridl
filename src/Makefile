RIDL_VERSION=0.1
RIDL=ridl
IDLDIR=/Applications/itt/idl
ARCH=x86_64
#ARCH=i386
ARCHFLAGS= -arch $(ARCH)
OS=`uname -s | tr [A-Z] [a-z]`
CC=cc
BINDIR=$(IDLDIR)/bin/bin.$(OS).$(ARCH)
READLINE_LIB=/usr/local/lib
LDFLAGS= -no-cpp-precomp -dynamic -fno-common \
	-L$(BINDIR) -L/usr/X11/lib -L$(READLINE_LIB) \
	-lidl -lXm -lMesaGL6_2 -lMesaGLU6_2 -lOSMesa6_2 -lreadline -lfreetype \
	-multiply_defined suppress
CFLAGS=

.PHONY: clean install both

$(RIDL) : $(RIDL).o $(RIDL)_strings.o $(RIDL)_magic.o $(RIDL)_history.o $(RIDL)_logging.o $(RIDL)_completion.o
	$(CC) $(RIDL).o $(RIDL)_strings.o $(RIDL)_magic.o $(RIDL)_history.o \
	      $(RIDL)_logging.o $(RIDL)_completion.o -o $(RIDL) \
	      $(ARCHFLAGS) $(LDFLAGS)
	
$(RIDL)_strings.o : $(RIDL)_strings.c
	$(CC) $(ARCHFLAGS) -I$(IDLDIR)/external/include -I/usr/local/include \
	      -c $(RIDL)_strings.c -o $(RIDL)_strings.o $(CFLAGS)

$(RIDL)_history.o : $(RIDL)_history.c
	$(CC) $(ARCHFLAGS) -I$(IDLDIR)/external/include -I/usr/local/include \
				-c $(RIDL)_history.c -o $(RIDL)_history.o $(CFLAGS)

$(RIDL)_magic.o : $(RIDL)_magic.c
	$(CC) $(ARCHFLAGS) -I$(IDLDIR)/external/include -I/usr/local/include \
				-c $(RIDL)_magic.c -o $(RIDL)_magic.o $(CFLAGS)

$(RIDL)_logging.o : $(RIDL)_logging.c
	$(CC) $(ARCHFLAGS) -I$(IDLDIR)/external/include -I/usr/local/include \
				-c $(RIDL)_logging.c -o $(RIDL)_logging.o $(CFLAGS)

$(RIDL)_completion.o : $(RIDL)_completion.c
	$(CC) $(ARCHFLAGS) -I$(IDLDIR)/external/include -I/usr/local/include \
				-c $(RIDL)_completion.c -o $(RIDL)_completion.o $(CFLAGS)

$(RIDL).o : $(RIDL).c Makefile
	echo "#define RIDL_VERSION \"$(RIDL_VERSION)\"" > ridl_version.h
	echo "#define RIDL_REVISION \"`svnversion`\"" >> ridl_version.h
	echo "#define RIDL_BUILD_DATE \"`date \"+%b %d %Y\"`\"" >> ridl_version.h
	$(CC) $(ARCHFLAGS) -I$(IDLDIR)/external/include -I/usr/local/include \
	      -c $(RIDL).c -o $(RIDL).o $(CFLAGS)
	
both:
	make clean
	make
	make install
	make clean
	make ARCH=i386 READLINE_LIB=/Users/mgalloy/lib/readline-6.0-32/lib
	make ARCH=i386 install
	
install:
	mkdir -p bin.$(OS).$(ARCH)
	cp $(RIDL) bin.$(OS).$(ARCH)
		
clean:
	rm -f $(RIDL).o
	rm -f $(RIDL)_strings.o
	rm -f $(RIDL)_magic.o
	rm -f $(RIDL)_history.o
	rm -f $(RIDL)_logging.o
	rm -f $(RIDL)_completion.o
	rm -f $(RIDL)
	rm -rf bin.*
	