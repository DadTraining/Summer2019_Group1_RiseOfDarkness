#include "Item.h"
#include "Inventory.h"
#include "ui/CocosGUI.h"
#include "cocos2d.h"
#include "MainCharacter.h"

USING_NS_CC;
using namespace std;
using namespace ui;
Inventory::Inventory(cocos2d::Sprite* sprite)
{
	Init(sprite);
	btnUse->removeFromParent();
	btnUse->setPosition(Vec2(btnUse->getContentSize().width / 2 - btnUse->getContentSize().width / 4,
		0 - btnUse->getContentSize().height / 4));
	btnSell->removeFromParent();
	btnSell->setPosition(Vec2(btnUse->getPositionX() + btnUse->getContentSize().width/2, btnUse->getPositionY()));
	GetTab(1)->addChild(btnUse, 99);
	GetTab(1)->addChild(btnSell, 99);
}

Inventory::~Inventory()
{
}

void Inventory::Init(cocos2d::Sprite* sprite)
{
	clickBox = Sprite::create("res/sprites/item/click.png");
	btnUse = ui::Button::create("res/sprites/item/buttonUse1.png", "res/sprites/item/buttonUse.png");
	btnUse->setScale(0.5);
	btnUse->retain();
	btnSell = ui::Button::create("res/sprites/item/buttonSell1.png", "res/sprites/item/buttonSell.png");
	btnSell->setScale(0.5);
	btnSell->retain();
	clickBox->setPosition(-500, -500);
	clickBox->retain();
	slotX = 6;
	slotY = 4;
	database = new ItemDatabase();
	database->Init();
	showTooltip = false;
	mSprite = sprite;
	tab = ui::TabControl::create();
	tab->setContentSize(Size(384, 325));
	tab->setHeaderHeight(69.f);
	tab->setHeaderWidth(70.f);
	tab->setHeaderSelectedZoom(.1f);
	tab->setHeaderDockPlace(ui::TabControl::Dock::TOP);
	weapon = cocos2d::ui::TabHeader::create();
	weapon->setTitleText("WEAPON");
	potion = cocos2d::ui::TabHeader::create();
	potion->setTitleText("POTION");
	armor = cocos2d::ui::TabHeader::create();
	armor->setTitleText("ARMOR");
	arrow = cocos2d::ui::TabHeader::create();
	arrow->setTitleText("ARROW");

	capacity = 24;
	container1 = ui::Layout::create();
	container1->setOpacity(255);
	container2 = ui::Layout::create();
	container2->setOpacity(50);
	container3 = ui::Layout::create();
	container3->setOpacity(50);
	arrowContainer = ui::Layout::create();
	arrowContainer->setOpacity(50);
	tab->insertTab(0, weapon, container1);
	tab->insertTab(1, potion, container2);
	tab->insertTab(2, armor, container3);
	tab->insertTab(3, arrow, arrowContainer);
	tab->setSelectTab(1);
	CC_SAFE_RETAIN(tab);
	CC_SAFE_RETAIN(container1);
	CC_SAFE_RETAIN(container2);
	CC_SAFE_RETAIN(container3);
	CC_SAFE_RETAIN(arrowContainer);

	for (int i = 0; i < slotX*slotY; i++)
	{
		inventory.push_back(new Item("sprites/item/box"));
		slots.push_back(new Item("sprites/item/box"));
		weapons.push_back(new Item("sprites/item/box"));
		arrows.push_back(new Item("sprites/item/box"));
		itemAmount.push_back(0);
		arrowAmount.push_back(0);
		auto label = Label::createWithTTF("", "fonts/Marker Felt.ttf", 14);
		label->retain();
		auto label1 = Label::createWithTTF("", "fonts/Marker Felt.ttf", 14);
		label1->retain();
		amountArrowLabels.push_back(label1);
		amountLabels.push_back(label);
	}
}

void Inventory::AddItem(int id)
{
	switch (database->items[id]->GetType())
	{
	case ItemType::potion:
		if (InventoryContains(id, ItemType::potion))
		{
			StackItem(id, ItemType::potion);
		}
		else
		{
			for (int i = 0; i < inventory.size(); i++)
			{
				if (slots[i]->GetID() == 99)
				{
					slots[i] = new Item(database->items[id]);
					slots[i]->GetIcon()->addClickEventListener(CC_CALLBACK_1(Inventory::ItemClick, this, i, ItemType::potion));
					itemAmount[i]++;
					break;
				}
			}
		}
		break;
	case ItemType::arrow:
		if (InventoryContains(id, ItemType::arrow))
		{
			StackItem(id, ItemType::arrow);
		}
		else
		{
			for (int i = 0; i < inventory.size(); i++)
			{
				if (arrows[i]->GetID() == 99)
				{
					arrows[i] = new Item(database->items[id]);
					arrows[i]->GetIcon()->addClickEventListener(CC_CALLBACK_1(Inventory::ItemClick, this, i, ItemType::arrow));
					arrowAmount[i]++;
					break;
				}
			}
		}
		break;
	case ItemType::weapon:
		for (int i = 0; i < inventory.size(); i++)
		{
			if (weapons[i]->GetID() == 99)
			{
				weapons[i] = new Item(database->items[id]);
				weapons[i]->GetIcon()->addClickEventListener(CC_CALLBACK_1(Inventory::ItemClick, this, i, ItemType::weapon));
				break;
			}
		}
		break;
	default:
		break;
	}
}

