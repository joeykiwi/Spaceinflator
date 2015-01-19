#ifndef _ACTOR_H_
#define _ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"
#include <iostream>
using namespace std;
//#include "StudentWorld.h"
// Students:  Add code to this file, actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject
{
public:
	Actor(StudentWorld* world, int imageID, int startX, int startY)
		:GraphObject(imageID, startX, startY)
	{
		s_ptr = world;
		N_alive = true;
	}
	virtual ~Actor(){}; //later implementation
	virtual void doSomething() = 0;  //pure virtual
	virtual bool Alive();
	virtual void Kill();
private:
	StudentWorld* s_ptr;
	bool N_alive;
};

class Shoting : public Actor
{
public:
	Shoting(StudentWorld* world, int imageID, int startX, int startY, bool playerFired)
		:Actor(world, imageID, startX, startY) 
	{
		Players = playerFired;
		s_ptr = world;
		shoting = this; 
	}
	virtual ~Shoting(){};
	virtual void doSomething();
	bool shotplayer(Actor* shoting);
	bool collidAlien(Actor* shoting);
	bool fromPlayers() {return Players;}
private:
	StudentWorld* s_ptr;
	bool Players;
	Actor* shoting;
};

class Bullet : public Shoting
{
public:
	Bullet(StudentWorld* world, int imageID, int startX, int startY, bool playerFired)
		:Shoting(world,IID_BULLET, startX, startY,playerFired) {}
	virtual ~Bullet(){};
	//virtual void doSomething();
private:
};

class Torpedo : public Shoting
{
public:
	Torpedo(StudentWorld* world,int imageID ,int startX, int startY, bool playerFired)
		:Shoting(world,IID_TORPEDO, startX, startY,playerFired) {}
	virtual ~Torpedo() {};
	//virtual void doSomething();
private:
};



class Alien: public Actor
{
public:
	Alien(StudentWorld* world, int imageID, int startX, int startY,int worth, int hp)
		:Actor(world, imageID, startX, startY), s_ptr(world), h_point(hp) , point(worth), hit(false) {}
	virtual ~Alien(){} //later implementation
	virtual void doSomething() =0;
	void shotbybullet(Alien* a);
	void shotbytorpedo(Alien* a);
	int getworth() {return point;}
	int getHp() {return h_point;}
	bool attacked() {return hit;}
	void sethit() {hit = false;}
private:
	int h_point, point;
	bool hit;
	StudentWorld* s_ptr;
};

class NachlingBase : public Alien
{
public:
	NachlingBase(StudentWorld* world, int image,int round,int worth,int hp)
		:Alien(world,image,rand() % VIEW_WIDTH,VIEW_HEIGHT-1,worth,hp), state(0), s_ptr(world),n_round(round)
	    ,MDB(-1),HMD(-1),HMR(-1), tick(0), direction(-1) {}
	virtual ~NachlingBase() {}
	virtual void doSomething();
private:
	StudentWorld* s_ptr;
	int n_round, state,tick;
	int MDB,HMD,HMR,direction;
};

class Nachling : public NachlingBase
{
public:
	Nachling(StudentWorld* world,int round)
		:NachlingBase(world,IID_NACHLING,round,1000,5*(0.9 + 0.1*round)){}
private:
};

class WealthyNachling : public NachlingBase
{
public:
	WealthyNachling(StudentWorld* world,int round)
		:NachlingBase(world, IID_WEALTHY_NACHLING, round, 1200, 8*(0.9 + 0.1*round)), malfun(false), tick(0), rannum(-1), s_ptr(world) {}
	virtual void doSomething();
private:
	int tick, rannum;
	bool malfun;
	StudentWorld* s_ptr;
};

class SmallBots : public Alien
{
public:
	SmallBots(StudentWorld* world, int round)
		:Alien(world,IID_SMALLBOT,rand() % VIEW_WIDTH,VIEW_HEIGHT-1,1500,12*(0.9 + 0.1*round)), s_ptr(world),tick(0), n_round(round){}
	virtual void doSomething();
private:
	StudentWorld* s_ptr;
	int tick,n_round;
};
class Stars : public Actor
{
public:
	Stars(StudentWorld* world) //constructor
		:Actor(world, IID_STAR, rand() % VIEW_WIDTH, VIEW_HEIGHT-1)
	{}
	virtual ~Stars(){}; //destructor
	virtual void doSomething(); //later implementation
private:

};

class ThePlayership : public Actor
{
public:
	ThePlayership(StudentWorld* world)   // constructor
		:Actor(world, IID_PLAYER_SHIP, VIEW_WIDTH/2,1), N_Key(0), hp(50), get(false), pre(0), re(false), shot(false)
		,t_count(0), n_torpedo(0),s_ptr(world) {}
	virtual ~ThePlayership(){}  // destructor
	virtual void doSomething(); //later implementation
	void keyboard(bool t,int key);
	int tcount() {t_count++; return t_count;}
	void setpre() {pre = 0;}
	int gethp() {return hp;}
	int getTorpedo() {return n_torpedo;}
	void damage(Shoting* s);
	void collision();
	void fullenergy() {hp = 50;}
	void picktorpedo() {n_torpedo = n_torpedo+5;}
private:
	StudentWorld* s_ptr;
	bool get, re,shot;
	int hp, n_torpedo;
	int N_Key,pre, t_count;
};

class Goodie : public Actor
{
public:
    Goodie(StudentWorld* world, int imageID, int startX, int startY,int round)
		:Actor(world,imageID,startX,startY), s_ptr(world), tick(0) ,lifetime((100/round) +30),tickleft((100/round)+30) {}
    virtual void doSomething();
	virtual bool pick();
	virtual void dogood() = 0;
private:
	int tick,lifetime,tickleft;
	StudentWorld* s_ptr;
};

class FreeShipGoodie : public Goodie
{
public:
	FreeShipGoodie(StudentWorld* world, int startX, int startY,int round)
		:Goodie(world,IID_FREE_SHIP_GOODIE,startX,startY, round), s_ptr(world) {}
	virtual void dogood();
private:
	StudentWorld* s_ptr;
};

class EnergyGoodie : public Goodie
{
public:
	EnergyGoodie(StudentWorld* world, int startX, int startY,int round)
		:Goodie(world,IID_ENERGY_GOODIE,startX,startY,round), s_ptr(world){}
	virtual void dogood();
private:
	StudentWorld* s_ptr;
};

class TorpedoGoodie : public Goodie
{
public:
	TorpedoGoodie(StudentWorld* world, int startX, int startY,int round)
		:Goodie(world,IID_TORPEDO_GOODIE,startX,startY,round), s_ptr(world){}
	virtual void dogood();
private:
	StudentWorld* s_ptr;
};


#endif // _ACTOR_H_