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
	EstablishedSession,
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
	double x, y;
} Pos;

typedef struct {
	Pos pos;
	double v;
} Ball;

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
	EMPTY = 0,
    WAITING = 1,
    START = 2,
	UPDATE = 3,
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
		:	session_id {count}, state { STATE::EMPTY }, ball {{0.5, 0.5}, 0.006} {
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

	bool addClient(Player player) {
		if(clients.size() < 2) {
			clients.insert(std::pair<int, Player>(player.GetID(), player));
			this->state = STATE::WAITING;
			if(clients.size() == 2)
				this->state = STATE::START;
			return true;
		}
		return false;
	}

	void removeClient(int id) {
		clients.erase(id);
		if(clients.size() == 1)
			this->state = STATE::WAITING;
		else if(clients.size() == 0)
			this->state = STATE::EMPTY;
	}

	Player* getPlayerById(int id) { 
		try {
			return &clients.at(id);
		} catch (std::exception e) {
			return nullptr;
		}
	}
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
		if(sessions.size() > 0) {
			auto iterator{ sessions.begin() };
			while( iterator != sessions.end() ) {
				if(iterator->second.getState() == STATE::WAITING) {
					if(!this->addClientToSession(player, iterator->second.getId()))
						return -1;
					return iterator->first;
				}
			}
		}
		return this->addClientToNewSession(player);
	}

	bool addClientToSession(Player player, int id) {
		if(!sessions.at(id).addClient(player)) return false;
		sessions.insert(std::make_pair(player.GetID(), sessions.at(id)));
		return true;
	}

	int addClientToNewSession(Player player) {
		std::cout << "Adding client [ " << player.GetID() << " ] to new session...\n";
		Session newSession;
		int id = newSession.getId();
		sessions.insert(std::make_pair(id, newSession));
		if(!this->addClientToSession(player, id))
			return -1;
		return id;
	}

	int addNewSession(Player client1, Player client2) {
		Session newSession;
		newSession.addClient(client1);
		newSession.addClient(client2);
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

	int getSessionIdByPlayerId(int id) {
		auto iterator{ sessions.begin() };
		while( iterator != sessions.end() ) {
			if(iterator->second.getPlayerById(id)) {
				return iterator->second.getId();
			}
		}
		return -1;
	}

	void removeClientFromSession(int id) {
		try {
			this->sessions.at(id).removeClient(id);
			if(this->sessions.at(id).getState() == STATE::EMPTY)
				this->sessions.erase(id);
		} catch (std::exception e) {
			return;
		}
		// int session_id = this->getSessionIdByPlayerId(id);
		// if(session_id < 0)
		// 	return;
		// this->sessions.at(session_id).removeClient(id);
		// if(this->sessions.at(session_id).getState() == STATE::EMPTY)
		// 	this->sessions.erase(session_id);
	}

	bool updateSession(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client, Pos racket, int score) {
		try {
			Player* player = sessions.at(client->GetID()).getPlayerById(client->GetID());
			if(player) {
				player->update(racket, score);
				player = nullptr;
				return true;
			}
			player = nullptr;
		} catch (std::exception e) {
			return false;
		}
		// auto iterator{ sessions.begin() };
		// while( iterator != sessions.end() ) {
		// 	Player* player = iterator->second.getPlayerById(client->GetID());
		// 	if(player) {
		// 		player->update(racket, score);
		// 		player = nullptr;
		// 		return true;
		// 	}
		// 	player = nullptr;
		// }
		// return false;
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

	bool getSessionState(int client_id, STATE* state) {
		try {
			*state = this->sessions.at(client_id).getState();
			return true;
		} catch (std::exception e) {
			return false;
		}
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
			msg >> data_string;
			JSON data = JSON::parse(data_string);
			std::cout << "[" << client->GetID() << "]: Joining Custom Session with id [ " << data["session_id"] << " ]\n";
			std::cout << "[" << client->GetID() << "]: data [ " << data << " ]\n";
			if(session_handler.addClientToSession(Player(client, data["username"]), data["session_id"])) {
				olc::net::message<CustomMsgTypes> response;
				response.header.id = CustomMsgTypes::JoinCustomSession;
				std::shared_ptr<olc::net::connection<CustomMsgTypes>> client2 = session_handler.getOpponent(client->GetID());
				if(client2) {
					response << 0;
					client2->Send(response);
				} else
					response << 1;
				client->Send(response);
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

			int session_id = session_handler.addClientToRandomSession(Player(client, username));
			if(session_id >= 0) {
				std::cout << "Succesfully added player to session...\n";
				olc::net::message<CustomMsgTypes> response;
				response.header.id = CustomMsgTypes::EstablishedSession;
				STATE state { STATE::EMPTY };
				if(session_handler.getSessionState(client->GetID(), &state)) {
					switch (state) {
						case STATE::WAITING :
							response << 1;
							break;
						case STATE::START :
							std::shared_ptr<olc::net::connection<CustomMsgTypes>> client2 (session_handler.getOpponent(client->GetID()));
							if(client2.get()) {
								response << 0;
								client2->Send(response);
							}
							break;
					}
				}
				client->Send(response);
			}
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
			bool done;
			Pos racket;
			int score;
			std::cout << "Received game_info from [" << client->GetID() << "]\n";
			msg >> done >> racket.x >> racket.y >> score;
			olc::net::message<CustomMsgTypes> response;
			if(session_handler.updateSession(client, racket, score)) {
				response.header.id = CustomMsgTypes::GameInfo;
				std::ostringstream oss;
				oss <<
				"{"
					"\"done\": " << done << ","
					"\"racket\": {"
					"\"x\": " << 1 - racket.x << ","
					"\"y\": " << racket.y <<
					"},"
					"\"score\": " << score <<
				"{";
				std::string data = oss.str();
				response << data;
				std::cout << "Sending game_info to opponent\n";
				session_handler.getOpponent(client->GetID())->Send(response);
			}
			else {
				response.header.id = CustomMsgTypes::ErrorMessage;
				response << 21;
			}
			if(done)
				session_handler.removeClientFromSession(client->GetID());
		}
		break;
		// case CustomMsgTypes::GameInfo:
		// {
		// 	char* game_info_string;
		// 	msg >> game_info_string;
		// 	std::cout << "Received game_info from [" << client->GetID() << "]:\n" << game_info_string << '\n';
		// 	JSON game_info = JSON::parse(game_info_string);
		// 	Pos racket { game_info["racket"]["x"], game_info["racket"]["y"] };
		// 	olc::net::message<CustomMsgTypes> response;
		// 	if(session_handler.updateSession(client, racket, game_info["score"])) {
		// 		response.header.id = CustomMsgTypes::GameInfo;
		// 		std::ostringstream oss;
		// 		oss <<
		// 		"{"
		// 			"\"done\": false,"
		// 			"\"racket\": {"
		// 			"\"x\": " << 1 - racket.x << ","
		// 			"\"y\": " << racket.y <<
		// 			"},"
		// 			"\"score\": " << game_info["score"] <<
		// 		"{";
		// 		std::string data = oss.str();
		// 		response << data;
		// 		std::cout << "Sending game_info to opponent:\n" << response << '\n';
		// 		session_handler.getOpponent(client->GetID())->Send(response);
		// 	}
		// 	else {
		// 		response.header.id = CustomMsgTypes::ErrorMessage;
		// 		response << 21;
		// 	}
		// 	if(game_info["done"])
		// 		session_handler.removeClientFromSession(client->GetID());
		// }
		// break;

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