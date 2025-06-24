// Dimitri LaPoudre //

#include "blackjack.h"

void Blackjack::calculateRealProbabilities(size_t points, size_t aceHigh,
                                           std::map<Card, size_t> &deck, size_t nbCard,
                                           std::array<double, 5> &resultProbabilities)
{
    for (auto it = deck.rbegin(); it != deck.rend();) {
        auto [key, value] = *it;
        if (value == 0) {
            it++;
            continue;
        }
        size_t aceHighCopy = (key == Card::Ace) ? aceHigh + 1 : aceHigh;
        std::map<Card, size_t> newDeck(deck);
        std::array<double, 5> probabilities = {};
        newDeck[key]--;
        if (points + static_cast<size_t>(key) <= 21) {
            calculateRealProbabilities(
            points + static_cast<size_t>(key), aceHighCopy, newDeck, nbCard - 1, probabilities);
            resultProbabilities[0] += probabilities[0] * value;
            resultProbabilities[1] += probabilities[1] * value;
            resultProbabilities[2] += probabilities[2] * value;
            resultProbabilities[3] += probabilities[3] * value;
            resultProbabilities[4] += probabilities[4] * value;
            it++;
        } else if (aceHighCopy > 0 && points + static_cast<size_t>(key) - 10 <= 21) {
            calculateRealProbabilities(points + static_cast<size_t>(key) - 10,
                                       aceHighCopy - 1,
                                       newDeck,
                                       nbCard - 1,
                                       probabilities);
            resultProbabilities[0] += probabilities[0] * value;
            resultProbabilities[1] += probabilities[1] * value;
            resultProbabilities[2] += probabilities[2] * value;
            resultProbabilities[3] += probabilities[3] * value;
            resultProbabilities[4] += probabilities[4] * value;
            it++;
        } else {
            auto tmp = std::next(it).base();
            it = std::reverse_iterator(deck.erase(tmp));
        }
    }
    resultProbabilities[0] =
    (points == 17) ? 1.0 : resultProbabilities[0] / static_cast<double>(nbCard);
    resultProbabilities[1] =
    (points == 18) ? 1.0 : resultProbabilities[1] / static_cast<double>(nbCard);
    resultProbabilities[2] =
    (points == 19) ? 1.0 : resultProbabilities[2] / static_cast<double>(nbCard);
    resultProbabilities[3] =
    (points == 20) ? 1.0 : resultProbabilities[3] / static_cast<double>(nbCard);
    resultProbabilities[4] =
    (points == 21) ? 1.0 : resultProbabilities[4] / static_cast<double>(nbCard);
}

void Blackjack::calculateSimpleProbabilities(size_t points, size_t aceHigh,
                                             std::map<Card, size_t> &deck, size_t nbCard,
                                             std::array<double, 5> &resultProbabilities)
{
    if (points >= 17) {
        resultProbabilities[points - 17] = 1.0;
        return;
    }
    for (auto it = deck.rbegin(); it != deck.rend();) {
        auto [key, value] = *it;
        if (value == 0) {
            it++;
            continue;
        }
        size_t aceHighCopy = (key == Card::Ace) ? aceHigh + 1 : aceHigh;
        std::map<Card, size_t> newDeck(deck);
        std::array<double, 5> probabilities = {};
        newDeck[key]--;
        if (points + static_cast<size_t>(key) <= 21) {
            calculateSimpleProbabilities(
            points + static_cast<size_t>(key), aceHighCopy, newDeck, nbCard - 1, probabilities);
            resultProbabilities[0] += probabilities[0] * value;
            resultProbabilities[1] += probabilities[1] * value;
            resultProbabilities[2] += probabilities[2] * value;
            resultProbabilities[3] += probabilities[3] * value;
            resultProbabilities[4] += probabilities[4] * value;
            it++;
        } else if (aceHighCopy > 0 && points + static_cast<size_t>(key) - 10 <= 21) {
            calculateSimpleProbabilities(points + static_cast<size_t>(key) - 10,
                                         aceHighCopy - 1,
                                         newDeck,
                                         nbCard - 1,
                                         probabilities);
            resultProbabilities[0] += probabilities[0] * value;
            resultProbabilities[1] += probabilities[1] * value;
            resultProbabilities[2] += probabilities[2] * value;
            resultProbabilities[3] += probabilities[3] * value;
            resultProbabilities[4] += probabilities[4] * value;
            it++;
        } else {
            auto tmp = std::next(it).base();
            it = std::reverse_iterator(deck.erase(tmp));
        }
    }
    resultProbabilities[0] /= static_cast<double>(nbCard);
    resultProbabilities[1] /= static_cast<double>(nbCard);
    resultProbabilities[2] /= static_cast<double>(nbCard);
    resultProbabilities[3] /= static_cast<double>(nbCard);
    resultProbabilities[4] /= static_cast<double>(nbCard);
}

