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
    bool heaterOn = false;

    // User command: heating initially disabled.
    bool heatingEnabled = false;

    // HC-002: Disabled heating commands the heater OFF.
    if (!heatingEnabled)
    {
        heaterOn = false;
    }

    if (currentState == State::OFF)
    {
        std::cout << "Starting State = OFF\n";
    }

    std::cout << "Heating enabled = " << heatingEnabled << '\n';
    std::cout << "Heater output " << heaterOn << '\n';
    return 0;
}
