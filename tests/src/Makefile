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

xor: ${OBJXOR}
	${CC} -L../../ -l${LIB} ${BINFLAGS} -o ${BIN} ${OBJXOR}
	mv ${BIN} ..

%.o:
	${CC} ${BINFLAGS} -c $(@:.o=.cc)

clean:
	rm -fR *.o *.a *.so .*.swp ${BIN}

