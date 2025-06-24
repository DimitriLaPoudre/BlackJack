// Dimitri LaPoudre //

#include "blackjack.h"

void Blackjack::shell(void)
{
    std::cout << "Welcome to the Blackjack game shell!" << std::endl;
    std::cout << "Type 'help' for available commands." << std::endl;
    std::string line;
    std::istringstream iss;
    std::string command;

    shellRunning = true;
    while (shellRunning) {
        std::cout << "> ";
        std::getline(std::cin, line);

        if (std::cin.eof() && std::cin.fail()) {
            std::cout << "End of input detected. Exiting the game." << std::endl;
            shellRunning = false;
        } else {
            iss.clear();
            iss.str(line);
            iss >> command;
            if (iss.fail()) {
                continue;
            }
            if (shellCommand.find(command) == shellCommand.end()) {
                std::cout << "Unknown command: " << command << ". Type 'help' for available commands." << std::endl;
                continue;
            } else {
                (this->*shellCommand[command])(iss);
            }
        }
    }
}