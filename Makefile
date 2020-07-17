all: compile link run
	compile && link && run

compile:
	g++ -c main.cpp -std=c++11

link:
	g++ -o pong main.o -F/Library/Frameworks/SDL2.framework/headers -framework SDL2 -Wl,-stack_size,1000000  -framework CoreFoundation /Library/Frameworks/Python.framework/Versions/3.8/Python 

run:
	./pong

cleanup:
	rm main.o && rm pong
