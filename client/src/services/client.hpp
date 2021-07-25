#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include "../../dependencies/net_framework/olc_net.h"

enum class CustomMsgTypes : uint32_t
{
	ServerAccept,
	ServerDeny,
	ServerPing,
	MessageAll,
	ServerMessage,
};



class CustomClient : public olc::net::client_interface<CustomMsgTypes>
{
public:
	void PingServer()
	{
		olc::net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::ServerPing;

		// Caution with this...
		std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();		

		msg << timeNow;
		Send(msg);
	}

	void MessageAll()
	{
		olc::net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::MessageAll;		
		Send(msg);
	}
};

int client() {
	//Initialize SDL
	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
	}
	std::cout << "Succesfully initialised SDL window and renderer.\n";

	//Connect to server
	CustomClient c;
	c.Connect("127.0.0.1", 3000);

	SDL_Event event;
	int waitCounter { 0 };
	bool bQuit = false;
	while (!bQuit)
	{
		SDL_PollEvent(&event);

        if(waitCounter > 5) {
            waitCounter = 0;
        } else if ( waitCounter > 0) {
            waitCounter++;
			continue;
        }

        switch(event.type) {
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym) {
                    case SDLK_SPACE:
                        std::cout << "Detected key press:\t'space'\npinging server...\n";
                        c.PingServer();
                        waitCounter++;
                        break;
                    case SDLK_1:
                        std::cout << "Detected key press:\t'1'\npinging server...\n";
                        c.PingServer();
                        waitCounter++;
                        break;
                    case SDLK_ESCAPE:
                        std::cout << "Detected key press:\t'escape'\nexiting...\n";
                        bQuit = true;
                        break;
                }
                break;
        }

		if (c.IsConnected())
		{
			if (!c.Incoming().empty())
			{
				auto msg = c.Incoming().pop_front().msg;

				switch (msg.header.id)
				{
				case CustomMsgTypes::ServerAccept:
				{
					// Server has accepted the connection
					std::cout << "Server Accepted Connection\n";
				}
				break;


				case CustomMsgTypes::ServerPing:
				{
					// Server has responded to a ping request
					std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
					std::chrono::system_clock::time_point timeThen;
					msg >> timeThen;
					std::cout << "Ping: " << std::chrono::duration<double>(timeNow - timeThen).count() << "\n";
				}
				break;

				case CustomMsgTypes::ServerMessage:
				{
					// Server has responded to a message request
					uint32_t clientID;
					msg >> clientID;
					std::cout << "Hello from [" << clientID << "]\n";
				}
				break;
				}
			}
		}
		else
		{
			std::cout << "Server Down\n";
			bQuit = true;
		}
	}

	return 0;
}