// Dimitri LaPoudre //

#ifndef BLACKJACK_H
#define BLACKJACK_H

#include <array>
#include <csignal>
#include <exception>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <unordered_map>
#include <vector>

enum class Card : size_t {
    Two = 2,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Jack = 10,
    Queen = 10,
    King = 10,
    Ace = 11
};

class Blackjack {
public:
    Blackjack(int ac, char **av);
    void shell(void);

private:
    bool shellRunning;
    size_t deckSize;
    std::map<Card, size_t> deck;
    std::map<Card, size_t> dealerHand;
    size_t nbPlayer;
    size_t currentPlayer;
    std::vector<std::map<Card, size_t>> playersHand;
    void initializeDeck(void);
    std::map<char, Card> cardSign;
    void drawCard(std::map<Card, size_t> &hand, char &card);
    void showHand(std::map<Card, size_t> &hand);
    size_t countPoints(std::map<Card, size_t> &hand, size_t *aceHighTmp = NULL);
    void showPoints(std::map<Card, size_t> &hand);
    void showDealderProbabilities(void);
    void showPlayerProbabilities(size_t player);
    void calculateRealProbabilities(size_t points, size_t aceHigh, std::map<Card, size_t> &deck,
                                    size_t nbCard, std::array<double, 5> &resultProbabilities);
    void calculateSimpleProbabilities(size_t points, size_t aceHigh, std::map<Card, size_t> &deck,
                                      size_t nbCard, std::array<double, 5> &resultProbabilities);

    std::unordered_map<std::string, void (Blackjack::*)(std::istringstream &)> shellCommand;
    void shuffleDeck(std::istringstream &args);
    void dealerDrawCard(std::istringstream &args);
    void playerDrawCard(std::istringstream &args);
    void showInfo(std::istringstream &args);
    void exitShell(std::istringstream &args);
};

#endif // BLACKJACK_H