void Inventory::SellItem(int id)
{
}

void Inventory::RemoveItem(int id,int index,ItemType type)
{
	switch (type)
	{
	case ItemType::weapon:
		for (int  i = 0; i < inventory.size(); i++)
		{
			if (weapons[i]->GetID()==id && i==index)
			{
				GetTab(0)->removeChild(weapons[i]->GetIcon());
				targetID = -1;
				weapons[i] = new Item("sprites/item/box");

				break;
			}
		}
		break;
	case ItemType::potion:
		for (int i = 0; i < inventory.size(); i++)
		{
			if (slots[i]->GetID() == id && (id == 0 || id == 1) && slots[i]->GetID() != 99 && i == index)
			{
				if (MainCharacter::GetInstance()->TakePotion(id))
				{
					itemAmount[i]--;
					if (itemAmount[i] == 0)
					{
						amountLabels[i]->setString("");
						GetTab(1)->removeChild(slots[i]->GetIcon());
						targetID = -1;
						slots[i] = new Item("sprites/item/box");
						AutoArrange();
					}
					else
					{
						amountLabels[i]->setString(to_string(itemAmount[i]));
					}
					log("removed item %d", i);
					break;
				}
			}
			else
			{
				if (slots[i]->GetID() == id && i == index)
				{
					itemAmount[i]--;
					if (itemAmount[i] == 0)
					{
						amountLabels[i]->setString("");
						GetTab(1)->removeChild(slots[i]->GetIcon());
						targetID = -1;
						slots[i] = new Item("sprites/item/box");
						AutoArrange();
					}
					else
					{
						amountLabels[i]->setString(to_string(itemAmount[i]));
					}
					log("removed item %d", i);
					break;
				}
				
			}
		}
		break;
	case ItemType::arrow:
		for (int i = 0; i < inventory.size(); i++)
		{
			if (arrows[i]->GetID() == id && (id == 0 || id == 1) && i == index)
			{
				if (MainCharacter::GetInstance()->TakePotion(id))
				{
					arrowAmount[i]--;
					if (arrowAmount[i] == 0)
					{
						amountLabels[i]->setString("");
						GetTab(3)->removeChild(arrows[i]->GetIcon());
						targetID = -1;
						arrows[i] = new Item("sprites/item/box");
						AutoArrange();
					}
					else
					{
						amountLabels[i]->setString(to_string(arrowAmount[i]));
					}
					break;
				}
			}
			else
			{
				if (arrows[i]->GetID() == id && i == index)
				{
					arrowAmount[i]--;
					if (arrowAmount[i] == 0)
					{
						amountLabels[i]->setString("");
						GetTab(1)->removeChild(arrows[i]->GetIcon());
						targetID = -1;
						arrows[i] = new Item();
						AutoArrange();
					}
					else
					{
						amountLabels[i]->setString(to_string(arrowAmount[i]));
					}
					log("removed item %d", i);
					break;
				}
			}
		}
		break;
	default:
		break;
	}
}

cocos2d::Sprite * Inventory::GetClickBox()
{
	return clickBox;
}

void Inventory::SetVisible(bool b)
{
	tab->setVisible(b);
	mSprite->setVisible(b);
}

bool Inventory::IsVisible()
{
	return (tab->isVisible() && mSprite->isVisible());
}

void Inventory::SetSpritePosition(cocos2d::Vec2 pos)
{
	mSprite->setPosition(pos);
	tab->setPosition(pos);
}

cocos2d::Vec2 Inventory::GetSpritePosition()
{
	return mSprite->getPosition();
}

void Inventory::AddToLayer(cocos2d::Layer *layer)
{
	mSprite->removeFromParent();
	tab->removeFromParent();
	layer->addChild(mSprite, 16);
	layer->addChild(tab, 17);
}

cocos2d::ui::Layout *Inventory::GetTab(int tabIndex)
{
	switch (tabIndex)
	{
	case 0:
		return container1;
	case 1:
		return container2;
	case 2: 
		return container3;
	case 3:
		return arrowContainer;
	default:
		return container3;
		break;
	}
}

std::vector<Item*> Inventory::GetItems()
{
	return slots;
}

std::vector<Item*> Inventory::GetItemsWeapon()
{
	return weapons;
}

std::vector<int> Inventory::GetItemAmount(int type)
{
	return (type == 0 ? itemAmount : arrowAmount);
}

std::vector<Item*> Inventory::GetArrows()
{
	return arrows;
}

std::vector<Label*> Inventory::GetAmountLabel(int type)
{
	if (type==0)
	{
		return amountLabels;
	}
	else
	{
		return amountArrowLabels;
	}
}

cocos2d::Vec2 Inventory::GetSize()
{
	return tab->getContentSize();
}

