include ../../Makefile.rules

all: ${OBJLET}
	${CC} -L../../ -l${LIB} ${BINFLAGS} -o ${BIN} ${OBJLET}
	mv ${BIN} ..

%.o:
	${CC} ${BINFLAGS} -c $(@:.o=.cc)

clean:
	rm -fR *.o *.a *.so .*.swp ${BIN}

