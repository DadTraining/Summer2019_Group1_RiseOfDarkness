#ifndef BOW_MOBLIN_H
#define BOW_MOBLIN_H	

#include "Enemy.h"
#include "Arrow.h"
#include <vector>

class BowMoblin : public Enemy
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

	static const int HP = 100;
	static const int DETECT_RANGE = 300;
	static const int ATTACK_RANGE = 200;
	static const int ATTACK_COOLDOWN = 3;
	static const int REVIVE_TIME = 5;
	static const int HP_REVIVE = 10;

	Action* mAction[9];
	std::vector<Arrow*> m_arrows;
public:
	BowMoblin();
	BowMoblin(Layer* layer, int direction, Vec2 pos);
	~BowMoblin();

	void Update(float deltaTime);

	void SetState(int nextState);

	void Run();

	void Idle();

	void Attack();

	std::vector<Arrow*> GetListArrow();
};

#endif
