#ifndef EXTERN_HPP
#define EXTERN_HPP

#include <iostream>
#include <map>
#include <vector>
#include <memory>
#include <string>
#include <chrono>
#include "../dependencies/net_framework/olc_net.h"
#include "utilities/timer.hpp"

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
	double x;
	double y;
} Pos;

typedef struct {
	bool done {false};
	Pos racketPos {0.5, 0.5};
} Player_Data;

#endif