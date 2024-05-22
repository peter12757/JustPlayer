//
// Created by Peter Xi on 2022/5/28.
//

#ifndef ANDROIDTEST_STATEMACHINE_H
#define ANDROIDTEST_STATEMACHINE_H

#include <map>
#include "LinkedList.h"
#include "State.h"
class StateMachine {
public:
    // Methods

    StateMachine();
    ~StateMachine();
    void init();
    void run();

    // When a stated is added we pass the function that represents
    // that state logic
    State* addState(void (*functionPointer)());
    State* transitionTo(State* s);
    int transitionTo(int i);

    // Attributes
    LinkedList<State*> *stateList;
    bool executeOnce = true; 	//Indicates that a transition to a different state has occurred
    int currentState = -1;	//Indicates the current state number
};


#endif //ANDROIDTEST_STATEMACHINE_H
