#include "tictactoe_env.hpp"


int main() {

    TTTEnvironment *env = new TTTEnvironment();
    ActionSpace actionSpace = ActionSpace();
    
    for (int i = 0; i < 9; i++) {
        actionSpace.addAction(new TTTAction(i));
    }

    return 0;
}