#include "Level3Scene.h"
#include "MainCharacter.h"
#include "ResourceManager.h"
#include "SpearMoblin.h"
#include "BowMoblin.h"
#include "MapScene.h"
#include "HomeScene.h"

USING_NS_CC;

Scene* Level3Scene::CreateScene()
{
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	auto layer = Level3Scene::create();

	scene->addChild(layer, -1);

	return scene;
}

bool Level3Scene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	MainCharacter::GetInstance()->Refresh();

	currentStage = 1;

	tileMap = ResourceManager::GetInstance()->GetTileMapById(7);
	upperTileMap = ResourceManager::GetInstance()->GetTileMapById(8);

	CreatePhysicsWorld("obstacles", "mc", this);

	CreateAllButton(this);

	AddListener();

	CreateMonster();

	CreateTreasure();

	scheduleUpdate();

	return true;
}

void Level3Scene::update(float deltaTime)
{
	UpdateInfoBar();

	MainCharacter::GetInstance()->Update(deltaTime);

	SetCamera(mainCharacter->getPosition());

	EnemyUpdate(deltaTime);

	if (CheckClear())
	{
		if (MainCharacter::GetInstance()->GetStageLevel() == currentStage)
		{
			Director::getInstance()->pause();
			MainCharacter::GetInstance()->IncreaseStage();
			clear->setVisible(true);
			m_buttons[4]->setVisible(false);
			m_buttons[5]->setVisible(false);
			m_buttons[6]->setVisible(true);
			m_buttons[7]->setVisible(true);
		}
	}
	if (!MainCharacter::GetInstance()->IsAlive())
	{
		m_buttons[4]->setVisible(false);
		m_buttons[5]->setVisible(false);
		m_buttons[6]->setVisible(true);
		m_buttons[7]->setVisible(true);
		gameover->setVisible(true);
	}

	UpdateJoystick();

	MainCharacter::GetInstance()->GetFlySlash()->Update(deltaTime);

	gold->setString(std::to_string(MainCharacter::GetInstance()->GetGold()));
}

void Level3Scene::CreateMonster()
{
	float x1, y1;
	int direction1;
	auto spearGoblinGroup = tileMap->getObjectGroup("spearMoblin");
	int amount1 = 4;
	char str1[10];
	for (int i = 1; i <= amount1; i++)
	{
		sprintf(str1, "%02d", i);
		direction1 = spearGoblinGroup->getObject(str1)["direction"].asInt();
		x1 = spearGoblinGroup->getObject(str1)["x"].asFloat();
		y1 = spearGoblinGroup->getObject(str1)["y"].asFloat();
		SpearMoblin *spearMoblin = new SpearMoblin(this, direction1, Vec2(x1, y1));
		spearMoblin->GetPhysicsBody()->setGroup(i - 1);
		m_enemies.push_back(spearMoblin);
	}

	float x2, y2;
	int direction2;
	auto bowGoblinGroup = tileMap->getObjectGroup("bowMoblin");
	int amount2 = 3;
	char str2[10];
	for (int i = 1; i <= amount2; i++)
	{
		sprintf(str2, "%02d", i);
		direction2 = bowGoblinGroup->getObject(str2)["direction"].asInt();
		x2 = bowGoblinGroup->getObject(str2)["x"].asFloat();
		y2 = bowGoblinGroup->getObject(str2)["y"].asFloat();
		BowMoblin *bowMoblin = new BowMoblin(this, direction2, Vec2(x2, y2), i - 1 + amount1);
		bowMoblin->GetPhysicsBody()->setGroup(i - 1 + amount1);
		m_enemies.push_back(bowMoblin);
	}
}

