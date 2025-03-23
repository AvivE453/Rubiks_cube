In this program I rendered a dynamic Rubik's cube using OpenGL in C++.

Keyboard keys:
  • "R" – press state for right wall rotation (90 degrees clockwise).
  • "L" – press state for left wall rotation (90 degrees clockwise).
  • "U" – press state for up wall rotation (90 degrees clockwise).
  • "D" – press state for down wall rotation (90 degrees clockwise).
  • "B" – press state for back wall rotation (90 degrees clockwise).
  • "F" – press state for front wall rotation (90 degrees clockwise).
  • " " (Space) – press state for flipping rotation direction (from clockwise to counterclockwise or vice versa).
  • "Z" – press state: dividing rotation angle by 2 (until minimum of 45).
  • "A" – press state: multiply rotation angle by 2 (until maximum of 180).
  • "M" -press state: mix the cube by doing random rotations.
  Notice: The rotation directions are only relative to the cube initial camera view.

Mouse keys:
 • Moving while holding left button will rotate the whole Rubik's cube.
 • Moving while holding the right button will move the camera up, down, left or right (can also use Arrow keys).
 • Scrolling up and down will move the camera along the Z axis backward and forward

Picking mode:
 • By using "P" key you can pick a specific cube with the mouse, for translation of the cube use the left mouse button and for 
     rotation of the cube use the right mouse button(disabled by pressing the "P" key again).
 Notice: After using this mode the cube will be destroyed.

 how to run ?
 • use "make" command to run the makefile 
 • enter bin directory and run "./main" command 

 
