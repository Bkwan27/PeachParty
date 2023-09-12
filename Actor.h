#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"
#include "GameConstants.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject {
public:
	Actor(StudentWorld* in, int imageID, int x, int y, int spriteDirection = 0, int graphDepth = 0, double size = 1, bool vortex1 = false) : GraphObject(imageID, x, y, spriteDirection, graphDepth, size)
	{
		alive = true;
		world = in;
		vortex = vortex1;
		impact = false;
	}

	virtual ~Actor() { }

	virtual void doSomething();

	bool isAlive() const {
		return alive;
	}

	void setDead() {
		alive = false;
	}

	StudentWorld* getWorld() {
		return world;
	}

	bool onSquare() const { return (getX() % 16 == 0 && getY() % 16 == 0); };

	virtual bool isAvatar() const {
		return false;
	}

	virtual bool didMove() const {
		return false;
	}

	bool isVortex() {
		return vortex;
	}

	virtual bool isVillain() { return false; }

	virtual void setImpact(bool res) { impact = res; }

	virtual bool isSquare() { return false; }

	virtual int whatSquare() { return 0; }

private:
	bool alive;
	StudentWorld* world;
	bool vortex;
	bool impact;
};

class Square : public Actor {
public:
	Square(StudentWorld* in, int imageID, int x, int y, int sprite, int graph, double size) : Actor(in, imageID, x, y, sprite, graph, size) {}

	virtual ~Square() {}
	virtual void doSomething();

	int landOnSquare(Avatar* player);

	bool isSquare() { return true; }

private:
	
};

class CoinSquare : public Square {
public:
	CoinSquare(StudentWorld* in, bool willAdd, int imageID, int x, int y) : Square(in, imageID, x, y, 0, 1, 1) {
		m_willAdd = willAdd;
	}

	virtual ~CoinSquare() {}

	virtual void doSomething();

	bool isBlue() const {
		return m_willAdd;
	}

	void doActivity(Avatar* player);

private:
	bool m_willAdd;

};

class StarSquare : public Square {
public:
	StarSquare(StudentWorld* in, int x, int y) : Square(in, IID_STAR_SQUARE, x, y, 0, 1, 1) {}

	virtual ~StarSquare() {}

	virtual void doSomething();

	void doActivity(Avatar* player);
};

class DirectionalSquare : public Square {
public:
	DirectionalSquare(StudentWorld* in, int force, int x, int y) : Square(in, IID_DIR_SQUARE, x, y, force, 1, 1) {
		forcingDir = force;
	}

	virtual ~DirectionalSquare() {}

	virtual void doSomething();

	void doActivity(Avatar* player);

private:
	int forcingDir;
};

class BankSquare : public Square {
public:
	BankSquare(StudentWorld* in, int x, int y) : Square(in, IID_BANK_SQUARE, x, y, 0, 1, 1) {
	}

	virtual ~BankSquare() {}

	virtual void doSomething();

	void doActivity(Avatar* player);
private:
};

class EventSquare : public Square {
public:
	EventSquare(StudentWorld* in, int x, int y) : Square(in, IID_EVENT_SQUARE, x, y, 0, 1, 1) {}

	virtual ~EventSquare() {}

	void doSomething();

	void doActivity(Avatar* player, Avatar* other);
private:

};

class DroppingSquare : public Square {
public:
	DroppingSquare(StudentWorld* in, int x, int y) : Square(in, IID_DROPPING_SQUARE, x, y, 0, 1, 1) {}

	virtual ~DroppingSquare() {}

	void doSomething();

	void doActivity(Avatar* player);

	int whatSquare() { return 1; }
};

class Mover : public Actor {
public:
	Mover(StudentWorld* in, int imageID, int x, int y) : Actor(in, imageID, x, y) {
		m_walkDirection = 0;
		m_ticks = 0;
		didWalk = true;
		m_walking = false;
		atStart = true;
	}

	virtual ~Mover() {}

