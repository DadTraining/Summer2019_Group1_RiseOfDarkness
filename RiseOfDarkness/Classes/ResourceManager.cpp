#include "ResourceManager.h"
#include <iostream>

ResourceManager * ResourceManager::m_instance = nullptr;

ResourceManager::ResourceManager() {}

ResourceManager* ResourceManager::GetInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new ResourceManager();
	}

	return m_instance;
}

ResourceManager::~ResourceManager()
{
	for (int i = 0; i < m_sprites.size(); i++)
	{
		m_sprites[i]->autorelease();
	}

	for (int i = 0; i < m_buttons.size(); i++)
	{
		m_sprites[i]->autorelease();
	}

	for (int i = 0; i < m_labels.size(); i++)
	{
		m_sprites[i]->autorelease();
	}

	for (int i = 0; i < m_actions.size(); i++)
	{
		m_actions[i]->autorelease();
	}

	for (int i = 0; i < m_tileMap.size(); i++)
	{
		m_tileMap[i]->autorelease();
	}

	for (int i = 0; i < m_loadingbar.size(); i++)
	{
		m_loadingbar[i]->autorelease();
	}
}

void ResourceManager::Init(const std::string &path)
{
	if (FileUtils::getInstance()->isFileExist(path))
	{
		std::string data, tempStr1, tempStr2;
		int amount;
		int count;
		float deltaTime;
		data = FileUtils::getInstance()->getStringFromFile(path);
		std::istringstream istr(data);

		// Sprite
		istr >> tempStr1 >> tempStr1;
		amount = std::stoi(tempStr1);
		for (int i = 0; i < amount; i++)
		{
			istr >> tempStr1 >> tempStr1;
			m_sprites.insert(std::pair<int, Sprite*>(i, Sprite::create(tempStr1)));
			m_sprites[i]->retain();
		}

		// Button
		istr >> tempStr1 >> tempStr1;
		amount = std::stoi(tempStr1);
		for (int i = 0; i < amount; i++)
		{
			istr >> tempStr1 >> tempStr1;
			istr >> tempStr2;
			m_buttons.insert(std::pair<int, ui::Button*>(i, ui::Button::create(tempStr1, tempStr2)));
			m_buttons[i]->retain();
		}

		// Animate
		istr >> tempStr1 >> tempStr1;
		amount = std::stoi(tempStr1);
		for (int i = 0; i < amount; i++)
		{
			istr >> tempStr1 >> tempStr1;
			deltaTime = std::stof(tempStr1);
			istr >> tempStr1;
			count = std::stoi(tempStr1);
			istr >> tempStr1;
			m_actions.insert(std::pair<int, Action* >(i, GetAction(tempStr1, count, deltaTime)));
			CC_SAFE_RETAIN(m_actions[i]);
		}

		// Tile Map
		istr >> tempStr1 >> tempStr1;
		amount = std::stoi(tempStr1);
		for (int i = 0; i < amount; i++)
		{
			istr >> tempStr1 >> tempStr1;
			m_tileMap.insert(std::pair<int, TMXTiledMap*>(i, TMXTiledMap::create(tempStr1)));
			m_tileMap[i]->retain();
		}

		// Label
		istr >> tempStr1 >> tempStr1;
		amount = std::stoi(tempStr1);
		for (int i = 0; i < amount; i++)
		{
			istr >> tempStr1 >> tempStr1;
			m_labels.insert(std::pair<int, Label*>(i, Label::createWithTTF("", tempStr1, 30)));
			m_labels[i]->retain();
		}

		// Loadingbar
		istr >> tempStr1 >> tempStr1;
		amount = std::stoi(tempStr1);
		for (int i = 0; i < amount; i++)
		{
			istr >> tempStr1 >> tempStr1;
			m_loadingbar.insert(std::pair<int, ui::LoadingBar*>(i, ui::LoadingBar::create(tempStr1)));
			m_loadingbar[i]->retain();
		}
	}
	auto mainCharacterFace = m_sprites[19];
	mainCharacterFace->setScale((Director::getInstance()->getVisibleSize().width / mainCharacterFace->getBoundingBox().size.width) / 20);

	auto infoBar = m_sprites[20];
	infoBar->setScale((Director::getInstance()->getVisibleSize().width / infoBar->getBoundingBox().size.width) / 5);

	auto hpBar = m_sprites[21];
	hpBar->setScale((Director::getInstance()->getVisibleSize().width / hpBar->getBoundingBox().size.width) / 8);

	auto mpBar = m_sprites[22];
	mpBar->setScale((Director::getInstance()->getVisibleSize().width / mpBar->getBoundingBox().size.width) / 8);

	auto hpLoadingBar = m_loadingbar[1];
	hpLoadingBar->setScale((Director::getInstance()->getVisibleSize().width / hpLoadingBar->getBoundingBox().size.width) / 8);

	auto mpLoadingBar = m_loadingbar[2];
	mpLoadingBar->setScale((Director::getInstance()->getVisibleSize().width / mpLoadingBar->getBoundingBox().size.width) / 8);
}

Sprite* ResourceManager::GetSpriteById(int id)
{
	return m_sprites[id];
}

ui::Button* ResourceManager::GetButtonById(int id)
{
	return m_buttons[id];
}

Label* ResourceManager::GetLabelById(int id)
{
	return m_labels[id];
}

Action* ResourceManager::GetActionById(int id)
{
	return m_actions[id];
}

TMXTiledMap* ResourceManager::GetTileMapById(int id)
{
	return m_tileMap[id];
}

ui::LoadingBar* ResourceManager::GetLoadingbar(int id)
{
	return m_loadingbar[id];
}

Sprite* ResourceManager::DuplicateSprite(Sprite* sprite)
{
	Sprite* pRet = Sprite::createWithTexture(sprite->getTexture());
	if (sprite->getChildrenCount() > 0)
	{
		for (int child = 0; child < sprite->getChildrenCount(); child++)
		{
			Sprite* spriteChild = (Sprite*)sprite->getChildren().at(child);
			Sprite* clone = DuplicateSprite((Sprite*)spriteChild);
			clone->boundingBox() = spriteChild->boundingBox();
			clone->setContentSize(spriteChild->getContentSize());
			clone->setPosition(spriteChild->getPosition());
			clone->setAnchorPoint(spriteChild->getAnchorPoint());
			pRet->addChild(clone, spriteChild->getZOrder(), spriteChild->getTag());
		}
	}
	return pRet;
}

Action* ResourceManager::GetAction(std::string &dir, int count, float deltaTime)
{
	auto spritecache = SpriteFrameCache::getInstance();
	spritecache->addSpriteFramesWithFile(dir);
	Vector<SpriteFrame*> animFrames;
	char str[100];
	for (int i = 1; i <= count; i++)
	{
		sprintf(str, "%02d.png", i);
		animFrames.pushBack(spritecache->getSpriteFrameByName(str));
	}
	spritecache->destroyInstance();
	return Repeat::create(Animate::create(Animation::createWithSpriteFrames(animFrames, deltaTime)), 1);
}