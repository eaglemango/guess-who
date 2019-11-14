#include "dotter.hpp"

int main(int argc, char* argv[]) {
    GuessTree guesser;

    if (argc < 2) {
        printf("Invalid launch\n");

        return 1;
    } else {
        if (argc >= 3) {
            guesser = GuessTree(argv[2]);
        }

        if (!strcasecmp(argv[1], MODE_PLAY)) {
            bool active_game = true;
            while (active_game) {
                printf("===== New game =====\n");
                guesser.AskQuestions();
                printf("\nEnter 1 for new round, 0 to exit: ");
                int response = 0;
                scanf("%d", &response);

                active_game = static_cast<bool>(response);
            } 

        } else if (!strcasecmp(argv[1], MODE_WHOIS)) {
            bool found = false;
            guesser.WhoIs(0, argv[3], found);

            if (found) {
                printf("\nThat's all, that I have for %s\n", argv[3]);

            } else {
                printf("I know nothing about %s, sorry\n", argv[3]);
            }

        } else if (!strcasecmp(argv[1], MODE_COMPARE)) {

        } else {
            printf("Invalid mode\n");

            return 2;
        }
    }

    Dotter dotter;
    dotter.ProcessGuessTree(guesser);

    return 0;
}
