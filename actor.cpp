#include "actor.h"
#include "StudentWorld.h"


bool Actor::Alive()
{
	return N_alive;
}

void Actor::Kill()
{
	N_alive = false;
}

void Stars::doSomething()
{
	//if stars y hit 0 delete stars
	
	if (getY() < 0)
	{
		Kill();
	}
	else
	{
		moveTo(getX(),getY()-1);
	}
}

void ThePlayership::keyboard(bool t,int key)
{
	get = t;
	N_Key = key;
}

void ThePlayership::doSomething()
{
	collision();
	if (hp <= 0)
	{
		Kill();
		return;
	}
	t_count++;
	if (t_count == 2)
	{
		pre = 0;
	}
	bool change = false;
	if (get)
	{
		switch(N_Key)
		{
			case KEY_PRESS_LEFT:
				if (getX() > 0)
				{	moveTo(getX()-1,getY());}
				pre = KEY_PRESS_LEFT;
				break;
			case KEY_PRESS_RIGHT:
				if (getX() < VIEW_WIDTH-1)
				{	moveTo(getX()+1,getY());}
				pre = KEY_PRESS_RIGHT;
				break;
			case KEY_PRESS_UP:
				if (getY() < VIEW_HEIGHT-1)
				{	moveTo(getX(),getY()+1);}
				pre = KEY_PRESS_UP;
				break;
			case KEY_PRESS_DOWN:
				if (getY() > 0)
				{	moveTo(getX(),getY()-1);}
				pre = KEY_PRESS_DOWN;
				break;
			case KEY_PRESS_SPACE:
				t_count = 0;
				if (pre != KEY_PRESS_SPACE && pre != KEY_PRESS_TAB)
				{
					if (getY() != VIEW_HEIGHT-1)
					{
						s_ptr->addshoting(IID_BULLET,true,this);
						s_ptr -> playSound(SOUND_PLAYER_FIRE);
						change = true;
					}
				}
				if (pre != KEY_PRESS_SPACE && pre != KEY_PRESS_TAB)
				{
					pre = KEY_PRESS_SPACE;
				}
				else
				{
					pre = 0;
				}
				break;
			case KEY_PRESS_TAB:
				t_count = 0;
				if (pre != KEY_PRESS_TAB && pre != KEY_PRESS_SPACE)
				{
					if (getY() != VIEW_HEIGHT-1)
					{
						if (n_torpedo > 0)
						{
							s_ptr->addshoting(IID_TORPEDO,true,this);
							s_ptr -> playSound(SOUND_PLAYER_TORPEDO);
							change = true;
							n_torpedo--;
						}
					}
				}
				if (pre != KEY_PRESS_SPACE && pre != KEY_PRESS_TAB)
				{
					pre = KEY_PRESS_TAB;
				}
				else
				{
					pre = 0;
				}
		}
	}
	if (change = false)
	{
		pre = 0;
	}
	collision();
}

bool Shoting::collidAlien(Actor* shoting)
{
	bool die = false;
	vector<Actor*> actors = s_ptr->getActors();
	for (unsigned i = 0;i < actors.size();i++)
	{
		Alien* a= dynamic_cast<Alien*>(actors[i]);
		if (a != NULL)
		{
			if(!a->Alive())
			{
				return false;
			}
			if (a->getX() == shoting->getX() && a->getY() == shoting->getY())
			{
				die = true;
				Bullet* s = dynamic_cast<Bullet*> (shoting);
				Shoting* check = dynamic_cast<Shoting*> (shoting);
				if (s != NULL)
				{
					if (check->fromPlayers())
					{
						a->shotbybullet(a);
					}
				}
				else
				{
					if (check->fromPlayers())
					{
						a->shotbytorpedo(a);
					}
				}
			}
		}
	}
	return die;
}

bool Shoting::shotplayer(Actor* shoting)
{
	if (s_ptr->getplayer()->getX() == shoting->getX() && s_ptr->getplayer()->getY() == shoting->getY())
	{
		Bullet* s = dynamic_cast<Bullet*>(shoting); 
		s_ptr->getplayer()->damage(s);
		return true;		
	}
	return false;
}

