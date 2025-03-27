#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>

using namespace std;

// Board and speed constants
const int BOARD_WIDTH = 10;
const int BOARD_HEIGHT = 20;
const int INITIAL_SPEED = 200;
const int MIN_SPEED = 100;
const int SPEED_INCREMENT = 2;

// Tetromino shapes (7 standard Tetris pieces)
vector<vector<vector<int>>> TETROMINOES = {
    {{1, 1, 1, 1}},             // I
    {{1, 1}, {1, 1}},           // O
    {{0, 1, 0}, {1, 1, 1}},     // T
    {{0, 1, 1}, {1, 1, 0}},     // S
    {{1, 1, 0}, {0, 1, 1}},     // Z
    {{1, 0, 0}, {1, 1, 1}},     // J
    {{0, 0, 1}, {1, 1, 1}}      // L
};

// Structure to represent a Tetromino (piece)
struct Tetromino {
    vector<vector<int>> shape;
    int x, y;
};

// Structure to maintain game state
struct GameState {
    vector<vector<int>> board;
    Tetromino currentPiece;
    int score;
    bool gameOver;
    int speed;
};

// Global game state variable
GameState game;
DWORD lastTick;

// Function to hide the console cursor
void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}

// Function to initialize/reset the game
void initializeGame() {
    game.board = vector<vector<int>>(BOARD_HEIGHT, vector<int>(BOARD_WIDTH, 0));
    game.score = 0;
    game.gameOver = false;
    game.speed = INITIAL_SPEED;
    lastTick = GetTickCount();
    hideCursor();
}

// Function to spawn a new Tetromino
Tetromino spawnPiece() {
    int randomIndex = rand() % TETROMINOES.size();
    return {TETROMINOES[randomIndex], BOARD_WIDTH / 2 - 1, 0};
}

// Function to check if the piece collides with anything
bool checkCollision(Tetromino piece) {
    for (int y = 0; y < piece.shape.size(); y++) {
        for (int x = 0; x < piece.shape[y].size(); x++) {
            if (piece.shape[y][x]) {
                int boardX = piece.x + x;
                int boardY = piece.y + y;
                if (boardX < 0 || boardX >= BOARD_WIDTH || boardY >= BOARD_HEIGHT || game.board[boardY][boardX]) {
                    return true;
                }
            }
        }
    }
    return false;
}

// Function to clear completed lines
void clearLines() {
    vector<vector<int>> newBoard(BOARD_HEIGHT, vector<int>(BOARD_WIDTH, 0));
    int newRow = BOARD_HEIGHT - 1;
    int clearedLines = 0;
    
    for (int y = BOARD_HEIGHT - 1; y >= 0; y--) {
        bool fullLine = true;
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (game.board[y][x] == 0) {
                fullLine = false;
                break;
            }
        }
        if (!fullLine) {
            newBoard[newRow--] = game.board[y];
        } else {
            clearedLines++;
        }
    }
    game.board = newBoard;
    game.score += clearedLines * 100;
}

// Function to merge the Tetromino into the board when it lands
void mergePiece() {
    for (int y = 0; y < game.currentPiece.shape.size(); y++) {
        for (int x = 0; x < game.currentPiece.shape[y].size(); x++) {
            if (game.currentPiece.shape[y][x]) {
                game.board[game.currentPiece.y + y][game.currentPiece.x + x] = 1;
            }
        }
    }
    clearLines();
    game.currentPiece = spawnPiece();
    if (checkCollision(game.currentPiece)) {
        game.gameOver = true;
    } else {
        game.speed = max(MIN_SPEED, game.speed - SPEED_INCREMENT);
    }
}

// Function to move the Tetromino
void movePiece(int dx, int dy) {
    Tetromino newPiece = game.currentPiece;
    newPiece.x += dx;
    newPiece.y += dy;
    if (!checkCollision(newPiece)) {
        game.currentPiece = newPiece;
    } else if (dy > 0) {
        mergePiece();
    }
}

// ** Function to rotate the Tetromino (FIXED) **
void rotatePiece() {
    Tetromino newPiece = game.currentPiece;
    int rows = newPiece.shape.size();
    int cols = newPiece.shape[0].size();
    vector<vector<int>> rotated(cols, vector<int>(rows, 0));

    // Rotate 90 degrees clockwise
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            rotated[x][rows - 1 - y] = newPiece.shape[y][x];
        }
    }

    newPiece.shape = rotated;
    if (!checkCollision(newPiece)) {
        game.currentPiece = newPiece; // Apply rotation if valid
    }
}

// Function to render the game board with the current falling piece
void renderBoard() {
    COORD cursorPosition = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);

    for (int y = 0; y <= BOARD_HEIGHT; y++) {
        for (int x = -1; x <= BOARD_WIDTH; x++) {
            if (x == -1 || x == BOARD_WIDTH || y == BOARD_HEIGHT) {
                cout << (y == BOARD_HEIGHT ? '_' : '|'); // Draw borders
            } else {
                bool isPiece = false;
                // Render the falling Tetromino
                for (int py = 0; py < game.currentPiece.shape.size(); py++) {
                    for (int px = 0; px < game.currentPiece.shape[py].size(); px++) {
                        if (game.currentPiece.shape[py][px] && game.currentPiece.y + py == y && game.currentPiece.x + px == x) {
                            cout << "O"; // Display falling piece
                            isPiece = true;
                        }
                    }
                }
                if (!isPiece) {
                    cout << (game.board[y][x] ? "#" : "."); // Display board state
                }
            }
        }
        cout << endl;
    }
    cout << "Score: " << game.score << endl;
    if (game.gameOver) {
        cout << "Game Over! Press 'X' to Restart or 'Q' to Quit." << endl;
    }
}

// Main game loop
void gameLoop() {
    while (!game.gameOver) {
        if (_kbhit()) {
            switch (_getch()) {
                case 75: movePiece(-1, 0); break; // Left arrow
                case 77: movePiece(1, 0); break;  // Right arrow
                case 80: movePiece(0, 1); break;  // Down arrow
                case 72: rotatePiece(); break;   // **Up arrow to rotate**
            }
        }
        if (GetTickCount() - lastTick >= game.speed) {
            movePiece(0, 1);
            renderBoard();
            lastTick = GetTickCount();
        }
    }
}

// Function to handle restart or quit
void restartOrQuit() {
    while (true) {
        if (_kbhit()) {
            char key = _getch();
            if (key == 'x' || key == 'X') {
                initializeGame();
                game.currentPiece = spawnPiece();
                gameLoop();
            } else if (key == 'q' || key == 'Q') {
                exit(0);
            }
        }
    }
}

// Main function
int main() {
    srand(time(0));
    initializeGame();
    game.currentPiece = spawnPiece();
    gameLoop();
    restartOrQuit();
    return 0;
}
