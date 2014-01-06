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
```

also can send `i` which returns current servo positions.

With this basic controller and a raspberry pi camera setup connected to it, 
we can now move the camera point of view and do fun timelapse stuff.