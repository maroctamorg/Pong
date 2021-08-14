#pragma once

#include <iostream>
#include <chrono>
// #include <SDL2/SDL.h>
#include "../../dependencies/net_framework/olc_net.h"
#include "../../dependencies/nlohmann/json.hpp"

using JSON = nlohmann::json;

enum class STATE {
    END = -1,
    ESTABLISHING = 0,
	WAITING = 1,
    START = 2,
    GAME_INFO = 3,
    QUIET = 4,
};

enum class CustomMsgTypes : uint32_t
{
	ServerPing,
	ServerAccept,
	JoinRandomSession,
	CreateCustomSession,
	JoinCustomSession,
	EstablishedSession,
	GameInfo,
	LeaveSession,
	ErrorMessage,
};

class CustomClient : public olc::net::client_interface<CustomMsgTypes> {
public:
	void PingServer();

	void JoinRandomSession(std::string username);

	void CreateCustomSession();

	void JoinCustomSession(std::string session_id);

	template <typename Local_Info>
	void SendGameInfo(Local_Info* server_info) {
		if(!server_info) return;
		olc::net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::GameInfo;
		msg << *server_info;
		// std::cout << "Sending server_info data:" << custom_struct_utils::toString(*server_info);
		Send(msg);
	}
	
	void LeaveSession();

	template <typename Server_Info>
	STATE handleIncoming(Server_Info* server_info) {
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
					if(flag == 0) {
						if(!server_info)
							return STATE::END;
						// msg >> server_info->x >> server_info->y;
						msg >> *server_info;
						return STATE::START;
					}
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
					if(!server_info) return STATE::END;
					msg >> *server_info;
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
};