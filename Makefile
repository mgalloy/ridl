.PHONY: all clean both
	
all:
	cd src; make

both:
	cd src; make both
	
clean:
	cd src; make clean