#include "StudentWorld.h"
#include "GameConstants.h"
#include "Board.h"
#include "Actor.h"
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;

const bool BLUE = true;
const bool RED = false;
const int left = 180;
const int right = 0;
const int up = 90;
const int down = 270;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    m_balance = 0;
    peach = nullptr;
    yoshi = nullptr;
}

StudentWorld::~StudentWorld() {
    cleanUp();
}

int StudentWorld::init()
{
    string board_file = assetPath() + "board0" + (char)(48 + getBoardNumber()) + ".txt";
    Board::LoadResult result = bd.loadBoard(board_file);
    if (result == Board::load_fail_file_not_found)
        cerr << "Could not find board data file\n";
    else if (result == Board::load_fail_bad_format)
        cerr << "Your board was improperly formatted\n";
    else if (result == Board::load_success) {
        cerr << "Successfully loaded board\n";
    }

    //placing Peach
    int x = 0, y = 0;
    for (int gy = 0; gy < BOARD_HEIGHT; gy++)
        for (int gx = 0; gx < BOARD_WIDTH; gx++) {
            if (bd.getContentsOf(gx, gy) == Board::player) {
                x = gx;
                y = gy;
            }
        }

    
    peach = new Avatar(this, IID_PEACH, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, 1);
    yoshi = new Avatar(this, IID_YOSHI, x * SPRITE_WIDTH, y * SPRITE_HEIGHT, 2);
    actors.push_back(new CoinSquare(this, BLUE, IID_BLUE_COIN_SQUARE, x * SPRITE_WIDTH, y * SPRITE_HEIGHT));

    for (int gy = 0; gy < BOARD_HEIGHT; gy++)
        for (int gx = 0; gx < BOARD_WIDTH; gx++) {
            if (bd.getContentsOf(gx, gy) == Board::blue_coin_square) {
                actors.push_back(new CoinSquare(this, BLUE, IID_BLUE_COIN_SQUARE, gx * SPRITE_WIDTH, gy * SPRITE_HEIGHT));
            }
            if (bd.getContentsOf(gx, gy) == Board::red_coin_square) {
                actors.push_back(new CoinSquare(this, RED, IID_RED_COIN_SQUARE, gx * SPRITE_WIDTH, gy * SPRITE_HEIGHT));
            }
            if (bd.getContentsOf(gx, gy) == Board::boo) {
                actors.push_back(new CoinSquare(this, BLUE, IID_BLUE_COIN_SQUARE, gx * SPRITE_WIDTH, gy * SPRITE_HEIGHT));
                actors.push_back(new Boo(this, gx * SPRITE_WIDTH, gy * SPRITE_HEIGHT));
            }
            if (bd.getContentsOf(gx, gy) == Board::bowser) {
                actors.push_back(new CoinSquare(this, BLUE, IID_BLUE_COIN_SQUARE, gx * SPRITE_WIDTH, gy * SPRITE_HEIGHT));
                actors.push_back(new Bowser(this, gx * SPRITE_WIDTH, gy * SPRITE_HEIGHT));
            }
            if (bd.getContentsOf(gx, gy) == Board::star_square)
                actors.push_back(new StarSquare(this, gx * SPRITE_WIDTH, gy * SPRITE_HEIGHT));
            if (bd.getContentsOf(gx, gy) == Board::up_dir_square)
                actors.push_back(new DirectionalSquare(this, up, gx * SPRITE_WIDTH, gy * SPRITE_HEIGHT));
            if (bd.getContentsOf(gx, gy) == Board::down_dir_square)
                actors.push_back(new DirectionalSquare(this, down, gx * SPRITE_WIDTH, gy * SPRITE_HEIGHT));
            if (bd.getContentsOf(gx, gy) == Board::left_dir_square)
                actors.push_back(new DirectionalSquare(this, 180, gx * SPRITE_WIDTH, gy * SPRITE_HEIGHT));
            if (bd.getContentsOf(gx, gy) == Board::right_dir_square)
                actors.push_back(new DirectionalSquare(this, 0, gx * SPRITE_WIDTH, gy * SPRITE_HEIGHT));
            if (bd.getContentsOf(gx, gy) == Board::bank_square)
                actors.push_back(new BankSquare(this, gx * SPRITE_WIDTH, gy * SPRITE_HEIGHT));
            if (bd.getContentsOf(gx, gy) == Board::event_square)
                actors.push_front(new EventSquare(this, gx * SPRITE_WIDTH, gy * SPRITE_HEIGHT));
        }

    m_balance = 0;

    startCountdownTimer(99);  //this placeholder causes timeout after 5 seconds
    return GWSTATUS_CONTINUE_GAME;
}


