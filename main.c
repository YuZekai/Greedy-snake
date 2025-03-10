#include "snake.h"

int main() {
    srand((unsigned int)time(0));
    LoadScores(); // Load scores from file
    int end = 1, result;
    while (end) {
        result = Menu();
        switch(result) {
            case 1:
                system("cls");
                InitMap();
                while (MoveSnake());
                break;
            case 2:
                system("cls");
                Help();
                break;
            case 3:
                system("cls");
                About();
                break;
            case 4:
                system("cls");
                ShowScores();
                break;
            case 0:
                system("cls");
                end = 0;
                break;
        }    
    }
    return 0;
}