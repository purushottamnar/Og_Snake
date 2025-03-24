#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <stdbool.h>

#define WIDTH 40
#define HEIGHT 20
#define SNAKE_LENGTH 256

// Game state
struct {
    int x[SNAKE_LENGTH];
    int y[SNAKE_LENGTH];
    int length;
    int direction;
    int foodX;
    int foodY;
    bool gameOver;
} game;

// Function declarations
void setup();
void draw();
void input();
void logic();
void generateFood();
void gotoxy(int x, int y);

int main() {
    setup();
    
    while (!game.gameOver) {
        draw();
        input();
        logic();
        Sleep(100); // Control game speed
    }
    
    system("cls");
    printf("\nGame Over! Score: %d\n", game.length - 1);
    return 0;
}

void setup() {
    game.length = 1;
    game.direction = 0; // 0: right, 1: left, 2: up, 3: down
    game.x[0] = WIDTH / 2;
    game.y[0] = HEIGHT / 2;
    game.gameOver = false;
    
    // Generate first food
    generateFood();
    
    // Hide cursor
    CONSOLE_CURSOR_INFO cursor_info = {1, 0};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

void draw() {
    system("cls");
    
    // Draw top border
    for (int i = 0; i < WIDTH + 2; i++)
        printf("#");
    printf("\n");
    
    // Draw game area
    for (int i = 1; i <= HEIGHT; i++) {
        printf("#");
        for (int j = 1; j <= WIDTH; j++) {
            bool printed = false;
            
            // Draw snake
            for (int k = 0; k < game.length; k++) {
                if (game.x[k] == j && game.y[k] == i) {
                    printf("O");
                    printed = true;
                    break;
                }
            }
            
            // Draw food
            if (!printed && game.foodX == j && game.foodY == i) {
                printf("*");
                printed = true;
            }
            
            // Draw empty space
            if (!printed)
                printf(" ");
        }
        printf("#\n");
    }
    
    // Draw bottom border
    for (int i = 0; i < WIDTH + 2; i++)
        printf("#");
    printf("\n");
    
    printf("Score: %d\n", game.length - 1);
}

void input() {
    if (_kbhit()) {
        switch (_getch()) {
            case 'a':
                if (game.direction != 0) game.direction = 1;
                break;
            case 'd':
                if (game.direction != 1) game.direction = 0;
                break;
            case 'w':
                if (game.direction != 3) game.direction = 2;
                break;
            case 's':
                if (game.direction != 2) game.direction = 3;
                break;
            case 'x':
                game.gameOver = true;
                break;
        }
    }
}

void logic() {
    // Move snake body
    for (int i = game.length - 1; i > 0; i--) {
        game.x[i] = game.x[i - 1];
        game.y[i] = game.y[i - 1];
    }
    
    // Move snake head
    switch (game.direction) {
        case 0: // Right
            game.x[0]++;
            break;
        case 1: // Left
            game.x[0]--;
            break;
        case 2: // Up
            game.y[0]--;
            break;
        case 3: // Down
            game.y[0]++;
            break;
    }
    
    // Check for collision with walls
    if (game.x[0] <= 0 || game.x[0] >= WIDTH + 1 ||
        game.y[0] <= 0 || game.y[0] >= HEIGHT + 1) {
        game.gameOver = true;
        return;
    }
    
    // Check for collision with self
    for (int i = 1; i < game.length; i++) {
        if (game.x[0] == game.x[i] && game.y[0] == game.y[i]) {
            game.gameOver = true;
            return;
        }
    }
    
    // Check if food is eaten
    if (game.x[0] == game.foodX && game.y[0] == game.foodY) {
        game.length++;
        generateFood();
    }
}

void generateFood() {
    bool valid;
    do {
        valid = true;
        game.foodX = rand() % WIDTH + 1;
        game.foodY = rand() % HEIGHT + 1;
        
        // Check if food spawned on snake
        for (int i = 0; i < game.length; i++) {
            if (game.foodX == game.x[i] && game.foodY == game.y[i]) {
                valid = false;
                break;
            }
        }
    } while (!valid);
}

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
} 