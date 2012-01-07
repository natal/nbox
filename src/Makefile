include ../Makefile.rules

all: ${OBJ}
	${CC} ${FLAGS} -o ${BIN} ${OBJ}
	mv ${BIN} ..

%.o:
	${CC} ${FLAGS} -c $(@:.o=.cc)

clean:
	rm -fR *.o *.a *.so .*.swp

distclean: clean

