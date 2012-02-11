include ../../Makefile.rules

all: ${OBJ}
	${CC} -L../../ -l${LIB} ${BINFLAGS} -o ${BIN} ${OBJ}
	mv ${BIN} ..

%.o:
	${CC} ${BINFLAGS} -c $(@:.o=.cc)

clean:
	rm -fR *.o *.a *.so .*.swp ${BIN}

