#include "Battle.h"

ID3D11ShaderResourceView* Battle::mBG;
ID3D11ShaderResourceView* Battle::mBrainP;
ID3D11ShaderResourceView* Battle::mBrainCPU;
ID3D11ShaderResourceView* Battle::mBrainBar;
ID3D11ShaderResourceView* Battle::mBrainBarOL;

std::vector<Text*> Battle::mTitleText;

std::vector<Text*> Battle::mAbilityUsedText;
std::vector<Text*> Battle::mDumbAssUsedText;
std::vector<Text*> Battle::mText;

Text* Battle::mAbilityUsed; //Pops up when ability is used
Text* Battle::mDumbyUsed;   //ditto
Text* Battle::mTitle;		//sybolizes the state the battle is in 

int Battle::mWaitTimer;
bool Battle::mMove;
bool Battle::mState;

ID3D11Device* Battle::mDevice;

std::string Battle::mType;

Battle::Battle() : mVB(0), mIB(0), mTickTimer(0.0f), mWaitAndAssign(0), CpuMove(false), mPIQ(Player::mIQ), mCPIQ(MathHelper::RandF(50.0f,90.0f))
{
	Inventory::BuildBattleAbilitys();
	
	*StateMachine::pBattleState = BattleState::START;

	AssignTitle(); mWaitTimer = 2;

	//BACKGROUND
	Entity* BG = new Entity(4, "enemy", 1920.0f, 1080.0f, 1.0f); BG->UseTexture(mBG);
	BG->SetGrowIn(1.0f, true); mEntities.push_back(BG); BG->SetPos(0.0f, 0.0f, 100.0f);


	//BUILD ENEMY
	Entity* E = new Entity(4, "enemy", 650.0f, 750.0f, 1.0f);
	if (mType == "mDude1"){			E->UseTexture(LevelSection::mDude1);  E->SetUpAnimation(50, 1, 24, 1.0); }
	else if (mType == "mDude2"){	E->UseTexture(LevelSection::mDude2);  E->SetUpAnimation(50, 1, 24, 1.0); E->mAnim->Flip(true); }
	else if (mType == "mDude3"){	E->UseTexture(LevelSection::mDude3);  E->SetUpAnimation(50, 1, 24, 1.0); }
	else if (mType == "mChick1"){	E->UseTexture(LevelSection::mChick1); E->SetUpAnimation(48, 1, 24, 1.0); }
	else if (mType == "mChick2"){	E->UseTexture(LevelSection::mChick2); E->SetUpAnimation(50, 1, 24, 1.0); }
	else if (mType == "mChick3"){	E->UseTexture(LevelSection::mChick3); E->SetUpAnimation(50, 1, 24, 1.0); }
	else if (mType == "mChick4"){	E->UseTexture(LevelSection::mChick4); E->SetUpAnimation(50, 1, 24, 1.0); }
	E->SetPos(500.0f,-200.0f, 90.0f);
	E->SetGrowIn(2.0f, true);
	mEntities.push_back(E);

	//PLAYER
	Entity* B = nullptr;
	B = new Entity(4, "player", 650.0f, 750.0f, 1.0f); B->UseTexture(Player::GetSelf()->mTexSRV);
	B->SetUpAnimation(24, 8, 24, 1.0);
	B->SetPos(-500.0f, -200.0f, 90.0f);
	B->SetGrowIn(2.0f, true);
	mEntities.push_back(B);

	//PLAYERS BRAIN 
	B = new Entity(4, "brain", 250.0f, 200.0f, 1.0f); B->UseTexture(mBrainP); B->mFlipTexture = true; B->mBasicTexTrans = true;
	B->SetPos(-500.0f, 300.0f, 90.0f); B->SetGrowIn(2.0f, true); mEntities.push_back(B);
	//PLayers Brain Bar 
	B = new Entity(4, "PlayerBrain", 250.0f, 40.0f, 1.0f); B->UseTexture(mBrainBar); B->progressBar = true;
	B->SetPos(-500.0f, 300.0f, 80.0f); B->SetGrowIn(1.0f, true); mEntities.push_back(B);
	//PLayers Brain Bar OL
	B = new Entity(4, "brain", 250.0f, 40.0f, 1.0f); B->UseTexture(mBrainBarOL);
	B->SetPos(-500.0f, 300.0f, 70.0f); B->SetGrowIn(1.0f, true); mEntities.push_back(B);


	//CPU's BRAIN 
	B = new Entity(4, "brain", 250.0f, 200.0f, 1.0f); B->UseTexture(mBrainCPU); 
	B->SetPos(550.0f, 300.0f, 90.0f); B->SetGrowIn(2.0f, true); mEntities.push_back(B);
	//CPU's IQ BAR 
	B = new Entity(4, "CPUBrain", 250.0f, 40.0f, 1.0f); B->UseTexture(mBrainBar); B->progressBar = true;
	B->SetPos(550.0f, 300.0f, 80.0f); B->SetGrowIn(1.0f, true); mEntities.push_back(B);
	//CPU's IQ BAR Outline
	B = new Entity(4, "brain", 250.0f, 40.0f, 1.0f); B->UseTexture(mBrainBarOL);
	B->SetPos(550.0f, 300.0f, 70.0f); B->SetGrowIn(1.0f, true); mEntities.push_back(B);



	mTitle->SetGrowIn(1.5, true);

	mMove = true; //Initially true;
}


