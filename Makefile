
CC=gcc
OPTS=-Wall
PROG=othello
LIBS= pile.o

all: ${PROG}

${PROG}: othello.c ${LIBS}
	${CC} othello.c ${LIBS} ${OPTS} -o ${PROG}

file.o: file.c file.h
	${CC} -c file.c ${OPTS}

pile.o: pile.c pile.h
	${CC} -c pile.c ${OPTS}

clean:
	rm -f ${LIBS} ${PROG}

run:
	./othello