void Level3Scene::AddListener()
{
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(Level3Scene::OnTouchBegan, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(Level3Scene::OnTouchEnded, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(Level3Scene::OnTouchMoved, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	// SKILLS
	m_buttons[1]->addTouchEventListener(CC_CALLBACK_2(Level3Scene::SpecialAttack, this));
	m_buttons[2]->addTouchEventListener(CC_CALLBACK_2(Level3Scene::Evade, this));
	m_buttons[0]->addTouchEventListener(CC_CALLBACK_2(Level3Scene::NormalAttack, this));
	m_buttons[3]->addTouchEventListener(CC_CALLBACK_2(Level3Scene::Defend, this));

	// PAUSE GAME
	m_buttons[4]->addClickEventListener([&](Ref* event) {
		if (!m_buttons[5]->isVisible())
		{
			m_buttons[4]->setVisible(false);
			m_buttons[5]->setVisible(true);
			m_buttons[6]->setVisible(true);
			m_buttons[7]->setVisible(true);
			paused->setVisible(true);
			shader->setOpacity(200);
			Director::getInstance()->pause();
		}
	});

	// RESUME GAME
	m_buttons[5]->addClickEventListener([&](Ref* event) {
		m_buttons[4]->setVisible(true);
		m_buttons[5]->setVisible(false);
		m_buttons[6]->setVisible(false);
		m_buttons[7]->setVisible(false);
		shader->setOpacity(0);
		paused->setVisible(false);
		Director::getInstance()->resume();
	});

	// GO TO HOMESCENE
	m_buttons[6]->addClickEventListener([&](Ref* event) {
		Director::getInstance()->resume();
		auto gotoMap = CallFunc::create([] {
			Director::getInstance()->replaceScene(HomeScene::CreateScene());
		});
		runAction(gotoMap);
	});

	// RESTART GAME STAGE
	m_buttons[7]->addClickEventListener([&](Ref* event) {
		Director::getInstance()->resume();
		auto gotoMap = CallFunc::create([] {
			Director::getInstance()->replaceScene(Level3Scene::CreateScene());
		});
		runAction(gotoMap);
	});

	// USE HP POTION
	m_buttons[9]->addClickEventListener([&](Ref* event) {
		int index = MainCharacter::GetInstance()->GetInventory()->GetIdByIcon(21, ItemType::potion);
		MainCharacter::GetInstance()->GetInventory()->RemoveItem(22, index, ItemType::potion);
	});

	// USE MP POTION
	m_buttons[10]->addClickEventListener([&](Ref* event) {
		int index = MainCharacter::GetInstance()->GetInventory()->GetIdByIcon(21, ItemType::potion);
		MainCharacter::GetInstance()->GetInventory()->RemoveItem(22, index, ItemType::potion);
	});
	

	// INVENTORY
	m_buttons[11]->addClickEventListener(CC_CALLBACK_1(Level3Scene::OpenInventory, this));

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(Level3Scene::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

bool Level3Scene::OnTouchBegan(Touch* touch, Event* event)
{
	return true;
}

bool Level3Scene::OnTouchEnded(Touch* touch, Event* event)
{
	return true;
}

void Level3Scene::OnTouchMoved(Touch* touch, Event* event)
{

}

bool Level3Scene::onContactBegin(PhysicsContact& contact)
{
	PhysicsBody* a = contact.getShapeA()->getBody();
	PhysicsBody* b = contact.getShapeB()->getBody();

	// MAIN CHARACTER WITH OBSTACLES
	if ((a->getCollisionBitmask() == MainCharacter::MAIN_CHARACTER_BITMASK && b->getCollisionBitmask() == MainCharacter::OBSTACLE_BITMASK)
		|| (a->getCollisionBitmask() == MainCharacter::OBSTACLE_BITMASK && b->getCollisionBitmask() == MainCharacter::MAIN_CHARACTER_BITMASK))
	{
		if (MainCharacter::GetInstance()->GetCurrentState() == MainCharacter::ROLL_BACK || MainCharacter::GetInstance()->GetCurrentState() == MainCharacter::ROLL_LEFT ||
			MainCharacter::GetInstance()->GetCurrentState() == MainCharacter::ROLL_FRONT)
		{
			mainCharacter->stopAllActions();
		}
		if (MainCharacter::GetInstance()->GetDirection() == 1)
		{
			mainCharacter->setPositionY(mainCharacter->getPositionY() - MainCharacter::GetInstance()->GetSpeed());
			MainCharacter::GetInstance()->SetPreventRun(1);
		}
		else if (MainCharacter::GetInstance()->GetDirection() == 2)
		{
			mainCharacter->setPositionY(mainCharacter->getPositionY() + MainCharacter::GetInstance()->GetSpeed());
			MainCharacter::GetInstance()->SetPreventRun(2);
		}
		else if (MainCharacter::GetInstance()->GetDirection() == 3)
		{
			mainCharacter->setPositionX(mainCharacter->getPositionX() + MainCharacter::GetInstance()->GetSpeed());
			MainCharacter::GetInstance()->SetPreventRun(3);
		}
		else if (MainCharacter::GetInstance()->GetDirection() == 4)
		{
			mainCharacter->setPositionX(mainCharacter->getPositionX() - MainCharacter::GetInstance()->GetSpeed());
			MainCharacter::GetInstance()->SetPreventRun(4);
		}
		else if (MainCharacter::GetInstance()->GetDirection() == 5)
		{
			mainCharacter->setPosition(Vec2(mainCharacter->getPositionX() + MainCharacter::GetInstance()->GetPace(),
				mainCharacter->getPositionY() - MainCharacter::GetInstance()->GetPace()));
			MainCharacter::GetInstance()->SetPreventRun(5);
		}
		else if (MainCharacter::GetInstance()->GetDirection() == 6)
		{
			mainCharacter->setPosition(Vec2(mainCharacter->getPositionX() + MainCharacter::GetInstance()->GetPace(),
				mainCharacter->getPositionY() + MainCharacter::GetInstance()->GetPace()));
			MainCharacter::GetInstance()->SetPreventRun(6);
		}
		else if (MainCharacter::GetInstance()->GetDirection() == 7)
		{
			mainCharacter->setPosition(Vec2(mainCharacter->getPositionX() - MainCharacter::GetInstance()->GetPace(),
				mainCharacter->getPositionY() - MainCharacter::GetInstance()->GetPace()));
			MainCharacter::GetInstance()->SetPreventRun(7);
		}
		else if (MainCharacter::GetInstance()->GetDirection() == 8)
		{
			mainCharacter->setPosition(Vec2(mainCharacter->getPositionX() - MainCharacter::GetInstance()->GetPace(),
				mainCharacter->getPositionY() + MainCharacter::GetInstance()->GetPace()));
			MainCharacter::GetInstance()->SetPreventRun(8);
		}
	}

	// ARROW COLLIDE WITH OBSTACLE
	if ((a->getCollisionBitmask() == MainCharacter::NORMAL_ARROW_BITMASK && b->getCollisionBitmask() == MainCharacter::OBSTACLE_BITMASK)
		|| (a->getCollisionBitmask() == MainCharacter::OBSTACLE_BITMASK && b->getCollisionBitmask() == MainCharacter::NORMAL_ARROW_BITMASK))
	{
		if (a->getCollisionBitmask() == MainCharacter::NORMAL_ARROW_BITMASK)
		{
			MainCharacter::GetInstance()->GetListArrow()[a->getGroup()]->SetVisible(false);
		}
		else if (b->getCollisionBitmask() == MainCharacter::NORMAL_ARROW_BITMASK)
		{
			MainCharacter::GetInstance()->GetListArrow()[b->getGroup()]->SetVisible(false);
		}
	}

	// MAIN CHARACTER SLASH SPEARMOBLIN
	if ((a->getCollisionBitmask() == MainCharacter::SLASH_BITMASK && b->getCollisionBitmask() == MainCharacter::SPEARMOBLIN_BITMASK)
		|| (a->getCollisionBitmask() == MainCharacter::SPEARMOBLIN_BITMASK && b->getCollisionBitmask() == MainCharacter::SLASH_BITMASK))
	{
		if (a->getCollisionBitmask() == MainCharacter::SPEARMOBLIN_BITMASK)
		{
			m_enemies[a->getGroup()]->GetDamage(MainCharacter::GetInstance()->GetAttack());
		}
		else if (b->getCollisionBitmask() == MainCharacter::SPEARMOBLIN_BITMASK)
		{
			m_enemies[b->getGroup()]->GetDamage(MainCharacter::GetInstance()->GetAttack());
		}
	}

	// MAIN CHARACTER'S ARROW COLLIDE SPEARMOBLIN
	if ((a->getCollisionBitmask() == MainCharacter::NORMAL_ARROW_BITMASK && b->getCollisionBitmask() == MainCharacter::SPEARMOBLIN_BITMASK)
		|| (a->getCollisionBitmask() == MainCharacter::SPEARMOBLIN_BITMASK && b->getCollisionBitmask() == MainCharacter::NORMAL_ARROW_BITMASK))
	{
		if (a->getCollisionBitmask() == MainCharacter::SPEARMOBLIN_BITMASK)
		{
			m_enemies[a->getGroup()]->GetDamage(MainCharacter::NORMAL_ARROW);
			MainCharacter::GetInstance()->GetListArrow()[b->getGroup()]->SetVisible(false);
		}
		else if (b->getCollisionBitmask() == MainCharacter::SPEARMOBLIN_BITMASK)
		{
			m_enemies[b->getGroup()]->GetDamage(MainCharacter::NORMAL_ARROW);
			MainCharacter::GetInstance()->GetListArrow()[a->getGroup()]->SetVisible(false);
		}
	}

	// SPEARMOBLIN PIERCE MAIN CHARACTER
	if ((a->getCollisionBitmask() == MainCharacter::PIERCE_BITMASK && b->getCollisionBitmask() == MainCharacter::MAIN_CHARACTER_BITMASK)
		|| (a->getCollisionBitmask() == MainCharacter::MAIN_CHARACTER_BITMASK && b->getCollisionBitmask() == MainCharacter::PIERCE_BITMASK))
	{
		MainCharacter::GetInstance()->GetDamage(MainCharacter::SPEARMOBLIN_DAMAGE);
	}

	// BOWMOBLIN ARROW DAMAGE MAIN CHARACTER
	if ((a->getCollisionBitmask() == MainCharacter::BOWMOBLIN_ARROW_BITMASK && b->getCollisionBitmask() == MainCharacter::MAIN_CHARACTER_BITMASK)
		|| (a->getCollisionBitmask() == MainCharacter::MAIN_CHARACTER_BITMASK && b->getCollisionBitmask() == MainCharacter::BOWMOBLIN_ARROW_BITMASK))
	{
		MainCharacter::GetInstance()->GetDamage(MainCharacter::BOWMOBLIN_DAMAGE);
		if (a->getCollisionBitmask() == MainCharacter::BOWMOBLIN_ARROW_BITMASK)
		{
			m_enemies[a->getGroup()]->GetArrow()->SetVisible(false);
		}
		else if (b->getCollisionBitmask() == MainCharacter::BOWMOBLIN_ARROW_BITMASK)
		{
			m_enemies[b->getGroup()]->GetArrow()->SetVisible(false);
		}
	}

	// SPEARMOBLIN COLLIDE OBSTACLES
	if ((a->getCollisionBitmask() == MainCharacter::OBSTACLE_BITMASK && b->getCollisionBitmask() == MainCharacter::SPEARMOBLIN_BITMASK)
		|| (a->getCollisionBitmask() == MainCharacter::SPEARMOBLIN_BITMASK && b->getCollisionBitmask() == MainCharacter::OBSTACLE_BITMASK))
	{
		if (a->getCollisionBitmask() == MainCharacter::SPEARMOBLIN_BITMASK)
		{
			m_enemies[a->getGroup()]->SetPreventRun();
			m_enemies[a->getGroup()]->ReverseDirection();
		}
		else if (b->getCollisionBitmask() == MainCharacter::SPEARMOBLIN_BITMASK)
		{
			m_enemies[b->getGroup()]->SetPreventRun();
			m_enemies[b->getGroup()]->ReverseDirection();
		}
	}

	//// BOWMOBLIN ARROW COLLIDE OBSTACLES
	if ((a->getCollisionBitmask() == MainCharacter::BOWMOBLIN_ARROW_BITMASK && b->getCollisionBitmask() == MainCharacter::OBSTACLE_BITMASK)
		|| (a->getCollisionBitmask() == MainCharacter::OBSTACLE_BITMASK && b->getCollisionBitmask() == MainCharacter::BOWMOBLIN_ARROW_BITMASK))
	{
		if (a->getCollisionBitmask() == MainCharacter::BOWMOBLIN_ARROW_BITMASK)
		{
			m_enemies[a->getGroup()]->GetArrow()->SetVisible(false);
		}
		else if (b->getCollisionBitmask() == MainCharacter::BOWMOBLIN_ARROW_BITMASK)
		{
			m_enemies[b->getGroup()]->GetArrow()->SetVisible(false);
		}
	}

	// COLLECT HEART CONTAINER
	if ((a->getCollisionBitmask() == MainCharacter::MAIN_CHARACTER_BITMASK && b->getCollisionBitmask() == MainCharacter::HEART_CONTAINER_BITMASK)
		|| (a->getCollisionBitmask() == MainCharacter::HEART_CONTAINER_BITMASK && b->getCollisionBitmask() == MainCharacter::MAIN_CHARACTER_BITMASK))
	{
		if (heartContainer->isVisible())
		{
			MainCharacter::GetInstance()->TakeHeartContainer();
			heartContainer->setVisible(false);
		}
	}

	// FLY SLASH COLLIDE OBSTACLES
	if ((a->getCollisionBitmask() == MainCharacter::FLY_SLASH_BITMASK && b->getCollisionBitmask() == MainCharacter::OBSTACLE_BITMASK)
		|| (a->getCollisionBitmask() == MainCharacter::OBSTACLE_BITMASK && b->getCollisionBitmask() == MainCharacter::FLY_SLASH_BITMASK))
	{
		MainCharacter::GetInstance()->GetFlySlash()->Disappear();
	}

	// FLY SLASH DAMAGE SPEARMOBLIN
	if ((a->getCollisionBitmask() == MainCharacter::FLY_SLASH_BITMASK && b->getCollisionBitmask() == MainCharacter::SPEARMOBLIN_BITMASK)
		|| (a->getCollisionBitmask() == MainCharacter::SPEARMOBLIN_BITMASK && b->getCollisionBitmask() == MainCharacter::FLY_SLASH_BITMASK))
	{
		if (MainCharacter::GetInstance()->GetFlySlash()->GetSprite()->isVisible())
		{
			if (a->getCollisionBitmask() == MainCharacter::SPEARMOBLIN_BITMASK)
			{
				m_enemies[a->getGroup()]->GetDamage(MainCharacter::FLY_SLASH_DAMAGE);
			}
			else if (b->getCollisionBitmask() == MainCharacter::SPEARMOBLIN_BITMASK)
			{
				m_enemies[b->getGroup()]->GetDamage(MainCharacter::FLY_SLASH_DAMAGE);
			}
		}
	}

	// FLY SLASH DAMAGE BOWMOBLIN
	if ((a->getCollisionBitmask() == MainCharacter::FLY_SLASH_BITMASK && b->getCollisionBitmask() == MainCharacter::BOWMOBLIN_BITMASK)
		|| (a->getCollisionBitmask() == MainCharacter::BOWMOBLIN_BITMASK && b->getCollisionBitmask() == MainCharacter::FLY_SLASH_BITMASK))
	{
		if (MainCharacter::GetInstance()->GetFlySlash()->GetSprite()->isVisible())
		{
			if (a->getCollisionBitmask() == MainCharacter::SPEARMOBLIN_BITMASK)
			{
				m_enemies[a->getGroup()]->GetDamage(MainCharacter::FLY_SLASH_DAMAGE);
			}
			else if (b->getCollisionBitmask() == MainCharacter::SPEARMOBLIN_BITMASK)
			{
				m_enemies[b->getGroup()]->GetDamage(MainCharacter::FLY_SLASH_DAMAGE);
			}
		}
	}
	return true;
}

void Level3Scene::NormalAttack(Ref* sender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		MainCharacter::GetInstance()->NormalAttack();

	}
}

void Level3Scene::SpecialAttack(Ref* sender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		MainCharacter::GetInstance()->SpecialAttack();
	}
}

void Level3Scene::Evade(Ref* sender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		MainCharacter::GetInstance()->Evade();
	}
}

void Level3Scene::Defend(Ref* sender, ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::BEGAN)
	{
		MainCharacter::GetInstance()->Defend();
	}
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		MainCharacter::GetInstance()->StopDefend();
	}
}

void Level3Scene::OpenInventory(cocos2d::Ref * sender)
{
	GamePlay::ShowInventoryGrid();
	MainCharacter::GetInstance()->GetInventory()->SetVisible(
		!(MainCharacter::GetInstance()->GetInventory()->IsVisible())
	);
}

void Level3Scene::CreateTreasure()
{
	auto heartContainerGroup = tileMap->getObjectGroup("heartContainer");
	heartContainer = ResourceManager::GetInstance()->GetSpriteById(29);
	heartContainer->setPosition(Vec2(heartContainerGroup->getObject("heartContainer")["x"].asFloat()
		, heartContainerGroup->getObject("heartContainer")["y"].asFloat()));
	this->addChild(heartContainer);

	auto physics = PhysicsBody::createBox(heartContainer->getContentSize(), PhysicsMaterial(0, 0, 0));
	physics->setRotationEnable(false);
	physics->setCollisionBitmask(MainCharacter::HEART_CONTAINER_BITMASK);
	physics->setDynamic(false);
	physics->setContactTestBitmask(true);
	physics->setGravityEnable(false);
	heartContainer->setPhysicsBody(physics);
}