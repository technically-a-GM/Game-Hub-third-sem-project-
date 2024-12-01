

#include <bits/stdc++.h>
#include <conio.h>
#include <windows.h>
using namespace std;

class SnakeGame {
    int gameHeight = 25;
    int gameWidth = 100;
    int moveUp = 0;
    int moveDown = 0;
    int moveLeft = 0;
    int moveRight = 0;
    int score;
    int gameOver = 0;
    int gameSpeed = 100;
    int foodCount;
    int snakeX;
    int snakeY;
    int foodX;
    int foodY;
    char playerName[30];

    struct Segment {
        int posX, posY;
        struct Segment* next;
        struct Segment* prev;
    };

    struct Segment* head = NULL;

    void setCursorPosition(int x, int y) {
    COORD position;
    position.X = static_cast<SHORT>(x); // Explicit cast to SHORT
    position.Y = static_cast<SHORT>(y); // Explicit cast to SHORT
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
}


    void updateScore() {
        setTextColor(7);
        setCursorPosition(105, 0);
        cout << playerName << "'s SCORE : " << score * 100 << endl;
    }

    void setTextColor(int colorCode) {
        HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(console, colorCode);
    }

public:
    void drawBorder() {
        setTextColor(11);
        for (int i = 0; i <= gameWidth; i++) {
            setCursorPosition(i, 0);
            cout << "#";
            setCursorPosition(i, gameHeight);
            cout << "#";
        }
        for (int i = 0; i <= gameHeight; i++) {
            setCursorPosition(0, i);
            cout << "#";
            setCursorPosition(gameWidth, i);
            cout << "#";
        }
    }

    void resetMovementFlags() {
        moveUp = 0;
        moveDown = 0;
        moveLeft = 0;
        moveRight = 0;
    }

    void initializeGame() {
        score = 0;
        gameSpeed = 100;
        gameOver = 0;
        drawBorder();
        resetMovementFlags();
        updateScore();

        head = new Segment;
        head->posX = gameWidth / 2;
        head->posY = gameHeight / 2;
        head->next = NULL;
        head->prev = NULL;
        snakeX = gameWidth / 2;
        snakeY = gameHeight / 2;

    regenerateFoodX:
        foodX = rand() % gameWidth;
        if (foodX == 0 || foodX == gameWidth) goto regenerateFoodX;

    regenerateFoodY:
        foodY = rand() % gameHeight;
        if (foodY == 0 || foodY == gameHeight) goto regenerateFoodY;
    }

    void drawSnake(struct Segment* headSegment) {
        struct Segment* current = headSegment;
        while (current != NULL) {
            setCursorPosition(current->posX, current->posY);
            cout << "O";
            current = current->next;
        }
    }

    void eraseSnake(struct Segment* headSegment) {
        struct Segment* current = headSegment;
        while (current != NULL) {
            setCursorPosition(current->posX, current->posY);
            cout << " ";
            current = current->next;
        }
    }

    void displayFood() {
        setTextColor(14);
        drawSnake(head);
        setCursorPosition(foodX, foodY);
        cout << "@";
        Sleep(gameSpeed);
        eraseSnake(head);
    }

    void handleInput() {
        char keyPress;
        if (kbhit()) {
            keyPress = getch();
            switch (keyPress) {
            case 72:
                if (!moveDown) {
                    resetMovementFlags();
                    moveUp = 1;
                }
                break;
            case 80:
                if (!moveUp) {
                    resetMovementFlags();
                    moveDown = 1;
                }
                break;
            case 75:
                if (!moveRight) {
                    resetMovementFlags();
                    moveLeft = 1;
                }
                break;
            case 77:
                if (!moveLeft) {
                    resetMovementFlags();
                    moveRight = 1;
                }
                break;
            case 'p':
                setCursorPosition(20, gameHeight / 2);
                cout << "Game Paused. Press any key to continue...";
                system("pause");
                system("cls");
                drawBorder();
                updateScore();
                break;
            default:
                break;
            }
        }
    }

