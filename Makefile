include Makefile.rules

all:
	make -C src/ all

clean:
	make -C src/ clean
	rm -fR *.o *.a *.so .*.swp ann

distclean: clean
	make -C src/ distclean
	rm -fRr doc/html doc/latex
	mv -fR Makefile.rules

dist: distclean
	cd .. && tar cjvf ann.tar.bz2 ann/ --exclude ='.git'
	mv ../ann.tar.bz2 .

check: all
	./check/check.sh
