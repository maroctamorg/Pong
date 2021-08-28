#ifndef MATCH_HPP
#define MATCH_HPP

#include "extern.hpp"

typedef struct {
    Pos pos;
    double w, h;
} Rect;

typedef struct {
	Rect rect;
	Pos vel;
} Ball;

typedef struct {
	bool done {false};
	int score1 {0}, score2 {0};
	Pos racketPos {0.5, 0.5};
    Pos ballPos {0.4875, 0.4875};
    Pos ballVel {0.025, 0.025};
} Game_Info;

namespace custom_struct_utils {
	std::string toString(Game_Info info);
}

class Player {
private:
	std::shared_ptr<olc::net::connection<CustomMsgTypes>> client;
	std::string username;
public:
	Player(std::shared_ptr<olc::net::connection<CustomMsgTypes>> connection, std::string username)
		:	client {connection}, username { username } {}
	
	std::shared_ptr<olc::net::connection<CustomMsgTypes>> getConnection() const;
	int GetID() const;
};

class CustomServer;

class Match {
public:
    const Rect b_inRect { 0.4875, 0.4875, 0.025, 0.025 };
    const Pos b_inVel { 0.001, 0 };
private:
    bool done { false };
    Ball ball;
    // std::map<int, int> score;
    // std::map<int, Rect> goals;
    // std::map<int, Rect> rackets;
    std::map<int, int> id_to_index;
    int score[2];
    Rect goals[2];
    Rect rackets[2];
    std::shared_ptr<Player> players[2];
public:
    Match(std::shared_ptr<Player> player1, std::shared_ptr<Player> player2)
        :   id_to_index {   std::pair(player1->GetID(), 0), std::pair(player2->GetID(), 1)  },
            ball        {   b_inRect, b_inVel   },
            score       {   0, 0    },
            goals       {   { 0, 0.2, 0.01, 0.6 }, { 0.99, 0.2, 0.01, 0.6 }     },
            rackets     {   { 0.2, 0.1, 0.01, 0.05 }, { 0.8, 0.9, 0.01, 0.05 }  },
            players     {   player1, player2    }
        {}

    ~Match() {
        players[0] = nullptr;
        players[1] = nullptr;
    }
    
private:
    void checkCollision();

public:
    void updateRacket(int id, Pos racketPos);
    void update();
    void send(CustomServer* server);
};

#endif