CC = g++
CFLAGS = -std=c++17
FRAMEWORKS = -F/Library/Frameworks
LINKER_FLAGS = -F/Library/Frameworks/ -framework SDL2 -framework SDL2_image -framework SDL2_ttf -framework SDL2_mixer
SOURCES = main.cpp text.cpp button.cpp menu.cpp global.cpp
TARGET = test_ui_colours

all: build run

build: compile link

compile:
	${CC} -c ${SOURCES} ${FRAMEWORKS} ${CFLAGS}

link:
	${CC} -o ${TARGET} ${SOURCES:.cpp=.o} ${LINKER_FLAGS}

run:
	./${TARGET}

cleanup:
	touch ${SOURCES:.cpp=.o} && rm ${SOURCES:.cpp=.o}
	touch ${TARGET} && rm ${TARGET}
