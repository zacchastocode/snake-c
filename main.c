#include "raylib.h"
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define GRID_SIZE 20
#define GRID_WIDTH (SCREEN_WIDTH / GRID_SIZE)
#define GRID_HEIGHT (SCREEN_HEIGHT / GRID_SIZE)
#define MAX_SNAKE_LENGTH (GRID_WIDTH * GRID_HEIGHT)
#define INITIAL_SPEED 0.15f

typedef struct {
    int x;
    int y;
} Vector2Int;

typedef enum {
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
} Direction;

typedef struct {
    Vector2Int body[MAX_SNAKE_LENGTH];
    int length;
    Direction direction;
    bool alive;
} Snake;

typedef struct {
    Vector2Int position;
    bool active;
} Food;

// Function prototypes
void InitGame(void);
void UpdateGame(void);
void DrawGame(void);
void ResetGame(void);
void SpawnFood(void);
bool IsFoodOnSnake(Vector2Int pos);

// Global variables
Snake snake;
Food food;
int score;
float moveTimer;
bool gameStarted;

int main(void) {
    // Initialization
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake Game");
    SetTargetFPS(60);
    
    srand(time(NULL));
    
    InitGame();
    
    // Main game loop
    while (!WindowShouldClose()) {
        // Update
        UpdateGame();
        
        // Draw
        BeginDrawing();
        ClearBackground(BLACK);
        DrawGame();
        EndDrawing();
    }
    
    // De-Initialization
    CloseWindow();
    
    return 0;
}

void InitGame(void) {
    // Initialize snake in the middle of the screen
    snake.length = 3;
    snake.direction = DIR_RIGHT;
    snake.alive = true;
    
    int startX = GRID_WIDTH / 2;
    int startY = GRID_HEIGHT / 2;
    
    for (int i = 0; i < snake.length; i++) {
        snake.body[i].x = startX - i;
        snake.body[i].y = startY;
    }
    
    // Initialize food
    food.active = false;
    SpawnFood();
    
    score = 0;
    moveTimer = 0.0f;
    gameStarted = false;
}

void ResetGame(void) {
    InitGame();
}

void UpdateGame(void) {
    if (!snake.alive) {
        // Wait for any key to restart
        if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
            ResetGame();
        }
        return;
    }
    
    // Handle input - change direction
    if (IsKeyPressed(KEY_UP) && snake.direction != DIR_DOWN) {
        snake.direction = DIR_UP;
        gameStarted = true;
    }
    if (IsKeyPressed(KEY_DOWN) && snake.direction != DIR_UP) {
        snake.direction = DIR_DOWN;
        gameStarted = true;
    }
    if (IsKeyPressed(KEY_LEFT) && snake.direction != DIR_RIGHT) {
        snake.direction = DIR_LEFT;
        gameStarted = true;
    }
    if (IsKeyPressed(KEY_RIGHT) && snake.direction != DIR_LEFT) {
        snake.direction = DIR_RIGHT;
        gameStarted = true;
    }
    
    if (!gameStarted) {
        return;
    }
    
    // Update movement timer
    moveTimer += GetFrameTime();
    
    if (moveTimer >= INITIAL_SPEED) {
        moveTimer = 0.0f;
        
        // Calculate new head position
        Vector2Int newHead = snake.body[0];
        
        switch (snake.direction) {
            case DIR_UP:
                newHead.y--;
                break;
            case DIR_DOWN:
                newHead.y++;
                break;
            case DIR_LEFT:
                newHead.x--;
                break;
            case DIR_RIGHT:
                newHead.x++;
                break;
        }
        
        // Check collision with walls
        if (newHead.x < 0 || newHead.x >= GRID_WIDTH || 
            newHead.y < 0 || newHead.y >= GRID_HEIGHT) {
            snake.alive = false;
            return;
        }
        
        // Check collision with self
        for (int i = 0; i < snake.length; i++) {
            if (newHead.x == snake.body[i].x && newHead.y == snake.body[i].y) {
                snake.alive = false;
                return;
            }
        }
        
        // Move snake body
        for (int i = snake.length - 1; i > 0; i--) {
            snake.body[i] = snake.body[i - 1];
        }
        
        // Update head position
        snake.body[0] = newHead;
        
        // Check if snake ate food
        if (newHead.x == food.position.x && newHead.y == food.position.y) {
            score += 10;
            
            // Grow snake
            if (snake.length < MAX_SNAKE_LENGTH) {
                snake.length++;
            }
            
            // Spawn new food
            SpawnFood();
        }
    }
}

void DrawGame(void) {
    if (!snake.alive) {
        // Draw game over screen
        DrawText("GAME OVER!", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 40, 40, RED);
        DrawText(TextFormat("Final Score: %d", score), SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2 + 10, 20, WHITE);
        DrawText("Press SPACE or ENTER to restart", SCREEN_WIDTH / 2 - 140, SCREEN_HEIGHT / 2 + 40, 20, GRAY);
        return;
    }
    
    // Draw grid (optional, for visual clarity)
    for (int i = 0; i < GRID_WIDTH; i++) {
        DrawLine(i * GRID_SIZE, 0, i * GRID_SIZE, SCREEN_HEIGHT, DARKGRAY);
    }
    for (int i = 0; i < GRID_HEIGHT; i++) {
        DrawLine(0, i * GRID_SIZE, SCREEN_WIDTH, i * GRID_SIZE, DARKGRAY);
    }
    
    // Draw snake
    for (int i = 0; i < snake.length; i++) {
        Color snakeColor = (i == 0) ? LIME : GREEN;
        DrawRectangle(
            snake.body[i].x * GRID_SIZE + 1,
            snake.body[i].y * GRID_SIZE + 1,
            GRID_SIZE - 2,
            GRID_SIZE - 2,
            snakeColor
        );
    }
    
    // Draw food
    if (food.active) {
        DrawRectangle(
            food.position.x * GRID_SIZE + 1,
            food.position.y * GRID_SIZE + 1,
            GRID_SIZE - 2,
            GRID_SIZE - 2,
            RED
        );
    }
    
    // Draw score
    DrawText(TextFormat("Score: %d", score), 10, 10, 20, WHITE);
    
    // Draw instructions if game hasn't started
    if (!gameStarted) {
        DrawText("Press arrow keys to start", SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT - 30, 20, GRAY);
    }
}

void SpawnFood(void) {
    int attempts = 0;
    int maxAttempts = GRID_WIDTH * GRID_HEIGHT;
    
    do {
        food.position.x = rand() % GRID_WIDTH;
        food.position.y = rand() % GRID_HEIGHT;
        attempts++;
        
        // Safety check: if grid is nearly full, just place food anywhere
        if (attempts >= maxAttempts) {
            break;
        }
    } while (IsFoodOnSnake(food.position));
    
    food.active = true;
}

bool IsFoodOnSnake(Vector2Int pos) {
    for (int i = 0; i < snake.length; i++) {
        if (pos.x == snake.body[i].x && pos.y == snake.body[i].y) {
            return true;
        }
    }
    return false;
}
