/*
* File: main.c
* Author: David Šimek
* Date: 6.8.2024
* Description: Terminal displays text from first argument and spawns particles that colide with it.
*/

#include <signal.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#define SLEEP_TIME 45000

typedef struct {
	float x;
	float y;
	float x_dir;	
	float y_dir;
} particle;

static int width;
static int height;
static char* text;

// writes characters into buffer based on particles position
void render(char* buffer, particle* particles, int particle_count);

// calculates new positon of particle
void update_movement(particle* particle);

// draws particles into terminal
void draw(char* buffer);

// initializes buffer
void init_buffer(char* buffer, size_t buffer_size);

// gets size of window
void get_window_sizes();

// clears screen after SIGINT
void do_on_exit(int signal);

int main(int argc, char** args) {
	signal(SIGINT, do_on_exit);
	if (argc != 2) {
		perror("You have to provide text.");
		exit(-1);
	}
	text = args[1];
	get_window_sizes();

	if (strlen(text) > width + 4) {
		perror("Too long text\ncommand <text>\n");
		exit(-1);
	}

	size_t buffer_size = width * height + 1;
	char buffer[buffer_size];

	init_buffer(buffer, buffer_size);

	int particle_count = 8;
	particle particles[] = {
		{0.0f, 0.0f, 1.0f, 0.1f},
		{0.0f, 0.0f, 1.0f, 0.4f},
		{0.0f, 0.0f, 1.0f, 0.7f},
		{0.0f, 0.0f, 1.0f, 0.9f},
		{0.0f, 0.0f, 0.1f, 1.0f},
		{0.0f, 0.0f, 0.3f, 1.0f},
		{0.0f, 0.0f, 0.6f, 1.0f},
		{0.0f, 0.0f, 0.9f, 1.0f}
	};

	while (true) {
		render(buffer, particles, particle_count);
		draw(buffer);
		for (int i = 0; i < particle_count; ++i) {
			update_movement(&(particles[i]));
		}
	}
	return 0;
}

void render(char* buffer, particle* particles, int particle_count) {
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			char* current = &(buffer[j + width * i]);
			switch (*current) {
				case '@' : *current = '#'; break;
				case '#' : *current = '='; break;
				case '=' : *current = '+'; break;
				case '+' : *current = '*'; break;
				case '*' : *current = '-'; break;
				case '-' : *current = ':'; break;
				case ':' : *current = '.'; break;
				case '.' : *current = ' '; break;
			}
		}
	}

	for (int i = 0; i < particle_count; ++i) {
		particle current = particles[i];
		int x = current.x;
		int y = current.y;

		buffer[x + y * width] = '@';
		if (x > 0)	buffer[(x - 1) + y  * width] = '@';
		if (y > 0)	buffer[ x + (y - 1) * width] = '@';
		if (x < width)	buffer[(x + 1) + y  * width] = '@';
		if (y < height) buffer[ x + (y + 1) * width] = '@';
	}

	memcpy(&(buffer[width / 2 + (height / 2) * width - strlen(text) / 2]), text, strlen(text));
}

void update_movement(particle* particle) {
	float x = particle->x;
	float y = particle->y;
	float x_dir = particle->x_dir;
	float y_dir = particle->y_dir;

	if ((int)(x + x_dir) >= width - 1 || (int)(x + x_dir) < 0)
		x_dir = -(x_dir);
	if ((int)(y + y_dir) >= height - 1 || (int)(y + y_dir) < 0)
		y_dir = -(y_dir);
	if (
		(int)(y + y_dir) == height / 2 &&
		(int)(x + x_dir) >= width / 2 - strlen(text) / 2 &&
		(int)(x + x_dir) <= width / 2 + strlen(text) / 2
	) {
		y_dir = -(y_dir);
	}

	x = x + x_dir;
	y = y + y_dir;
	particle->x = x;
	particle->y = y;
	particle->x_dir = x_dir;
	particle->y_dir = y_dir;
}

void draw(char* buffer) {
	printf("\x1b[2J");
	printf("\033[H");

	for (int i = 0; i < (width * 2) + 3; ++i) {
		printf("-");
	}
	printf("\n");
	for (int i = 0; i < height; ++i) {
		printf("| ");
		for (int j = 0; j < width; ++j) {
			char current = buffer[j + width * i];

			switch (current) {
				case '@' : printf("\033[38;2;255;0;0m%c \033[0m", current); break;
				case '#' : printf("\033[38;2;255;56;0m%c \033[0m", current); break;
				case '=' : printf("\033[38;2;255;81;0m%c \033[0m", current); break;
				case '+' : printf("\033[38;2;255;101;0m%c \033[0m", current); break;
				case '*' : printf("\033[38;2;255;118;0m%c \033[0m", current); break;
				case '-' : printf("\033[38;2;253;133;5m%c \033[0m", current); break;
				case ':' : printf("\033[38;2;250;146;17m%c \033[0m", current); break;
				case '.' : printf("\033[38;2;247;159;29m%c \033[0m", current); break;
				default  : printf("%c ", current); break;
			}
		}

		printf("|");
		printf("\n");
	}
	for (int i = 0; i < (width * 2) + 3; ++i) {
		printf("-");
	}
	fflush(stdout);
	usleep(SLEEP_TIME);
}

void init_buffer(char* buffer, size_t buffer_size) {
	memset(buffer, ' ', buffer_size);
	buffer[buffer_size] = 0;
}

void get_window_sizes() {
	struct winsize w_size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w_size);
	width = w_size.ws_col / 2 - 2;
	height = w_size.ws_row - 2;

}

void do_on_exit(int signal) {
	printf("\033[H\033[J");
	exit(0);
}
