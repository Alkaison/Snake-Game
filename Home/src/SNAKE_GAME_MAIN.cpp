/*
    Project Name: Snake Game 
    Language: C++ with Graphics.h Library 
    Team Members: Ganesh, Amit, Vivek, Aaditya 
    Source Code: https://github.com/Alkaison 
*/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <chrono>
#include <fstream>
#include <conio.h>
#include <graphics.h>

using namespace std;

// Global Variables 
const int TB_Spacing = 64;
const int LR_Spacing = 64;
const int MAX_WIDTH = 1504;
const int MAX_HEIGHT = 832;
const int CELL_SIZE = 32;
const int CELLS_X = MAX_WIDTH / CELL_SIZE;      // 47
const int CELLS_Y = MAX_HEIGHT / CELL_SIZE;     // 26
const int MAX_SNAKE_LENGTH = CELLS_X * CELLS_Y; // 1222
const int MAX_WIDTH_BORDER = MAX_WIDTH - 29;    // 1475
const int MAX_HEIGHT_BORDER = MAX_HEIGHT - 61;  // 771
int i = 0, timer = 150, score = 0, choice = 0;
char ch;
char name[25] = "Player";
char timeString[9];
std::chrono::steady_clock::time_point start_time;

// Define Constant KeyWords 
#define ENTER 13
#define TAB 9
#define BKSP 8

struct Point
{
    int x, y;
};

struct Snake
{
    Point body[MAX_SNAKE_LENGTH];
    int length;
    int direction;
};

enum GameState
{
    Playing,
    GameOver
};

GameState gameState = Playing;             // Determines the Game status
void getGraphicsDriver(void);              // starts the graphics driver and resizes it
void generateSnakePoints(Snake &);         // generates initial snake spawn point
void generateFoodPoints(Point &, Snake &); // generate food spawn points randomly
void gameBorder(void);                     // draws the rectangular border of death
void setGameBackground(void);              // set the background texture image
void eraseSnake(Snake &);                  // erase the snake head and body
void moveSnake(Snake &);                   // moves the snake by 1 grid/cell
void drawSnake(Snake);                     // draws the snake head & body
bool checkCollision(Snake);                // check for snake head & body collision
void drawFood(Point &);                    // draw food at new points
bool checkFood(Snake &, Point);            // check for food being EATEN or not
void loading(void);                        // loading bar for switching between screens
void pauseGame(void);                      // stop the game and snake moving
void printStats(int);                      // print or update the score and gameStatus
void updateTimer(Snake);                   // adjust the delay timer as per snake length
int mainMenu(void);                        // draws the main menu of game
void gameFlow();                           // Main Logic of Snake Game Flow
void leaderBoard(void);                    // display the list of players and their scores
void gameInfo(void);                       // display game information screen on how to play
void saveData(void);                       // save player name and score in the file
void clearKeyboardBuffer(void);            // clear extra key pressed events
void getPlayerName(void);                  // get players name for starting the game
void startProgram();                       // Almight Program Starting Logic for MainMenu

int main()
{
    // Load Graphics Driver, Resize the Application 
    getGraphicsDriver();

    // Almight Program Starting Logic for MainMenu 
    startProgram();

    return 0;
}

void getGraphicsDriver()
{
    int gd = DETECT, gm, errorCode;           // graphics initial variables
    char data[] = "C:\\MinGW\\lib\\libbgi.a"; // static file for graphics

    initgraph(&gd, &gm, data); // starting graphics driver

    errorCode = graphresult(); // getting graphics status
    if (errorCode != grOk)     // checking for the error
    {
        // printing the error message and ending the task/program
        closegraph();
        cout << "Graphics Error: " << grapherrormsg(errorCode) << endl;
        system("pause");
        exit(1); // unsuccessful program ending
    }

    initwindow(getmaxwidth(), getmaxheight(), "Snake Game | Made with C++"); // resize the graphics window
    // printing the graphic execution code for error no.
    // cout << "Graphic Code: " << errorCode << endl;  // result '0' if no errors
}

void gameBorder()
{
    setcolor(RED);
    setlinestyle(0, 0, 5);
    rectangle(LR_Spacing - 4, TB_Spacing - 4, MAX_WIDTH - 29, MAX_HEIGHT - 61); // original border line
}

