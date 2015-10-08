#include "Inventory.h"
#include "Player.h"
#include "Engine.h"

ID3D11ShaderResourceView* Inventory::mBG;
ID3D11ShaderResourceView* Inventory::mBeer;
ID3D11ShaderResourceView* Inventory::mPill;
ID3D11ShaderResourceView* Inventory::mMoonshine;
ID3D11ShaderResourceView* Inventory::mApple;
ID3D11ShaderResourceView* Inventory::mRubix;
ID3D11ShaderResourceView* Inventory::mFish;
ID3D11ShaderResourceView* Inventory::mFishRod;
ID3D11ShaderResourceView* Inventory::mLight;
ID3D11ShaderResourceView* Inventory::mBook;
ID3D11ShaderResourceView* Inventory::mChicken;
ID3D11ShaderResourceView* Inventory::mBandaid;
ID3D11ShaderResourceView* Inventory::mFire;
ID3D11ShaderResourceView* Inventory::mTools;

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

Inventory::Inventory() : mVB(0), mIB(0)
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
	baseX = -830.0f;
	currX = baseX;
	currY = 250.0f;
	for (int i = 0; i < 10; i++)
	{
		if (currX > baseX + size / 1.4f){ currX = baseX;  currY -= size / 1.4f; } // 2 per row 
		MakeAbility(currX, currY, size / 1.8);
		currX += size / 1.4f;
	}
	Engine::BuildVertexAndIndexBuffer(&mVB, &mIB, mItems);			// Inventory
	Engine::BuildVertexAndIndexBuffer(&mVB2, &mIB2, mAbilitys);		// Abilities
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
			AssignItem(mAbilitys[i], mAbilityStrings[AbSpot]); AbSpot++;
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

	mTitles[2]->Rebuild("Lv " + LV + " ",	-460.0f, 400.0f,	-90.0f, 40.0f, 0, false); // TITLE2
	mTitles[3]->Rebuild(IQ + " ",			-460.0f, 200.0f,	-90.0f, 40.0f, 0, false); // TITLE3
	mTitles[4]->Rebuild(SP + " ",			-460.0f,   0.0f,	-90.0f, 40.0f, 0, false); // TITLE4
	mTitles[5]->Rebuild(JH + " ",			-460.0f, -200.0f,	-90.0f, 40.0f, 0, false); // TITLE5
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
			AssignItem(mAbilitys[i], mBattleStrings[AbSpot]); AbSpot++;
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
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/inventory.dds",		0, 0, &mBG, 0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/beer.dds",			0, 0, &mBeer, 0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/moonshine.dds",		0, 0, &mMoonshine, 0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/apple.dds",			0, 0, &mApple, 0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/pill.dds",			0, 0, &mPill, 0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/empty.dds",			0, 0, &mEmpty, 0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/book.dds",			0, 0, &mBook, 0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/rubix.dds",			0, 0, &mRubix, 0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/lightbulb.dds",		0, 0, &mLight, 0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/fishing.dds",			0, 0, &mFishRod, 0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/fish.dds",			0, 0, &mFish, 0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/chicken.dds",			0, 0, &mChicken, 0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/bandaid.dds",			0, 0, &mBandaid, 0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/fire.dds",			0, 0, &mFire, 0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/tools.dds",			0, 0, &mTools, 0));
	mDevice = *device;

	Text* T = nullptr;
	T = new Text("Inventory",  200.0f, 400.0f, -90.0f, 80.0f, 0, false);		mTitles.push_back(T);	// TITLE0
	T = new Text("Abilities", -845.0f, 350.0f, -90.0f, 30.0f, 0, false);		mTitles.push_back(T);	// TITLE1
	T = new Text("test",		-460.0f, 400.0f, -90.0f, 50.0f, 0, false);		mTitles.push_back(T);	// TITLE2
	T = new Text("test",		-460.0f, 200.0f, -90.0f, 50.0f, 0, false);		mTitles.push_back(T);	// TITLE3
	T = new Text("test",		-460.0f, 0.0f, -90.0f, 50.0f, 0, false);		mTitles.push_back(T);	// TITLE4
	T = new Text("test",		-460.0f, -200.0f, -90.0f, 50.0f, 0, false);		mTitles.push_back(T);	// TITLE5
	T = new Text("IQ? ",	  -460.0f,  300.0f, -90.0f, 50.0f, 1, false);		mTitles.push_back(T);	// TITLE6
	T = new Text("Speed? ",	  -460.0f,  100.0f, -90.0f, 50.0f, 1, false);		mTitles.push_back(T);	// TITLE7
	T = new Text("Jump? ",	  -460.0f, -100.0f, -90.0f, 50.0f, 1, false);		mTitles.push_back(T);	// TITLE8


	//MAKE TEXT DESCRIPTIONS FOR ITEMS
	Text* t = nullptr;
	/*0*/t = new Text("Drink This Beer And you may or may not get smarter ...",												-750, -400.0f, -100.0f, 50.0f, 0, false);	mText.push_back(t);
	/*1*/t = new Text("Mmm Pills Are Good .../lower iq for speed and jump?",												-750, -400.0f, -100.0f, 50.0f, 0, false);	mText.push_back(t);
	/*2*/t = new Text("Moonshine a day keeps the doctor away.../This could make you really dumb!",							-750, -400.0f, -100.0f, 50.0f, 0, false);	mText.push_back(t);
	/*3*/t = new Text("Did this thing create gravity ? .../increase Iq by 1ish.",											-750, -400.0f, -100.0f, 50.0f, 0, false);	mText.push_back(t);
	/*4*/t = new Text("Thomas Edison was one smart fellow! not me though../increase Iq by 2ish.",							-750, -400.0f, -100.0f, 50.0f, 0, false);	mText.push_back(t);
	/*5*/t = new Text("Tasty fish will give you mad hops!/increase Iq by 2ish and Jump by 3ish.",							-750, -400.0f, -100.0f, 50.0f, 0, false);	mText.push_back(t);
	/*6*/t = new Text("Never heard any of this mcs music?/increase Iq by 3ish.",											-750, -400.0f, -100.0f, 50.0f, 0, false);	mText.push_back(t);
	/*7*/t = new Text("Solve one of these puppies and you have patience../increase Iq by 4ish..But may slow you down.",		-750, -400.0f, -100.0f, 50.0f, 0, false);	mText.push_back(t);
	/*8*/t = new Text("Teach a man to fish and he wont eat as many big macs./increase Iq by 4ish..But may slow you down.",	-750, -400.0f, -100.0f, 50.0f, 0, false);	mText.push_back(t);
	/*9*/t = new Text("Chickens Lay some tasty thingies./increase Iq by 4ish..But may slow you down.",						-750, -400.0f, -100.0f, 50.0f, 0, false);	mText.push_back(t);
	/*10*/t = new Text("Bandaids keep ur booboo from oozing liquids ./increase Iq by 4ish..But may slow you down.",			-750, -400.0f, -100.0f, 50.0f, 0, false);	mText.push_back(t);
	/*11*/t = new Text("Man learn fire ... man take over world!/increase Iq by 4ish..But may slow you down.",				-750, -400.0f, -100.0f, 50.0f, 0, false);	mText.push_back(t);
	/*12*/t = new Text("Learn how to fix things.. or break them worse..",													-750, -400.0f, -100.0f, 50.0f, 0, false);	mText.push_back(t);

	//TEXT DESCRIPTIONS FOR ABILITYS 
	/*13*/t = new Text("Teach them a valuable fix that/ could change their lives forever!", -750, -400.0f, -100.0f, 40.0f, 0, false);		mText.push_back(t);
	/*14*/t = new Text("You could use this to catch fish .../ or grenades!",				-750, -400.0f, -100.0f, 40.0f, 0, false);		mText.push_back(t);
	/*14*/t = new Text("If you scratch your head for/long enough this could happen...",				-750, -400.0f, -100.0f, 40.0f, 0, false);		mText.push_back(t);
}

