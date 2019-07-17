#ifndef __MONSTER_H__
#define __MONSTER_H__

#include "cocos2d.h"
#include <vector>

USING_NS_CC;

class Monster
{
public:
	static const int MONSTER_BITMASK = 4;

private:
	int mBlood;
	int mDamage;
	float mSpeed;
	float mRange;
	int mDirection;
	int mCurrentState;
	Sprite* mSprite;
	Action* mAction[10];
	PhysicsBody* mPhysicsBody;

public:
	Monster(Layer* layer);
	~Monster();
	void SetBlood(int blood);
	int GetBlood();
	void SetDamage(int damage);
	int GetDamage();
	void SetSpeed(float speed);
	float GetSpeed();
	void SetRange(float range);
	float GetRange();
	void SetDirection(int direction);
	int GetDirection();
	void SetCurrentState(int currentState);
	int GetCurrentState();
	Sprite* GetSprite();
	PhysicsBody* GetPhysicsBody();

	void Init();
	void Run();
	void Update(float deltaTime);
	bool Detect(Vec2* pos);
	void Hit();
};

#endif // !__MONSTER_H__

