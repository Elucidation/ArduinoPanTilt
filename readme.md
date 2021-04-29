A simple pan tilt servo motor arduino controller.

Commands
---
Communication is over the serial line by sending a command in the format of letter + integer values as needed:
```
s PAN_ANGLE TILT_ANGLE      --- Set Pan Tilt positions in degrees
p PAN_ANGLE                 --- Set Pan  position in degrees
t TILT_ANGLE                --- Set Tilt position in degrees
z                           --- Zero out to default center positions
i                           --- Info on current pan/tilt servo angles/positions
--- Rotation relative to current position
  u                           --- Up by PREC amount
  d                           --- Down by PREC amount
  l                           --- Left by PREC amount
  r                           --- Right by PREC amount
```


With this basic controller and a raspberry pi camera setup connected to it, 
we can now move the camera point of view and do fun timelapse stuff.

Possibly breaking changes since first version:
- soft turn when setting pan or tilt (with speed of 1 degree per 100ms)
- Information being sent back is now in form of a json data