    void moveSnake() {
        if (moveUp) snakeY--;
        else if (moveDown) snakeY++;
        else if (moveLeft) snakeX--;
        else if (moveRight) snakeX++;
    }

    void updateSnakeSegments(struct Segment* headSegment, int newX, int newY) {
        struct Segment* current = headSegment;
        struct Segment* prev = headSegment;

        while (current->next != NULL) {
            prev = current;
            current = current->next;
        }
        while (prev != headSegment) {
            current->posX = prev->posX;
            current->posY = prev->posY;
            prev = prev->prev;
            current = current->prev;
        }
        current->posX = prev->posX;
        current->posY = prev->posY;
        prev->posX = newX;
        prev->posY = newY;
    }

    void checkCollision() {
        if (snakeX == gameWidth || snakeX == 0 || snakeY == gameHeight || snakeY == 0) {
            gameOver = 1;
        }

        struct Segment* current = head->next;
        while (current != NULL) {
            if (snakeX == current->posX && snakeY == current->posY) {
                gameOver = 1;
                break;
            }
            current = current->next;
        }

        if (snakeX == foodX && snakeY == foodY) {
            struct Segment* newSegment = new Segment;
            newSegment->next = NULL;
            newSegment->prev = NULL;

            struct Segment* tail = head;
            while (tail->next != NULL) {
                tail = tail->next;
            }
            tail->next = newSegment;
            newSegment->prev = tail;

            regenerateFood();
            gameSpeed -= 2;
            score++;
            updateScore();
        }

        updateSnakeSegments(head, snakeX, snakeY);
    }

    void regenerateFood() {
    regenerateFoodX:
        foodX = rand() % gameWidth;
        if (foodX == 0 || foodX == gameWidth) goto regenerateFoodX;
    regenerateFoodY:
        foodY = rand() % gameHeight;
        if (foodY == 0 || foodY == gameHeight) goto regenerateFoodY;
    }

    void gameLoop() {
        char playAgain;
        do {
            initializeGame();
            drawBorder();
            while (!gameOver) {
                displayFood();
                handleInput();
                moveSnake();
                checkCollision();
            }
            playAgain = displayGameOverScreen();
        } while (tolower(playAgain) == 'y');
    }

    char displayGameOverScreen() {
        setCursorPosition(gameWidth / 2 - 5, gameHeight / 2 - 4);
        cout << "GAME OVER";
        setCursorPosition(gameWidth / 2 - 15, gameHeight / 2 - 2);
        cout << playerName << " Scored : " << score * 100;

        saveScore();

        setCursorPosition(gameWidth / 2 - 15, gameHeight / 2 + 4);
        cout << "Play Again? (Y/N): ";
        char choice;
        cin >> choice;
        system("cls");
        return choice;
    }

    void saveScore() {
        ofstream outFile("scores.txt", ios::app);
        outFile << playerName << " " << score * 100 << endl;
        outFile.close();
    }

    void displayInstructions() {
        system("cls");
        cout << "Welcome to Snake Game!" << endl;
        cout << "Use arrow keys to control the snake." << endl;
        cout << "Press 'P' to pause the game." << endl;
        cout << "Avoid colliding with walls or the snake's body." << endl;
        cout << "Eat '@' to grow and score points." << endl;
        cout << "Enter your name: ";
        
        cin >> playerName;
        system("cls");
    }
};

int snakeGame() {
    SnakeGame game;
    game.displayInstructions();
    game.gameLoop();
    return 0;
}

//------------------------------------------------------------------------------------------------------------------------------------

// Mine Sweeper


const int DIMX = 6;
const int DIMY = 6;
const int MINES = 6;
const int MINE = -1;
const char COVERED = 'X';
const char UNCOVERED = ' ';
const char FLAG = 'F';

int state[DIMX][DIMY];
char display[DIMX][DIMY];
int score = 0; // Variable to keep track of the score

vector<pair<int, int>> moveHistory; // Stores the history of moves (x, y)

