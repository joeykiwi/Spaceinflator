#ifndef _STUDENTWORLD_H_
#define _STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include "actor.h"
#include <vector>
#include <iostream>
using namespace std;
// Students:  Add code to this file, StudentWorld.cpp, actor.h, and actor.cpp





class StudentWorld : public GameWorld
{
public:
	StudentWorld() : key(0), n_alien(0), n_round(1), e_shoting(0), n_des(0)//constructor
	{}
	virtual ~StudentWorld() //destructor
	{
		delete create;
		vector<Actor*>::iterator it ;
		for (it = actors.begin();it != actors.end() ;)
		{
			delete *it;
			it = actors.erase(it);
		}
	}
	void UpdateDisplay();
	void AddActor();
	void MakedoSomething();
	bool RemoveDead();
	void UpdateStatus();
	void addshoting(int ID,bool players,Actor* ptr);
	void tickcounter();
	void decailen() {n_alien--;}
	void wealthydrop(Actor* a);
	void smallbotdrop(Actor* a);
	void incdest() {n_des++;}
	ThePlayership* getplayer();
	int getRound() {return n_round;}
	vector<Actor*> getActors() {return actors;}
	int getEshoting() {return e_shoting;}
	virtual void init()
    {
		create = new ThePlayership(this);
		actors.push_back(create); 
    }
	virtual int move()
    {
		bool t = getKey(key);
		create->keyboard(t,key);
		//add new aliens or stars
		AddActor(); 
		//give each actor a chance to do something
		MakedoSomething();
		//remove newly dead actors after each tick
		if (RemoveDead())
		{
			playSound(SOUND_PLAYER_DIE);
			decLives();
			n_des = 0;
			return GWSTATUS_PLAYER_DIED;
		}
		//update status
		UpdateStatus();
		//set visible
		UpdateDisplay();
				
          // This code is here merely to allow the game to build, run, and terminate after hitting enter a few times 
        //decLives();
		return GWSTATUS_CONTINUE_GAME;
    }
	virtual void cleanUp()
    {
		if (!actors.empty())
		{
			for (unsigned int i=0;i < actors.size();)
			{
				Alien* a = dynamic_cast<Alien*> (actors[actors.size()-1]);
				if (a !=NULL)
				{
					n_alien--;
				}
				Shoting* s = dynamic_cast<Shoting*>(actors[actors.size()-1]);
				if (s != NULL && !s->fromPlayers())
				{
					e_shoting--;
				}
				delete actors[actors.size()-1];
				actors.pop_back();
			}
		}
    }
private:
	int n_alien, key, n_round, e_shoting, n_des;
	ThePlayership* create;
	vector<Actor*> actors;
};

#endif // _GAMEWORLD_H_
