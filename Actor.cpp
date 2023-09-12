#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>
#include <list>
using namespace std;

void Actor::doSomething() {}

void Square::doSomething() {
	if (!isAlive()) {
		return;
	}
}

int Square::landOnSquare(Avatar* player) {
	Avatar* peach = player;
	if (peach->getX() == getX() && peach->getY() == getY() && peach->onSquare() && !(peach->isWalking())) {
		if (peach->didMove()) {
			return 0; //if it is a newPlayer
		}
		else
			return 1; //if it is not a new Player but on Square
	}
	else if (peach->getX() == getX() && peach->getY() == getY() && peach->onSquare() && (peach->isWalking()))
		return 2; //if it is passing over the Square
	return 3; //if it is not on the Square
}

void CoinSquare::doActivity(Avatar* player) {
	if (player->atBegin() && player->didMove()) {
		player->addCoins(3);
		getWorld()->playSound(SOUND_GIVE_COIN);
		player->walked(false);
	}

	if (landOnSquare(player) == 0 && isBlue()) {
		player->addCoins(3);
		getWorld()->playSound(SOUND_GIVE_COIN);
		player->walked(false);
	}
	else if (landOnSquare(player) == 0 && !isBlue()) {
		if (player->getCoins() < 3)
			player->setCoins(0);
		else
			player->addCoins(-3);
		getWorld()->playSound(SOUND_TAKE_COIN);
		player->walked(false);
	}
}

void CoinSquare::doSomething() {
	if (!isAlive())
		return;
	Avatar* peach = getWorld()->getPeach();
	Avatar* yoshi = getWorld()->getYoshi();
	doActivity(peach);
	doActivity(yoshi);
}

void DirectionalSquare::doActivity(Avatar* player) {
	if (landOnSquare(player) < 3) {
		player->setWalkDirection(forcingDir);
		if (forcingDir == left)
			player->setDirection(left);
		else
			player->setDirection(right);
		player->setOnDirect(true);
	}
}

void DirectionalSquare::doSomething() {
	Avatar* peach = getWorld()->getPeach();
	Avatar* yoshi = getWorld()->getYoshi();
	if (!(isAlive()))
		return;
	doActivity(peach);
	doActivity(yoshi);
}

void StarSquare::doActivity(Avatar* player) {
	if (landOnSquare(player) == 0 || landOnSquare(player) == 2) {
		if (player->getCoins() < 20)
			return;
		player->addCoins(-20);
		player->addStars(1);
		getWorld()->playSound(SOUND_GIVE_STAR);
	}
}

void StarSquare::doSomething() {
	Avatar* peach = getWorld()->getPeach();
	Avatar* yoshi = getWorld()->getYoshi();
	if (!(isAlive()))
		return;
	doActivity(peach);
	doActivity(yoshi);
}

void BankSquare::doActivity(Avatar* player) {
	if (landOnSquare(player) == 0) {
		player->addCoins(getWorld()->getBalance());
		getWorld()->addToBank(-(getWorld()->getBalance()));
		getWorld()->playSound(SOUND_WITHDRAW_BANK);
		player->walked(false);
	}
	else if (landOnSquare(player) == 2) {
		if (player->getCoins() < 5)
			getWorld()->addToBank(player->setCoins(0));
		else {
			player->addCoins(-5);
			getWorld()->addToBank(5);
		}
		getWorld()->playSound(SOUND_DEPOSIT_BANK);
	}
}

void BankSquare::doSomething() {
	Avatar* peach = getWorld()->getPeach();
	Avatar* yoshi = getWorld()->getYoshi();
	if (!(isAlive()))
		return;
	doActivity(peach);
	doActivity(yoshi);
}

void EventSquare::doActivity(Avatar* player, Avatar* other) {
	int rand = randInt(1, 3);
	if ((player->getWasOnEvent()))
		return;
	switch (rand) {
	case 1:
		player->teleport(-1, -1);
		getWorld()->playSound(SOUND_PLAYER_TELEPORT);
		player->walked(true);
		player->setWasOnEvent(true);
		break;
	case 2:
		if (!(player->getWasOnEvent())) {
			player->teleport(other->getX(), other->getY());
			getWorld()->playSound(SOUND_PLAYER_TELEPORT);
			player->walked(true);
			player->setWasOnEvent(true);
		}
		//if (!(peach->getWasOnEvent()))
		break;
	case 3:
		if (!player->ifHasVortex()) {
			player->giveVortex();
			getWorld()->playSound(SOUND_GIVE_VORTEX);
			player->walked(false);
		}
		break;
	}
}

void EventSquare::doSomething() {
	Avatar* peach = getWorld()->getPeach();
	Avatar* yoshi = getWorld()->getYoshi();
	if (!(isAlive()))
		return;
	if (landOnSquare(peach) == 0) {
		doActivity(peach, yoshi);
	}

	if (landOnSquare(yoshi) == 0) {
		doActivity(yoshi, peach);
	}
}

