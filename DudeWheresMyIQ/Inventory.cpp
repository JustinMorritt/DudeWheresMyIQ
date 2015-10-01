#include "Inventory.h"
#include "Player.h"

ID3D11ShaderResourceView* Inventory::mBG;
ID3D11ShaderResourceView* Inventory::mBeer;
ID3D11ShaderResourceView* Inventory::mEmpty;
ID3D11Device* Inventory::mDevice;

ID3D11Buffer* Inventory::mVB2;
ID3D11Buffer* Inventory::mIB2;

std::vector<std::string> Inventory::mItemStrings;

std::vector<std::string> Inventory::mAbilityStrings;
std::vector<std::string> Inventory::mBattleStrings;

std::vector<Entity*> Inventory::mItems;

std::vector<Entity*> Inventory::mItemButtons;

std::vector<Entity*> Inventory::mAbilitys;
std::vector<Entity*> Inventory::mAbilityButtons;
std::vector<Entity*> Inventory::mBattleButtons;

std::vector<Text*> Inventory::mTitles;
std::vector<Text*> Inventory::mText; //Descriptions

Text* Inventory::mItemDescription;

int Inventory::mSlots;

Inventory::Inventory()
{
	UpdateStats();
	float size = 150.0f;
	float baseX = 0.0f;
	float currX = baseX;
	float currY = 200.0f;

	//INVENTORY SLOTS
	for (int i = 0; i < mSlots; i++)
	{
		if (currX > size * 5){ currX = baseX;  currY -= size; } // 5 per row 
		MakeInv(currX, currY, size/1.2);
		currX += size;
	}

	//ABILITIES
	baseX = -800.0f;
	currX = baseX;
	currY = 250.0f;
	for (int i = 0; i < 10; i++)
	{
		if (currX > baseX + size / 1.4f){ currX = baseX;  currY -= size / 1.4f; } // 2 per row 
		MakeAbility(currX, currY, size / 1.8);
		currX += size / 1.4f;
	}

}

Inventory::~Inventory()
{
}

void Inventory::Update(const Camera& cam, float dt)
{
	//DESCRIPTIONS
	for (int i = 0; i < mText.size(); i++)
	{
		mText[i]->Update(cam, dt);
	}
	//TITLES
	for (int i = 0; i < mTitles.size(); i++)
	{
		mTitles[i]->Update(cam, dt);
	}

	//ITEMS
	int ItemSpot = 0;
	for (int i = 0; i < mItems.size(); i++)
	{
		if (mItems[i]->mLabel != "inv")
		{
			AssignItem(mItems[i], mItemStrings[ItemSpot]); ItemSpot++;
		}
		mItems[i]->Update(cam, dt);
	}

	//ABILITIES
	int AbSpot = 0;
	for (int i = 0; i < mAbilitys.size(); i++)
	{
		if (mAbilitys[i]->mLabel != "ability")
		{
			AssignAbility(mAbilitys[i], mAbilityStrings[AbSpot]); AbSpot++;
		}
		mAbilitys[i]->Update(cam, dt);
	}
}

void Inventory::UpdateStats()
{
	std::string LV = std::to_string(Player::mLevel);
	std::string IQ = std::to_string(Player::mIQ);
	std::string SP = std::to_string(Player::mMaxSpeed);
	std::string JH = std::to_string(Player::mJumpHeight);

	mTitles[2]->Rebuild("Lv " + LV + " ",	-400.0f, 400.0f,	-90.0f, 50.0f, 0, false); // TITLE2
	mTitles[3]->Rebuild(IQ + " ",			-400.0f, 200.0f,	-90.0f, 50.0f, 0, false); // TITLE3
	mTitles[4]->Rebuild(JH + " ",			-400.0f,   0.0f,	-90.0f, 50.0f, 0, false); // TITLE4
	mTitles[5]->Rebuild(SP + " ",			-400.0f, -200.0f,	-90.0f, 50.0f, 0, false); // TITLE5
}

void Inventory::UpdateAbilitys(const Camera& cam, float dt)
{
	//DESCRIPTIONS
	for (int i = 0; i < mText.size(); i++)
	{
		mText[i]->Update(cam, dt);
	}
	//TITLES
	for (int i = 0; i < mTitles.size(); i++)
	{
		mTitles[i]->Update(cam, dt);
	}

	//ABILITIES
	int AbSpot = 0;
	for (int i = 0; i < mAbilitys.size(); i++)
	{
		if (mAbilitys[i]->mLabel != "ability")
		{
			AssignAbility(mAbilitys[i], mBattleStrings[AbSpot]); AbSpot++;
		}
		mAbilitys[i]->Update(cam, dt);
	}
}

