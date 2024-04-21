#include <bits/stdc++.h> //All main STD libraries
#include "bfs.hpp"


class TTTState: public GameState {
public:
    int board_player1;
    int board_player2;
    short player;

    GameState *clone() {
        return new TTTState(*this);
    }
};

class TTTGame: public GameEngine {
public:
    int winMask[8];
    vector<int> mValidAction;

    TTTGame(): GameEngine() {
        // Vertical in line
        winMask[0] = 0x124;  // 100100100
        winMask[1] = 0x92;   // 010010010
        winMask[2] = 0x49;   // 001001001

        // horizontal in line
        winMask[3] = 0x1c0;  // 111000000
        winMask[4] = 0x38;   // 000111000
        winMask[5] = 0x7;   //  000000111
    
        // diagonal 1
        winMask[6] = 0x111;  // 100010001
        // diagonal 2
        winMask[7] = 054;   // 001010100
    }

    int checkWin(int board1, int board2) {
        for(int i=0; i<8; i++) {
            if ((board1 & winMask[i]) == winMask[i]) return 1;
            if ((board2 & winMask[i]) == winMask[i]) return 2;
        }
        return 0;
    }

    void getNextStateAndReward(const GameState *state, int action, GameState *nextState, double &reward) {
        const TTTState *tttstate = (TTTState *)state;
        // do the action to the nextstate
        if(tttstate->player == 1) {
            ((TTTState *)nextState)->board_player1 = ((TTTState *)nextState)->board_player1 | (1 << (9-action));
            ((TTTState *)nextState)->player = 2;
        }

        if(tttstate->player == 2) {
            ((TTTState *)nextState)->board_player2 =  ((TTTState *)nextState)->board_player2 | (1 << (9-action));
            ((TTTState *)nextState)->player = 1;
        }

        // Check Win
        int winner = checkWin(((TTTState *)nextState)->board_player1, ((TTTState *)nextState)->board_player2);
        nextState->setTerminal(false);
        if (winner) {
            nextState->setTerminal(true);
            if(winner == 1) {
                nextState->setValue(1);
                reward = 1;
            }
                
            if(winner == 2) {
                nextState->setTerminal(true);
                nextState->setValue(-1);
                reward = -1;
            }
        }
    }

    bool isValidAction(const GameState *state, int action) {
        const TTTState *tttstate = (TTTState *)state;
        if ((1 << (9-action)) & (tttstate->board_player1)) return false;
        if ((1 << (9-action)) & (tttstate->board_player2)) return false;
        return true;
    }

    void printState(TTTState *state) {
        char board[3][3];
        for (int i = 0; i < 9; i++) {
            if ((1 << (9-i)) & (state->board_player1)) {
                board[(int)(i/3)][i%3] = 'X';
            } else if ((1 << (9-i)) & (state->board_player2))  {
                board[(int)(i/3)][i%3] = 'O';
            } else {
                board[(int)(i/3)][i%3] = '.';
            }
        }
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                cerr << board[i][j];
            }
            cerr << endl;
        }
    }

};


class RandomAgent {
public:
    int getAction(TTTState *state, vector<int> &validActions) {
        // choose random action
        int rand_idx = rand() % validActions.size();
        int action = validActions[rand_idx];
        return action;
    }
};

int main() {
    bool debug = false;
    TTTGame game;
    TTTState *state = new TTTState(), *nextState = new TTTState();
    game.setActionSet(9);

    RandomAgent agent1, agent2;
    
    // init game
    int action;
    double reward;
    double mean_reward = 0;
    int num_games = 100;
    
    for(int i=0; i<num_games; i++) {
        TTTState *state = new TTTState(), *nextState = new TTTState();
        state->board_player1 = 0;
        state->board_player2 = 0;
        state->player = 1;
        state->setTerminal(false);
        while(1) {
            vector<int> validAction;
            game.getValidActions(state, validAction);
            if(state->player == 1) {
                action = agent1.getAction(state, validAction);
            }
            if(state->player == 2) {
                action = agent2.getAction(state, validAction);
            }

            if(debug) {
                cerr << "Current player " << state->player << " choosed action "<<action << endl;
            }
            game.getNextStateAndReward(state, action, nextState, reward);

            if(debug) {
                game.printState(nextState);
                cerr << "---------------------------"<<endl;
                int tmp;
                cin >> tmp;
            }

            state->board_player1 = nextState->board_player1;
            state->board_player2 = nextState->board_player2;
            state->player = nextState->player;
            state->setTerminal(nextState->isTerminal());
            if(debug) {
                if(state->isTerminal()) {
                    cerr << "Player wins " << reward << "!" << endl;
                }
            }
            if(state->isTerminal()) {
                mean_reward += reward;
                break;
            }
        }
        delete state;
        delete nextState;
    }
    cerr << "Mean reward for " << num_games << " = " << mean_reward/num_games << endl;
    return 0;
}