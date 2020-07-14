all: compile runCpp
	compile && runCpp

compile:
	g++ -o pong pong.cpp -F/Library/Frameworks/Python.framework/Versions/3.8/Headers -framework Python -F/Library/Frameworks/SDL2.framework/headers -framework SDL2 -std=c++11

runCpp:
	./pong
