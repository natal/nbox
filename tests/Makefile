include ../Makefile.rules

all:
	make -C src/ all

clean:
	make -C src/ clean
	rm -fR *.o *.a *.so .*.swp ${BIN}

exec:
	./ann map-samples/map.nm

check: all
	LD_LIBRARY_PATH='../' ./${BIN} map-samples/map.nm
