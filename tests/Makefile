include ../Makefile.rules

all:
	make -C src/ all

clean:
	make -C src/ clean
	rm -fR *.o *.a *.so .*.swp ${BIN}

exec: all
	LD_LIBRARY_PATH='../' ./${BIN} map-samples/map3.nm

debug: all
	LD_LIBRARY_PATH='../' gdb ./${BIN}
