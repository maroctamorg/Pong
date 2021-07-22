CC = g++
CFLAGS = -std=c++17
INCLUDE = -I${HOME}/Library/include/asio-1.18.2/include
SOURCES = server.cpp
TARGET = pong_server

all: build run

build: compile link

compile:
	${CC} -c ${SOURCES} ${INCLUDE} ${CFLAGS}
	mv ${SOURCES:.cpp=.o} build/

link:
	${CC} -o ${TARGET} build/${SOURCES:.cpp=.o} 
	mv ${TARGET} build/

run:
	build/${TARGET}

cleanup:
	touch build/${SOURCES:.cpp=.o} && rm build/${SOURCES:.cpp=.o}
	touch build/${TARGET} && rm build/${TARGET}