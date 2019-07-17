#ifndef MAIN_CHARACTER_H
#define MAIN_CHARACTER_H

#include <vector>
#include "cocos2d.h"
#include "Slash.h"

using namespace cocos2d;

class MainCharacter
{
public:
	static const int MAIN_CHARACTER_BITMASK = 0;
	static const int OBSTACLE_BITMASK = 1;
	static const int SLASH_BITMASK = 2;
	static const int MONSTER_ATTACK_BITMASK = 3;

	static const int SPEED = 2;
	static const int ATTACK = 20;
	static const int DEFEND = 10;

	static const int FRONT_IDLE = 0;
	static const int BACK_IDLE = 1;
	static const int LEFT_IDLE = 2;
	static const int GO_DOWN = 3;
	static const int GO_UP = 4;
	static const int GO_LEFT = 5;
	static const int ROLL_FRONT = 6;
	static const int ROLL_BACK = 7;
	static const int ROLL_LEFT = 8;
	static const int FRONT_SLASH = 9;
	static const int BACK_SLASH = 10;
	static const int LEFT_SLASH = 11;
	static const int FRONT_ARCHERY = 12;
	static const int BACK_ARCHERY = 13;
	static const int LEFT_ARCHERY = 14;
	static const int FRONT_SHIELD = 15;
	static const int BACK_SHIELD = 16;
	static const int LEFT_SHIELD = 17;
	static const int FRONT_GET_DAMAGE = 18;
	static const int BACK_GET_DAMAGE = 19;
	static const int LEFT_GET_DAMAGE = 20;
	static const int DEAD = 21;

public:
	static MainCharacter* GetInstance();
	void Init(std::string name);
	void Refresh();
	Sprite* GetSprite();
	PhysicsBody* GetPhysicsBody();
	int GetCurrentState();
	void Update(float deltaTime);
	void SetState(int nextStage);
	int GetSpeed();
	std::string GetName();
	int GetStageLevel();
	int GetDirection();
	void SetDirection(int direction);
	void Idle();
	void NormalAttack();
	void SpecialAttack();
	void Defend();
	void Evade();
	void Run();
	void StopRun();
	void StopDefend();
	float GetPercentHP();
	float GetPercentMP();
	void AutoRevive(float deltaTime);
	void AddToLayer(Layer* layer);
	void GetDamage(int damage);
	bool IsAlive();
private:
	static MainCharacter* m_instance;
	Sprite* mSprite;
	Action* mAction[22];
	PhysicsBody* mPhysicsBody;
	int stageLevel;
	int currentState;
	int direction;
	int speed;
	int attack;
	int defend;
	float maxHP, currentHP, maxMP, currentMP;
	Size box;
	float countingTime;
	std::string mName;
	Slash* slash;
	MainCharacter();
	~MainCharacter();
	void CreateMainCharacter();
};

#endif