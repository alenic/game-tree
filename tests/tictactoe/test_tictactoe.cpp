#include "tictactoe_env.hpp"

int main()
{

    TTTEnvironment *env = new TTTEnvironment();
    ActionSpace actionSpace = ActionSpace();
    bool done = false;

    for (int i = 0; i < 9; i++)
    {
        actionSpace.addAction(new TTTAction(i));
    }

    TTTGlobalState *globalState = new TTTGlobalState();
    TTTState *state = new TTTState();
    TTTState *nextState = nullptr;
    TTTAction *action;

    cout << "Initial state: " << *state << endl;
    cout << *globalState << endl;

    while (!done)
    {
        vector<unsigned int> *validActions = env->getValidActionsIds(globalState, state, &actionSpace);

        for (int i = 0; i < validActions->size(); i++)
        {
            cout << "Valid action: " << validActions->at(i) << endl;
        }

        int actionId = rand() % validActions->size();
        action = (TTTAction *)actionSpace.getAction(validActions->at(actionId));
        cout << "Action: " << *action << endl;

        double reward;
        nextState = dynamic_cast<TTTState *>(env->transitionFunction(state, action, globalState, reward, done));
        cout << "Next state: " << *nextState << endl;
        cout << *globalState << endl;

        delete state;
        state = nextState;

        delete validActions;
    }
    delete env;
    return 0;
}