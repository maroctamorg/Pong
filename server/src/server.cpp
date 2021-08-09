#include <iostream>
#include "../dependencies/net_framework/olc_net.h"
#include "../dependencies/nlohmann/json.hpp"

using JSON = nlohmann::json;

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
// enum class CustomMsgTypes : uint32_t
// {
// 	ServerAccept,
// 	ServerDeny,
// 	ServerPing,
// 	MessageAll,
// 	ServerMessage,
// };

typedef struct {
	Pos pos;
	double v;
} Ball;

typedef struct {
	double x, y;
} Pos;

class Player {
private:
	std::shared_ptr<olc::net::connection<CustomMsgTypes>> client;
	std::string username;
	int score;
	Pos racket;
public:
	Player(std::shared_ptr<olc::net::connection<CustomMsgTypes>> connection, std::string username)
		:	client {connection}, username { username }, score{0}, racket({0.05, 0.475}) {}
	
	std::shared_ptr<olc::net::connection<CustomMsgTypes>> getConnection() { return client; }

	void update(Pos racket, int score) {
		this->racket = racket;
		this->score = score;
	}

	int GetID() const { return client->GetID(); }
};


enum class STATE {
    CLOSED = -1,
    WAITING = 0,
    START = 1,
	UPDATE = 2,
};

class Session {
private:
	int session_id;
	STATE state;
	Ball ball;
	std::map<int, Player> clients;

public:
	static int count;
	Session()
		:	session_id {count}, state { STATE::WAITING }, ball {{0.5, 0.5}, 0.006}, clients {std::pair(-1, nullptr), std::pair(-2, nullptr)} {
		count++;
	}

	STATE getState() { return state; }

	// int getClientIdByIndex (int index) const {
	// 	auto iterator{ clients.cbegin() };
	// 	while( iterator != clients.cend() ) {
	// 		if(iterator->second == index) return iterator->first;
	// 	}
	// 	return -1;
	// }

	int getId() const { return session_id; }

	bool addClientToSession(Player player) {
		for(int i{0}; i < 2; i++) {
			if(!clients.at(i).getConnection()) {
				clients.at(i) = player;
				if(i == 2) state = STATE::START;
				return true;
			}
		}
		return false;
	}

	Player* getPlayerById(int id) { return clients.at(id); }
	std::shared_ptr<olc::net::connection<CustomMsgTypes>> getOpponent(int id) {
		auto iterator = clients.begin();
		std::shared_ptr<olc::net::connection<CustomMsgTypes>> client;
		while(iterator != clients.end()) {
			client = iterator->second.getConnection();
			if(client) {
				if(client->GetID() != id) return client;
			}
		}
		return nullptr;
	}
};

int Session::count {0};

class SessionHandler {
private:
	// maps client ids and session_id to session objects
	std::map<int, Session> sessions;

public:
	SessionHandler() = default;

	int addClientToRandomSession(Player player) {
		auto iterator{ sessions.begin() };
		while( iterator != sessions.end() ) {
			if(iterator->second.getState() == STATE::WAITING) {
				iterator->second.addClientToSession(player);
				return iterator->first;
			}
		}
		addClientToNewSession(player);
	}

	bool addClientToSession(Player player, int id) {
		return sessions.at(id).addClientToSession(player);
	}

	int addClientToNewSession(Player player) {
		Session newSession;
		newSession.addClientToSession(player);
		int id = newSession.getId();
		sessions.insert(std::make_pair(id, newSession));
		sessions.insert(std::make_pair(player.GetID(), newSession));
		return id;
	}

	int addNewSession(Player client1, Player client2) {
		Session newSession;
		newSession.addClientToSession(client1);
		newSession.addClientToSession(client2);
		int id = newSession.getId();
		sessions.insert(std::make_pair(id, newSession));
		sessions.insert(std::make_pair(client1.GetID(), newSession));
		sessions.insert(std::make_pair(client2.GetID(), newSession));
		return id;
	}

	// void addNewSession(const Session& newSession) {
	// 	sessions.insert(std::make_pair(newSession.getId(), newSession));
	// 	auto iterator = sessions.cbegin();
	// 	while(iterator != sessions.cend()) {
	// 		sessions.insert(std::make_pair(iterator->second.getId))
	// 	}
	// 	// sessions.insert(std::make_pair(newSession.getClientIdByIndex(0), newSession));
	// 	// sessions.insert(std::make_pair(newSession.getClientIdByIndex(1), newSession));
	// }