void DroppingSquare::doActivity(Avatar* player) {
	int rand = randInt(1, 2);
	if (rand == 1) {
		if (player->getCoins() < 10)
			player->setCoins(0);
		else
			player->addCoins(-10);
	}
	else {
		if (player->getStars() >= 1)
			player->addStars(-1);
	}
}

void DroppingSquare::doSomething() {
	Avatar* peach = getWorld()->getPeach();
	Avatar* yoshi = getWorld()->getYoshi();

	if (landOnSquare(peach) == 0) {
		doActivity(peach);
		getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
		peach->walked(false);
	}

	if (landOnSquare(yoshi) == 0) {
		doActivity(yoshi);
		getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
		yoshi->walked(false);
	}
}

bool Mover::multipleDir() {
	int x = getX();
	int y = getY();
	Board game = getWorld()->getBoard();
	int newX1 = 0, newX2 = 0, newY1 = 0, newY2 = 0;
	int possible = 0;
	if (m_walkDirection == left) {
		newY1 = y + 16;
		newY2 = y - 16;
		newX1 = x - 16;
		if (game.getContentsOf(x / 16, newY1 / 16) != Board::empty)
			possible++;
		if (game.getContentsOf(x / 16, newY2 / 16) != Board::empty)
			possible++;
		if (game.getContentsOf(newX1 / 16, y / 16) != Board::empty)
			possible++;
	}

	if (m_walkDirection == right) {
		newY1 = y + 16;
		newY2 = y - 16;
		newX1 = x + 16;
		if (game.getContentsOf(x / 16, newY1 / 16) != Board::empty)
			possible++;
		if (game.getContentsOf(x / 16, newY2 / 16) != Board::empty)
			possible++;
		if (game.getContentsOf(newX1 / 16, y / 16) != Board::empty)
			possible++;
	}

	if (m_walkDirection == up) {
		newX1 = x + 16;
		newX2 = x - 16;
		newY1 = y + 16;
		if (game.getContentsOf(newX1 / 16, y / 16) != Board::empty)
			possible++;
		if (game.getContentsOf(newX2 / 16, y / 16) != Board::empty)
			possible++;
		if (game.getContentsOf(x / 16, newY1 / 16) != Board::empty)
			possible++;
	}

	if (m_walkDirection == down) {
		newX1 = x + 16;
		newX2 = x - 16;
		newY1 = y - 16;
		if (game.getContentsOf(newX1 / 16, y / 16) != Board::empty)
			possible++;
		if (game.getContentsOf(newX2 / 16, y / 16) != Board::empty)
			possible++;
		if (game.getContentsOf(x / 16, newY1 / 16) != Board::empty)
			possible++;
	}

	if (possible > 1) {
		return true;
	}

	return false;

}

void Mover::doSomething() {
	int x = getX();
	int y = getY();
	Board game = getWorld()->getBoard();
	int dir = m_walkDirection;
	int newX, newY;

	if (m_walking) {
		switch (dir) {
		case left:
			x -= 16;
			break;
		case right:
			x += 16;
			break;
		case up:
			y += 16;
			break;
		case down:
			y -= 16;
			break;
		}

		newX = x / 16;
		newY = y / 16;
		if (onSquare() && multipleDir() && didWalk && !atBegin()) {

		}
		else if (game.getContentsOf(newX, newY) == Board::empty && onSquare()) {
			if (dir == right || dir == left) {
				bool up1 = true, down1 = true;
				x = getX();
				y = getY();
				newX = x;
				newY = y + 16;
				if (game.getContentsOf(newX / 16, newY / 16) == Board::empty)
					up1 = false;
				newY = y - 16;
				if (game.getContentsOf(newX / 16, newY / 16) == Board::empty)
					down1 = false;

				if (up1 && down1)
				{
					setDirection(right);
					m_walkDirection = up;
				}
				else if (up1) {
					setDirection(right);
					m_walkDirection = up;
				}
				else {
					setDirection(right);
					m_walkDirection = down;
				}

			}
			else {
				bool left1 = true, right1 = true;
				y = getY();
				x = getX();
				newX = x - 16;
				newY = y;
				if (game.getContentsOf(newX / 16, newY / 16) == Board::empty)
					left1 = false;
				newX = x + 16;
				if (game.getContentsOf(newX / 16, newY / 16) == Board::empty)
					right1 = false;

				if (left1 && right1)
				{
					setDirection(right);
					m_walkDirection = right;
				}
				else if (right1) {
					setDirection(right);
					m_walkDirection = right;
				}
				else {
					setDirection(left);
					m_walkDirection = left;
				}
			}
		}
		int moveX, moveY;
		getPositionInThisDirection(m_walkDirection, 2, moveX, moveY);
		moveTo(moveX, moveY);
		didWalk = true;
		atStart = false;
		m_ticks -= 1;
	}
}

