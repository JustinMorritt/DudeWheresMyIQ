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
Text* Battle::mLeaveMsg;

int Battle::mWaitTimer;
bool Battle::mMove;
bool Battle::mState;

ID3D11Device* Battle::mDevice;

std::string Battle::mType;

Battle::Battle() : mVB(0), mIB(0), mTickTimer(0.0f), mWaitAndAssign(0), CpuMove(false), mPIQ(Player::mIQ), mCPIQ(MathHelper::RandF(30.0f,90.0f))
{
	Inventory::BuildBattleAbilitys();
	
	*StateMachine::pBattleState = BattleState::START;

	AssignTitle(); mWaitTimer = 2;

	//BACKGROUND
	Entity* BG = new Entity(4, "enemy", 1920.0f, 1080.0f, 1.0f); BG->UseTexture(mBG);
	BG->SetGrowIn(1.0f, true); mEntities.push_back(BG); BG->SetPos(0.0f, 0.0f, 100.0f);

	mLeaveMsg->SetGrowIn(0.5f, true);

	//BUILD ENEMY
	Entity* E = new Entity(4, "enemy", 630.0f, 750.0f, 1.0f);
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
	B = new Entity(4, "player", 630.0f, 750.0f, 1.0f); B->UseTexture(Player::GetSelf()->mTexSRV);
	B->SetUpAnimation(24, 8, 24, 1.0);
	B->SetPos(-500.0f, -200.0f, 90.0f);
	B->SetGrowIn(2.0f, true);
	mEntities.push_back(B);

	//PLAYERS BRAIN 
	B = new Entity(4, "brain", 450.0f, 300.0f, 1.0f); B->UseTexture(mBrainP);
	B->SetPos(-500.0f, 300.0f, 90.0f); B->SetGrowIn(2.0f, true); mEntities.push_back(B);
	//PLayers Brain Bar 
	B = new Entity(4, "PlayerBrain", 350.0f, 45.0f, 1.0f); B->UseTexture(mBrainBar); B->progressBar = true;
	B->SetPos(-500.0f, 300.0f, 80.0f); B->SetGrowIn(1.0f, true); mEntities.push_back(B);
	//PLayers Brain Bar OL
	B = new Entity(4, "brain", 350.0f, 45.0f, 1.0f); B->UseTexture(mBrainBarOL);
	B->SetPos(-500.0f, 300.0f, 70.0f); B->SetGrowIn(1.0f, true); mEntities.push_back(B);


	//CPU's BRAIN 
	B = new Entity(4, "brain", 450.0f, 300.0f, 1.0f); B->UseTexture(mBrainCPU); 
	B->SetPos(550.0f, 300.0f, 90.0f); B->SetGrowIn(2.0f, true); mEntities.push_back(B);
	//CPU's IQ BAR 
	B = new Entity(4, "CPUBrain", 350.0f, 45.0f, 1.0f); B->UseTexture(mBrainBar); B->progressBar = true;
	B->SetPos(550.0f, 300.0f, 80.0f); B->SetGrowIn(1.0f, true); mEntities.push_back(B);
	//CPU's IQ BAR Outline
	B = new Entity(4, "brain", 350.0f, 45.0f, 1.0f); B->UseTexture(mBrainBarOL);
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
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/bar.dds", 0, 0,   &mBrainBar, 0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/barOL.dds", 0, 0,     &mBrainBarOL, 0));
	mDevice = *device;

	mLeaveMsg = new Text("press B to run!", -100.0f, -500.0f, -90.0f, 30.0f, 2, false);

	Text* T = nullptr;
	/*0*/T = new Text("Entered Battle!/   Good Luck!/", -240.0f, 450.0f, -90.0f, 60.0f, 0, false);		 mTitleText.push_back(T);
	/*1*/T = new Text("Players Turn!/Choose ability../", -240.0f, 450.0f, -90.0f, 60.0f, 0, false);		 mTitleText.push_back(T);
	/*2*/T = new Text("Computers Turn!/ What a Dumbass./", -240.0f, 450.0f, -90.0f, 60.0f, 0, false);	 mTitleText.push_back(T);
	/*3*/T = new Text("Good job!/ Dumbass to Smartass!.// B to leave./", -340.0f, 450.0f, -90.0f, 60.0f, 0, false);	 mTitleText.push_back(T);
	/*4*/T = new Text("Boo..Some People/ Dumbass for life!. // B to leave./", -240.0f, 450.0f, -90.0f, 60.0f, 0, false); mTitleText.push_back(T);

	//USED ABILITYS
	/*0*/T = new Text("You can use duck tape/ to hold the battery case/ on your remote control eh !/", -300.0f, 100.0f, -100.0f, 35.0f, 3, false);		mAbilityUsedText.push_back(T);
	/*1*/T = new Text("If you catch a fish with 3 eyes ..../ definetly eat it!/",						-300.0f, 100.0f, -100.0f, 35.0f, 3, false);		mAbilityUsedText.push_back(T);
	/*2*/T = new Text("Rub a couple sticks together/ make a fire.../ then hop on in!/",					-300.0f, 100.0f, -100.0f, 35.0f, 3, false);		mAbilityUsedText.push_back(T);
	/*3*/T = new Text("Ability 4", -300.0f, 100.0f, -100.0f, 35.0f, 3, false);		mAbilityUsedText.push_back(T);
	/*4*/T = new Text("Ability 5", -300.0f, 100.0f, -100.0f, 35.0f, 3, false);		mAbilityUsedText.push_back(T);
			
	//Computer Abilitys 
	/*0*/T = new Text("If brains were leather/ you wouldn’t have enough/ to saddle a June bug./",-250.0f, 100.0f, -100.0f, 35.0f, 4, false);	mDumbAssUsedText.push_back(T);
	/*1*/T = new Text("Your so dumb you could/ throw yourself to the ground/ and miss../",		-250.0f, 100.0f, -100.0f, 35.0f, 4, false);		mDumbAssUsedText.push_back(T);
	/*2*/T = new Text("I think you are a few sandwiches / short of a piknik.../",				-250.0f, 100.0f, -100.0f, 35.0f, 4, false);		mDumbAssUsedText.push_back(T);
	/*3*/T = new Text("If rabbits feet are so lucky .../ what happened to the rabbit?/",		-250.0f, 100.0f, -100.0f, 35.0f, 4, false);		mDumbAssUsedText.push_back(T);
	/*4*/T = new Text("If everything is coming your way.../you are in the wrong lane!/ ",		-250.0f, 100.0f, -100.0f, 35.0f, 4, false);		mDumbAssUsedText.push_back(T);
	/*5*/T = new Text("Can you explain why my toilet/ paper has page numbers on it?!/ ",		-250.0f, 100.0f, -100.0f, 35.0f, 4, false);		mDumbAssUsedText.push_back(T);
	/*6*/T = new Text("I been thinking about installing/ an ashtray on my motorcycle.../ ",		-250.0f, 100.0f, -100.0f, 35.0f, 4, false);		mDumbAssUsedText.push_back(T);
	/*7*/T = new Text("Ever been skinny dippin with snappin turtles?/ ",						-250.0f, 100.0f, -100.0f, 35.0f, 4, false);		mDumbAssUsedText.push_back(T);

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

	mLeaveMsg->Update(cam, dt);

	Inventory::UpdateAbilitys(cam, dt);
	if (mTitle)		  { mTitle->Update(cam, dt); }
	if (mAbilityUsed) { mAbilityUsed->Update(cam, dt);	if (mAbilityUsed->mDead){   mAbilityUsed = nullptr;  } }
	if (mDumbyUsed)   {
		mDumbyUsed->Update(cam, dt);	if (mDumbyUsed->mDead){ mDumbyUsed = nullptr; if (*StateMachine::pBattleState != BattleState::PLAYERWIN || *StateMachine::pBattleState != BattleState::PLAYERLOSE){ mMove = true; }}}

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
	if (*StateMachine::pBattleState == BattleState::PLAYERTURN || *StateMachine::pBattleState == BattleState::CPUTURN){ mLeaveMsg->DrawText2D(activeTech, context, pass, camera, ortho); }
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
	case BattleState::START:		*StateMachine::pBattleState = BattleState::PLAYERTURN;	AssignTitle(); mTitle->SetGrowIn(1.5, true); if (Inventory::NumAbilitys() < 1){ mMove = true; } Wait(2); mState = false; break;
	case BattleState::PLAYERTURN:	*StateMachine::pBattleState = BattleState::CPUTURN;		AssignTitle(); mTitle->SetGrowIn(0.3, true); 	Wait(3); CpuMove = true; mState = false; break;
	case BattleState::CPUTURN:		*StateMachine::pBattleState = BattleState::PLAYERTURN;	AssignTitle(); mTitle->SetGrowIn(1.5, true);	Wait(2); mState = false; break;
	case BattleState::PLAYERWIN:		AssignTitle(); Player::mLevel++; mTitle->SetGrowIn(1.5, true); Wait(2); mState = false; break;
	case BattleState::PLAYERLOSE:		AssignTitle(); mTitle->SetGrowIn(1.5, true); Wait(2); mState = false; break;
	}
}

