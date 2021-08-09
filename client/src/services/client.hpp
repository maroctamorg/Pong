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

	void SendGameInfo(JSON* local_data);
	
	void LeaveSession();

	STATE handleIncoming(JSON* serverData);
};