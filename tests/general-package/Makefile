include ../../Makefile.rules
include learning.config

all: ${OBJTEST} ${OBJLEARN}
	${CC} -L../../ -l${LIB} ${BINFLAGS} -o ${BINLEARN} ${OBJLEARN}
	${CC} -L../../ -l${LIB} ${BINFLAGS} -o ${BINTEST} ${OBJTEST}

%.o:
	${CC} ${BINFLAGS} -c $(@:.o=.cc)

learn: all
	LD_LIBRARY_PATH='../../' ./${BINLEARN} -i${NBITERATIONS} ${DATA_FILE} ${NEURAL_MAP} ${WEIGHT_FILE}


clean:
	rm -fR *.o *.a *.so .*.swp ${BINLEARN} ${BINTEST}

