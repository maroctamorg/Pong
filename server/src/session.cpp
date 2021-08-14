#include "session.hpp"

STATE Session::getState() { return state; }

// int getClientIdByIndex (int index) const {
// 	auto iterator{ clients.cbegin() };
// 	while( iterator != clients.cend() ) {
// 		if(iterator->second == index) return iterator->first;
// 	}
// 	return -1;
// }


int Session::count {0};
int Session::getId() const { return session_id; }
bool Session::addClient(Player player) {
	if(clients.size() < 2) {
		clients.insert(std::pair<int, Player>(player.GetID(), player));
		this->state = STATE::WAITING;
		if(clients.size() == 2) {
			this->state = STATE::START;
			this->match = new Match(&clients.at(player.GetID()), this->getPlayerById(this->getOpponent(player.GetID())->GetID()));
		}
		return true;
	}
	return false;
}
void Session::removeClient(int id) {
	clients.erase(id);
	if(clients.size() == 1)
		this->state = STATE::WAITING;
	else if(clients.size() == 0)
		this->state = STATE::EMPTY;
}
Player* Session::getPlayerById(int id) { 
	try {
		return &(clients.at(id));
	} catch (std::exception e) {
		return nullptr;
	}
}
std::shared_ptr<olc::net::connection<CustomMsgTypes>> Session::getOpponent(int id) const {
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
void Session::updatePlayer(int id, Pos racketPos) {
	match->updateRacket(id, racketPos);
}

void Session::update() {
	if(match)
		match->update();
}

void Session::informPlayers(CustomServer* server) {
    if(match)
		match->send(server);
}

int SessionHandler::addClientToRandomSession(Player player) {
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

bool SessionHandler::addClientToSession(Player player, int id) {
	if(!(sessions.at(id)->addClient(player))) return false;
	sessions.insert(std::make_pair(player.GetID(), sessions.at(id)));
	return true;
}

int SessionHandler::addClientToNewSession(Player player) {
	std::shared_ptr<Session> newSession{ std::make_shared<Session>() };
	int id = newSession->getId();
	sessions.insert(std::make_pair(id, newSession));
	if(!(this->addClientToSession(player, id)))
		return -1;
	std::cout << "Adding client [ " << player.GetID() << " ] to new session " << id << "...\n";
	return id;
}

int SessionHandler::addNewSession(Player client1, Player client2) {
	std::shared_ptr<Session> newSession{ std::make_shared<Session>() };
	newSession->addClient(client1);
	newSession->addClient(client2);
	int id = newSession->getId();
	sessions.insert(std::make_pair(id, newSession));
	sessions.insert(std::make_pair(client1.GetID(), newSession));
	sessions.insert(std::make_pair(client2.GetID(), newSession));
	return id;
}

// void SessionHandler::addNewSession(const Session& newSession) {
// 	sessions.insert(std::make_pair(newSession.getId(), newSession));
// 	auto iterator = sessions.cbegin();
// 	while(iterator != sessions.cend()) {
// 		sessions.insert(std::make_pair(iterator->second.getId))
// 	}
// 	// sessions.insert(std::make_pair(newSession.getClientIdByIndex(0), newSession));
// 	// sessions.insert(std::make_pair(newSession.getClientIdByIndex(1), newSession));
// }

int SessionHandler::getSessionIdByPlayerId(int id) const {
	auto iterator{ sessions.begin() };
	while( iterator != sessions.end() ) {
		if(iterator->second->getPlayerById(id)) {
			return iterator->second->getId();
		}
		iterator++;
	}
	return -1;
}

void SessionHandler::removeClientFromSession(int id) {
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

bool SessionHandler::updateSession(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client, Pos racket, int score) {
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

std::shared_ptr<olc::net::connection<CustomMsgTypes>> SessionHandler::getOpponent(int id) const {
	// std::cout <<  "Call to getOpponent!\n";
	try {
		return sessions.at(id)->getOpponent(id);
	}
	catch (std::exception e) {
		std::cout << "Failed to retrieve opponent!\n";
		return std::shared_ptr<olc::net::connection<CustomMsgTypes>>{nullptr};
	}
}

bool SessionHandler::getSessionState(int client_id, STATE* state) const {
	try {
		*state = this->sessions.at(client_id)->getState();
		return true;
	} catch (std::exception e) {
		return false;
	}
}