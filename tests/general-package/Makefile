include ../../Makefile.rules
include learning.config

all: ${OBJTEST} ${OBJLEARN}
	${CC} -L../../ -l${LIB} ${BINFLAGS} -o ${BINLEARN} ${OBJLEARN}
	${CC} -L../../ -l${LIB} ${BINFLAGS} -o ${BINTEST} ${OBJTEST}

%.o:
	${CC} ${BINFLAGS} -c $(@:.o=.cc)

learn: all
	LD_LIBRARY_PATH='../../' ./${BINLEARN} -i${NBITERATIONS} ${DATA_FILE} ${NEURAL_MAP} ${WEIGHT_FILE}

test: all
	LD_LIBRARY_PATH='../../' ./${BINTEST} ${DATA_FILE} ${NEURAL_MAP} ${WEIGHT_FILE}

learndebug:all
	LD_LIBRARY_PATH='../../' gdb ./${BINLEARN}

testdebug:all
	LD_LIBRARY_PATH='../../' gdb ./${BINTEST}

clean:
	rm -fR *.o *.a *.so .*.swp ${BINLEARN} ${BINTEST}