Battle::~Battle()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
	SafeVecEmpty(mEntities);
}

void Battle::Init(ID3D11Device** device)
{
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/battleBG.dds", 0, 0,  &mBG,    0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/brain.dds", 0, 0,     &mBrainP, 0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/brain.dds", 0, 0,     &mBrainCPU, 0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/bugsBar.dds", 0, 0,   &mBrainBar, 0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/BarOL.dds", 0, 0,     &mBrainBarOL, 0));
	mDevice = *device;

	Text* T = nullptr;
	/*0*/T = new Text("Entered Battle!/   Good Luck!", -280.0f, 450.0f, -90.0f, 60.0f, 0, false);		 mTitleText.push_back(T);
	/*1*/T = new Text("Players Turn!/Choose ability..", -280.0f, 450.0f, -90.0f, 60.0f, 0, false);		 mTitleText.push_back(T);
	/*2*/T = new Text("Computers Turn!/ What a Dumbass.", -280.0f, 450.0f, -90.0f, 60.0f, 0, false);	 mTitleText.push_back(T);
	/*3*/T = new Text("Good job!/ Dumbass to Smartass!.", -280.0f, 450.0f, -90.0f, 60.0f, 0, false);	 mTitleText.push_back(T);
	/*4*/T = new Text("Boo..Some People/ Dumbass for life!.", -280.0f, 450.0f, -90.0f, 60.0f, 0, false); mTitleText.push_back(T);

	//USED ABILITYS
	/*0*/T = new Text("Ability 1", -300.0f, 100.0f, -100.0f, 60.0f, 3, false);		mAbilityUsedText.push_back(T);
	/*1*/T = new Text("Ability 2", -300.0f, 100.0f, -100.0f, 60.0f, 3, false);		mAbilityUsedText.push_back(T);
	/*2*/T = new Text("Ability 3", -300.0f, 100.0f, -100.0f, 60.0f, 3, false);		mAbilityUsedText.push_back(T);
	/*3*/T = new Text("Ability 4", -300.0f, 100.0f, -100.0f, 60.0f, 3, false);		mAbilityUsedText.push_back(T);
	/*4*/T = new Text("Ability 5", -300.0f, 100.0f, -100.0f, 60.0f, 3, false);		mAbilityUsedText.push_back(T);
												
	/*0*/T = new Text("Ability 1", -100.0f, 100.0f, -100.0f, 60.0f, 4, false);		 mDumbAssUsedText.push_back(T);
	/*1*/T = new Text("Ability 2", -100.0f, 100.0f, -100.0f, 60.0f, 4, false);		 mDumbAssUsedText.push_back(T);
	/*2*/T = new Text("Ability 3", -100.0f, 100.0f, -100.0f, 60.0f, 4, false);		 mDumbAssUsedText.push_back(T);
	/*3*/T = new Text("Ability 4", -100.0f, 100.0f, -100.0f, 60.0f, 4, false);		 mDumbAssUsedText.push_back(T);
	/*4*/T = new Text("Ability 5", -100.0f, 100.0f, -100.0f, 60.0f, 4, false);		 mDumbAssUsedText.push_back(T);

}

void Battle::Update(const Camera& cam, float dt)
{
	//BATTLE TIMING
	mTickTimer += dt;
	if (mTickTimer > 1.0f)
	{
		if (mWaitTimer > 0){mWaitTimer--;}
		else
		{
			if (mMove){		MakeMove(); }
			if (mState){	AssignState(); }
			if (CpuMove){	CPUUsedAbility(); }
		}
		mTickTimer = 0.0f;
	}


	Inventory::UpdateAbilitys(cam, dt);
	if (mTitle)		  { mTitle->Update(cam, dt); }
	if (mAbilityUsed) { mAbilityUsed->Update(cam, dt);	if (mAbilityUsed->mDead){   mAbilityUsed = nullptr;  } }
	if (mDumbyUsed)   { mDumbyUsed->Update(cam, dt);	if (mDumbyUsed->mDead){		mDumbyUsed = nullptr; mMove = true; } }

	int ItemSpot = 0;
	for (int i = 0; i < mEntities.size(); i++)
	{
		if (mEntities[i]->mLabel == "PlayerBrain"){ mEntities[i]->currProgress = mPIQ / 140.0f; } //NORMALIZED VALUE = VALUE-MIN/MAX-MIN
		if (mEntities[i]->mLabel == "CPUBrain"){	mEntities[i]->currProgress = mCPIQ / 140.0f; } //NORMALIZED VALUE = VALUE-MIN/MAX-MIN
		mEntities[i]->Update(cam, dt);
	}
}
void Battle::Draw(ID3DX11EffectTechnique** activeTech, ID3D11DeviceContext* context, UINT pass, const Camera& camera, XMMATRIX& ortho)
{
	UINT stride = sizeof(Vertex::Basic32);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	context->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

	for (int i = 0; i < mEntities.size(); i++)
	{
		mEntities[i]->Draw2D(*activeTech, context, pass, camera, ortho);
	}

	//DRAW ALL TEXT AFTER THE ENTITIES .. THEY USE THEIER OWN Vertex and Index Buffers.
	if (mTitle)		  { mTitle->DrawText2D(activeTech, context, pass, camera, ortho); }
	if (mAbilityUsed) { mAbilityUsed->DrawText2D(activeTech, context, pass, camera, ortho); }
	if (mDumbyUsed)   { mDumbyUsed->DrawText2D(activeTech, context, pass, camera, ortho); }
	Inventory::DrawAbilitys(activeTech, context, pass, camera, ortho);
}