int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit ESC.
    // Notice that the return value GWSTATUS_NOT_IMPLEMENTED will cause our framework to end the game.

    list<Actor*>::iterator i = actors.begin();
    
    for (; i != actors.end();) {
        if ((*i)->isAlive()) {
            (*i)->doSomething();
            i++;
        }
    }
    peach->doSomething();
    yoshi->doSomething();

    for (i = actors.begin(); i != actors.end();) {
        if (!((*i)->isAlive()) && ((*i)->whatSquare() != 1)) {
            delete* i;
            i = actors.erase(i);
        }
        else
            i++;
    }


    ostringstream oss;
    oss << "P1 roll: " << (peach->getTicks() / 8) << " Stars: " << (peach->getStars()) << " $$: " << peach->getCoins();
    if (peach->ifHasVortex()) {
        oss << " VOR ";
    }
    oss << " | " << "Time: " << timeRemaining() << " | Bank: " << m_balance << " | ";
    oss << "P2 roll: " << (yoshi->getTicks() / 8) << " Stars: " << (yoshi->getStars()) << " $$: " << yoshi->getCoins();
    if (yoshi->ifHasVortex()) {
        oss << " VOR ";
    }
    setGameStatText(oss.str());

    if (timeRemaining() == 0) {
        if (peachWon()) {
            playSound(SOUND_GAME_FINISHED);
            setFinalScore(peach->getStars(), peach->getCoins());
            return GWSTATUS_PEACH_WON;
        }
        else {
            playSound(SOUND_GAME_FINISHED);
            setFinalScore(yoshi->getStars(), yoshi->getCoins());
            return GWSTATUS_YOSHI_WON;
        }
    }
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    if (actors.empty())
        return;
    list<Actor*>::iterator i = actors.begin();
    while (!actors.empty() && i != actors.end()) {
        delete* i;
        i = actors.erase(i);
    }
    if (peach != nullptr)
        delete peach;
    if (yoshi != nullptr)
        delete yoshi;
}

bool StudentWorld::peachWon() {
    if (peach->getStars() > yoshi->getStars())
        return true;
    else if (yoshi->getStars() > peach->getStars())
        return false;

    if (peach->getCoins() > yoshi->getCoins())
        return true;
    else if (yoshi->getCoins() > peach->getCoins())
        return false;

    int won = randInt(0, 1);
    if (won == 0)
        return true;
    else
        return false;
}

Actor* StudentWorld::getRandomSquare() {
    int rand = randInt(1, actors.size());
    list<Actor*>::iterator i = actors.begin();

    for (; i != actors.end(); i++) {
        if (rand == 1 && !((*i)->isVortex())) {
            return *i;
        }
        rand--;
    }
    return nullptr;
}

bool StudentWorld::overlap(int x, int y) {
    int xTop = x + 15; //vortex x top
    int yTop = y + 15; //vortex y top
    list<Actor*>::iterator i = actors.begin();
    for (; i != actors.end(); i++) {
        if ((*i)->isVillain()) {
            int vx = (*i)->getX();
            int vy = (*i)->getY();
            int vxTop = vx + 15;
            int vyTop = vy + 15;
            if (xTop >= vx && xTop <= vxTop && ((y <= vyTop && y >= vy) || (yTop <= vyTop && yTop >= vy))) { //going right
                (*i)->setImpact(true);
                return true;
            }
            else if (x >= vx && x <= vxTop && ((y <= vyTop && y >= vy) || (yTop <= vyTop && yTop >= vy))) {
                (*i)->setImpact(true);
                return true;
            }
            else if (x >= vx && yTop >= vy && x <= vxTop && yTop <= vyTop) {
                (*i)->setImpact(true);
                return true;
            }
        }
    }

    return false;
}

bool StudentWorld::removeSquareAt(int x, int y) {
    list<Actor*>::iterator i = actors.begin();
    for (; i != actors.end();) {
        if ((((*i)->getX() / 16) == x) && (((*i)->getY() / 16) == y) && (*i)->isSquare() && ((*i)->whatSquare() != 1)) {
            delete* i;
            i = actors.erase(i);
            return true;
        }
        i++;
    }
    return false;
}

