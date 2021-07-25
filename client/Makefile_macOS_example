CC = g++
CFLAGS = -std=c++17
INCLUDE = -I${HOME}/Library/include/asio-1.18.2/include
FRAMEWORKS = -F/Library/Frameworks
LINKER_FLAGS = -F/Library/Frameworks/ -framework SDL2 -framework SDL2_image -framework SDL2_ttf -framework SDL2_mixer
LIB_GRAPH_SOURCES = $(patsubst %, engine/graphics/%, image.cpp rectangles.cpp text.cpp)
LIB_GRAPH_OBJECT_FILES = $(shell basename -a ${LIB_GRAPH_SOURCES:.cpp=.o})
# LIB_UI_SOURCES = 
SOURCES = main.cpp
SOURCE_OBJECT_FILES = $(shell basename -a ${SOURCES:.cpp=.o})
TARGET = pong_client

all: build_all run

build_all: build_lib_graph compile link

build_lib_graph:
	${CC} -c $(patsubst %, src/%, $(LIB_GRAPH_SOURCES)) ${FRAMEWORKS} ${CFLAGS}
	mv ${LIB_GRAPH_OBJECT_FILES} build/lib/

build: compile link

compile:
	${CC} -c $(patsubst %, src/%, $(SOURCES)) ${INCLUDE} ${FRAMEWORKS} ${CFLAGS}
	mv ${SOURCE_OBJECT_FILES} build/

link:
	${CC} -o ${TARGET} $(patsubst %, build/%, ${SOURCE_OBJECT_FILES}) $(patsubst %, build/lib/%, ${LIB_GRAPH_OBJECT_FILES}) ${LINKER_FLAGS}
	mv ${TARGET} build/

run:
	build/${TARGET}

cleanup_all: cleanup cleanup_lib

cleanup:
	rm $(patsubst %, build/%, ${SOURCE_OBJECT_FILES})

cleanup_lib:
	rm $(patsubst %, build/lib/%, ${LIB_GRAPH_OBJECT_FILES})
