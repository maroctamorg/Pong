#pragma once

#include <iostream>
#include <chrono>
// #include <SDL2/SDL.h>
#include "../../dependencies/net_framework/olc_net.h"

enum class CustomMsgTypes : uint32_t
{
	ServerPing,
	ServerAccept,
	JoinRandomSession,
	CreateCustomSession,
	JoinCustomSession,
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

	int handleIncoming(JSON* serverData);
};