void Inventory::UseItem(int E)
{
	if (mItemButtons[E]->mLabel == "beer"){				AddToIQ(MathHelper::RandF(-5.0f, 5.0f));	mItemButtons[E]->mLabel = "empty"; }
	else if (mItemButtons[E]->mLabel == "pill"){		AddToIQ(MathHelper::RandF(-5.0f, 0.0f));	AddToJump(MathHelper::RandF(5.0f, 10.0f)); AddToSpeed(MathHelper::RandF(5.0f, 10.0f));	mItemButtons[E]->mLabel = "empty"; }
	else if (mItemButtons[E]->mLabel == "moonshine"){	AddToIQ(MathHelper::RandF(1.0f, -10.0f));	mItemButtons[E]->mLabel = "empty"; }
	else if (mItemButtons[E]->mLabel == "apple"){		AddToIQ(MathHelper::RandF(0.0f, 2.0f));		mItemButtons[E]->mLabel = "empty"; }
	else if (mItemButtons[E]->mLabel == "light"){		AddToIQ(MathHelper::RandF(1.0f, 3.0f));		mItemButtons[E]->mLabel = "empty"; }
	else if (mItemButtons[E]->mLabel == "fish"){		AddToIQ(MathHelper::RandF(1.0f, 3.0f));		AddToJump(MathHelper::RandF(2.0f, 5.0f));		mItemButtons[E]->mLabel = "empty"; }
	else if (mItemButtons[E]->mLabel == "book"){		AddToIQ(MathHelper::RandF(2.0f, 4.0f));														mItemButtons[E]->mLabel = "empty"; }
	else if (mItemButtons[E]->mLabel == "rubix"){		AddToIQ(MathHelper::RandF(2.0f, 5.0f));		AddToSpeed(MathHelper::RandF(-2.0f, -5.0f));	mItemButtons[E]->mLabel = "empty"; }
	else if (mItemButtons[E]->mLabel == "fishrod"){		AddToAbilitys("fishrod");					AddToIQ(MathHelper::RandF(2.0f, 5.0f));			AddToSpeed(MathHelper::RandF(-2.0f, -5.0f));	mItemButtons[E]->mLabel = "empty"; }
	else if (mItemButtons[E]->mLabel == "chicken"){		AddToIQ(MathHelper::RandF(2.0f, 5.0f));		AddToSpeed(MathHelper::RandF(-2.0f, -5.0f));	mItemButtons[E]->mLabel = "empty"; }
	else if (mItemButtons[E]->mLabel == "bandaid"){		AddToIQ(MathHelper::RandF(2.0f, 5.0f));		AddToSpeed(MathHelper::RandF(-2.0f, -5.0f));	mItemButtons[E]->mLabel = "empty"; }
	else if (mItemButtons[E]->mLabel == "fire"){		AddToAbilitys("fire");						AddToIQ(MathHelper::RandF(2.0f, 5.0f));		AddToSpeed(MathHelper::RandF(-2.0f, -5.0f));	mItemButtons[E]->mLabel = "empty"; }
	else if (mItemButtons[E]->mLabel == "tools"){		AddToAbilitys("tools");						mItemButtons[E]->mLabel = "empty"; }

	UpdateStats();
	mItemButtons[E]->UseTexture(mEmpty);
	mItemStrings[E] = "";
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
	Entity* B = new Entity(2, "empty", size, size);		B->SetPos(x, y, -100.0f); B->UseTexture(mEmpty); B->reverseLook = true; mAbilitys.push_back(B); mAbilityButtons.push_back(B); mBattleButtons.push_back(B);
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
		if (mAbilityStrings[i] == item){ return true; }
		if (mAbilityStrings[i] == ""){ mAbilityStrings[i] = item; return true; }
	}
	return false;
}

