<!--
Disabled: heating OFF.
Normal control: starts below 38°C; stops at 40°C.
Sensor fault: invalid reading or 5 seconds without an update.
Overtemperature: 60°C or above.
Ineffective heating: less than 2°C rise after 30 seconds of continuous heating.
Fault recovery: stays OFF until recovery conditions and reset are satisfied.
-->

# Heating Controller with Fault Detection and Automated Testing

## Purpose

Build a heating controller, detect faults, and automatically
test whether its behaviour matches the requirements.

## Behaviour Overview

### 1. Heating disabled
- Heating stays OFF, regardless of temperature.
- Disabling heating does not clear an existing fault.

### 2. Normal heating
- When enabled, heating starts below 38°C.
- Once heating starts, it stays ON until temperature reaches 40°C.
- At 40°C or above, heating turns OFF.
- It stays OFF until temperature falls below 38°C.
- At exactly 38°C, keep the previous ON/OFF state.
- If first enabled between 38°C and 40°C, heating stays OFF.

### 3. Sensor faults
- The accepted sensor range is -20°C to 120°C, including both limits.
- A reading outside this range is invalid.
- No new reading for 5 seconds or longer causes a timeout.
- Measure the timeout from the last reading, not from startup.
- A sensor fault turns heating OFF and reports its reason.

### 4. Overtemperature
- A valid reading of 60°C or above causes an overtemperature fault.
- Heating turns OFF and the fault reason is reported.
- A reading can be valid but still indicate overheating.

### 5. Ineffective heating
- Record the temperature when continuous heating starts.
- After 30 seconds, check how much the temperature has risen.
- A rise below 2°C causes an ineffective-heating fault.
- A rise of exactly 2°C passes this check.
- Restart the observation window if heating stops and starts again.

### 6. Fault recovery
- A fault keeps heating OFF even if the original problem disappears.
- A reset request alone is not enough.
- Recovery conditions must also be satisfied.
- An accepted reset returns the controller to OFF.
- Reset is accepted only when all these conditions are true:
  - Heating is disabled.
  - A valid sensor reading is available (-20°C to 120°C).
  - The reading is less than 5 seconds old.
  - Temperature is below 50°C.
  - A reset is requested.
- Otherwise, the fault remains active and heating stays OFF.

## Important Notes

- These limits are learning-project design choices.
- Heating OFF means no heating is commanded.
- It does not mean the temperature immediately falls.
