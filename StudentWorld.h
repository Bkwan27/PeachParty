#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include <list>
#include <string>

class Actor;
class Avatar;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  ~StudentWorld();
  virtual int init();
  virtual int move();
  virtual void cleanUp();

  Board getBoard() const { return bd; }

  StudentWorld* getWorld() { return this; }

  Avatar* getPeach() { return peach;  }
  Avatar* getYoshi() { return yoshi; }

  void addToBank(int num) {
	  m_balance += num;
  }

  int getBalance() const {
	  return m_balance;
  }

  void addToActors(Actor* add) {
	  actors.push_front(add);
  }

  Actor* getRandomSquare();

  bool overlap(int x, int y);

  bool removeSquareAt(int x, int y);
private:
	Board bd;
	std::list<Actor*> actors;
	Avatar* peach;
	Avatar* yoshi;

	int m_balance;

	bool peachWon();
};

#endif // STUDENTWORLD_H_