int Inventory::GetIdByIcon(int id, ItemType type)
{
	switch (type)
	{
	case ItemType::weapon:
		for (int i = 0; i < weapons.size(); i++)
		{
			if (weapons[id]->GetIcon() != NULL)
			{
				return weapons[id]->GetID();
			}
		}
		return -1;
		break;
	case ItemType::potion:
		for (int i = 0; i < slots.size(); i++)
		{
			if (slots[i]->GetID() == id)
			{
				return i;
			}
		}
		return -1;
		break;
	default:
		return -1;
		break;
	}
	
	
}

void Inventory::StackItem(int id, ItemType type)
{
	switch (type)
	{
	case ItemType::potion:
		for (int i = 0; i < slots.size(); i++)
		{
			if (slots[i]->GetID() == id)
			{
				itemAmount[i]++;
				break;
			}
		}
	case ItemType::arrow:
		for (int i = 0; i < arrows.size(); i++)
		{
			if (arrows[i]->GetID() == id)
			{
				arrowAmount[i]++;
				break;
			}
		}
	default:
		break;
	}
}

void Inventory::AutoArrange()
{
	//ARRANGE POTION
	for (int i = 0; i < slots.size()-1; i++)
	{
		if (slots[i]->GetIcon()==NULL)
		{
			for (int j = i+1; j < slots.size(); j++)
			{
				if (slots[j]->GetIcon()!=NULL)
				{
					swap(slots[i], slots[j]);
					swap(itemAmount[i], itemAmount[j]);
					swap(amountLabels[i], amountLabels[j]);
					break;
				}
			}
		}
	}
	//ARRANGE WEAPON
	for (int i = 0; i < weapons.size() - 1; i++)
	{
		if (weapons[i]->GetIcon() == NULL)
		{
			for (int j = i + 1; j < weapons.size(); j++)
			{
				if (weapons[j]->GetIcon() != NULL)
				{
					swap(weapons[i], weapons[j]);
					break;
				}
			}
		}
	}
}

bool Inventory::InventoryContains(int id, ItemType type)
{
	bool result = false;
	switch (type)
	{
	case ItemType::potion:
		
		for (int i = 0; i < slots.size(); i++)
		{
			result = (slots[i]->GetID() == id && slots[i]->GetIcon() != NULL);
			if (result)
			{
				break;
			}
		}
		return result;
		break;
	case ItemType::arrow:
		for (int i = 0; i < arrows.size(); i++)
		{
			result = (arrows[i]->GetID() == id && arrows[i]->GetIcon() != NULL);
			if (result)
			{
				break;
			}
		}
		return result;
		break;
	default:
		return false;
		break;
	}
	
}

void Inventory::ItemClick(cocos2d::Ref *pSender, int id, ItemType type)
{
	switch (type)
	{
	case ItemType::potion:
		btnSell->removeFromParent();
		btnUse->removeFromParent();
		GetTab(1)->addChild(btnUse, 99);
		GetTab(1)->addChild(btnSell, 99);
		
		if (slots[id]->GetIcon() != NULL)
		{
			clickBox->removeFromParent();
			GetTab(1)->addChild(clickBox, 22);
			clickBox->setPosition(slots[id]->GetIcon()->getPosition());
		}
		targetID = id;
		if (targetID >= 0)
		{
			btnUse->addClickEventListener(CC_CALLBACK_1(Inventory::btnEquipInventory, this));
		}
		break;
	case ItemType::weapon:
		btnSell->removeFromParent();
		btnUse->removeFromParent();
		GetTab(0)->addChild(btnUse, 99);
		GetTab(0)->addChild(btnSell, 99);
		log("weapon %d click", id);
		
		if (weapons[id]->GetIcon() != NULL)
		{
			clickBox->removeFromParent();
			GetTab(0)->addChild(clickBox, 22);
			
			clickBox->setPosition(weapons[id]->GetIcon()->getPosition());
		}
		targetID = id;
		if (targetID >= 0)
		{
			btnUse->addClickEventListener(CC_CALLBACK_1(Inventory::EquipItem, this));
		}
		break;
	case ItemType::arrow:
		btnSell->removeFromParent();
		btnUse->removeFromParent();
		GetTab(3)->addChild(btnUse, 99);
		GetTab(3)->addChild(btnSell, 99);
		
		if (arrows[id]->GetIcon() != NULL)
		{
			clickBox->removeFromParent();
			GetTab(3)->addChild(clickBox, 22);

			clickBox->setPosition(arrows[id]->GetIcon()->getPosition());
		}
		targetID = id;
		if (targetID >= 0)
		{
			//btnUse->addClickEventListener(CC_CALLBACK_1(Inventory::EquipItem, this));
		}
		break;
	default:
		break;
	}
	
}

void Inventory::EquipItem(cocos2d::Ref *pSender)
{
	log("equiped this item");
	if (targetID >= 0)
	{
		RemoveItem(weapons[targetID]->GetID(), targetID,ItemType::weapon);
	}
}

void Inventory::btnEquipInventory(cocos2d::Ref *pSender)
{
	if (targetID>=0)
	{
		RemoveItem(slots[targetID]->GetID(), targetID,ItemType::potion);
	}
}
