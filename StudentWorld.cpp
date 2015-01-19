#include "StudentWorld.h"
#include <iostream>
#include <sstream>

using namespace std;

GameWorld* createStudentWorld()
{
    return new StudentWorld();
}

void StudentWorld::tickcounter()
{
	int tick = create->tcount();
	if (tick > 1)
	{ create->setpre();}
}

ThePlayership* StudentWorld::getplayer()
{
	return create;
}

void StudentWorld::AddActor()
{
	if (testParamsProvided()) //test parameter provide
	{
		if (n_alien > 0)
		{
			return;
		}
		n_alien++;
		int test = TEST_PARAM_ACTOR_INDEX;
		if (test == TEST_PARAM_NACHLING)
		{
			Actor* create = new Nachling(this,n_round);
			actors.push_back(create);
		}
		else if (test == TEST_PARAM_WEALTHY_NACHLING)
		{
			Actor* create = new WealthyNachling(this,n_round);
			actors.push_back(create);
		}
		else if (test == TEST_PARAM_SMALLBOT)
		{
			Actor* create = new SmallBots(this,n_round);
			actors.push_back(create);
		}
		else if (test == TEST_PARAM_GOODIE_ENERGY)
		{
			Actor* create = new EnergyGoodie(this,VIEW_WIDTH/2,VIEW_HEIGHT-1,n_round);
			actors.push_back(create);
		}
		else if (test == TEST_PARAM_GOODIE_TORPEDO)
		{
			Actor* create = new TorpedoGoodie(this,VIEW_WIDTH/2,VIEW_HEIGHT-1,n_round);
			actors.push_back(create);
		}
		else if (test == TEST_PARAM_GOODIE_FREE_SHIP)
		{
			Actor* create = new FreeShipGoodie(this,VIEW_WIDTH/2,VIEW_HEIGHT-1,n_round);
			actors.push_back(create);
		}
		return;
	}
	else
	{
		int s = int(2+0.5*n_round);
		int left = 4*n_round - n_des;
		int p3 = rand() % 3;
		if (p3 == 0)
		{
			Stars* Screate = new Stars(this);
			actors.push_back(Screate);
		}
		int p1 = rand() % 101;
		int p2 = rand() % 101;
		if (p1 < 71)
		{
			if (p2 >= 80)
			{
				if (n_alien < s)
				{
					Nachling* Ncreate = new Nachling(this,n_round);
					actors.push_back(Ncreate);
					n_alien++;
				}
		}	
			if (p2 < 20)
			{
				if (n_alien < s)
				{
					WealthyNachling* WNcreate = new WealthyNachling(this,n_round);
					actors.push_back(WNcreate);
					n_alien++;
				}
			}
		}
		else
		{
			if (n_alien < s)
			{
				SmallBots* sbcreate = new SmallBots(this,n_round);
				actors.push_back(sbcreate);
				n_alien++;
			}
		}
	}
}
void StudentWorld::MakedoSomething()
{
	for (unsigned int i=0;i < actors.size();i++)
	{ 
		actors[i]->doSomething();
	}
	if (n_des >= 4*n_round)
	{
		n_round++;
		n_des = 0;
	}
}

bool StudentWorld::RemoveDead()
{
	if (!actors[0]->Alive())
	{
		return true;
	}
	vector<Actor*>::iterator it ;
	for (it = actors.begin();it != actors.end() ;)
	{
		if (!(*it)->Alive())
		{
			Alien* a = dynamic_cast<Alien*>(*it);
			if (a != NULL)
			{
				n_alien--;
			}
			Shoting* s = dynamic_cast<Shoting*>(*it);
			if (s != NULL && !s->fromPlayers())
			{
				e_shoting--;
			}
			delete *it;
			it = actors.erase(it);
		}
		else
		{
			it++;
		}
	}
	return false;
}

void StudentWorld::UpdateDisplay()
{
	for (unsigned int i=0;i < actors.size();i++)
	{
		actors[i]->setVisible(true);
	}
}


void StudentWorld::wealthydrop(Actor* a)
{
	int ran = rand() % 2;
	if (ran)
	{
		Goodie* g = new EnergyGoodie(this,a->getX(),a->getY(),n_round);
		actors.push_back(g);
	}
	else
	{
		Goodie* g = new TorpedoGoodie(this,a->getX(),a->getY(),n_round);
		actors.push_back(g);
	}
}

void StudentWorld::smallbotdrop(Actor* a)
{
	Goodie* g = new FreeShipGoodie(this,a->getX(),a->getY(),n_round);
	actors.push_back(g);
}

string makestring(int score,int round, int percent, int torpedo, int ships)
{
	string sscore = "0000000";
	for (unsigned int i = 0; i < 7;i++)
	{
		sscore[6-i] = (score % 10) + '0';
		score = score/10;
	}
	string sround = "00";
	for (unsigned int i = 0; i < 2;i++)
	{
		sround[1-i] = (round % 10) + '0';
		round = round/10;
	}
	string spercent;
	if (percent < 100)
	{
		spercent = "00";
		for (unsigned int i = 0; i < 2; i++)
		{
			spercent[1-i] = (percent % 10) + '0';
			percent = percent/10;
		}
	}
	else if (percent == 100)
	{
		spercent = "100";
	}
	string storpedo = "000";
	for (unsigned int i = 0; i < 3;i++)
	{
		storpedo[2-i] = (torpedo % 10) + '0';
		torpedo = torpedo/10;
	}
	string sships = "00";
	for (unsigned int i = 0; i < 2;i++)
	{
		sships[1-i] = (ships % 10) + '0';
		ships = ships/10;
	}
	string result = "Score: " +sscore + "  Round: " + sround + "  Energy " + spercent +"%  Torpedoes: " + storpedo +  "  Ships: " + sships;
	return result;
}

void StudentWorld::UpdateStatus()
{
	int score = getScore();
	int round = getRound();
	int energypercent = 2*create->gethp();
	int torpedo = create->getTorpedo();
	int ships = getLives();
	string s = makestring(score,round,energypercent,torpedo,ships);
	setGameStatText(s);
}

void StudentWorld::addshoting(int ID,bool players,Actor* ptr)
{
	if (!players)
	{
		e_shoting++;
	}
	if (ID == IID_TORPEDO)
	{
		Actor* shoting = new Torpedo(this,ID,ptr->getX(),ptr->getY()+1,players);
		actors.push_back(shoting);
	}
	if (ID == IID_BULLET)
	{
		Actor* shoting = new Bullet(this,ID,ptr->getX(),ptr->getY()+1,players);
		actors.push_back(shoting);
	}
}

// Students:  Add code to this file (if you wish), StudentWorld.h, actor.h and actor.cpp