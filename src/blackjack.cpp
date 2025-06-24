// Dimitri LaPoudre //

#include "blackjack.h"

Blackjack::Blackjack(int ac, char **av)
: shellRunning(false), deckSize(0), nbPlayer(0), currentPlayer(0)
{
    if (ac != 3) {
        throw std::invalid_argument("Usage: blackjack <number_of_decks> <numbers_of_players>");
    }

    deckSize = std::stoul(av[1]);
    nbPlayer = std::stoul(av[2]);
    initializeDeck();
    playersHand.resize(nbPlayer);
    shellCommand["shuffle"] = &Blackjack::shuffleDeck;
    shellCommand["s"] = &Blackjack::shuffleDeck;
    shellCommand["dealer"] = &Blackjack::dealerDrawCard;
    shellCommand["d"] = &Blackjack::dealerDrawCard;
    shellCommand["player"] = &Blackjack::playerDrawCard;
    shellCommand["p"] = &Blackjack::playerDrawCard;
    shellCommand["info"] = &Blackjack::showInfo;
    shellCommand["i"] = &Blackjack::showInfo;
    shellCommand["exit"] = &Blackjack::exitShell;
    shellCommand["e"] = &Blackjack::exitShell;
    cardSign['2'] = Card::Two;
    cardSign['3'] = Card::Three;
    cardSign['4'] = Card::Four;
    cardSign['5'] = Card::Five;
    cardSign['6'] = Card::Six;
    cardSign['7'] = Card::Seven;
    cardSign['8'] = Card::Eight;
    cardSign['9'] = Card::Nine;
    cardSign['t'] = Card::Ten;
    cardSign['T'] = Card::Ten;
    cardSign['j'] = Card::Jack;
    cardSign['J'] = Card::Jack;
    cardSign['q'] = Card::Queen;
    cardSign['Q'] = Card::Queen;
    cardSign['k'] = Card::King;
    cardSign['K'] = Card::King;
    cardSign['a'] = Card::Ace;
    cardSign['A'] = Card::Ace;
}

void Blackjack::initializeDeck(void)
{
    deck[Card::Two] = deckSize * 4;
    deck[Card::Three] = deckSize * 4;
    deck[Card::Four] = deckSize * 4;
    deck[Card::Five] = deckSize * 4;
    deck[Card::Six] = deckSize * 4;
    deck[Card::Seven] = deckSize * 4;
    deck[Card::Eight] = deckSize * 4;
    deck[Card::Nine] = deckSize * 4;
    deck[Card::Ten] = deckSize * 4;
    deck[Card::Jack] += deckSize * 4;
    deck[Card::Queen] += deckSize * 4;
    deck[Card::King] += deckSize * 4;
    deck[Card::Ace] = deckSize * 4;
}

void Blackjack::drawCard(std::map<Card, size_t> &hand, char &card)
{
    if (cardSign.find(card) == cardSign.end()) {
        std::cerr << "Invalid card sign: " << card << std::endl;
        return;
    }
    deck[cardSign[card]]--;
    hand[cardSign[card]]++;
}

void Blackjack::showHand(std::map<Card, size_t> &hand)
{
    if (&hand == &dealerHand) {
        std::cout << "Dealer Hand after draw: ";
    } else {
        for (size_t i = 0; i < playersHand.size(); i++) {
            if (&hand == &playersHand[i]) {
                std::cout << "Player " << (i + 1) << " Hand after draw: ";
                break;
            }
        }
    }

    size_t points = 0;
    size_t aceHigh = 0;
    for (auto &[key, value] : hand) {
        if (value == 0) {
            continue;
        }
        points += static_cast<size_t>(key) * value;
        if (key == Card::Ace) {
            aceHigh = value;
        }
    }
    while (points > 21 && aceHigh > 0) {
        points -= 10;
        aceHigh--;
    }
    std::cout << points << std::endl;
}

void Blackjack::shuffleDeck(std::istringstream &args)
{
    (void)args;
    for (auto &[key, value] : deck) {
        value += dealerHand[key];
        dealerHand[key] = 0;
        for (auto &player : playersHand) {
            value += player[key];
            player[key] = 0;
        }
    }
    std::cout << "Cards shuffle into the deck." << std::endl;
}

void Blackjack::dealerDrawCard(std::istringstream &args)
{
    std::string cards;
    args >> cards;
    if (args.fail()) {
        std::cerr << "Invalid card list." << std::endl;
        return;
    }

    for (auto &card : cards) {
        drawCard(dealerHand, card);
    }
    showHand(dealerHand);
}

void Blackjack::playerDrawCard(std::istringstream &args)
{
    std::size_t player;
    args >> player;
    if (args.fail() || player - 1 >= nbPlayer) {
        std::cerr << "Invalid player number." << std::endl;
        return;
    }
    std::string cards;
    args >> cards;
    if (args.fail()) {
        std::cerr << "Invalid card list." << std::endl;
        return;
    }

    for (auto &card : cards) {
        drawCard(playersHand[player - 1], card);
    }
    showHand(playersHand[player - 1]);
}

void Blackjack::exitShell(std::istringstream &args)
{
    (void)args;
    std::cout << "Exiting the table. Goodbye!" << std::endl;
    shellRunning = false;
}