	bool updateSession(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client, Pos racket, int score) {
		auto iterator{ sessions.begin() };
		while( iterator != sessions.end() ) {
			Player* player = iterator->second.getPlayerById(client->GetID());
			if(player) {
				player->update(racket, score);
				player = nullptr;
				return true;
			}
		}
		return false;
	}

	std::shared_ptr<olc::net::connection<CustomMsgTypes>> getOpponent(int id) {
		auto iterator{ sessions.begin() };
		while( iterator != sessions.end() ) {
			if(iterator->second.getPlayerById(id)) {
				return iterator->second.getOpponent(id);
			}
		}
		return nullptr;
	}
};

class CustomServer : public olc::net::server_interface<CustomMsgTypes> {
private:
	SessionHandler session_handler;

public:
	CustomServer(uint16_t nPort) : olc::net::server_interface<CustomMsgTypes>(nPort) {

	}

protected:
	virtual bool OnClientConnect(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client) {
		olc::net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::ServerAccept;
		client->Send(msg);
		return true;
	}

	// Called when a client appears to have disconnected
	virtual void OnClientDisconnect(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client) {
		std::cout << "Removing client [" << client->GetID() << "]\n";
	}

	// Called when a message arrives
	virtual void OnMessage(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client, olc::net::message<CustomMsgTypes>& msg) {
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
			std::string data_string;
			JSON data;
			msg >> data_string >> data;
			std::cout << "[" << client->GetID() << "]: Joining Custom Session with id [ " << data["session_id"] << " ]\n";
			if(session_handler.addClientToSession(Player(client, data["username"]), data["session_id"])) {
				olc::net::message<CustomMsgTypes> response;
				response.header.id = CustomMsgTypes::JoinCustomSession;
				response << 0;
				std::shared_ptr<olc::net::connection<CustomMsgTypes>> client2 = session_handler.getOpponent(client->GetID());
				client->Send(response);
				if(client2)
					client2->Send(response);
			}
			else {
				olc::net::message<CustomMsgTypes> msg;
				msg.header.id = CustomMsgTypes::ErrorMessage;
				msg << 11;
				client->Send(msg);
			}
		}
		break;
		
		case CustomMsgTypes::JoinRandomSession:
		{
			std::string username;
			msg >> username;
			std::cout << "[" << client->GetID() << "]: Joining Random Session with username <" << username << ">\n";

			if(session_handler.addClientToRandomSession(Player(client, username)) >= 0)
				olc::net::message<CustomMsgTypes> response;
				response.header.id = CustomMsgTypes::JoinCustomSession;
				response << 0;
				std::shared_ptr<olc::net::connection<CustomMsgTypes>> client2 = session_handler.getOpponent(client->GetID());
				client->Send(response);
				if(client2)
					client2->Send(response);
			else {
				olc::net::message<CustomMsgTypes> msg;
				msg.header.id = CustomMsgTypes::ErrorMessage;
				msg << 12;
				client->Send(msg);
			}
		}
		break;

		case CustomMsgTypes::GameInfo:
		{
			std::string game_info_string;
			JSON game_info;
			msg >> game_info_string >> game_info;
			Pos racket { game_info["racket"]["x"], game_info["racket"]["y"] };
			olc::net::message<CustomMsgTypes> response;
			if(game_info["done"]) {
				// handle end of match
			}
			else if(session_handler.updateSession(client, racket, game_info["score"])) {
				response.header.id = CustomMsgTypes::GameInfo;
				std::ostringstream oss;
				oss <<
				"{"
					"'done': false,"
					"'racket': {"
					"'x': " << 1 - racket.x << ","
					"'y': " << racket.y <<
					"},"
					"'score': " << game_info["score"] <<
				"{";
				std::string data = oss.str();
				response << data;
				session_handler.getOpponent(client->GetID())->Send(response);
			}
			else {
				response.header.id = CustomMsgTypes::ErrorMessage;
				response << 21;
			}
		}
		break;

		// case CustomMsgTypes::MessageAll:
		// {
		// 	std::cout << "[" << client->GetID() << "]: Message All\n";

		// 	// Construct a new message and send it to all clients
		// 	olc::net::message<CustomMsgTypes> msg;
		// 	msg.header.id = CustomMsgTypes::ServerMessage;
		// 	msg << client->GetID();
		// 	MessageAllClients(msg, client);

		// }
		// break;
		}
	}
};

int main() {
	CustomServer server(3000); 
	server.Start();

	while (1) {
		server.Update(-1, true);
	}
	


	return 0;
}