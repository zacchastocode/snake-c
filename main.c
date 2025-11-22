#include "raylib.h"
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define GRID_SIZE 20
#define GRID_WIDTH (SCREEN_WIDTH / GRID_SIZE)
#define GRID_HEIGHT (SCREEN_HEIGHT / GRID_SIZE)
#define MAX_SNAKE_LENGTH (GRID_WIDTH * GRID_HEIGHT)
#define INITIAL_SNAKE_LENGTH 4

typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    Position segments[MAX_SNAKE_LENGTH];
    int length;
    int direction; // 0: UP, 1: RIGHT, 2: DOWN, 3: LEFT
    int nextDirection;
} Snake;

typedef struct {
    Position position;
    bool active;
} Food;

// Function prototypes
void InitGame(Snake *snake, Food *food, int *score);
void UpdateGame(Snake *snake, Food *food, int *score, bool *gameOver, float *moveTimer, float moveInterval);
void DrawGame(Snake *snake, Food *food, int score, bool gameOver);
void SpawnFood(Food *food, Snake *snake);
bool CheckCollision(Snake *snake);
bool CheckFoodCollision(Snake *snake, Food *food);

int main(void) {
    // Initialize window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake Game");
    SetTargetFPS(60);
    
    // Seed random number generator
    srand(time(NULL));
    
    // Game variables
    Snake snake = {0};
    Food food = {0};
    int score = 0;
    bool gameOver = false;
    float moveTimer = 0.0f;
    float moveInterval = 0.15f; // Snake moves every 0.15 seconds
    
    InitGame(&snake, &food, &score);
    
    // Main game loop
    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        
        // Input handling
        if (IsKeyPressed(KEY_UP) && snake.direction != 2) {
            snake.nextDirection = 0;
        }
        if (IsKeyPressed(KEY_RIGHT) && snake.direction != 3) {
            snake.nextDirection = 1;
        }
        if (IsKeyPressed(KEY_DOWN) && snake.direction != 0) {
            snake.nextDirection = 2;
        }
        if (IsKeyPressed(KEY_LEFT) && snake.direction != 1) {
            snake.nextDirection = 3;
        }
        
        // Restart game on game over
        if (gameOver && IsKeyPressed(KEY_SPACE)) {
            InitGame(&snake, &food, &score);
            gameOver = false;
            moveTimer = 0.0f;
        }
        
        // Update game
        if (!gameOver) {
            UpdateGame(&snake, &food, &score, &gameOver, &moveTimer, moveInterval);
        }
        
        // Draw
        BeginDrawing();
        ClearBackground(DARKGRAY);
        DrawGame(&snake, &food, score, gameOver);
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}

void InitGame(Snake *snake, Food *food, int *score) {
    // Initialize snake in the middle of the screen
    snake->length = INITIAL_SNAKE_LENGTH;
    snake->direction = 1; // Start moving right
    snake->nextDirection = 1;
    
    int startX = GRID_WIDTH / 2;
    int startY = GRID_HEIGHT / 2;
    
    for (int i = 0; i < snake->length; i++) {
        snake->segments[i].x = startX - i;
        snake->segments[i].y = startY;
    }
    
    // Initialize food
    food->active = false;
    SpawnFood(food, snake);
    
    // Reset score
    *score = 0;
}

