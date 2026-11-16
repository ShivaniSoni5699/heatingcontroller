#include <iostream>
enum class State
{
    OFF,
    ON,
    HOLD,
    FAULT
};

int main()
{
    // HC-001: The controller starts in state OFF.
    State currentState = State::OFF;
    

    if (currentState == State::OFF)
    {
        std::cout << "Starting State = OFF\n";
    }
    return 0;
}