bool Mover::getRandomDir() {
	int dir = randInt(1, 4);
	Board game = getWorld()->getBoard();
	int x = getX();
	int y = getY();
	switch (dir) {
	case 1:
		if (game.getContentsOf((x - 16) / 16, y / 16) != Board::empty) {
			setDirection(left);
			setWalkDirection(left);
			return true;
		}
		break;
	case 2:
		if (game.getContentsOf((x + 16) / 16, y / 16) != Board::empty) {
			setDirection(right);
			setWalkDirection(right);
			return true;
		}
		break;
	case 3:
		if (game.getContentsOf(x / 16, (y + 16) / 16) != Board::empty) {
			setDirection(right);
			setWalkDirection(up);
			return true;
		}
		break;
	case 4:
		if (game.getContentsOf(x / 16, (y - 16) / 16) != Board::empty) {
			setDirection(right);
			setWalkDirection(down);
			return true;
		}
		break;
	}
	return false;
}

void Avatar::doSomething() {
	Board game = getWorld()->getBoard();
	if (!Mover::isWalking()) {
		int x = 0;
		int y = 0;
		getPositionInThisDirection(getWalkDireciton(), 16, x, y);
		x /= 16;
		y /= 16;
		if (game.getContentsOf(x, y) == Board::empty && onSquare() && wasOnEvent) {
			bool randir = false;
			while (!randir) {
				randir = getRandomDir();
			}
			setAtStart(true);
		}
		int key = getWorld()->getAction(m_playerNum);
		Actor* fire;
		switch (key) {
			int roll;
		case ACTION_ROLL:
			roll = randInt(1, 10);
			Mover::changeTicks(roll * 8);
			Mover::setWalking(true);
			Mover::walked(false);
			break;
		case ACTION_FIRE:
			if (hasVortex) {
				int x = getX();
				int y = getY();
				getPositionInThisDirection(getWalkDireciton(), 16, x, y);
				fire = new Vortex(getWorld(), x, y, getWalkDireciton());
				getWorld()->addToActors(fire);
				getWorld()->playSound(SOUND_PLAYER_FIRE);
				hasVortex = false;
			}
			break;
		default:
			Mover::walked(false);
			return;
		} 
	}

	if (isWalking()) {
		Board game = getWorld()->getBoard();
		int x = getX();
		int y = getY();
		if (onSquare() && Mover::multipleDir() && !atBegin() && !onDirectional && !atBegin()) {
			walked(false);
			int key = getWorld()->getAction(m_playerNum);
			if (key == ACTION_DOWN || key == ACTION_UP || key == ACTION_RIGHT || key == ACTION_LEFT) {
				if (key == ACTION_DOWN || key == KEY_PRESS_DOWN) {
					if (getWalkDireciton() != up && game.getContentsOf(x / 16, (y - 16) / 16) != Board::empty) {
						setWalkDirection(down);
						setDirection(right);
					}
					else
						return;
				}
				if (key == ACTION_UP || key == KEY_PRESS_UP) {
					if (getWalkDireciton() != down && game.getContentsOf(x / 16, (y + 16) / 16) != Board::empty) {
						setWalkDirection(up);
						setDirection(right);
					}
					else
						return;
				}
				if (key == ACTION_RIGHT || key == KEY_PRESS_RIGHT) {
					if (getWalkDireciton() != left && game.getContentsOf((x + 16) / 16, y / 16) != Board::empty) {
						setWalkDirection(right);
						setDirection(right);
					}
					else
						return;
				}
				if (key == ACTION_LEFT || key == KEY_PRESS_LEFT) {
					if (getWalkDireciton() != right && game.getContentsOf((x - 16) / 16, y / 16) != Board::empty) {
						setWalkDirection(left);
						setDirection(left);
					}
					else
						return;
				}
			}
			else
				return;
		}
	}
	Mover::doSomething();
	onDirectional = false;
	wasOnEvent = false;
	setWasOnEvent(false);
	if (Mover::getTicks() == 0) {
		Mover::setWalking(false);
	}
}

bool Mover::teleport(){
	Actor* randomSquare;
	randomSquare = getWorld()->getRandomSquare();
	int x = randomSquare->getX();
	int y = randomSquare->getY();
	if (x % 16 == 0 && y % 16 == 0) {
		moveTo(x, y);
		return true;
	}
	else {
		return false;
	}
}