void generateSnakePoints(Snake &snake)
{
    snake.length = 3;
    snake.direction = 0;

    snake.body[0].x = CELLS_X / 2;
    snake.body[0].y = CELLS_Y / 2;

    for (i = 1; i < snake.length; i++)
    {
        snake.body[i].x = snake.body[0].x;
        snake.body[i].y = snake.body[0].y + i;
    }
}

void generateFoodPoints(Point &food, Snake &snake)
{
    bool valid = false;
    while (!valid)
    {
        srand(time(0));
        food.x = rand() % ((CELLS_X - 1) - 2) + 2;
        food.y = rand() % ((CELLS_Y - 2) - 2) + 2;
        valid = true;
        for (i = 0; i < snake.length; i++)
        {
            if (food.x == snake.body[i].x && food.y == snake.body[i].y)
            {
                valid = false;
                break;
            }
        }
    }
    drawFood(food);
}

void drawSnake(Snake snake)
{
    // Draws Head in particular direction 
    if(snake.direction == 0)  // up 
        readimagefile("./assets/snakeHead_0.jpg", snake.body[0].x * CELL_SIZE, snake.body[0].y * CELL_SIZE, (snake.body[0].x + 1) * CELL_SIZE, (snake.body[0].y + 1) * CELL_SIZE);
    else if(snake.direction == 1)  // right 
        readimagefile("./assets/snakeHead_1.jpg", snake.body[0].x * CELL_SIZE, snake.body[0].y * CELL_SIZE, (snake.body[0].x + 1) * CELL_SIZE, (snake.body[0].y + 1) * CELL_SIZE);
    else if(snake.direction == 2)  // down 
        readimagefile("./assets/snakeHead_2.jpg", snake.body[0].x * CELL_SIZE, snake.body[0].y * CELL_SIZE, (snake.body[0].x + 1) * CELL_SIZE, (snake.body[0].y + 1) * CELL_SIZE);
    else if(snake.direction == 3)  // left 
        readimagefile("./assets/snakeHead_3.jpg", snake.body[0].x * CELL_SIZE, snake.body[0].y * CELL_SIZE, (snake.body[0].x + 1) * CELL_SIZE, (snake.body[0].y + 1) * CELL_SIZE);

    // Draws Body Parts 
    for (i = 1; i < snake.length; i++)
    {
        readimagefile("./assets/snakeBody_1.jpg", snake.body[i].x * CELL_SIZE, snake.body[i].y * CELL_SIZE, (snake.body[i].x + 1) * CELL_SIZE, (snake.body[i].y + 1) * CELL_SIZE);
    }
}

void moveSnake(Snake &snake)
{
    for (i = snake.length - 1; i > 0; i--)
    {
        snake.body[i] = snake.body[i - 1];
    }
    switch (snake.direction)
    {
        case 0: // up
            snake.body[0].y--;
            break;
        case 1: // right
            snake.body[0].x++;
            break;
        case 2: // down
            snake.body[0].y++;
            break;
        case 3: // left
            snake.body[0].x--;
            break;
    }
}

bool checkCollision(Snake snake)
{
    Point head = snake.body[0];
    if (head.x < 2 || head.x >= (CELLS_X - 1) || head.y < 2 || head.y >= (CELLS_Y - 2))
    {
        return true; // hit wall
    }
    for (int i = 1; i < snake.length; i++)
    {
        if (snake.body[i].x == head.x && snake.body[i].y == head.y)
        {
            return true; // hit self
        }
    }
    return false;
}

bool checkFood(Snake &snake, Point food)
{
    if (snake.body[0].x == food.x && snake.body[0].y == food.y)
    {
        snake.length++;
        snake.body[snake.length - 1] = snake.body[snake.length - 2];
        return true;
    }
    return false;
}

void drawFood(Point &food)
{
    readimagefile("./assets/snakeFood.jpg", food.x * CELL_SIZE, food.y * CELL_SIZE, (food.x + 1) * CELL_SIZE, (food.y + 1) * CELL_SIZE);
}

