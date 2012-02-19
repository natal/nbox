include ../Makefile.rules

all: lib${LIB}.so
	echo "lib${LIB}.so generated"

lib${LIB}.so: ${LIBOBJ}
	${CC} ${SOFLAGS} -shared -o lib${LIB}.so ${LIBOBJ}
	mv lib${LIB}.so ..

%.o:
	${CC} ${SOFLAGS} -c $(@:.o=.cc)

clean:
	rm -fR *.o *.a *.so .*.swp
	rm -fR headers/*.o headers/*.a headers/*.so headers/.*.swp

distclean: clean

