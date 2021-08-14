#ifndef SERVER_HPP
#define SERVER_HPP

#include "extern.hpp"
#include "session.hpp"

class CustomServer : public olc::net::server_interface<CustomMsgTypes> {
private:
	SessionHandler session_handler;

public:
	CustomServer(uint16_t nPort) : olc::net::server_interface<CustomMsgTypes>(nPort) {}
    bool update();

protected:
	virtual bool OnClientConnect(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client) override;
	// Called when a client appears to have disconnected
	virtual void OnClientDisconnect(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client) override;
	// Called when a message arrives
	virtual void OnMessage(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client, olc::net::message<CustomMsgTypes>& msg) override;
};

#endif