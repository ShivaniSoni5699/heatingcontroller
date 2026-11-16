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

Develop a heating controller, detect faults, and automatically
test whether its behaviour matches these requirements.

## Definitions

- Heating enabled: the user gives permission to heat.
- Heater ON: the controller commands heat.
- Heater OFF: no heat is commanded; the controller still runs.
- Valid reading: a finite number from -20°C to 120°C inclusive.
- Fresh reading: a reading less than 5 seconds old.
- Latched fault: a remembered fault that requires an accepted
  reset before normal operation can resume.

## 1. Startup and Heating Commands

| ID | Requirement |
|---|---|
| HC-001 | At startup, the controller shall enter OFF with the heater output OFF. |
| HC-002 | When heating is disabled, the heater output shall remain OFF. Disabling shall not clear a latched fault. |
| HC-003 | Enabling heating shall permit temperature control only when a valid, fresh reading is available and no fault is latched. |
| HC-004 | The controller shall continue monitoring sensor updates while the heater output is OFF. |

## 2. Normal Temperature Control

| ID | Requirement |
|---|---|
| HC-005 | When enabled from OFF with a valid, fresh reading below 38°C and no fault, the controller shall enter ON. |
| HC-006 | When enabled from OFF with a valid, fresh reading from 38°C to below 60°C and no fault, the controller shall enter HOLD. |
| HC-007 | The heater output shall be ON only in state ON. It shall be OFF in OFF, HOLD and FAULT. |
| HC-008 | While enabled and without a fault, ON shall change to HOLD when a valid, fresh reading reaches 40°C or above. |
| HC-009 | While enabled and without a fault, HOLD shall change to ON when a valid, fresh reading falls below 38°C. |
| HC-010 | At exactly 38°C, the controller shall retain its previous ON or HOLD state. If enabled from OFF at exactly 38°C, it shall enter HOLD. |
| HC-011 | Disabling heating in ON or HOLD shall return the controller to OFF unless a fault is detected. |

<!--
Heating depends on previous states 
Already heating at 39°C → continue heating until 40°C.
Already in HOLD at 39°C → stay OFF until below 38°C.
-->
## 3. Sensor Validity and Freshness

| ID | Requirement |
|---|---|
| HC-012 | A sensor reading outside -20°C to 120°C, infinity or not a number shall cause an invalid-sensor fault. Both range limits shall be accepted. |
| HC-013 | Reading age shall be measured from receipt of the latest sensor update. Only a new update shall restart this timer. |
| HC-014 | Once a sensor update has arrived, a reading age of 5 seconds or longer shall cause a sensor-timeout fault. |
| HC-015 | Temperature decisions shall use only valid, fresh readings. |
| HC-016 | Invalid readings and sensor timeout shall be monitored even when heating is disabled, subject to the first-reading rules below. |

## 4. Waiting for the First Reading

| ID | Requirement |
|---|---|
| HC-017 | If heating is enabled before any sensor reading has arrived, the controller shall remain in OFF with the heater output OFF while waiting. |
| HC-018 | The initial waiting timer shall start when heating is enabled. If no reading arrives and waiting reaches 5 seconds, the controller shall report sensor timeout and enter FAULT. |
| HC-019 | A reading received before the initial timeout shall be evaluated using the normal validity, freshness and temperature rules. |
| HC-020 | Disabling heating before the first reading shall cancel the initial waiting timer. Enabling again shall start a new initial waiting timer. |
| HC-021 | Before any reading has arrived, remaining disabled shall not cause a sensor timeout. |
<!--
After receiving a reading: count 5 seconds from the latest sensor update.
Before receiving any reading: count 5 seconds from enabling heating. Keep the heater OFF while waiting.
-->
## 5. Overtemperature

| ID | Requirement |
|---|---|
| HC-022 | A valid, fresh reading at or above 60°C shall cause an overtemperature fault, even when heating is disabled. |
| HC-023 | Invalid or stale readings shall not be used to confirm overtemperature. They shall be handled as sensor faults. |

## 6. Ineffective Heating

| ID | Requirement |
|---|---|
| HC-024 | Whenever the heater turns ON, the controller shall record the current time and temperature as the start of an observation window. |
| HC-025 | After 30 seconds of continuous heating, a temperature rise below 2°C during that window shall cause an ineffective-heating fault. |
| HC-026 | A rise of exactly 2°C or more shall pass the check. After a pass, a new 30-second window shall start using the current time and temperature. |
| HC-027 | Turning the heater OFF shall cancel the observation window. Turning it ON again shall start a fresh window. |
| HC-028 | Reaching 40°C normally shall end heating and cancel the observation window without causing an ineffective-heating fault. |

## 7. Fault Handling and Reporting

| ID | Requirement |
|---|---|
| HC-029 | Every detected fault shall cause FAULT and command the heater output OFF in the same controller evaluation. |
| HC-030 | Overtemperature, invalid sensor readings and sensor timeout shall share the critical classification. Ineffective heating shall have the performance classification. |
| HC-031 | The controller shall report all fault reasons detected in the evaluation that first enters FAULT. |
| HC-032 | Those reasons shall remain latched until an accepted reset. Later readings shall not erase or replace them. |
| HC-033 | Fault detection shall take priority over normal heating transitions. Fault classification shall not delay stopping the heater. |

## 8. Fault Recovery

| ID | Requirement |
|---|---|
| HC-034 | In FAULT, reset shall be accepted only when heating is disabled, a valid reading is available, its age is less than 5 seconds, temperature is below 50°C, and reset is requested. |
| HC-035 | An accepted reset shall clear the stored fault reasons, return the controller to OFF and keep the heater output OFF. |
| HC-036 | If any recovery condition is not satisfied, the controller shall remain in FAULT with the heater output OFF. |
| HC-037 | Disabling heating or receiving valid readings alone shall not clear a fault. Reset shall not automatically enable heating. |
<!--
Every fault stops heating and remembers why. 
Reset clears that memory only when all recovery conditions pass, it leaves the heater OFF.
-->

## Important Notes

- These thresholds and timings are learning project design choices.
- Heater OFF does not mean the temperature immediately falls.
- An accepted reset does not prove the underlying fault is repaired.
- Timing is evaluated when the controller processes an update.
  The execution interval will be defined before implementation.