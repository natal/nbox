include ../Makefile.rules

all: lib${LIB}.so
	echo "lib${LIB}.so generated"

lib${LIB}.so: ${LIBOBJ}
	${CC} ${SOFLAGS} -o lib${LIB}.so ${LIBOBJ}
	mv lib${LIB}.so ..

%.o:
	${CC} ${FLAGS} -c $(@:.o=.cc)

clean:
	rm -fR *.o *.a *.so .*.swp

distclean: clean