void pauseGame()
{
    bool is_paused = false;
    std::chrono::steady_clock::time_point pause_time;
    char ps[] = "Status: Paused  ";

    if (!is_paused)
    {
        pause_time = std::chrono::steady_clock::now();
        is_paused = true;
    }  

    setcolor(4);
    settextstyle(6, HORIZ_DIR, 2);
    outtextxy(1250, 32, ps);

    while (ch != 13)  // Waits for ENTER KEY press 
        ch = getch();

    start_time += std::chrono::steady_clock::now() - pause_time;
    is_paused = false;
    printStats(0);
}

void setGameBackground()
{
    int j;

    for (i = 2; i < CELLS_X - 1; i++)
    {
        for (j = 2; j < CELLS_Y - 2; j++)
        {
            readimagefile("./assets/new_grid_cell-single.jpg", i * 32, j * 32, (i * 32) + 32, (j * 32) + 32);
        }
    }
}

void eraseSnake(Snake &snake)
{
    for (i = 0; i < snake.length; i++)
    {
        readimagefile("./assets/new_grid_cell-single.jpg", snake.body[i].x * CELL_SIZE, snake.body[i].y * CELL_SIZE, (snake.body[i].x + 1) * CELL_SIZE, (snake.body[i].y + 1) * CELL_SIZE);
    }
}

void printStats(int s1)
{
    char points[] = "Score: ";
    char status[] = "Status: Playing ";
    char prefix[] = "Time: ";
    char str[10];

    score += s1;
    sprintf(str, "%d", score);
    strcat(points, str);

    if (gameState == GameOver)
        strcpy(status, "Status: GameOver");

    setcolor(4);
    settextstyle(6, HORIZ_DIR, 2);

    outtextxy(64, 32, points);
    outtextxy(1250, 32, status);

    std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_time - start_time;
    int total_seconds = elapsed_seconds.count();
    int hours = total_seconds / 3600;
    int minutes = (total_seconds / 60) % 60;
    int seconds = total_seconds % 60;

    sprintf(timeString, "%02d:%02d:%02d", hours, minutes, seconds);
    outtextxy(625, 32, prefix);
    outtextxy(700, 32, timeString);
}

void loading()
{
    char load[] = "Loading...";

    cleardevice();
    setcolor(WHITE);

    // Draw loading text
    settextstyle(10, 0, 4);
    outtextxy(550, 465, load);

    // Set line thickness to 3 pixels
    setlinestyle(SOLID_LINE, 0, THICK_WIDTH);

    // Draw loading bar outline
    rectangle(550, 500, 950, 520);

    // Set line thickness to 1 pixel
    setlinestyle(SOLID_LINE, 0, NORM_WIDTH);

    // Set fill style to solid
    setfillstyle(SOLID_FILL, WHITE);

    // Draw loading bar progress
    for (int i = 0; i <= 400; i += 20)
    {
        bar(550, 500, 550 + i, 520);
        delay(50);
    }
}

void updateTimer(Snake snake)
{
    int num;
    num = snake.length % 10;

    if(num == 0 && timer > 10)
        timer -= 20;
}

void gameFlow()
{
    // Snake & Food Struct Object Declaration
    Snake snake;
    Point food;

    // Clear Screen & Draw Border, BackGround, Stats & GameStatus
    cleardevice();
    gameBorder();
    setGameBackground();
    
    // Starts the Timer & Prints the Stats 
    score = 0;
    gameState = Playing;
    start_time = std::chrono::steady_clock::now();
    printStats(0);

    // Generate Initial coordinates of spawn for Food & Snake
    generateSnakePoints(snake);
    generateFoodPoints(food, snake);

    while (gameState == Playing)
    {
        // Get User Key Press and Change Snake Direction 
        if (kbhit())
        {
            ch = getch();

            if(ch == 0 || ch == -32)  // holds the 224 for ARROW KEYS ASCII 
                ch = getch();
            
            switch (ch)
            {
                case 'w':  // UP 
                case 'W':
                case 72:
                    if (snake.direction != 2)
                        snake.direction = 0;
                    break;
                case 'd':  // RIGHT 
                case 'D':
                case 77:
                    if (snake.direction != 3)
                        snake.direction = 1;
                    break;
                case 's':  // DOWN
                case 'S':
                case 80:
                    if (snake.direction != 0)
                        snake.direction = 2;
                    break;
                case 'a':  // LEFT 
                case 'A':
                case 75:
                    if (snake.direction != 1)
                        snake.direction = 3;
                    break;
                case 27: // ASCII value for ESC key
                    pauseGame();
                    break;
            }
        }

        // Erase, Shift, Draw the Snake
        eraseSnake(snake);
        moveSnake(snake);
        drawSnake(snake);

        // Check for Snake Collision
        if (checkCollision(snake))
        {
            gameState = GameOver;
            clearKeyboardBuffer();
        }

        // Check for Food EAT
        if (checkFood(snake, food))
        {
            printStats(5);
            updateTimer(snake);
            generateFoodPoints(food, snake);
        }
        else
        {
            drawFood(food);
            printStats(0);
        }

        delay(timer);
    }

    printStats(0);

    // Draw GameOver Screen 
    setlinestyle(0, 0, 3);
    readimagefile("./assets/gameOver.jpg", 550, 250, 1000, 500);
    rectangle(550, 250, 1000, 500);

    // Save Player Data 
    saveData();

    // Pause for 0.5s 
    delay(500);
    getch();
}