void init();
int countSurroundingMines(int x, int y);
void reveal(int x, int y);
void player(bool & playing);
void setFlag();
void uncover();
void print();
void cheat();
bool isWin();
bool isLose();
bool playNewGame();

int minesweeperGame() {
    cout << "Welcome to MineSweeper!" << endl;
      bool play = true;
    init();
    bool playing = true;
    while (playing) {
        print();
        cout << "Current Score: " << score << endl;
        player(play);
        if(!play){
            play = true;
            break;
        }

        if (isWin()) {
            cout << "You Win!" << endl;
            cout << "Final Score: " << score << endl;
            playing = false;
        } else if (isLose()) {
            cout << "You Lose!" << endl;
            cout << "Final Score: " << score << endl;
            playing = false;
        }

        if (!playing) {
            playing = playNewGame();
            if (playing) {
                init();
            }
        }
    }
    cout << "Exiting Minesweeper!" << endl;
    return 0;
}

void init() {
    cout << "New Game!" << endl;
    score = 0;
    moveHistory.clear(); // Clear move history

    for (int y = 0; y < DIMY; y++) {
        for (int x = 0; x < DIMX; x++) {
            display[x][y] = COVERED;
            state[x][y] = 0; // Reset state
        }
    }

    srand(time(0));
    for (int i = 0; i < MINES; i++) {
        bool placed = false;
        while (!placed) {
            int x = rand() % DIMX;
            int y = rand() % DIMY;
            if (state[x][y] != MINE) {
                placed = true;
                state[x][y] = MINE;
            }
        }
    }

    for (int y = 0; y < DIMY; y++) {
        for (int x = 0; x < DIMX; x++) {
            if (state[x][y] != MINE) {
                state[x][y] = countSurroundingMines(x, y);
            }
        }
    }
}

void reveal(int x, int y) {
    stack<pair<int, int>> toReveal; // Stack for tiles to reveal
    toReveal.push(make_pair(x, y));

    while (!toReveal.empty()) {
        pair<int, int> current = toReveal.top();
        toReveal.pop();

        int cx = current.first;
        int cy = current.second;

        if (cx >= 0 && cx < DIMX && cy >= 0 && cy < DIMY && display[cx][cy] == COVERED) {
            display[cx][cy] = UNCOVERED;
            score += 10;

            if (state[cx][cy] == 0) {
                // Add neighboring tiles to stack
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        if (dx != 0 || dy != 0) {
                            int nx = cx + dx;
                            int ny = cy + dy;
                            if (nx >= 0 && nx < DIMX && ny >= 0 && ny < DIMY) {
                                toReveal.push(make_pair(nx, ny));
                            }
                        }
                    }
                }
            }
        }
    }

    // Log the move
    moveHistory.push_back(make_pair(x, y));
}


int countSurroundingMines(int x, int y) {
    if (state[x][y] == MINE) {
        return MINE;
    }
    int num = 0;

    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            int nx = x + dx, ny = y + dy;
            if (nx >= 0 && nx < DIMX && ny >= 0 && ny < DIMY && state[nx][ny] == MINE) {
                num++;
            }
        }
    }
    return num;
}

// Remaining functions like `player`, `setFlag`, `print`, `cheat`, `isWin`, `isLose` remain unchanged

// Other functions remain largely unchanged; the reveal() function is now iterative,
// and flaggedPositions and minePositions are used as needed in the respective functions.
bool isWin() {
    bool win = true;
    for (int y = 0; y < DIMY; y++) {
        for (int x = 0; x < DIMX; x++) {
            win &= ((display[x][y] == UNCOVERED && state[x][y] != MINE) ||
                    ((display[x][y] == COVERED || display[x][y] == FLAG) && state[x][y] == MINE));
        }
    }
    return win;
}

bool isLose() {
    for (int y = 0; y < DIMY; y++) {
        for (int x = 0; x < DIMX; x++) {
            if (display[x][y] == UNCOVERED && state[x][y] == MINE) {
                return true;
            }
        }
    }
    return false;
}