void Inventory::AssignItem(Entity* E, std::string item)
{
	if (item == "beer"){					E->UseTexture(mBeer);				E->mLabel = "beer"; }
	else if (item == "pill"){				E->UseTexture(mPill);				E->mLabel = "pill"; }
	else if (item == "moonshine"){			E->UseTexture(mMoonshine);			E->mLabel = "moonshine"; }
	else if (item == "apple"){				E->UseTexture(mApple);				E->mLabel = "apple"; }
	else if (item == "light"){				E->UseTexture(mLight);				E->mLabel = "light"; }
	else if (item == "fish"){				E->UseTexture(mFish);				E->mLabel = "fish"; }
	else if (item == "book"){				E->UseTexture(mBook);				E->mLabel = "book"; }
	else if (item == "rubix"){				E->UseTexture(mRubix);				E->mLabel = "rubix"; }
	else if (item == "fishrod"){			E->UseTexture(mFishRod);			E->mLabel = "fishrod"; }
	else if (item == "chicken"){			E->UseTexture(mChicken);			E->mLabel = "chicken"; }
	else if (item == "bandaid"){			E->UseTexture(mBandaid);			E->mLabel = "bandaid"; }
	else if (item == "fire"){				E->UseTexture(mFire);				E->mLabel = "fire"; }
	else if (item == "tools"){				E->UseTexture(mTools);				E->mLabel = "tools"; }
	else{ E->UseTexture(mEmpty); }
}

