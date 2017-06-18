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

.PHONY: all clean

all : ${FILENAME}
	@echo "Building ..."

${FILENAME}: ${FILES}
	${CC} ${CFLAGS} -o $@ $^ -I${INCLUDE_DIR} ${LDFLAGS}

clean:
	@echo "cleaning up ..."
	${RM} *.o ${FILENAME}

run: ${FILENAME}
	@echo "Running application ..."
	./${FILENAME} ${arg}