void player(bool & playing) {
    cout << "Commands: R => Show, F => Flag, C => Cheat, N => NewGame, E => Exit" << endl;
    bool selected = false;
    while (!selected) {
        char choice;
        cin >> choice;
        if (choice == 'R' || choice == 'r') {
            uncover();
            selected = true;
        } else if (choice == 'F' || choice == 'f') {
            setFlag();
            selected = true;
        } else if (choice == 'C' || choice == 'c') {
            cheat();
        } else if (choice == 'N' || choice == 'n') {
            init();
            selected = true;
        } 
        else if (choice == 'E' || choice == 'e') {
            playing = false;
            return;
        }
        else {
            cout << "Invalid Choice!" << endl;
        }
    }
}

void uncover() {
    bool selected = false;
    while (!selected) {
        int x;
        int y;
        cout << "Input X (1 - " << (DIMX) << ")" << endl;
        cin >> x;
        cout << "Input Y (1 - " << (DIMY) << ")" << endl;
        cin >> y;
        x--;
        y--;
        if (x >= 0 && x < DIMX && y >= 0 && y < DIMY) {
            reveal(x, y); // call recursive reveal algorithm
            selected = true;
        } else {
            cout << "(X,Y) Values out of range!" << endl;
        }
    }
}

void setFlag() {
    bool selected = false;
    while (!selected) {
        int x;
        int y;
        cout << "Input X (1 - " << (DIMX) << ")" << endl;
        cin >> x;
        cout << "Input Y (1 - " << (DIMY) << ")" << endl;
        cin >> y;
        x--;
        y--;
        if (x >= 0 && x < DIMX && y >= 0 && y < DIMY) {
            display[x][y] = FLAG;
            score += 5; // Increment score for placing a flag
            selected = true;
        } else {
            cout << "(X,Y) Values out of range!" << endl;
        }
    }
}

void cheat() {
    cout << "Only this once!!" << endl;
    cout << "   ";
    for (int x = 0; x < DIMX; x++) {
        cout << (x + 1) << " ";
    }
    cout << endl
         << "";
    for (int x = 0; x < DIMX; x++) {
        cout << "";
    }
    cout << endl;
    for (int y = 0; y < DIMY; y++) {
        cout << (y + 1) << "  ";
        for (int x = 0; x < DIMX; x++) {
            if (state[x][y] == MINE) {
                cout << "@ ";
            } else {
                if (state[x][y] == 0) {
                    cout << "  ";
                } else {
                    cout << state[x][y] << " ";
                }
            }
        }
        cout << endl;
    }
}

void print() {
    cout << "   ";
    for (int x = 0; x < DIMX; x++) {
        cout << (x + 1) << " ";
    }
    cout << endl
         << "";
    for (int x = 0; x < DIMX; x++) {
        cout << "";
    }
    cout << endl;
    for (int y = 0; y < DIMY; y++) {
        cout << (y + 1) << "  ";
        for (int x = 0; x < DIMX; x++) {
            if (display[x][y] == COVERED || display[x][y] == 'F') { // is it tile still uncovered
                cout << display[x][y] << " ";
            } else { // display the state
                if (state[x][y] == MINE) {
                    cout << "@ ";
                } else {
                    if (state[x][y] == 0) {
                        cout << "  ";
                    } else {
                        cout << state[x][y] << " ";
                    }
                }
            }
        }

        cout << endl;
    }
}