void Inventory::InventoryOn()
{
	UpdateStats();
	//ITEMS
	int ItemSpot = 0;
	for (int i = 0; i < mItems.size(); i++)
	{
		mItems[i]->SetGrowIn(1.5, true);
	}

	//ABILITIES
	for (int i = 0; i < mAbilitys.size(); i++)
	{
		mAbilitys[i]->SetGrowIn(1.5, true);
	}
	*StateMachine::pGameState = GameState::INVENTORY;
}

void Inventory::HoveringItem(int E)
{
	mItemButtons[E]->hovering = true;
	SetDescription(mItemButtons[E]->mLabel);
}

void Inventory::HoveringAbility(int E)
{
	
	if (*StateMachine::pGameState == GameState::BATTLE)
	{
		mBattleButtons[E]->hovering = true;
		SetAbilityDescription(mBattleButtons[E]->mLabel);
	}
	else
	{
		mAbilityButtons[E]->hovering = true;
		SetAbilityDescription(mAbilityButtons[E]->mLabel);
	}
}

void Inventory::NotHovering(int E)
{
	if (*StateMachine::pGameState == GameState::BATTLE)
	{
		mBattleButtons[E]->hovering = false;
	}
	else
	{
		mItemButtons[E]->hovering = false;
	}
}

void Inventory::AddToIQ(float num)
{
	Player::mIQ += num;
	if (Player::mIQ > 140.0f){ Player::mIQ = 140.0f; }
	if (Player::mIQ < 0.0f){	Player::mIQ = 0.0f; }
}

void Inventory::AddToJump(float num)
{
	Player::mJumpHeight += num;
}

void Inventory::AddToSpeed(float num)
{
	Player::mMaxSpeed += num;
}

void Inventory::NotHoveringAbility(int E)
{
	if (*StateMachine::pGameState == GameState::BATTLE)
	{
		mBattleButtons[E]->hovering = false;
	}
	else
	{
		mAbilityButtons[E]->hovering = false;
	}
}

void Inventory::BuildBattleAbilitys()
{
	for (int i = 0; i < mAbilitys.size(); i++)
	{
		mAbilitys[i]->SetGrowIn(1.5, true);
	}

	//Empty Battle Strings Before Use in Battle..
	mBattleStrings.clear();

	//Insert New Battle Strings
	for (int i = 0; i < mAbilityStrings.size(); i++)
	{
		std::string str = mAbilityStrings[i];
		mBattleStrings.push_back(str);
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
	else if (item == "beer"){	mItemDescription		= mText[0]; }
	else if(item == "pill"){ mItemDescription			= mText[1]; }
	else if(item == "moonshine"){ mItemDescription		= mText[2]; }
	else if(item == "apple"){ mItemDescription			= mText[3]; }
	else if(item == "light"){ mItemDescription			= mText[4]; }
	else if(item == "fish"){ mItemDescription			= mText[5]; }
	else if(item == "book"){ mItemDescription			= mText[6]; }
	else if(item == "rubix"){ mItemDescription			= mText[7]; }
	else if(item == "fishrod"){ mItemDescription		= mText[8]; }
	else if(item == "chicken"){ mItemDescription		= mText[9]; }
	else if(item == "bandaid"){ mItemDescription		= mText[10]; }
	else if(item == "fire"){	mItemDescription		= mText[11]; }
	else if(item == "tools"){	mItemDescription		= mText[12]; }

}

void Inventory::SetAbilityDescription(std::string item)
{
	if (item == ""){ mItemDescription = nullptr; return; }
	if (item == "tools"){	mItemDescription = mText[13]; } //CHANGE THESE UP DEPENDING ON ABILITY NAME
	if (item == "fishrod"){ mItemDescription = mText[14]; }
	if (item == "fire"){	mItemDescription = mText[15]; }
}

int Inventory::NumAbilitys()
{
	int num = 0;
	for (int i = 0; i < mBattleStrings.size(); i++)
	{
		if (mBattleStrings[i] != ""){ num++; }
	}
	return num;
}

std::vector<Text*> Inventory::GetText()
{
	return mText;
}

void Inventory::UseAbility(int E)
{
	mBattleButtons[E]->mLabel = "empty"; 
	mBattleButtons[E]->UseTexture(mEmpty);
	mBattleStrings[E] = "";
}
