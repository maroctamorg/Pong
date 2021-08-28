#include "server.hpp"
#include "match.hpp"

std::string custom_struct_utils::toString(Game_Info info) {
    std::ostringstream oss;
    oss << "\ndone: " << (info.done ? "true" : "false") << ",\tscore1: " << info.score1 << ",\tscore2: " << info.score2 << "\nracket:\tx: " << info.racketPos.x << ", y: " << info.racketPos.y << '\n';
    return oss.str();
}

std::shared_ptr<olc::net::connection<CustomMsgTypes>> Player::getConnection() const { return client; }

int Player::GetID() const { return client->GetID(); }

bool check4EdgeCollision(Rect rect1, Rect rect2) {
    return  ( (rect1.pos.x >= rect2.pos.x && rect1.pos.x <= rect2.pos.x + rect2.w) && (rect1.pos.y >= rect2.pos.y && rect1.pos.y <= rect2.pos.y + rect2.h) ) // edge 1
        ||  ( (rect1.pos.x + rect1.w >= rect2.pos.x && rect1.pos.x + rect1.w <= rect2.pos.x + rect2.w) && (rect1.pos.y >= rect2.pos.y && rect1.pos.y <= rect2.pos.y + rect2.h) ) // edge 2
        ||  ( (rect1.pos.x + rect1.w >= rect2.pos.x && rect1.pos.x + rect1.w <= rect2.pos.x + rect2.w) && (rect1.pos.y + rect1.h >= rect2.pos.y && rect1.pos.y + rect1.h<= rect2.pos.y + rect2.h) )// edge 3
        ||  ( (rect1.pos.x >= rect2.pos.x && rect1.pos.x <= rect2.pos.x + rect2.w) && (rect1.pos.y + rect1.h >= rect2.pos.y && rect1.pos.y + rect1.h<= rect2.pos.y + rect2.h) ); // edge 4
}

bool checkRelCollision(Rect rect1, Rect rect2) {
    return check4EdgeCollision(rect1, rect2) || check4EdgeCollision(rect2, rect1);
}

void Match::checkCollision() {
    // CHECK FOR GOALS
    if(checkRelCollision(ball.rect, goals[0])) {
        score[1] += 1;
        ball.rect = b_inRect;
        ball.vel = b_inVel;
    }
    else if (checkRelCollision(ball.rect, goals[1])) {
        score[0] += 1;
        ball.rect = b_inRect;
        ball.vel = {-b_inVel.x, -b_inVel.y};
    }

    // CHECK FOR BOUNCE ON EDGES
    if (ball.rect.pos.x >= 1) {
        ball.rect.pos.x = 1;
        ball.vel.x = -ball.vel.x;
    } else if (ball.rect.pos.x <= 0) {
        ball.rect.pos.x = 0;
        ball.vel.x = -ball.vel.x;
    }

    if (ball.rect.pos.y >= 1) {
        ball.rect.pos.y = 1;
        ball.vel.y = -ball.vel.y;
    } else if (ball.rect.pos.y <= 0) {
        ball.rect.pos.y = 0;
        ball.vel.y = -ball.vel.y;
    }

    // CHECK FOR PADDLE COLLISIONS
    if (checkRelCollision(ball.rect, rackets[0])) {
        if(ball.rect.pos.x <= rackets[0].pos.x + static_cast<int>(rackets[0].w/2)) {
            ball.rect.pos.x = rackets[0].pos.x - ball.rect.w;
            ball.vel.x = ball.vel.x >= 0 ? -ball.vel.x : ball.vel.x;
        } else {
            ball.rect.pos.x = rackets[0].pos.x + rackets[0].w;
            ball.vel.x = ball.vel.x >= 0 ? ball.vel.x : -ball.vel.x;
        }
        if (rackets[0].pos.y <= 0.2) {
            ball.vel.y = ball.vel.y >= 0 ? (ball.vel.y == 0 ? this->b_inVel.x : ball.vel.y) : -ball.vel.y;
        } else if (rackets[0].pos.y >= 0.8) {
            ball.vel.y = ball.vel.y >= 0 ? (ball.vel.y == 0 ? -this->b_inVel.x : -ball.vel.y) : ball.vel.y;
        }
    }
    else if (checkRelCollision(ball.rect, rackets[1])) {
        if(ball.rect.pos.x <= rackets[1].pos.x + static_cast<int>(rackets[1].w/2)) {
            ball.rect.pos.x = rackets[1].pos.x - ball.rect.w;
            ball.vel.x = ball.vel.x >= 0 ? -ball.vel.x : ball.vel.x;
        } else {
            ball.rect.pos.x = rackets[1].pos.x + rackets[1].w;
            ball.vel.x = ball.vel.x >= 0 ? ball.vel.x : -ball.vel.x;
        }
        if (rackets[1].pos.y <= 0.2) {
            ball.vel.y = ball.vel.y >= 0 ? (ball.vel.y == 0 ? this->b_inVel.x : ball.vel.y) : -ball.vel.y;
        } else if (rackets[1].pos.y >= 0.8) {
            ball.vel.y = ball.vel.y >= 0 ? (ball.vel.y == 0 ? -this->b_inVel.x : -ball.vel.y) : ball.vel.y;
        }
    }
}

void Match::updateRacket(int id, Pos racketPos) {
    try {
        int index = id_to_index.at(id);
        rackets[index] = {( index == 0 ? racketPos : Pos({ 1.0 - racketPos.x, racketPos.y }) ), rackets[index].w, rackets[index].h};
    } catch (std::exception e) {
        std::cout << "Unable to find index corresponding to user id passed in as an argument!\n";
        return;
    }
}

void Match::update() {
    ball.rect.pos.x += ball.vel.x;
    ball.rect.pos.y += ball.vel.y;

    checkCollision();
}

void Match::send(CustomServer* server) {
    olc::net::message<CustomMsgTypes> message1;
    message1.header.id = CustomMsgTypes::GameInfo;
    Game_Info game_info {done, score[0], score[1], rackets[1].pos, ball.rect.pos, ball.vel};
    message1 << game_info;
    server->MessageClient(players[0]->getConnection(), message1);

    olc::net::message<CustomMsgTypes> message2;
    message2.header.id = CustomMsgTypes::GameInfo;
    game_info = {done, score[1], score[0], Pos({1.0 - rackets[0].pos.x, rackets[0].pos.y}), ball.rect.pos, {-ball.vel.x, ball.vel.y}};
    message2 << game_info;
    server->MessageClient(players[1]->getConnection(), message2);
}