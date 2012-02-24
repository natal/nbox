include ../../Makefile.rules

all: ${OBJSIM}
	${CC} -L../../ -l${LIB} ${BINFLAGS} -o ${BIN} ${OBJSIM}
	mv ${BIN} ..

simple: ${OBJSIM}
	${CC} -L../../ -l${LIB} ${BINFLAGS} -o ${BIN} ${OBJSIM}
	mv ${BIN} ..

funreg: ${OBJFR}
	${CC} -L../../ -l${LIB} ${BINFLAGS} -o ${BIN} ${OBJFR}
	mv ${BIN} ..

%.o:
	${CC} ${BINFLAGS} -c $(@:.o=.cc)

clean:
	rm -fR *.o *.a *.so .*.swp ${BIN}

