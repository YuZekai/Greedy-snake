#ifndef SNAKE_H
#define SNAKE_H

#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <time.h>
#include <string.h>

#define MAP_HEIGHT 20
#define MAP_WIDTH 40
#define UP 'w'
#define DOWN 's'
#define LEFT 'a'
#define RIGHT 'd'
#define MAX_OBSTACLES 10
#define MAX_SCORES 10
#define NAME_LENGTH 50

typedef struct {
    int x;
    int y;
} Snakenode;

typedef struct {
    int x;
    int y;
    int type; // Food type: 1-+1 length, 2-+2 length, 3-+3 length
} Food;

typedef struct {
    int x;
    int y;
} Obstacle;

typedef struct {
    char name[NAME_LENGTH];
    int score;
    time_t timestamp; // Timestamp of the game end
    int survivalTime; // Survival time in seconds
} ScoreEntry;

typedef struct {
    Snakenode snakeNode[1000];
    int length;
    int speed;
} Snake;

// Global variables
extern Snake snake;
extern Food food;
extern Obstacle obstacles[MAX_OBSTACLES];
extern int obstacleCount;
extern ScoreEntry scores[MAX_SCORES];
extern int scoreCount;
extern char now_Dir;
extern char direction;

// Function declarations
void GotoXY(int x, int y);
void Hide();
int Menu();
void Help();
void About();
void InitMap();
void PrintFood();
int MoveSnake();
int IsCorrect();
void SpeedControl();
void InitObstacles();
void AddScore(int score, const char* name);
void ShowScores();
void SaveScores();
void LoadScores();

#endif