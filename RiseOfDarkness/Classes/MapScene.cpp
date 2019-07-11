#include "MapScene.h"
#include "ui/CocosGUI.h"
#include "Level1Scene.h"
#include "SelectScene.h"

using namespace std;
USING_NS_CC;

int lv = 11;
vector<ui::Button*> buttonList;
Scene* MapScene::CreateScene()
{
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
	auto layer = MapScene::create();

	scene->addChild(layer);

	return scene;
}
bool MapScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto btnMap = TMXTiledMap::create("res/tiledMaps/SelectScene/selectMap.tmx");
	btnMap->setAnchorPoint(Point(0, 0));
	btnMap->setPosition(0,0);
	btnMap->setScale(visibleSize.width / (btnMap->getMapSize().width*btnMap->getTileSize().width),
		visibleSize.height / (btnMap->getMapSize().height*btnMap->getTileSize().height));
	addChild(btnMap, 0);
	auto background = Sprite::create("res/tiledMaps/SelectScene/background.png");
	background->setAnchorPoint(Vec2(0.5, 0.5));
	background->setPosition(Point(visibleSize/2));
	
	auto btnBack = ui::Button::create("res/tiledMaps/SelectScene/btnBack.png");
	btnBack->setAnchorPoint(Point(1, 1));
	btnBack->setScale(0.12);
	btnBack->setPosition(Vec2(visibleSize.width, visibleSize.height));
	addChild(btnBack, 1);
	btnBack->addClickEventListener(CC_CALLBACK_1(MapScene::btnBackCallback, this));
	TMXObjectGroup *btnGroup = btnMap->objectGroupNamed("ObjButton");

	for (int i = 0; i < 11; i++)
	{
		auto btnPos = btnGroup->getObject("btnLv"+to_string(i+1));
		int x = btnPos["x"].asInt();
		int y = btnPos["y"].asInt();
		auto btn = ui::Button::create("res/tiledMaps/SelectScene/x.png");
		btn->setPosition(Point(x, y));
		btn->setScale(0.5);
		btn->setVisible(false);
		addChild(btn, 1);
		btn->addClickEventListener(CC_CALLBACK_1(MapScene::LoadLevel, this, i+1));
		buttonList.push_back(btn);
	}

	for (int i = 0; i < lv; i++)
	{
		if (i>11)
		{
			break;
		}
		if (!buttonList[i]->isVisible())
		{
			buttonList[i]->setVisible(true);
		}
	}

	return true;
}

void MapScene::LoadLevel(Ref *pSender,int level)
{
	switch (level)
	{
	case 1:
		
		Director::getInstance()->replaceScene(Level1Scene::CreateScene());
		
	default:
		break;
	}
}

void MapScene::CreateMap()
{

}

void MapScene::btnBackCallback(cocos2d::Ref * pSender)
{
	buttonList.clear();
	Director::getInstance()->replaceScene(SelectScene::CreateScene());
}

void MapScene::onTouchEnded(Touch *touch, Event *unused_event)
{
}