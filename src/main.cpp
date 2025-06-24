// Dimitri LaPoudre //

#include "blackjack.h"

int main(int argc, char **argv)
{
    int code = 0;

    std::signal(SIGINT, [](int) {
        std::cout << "\b\b  \nExiting the game due to interrupt signal." << std::endl;
        exit(0);
    });
    try {
        Blackjack game(argc, argv);
        game.shell();
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        code = 1;
    } catch (...) {
        std::cerr << "An unknown error occurred." << std::endl;
        code = 1;
    }
    return code;
}