void UpdateGame(Snake *snake, Food *food, int *score, bool *gameOver, float *moveTimer, float moveInterval) {
    *moveTimer += GetFrameTime();
    
    // Move snake at intervals
    if (*moveTimer >= moveInterval) {
        *moveTimer = 0.0f;
        
        // Update direction
        snake->direction = snake->nextDirection;
        
        // Calculate new head position
        Position newHead = snake->segments[0];
        
        switch (snake->direction) {
            case 0: newHead.y -= 1; break; // UP
            case 1: newHead.x += 1; break; // RIGHT
            case 2: newHead.y += 1; break; // DOWN
            case 3: newHead.x -= 1; break; // LEFT
        }
        
        // Check wall collision
        if (newHead.x < 0 || newHead.x >= GRID_WIDTH ||
            newHead.y < 0 || newHead.y >= GRID_HEIGHT) {
            *gameOver = true;
            return;
        }
        
        // Check self collision
        for (int i = 0; i < snake->length; i++) {
            if (newHead.x == snake->segments[i].x && 
                newHead.y == snake->segments[i].y) {
                *gameOver = true;
                return;
            }
        }
        
        // Check food collision
        bool ateFood = false;
        if (food->active && newHead.x == food->position.x && 
            newHead.y == food->position.y) {
            ateFood = true;
            food->active = false;
            (*score)++;
            SpawnFood(food, snake);
        }
        
        // Move snake body
        if (ateFood) {
            // Add new head without removing tail (snake grows)
            for (int i = snake->length; i > 0; i--) {
                snake->segments[i] = snake->segments[i - 1];
            }
            snake->length++;
        } else {
            // Move body forward
            for (int i = snake->length - 1; i > 0; i--) {
                snake->segments[i] = snake->segments[i - 1];
            }
        }
        
        // Set new head position
        snake->segments[0] = newHead;
    }
}

void DrawGame(Snake *snake, Food *food, int score, bool gameOver) {
    // Draw grid lines (optional, for visual reference)
    for (int i = 0; i <= GRID_WIDTH; i++) {
        DrawLine(i * GRID_SIZE, 0, i * GRID_SIZE, SCREEN_HEIGHT, GRAY);
    }
    for (int i = 0; i <= GRID_HEIGHT; i++) {
        DrawLine(0, i * GRID_SIZE, SCREEN_WIDTH, i * GRID_SIZE, GRAY);
    }
    
    // Draw food
    if (food->active) {
        DrawRectangle(
            food->position.x * GRID_SIZE + 2,
            food->position.y * GRID_SIZE + 2,
            GRID_SIZE - 4,
            GRID_SIZE - 4,
            RED
        );
    }
    
    // Draw snake
    for (int i = 0; i < snake->length; i++) {
        Color segmentColor = (i == 0) ? LIME : GREEN;
        DrawRectangle(
            snake->segments[i].x * GRID_SIZE + 1,
            snake->segments[i].y * GRID_SIZE + 1,
            GRID_SIZE - 2,
            GRID_SIZE - 2,
            segmentColor
        );
    }
    
    // Draw score
    DrawText(TextFormat("SCORE: %d", score), 10, 10, 20, WHITE);
    
    // Draw game over message
    if (gameOver) {
        const char *gameOverText = "GAME OVER!";
        const char *restartText = "Press SPACE to restart";
        int textWidth1 = MeasureText(gameOverText, 40);
        int textWidth2 = MeasureText(restartText, 20);
        
        DrawText(gameOverText, SCREEN_WIDTH / 2 - textWidth1 / 2, 
                 SCREEN_HEIGHT / 2 - 30, 40, RED);
        DrawText(restartText, SCREEN_WIDTH / 2 - textWidth2 / 2, 
                 SCREEN_HEIGHT / 2 + 20, 20, WHITE);
    }
}

void SpawnFood(Food *food, Snake *snake) {
    // If snake fills the entire grid, no valid position exists
    if (snake->length >= GRID_WIDTH * GRID_HEIGHT) {
        food->active = false;
        return;
    }
    
    bool validPosition = false;
    int maxAttempts = GRID_WIDTH * GRID_HEIGHT * 2; // Safety limit
    int attempts = 0;
    
    while (!validPosition && attempts < maxAttempts) {
        food->position.x = rand() % GRID_WIDTH;
        food->position.y = rand() % GRID_HEIGHT;
        
        validPosition = true;
        
        // Check if food spawns on snake
        for (int i = 0; i < snake->length; i++) {
            if (food->position.x == snake->segments[i].x &&
                food->position.y == snake->segments[i].y) {
                validPosition = false;
                break;
            }
        }
        
        attempts++;
    }
    
    food->active = validPosition;
}