void ThePlayership::damage(Shoting* s)
{
	if (s != NULL)
	{
		hp = hp - 2;
	}
	else
	{
		hp = hp - 8; 
	}
}

void Shoting::doSomething()
{
	//when collid to alien
	if (Players)
	{
		if (getY() == VIEW_HEIGHT -1 || collidAlien(this))
		{
			Kill();
		}
		else
		{
			moveTo(getX(),getY()+1);
		}
		if (collidAlien(this))
		{
			Kill();
		}
	}
	else
	{
		if (getY() == 0 || shotplayer(this)) //or hit player ship
		{
			Kill();
		}
		else
		{
			moveTo(getX(),getY()-1);
		}
		if (shotplayer(this))
		{
			Kill();
		}
	}
}

void ThePlayership::collision()
{
	vector<Actor*> actors = s_ptr->getActors();
	for (unsigned i = 0;i < actors.size();i++)
	{
		Alien* a= dynamic_cast<Alien*>(actors[i]);
		if (a != NULL)
		{
			if(!a->Alive())
			{
				return;
			}
			if (a->getX() == getX() && a->getY() == getY())
			{
				s_ptr->playSound(SOUND_ENEMY_PLAYER_COLLISION);
				a->Kill();
				hp = hp -15;
			}
		}
	}
}




void NachlingBase::doSomething()
{
//	cout <<"state " <<state <<endl;
//	cout <<"x" <<getX() <<endl;
	if (getHp() <= 0)
	{
		WealthyNachling* na = dynamic_cast<WealthyNachling*> (this);
		if (rand() % 3 == 0 && na != NULL)
		{
			s_ptr->wealthydrop(this);
		}
		s_ptr->playSound(SOUND_ENEMY_DIE);
		s_ptr->increaseScore(getworth());
		s_ptr->incdest();
		Kill();
		return;
	}
	tick++;
	if (tick == 2)
	{
		tick = 0;
		return; 
	}
	if (state == 0)
	{
		int ran = rand()% 3;
		if (getX() != s_ptr->getplayer()->getX())
		{
			if (ran == 0)
			{
				if (getX() < s_ptr->getplayer()->getX())
				{
					moveTo(getX()+1,getY()-1);
				}
				else
				{
					moveTo(getX()-1,getY()-1);
				}
			}
			else
			{
				moveTo(getX(),getY()-1);
			}
			return;
		}
		else
		{
			state = 1;
			MDB = min(getX(), VIEW_WIDTH-getX());
			if (MDB > 3)
			{
				HMD = (rand() % 3) + 1;
			}
			direction = rand() % 2; //0 will be left 1 will be right
			HMR = HMD;
		}
		moveTo(getX(),getY()-1);
		if (getY() < 0)
		{
			Kill();
		}
		return;
	}
	if (state == 1)
	{
		if (s_ptr->getplayer()->getY() > getY())
		{
			state = 2;
			return;
		}
		if (HMR == 0)
		{
			if (direction == 0)
			{direction = 1;}
			else if (direction == 1)
			{direction = 0;}
			HMR = HMD * 2;
		}
		if (HMR != 0)
		{
			HMR--;
		}
		if (direction == 0)
		{
			moveTo(getX()-1,getY());
		}
		if (direction == 1)
		{
			moveTo(getX()+1,getY());
		}
		int basechance = 10;
		int chance = (basechance/n_round)+1;
		int n_eshoting = 0;
		if ((rand() % 3) == 1)
		{
			n_eshoting = s_ptr->getEshoting();
			if (n_eshoting < 2*n_round)
			{
				s_ptr->addshoting(IID_BULLET,false,this);
				s_ptr->playSound(SOUND_ENEMY_FIRE);
			}
		}
		if (rand() % 20)
		{
			state = 2;
			return;
		}
		return;
	}
	if (state == 2)
	{
		if (getY() == VIEW_HEIGHT-1)
		{
			state = 0;
			return;
		}
		else if (getX() == 0)
		{
			direction = 1;
		}
		else if (getX() == VIEW_WIDTH-1)
		{
			direction = 0;
		}
		else
		{
			direction = rand() % 2;
		}
		if (direction == 0)
		{
			moveTo(getX()-1,getY()+1);
		}
		else 
		{
			moveTo(getX()+1,getY()+1);
		}
		return;
	}
}

