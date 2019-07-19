#ifndef SPEAR_MOBLIN_H
#define SPEAR_MOBLIN_H	

#include "Enemy.h"
#include "Pierce.h"

class SpearMoblin : public Enemy
{
private:
	static const int FRONT_IDLE = 0;
	static const int BACK_IDLE = 1;
	static const int LEFT_IDLE = 2;
	static const int GO_UP = 3;
	static const int GO_DOWN = 4;
	static const int GO_LEFT = 5;
	static const int FRONT_ATTACK = 6;
	static const int BACK_ATTACK = 7;
	static const int LEFT_ATTACK = 8;

	Action* mAction[9];
	Pierce* pierce;
	bool onTarget;
public:
	SpearMoblin();
	SpearMoblin(Layer* layer, int id);
	~SpearMoblin();

	void Update(float deltaTime);

	void SetState(int nextState);

	void Auto(float deltaTime);

	void Run();

	void Idle();

	bool Detect();

	void Attack();

	void CollideObstacle();

	void SetDirection(int dir);

};

#endif
