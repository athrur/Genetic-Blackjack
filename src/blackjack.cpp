#include "blackjack.h"

Deck::Deck() : cards{2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11}, gen(rd()), dis(0, 12)
{
}

int Deck::deal()
{
    return cards[dis(gen)];
}

int Player::hit(int card)
{
    hand += card;
    if (card == 11)
    {
        aces++;
    }
    return hand;
}

int Player::decide()
{
    return STAND;
}

int Dealer::decide()
{
    if (hand > 21 && aces > 0)
    {
        hand -= 10;
        aces--;
    }
    if (hand < 17)
    {
        return HIT;
    }
    else
    {
        return STAND;
    }
}

void Dealer::setup_round()
{
    aces = 0;
    blackjack = false;
    hand = 0;
}

void Agent::setup_round(int card)
{
    dealer_card = card;
    aces = 0;
    blackjack = false;
    hand = 0;
}

void Agent::add_cash(double amount)
{
    cash += amount;
}

void Agent::set_cash(double amount)
{
    cash = amount;
}

double Agent::get_cash()
{
    return cash;
}

void Agent::set_strategy(int hard[17][10], int soft[9][10])
{
    memcpy(hard_strategy, hard, sizeof(hard_strategy));
    memcpy(soft_strategy, soft, sizeof(soft_strategy));
}

int Agent::decide()
{
    if (hand > 21 && aces > 0)
    {
        hand -= 10;
        aces--;
    }
    if (hand >= 21)
    {
        return STAND;
    }
    if (aces > 0)
    {
        return soft_strategy[hand - 12][dealer_card - 2];
    }
    else
    {
        return hard_strategy[hand - 4][dealer_card - 2];
    }
}

double Game::get_cash()
{
    return agent.get_cash();
}

void Game::set_strategy(int hard[17][10], int soft[9][10])
{
    agent.set_strategy(hard, soft);
    agent.set_cash(0);
}

double Game::calculate_payout()
{
    if ((dealer.blackjack && !agent.blackjack) || agent.hand > 21)
    {
        return -1;
    }
    else if (agent.blackjack && !dealer.blackjack)
    {
        return 1.5;
    }
    else if (dealer.hand > 21 || agent.hand > dealer.hand)
    {
        return 1;
    }
    else if (dealer.hand == agent.hand)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

double Game::play_game(bool verbose)
{
    int dealer_card = deck.deal();
    dealer.hit(dealer_card);
    dealer.setup_round();
    agent.setup_round(dealer_card);
    dealer.hit(deck.deal());
    agent.hit(deck.deal());
    agent.hit(deck.deal());
    if (dealer.hand == 21)
    {
        dealer.blackjack = true;
    }
    else
    {
        while (dealer.decide() == HIT)
        {
            dealer.hit(deck.deal());
        }
    }

    if (agent.hand == 21)
    {
        agent.blackjack = true;
    }
    else
    {
        while (agent.decide() == HIT)
        {
            agent.hit(deck.deal());
        }
    }

    double payout = calculate_payout();
    agent.add_cash(payout);
    if (verbose)
    {
        std::cout << "Dealer: " << dealer.hand << std::endl;
        std::cout << "Agent: " << agent.hand << std::endl;
        std::cout << "Payout: " << payout << std::endl;
        std::cout << "Cash: " << agent.get_cash() << std::endl;
    }
    return payout;
}