void Alien::shotbybullet(Alien* a)
{
	h_point = h_point - 2;
	hit = true;
	s_ptr->playSound(SOUND_ENEMY_HIT);
}

void Alien::shotbytorpedo(Alien* a)
{
	h_point = h_point - 8;
	hit = true;
	s_ptr->playSound(SOUND_ENEMY_HIT);
}

void WealthyNachling::doSomething()
{
	if (rannum == 0)
	{
		tick++;
		if (tick == 30)
		{
			malfun = false;
		}
	}
	if (malfun)
	{
		if (getHp() <= 0)
		{
			Kill();
		}
		return;
	}
	else
	{
		rannum = rand() % 200;
		if (rannum == 0)
		{
			malfun = true;
			return;
		}
	}
	NachlingBase::doSomething();
}



void SmallBots::doSomething()
{
	if (getHp() <= 0)
	{
		s_ptr->smallbotdrop(this);
		s_ptr->increaseScore(getworth());
		s_ptr->playSound(SOUND_ENEMY_DIE);
		s_ptr->incdest();
		Kill();
		return;
	}
	tick++;
	if (tick == 2)
	{
		tick = 0;
		return; 
	}
	if (attacked())
	{
		int ran = rand() % 3;
		if (ran == 0)
		{
			if (getX() == 0)
			{
				moveTo(getX()+1,getY()-1);
			}
			else if (getX() == VIEW_WIDTH-1)
			{
				moveTo(getX()-1,getY()-1);
			}
			else
			{
				int rn = rand() % 2;
				if (rn == 0)
				{
					moveTo(getX()-1,getY()-1);
				}
				else
				{
					moveTo(getX()+1,getY()-1);
				}
			}
		}
		else
		{
			moveTo(getX(),getY()-1);
		}
	}
	else
	{
		moveTo(getX(),getY()-1);
	}
	if (getX() == s_ptr->getplayer()->getX())
	{
		int q = 100/n_round +1;
		int ran = rand() % q;
		if (ran == 0)
		{
			int n_eshoting = s_ptr->getEshoting();
			if (n_eshoting < 2 * n_round)
			{
				s_ptr->addshoting(IID_TORPEDO,false,this);
				s_ptr->playSound(SOUND_ENEMY_FIRE);
			}
		}
		else
		{
			int n_eshoting = s_ptr->getEshoting();
			if (n_eshoting < 2 * n_round)
			{
				s_ptr->addshoting(IID_BULLET,false,this);
				s_ptr->playSound(SOUND_ENEMY_FIRE);
			}
		}
	}
	if (getY() < 0)
	{
		Kill();
	}
	sethit();
}

bool Goodie::pick()
{
	if (getX() == s_ptr->getplayer()->getX() && getY() == s_ptr->getplayer()->getY())
	{
		s_ptr->playSound(SOUND_GOT_GOODIE);
		return true;
	}
	return false;
}

void Goodie::doSomething()
{
	tick++;
	tickleft--;
	if (pick())
	{
		dogood();
		Kill();
		return;
	}
	setBrightness((tickleft/lifetime)+.2);
	if (tickleft == 0)
	{
		Kill();
		return;
	}
	if (getY() < 0)
	{
		Kill();
		return;
	}
	else
	{
		if (tick == 3)
		{
			moveTo(getX(),getY()-1);
			tick = 0;
		}
	}
	if (pick())
	{
		dogood();
		Kill();
		return;
	}
	if (getY() < 0)
	{
		Kill();
		return;
	}
}

void FreeShipGoodie::dogood()
{
	s_ptr->incLives();
	s_ptr->increaseScore(5000);
}

void EnergyGoodie::dogood()
{
	s_ptr->getplayer()->fullenergy();
}

void TorpedoGoodie::dogood()
{
	s_ptr->getplayer()->picktorpedo();
}



// :  Add code to this file (if you wish), actor.h, StudentWorld.h, and StudentWorld.cpp