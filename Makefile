# Makefile permettant de créer le projet LO41

CC = gcc
C_STANDARD = -std=c99
INCLUDE_DIR = headers
SOURCES_DIR = sources
RESSOURCE_DIR = ressources
BUILD_DIR = build
APPLICATION_NAME = APP_LO41
FILENAME = ${APPLICATION_NAME}
CFLAGS = -Wall -ansi -pedantic ${C_STANDARD}
LDFLAGS = -lpthread
DEBUG_SUFFIX = _debug
CFLAGS_DEBUG = -g -DDEBUG
FILES = ${SOURCES_DIR}/*.c

all : ${FILENAME} ${FILENAME}${DEBUG_SUFFIX}

${FILENAME}: ${FILES}
	${CC} ${CFLAGS} -o $@ $^ -I${INCLUDE_DIR} ${LDFLAGS}

${FILENAME}${DEBUG_SUFFIX}: ${FILES}
	${CC} ${CFLAGS} ${CFLAGS_DEBUG} -o $@ $^ -I${INCLUDE_DIR} ${LDFLAGS}

clean:
	${RM} *.o ${FILENAME} ${FILENAME}${DEBUG_SUFFIX}
	${RM} -rf ${BUILD_DIR}/*.dSYM
