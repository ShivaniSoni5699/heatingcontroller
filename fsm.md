# Heating Controller - Finite State Machine

## States

| State | Meaning | Heater output |
|---|---|---|
| OFF | Heating is disabled | OFF |
| ON | Heating is enabled and actively heating | ON |
| HOLD | Heating is enabled, waiting for temperature to fall | OFF |
| FAULT | A fault is latched | OFF |

OFF and HOLD have different restart behaviour:
- OFF requires heating to be enabled before it can restart.
- HOLD automatically restarts heating below 38°C.

## State Transitions

| Current state | Condition | Next state | Heater output |
|---|---|---|---|
| Startup | Controller starts | OFF | OFF |
| OFF | Enabled, valid fresh reading below 38°C | ON | ON |
| OFF | Enabled, valid fresh reading from 38°C to below 60°C | HOLD | OFF |
| OFF | Enabled with a fault condition | FAULT | OFF |
| ON | Temperature reaches or exceeds 40°C, with no fault | HOLD | OFF |
| HOLD | Temperature falls below 38°C, with no fault | ON | ON |
| ON or HOLD | Heating disabled, with no fault | OFF | OFF |
| ON or HOLD | A fault is detected | FAULT | OFF |
| FAULT | Reset requested and all recovery checks pass | OFF | OFF |
| Any state | No applicable transition | Remain in current state | Unchanged |

## Fault Conditions

- Invalid reading: outside -20°C to 120°C, or not a number.
- Sensor timeout: no new reading for 5 seconds or longer.
- Overtemperature: a valid reading at or above 60°C.
- Ineffective heating: temperature rises less than 2°C
  after 30 seconds of continuous heating.
- Exactly 2°C rise passes the ineffective-heating check.
- Restart the heating observation window whenever heating stops
  and starts again.

## Fault Recovery

A reset is accepted only when all conditions are true:
- Heating is disabled.
- A valid sensor reading is available.
- The reading is less than 5 seconds old.
- Temperature is below 50°C.
- A reset is requested.

An accepted reset returns the controller to OFF.
It does not immediately restart heating.
Disabling heating alone does not clear a fault.

## Evaluation Rules

- Fault detection takes priority over normal transitions.
- Invalid readings are not used for temperature decisions.
- Exactly 38°C retains the previous ON/HOLD state.
- Exactly 40°C changes ON to HOLD.
- If first enabled at exactly 38°C, enter HOLD.
- Report the fault reason when entering FAULT.