void Battle::AssignState()
{
	//Could Random Roll for either player or CPu to start
	switch (*StateMachine::pBattleState)
	{
	case BattleState::START:		*StateMachine::pBattleState = BattleState::PLAYERTURN;	AssignTitle(); mTitle->SetGrowIn(1.5, true);	Wait(2); mState = false; break;
	case BattleState::PLAYERTURN:	*StateMachine::pBattleState = BattleState::CPUTURN;		AssignTitle(); mTitle->SetGrowIn(0.3, true); 	Wait(3); CpuMove = true; mState = false; break;
	case BattleState::CPUTURN:		*StateMachine::pBattleState = BattleState::PLAYERTURN;	AssignTitle(); mTitle->SetGrowIn(1.5, true);	Wait(2); mState = false; break;
	case BattleState::PLAYERWIN:		break;
	case BattleState::PLAYERLOSE:		break;
	}
}

void Battle::UpdateIQs()
{

}

void Battle::MakeMove()
{
	switch (*StateMachine::pBattleState)
	{
	case BattleState::START:		 mTitle = nullptr; Wait(1); mMove = false; mState = true; 	break;
	case BattleState::PLAYERTURN:	 mTitle = nullptr; mMove = false; AssignState(); 	break; //TRIGGERS AFTER PLAYER CLICKED MOVE
	case BattleState::CPUTURN:		 mTitle = nullptr; Wait(1); mMove = false; mState = true;  break; //TRIGGER AFTER CPU MAKES TURN
	case BattleState::PLAYERWIN:		break;
	case BattleState::PLAYERLOSE:		break;
	}
}

void Battle::AssignTitle()
{
	switch (*StateMachine::pBattleState)
	{
	case BattleState::START:		mTitle = mTitleText[0];	break;
	case BattleState::PLAYERTURN:	mTitle = mTitleText[1];	break;
	case BattleState::CPUTURN:		mTitle = mTitleText[2];	break;
	case BattleState::PLAYERWIN:	mTitle = mTitleText[3];	break;
	case BattleState::PLAYERLOSE:	mTitle = mTitleText[4];	break;
	}
}


void Battle::UsedAbility(std::string ability)
{
	//DISECT THE STRING 
	if (ability == "beer"){ mAbilityUsed = mAbilityUsedText[0]; }

	if (mAbilityUsed)
	{
		mAbilityUsed->SetLife(false, 3.0f);
		mAbilityUsed->SetGrowIn(2.5, true);
		MakeMove();
	}
}

void Battle::CPUUsedAbility()
{
	int rand = MathHelper::RandI(0.0f, 4.0f);
	switch (rand)
	{
	case 0:	mDumbyUsed = mDumbAssUsedText[0]; break;
	case 1:	mDumbyUsed = mDumbAssUsedText[1]; break;
	case 2:	mDumbyUsed = mDumbAssUsedText[2]; break;
	case 3:	mDumbyUsed = mDumbAssUsedText[3]; break;
	case 4: mDumbyUsed = mDumbAssUsedText[4]; break;
	}
	mDumbyUsed->SetLife(false, 5.0f);
	mDumbyUsed->SetGrowIn(0.5, true);
	Wait(2);
	CpuMove = false;
}





void Battle::Wait(int i)
{
	mWaitTimer = i;
}

void Battle::DrawAbilitys(ID3DX11EffectTechnique** activeTech, ID3D11DeviceContext* context, UINT pass, const Camera& camera, XMMATRIX& ortho)
{
	Inventory::DrawAbilitys(activeTech, context, pass, camera, ortho);
}





void Battle::SetBattleType(std::string type)
{
	mType = type;
}

void Battle::ShutDown()
{
	SafeVecEmpty(mButtons);
	SafeVecEmpty(mText);
	SafeVecEmpty(mTitleText);
	ReleaseCOM(mBG);
	ReleaseCOM(mBrainP);
	mDevice = 0;
}