void Inventory::Draw(ID3DX11EffectTechnique** activeTech, ID3D11DeviceContext* context, UINT pass, const Camera& camera, XMMATRIX& ortho)
{
	UINT stride = sizeof(Vertex::Basic32);
	UINT offset = 0;
	

	//ITEMS
	context->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	context->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	for (int i = 0; i < mItems.size(); i++)
	{
		mItems[i]->Draw2D(*activeTech, context, pass, camera, ortho);
	}



	//ABILITIES
	context->IASetVertexBuffers(0, 1, &mVB2, &stride, &offset);
	context->IASetIndexBuffer(mIB2, DXGI_FORMAT_R32_UINT, 0);
	for (int i = 0; i < mAbilitys.size(); i++)
	{
		mAbilitys[i]->Draw2D(*activeTech, context, pass, camera, ortho);
	}



	//DRAW ALL TEXT AFTER ENTITYS
	if (mItemDescription){ mItemDescription->DrawText2D(activeTech, context, pass, camera, ortho); }
	for (int i = 0; i < mTitles.size(); i++)
	{
		mTitles[i]->DrawText2D(activeTech, context, pass, camera, ortho);
	}
}

void Inventory::DrawAbilitys(ID3DX11EffectTechnique** activeTech, ID3D11DeviceContext* context, UINT pass, const Camera& camera, XMMATRIX& ortho)
{
	UINT stride = sizeof(Vertex::Basic32);
	UINT offset = 0;

	//ABILITIES
	context->IASetVertexBuffers(0, 1, &mVB2, &stride, &offset);
	context->IASetIndexBuffer(mIB2, DXGI_FORMAT_R32_UINT, 0);
	for (int i = 0; i < mAbilitys.size(); i++)
	{
		mAbilitys[i]->Draw2D(*activeTech, context, pass, camera, ortho);
	}
	mTitles[1]->DrawText2D(activeTech, context, pass, camera, ortho);
	if (mItemDescription){ mItemDescription->DrawText2D(activeTech, context, pass, camera, ortho); }
}

void Inventory::Init(ID3D11Device** device)
{
	mSlots = 18;
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/inventory.dds", 0, 0, &mBG, 0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/beer.dds", 0, 0, &mBeer, 0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/empty.dds", 0, 0, &mEmpty, 0));
	mDevice = *device;

	Text* T = nullptr;
	T = new Text("Inventory",  200.0f, 400.0f, -90.0f, 100.0f, 0, false);		mTitles.push_back(T);	// TITLE0
	T = new Text("Abilities", -825.0f, 400.0f, -90.0f, 50.0f, 0, false);		mTitles.push_back(T);	// TITLE1
	T = new Text(" ",			-400.0f, 400.0f, -90.0f, 50.0f, 0, false);		mTitles.push_back(T);	// TITLE2
	T = new Text(" ",			-400.0f, 200.0f, -90.0f, 50.0f, 0, false);		mTitles.push_back(T);	// TITLE3
	T = new Text(" ",			-400.0f, 0.0f, -90.0f, 50.0f, 0, false);		mTitles.push_back(T);	// TITLE4
	T = new Text(" ",			-400.0f, -200.0f, -90.0f, 50.0f, 0, false);		mTitles.push_back(T);	// TITLE5
	T = new Text("IQ? ",	  -400.0f,  300.0f, -90.0f, 50.0f, 1, false);		mTitles.push_back(T);	// TITLE6
	T = new Text("Speed? ",	  -400.0f,  100.0f, -90.0f, 50.0f, 1, false);		mTitles.push_back(T);	// TITLE7
	T = new Text("Jump? ",	  -400.0f, -100.0f, -90.0f, 50.0f, 1, false);		mTitles.push_back(T);	// TITLE8


	//MAKE TEXT DESCRIPTIONS
	Text* t = new Text("Drink This Beer And you may or may not get smarter ...", -650, -400.0f, 90.0f, 50.0f, 0, false); mText.push_back(t);
}


std::vector<Entity*> Inventory::GetItems()
{
	return mItemButtons;
}