	int getTicks() const {
		return m_ticks;
	}

	void changeTicks(int numTicks) {
		m_ticks += numTicks;
	}

	void setTicks(int num) {
		m_ticks = num;
	}

	virtual bool didMove() const {
		return didWalk;
	}

	void walked(bool result) {
		didWalk = result;
	}

	int getWalkDireciton() const {
		return m_walkDirection;
	}

	void setWalkDirection(int walkDir) {
		m_walkDirection = walkDir;
	}

	bool isWalking() {
		return m_walking;
	}

	void setWalking(bool walking) {
		m_walking = walking;
	}

	bool atBegin() const {
		return atStart;
	}

	void setAtStart(bool res) {
		atStart = res;
	}
	virtual void doSomething();

	bool multipleDir();

	virtual bool teleport();

	bool getRandomDir();

private:
	bool m_walking;
	int m_ticks;
	bool didWalk;
	int m_walkDirection;
	bool atStart;
};
class Villain : public Mover {
public:
	Villain(StudentWorld* in, int imageID, int x, int y) : Mover(in, imageID, x, y) {
		travelDistance = 0;
		pauseCounter = 180;
		activate = true;
		impacted = false;
		dropped = false;
	}

	virtual void doSomething();

	bool getActivate() { return activate; }

	void setActivate(bool res) { activate = res; }

	virtual bool isVillain() { return true; }

	void setImpact(bool res) { impacted = res; }

	bool isDropped() { return dropped; }

	void setDropped(bool res) { dropped = res; }

	virtual bool isBoo() { return false; }

private:
	int travelDistance;
	int pauseCounter;
	bool activate;
	bool impacted;
	bool dropped;
};

class Bowser : public Villain {
public:
	Bowser(StudentWorld* in, int x, int y) : Villain(in, IID_BOWSER, x, y) {

	}

	virtual void doSomething();

	void doActivity(Avatar* player);

private:
};

class Boo : public Villain {
public:
	Boo(StudentWorld* in, int x, int y) : Villain(in, IID_BOO, x, y) {}

	virtual void doSomething();

	void doActivity(Avatar* player, Avatar* other);

	virtual bool isBoo() { return true; }
};

class Vortex : public Actor {
public:
	Vortex(StudentWorld* in, int x, int y, int dir) : Actor(in, IID_VORTEX, x, y, 0, 0, 1, true) {
		direction = dir;
	}

	virtual ~Vortex() {}

	virtual void doSomething();
private:
	int direction;
};

class Avatar : public Mover {
public:
	Avatar(StudentWorld* in, int imageID, int x, int y, int playerNum) : Mover(in, imageID, x, y) {
		m_playerNum = playerNum;
		coins = 0;
		stars = 0;
		onDirectional = false;
		hasVortex = false; //for testing purposes set to true
		wasOnEvent = false;
	}

	virtual ~Avatar() {}

	virtual void doSomething();

	void teleport(int x, int y);

	virtual bool isAvatar() const {
		return true;
	}

	void addCoins(int numCoins) {
		coins += numCoins;
	}

	int setCoins(int num) {
		int coinsDeducted = coins - num;
		coins = num;
		return coinsDeducted;
	}

	int getCoins() const {
		return coins;
	}

	int getStars() const {
		return stars;
	}

	void addStars(int num) {
		stars += num;
	}

	void setStars(int num) {
		stars = num;
	}

	void setOnDirect(bool on) {
		onDirectional = on;
	}

	bool getWasOnEvent() {
		return wasOnEvent;
	}

	void setWasOnEvent(bool res) {
		wasOnEvent = res;
	}

	void giveVortex() {
		hasVortex = true;
	}

	bool ifHasVortex() {
		return hasVortex;
	}

private:
	int m_playerNum;
	int coins;
	int stars;
	bool onDirectional;
	bool hasVortex;
	bool wasOnEvent;
};

#endif // ACTOR_H_
