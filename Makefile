# Makefile permettant de créer le projet LO41

CC = gcc
C_STANDARD = -std=c99
INCLUDE_DIR = headers
SOURCES_DIR = sources
APPLICATION_NAME = APP_LO41
FILENAME = ${APPLICATION_NAME}
CFLAGS = -Wall -ansi -pedantic ${C_STANDARD}
LDFLAGS = -lpthread
FILES = ${SOURCES_DIR}/*.c

all : ${FILENAME}

${FILENAME}: ${FILES}
	${CC} ${CFLAGS} -o $@ $^ -I${INCLUDE_DIR} ${LDFLAGS}

clean:
	${RM} *.o ${FILENAME}

run: ${FILENAME}
	./${FILENAME} ${arg}

