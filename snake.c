#include "snake.h"

// Global variables
Snake snake;
Food food;
Obstacle obstacles[MAX_OBSTACLES];
int obstacleCount = 5;
ScoreEntry scores[MAX_SCORES];
int scoreCount = 0;
char now_Dir = RIGHT;
char direction = RIGHT;

void GotoXY(int x, int y) {
    HANDLE hout;
    COORD cor;
    hout = GetStdHandle(STD_OUTPUT_HANDLE);
    cor.X = x;
    cor.Y = y;
    SetConsoleCursorPosition(hout, cor);
}

void Hide() {
    HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cor_info = { 1,0 };
    SetConsoleCursorInfo(hout, &cor_info);
}

int Menu() {
    system("cls"); // Clear screen before showing menu
    GotoXY(40, 12);
    printf("Welcome to Snake Game!");
    GotoXY(43, 14);
    printf("1. Start Game");
    GotoXY(43, 16);
    printf("2. Help");
    GotoXY(43, 18);
    printf("3. About");
    GotoXY(43, 20);
    printf("4. Leaderboard");
    GotoXY(43, 22);
    printf("Press any other key to exit");
    Hide();
    char ch = _getch();
    int result = 0;
    switch (ch) {
        case '1': result = 1; break;
        case '2': result = 2; break;
        case '3': result = 3; break;
        case '4': result = 4; break;
        default: result = 0; break;
    }
    system("cls"); // Clear screen after selecting an option
    return result;
}

void Help() {
    system("cls"); // Clear screen before showing help
    GotoXY(40, 12);
    printf("w - Up");
    GotoXY(40, 14);
    printf("s - Down");
    GotoXY(40, 16);
    printf("a - Left");
    GotoXY(40, 18);
    printf("d - Right");
    GotoXY(40, 20);
    printf("Avoid obstacles and don't crash into yourself!");
    GotoXY(45, 22);
    printf("Press any key to return to menu");
    Hide();
    _getch();
    system("cls"); // Clear screen after returning to menu
}

void About() {
    system("cls"); // Clear screen before showing about
    GotoXY(39, 12);
    printf("Snake Game - Programming Practice");
    GotoXY(43, 14);
    printf("By Zekai Yu");
    GotoXY(43, 16);
    printf("yuzekai@hdu.edu.cn");
    GotoXY(39, 20);
    printf("Press any key to return to menu");
    Hide();
    _getch();
    system("cls"); // Clear screen after returning to menu
}

void InitObstacles() {
    for (int i = 0; i < obstacleCount; i++) {
        obstacles[i].x = rand() % (MAP_WIDTH - 2) + 1;
        obstacles[i].y = rand() % (MAP_HEIGHT - 2) + 1;
        GotoXY(obstacles[i].x, obstacles[i].y);
        printf("#");
    }
}

void InitMap() {
    system("cls"); // Clear screen before initializing map
    Hide();
    snake.snakeNode[0].x = MAP_WIDTH / 2 - 1;
    snake.snakeNode[0].y = MAP_HEIGHT / 2 - 1;
    GotoXY(snake.snakeNode[0].x, snake.snakeNode[0].y);
    printf("@");
    snake.length = 3;
    snake.speed = 250;
    now_Dir = RIGHT;

    // Snake body
    for (int i = 1; i < snake.length; i++) {
        snake.snakeNode[i].y = snake.snakeNode[i - 1].y;
        snake.snakeNode[i].x = snake.snakeNode[i - 1].x - 1;
        GotoXY(snake.snakeNode[i].x, snake.snakeNode[i].y);
        printf("o");
    }

    // Map borders
    for (int i = 0; i < MAP_HEIGHT; i++) {
        GotoXY(0, i);
        printf("X");
        GotoXY(MAP_WIDTH - 1, i);
        printf("X");
    }

    for (int i = 1; i < MAP_WIDTH - 1; i++) {
        GotoXY(i, 0);
        printf("X");
        GotoXY(i, MAP_HEIGHT - 1);
        printf("X");
    }

    InitObstacles(); // Initialize obstacles
    PrintFood();
    GotoXY(50, 5);
    printf("Current Score: 0");
}

