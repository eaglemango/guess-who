#include "dotter.hpp"

int main(int argc, char* argv[]) {
    GuessTree guesser;

    int i = 0;
    while (i < 5) {
        printf("===== New game =====\n");
        guesser.AskQuestions();
        printf("=====          =====\n");

        ++i;
    }

    Dotter dotter;
    dotter.ProcessGuessTree(guesser);

    return 0;
}
