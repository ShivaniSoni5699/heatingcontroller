# Controller Interface

## Inputs

Each input line represents one controller evaluation.

| Field | Meaning |
|---|---|
| time_ms | Current simulated time in milliseconds |
| enable | 1 gives permission to heat; 0 disables heating |
| new_reading | 1 means a sensor update arrived; 0 means no update |
| temp_c | Temperature in degrees Celsius; ignored when new_reading is 0 |
| reset | 1 requests fault reset; 0 makes no reset request |

Input format:

time_ms enable new_reading temp_c reset

Example:

0 1 1 25.0 0

Meaning: at time 0 ms, heating is enabled, a new reading
of 25°C arrives, and no reset is requested.