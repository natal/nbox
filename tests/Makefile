include ../Makefile.rules

all:
	make -C src/ all

clean:
	make -C src/ clean
	make -C letters/ clean
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

letter:
	make -C letters/ all
	LD_LIBRARY_PATH='../' ./${BIN} letters/letter_map.nm

exec: all
	LD_LIBRARY_PATH='../' ./${BIN} map-samples/map3.nm

debug:
	make -C src/ funreg
	LD_LIBRARY_PATH='../' gdb ./${BIN}