int mainMenu()
{
    setbkcolor(2);
    cleardevice();
    setcolor(WHITE);
    setlinestyle(0, 0, 5);
    
    readimagefile("./assets/snakeGameUI.jpg", LR_Spacing - 4, TB_Spacing - 4, MAX_WIDTH - 29, MAX_HEIGHT - 61); // background image
    rectangle(LR_Spacing - 6, TB_Spacing - 6, MAX_WIDTH - 26, MAX_HEIGHT - 58); // white border line 

    setlinestyle(0, 0, 1);
    clearmouseclick(WM_LBUTTONDOWN);

    /* rectangle(1100, 590, 1405, 718); // play button
    rectangle(615, 595, 915, 723);   // score button
    rectangle(130, 595, 430, 723);   // exit button
    rectangle(1300, 90, 1405, 195);  // info button */

    while (1) {
        // check for mouse click
        if (ismouseclick(WM_LBUTTONDOWN))
        {
            // get mouse click coordinates
            int x = mousex();
            int y = mousey();

            // check if mouse click is within button border 
            if (x >= 1100 && x <= 1405 && y >= 590 && y <= 718)  // play button 
            {
                cout << "Play Button Clicked" << endl;
                return 1;
            }
            else if(x >= 615 && x <= 915 && y >= 595 && y <= 723)  // score button
            {
                cout << "Score Button Clicked" << endl;
                return 2;
            }
            else if(x >= 130 && x <= 430 && y >= 595 && y <= 723)  // exit button
            {
                cout << "Exit Button Clicked" << endl;
                return 3;
            }
            else if(x >= 1300 && x <= 1405 && y >= 90 && y <= 195)  // info button
            {
                cout << "Info Button Clicked" << endl;
                return 4;
            }
        }

        clearmouseclick(WM_LBUTTONDOWN);
        delay(100);
   }
}

void leaderBoard()
{
    char getName[30], getTime[30], getScore[30];
    int x = 120, y = 180;
    i = 1;
    bool end = false;

    // Text to print on scoreboard 
    char scoreTitle[] = "Snake Game Leaderboard";
    char fileError[] = "Error: File Not Found. Please play the game once to activate leaderboard.";
    char boardTitle1[] = "Player Names";
    char boardTitle2[] = "Score";
    char boardTitle3[] = "Timing";

    setbkcolor(2);
    cleardevice();

    setcolor(WHITE);
    settextstyle(9, 0, 5);
    outtextxy(500, 40, scoreTitle);
    line(490, 85, 1140, 85);

    fstream obj;
    obj.open("./leaderboard.txt", ios::in);

    if (!obj)
    {
        // Print Error if leaderboard.txt file not found at directory 
        setcolor(RED);
        settextstyle(10, 0, 3);
        outtextxy(125, 200, fileError);
    }
    else
    {
        setlinestyle(0, 0, 5);
        line(100, 175, 700, 175);  // header subtractor line 
        line(490, 125, 490, 725);  // straight lines
        line(580, 125, 580, 725);  // straight lines 
        rectangle(100, 125, 700, 725);  // main rectangle 

        settextstyle(10, 0, 3);
        outtextxy(225, 140, boardTitle1);
        outtextxy(500, 140, boardTitle2);
        outtextxy(600, 140, boardTitle3);
    
        settextstyle(10, 0, 2);
        while (!end)
        {
            x = 120;
            y += 20;

            obj >> getName >> getScore >> getTime;

            if (obj.eof())
            {
                end = true;
                break;
            }
            else if (i > 25)
                break;

            outtextxy(x, y, getName);
            x += 400;
            outtextxy(x, y, getScore);
            x += 70;
            outtextxy(x, y, getTime);
            i++;
        }

        if (i > 25 && !end)
        {
            settextstyle(10, 0, 2);
            y = 180;
            while (!end)
            {
                x = 820;
                y += 20;

                if (i != 26)
                        obj >> getName >> getScore >> getTime;

                if (obj.eof())
                {
                        end = true;
                        break;
                }
                else if (i > 50)
                        break;

                outtextxy(x, y, getName);
                x += 400;
                outtextxy(x, y, getScore);
                x += 70;
                outtextxy(x, y, getTime);

                i++;
            }

            if (i > 26)
            {
                setlinestyle(0, 0, 5);
                line(800, 175, 1400, 175);      // header subtractor line
                line(1190, 125, 1190, 725);     // straight lines
                line(1280, 125, 1280, 725);     // straight lines
                rectangle(800, 125, 1400, 725); // main rectangle

                settextstyle(10, 0, 3);
                outtextxy(925, 140, boardTitle1);
                outtextxy(1200, 140, boardTitle2);
                outtextxy(1300, 140, boardTitle3);
            }
        }
    }
    obj.close();
    getch();
}

