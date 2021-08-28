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
bool Session::addClient(std::shared_ptr<Player> player) {
	if(clients.size() < 2) {
		clients.insert(std::pair<int, std::shared_ptr<Player>>(player->GetID(), player));
		this->state = STATE::WAITING;
		if(clients.size() == 2) {
			this->state = STATE::START;
			this->match = new Match(clients.at(player->GetID()), clients.at(this->getOpponent(player->GetID())->GetID()));
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
std::shared_ptr<Player> Session::getPlayerById(int id) { 
	try {
		return clients.at(id);
	} catch (std::exception e) {
		return std::shared_ptr<Player>{nullptr};
	}
}
std::shared_ptr<olc::net::connection<CustomMsgTypes>> Session::getOpponent(int id) const {
	auto iterator = clients.cbegin();
	std::shared_ptr<olc::net::connection<CustomMsgTypes>> client;
	while(iterator != clients.cend()) {
		client = iterator->second->getConnection();
		if(client->GetID() != id) {
			return client;
		}
		iterator++;
	}
	return std::shared_ptr<olc::net::connection<CustomMsgTypes>>(nullptr);
}
void Session::updatePlayer(int id, Pos racketPos) {
	if(match)
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

int SessionHandler::addClientToRandomSession(std::shared_ptr<Player> player) {
	// for(std::shared_ptr<Session> session : sessions) {
	for(auto session : sessions) {
		if(session->getState() == STATE::WAITING) {
			int id { session->getId() };
			if(!this->addClientToSession(player, id))
				return -1;
			std::cout << "client [" << player->GetID() << "] added to sesssion [" << id << "]\n";
			return id;
		}
		// session = nullptr;
	}
	// if(sessions.size() > 0) {
	// 	auto iterator{ sessions.begin() };
	// 	while( iterator != sessions.end() ) {
	// 		if(iterator->second->getState() == STATE::WAITING) {
	// 			if(!this->addClientToSession(player, iterator->second->getId()))
	// 				return -1;
	// 			std::cout << "client [" << player.GetID() << "] added to sesssion [" << iterator->first << "]\n";
	// 			return iterator->first;
	// 		}
	// 		iterator++;
	// 	}
	// }
	return this->addClientToNewSession(player);
}

bool SessionHandler::addClientToSession(std::shared_ptr<Player> player, int id) {
	try {
		if(!(id_to_session.at(id)->addClient(player))) return false;
		id_to_session.insert(std::make_pair(player->GetID(), id_to_session.at(id)));
		return true;
	} catch (std::exception e) {
		return false;
	}
}

int SessionHandler::addClientToNewSession(std::shared_ptr<Player> player) {
	// std::shared_ptr<Session> newSession{ std::make_shared<Session>() };
	std::shared_ptr<Session> newSession{ std::make_shared<Session>() };
	int id = newSession->getId();
	sessions.push_back(newSession);
	id_to_session.insert(std::make_pair(id, newSession));
	if(!(this->addClientToSession(player, id)))
		return -1;
	std::cout << "Adding client [ " << player->GetID() << " ] to new session " << id << "...\n";
	return id;
}

int SessionHandler::addNewSession(std::shared_ptr<Player> client1, std::shared_ptr<Player> client2) {
	// std::shared_ptr<Session> newSession{ std::make_shared<Session>() };
	std::shared_ptr<Session> newSession{ std::make_shared<Session>() };
	newSession->addClient(client1);
	newSession->addClient(client2);
	int id = newSession->getId();
	sessions.push_back(newSession);
	id_to_session.insert(std::make_pair(id, newSession));
	id_to_session.insert(std::make_pair(client1->GetID(), newSession));
	id_to_session.insert(std::make_pair(client2->GetID(), newSession));
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
	try {
		return id_to_session.at(id)->getId();
	} catch (std::exception e) {
		return -1;
	}
	// auto iterator{ sessions.begin() };
	// while( iterator != sessions.end() ) {
	// 	if(iterator->second->getPlayerById(id)) {
	// 		return iterator->second->getId();
	// 	}
	// 	iterator++;
	// }
	// return -1;
}

void SessionHandler::removeClientFromSession(int id) {
	try {
		// std::shared_ptr<Session> session = this->id_to_session.at(id);
		std::shared_ptr<Session> session = this->id_to_session.at(id);
		session->removeClient(id);
		this->id_to_session.erase(id);
		if(session->getState() == STATE::EMPTY)
			this->id_to_session.erase(session->getId());
			// delete session from vector	!!!!
	} catch (std::exception e) {
		std::cout << "Failed to remove client from session\n";
		return;
	}
}

// bool SessionHandler::updateSession(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client, Pos racket, int score) {
// 	try {
// 		Player* player = sessions.at(client->GetID())->getPlayerById(client->GetID());
// 		if(player) {
// 			player->update(racket, score);
// 			player = nullptr;
// 			return true;
// 		}
// 		player = nullptr;
// 	} catch (std::exception e) {
// 		return false;
// 	}
// }

std::shared_ptr<olc::net::connection<CustomMsgTypes>> SessionHandler::getOpponent(int id) const {
	// std::cout <<  "Call to getOpponent!\n";
	try {
		return id_to_session.at(id)->getOpponent(id);
	}
	catch (std::exception e) {
		std::cout << "Failed to retrieve opponent!\n";
		return std::shared_ptr<olc::net::connection<CustomMsgTypes>>{nullptr};
	}
}

bool SessionHandler::getSessionState(int client_id, STATE* state) const {
	try {
		*state = this->id_to_session.at(client_id)->getState();
		return true;
	} catch (std::exception e) {
		return false;
	}
}