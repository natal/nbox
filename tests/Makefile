include ../Makefile.rules

all:
	make -C src/ all

clean:
	make -C src/ clean
	rm -fR *.o *.a *.so .*.swp *.dot *.png ${BIN}

simple:
	make -C src/ simple
	LD_LIBRARY_PATH='../' ./${BIN} map-samples/map3.nm

funreg:
	make -C src/ funreg
	LD_LIBRARY_PATH='../' ./${BIN} map-samples/map2.nm

xor:
	make -C src/ xor
	LD_LIBRARY_PATH='../' ./${BIN} map-samples/xormap.nm

exec: all
	LD_LIBRARY_PATH='../' ./${BIN} map-samples/map3.nm

debug: funreg
	LD_LIBRARY_PATH='../' gdb ./${BIN}
