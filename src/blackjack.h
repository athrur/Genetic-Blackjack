#ifndef BLACKJACK_H
#define BLACKJACK_H

#include <iostream>
#include <random>
#include <array>

enum Move
{
    STAND = 0,
    HIT = 1,
    DOUBLE = 2,
    SPLIT = 3
};

class Deck
{
private:
    std::array<int, 13> cards;
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<> dis;

public:
    Deck();
    int deal();
};

class Player
{
public:
    int hand = 0;
    int aces = 0;
    bool blackjack = false;

    int hit(int card);
    int decide();
};

class Dealer : public Player
{
public:
    int decide();
    void setup_round();
};

class Agent : public Player
{
private:
    int dealer_card = 0;
    double cash = 0;
    int hard_strategy[17][10];
    int soft_strategy[9][10];

public:
    void setup_round(int card);
    void add_cash(double amount);
    void set_cash(double amount);
    double get_cash();
    void set_strategy(int hard[17][10], int soft[10][10]);
    int decide();
};

class Game
{
private:
    Deck deck;
    Dealer dealer;
    Agent agent;

public:
    double get_cash();
    void set_strategy(int hard[17][10], int soft[9][10]);
    double calculate_payout();
    double play_game(bool verbose = false);
};

#endif
