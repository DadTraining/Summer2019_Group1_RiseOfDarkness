#include "Slash.h"
#include "MainCharacter.h"
#include "ResourceManager.h"

Slash::Slash()
{
	mSprite = ResourceManager::GetInstance()->DuplicateSprite(ResourceManager::GetInstance()->GetSpriteById(23));
	mSprite->setVisible(false);
	mSprite->retain();
	mPhysicsBody = PhysicsBody::createBox(mSprite->getContentSize(), PhysicsMaterial(0, 0, 0));
	mPhysicsBody->setRotationEnable(false);
	mPhysicsBody->setCollisionBitmask(MainCharacter::SLASH_BITMASK);
	mPhysicsBody->setDynamic(false);
	mPhysicsBody->setContactTestBitmask(true);
	mPhysicsBody->setGravityEnable(false);
	mSprite->setPhysicsBody(mPhysicsBody);
}

Slash::~Slash() {}

void Slash::AddToLayer(Layer* layer)
{
	layer->addChild(mSprite, 1);

}

Sprite* Slash::GetSprite()
{
	return mSprite;
}

PhysicsBody* Slash::GetPhysicsBody()
{
	return mPhysicsBody;
}