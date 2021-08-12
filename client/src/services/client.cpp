#include "client.hpp"

std::string custom_struct_utils::toString(Game_Info info) {
	std::ostringstream oss;
	oss << "\ndone: " << (info.done ? "true" : "false") << ",\tscore: " << info.score << "\nracket:\tx: " << info.x << ", y: " << info.y << '\n';
	return oss.str();
}

void CustomClient::PingServer() {
	olc::net::message<CustomMsgTypes> msg;
	msg.header.id = CustomMsgTypes::ServerPing;

	// Caution with this...
	std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();

	msg << timeNow;
	Send(msg);
}

void CustomClient::JoinRandomSession(std::string username) {
	olc::net::message<CustomMsgTypes> msg;
	msg.header.id = CustomMsgTypes::JoinRandomSession;
	msg.write(username);
	Send(msg);
}

void CustomClient::CreateCustomSession() {
	olc::net::message<CustomMsgTypes> msg;
	msg.header.id = CustomMsgTypes::CreateCustomSession;
	Send(msg);
}

void CustomClient::JoinCustomSession(std::string session_id) {
	olc::net::message<CustomMsgTypes> msg;
	msg.header.id = CustomMsgTypes::JoinCustomSession;
	msg.write(session_id);
	Send(msg);
}

void CustomClient::SendGameInfo(Game_Info* game_info) {
	if(!game_info) return;
	olc::net::message<CustomMsgTypes> msg;
	msg.header.id = CustomMsgTypes::GameInfo;
	msg << *game_info;
	// std::cout << "Sending game_info data:" << custom_struct_utils::toString(*game_info);
	Send(msg);
}

void CustomClient::LeaveSession() {
	olc::net::message<CustomMsgTypes> msg;
	msg.header.id = CustomMsgTypes::LeaveSession;
	Send(msg);
}

STATE CustomClient::handleIncoming(Game_Info* game_info) {
	if (this->olc::net::client_interface<CustomMsgTypes>::IsConnected()) {
		if (!this->olc::net::client_interface<CustomMsgTypes>::Incoming().empty()) {
			auto msg = this->olc::net::client_interface<CustomMsgTypes>::Incoming().pop_front().msg;

			switch (msg.header.id) {
			case CustomMsgTypes::ServerPing: {
				// Server has responded to a ping request
				std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
				std::chrono::system_clock::time_point timeThen;
				msg >> timeThen;
				std::cout << "Ping: " << std::chrono::duration<double>(timeNow - timeThen).count() << "\n";
			}
			break;

			case CustomMsgTypes::ServerAccept:
			{
				// Server has accepted the connection
				std::cout << "Server Accepted Connection\n";
			}
			break;

			// case CustomMsgTypes::JoinRandomSession: {
			// 	// HANDLE RANDOM SESSION
			// }
			// break;

			// case CustomMsgTypes::CreateCustomSession: {
			// 	// HANDLE CREATE CUSTOM SESSION
			// }
			// break;
			
			// case CustomMsgTypes::JoinCustomSession: {
			// 	// HANDLE JOIN CUSTOM SESSION
			// }
			// break;

			case CustomMsgTypes::EstablishedSession: {
				int flag { -1 };
				msg >> flag;
				if(flag == 0)
					return STATE::START;
				else if (flag == 1)
					return STATE::WAITING;
				else {
					return STATE::END;
					std::cout << "Unexpected return type!\n";
				}
			}
			break;
			
			case CustomMsgTypes::GameInfo: {
				// HANDLE GAME INFO
				if(!game_info) return STATE::END;
				msg >> *game_info;
				return STATE::GAME_INFO;
			}
			break;
			
			case CustomMsgTypes::ErrorMessage: {
				std::cout << "Received error message from server! Disconnecting...\n";
				return STATE::END;
			}
			break;

			case CustomMsgTypes::LeaveSession: {
				// HANDLE LEAVE SESSION
			}
			break;
			}
		}
		return STATE::QUIET;
	}
	else {
		std::cout << "The connection to the server was closed!\n";
		return STATE::END;
	}
}

// int client() {
// 	//Initialize SDL
// 	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
// 	{
// 		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
// 	}
// 	std::cout << "Succesfully initialised SDL window and renderer.\n";

// 	//Connect to server
// 	CustomClient c;
// 	c.Connect("127.0.0.1", 3000);

// 	SDL_Event event;
// 	int waitCounter { 0 };
// 	bool bQuit = false;
// 	while (!bQuit)
// 	{
// 		SDL_PollEvent(&event);

//         if(waitCounter > 5) {
//             waitCounter = 0;
//         } else if ( waitCounter > 0) {
//             waitCounter++;
// 			continue;
//         }

//         switch(event.type) {
//             case SDL_KEYDOWN:
//                 switch(event.key.keysym.sym) {
//                     case SDLK_SPACE:
//                         std::cout << "Detected key press:\t'space'\npinging server...\n";
//                         c.PingServer();
//                         waitCounter++;
//                         break;
//                     case SDLK_1:
//                         std::cout << "Detected key press:\t'1'\npinging server...\n";
//                         c.PingServer();
//                         waitCounter++;
//                         break;
//                     case SDLK_ESCAPE:
//                         std::cout << "Detected key press:\t'escape'\nexiting...\n";
//                         bQuit = true;
//                         break;
//                 }
//                 break;
//         }

// 		if (c.IsConnected())
// 		{
// 			if (!c.Incoming().empty())
// 			{
// 				auto msg = c.Incoming().pop_front().msg;

// 				switch (msg.header.id)
// 				{
// 				case CustomMsgTypes::ServerAccept:
// 				{
// 					// Server has accepted the connection
// 					std::cout << "Server Accepted Connection\n";
// 				}
// 				break;


// 				case CustomMsgTypes::ServerPing:
// 				{
// 					// Server has responded to a ping request
// 					std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
// 					std::chrono::system_clock::time_point timeThen;
// 					msg >> timeThen;
// 					std::cout << "Ping: " << std::chrono::duration<double>(timeNow - timeThen).count() << "\n";
// 				}
// 				break;

// 				case CustomMsgTypes::ServerMessage:
// 				{
// 					// Server has responded to a message request
// 					uint32_t clientID;
// 					msg >> clientID;
// 					std::cout << "Hello from [" << clientID << "]\n";
// 				}
// 				break;
// 				}
// 			}
// 		}
// 		else
// 		{
// 			std::cout << "Server Down\n";
// 			bQuit = true;
// 		}
// 	}

// 	return 0;
// }