void gameInfo()
{
	setbkcolor(9);
    cleardevice();
	setcolor(15);
    setlinestyle(0, 0, 5);
    readimagefile("./assets/infoScreenUI.jpg", LR_Spacing - 4, TB_Spacing - 4, MAX_WIDTH - 29, MAX_HEIGHT - 61);
    rectangle(LR_Spacing - 4, TB_Spacing - 4, MAX_WIDTH - 29, MAX_HEIGHT - 61); // original border line
	getch();
    setbkcolor(2);
}

void clearKeyboardBuffer() {
   while (kbhit()) {
        getch();
   }
}

void saveData()
{
    fstream obj;

    obj.open("./leaderboard.txt", ios::app);

    obj << name << " " << score << " " << timeString << endl;

    obj.close();
}

void getPlayerName()
{
    cleardevice();
    char hk = '\0';
    char backSpace[] = "  ";
    i = 0;

    setcolor(WHITE);
    setlinestyle(0, 0, 5);

    readimagefile("./assets/userNameDisplay.jpg", LR_Spacing - 4, TB_Spacing - 4, MAX_WIDTH - 29, MAX_HEIGHT - 61); // background image
    rectangle(LR_Spacing - 6, TB_Spacing - 6, MAX_WIDTH - 26, MAX_HEIGHT - 58); // white border line 

    fflush(stdin);
    settextstyle(10, 0, 3);
    // outtextxy(500, 300, "Enter your Name: ");

    while (1)
    {
        hk = getch();

        if (hk == ENTER || hk == TAB)
        {
            name[i] = '\0';
            
            if(name[0] == '\0')
                strcpy(name, "Player");
            
            gameFlow();
            break;
        }
        else if (hk == BKSP)
        {
            if (i > 0)
            {
                i--;
                outtextxy(475 + i * 20, 335, backSpace);
            }
        }
        else if (hk == 27)
        {
            startProgram();
            break;
        }
        else
        {
            if (hk == ' ')
                hk = '_';
            else if(!isalpha(hk))
                continue;

            if(i > 25)
                continue;

            // hk = tolower(hk);
            name[i++] = hk;
            outtextxy(475 + (i - 1) * 20, 335, &hk);
        }
    }
}

void startProgram()
{
    while(1)
    {
        // Add Colors 
        setcolor(2);
        setbkcolor(2);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);

        // Draws Game Main Menu 
        choice = mainMenu();

        // Clears Old Key Pressed Buffers 
        clearKeyboardBuffer();

        switch (choice)
        {
            case 1:
                getPlayerName();
                break;
            case 2:
                leaderBoard();
                break;
            case 3:
                closegraph();
                exit(0);
                break;
            case 4:
                gameInfo();
                break;
            default:
                cout << "Error: Got unexpected event. Please try again." << endl; 
        }
        loading();
    }
}