void Battle::UpdateIQs()
{

}

void Battle::MakeMove()
{
	if (mCPIQ == 140.0f){	*StateMachine::pBattleState = BattleState::PLAYERWIN; }
	if (mPIQ == 0.0f){		*StateMachine::pBattleState = BattleState::PLAYERLOSE; }
	switch (*StateMachine::pBattleState)
	{
	case BattleState::START:		 mTitle = nullptr; Wait(1); mMove = false; mState = true; 	break;
	case BattleState::PLAYERTURN:	 mTitle = nullptr; mMove = false; AssignState(); if (Inventory::NumAbilitys() < 1){ mMove = true; } 	break; //TRIGGERS AFTER PLAYER CLICKED MOVE
	case BattleState::CPUTURN:		 mTitle = nullptr; Wait(1); mMove = false; mState = true;  break; //TRIGGER AFTER CPU MAKES TURN
	case BattleState::PLAYERWIN:	 mTitle = nullptr; Wait(1); mMove = false; mState = true;	break;
	case BattleState::PLAYERLOSE:	 mTitle = nullptr; Wait(1); mMove = false; mState = true;	break;
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
	if (ability == "tools"){   mAbilityUsed = mAbilityUsedText[0];   mCPIQ += 20.0f; }
	if (ability == "fishrod"){ mAbilityUsed = mAbilityUsedText[1];   mCPIQ += 20.0f; }
	if (ability == "fire"){	   mAbilityUsed = mAbilityUsedText[2];   mCPIQ += 20.0f; }

	if (mCPIQ > 140.0f){ mCPIQ = 140.0f; }

	if (mAbilityUsed)
	{
		mAbilityUsed->SetLife(false, 7.0f);
		mAbilityUsed->SetGrowIn(0.5, true);
		//MakeMove();
		mMove = true;
		Wait(5);
	}
}

void Battle::CPUUsedAbility()
{
	int rand = MathHelper::RandI(0.0f, mDumbAssUsedText.size()-1);
	mDumbyUsed = mDumbAssUsedText[rand];
	mDumbyUsed->SetLife(false, 7.0f);
	mDumbyUsed->SetGrowIn(0.5, true);
	mPIQ -= 20.0f; if (mPIQ < 0.0f) { mPIQ = 0.0f; }
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