void PrintFood() {
    int flag = 1;
    while (flag) {
        flag = 0;
        food.x = rand() % (MAP_WIDTH - 2) + 1;
        food.y = rand() % (MAP_HEIGHT - 2) + 1;
        food.type = rand() % 3 + 1; // Random food type: 1, 2, or 3

        for (int k = 0; k <= snake.length - 1; k++) {
            if (snake.snakeNode[k].x == food.x && snake.snakeNode[k].y == food.y) {
                flag = 1;
                break;
            }
        }
    }
    GotoXY(food.x, food.y);
    switch (food.type) {
        case 1: printf("1"); break; // Food type 1: +1 length
        case 2: printf("2"); break; // Food type 2: +2 length
        case 3: printf("3"); break; // Food type 3: +3 length
    }
}

int MoveSnake() {
    Snakenode temp;
    int flag = 0;
    temp = snake.snakeNode[snake.length - 1];
    for (int i = snake.length - 1; i >= 1; i--)
        snake.snakeNode[i] = snake.snakeNode[i - 1];
    GotoXY(snake.snakeNode[1].x, snake.snakeNode[1].y);
    printf("o");

    if (_kbhit()) {
        char key = _getch();
        if (key == 27) { // ESC key
            GotoXY(45, 10);
            printf("Paused");
            GotoXY(45, 12);
            printf("Press ESC or any key to continue...");
            while (1) {
                if (_kbhit()) {
                    key = _getch();
                    if (key == 27 || key != EOF) { // ESC or any key
                        GotoXY(45, 10);
                        printf("                  "); // Clear "Paused"
                        GotoXY(45, 12);
                        printf("                                      "); // Clear message
                        break;
                    }
                }
            }
        } else {
            switch (key) {
            case UP:
                if (now_Dir != DOWN)
                    now_Dir = key;
                break;
            case DOWN:
                if (now_Dir != UP)
                    now_Dir = key;
                break;
            case LEFT:
                if (now_Dir != RIGHT)
                    now_Dir = key;
                break;
            case RIGHT:
                if (now_Dir != LEFT)
                    now_Dir = key;
                break;
            }
        }
    }

    // Update snake head position
    switch (now_Dir) {
    case UP: snake.snakeNode[0].y--; break;
    case DOWN: snake.snakeNode[0].y++; break;
    case LEFT: snake.snakeNode[0].x--; break;
    case RIGHT: snake.snakeNode[0].x++; break;
    }

    // Display snake head
    GotoXY(snake.snakeNode[0].x, snake.snakeNode[0].y);
    printf("@");

    // Check if food is eaten
    if (snake.snakeNode[0].x == food.x && snake.snakeNode[0].y == food.y) {
        switch (food.type) {
            case 1: snake.length += 1; break; // +1 length
            case 2: snake.length += 2; break; // +2 length
            case 3: snake.length += 3; break; // +3 length
        }
        flag = 1;
        snake.snakeNode[snake.length - 1] = temp;
    }

    if (flag == 0) {
        GotoXY(temp.x, temp.y);
        printf(" ");
    }
    else {
        PrintFood();
        GotoXY(50, 5);
        printf("Current Score: %d      ", snake.length - 3); // Update score display
    }

    if (!IsCorrect()) {
        system("cls");
        int finalScore = snake.length - 3; // Final score = length - 3
        GotoXY(45, 14);
        printf("Final Score: %d", finalScore);

        // Ask for player's name
        char name[NAME_LENGTH];
        GotoXY(45, 16);
        printf("Enter your name: ");
        scanf("%s", name);
        AddScore(finalScore, name); // Add score to leaderboard

        GotoXY(45, 18);
        printf("Game Over!");
        GotoXY(45, 20);
        printf("Press any key to return to menu");
        _getch();
        system("cls");
        return 0;
    }

    SpeedControl();
    Sleep(snake.speed);
    return 1;
}

int IsCorrect() {
    // Wall collision
    if (snake.snakeNode[0].x == 0 || snake.snakeNode[0].y == 0 ||
        snake.snakeNode[0].x == MAP_WIDTH - 1 || snake.snakeNode[0].y == MAP_HEIGHT - 1)
        return 0;

    // Obstacle collision
    for (int i = 0; i < obstacleCount; i++) {
        if (snake.snakeNode[0].x == obstacles[i].x && snake.snakeNode[0].y == obstacles[i].y)
            return 0;
    }

    // Self collision
    for (int i = 1; i < snake.length; i++) {
        if (snake.snakeNode[0].x == snake.snakeNode[i].x &&
            snake.snakeNode[0].y == snake.snakeNode[i].y) {
            // Clear the truncated part
            for (int j = i; j < snake.length; j++) {
                GotoXY(snake.snakeNode[j].x, snake.snakeNode[j].y);
                printf(" ");
            }
            snake.length = i; // Cut the tail at collision point
            GotoXY(50, 5);
            printf("Current Score: %d      ", snake.length - 3); // Update score display
            return 1;
        }
    }
    return 1;
}

