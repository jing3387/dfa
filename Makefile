CC = clang
CFLAGS = -g
LDFLAGS = -lcgraph -lgvc

NAME = cdfa
SRC = cdfa.c
OBJ = ${SRC:.c=.o}

${NAME}: ${OBJ}
	${CC} ${LDFLAGS} -o $@ ${OBJ}

${OBJ}:

.c.o:
	${CC} ${CFLAGS} -c $<

clean:
	rm -f ${NAME} ${OBJ}

.PHONY: clean