bool playNewGame() {
    while (true) {
        char choice;
        cout << "Play another game? (Y/N): ";
        cin >> choice;

        if (choice == 'Y' || choice == 'y') {
            return true; // Start a new game
        } else if (choice == 'N' || choice == 'n') {
            return false; // Exit the game
        } else {
            cout << "Invalid input! Please enter 'Y' for Yes or 'N' for No." << endl;
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------

//Maze Runner




const int ROWS = 10;
const int COLS = 20;

// Symbols for the maze
const char WALL = '*';
const char PATH = ' ';
const char PLAYER = 'P';
const char GOAL = 'G';

// Maze array
char maze[ROWS][COLS];

// Player's position
int playerX, playerY;

// points linked list
struct Node {
    int points;
    Node* next;
};
Node* head = nullptr;

// Player stats
int wallCollisions = 0;
int points = 100;

void addpoints(int value) {
    points += value;

    // Add points to the linked list
    Node* newNode = new Node{points, head};
    head = newNode;
}

void displaypointss() {
    cout << "points History: ";
    Node* current = head;
    while (current) {
        cout << current->points << " ";
        current = current->next;
    }
    cout << endl;
}

void generateMaze() {
    srand(time(0));

    // Initialize the maze with walls
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            maze[i][j] = WALL;
        }
    }

    // Create a guaranteed path from the start to the goal
    int x = 1, y = 1;
    maze[x][y] = PATH;

    while (x < ROWS - 2 || y < COLS - 2) {
        if (x < ROWS - 2 && (rand() % 2 == 0 || y == COLS - 2)) {
            x++;
        } else if (y < COLS - 2) {
            y++;
        }
        maze[x][y] = PATH;
    }

    // Randomly add some additional paths for complexity
    for (int i = 1; i < ROWS - 1; i++) {
        for (int j = 1; j < COLS - 1; j++) {
            if (maze[i][j] == WALL && rand() % 4 == 0) {
                maze[i][j] = PATH;
            }
        }
    }

    // Place the player and the goal
    playerX = 1;
    playerY = 1;
    maze[playerX][playerY] = PLAYER;

    maze[ROWS - 2][COLS - 2] = GOAL;
}

void displayMaze() {
    system("cls"); // Clear the console

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            cout << maze[i][j];
        }
        cout << endl;
    }
    cout << "Current points: " << points << endl;
    cout << "Wall Collisions: " << wallCollisions << endl;
}

bool movePlayer(int dx, int dy) {
    int newX = playerX + dx;
    int newY = playerY + dy;

    // Check if the move is valid
    if (maze[newX][newY] == WALL) {
        wallCollisions++;
        addpoints(-5); // Decrease points
        if (wallCollisions > 10) {
            displayMaze();
            cout << "Game Over! You collided with the wall too many times!" << endl;
            return true;
        }
        return false;
    }

    // Check if the goal is reached
    if (maze[newX][newY] == GOAL) {
        maze[playerX][playerY] = PATH; // Clear previous position
        playerX = newX;
        playerY = newY;
        maze[playerX][playerY] = PLAYER;
        displayMaze();
        cout << "Congratulations! You reached the goal!" << endl;
        return true;
    }

    // Move the player
    maze[playerX][playerY] = PATH; // Clear previous position
    playerX = newX;
    playerY = newY;
    maze[playerX][playerY] = PLAYER;

    addpoints(10); // Increase points for a successful move
    return false;
}

void playGame() {
    while (true) {
        displayMaze();

        // Listen for arrow key inputs
        if (GetAsyncKeyState(VK_UP) & 0x8000) {
            if (movePlayer(-1, 0)) break; // Move up
        }
        if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
            if (movePlayer(1, 0)) break; // Move down
        }
        if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
            if (movePlayer(0, -1)) break; // Move left
        }
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
            if (movePlayer(0, 1)) break; // Move right
        }

        Sleep(150); // Slower player movement
    }
}

int mazeRunnerGame() {
    generateMaze();
    addpoints(0); // Initialize points tracking
    playGame();

    // Cleanup linked list
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }

    return 0;
}

void mainMenu() {
    while (true) {
        cout << "Welcome to the Game Hub!" << endl;
        cout << "1. Snake Game" << endl;
        cout << "2. Minesweeper" << endl;
        cout << "3. Maze Runner" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                snakeGame();
                break;
            case 2:
                minesweeperGame();
                break;
            case 3:
                mazeRunnerGame();
                break;
            case 4:
                cout << "Exiting the Game Hub. Goodbye!" << endl;
                return;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
    }
}

// ------------ Main Function ------------
int main() {
    mainMenu();
    return 0;
}