void Blackjack::showPlayerProbabilities(size_t player)
{
    std::cout << std::endl << "Info Player " << player << ":" << std::endl;
    size_t points = 0;
    size_t aceHigh = 0;
    std::cout << "Hand:" << std::endl;
    for (auto &[key, value] : playersHand[player - 1]) {
        if (value == 0) {
            continue;
        }
        points += static_cast<size_t>(key) * value;
        if (key == Card::Ace) {
            aceHigh = value;
        }
        std::cout << static_cast<size_t>(key) << " x" << value << std::endl;
    }
    while (points > 21 && aceHigh > 0) {
        points -= 10;
        aceHigh--;
    }
    std::cout << "Probabilities (Simple -> Real) to get: " << std::endl;
    std::map<Card, size_t> newDeck(deck);
    size_t nbCard = 0;
    for (auto &[key, value] : deck) {
        nbCard += value;
    }
    std::array<double, 5> realProbabilities = {};
    calculateRealProbabilities(points, aceHigh, newDeck, nbCard, realProbabilities);
    std::array<double, 5> simpleProbabilities = {};
    calculateSimpleProbabilities(points, aceHigh, newDeck, nbCard, simpleProbabilities);
    for (size_t i = 0; i < 5; i++) {
        std::cout << 17 + i << ": " << std::fixed << simpleProbabilities[i] << " -> "
                  << realProbabilities[i] << std::endl;
    }
    std::cout << "No-Bust: " << std::fixed
              << (simpleProbabilities[0] + simpleProbabilities[1] + simpleProbabilities[2]
                  + simpleProbabilities[3] + simpleProbabilities[4])
              << std::endl;
    std::cout << "Bust: " << std::fixed
              << 1.0
                 - (simpleProbabilities[0] + simpleProbabilities[1] + simpleProbabilities[2]
                    + simpleProbabilities[3] + simpleProbabilities[4])
              << std::endl;
}

void Blackjack::showInfo(std::istringstream &args)
{
    std::size_t player;
    args >> player;
    if (args.eof() && args.fail()) {
        player = 0;
    } else if (args.fail()) {
        std::cerr << "Invalid input." << std::endl;
        return;
    } else if (player == 0) {
        std::cerr << "Invalid player number." << std::endl;
        return;
    }

    std::cout << "Deck cards:" << std::endl;
    for (auto &[key, value] : deck) {
        if (value == 0) {
            continue;
        }
        std::cout << static_cast<size_t>(key) << ": x" << value << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Probabilities for Dealer:" << std::endl;
    size_t points = 0;
    size_t aceHigh = 0;
    std::cout << "Hand:" << std::endl;
    for (auto &[key, value] : dealerHand) {
        if (value == 0) {
            continue;
        }
        points += static_cast<size_t>(key) * value;
        if (key == Card::Ace) {
            aceHigh = value;
        }
        std::cout << static_cast<size_t>(key) << " x" << value << std::endl;
    }
    while (points > 21 && aceHigh > 0) {
        points -= 10;
        aceHigh--;
    }
    std::cout << "Simple Probabilities to get: " << std::endl;
    std::map<Card, size_t> newDeck(deck);
    size_t nbCard = 0;
    for (auto &[key, value] : newDeck) {
        nbCard += value;
    }
    std::array<double, 5> probabilities = {};
    calculateSimpleProbabilities(points, aceHigh, newDeck, nbCard, probabilities);
    for (size_t i = 0; i < 5; i++) {
        std::cout << 17 + i << ": " << std::fixed << probabilities[i] << std::endl;
    }
    std::cout << "No-Bust: " << std::fixed
              << (probabilities[0] + probabilities[1] + probabilities[2] + probabilities[3]
                  + probabilities[4])
              << std::endl;
    std::cout << "Bust: " << std::fixed
              << 1.0
                 - (probabilities[0] + probabilities[1] + probabilities[2] + probabilities[3]
                    + probabilities[4])
              << std::endl;
    if (player > 0) {
        if (player - 1 >= playersHand.size()) {
            std::cerr << "Invalid player number." << std::endl;
            return;
        }
        showPlayerProbabilities(player);
    } else {
        for (player = 1; player <= playersHand.size(); player++) {
            showPlayerProbabilities(player);
        }
    }
}
