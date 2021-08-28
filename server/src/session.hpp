#ifndef SESSION_HPP
#define SESSION_HPP

#include "extern.hpp"
#include "match.hpp"

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
	std::map<int, std::shared_ptr<Player>> clients;
    Match* match { nullptr };

public:
	static int count;
	Session()
		:	session_id {count}, state { STATE::EMPTY } {
		std::cout << "Created new session with session_id: " << this->session_id << '\n';
		count++;
	}

	STATE getState();
	int getId() const;
    std::shared_ptr<Player> getPlayerById(int id);
	std::shared_ptr<olc::net::connection<CustomMsgTypes>> getOpponent(int id) const;
	bool addClient(std::shared_ptr<Player> player);
	void removeClient(int id);
    void update();
    void updatePlayer(int id, Pos racketPos);
    void informPlayers(CustomServer* server);
};

class SessionHandler {
public:
	std::vector<std::shared_ptr<Session>> sessions;
	// maps client ids and session_id to session objects
	std::map<int, std::shared_ptr<Session>> id_to_session;

public:
	SessionHandler() = default;
	~SessionHandler() {
		auto iterator { id_to_session.begin() };
		while(iterator != id_to_session.end()) {
			iterator->second = nullptr;
			iterator++;
		}
	}

	int addClientToRandomSession(std::shared_ptr<Player> player);
	bool addClientToSession(std::shared_ptr<Player> player, int id);
	int addClientToNewSession(std::shared_ptr<Player> player);
	int addNewSession(std::shared_ptr<Player> client1, std::shared_ptr<Player> client2);
	// void addNewSession(const Session& newSession);
	int getSessionIdByPlayerId(int id) const;
	void removeClientFromSession(int id);
	// bool updateSession(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client, Pos racket, int score);
	std::shared_ptr<olc::net::connection<CustomMsgTypes>> getOpponent(int id) const;
	bool getSessionState(int client_id, STATE* state) const;
};

#endif