void SpeedControl() {
    if (snake.length >= 30) {
        snake.speed = 40;
    } else if (snake.length >= 27) {
        snake.speed = 60;
    } else if (snake.length >= 24) {
        snake.speed = 80;
    } else if (snake.length >= 21) {
        snake.speed = 100;
    } else if (snake.length >= 18) {
        snake.speed = 120;
    } else if (snake.length >= 15) {
        snake.speed = 140;
    } else if (snake.length >= 12) {
        snake.speed = 160;
    } else if (snake.length >= 9) {
        snake.speed = 180;
    } else if (snake.length >= 6) {
        snake.speed = 200;
    } else {
        snake.speed = 250;
    }
}

int CompareScores(const void* a, const void* b) {
    ScoreEntry* entryA = (ScoreEntry*)a;
    ScoreEntry* entryB = (ScoreEntry*)b;
    return entryB->score - entryA->score; // Sort in descending order
}

void AddScore(int score, const char* name) {
    if (scoreCount < MAX_SCORES) {
        strcpy(scores[scoreCount].name, name);
        scores[scoreCount].score = score;
        scores[scoreCount].timestamp = time(NULL); // Record current time
        scoreCount++;
    } else {
        // Replace the lowest score
        int minIndex = 0;
        for (int i = 1; i < MAX_SCORES; i++) {
            if (scores[i].score < scores[minIndex].score)
                minIndex = i;
        }
        if (score > scores[minIndex].score) {
            strcpy(scores[minIndex].name, name);
            scores[minIndex].score = score;
            scores[minIndex].timestamp = time(NULL); // Record current time
        }
    }

    // Sort the scores array in descending order
    qsort(scores, scoreCount, sizeof(ScoreEntry), CompareScores);

    SaveScores(); // Save sorted scores to file
}

void ShowScores() {
    system("cls"); // Clear screen before showing leaderboard
    GotoXY(50, 0);
    printf("Leaderboard");
    GotoXY(30, 2);
    printf("%-20s %7s %17s", "Name", "Score", "Played on"); // Header

    for (int i = 0; i < scoreCount; i++) {
        char timeStr[20];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localtime(&scores[i].timestamp));

        // Calculate the padding for score to center it
        int scoreWidth = 10; // Column width for score
        int scoreDigits = snprintf(NULL, 0, "%d", scores[i].score); // Get number of digits in score
        int padding = (scoreWidth - scoreDigits) / 2; // Calculate padding for centering

        GotoXY(30, 4 + i);
        printf("%-20s", scores[i].name); // Left-aligned name

        // Print leading spaces for centering the score
        for (int j = 0; j < padding; j++) {
            printf(" ");
        }
        printf("%d", scores[i].score); // Print the score

        // Print trailing spaces for centering the score
        for (int j = 0; j < scoreWidth - scoreDigits - padding; j++) {
            printf(" ");
        }

        printf("%20s", timeStr); // Right-aligned timestamp
    }

    GotoXY(40, 6 + scoreCount);
    printf("Press any key to return to menu");
    _getch();
    system("cls"); // Clear screen after returning to menu
}
void SaveScores() {
    FILE* file = fopen("scores.txt", "w");
    if (file) {
        for (int i = 0; i < scoreCount; i++) {
            fprintf(file, "%s %d %ld\n", scores[i].name, scores[i].score, scores[i].timestamp);
        }
        fclose(file);
    }
}

void LoadScores() {
    FILE* file = fopen("scores.txt", "r");
    if (file) {
        scoreCount = 0;
        while (fscanf(file, "%s %d %ld", scores[scoreCount].name, &scores[scoreCount].score, &scores[scoreCount].timestamp) != EOF) {
            scoreCount++;
            if (scoreCount >= MAX_SCORES) break;
        }
        fclose(file);
    }
}