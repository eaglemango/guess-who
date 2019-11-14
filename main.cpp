#include "dotter.hpp"

int main(int argc, char* argv[]) {
    GuessTree guesser(argv[1]);

    int i = 0;
    while (i < 3) {
        printf("===== New game =====\n");
        guesser.AskQuestions();
        printf("=====          =====\n");

        ++i;
    }

    Dotter dotter;
    dotter.ProcessGuessTree(guesser);

    return 0;
}