void Inventory::MakeInv(float x, float y, float size)
{
	Entity* E = new Entity(2, "inv", size, size);  E->SetPos(x, y, -90.0f); E->UseTexture(mBG);		E->reverseLook = true; mItems.push_back(E);
	Entity* B = new Entity(2, "empty", size, size); B->SetPos(x, y, -90.0f); B->UseTexture(mEmpty);	B->reverseLook = true; mItems.push_back(B); mItemButtons.push_back(B);
	mItemStrings.push_back("");
}

void Inventory::MakeAbility(float x, float y, float size)
{
	Entity* E = new Entity(2, "ability", size, size);   E->SetPos(x, y, -90.0f); E->UseTexture(mBG);	E->reverseLook = true; mAbilitys.push_back(E);
	Entity* B = new Entity(2, "empty", size, size);		B->SetPos(x, y, -90.0f); B->UseTexture(mEmpty); B->reverseLook = true; mAbilitys.push_back(B); mAbilityButtons.push_back(B);
	mAbilityStrings.push_back("");
}

bool Inventory::AddToInventory(std::string item)
{
	for (int i = 0; i < mItemStrings.size(); i++)
	{
		if (mItemStrings[i] == ""){ mItemStrings[i] = item; return true;}
	}
	return false;
}

bool Inventory::AddToAbilitys(std::string item)
{
	for (int i = 0; i < mAbilityStrings.size(); i++)
	{
		if (mAbilityStrings[i] == ""){ mAbilityStrings[i] = item; return true; }
	}
	return false;
}

void Inventory::AssignItem(Entity* E, std::string item)
{
	if (item == "beer"){ E->UseTexture(mBeer); E->mLabel = "beer"; }
	else if (item == "water"){ }
	else{ E->UseTexture(mEmpty); }
}

void Inventory::AssignAbility(Entity* E, std::string item)
{
	if (item == "beer"){ E->UseTexture(mBeer); E->mLabel = "beer"; }
	else if (item == "water"){}
	else{ E->UseTexture(mEmpty); }
}

void Inventory::HoveringItem(int E)
{
	mItemButtons[E]->hovering = true;
	SetDescription(mItemButtons[E]->mLabel);
}

void Inventory::HoveringAbility(int E)
{
	mAbilityButtons[E]->hovering = true;
	SetDescription(mItemButtons[E]->mLabel);
}

void Inventory::NotHovering(int E)
{
	mItemButtons[E]->hovering = false;
}

void Inventory::NotHoveringAbility(int E)
{
	mAbilityButtons[E]->hovering = false;
}

void Inventory::BuildBattleAbilitys()
{
	//Empty Battle Strings Before Use in Battle..
	mBattleStrings.clear();

	//Insert New Battle Strings
	for (int i = 0; i < mAbilityStrings.size(); i++)
	{
		mBattleStrings.push_back(mAbilityStrings[i]);
	}

	//CLEAR OLD BATTLE BUTTONS 
	for (int i = 0; i < mBattleButtons.size(); i++)
	{
		mBattleButtons[i] = nullptr;
	}
	mBattleButtons.clear();

	//INSERT NEW BATTLE BUTTONS 
	for (int i = 0; i < mAbilityButtons.size(); i++)
	{
		Entity* E = mAbilityButtons[i];
		mBattleButtons.push_back(E);
	}
}

void Inventory::SetDescription(std::string item)
{
	if (item == ""){ mItemDescription = nullptr; return; }
	if (item == "beer"){ mItemDescription = mText[0]; }
}

void Inventory::SetAbilityDescription(std::string item)
{
	if (item == ""){ mItemDescription = nullptr; return; }
	if (item == "beer"){ mItemDescription = mText[0]; } //CHANGE THESE UP DEPENDING ON ABILITY NAME
}

std::vector<Text*> Inventory::GetText()
{
	return mText;
}

void Inventory::UseItem(int E)
{
	if (mItemButtons[E]->mLabel == "beer"){/*Beer Has Been Used*/mItemButtons[E]->mLabel = "empty"; }

	
	mItemButtons[E]->UseTexture(mEmpty);
	mItemStrings[E] = "";
}

void Inventory::UseAbility(int E)
{
	if (mAbilityButtons[E]->mLabel == "beer"){/*Beer Has Been Used*/mItemButtons[E]->mLabel = "empty"; }


	mItemButtons[E]->UseTexture(mEmpty);
	mItemStrings[E] = "";
}
