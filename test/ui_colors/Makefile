CC = g++
CFLAGS = -std=c++17
FRAMEWORKS = -F/Library/Frameworks
LINKER_FLAGS = -F/Library/Frameworks/ -framework SDL2 -framework SDL2_image -framework SDL2_ttf -framework SDL2_mixer
SOURCES = main.cpp text.cpp button.cpp menu.cpp global.cpp
SOURCE_OBJECT_FILES = $(shell basename -a ${SOURCES:.cpp=.o})
TARGET = test_ui_colours

all: build run

build: compile link

compile:
	${CC} -c $(SOURCES) ${INCLUDE} ${FRAMEWORKS} ${CFLAGS}
	mv ${SOURCE_OBJECT_FILES} build/

link:
	${CC} -o ${TARGET} $(patsubst %, build/%, ${SOURCE_OBJECT_FILES}) ${LINKER_FLAGS}
	mv ${TARGET} build/

run:
	build/${TARGET}

cleanup:
	rm $(patsubst %, build/%, ${SOURCE_OBJECT_FILES})