void Avatar::teleport(int x, int y) {
	Avatar* other;
	if (m_playerNum == 1)
		other = getWorld()->getYoshi();
	else
		other = getWorld()->getPeach();
	if (x < 0 || y < 0) {
		bool success = Mover::teleport();
		while (!success)
			success = Mover::teleport();
	}
	else {
		int oldX = getX(), oldY = getY();
		moveTo(x, y);
		other->walked(false);
		other->moveTo(oldX, oldY);

		int ticks = getTicks();
		setTicks(other->getTicks());
		other->setTicks(ticks);

		int dir = getWalkDireciton();
		setWalkDirection(other->getWalkDireciton());
		other->setWalkDirection(dir);

		int sprite = getDirection();
		setDirection(other->getDirection());
		other->setDirection(sprite);

		int walking = isWalking();
		setWalking(other->isWalking());
		other->setWalking(walking);

		wasOnEvent = true;
	}
}

void Vortex::doSomething() {
	if (isAlive()) {
		int moveX = 0, moveY = 0;
		getPositionInThisDirection(direction, 2, moveX, moveY);
		moveTo(moveX, moveY);
		if (getX() < 0 || getX() >= VIEW_WIDTH || getY() < 0 || getY() >= VIEW_HEIGHT) {
			setDead();
		}

		//need to do impact on Bowser and Boo and any actor that is impacted
		int x = getX();
		int y = getY();
		if (getWorld()->overlap(x, y)) {
			setDead();
			getWorld()->playSound(SOUND_HIT_BY_VORTEX);
		}
	}
}

void Villain::doSomething() {
	Board bd = getWorld()->getBoard();
	setAtStart(false);
	if (impacted) {
		bool success = Mover::teleport();
		while (!success)
			success = Mover::teleport();
		if (bd.getContentsOf((getX() + 16) / 16, getY() / 16) != Board::empty) {
			setWalkDirection(right);
			setDirection(right);
		}
		else {
			bool randDir = false;
			while (randDir == false) {
				randDir = getRandomDir();
			}
		}
		setWalking(false);
		pauseCounter = 180;
		impacted = false;
	}

	if (!isWalking()) {
		pauseCounter--;
		if (pauseCounter <= 0) {
			if (isBoo())
				travelDistance = randInt(1, 3);
			else
				travelDistance = randInt(1, 10);
			changeTicks(travelDistance * 8);
			bool randDir = false;
			while (randDir == false) {
				randDir = getRandomDir();
			}
			setWalking(true);
		}

	}

	if (isWalking()) {
		if (onSquare() && multipleDir() && !atBegin()) {
			bool randDir = false;
			while (randDir == false) {
				randDir = getRandomDir();
			}
		}
		Mover::doSomething();
		setActivate(true);
		if (getTicks() == 0) {
			setWalking(false);
			pauseCounter = 180;
			int rand = randInt(1, 4);
			if (rand == 1) {
				dropped = true;
			}
		}
	}
}

void Bowser::doActivity(Avatar* player) {
	if (player->getX() == getX() && player->getY() == getY() && player->onSquare() && onSquare() && !player->isWalking() && getActivate()) {
		int chance = randInt(1, 2);
		if (chance == 1) {
			player->setCoins(0);
			player->setStars(0);
			getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
		}
		setActivate(false);
	}
}

void Bowser::doSomething() {
	Avatar* peach = getWorld()->getPeach();
	Avatar* yoshi = getWorld()->getYoshi();

	if (!isWalking()) {
		doActivity(peach);
		doActivity(yoshi);
	}
	Villain::doSomething();
	if (getTicks() == 0 && isDropped() && onSquare()) {
		setDropped(false);
		int x = getX() / 16;
		int y = getY() / 16;
		bool res = getWorld()->removeSquareAt(x, y);
		if (!res)
			return;
		Actor* drop = new DroppingSquare(getWorld(), getX(), getY());
		getWorld()->addToActors(drop);
		getWorld()->playSound(SOUND_DROPPING_SQUARE_CREATED);
	}
}

void Boo::doActivity(Avatar* player, Avatar* other) {
	int rand = randInt(1, 2);
	if (player->getX() == getX() && player->getY() == getY() && player->onSquare() && onSquare() && !player->isWalking() && getActivate()) {
		if (rand == 1) {
			int coins = other->getCoins();
			other->setCoins(player->getCoins());
			player->setCoins(coins);
		}
		else {
			int stars = other->getStars();
			other->setStars(player->getStars());
			player->setStars(stars);
		}
		getWorld()->playSound(SOUND_BOO_ACTIVATE);
		setActivate(false);
	}
}

void Boo::doSomething() {
	Avatar* peach = getWorld()->getPeach();
	Avatar* yoshi = getWorld()->getYoshi();

	if (!isWalking() && getActivate()) {
		doActivity(peach, yoshi);
		doActivity(yoshi, peach);
	}

	Villain::doSomething();
}

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
