## RiddleMeInt
A fun little game I made in C++ out of boredom. 
No security at all over input lol.
### Game Rules and Other Stuff:
This game involves the player to try and guess the number within the time limit and using as little hints as possible. Numbers generated 5 digits or less.

Gamemodes:
- zen (no time limit)
- easy (3 minutes)
- normal (2 minutes)
- hard (30 seconds)
- insane (10 seconds)
- nowaythisisactuallypossible (1 second)

### How to build:
Download from the releases tab

Use GNU to compile it:

Linux:\
`g++ -ffreestanding -static -o riddlemeint main.cpp`

Windows:\
`g++ -ffreestanding -static -o riddlemeint main.cpp`