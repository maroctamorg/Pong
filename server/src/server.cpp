#include "server.hpp"

bool CustomServer::OnClientConnect(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client) {
	olc::net::message<CustomMsgTypes> msg;
	msg.header.id = CustomMsgTypes::ServerAccept;
	client->Send(msg);
	return true;
}

// Called when a client appears to have disconnected
void CustomServer::OnClientDisconnect(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client) {
	std::cout << "Removing client [" << client->GetID() << "]\n";
	session_handler.removeClientFromSession(client->GetID());
}

// Called when a message arrives
void CustomServer::OnMessage(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client, olc::net::message<CustomMsgTypes>& msg) {
	switch (msg.header.id)
	{
	case CustomMsgTypes::ServerPing:
	{
		std::cout << "[" << client->GetID() << "]: Server Ping\n";

		// Simply bounce message back to client
		client->Send(msg);
	}
	break;
	
	case CustomMsgTypes::JoinCustomSession:
	{
		std::string username;
		int session_id;
		msg.read<50>(username);
		msg >> session_id;
		std::cout << "[" << client->GetID() << "]: '" << username << "' has requested to join custom session '" << session_id << "'\n";
		if(session_handler.addClientToSession(std::make_shared<Player>(client, username), session_id)) {
			olc::net::message<CustomMsgTypes> response;
			response.header.id = CustomMsgTypes::JoinCustomSession;
			std::shared_ptr<olc::net::connection<CustomMsgTypes>> client2 = session_handler.getOpponent(client->GetID());
			if(client2.get()) {
				response << 0;
				this->MessageClient(client2, response);
			} else
				response << 1;
			this->MessageClient(client, response);
		}
		else {
			olc::net::message<CustomMsgTypes> msg;
			msg.header.id = CustomMsgTypes::ErrorMessage;
			msg << 11;
			this->MessageClient(client, msg);
		}
	}
	break;
	
	case CustomMsgTypes::JoinRandomSession:
	{
		std::string username;
		msg.read<50>(username);
		std::cout << "[" << client->GetID() << "]: Joining Random Session with username <" << username << ">\n";

		int session_id = session_handler.addClientToRandomSession(std::make_shared<Player>(client, username));
		if(session_id >= 0) {
			std::cout << "Succesfully added player to session...\n";
			olc::net::message<CustomMsgTypes> response;
			response.header.id = CustomMsgTypes::EstablishedSession;
			STATE state { session_handler.id_to_session.at(session_id)->getState() };
			// if(session_handler.getSessionState(client->GetID(), &state)) {
			switch (state) {
				case STATE::WAITING :
					response << 1;
					this->MessageClient(client, response);
					break;
				case STATE::START :
					std::shared_ptr<olc::net::connection<CustomMsgTypes>> client2 (session_handler.getOpponent(client->GetID()));
					if(client2.get()) {
						// response << 0.0 << 0.001 << 0;
						response << Pos({0.001, 0.0}) << 0;
						this->MessageClient(client, response);
						olc::net::message<CustomMsgTypes> response2;
						response2.header.id = CustomMsgTypes::EstablishedSession;
						// response2 << 0.0 << -0.001 << 0;
						response2 << Pos({-0.001, 0.0}) << 0;
						this->MessageClient(client2, response2);
					}
					break;
			}
		}
		else {
			olc::net::message<CustomMsgTypes> msg;
			msg.header.id = CustomMsgTypes::ErrorMessage;
			msg << 12;
			this->MessageClient(client, msg);
		}
	}
	break;

	case CustomMsgTypes::GameInfo:
	{
		olc::net::message<CustomMsgTypes> response;
		try {
			Player_Data data;
			msg >> data;
			if(data.done) {
				//remove session
				return;
			}
			session_handler.id_to_session.at(client->GetID())->updatePlayer(client->GetID(), data.racketPos);
			// std::cout << "Received game_info from [" << client->GetID() << "]:" << custom_struct_utils::toString(info);
		}
		catch (std::exception e) {
			response.header.id = CustomMsgTypes::ErrorMessage;
			response << 21;
			this->MessageClient(client, response);
			//remove session?
		}
	}
	break;
	}
}

bool CustomServer::update() {
	// for(std::shared_ptr<Session> session : session_handler.sessions) {
	for(auto& session : session_handler.sessions) {
		session->update();
		session->informPlayers(this);
	}
	return false;
	
	// auto iterator { session_handler.sessions.begin() };
	// while(iterator != session_handler.sessions.end() ) {
	// 	iterator->second->update();
	// 	iterator->second->informPlayers(this);
	// }
	// return false;
}

int main() {
	CustomServer server(3000); 
	server.Start();

	std::thread game_thr {
		[&server]() {
			bool done { false };
			Timer loop;
			while(!done) {
				done = server.update();
				std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1000*(1.0/60 - loop.elapsed()))));
				loop.reset();
			}
		}
	};

	while (true) {
		server.Update(-1, true);
	}

	if(game_thr.joinable()) game_thr.join();

	return 0;
}