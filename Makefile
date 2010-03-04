.PHONY: all clean both
	
all:
	cd src; make

both:
	cd src; make both
	
clean:
	cd src; make clean
	rm -rf capi-docs
	
install:
	cd src; make install
	
doc:
	doxygen doxygen.config