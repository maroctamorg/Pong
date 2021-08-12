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

typedef struct {
	bool done {false};
	double x {0.5}, y {0.5};
	int score {0};
} Game_Info;

namespace custom_struct_utils {
	std::string toString(Game_Info info);
}

class CustomClient : public olc::net::client_interface<CustomMsgTypes> {
public:
	void PingServer();

	void JoinRandomSession(std::string username);

	void CreateCustomSession();

	void JoinCustomSession(std::string session_id);

	void SendGameInfo(Game_Info* game_info);
	
	void LeaveSession();

	STATE handleIncoming(Game_Info* game_info);
};