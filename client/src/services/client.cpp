#include "client.hpp"

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

// template<typename Server_Info>
// void CustomClient::SendGameInfo(Server_Info* server_info) {
// 	if(!server_info) return;
// 	olc::net::message<CustomMsgTypes> msg;
// 	msg.header.id = CustomMsgTypes::GameInfo;
// 	msg << *server_info;
// 	// std::cout << "Sending server_info data:" << custom_struct_utils::toString(*server_info);
// 	Send(msg);
// }

void CustomClient::LeaveSession() {
	olc::net::message<CustomMsgTypes> msg;
	msg.header.id = CustomMsgTypes::LeaveSession;
	Send(msg);
}

// template <typename Server_Info>
// STATE CustomClient::handleIncoming(Server_Info* server_info) {
// 	if (this->olc::net::client_interface<CustomMsgTypes>::IsConnected()) {
// 		if (!this->olc::net::client_interface<CustomMsgTypes>::Incoming().empty()) {
// 			auto msg = this->olc::net::client_interface<CustomMsgTypes>::Incoming().pop_front().msg;

// 			switch (msg.header.id) {
// 			case CustomMsgTypes::ServerPing: {
// 				// Server has responded to a ping request
// 				std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
// 				std::chrono::system_clock::time_point timeThen;
// 				msg >> timeThen;
// 				std::cout << "Ping: " << std::chrono::duration<double>(timeNow - timeThen).count() << "\n";
// 			}
// 			break;

// 			case CustomMsgTypes::ServerAccept:
// 			{
// 				// Server has accepted the connection
// 				std::cout << "Server Accepted Connection\n";
// 			}
// 			break;

// 			// case CustomMsgTypes::JoinRandomSession: {
// 			// 	// HANDLE RANDOM SESSION
// 			// }
// 			// break;

// 			// case CustomMsgTypes::CreateCustomSession: {
// 			// 	// HANDLE CREATE CUSTOM SESSION
// 			// }
// 			// break;
			
// 			// case CustomMsgTypes::JoinCustomSession: {
// 			// 	// HANDLE JOIN CUSTOM SESSION
// 			// }
// 			// break;

// 			case CustomMsgTypes::EstablishedSession: {
// 				int flag { -1 };
// 				msg >> flag;
// 				if(flag == 0) {
// 					if(!server_info)
// 						return STATE::END;
// 					msg >> *server_info;
// 					return STATE::START;
// 				}
// 				else if (flag == 1)
// 					return STATE::WAITING;
// 				else {
// 					return STATE::END;
// 					std::cout << "Unexpected return type!\n";
// 				}
// 			}
// 			break;
			
// 			case CustomMsgTypes::GameInfo: {
// 				// HANDLE GAME INFO
// 				if(!server_info) return STATE::END;
// 				msg >> *server_info;
// 				return STATE::GAME_INFO;
// 			}
// 			break;
			
// 			case CustomMsgTypes::ErrorMessage: {
// 				std::cout << "Received error message from server! Disconnecting...\n";
// 				return STATE::END;
// 			}
// 			break;

// 			case CustomMsgTypes::LeaveSession: {
// 				// HANDLE LEAVE SESSION
// 			}
// 			break;
// 			}
// 		}
// 		return STATE::QUIET;
// 	}
// 	else {
// 		std::cout << "The connection to the server was closed!\n";
// 		return STATE::END;
// 	}
// }