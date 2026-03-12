//
// Created by loren on 12/03/2026.
//

#include "Player.h"

Player::Player(int X, int Y, char S, int LIFE) : Entity(X, Y, S) {
    life = LIFE;
}

int Player::getlife() {
    return life;
}

void Player::take_damage() {
    life = life - 1;
}

void Player::bomb_placement() {

}

void Player::collect_item() {

}

void Player::move(int input, Livello *currentLevel) {

}






