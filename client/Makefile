CC = g++
CFLAGS = -std=c++17
INCLUDE = -I${HOME}/Library/include/asio-1.18.2/include
FRAMEWORKS = -F/Library/Frameworks
LINKER_FLAGS = -F/Library/Frameworks/ -framework SDL2
SOURCES = main.cpp engine/game/pong.cpp
TARGET = pong_client

all: build run

build: compile link

compile:
	${CC} -c ${SOURCES} ${INCLUDE} ${FRAMEWORKS} ${CFLAGS}
	mv ${SOURCES:.cpp=.o} build/

link:
	${CC} -o ${TARGET} build/${SOURCES:.cpp=.o} ${LINKER_FLAGS}
	mv ${TARGET} build/

run:
	build/${TARGET}

cleanup:
	mkdir build && touch build/${SOURCES:.cpp=.o} && rm build/${SOURCES:.cpp=.o}
	mkdir build && touch build/${TARGET} && rm build/${TARGET}
