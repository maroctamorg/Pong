#include "server.hpp"

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
	double x, y;
} Pos;

typedef struct {
	Pos pos;
	double v;
} Ball;

typedef struct {
	bool done {false};
	double x {0.5}, y {0.5};
	int score {0};
} Game_Info;

namespace custom_struct_utils {
	std::string toString(Game_Info info) {
		std::ostringstream oss;
		oss << "\ndone: " << (info.done ? "true" : "false") << ",\tscore: " << info.score << "\nracket:\tx: " << info.x << ", y: " << info.y << '\n';
		return oss.str();
	}
}

class Player {
private:
	std::shared_ptr<olc::net::connection<CustomMsgTypes>> client;
	std::string username;
	int score;
	Pos racket;
public:
	Player(std::shared_ptr<olc::net::connection<CustomMsgTypes>> connection, std::string username)
		:	client {connection}, username { username }, score{0}, racket({0.05, 0.475}) {}
	
	std::shared_ptr<olc::net::connection<CustomMsgTypes>> getConnection() const { return client; }

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
		std::cout << "Created new session with session_id: " << this->session_id << '\n';
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
			return &(clients.at(id));
		} catch (std::exception e) {
			return nullptr;
		}
	}
	std::shared_ptr<olc::net::connection<CustomMsgTypes>> getOpponent(int id) {
		auto iterator = clients.cbegin();
		std::shared_ptr<olc::net::connection<CustomMsgTypes>> client;
		while(iterator != clients.cend()) {
			client = iterator->second.getConnection();
			if(client->GetID() != id) {
				return client;
			}
			iterator++;
		}
		return std::shared_ptr<olc::net::connection<CustomMsgTypes>>(nullptr);
	}
};

int Session::count {0};

class SessionHandler {
private:
	// maps client ids and session_id to session objects
	std::map<int, std::shared_ptr<Session>> sessions;

public:
	SessionHandler() = default;

	int addClientToRandomSession(Player player) {
		if(sessions.size() > 0) {
			auto iterator{ sessions.begin() };
			while( iterator != sessions.end() ) {
				if(iterator->second->getState() == STATE::WAITING) {
					if(!this->addClientToSession(player, iterator->second->getId()))
						return -1;
					std::cout << "client [" << player.GetID() << "] added to sesssion [" << iterator->first << "]\n";
					return iterator->first;
				}
				iterator++;
			}
		}
		return this->addClientToNewSession(player);
	}

	bool addClientToSession(Player player, int id) {
		if(!(sessions.at(id)->addClient(player))) return false;
		sessions.insert(std::make_pair(player.GetID(), sessions.at(id)));
		return true;
	}

	int addClientToNewSession(Player player) {
		std::shared_ptr<Session> newSession{ std::make_shared<Session>() };
		int id = newSession->getId();
		sessions.insert(std::make_pair(id, newSession));
		if(!(this->addClientToSession(player, id)))
			return -1;
		std::cout << "Adding client [ " << player.GetID() << " ] to new session " << id << "...\n";
		return id;
	}

	int addNewSession(Player client1, Player client2) {
		std::shared_ptr<Session> newSession{ std::make_shared<Session>() };
		newSession->addClient(client1);
		newSession->addClient(client2);
		int id = newSession->getId();
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
			if(iterator->second->getPlayerById(id)) {
				return iterator->second->getId();
			}
			iterator++;
		}
		return -1;
	}

	void removeClientFromSession(int id) {
		try {
			std::shared_ptr<Session> session = this->sessions.at(id);
			session->removeClient(id);
			this->sessions.erase(id);
			if(session->getState() == STATE::EMPTY)
				this->sessions.erase(session->getId());
		} catch (std::exception e) {
			std::cout << "Failed to remove client from session\n";
			return;
		}
	}

	bool updateSession(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client, Pos racket, int score) {
		try {
			Player* player = sessions.at(client->GetID())->getPlayerById(client->GetID());
			if(player) {
				player->update(racket, score);
				player = nullptr;
				return true;
			}
			player = nullptr;
		} catch (std::exception e) {
			return false;
		}
	}

	std::shared_ptr<olc::net::connection<CustomMsgTypes>> getOpponent(int id) {
		std::cout <<  "Call to getOpponent!\n";
		try {
			return sessions.at(id)->getOpponent(id);
		}
		catch (std::exception e) {
			std::cout << "Failed to retrieve opponent!\n";
			return std::shared_ptr<olc::net::connection<CustomMsgTypes>>{nullptr};
		}
	}

	bool getSessionState(int client_id, STATE* state) {
		try {
			*state = this->sessions.at(client_id)->getState();
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
		session_handler.removeClientFromSession(client->GetID());
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
			std::string username;
			int session_id;
			msg.read<50>(username);
			msg >> session_id;
			std::cout << "[" << client->GetID() << "]: '" << username << "' has requested to join custom session '" << session_id << "'\n";
			if(session_handler.addClientToSession(Player(client, username), session_id)) {
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
								this->MessageClient(client2, response);
							}
							break;
					}
				}
				this->MessageClient(client, response);
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
			Game_Info info;
			msg >> info;
			std::cout << "Received game_info from [" << client->GetID() << "]:" << custom_struct_utils::toString(info);
			olc::net::message<CustomMsgTypes> response;
			// if(session_handler.updateSession(client, {info.x, info.y}, info.score)) {
				std::shared_ptr<olc::net::connection<CustomMsgTypes>> opponent = session_handler.getOpponent(client->GetID());
				if(opponent.get()) {
					response.header.id = CustomMsgTypes::GameInfo;
					info.x = 1 - info.x;
					response << info;
					std::cout << "Sending game_info to opponent: [" << opponent->GetID() << "]" << custom_struct_utils::toString(info);
					this->MessageClient(opponent, response);
				}
			//}
				else {
					response.header.id = CustomMsgTypes::ErrorMessage;
					response << 21;
					this->MessageClient(client, response);
					info.done = true;
				}
			if(info.done)
				session_handler.removeClientFromSession(client->GetID());
		}
		break;
		}
	}
};

int main() {
	CustomServer server(3000); 
	server.Start();

	while (true) {
		server.Update(-1, true);
	}

	return 0;
}