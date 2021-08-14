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
    Match* match { nullptr };
	std::map<int, Player> clients;

public:
	static int count;
	Session()
		:	session_id {count}, state { STATE::EMPTY } {
		std::cout << "Created new session with session_id: " << this->session_id << '\n';
		count++;
	}

	STATE getState();
	int getId() const;
    Player* getPlayerById(int id);
	std::shared_ptr<olc::net::connection<CustomMsgTypes>> getOpponent(int id) const;
	bool addClient(Player player);
	void removeClient(int id);
    void update();
    void updatePlayer(int id, Pos racketPos);
    void informPlayers(CustomServer* server);
};

class SessionHandler {
public:
	// maps client ids and session_id to session objects
	std::map<int, std::shared_ptr<Session>> sessions;

public:
	SessionHandler() = default;

	int addClientToRandomSession(Player player);
	bool addClientToSession(Player player, int id);
	int addClientToNewSession(Player player);
	int addNewSession(Player client1, Player client2);
	// void addNewSession(const Session& newSession);
	int getSessionIdByPlayerId(int id) const;
	void removeClientFromSession(int id);
	bool updateSession(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client, Pos racket, int score);
	std::shared_ptr<olc::net::connection<CustomMsgTypes>> getOpponent(int id) const;
	bool getSessionState(int client_id, STATE* state) const;
};

#endif