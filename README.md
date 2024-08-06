# Terminal Text Display with Particle Effects

This program displays text provided as the first argument in the terminal and spawns particles that collide with the text.

#### Author
David Šimek

#### Date
August 6, 2024

## Description

This program uses terminal graphics to display a specified text string and animates particles that move around and interact with the text. The particles will change their direction upon collision with the text or the terminal window edges.

## Example
https://github.com/user-attachments/assets/2725cda0-5c9e-4157-a28e-d662886c86b0

## Features
Displays text at the center of the terminal.
Spawns particles that move and interact with the displayed text.
Changes particle characters as they move to create a visual effect.
Handles window resizing and terminal clearing upon exit.

## Requirements
- unix-like operating system with a terminal that supports ANSI escape codes.
- gcc or another C compiler above C99/C9X.

## Compilation
To compile the program, run:
```bash
gcc -o terminal_particles main.c
```
## Usage
To run the program, use:
```bash
./terminal_particles "Your Text Here"
```
Replace "Your Text Here" with the text you want to display in the terminal.
## Example
```bash
./terminal_particles "Hello, World!"
```
## Code Overview
Main Functions

- `main`: The entry point of the program. It initializes the signal handler, checks arguments, initializes the buffer, and starts the animation loop.
- `render`: Updates the buffer based on particle positions and the text.
- update_movement: Calculates new positions for particles and handles collisions.
- `draw`: Draws the buffer to the terminal with color-coded particles.
- `init_buffer`: Initializes the buffer with empty spaces.
- `get_window_sizes`: Retrieves the current terminal window size.
- `do_on_exit`: Handles cleanup and terminal clearing when the program is interrupted.

## Data Structures

- `particle`: Struct to store the position and direction of a particle.

## Constants

- `SLEEP_TIME`: The sleep time in microseconds between each frame of animation.

## Signal Handling

The program handles SIGINT (Ctrl+C) to clear the terminal and exit gracefully.
```c
void do_on_exit(int signal) {
printf("\033[H\033[J");
printf("Program ended with SIGINT\n");
printf("demo by @davidSimek\n");
exit(0);

}
```
## License
This project is licensed under the MIT License.
