#ifndef HOME_SCENE_H
#define HOME_SCENE_H

#include "GamePlay.h"
#include "cocos2d.h"

using namespace cocos2d;

class HomeScene : public Layer, GamePlay
{
private:
	Sprite* weaponSeller, *potionSeller, *equipmentSeller;
	Action* weapon, *potion, *equipment;
	ui::ScrollView* weaponscrollView;
public:
	static cocos2d::Scene* CreateScene();
	virtual bool init();
	void update(float deltaTime);
	void AddListener();

	bool OnTouchBegan(Touch* touch, Event* event);
	bool OnTouchEnded(Touch* touch, Event* event);
	void OnTouchMoved(Touch* touch, Event* event);

	bool onContactBegin(PhysicsContact& contact);
	void OpenInventory(cocos2d::Ref * sender);
	void CreateNPC();
	void RunActionNPC();
	void OpenWeaponShop();
	void OpenPotionShop();
	void OpenEquipmentShop();
	void CreateShop();
	void CreateAllButton(Layer* layer);
	void ClickShowInfor(Ref* pSender);

	CREATE_FUNC(HomeScene);
};

#endif
