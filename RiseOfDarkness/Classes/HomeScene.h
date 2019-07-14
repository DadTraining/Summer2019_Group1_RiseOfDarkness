#ifndef HOME_SCENE_H
#define HOEM_SCENE_H

#include "MainCharacter.h"
#include "GamePlay.h"

class HomeScene : public Layer, GamePlay
{
private:
	
public:
	static Scene* CreateScene();
	virtual bool init();
	void update(float deltaTime);
	void AddListener();
	
	bool OnTouchBegan(Touch* touch, Event* event);
	bool OnTouchEnded(Touch* touch, Event* event);
	void OnTouchMoved(Touch* touch, Event* event);
	void NormalAttack(Ref* sender, ui::Widget::TouchEventType type);
	void SpecialAttack(Ref* sender, ui::Widget::TouchEventType type);
	void Evade(Ref* sender, ui::Widget::TouchEventType type);
	void Defend(Ref* sender, ui::Widget::TouchEventType type);

	void SetCamera(Vec2 pos);
	void CreateAllButton(Layer* layer);

	CREATE_FUNC(HomeScene);
};

#endif
