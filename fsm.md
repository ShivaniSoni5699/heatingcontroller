# Heating Controller - Finite State Machine

## States

| State | Meaning | Heater output |
|---|---|---|
| OFF | Heating is disabled, or waiting for the first reading | OFF |
| ON | Heating is enabled and actively heating | ON |
| HOLD | Heating is enabled, waiting for temperature to fall | OFF |
| FAULT | A fault is latched | OFF |

- Heating enabled means permission to heat.
- Heater ON means heat is commanded.
- OFF can wait for the first reading while heating is enabled.
- HOLD restarts heating below 38°C if heating remains enabled
  and no fault exists.
- The controller continues monitoring while the heater is OFF.

## State Transitions

Fault rules take priority over normal transitions.

| Current state | Condition | Next state | Heater output |
|---|---|---|---|
| Startup | Controller starts | OFF | OFF |
| OFF | Enabled, valid fresh reading below 38°C, no fault | ON | ON |
| OFF | Enabled, valid fresh reading from 38°C to below 60°C, no fault | HOLD | OFF |
| OFF | Enabled, no reading has ever arrived, waiting less than 5 seconds since enabling | OFF | OFF |
| OFF, ON or HOLD | A fault is detected | FAULT | OFF |
| ON | Enabled, valid fresh reading reaches 40°C or above, no fault | HOLD | OFF |
| HOLD | Enabled, valid fresh reading falls below 38°C, no fault | ON | ON |
| ON or HOLD | Heating disabled, no fault | OFF | OFF |
| FAULT | Reset requested and all recovery conditions pass | OFF | OFF |
| Any state | No applicable transition | Remain in current state | Unchanged |

## Sensor Validity and Freshness

- A valid reading is a finite number from -20°C to 120°C,
  including both limits.
- A reading outside this range, infinity or not a number
  is invalid.
- A fresh reading is less than 5 seconds old.
- Measure reading age from when the latest sensor update arrived.
- Only a new sensor update restarts the reading-age timer.
- Temperature decisions require a valid, fresh reading.

## Waiting for the First Reading

- If heating is enabled before any reading has arrived,
  keep the heater output OFF.
- Start the initial waiting timer when heating is enabled.
- If no reading arrives and waiting reaches 5 seconds,
  enter FAULT and report sensor timeout.
- If a reading arrives before the timeout, evaluate it
  using the normal sensor and temperature rules.
- Disabling heating before the first reading cancels
  the initial waiting timer.
- Enabling again starts a new initial waiting timer.
- Before any reading has arrived, remaining disabled
  does not cause a sensor timeout.

## Fault Conditions

### Invalid Sensor Reading

- An invalid sensor reading causes FAULT.

### Sensor Timeout

- Once a sensor update has arrived, an age of 5 seconds
  or longer causes FAULT.
- Before the first update, use the initial waiting rule above.

### Overtemperature

- A valid, fresh reading at or above 60°C causes FAULT.
- Detect overtemperature even when heating is disabled.

### Ineffective Heating

- Record the time and temperature when the heater turns ON.
- After 30 seconds of continuous heating, compare the current
  temperature with the temperature at the start of the window.
- A rise below 2°C causes FAULT.
- A rise of exactly 2°C or more passes.
- After a pass, start a new 30-second window using the current
  time and temperature.
- Repeat while the heater remains continuously ON.
- Turning the heater OFF cancels the observation window.
- Turning it ON again starts a fresh observation window.
- Reaching 40°C normally ends heating and cancels the check;
  it does not cause an ineffective-heating fault.

## Fault Classification and Reporting

| Classification | Faults | Action |
|---|---|---|
| Critical | Overtemperature, invalid sensor reading, sensor timeout | Heater OFF; fault latched |
| Performance | Ineffective heating | Heater OFF; fault latched |

- Invalid readings and sensor timeout are monitored even
  when heating is disabled, subject to the first-reading rules.
- Report all fault reasons detected in the evaluation
  that first enters FAULT.
- Keep those reasons latched until an accepted reset.
- Later readings do not erase or replace the stored reasons.
- An invalid or stale reading cannot confirm overtemperature.
- Fault classification does not delay stopping the heater.

## Fault Recovery

A reset is accepted only when all these conditions are true:

- Heating is disabled.
- A valid sensor reading is available.
- The reading is less than 5 seconds old.
- Temperature is below 50°C.
- A reset is requested.

An accepted reset:

- Clears the latched fault reasons.
- Returns the controller to OFF.
- Keeps the heater output OFF.

Otherwise, remain in FAULT.

- Disabling heating alone does not clear a fault.
- Valid readings returning alone do not clear a fault.
- Reset does not automatically enable heating.
- Reset does not prove that the underlying problem is repaired.

## Boundary and Evaluation Rules

- Exactly 38°C retains the previous ON or HOLD state.
- If enabled from OFF at exactly 38°C, enter HOLD.
- Exactly 40°C changes ON to HOLD.
- Exactly 60°C causes overtemperature if the reading is valid
  and fresh.
- A reading exactly 5 seconds old causes sensor timeout.
- Exactly 50°C does not satisfy fault recovery.
- Evaluate sensor validity and freshness before using
  temperature for a decision.
- A detected fault takes priority over normal heating behaviour.
- An existing latched fault stays active until an accepted reset.
- All temperature limits and timing values are design choices
  for this learning project.