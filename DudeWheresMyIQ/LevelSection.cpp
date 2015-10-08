#include "LevelSection.h"
ID3D11ShaderResourceView* LevelSection::mGrass;
ID3D11ShaderResourceView* LevelSection::mBush;
ID3D11ShaderResourceView* LevelSection::mEarth;
ID3D11ShaderResourceView* LevelSection::mMoon;
ID3D11ShaderResourceView* LevelSection::mBeachBall;
ID3D11ShaderResourceView* LevelSection::mCow;
ID3D11ShaderResourceView* LevelSection::mTwister;
ID3D11ShaderResourceView* LevelSection::mPortal;
ID3D11ShaderResourceView* LevelSection::mDeadChicken;
ID3D11ShaderResourceView* LevelSection::mLog;
ID3D11ShaderResourceView* LevelSection::mGlassBlock;
ID3D11ShaderResourceView* LevelSection::mAcordian;

ID3D11ShaderResourceView* LevelSection::mChick1;
ID3D11ShaderResourceView* LevelSection::mChick2;
ID3D11ShaderResourceView* LevelSection::mChick3;
ID3D11ShaderResourceView* LevelSection::mChick4;
ID3D11ShaderResourceView* LevelSection::mDude1;
ID3D11ShaderResourceView* LevelSection::mDude2;
ID3D11ShaderResourceView* LevelSection::mDude3;
ID3D11Device* LevelSection::mDevice;

LevelSection::LevelSection(float x, float y, float z, float size)
{
	float origX = x;
	float origY = y;
	float currX = x;
	float currY = y;
	float totalWidth = 0.0f;
	float textHeight = size;
	float finalWidth = 0.0f;
	float mult = 10.0f;

	MakeChunk(currX, currY, z, size*mult, size, size*mult); currX += size * mult; //Initial Landing

	int num = MathHelper::RandI(25.0f, 25.0f);
	for (int i = 0; i < num; i++)
	{
		float rand = MathHelper::RandF();
		rand > 0.3 ? MakeChunk(currX, currY, z, size*mult, size, size*mult) : MakeObstacle(currX, currY, z, size*mult, size, size*mult);
		rand > 0.8 ? currX += size * mult*1.2 : currX += size * mult;  //MOVE NEXT AND RAND SPACING 
		rand > 0.5f ? currY += size : currY -= size ;				   //UP AND DOWN
		totalWidth += size; 
	}
	EndPortal(currX, currY, z, size*mult, size, size*mult);
}
LevelSection::~LevelSection()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
	SafeVecEmpty(mEntities);
}

void LevelSection::Init(ID3D11Device** device)
{
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/earth.dds", 0, 0,				&mEarth,	0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/beachball.dds", 0, 0,			&mBeachBall,0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/moon.dds", 0, 0,				&mMoon,		0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/grass2.dds", 0, 0,			&mGrass,	0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/bush.dds", 0, 0,				&mBush,		0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/cow.dds", 0, 0,				&mCow,		0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/twister.dds", 0, 0,			&mTwister,	0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/portal.dds", 0, 0,			&mPortal,	0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/deadchicken.dds", 0, 0,		&mDeadChicken,	0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/log.dds", 0, 0,				&mLog,		0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/glassblock.dds", 0, 0,		&mGlassBlock,0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/acordian.dds", 0, 0,		&mAcordian,0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/Enemys/48chick1.dds", 0, 0,	&mChick1,	0));
 	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/Enemys/50chick2.dds", 0, 0,	&mChick2,	0));
 	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/Enemys/50chick3.dds", 0, 0,	&mChick3,	0));
 	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/Enemys/50chick4.dds", 0, 0,	&mChick4,	0));
 	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/Enemys/50dude1.dds", 0, 0,	&mDude1,	0));
 	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/Enemys/50dude2.dds", 0, 0,	&mDude2,	0));
 	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/Enemys/50dude3.dds", 0, 0,	&mDude3,	0));
 	
	mDevice = *device;
}
void LevelSection::Update(const Camera& cam, float dt)
{
	for (int i = 0; i < mEntities.size(); i++)
	{
		mEntities[i]->Update(cam, dt);
		if (mEntities[i]->mDead)
		{
			delete mEntities[i];
			mEntities[i] = nullptr;
			mEntities.erase(mEntities.begin() + i);
		}
	}
}
void LevelSection::Draw(ID3DX11EffectTechnique** activeTech, ID3D11DeviceContext* context, UINT pass, const Camera& camera, float dt, XMMATRIX& shadow)
{
	UINT stride = sizeof(Vertex::Basic32);
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	context->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

	for (int i = 0; i < mEntities.size(); i++)
	{
		mEntities[i]->SetShadTrans(shadow);
		mEntities[i]->Draw(*activeTech, context, pass, camera, dt);
	}
}
void LevelSection::DrawShad(ID3DX11EffectTechnique** activeTech, ID3D11DeviceContext* context, const Camera& camera, XMFLOAT4X4 lightView, XMFLOAT4X4 lightProj)
{
	UINT stride = sizeof(Vertex::Basic32);
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	context->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

	for (int i = 0; i < mEntities.size(); i++)
	{
		mEntities[i]->DrawShadow(*activeTech, context, camera, lightView, lightProj);
	}
}
void LevelSection::ShutDown()
{
	if (mGrass){ mGrass->Release(); mGrass = 0; }
}



//HELPERS
void LevelSection::MakeChunk(float x, float y, float z, float w, float h, float d)
{
	float rand = MathHelper::RandF();
	MakeGround(x, y, z, w, h, d);
	if (rand > 0.5){ MakeBush(x, y, z, w, h, d, false);}
	rand < 0.2 ? MakeRandomEnemy(x, y, z, w, h, d) : RandomItem(x, y, z, w, h, d, false);
}
void LevelSection::MakeGround(float x, float y, float z, float w, float h, float d)
{
	Entity* E = new Entity(6, "ground", w, h, d); 
	E->UseTexture(mGrass); 
	E->mUseAABOnce = true; E->mUseAAB = true; 
	E->SetPos(x, y, z); 
	
	mEntities.push_back(E);
}
void LevelSection::RandomItem(float x, float y, float z, float w, float h, float d, bool Abs)
{
	Entity* B = nullptr;
	float rand = MathHelper::RandF(0.0f, 1.3f);
	rand > 1.2 ? MakeTools(x, y, z, w, h, d, Abs)		:
	rand > 1.1 ? MakeChicken(x, y, z, w, h, d,Abs)		:
	rand > 1.0 ? MakeBandaid(x, y, z, w, h, d, Abs)		:
	rand > 0.9 ? MakeFire(x, y, z, w, h, d, Abs)		:
	rand > 0.8 ? MakeRubix(x, y, z, w, h, d, Abs)		:
	rand > 0.7 ? MakeFish(x, y, z, w, h, d, Abs)		:
	rand > 0.6 ? MakeFishRod(x, y, z, w, h, d, Abs)		:
	rand > 0.5 ? MakeBook(x, y, z, w, h, d, Abs)		:
	rand > 0.4 ? MakeLight(x, y, z, w, h, d, Abs)		:
	rand > 0.3 ? MakeMoonshine(x, y, z, w, h, d, Abs)	:
	rand > 0.2 ? MakePill(x, y, z, w, h, d, Abs)		:
	rand > 0.1 ? MakeApple(x, y, z, w, h, d, Abs)		:
	MakeBeer(x, y, z, w, h, d, Abs);
}
void LevelSection::MakeObstacle(float x, float y, float z, float w, float h, float d)
{
	//SquishBlockZ(x, y, z, w, h, d);

	float rand = MathHelper::RandF(0.0f,2.2f);
	rand > 2.1 ? SquishBlockZ(x, y, z, w, h, d)			:
	rand > 2.0 ? SquishBlockX(x, y, z, w, h, d)			:
	rand > 1.9 ? SquishBlockY(x, y, z, w, h, d)			:
	rand > 1.8 ? SkinnyFlipBlock(x, y, z, w, h, d)		:
	rand > 1.7 ? SkinnyRollBlock(x, y, z, w, h, d)		:
	rand > 1.6 ? SkinnyRollBlocks(x, y, z, w, h, d)		:
	rand > 1.5 ? RandomTwister(x, y, z, w, h, d)		:
	rand > 1.4 ? EarthOrbit(x, y, z, w, h, d)			:
	rand > 1.3 ? MovingBalls(x, y, z, w, h, d)			:
	rand > 1.2 ? PulseBall(x, y, z, w, h, d)			:
	rand > 1.1 ? MakeDoubleStairClimb(x, y, z, w, h, d)	:
	rand > 1.0 ? MakeSideSpindal(x, y, z, w, h, d)		:
	rand > 0.9 ? MakeSpindal(x, y, z, w, h, d)			:
	rand > 0.8 ? MakeSpinBlock2(x, y, z, w, h, d)		:
	rand > 0.7 ? MakeMovingStairs2(x, y, z, w, h, d)	:
	rand > 0.6 ? MakeMovingStairs(x, y, z, w, h, d)		:
	rand > 0.5 ? MakeSpinBlock(x, y, z, w, h, d)		:
	rand > 0.4 ? MakeUpDownBlock(x, y, z, w, h, d)		:
	rand > 0.3 ? MakeFlippingBlock(x, y, z, w, h, d)	:
	rand > 0.2 ? MakeRollingBlock(x, y, z, w, h, d)		:
	rand > 0.1 ? MakeSideToSideBlock(x, y, z, w, h, d)	:
	MakeSpinningStairs(x, y, z, w, h, d);
}


//OBSTACLES
void LevelSection::MakeRandomEnemy(float x, float y, float z, float w, float h, float d)
{
	int rand = MathHelper::RandI(-1.0f, 7.0f);
	Entity* E = nullptr;

	switch (rand)
	{
	case 0: E = new Entity(4, "enemy.mDude1", 60.0f, 80.0f, 1.0f);  E->UseTexture(mDude1);  E->SetUpAnimation(50, 1, 24, 1.0); E->SetPos(x + (MathHelper::RandF(-w / 2, w / 2)), y + 50.0f, z + (MathHelper::RandF(-d / 2, d / 2))); E->mUseAABOnce = true; E->mUseAAB = true;  mEntities.push_back(E); break;
	case 1: E = new Entity(4, "enemy.mDude2", 60.0f, 80.0f, 1.0f);  E->UseTexture(mDude2);  E->SetUpAnimation(50, 1, 24, 1.0); E->mAnim->Flip(true); E->SetPos(x + (MathHelper::RandF(-w / 2, w / 2)), y + 50.0f, z + (MathHelper::RandF(-d / 2, d / 2))); E->mUseAABOnce = true; E->mUseAAB = true;  mEntities.push_back(E); break;
	case 2: E = new Entity(4, "enemy.mDude3", 60.0f, 80.0f, 1.0f);  E->UseTexture(mDude3);  E->SetUpAnimation(50, 1, 24, 1.0); E->SetPos(x + (MathHelper::RandF(-w / 2, w / 2)), y + 50.0f, z + (MathHelper::RandF(-d / 2, d / 2))); E->mUseAABOnce = true; E->mUseAAB = true;  mEntities.push_back(E); break;
	case 3: E = new Entity(4, "enemy.mChick1", 60.0f, 80.0f, 1.0f); E->UseTexture(mChick1); E->SetUpAnimation(48, 1, 24, 1.0); E->SetPos(x + (MathHelper::RandF(-w / 2, w / 2)), y + 50.0f, z + (MathHelper::RandF(-d / 2, d / 2))); E->mUseAABOnce = true; E->mUseAAB = true;  mEntities.push_back(E); break;
	case 4: E = new Entity(4, "enemy.mChick2", 60.0f, 80.0f, 1.0f); E->UseTexture(mChick2); E->SetUpAnimation(50, 1, 24, 1.0); E->SetPos(x + (MathHelper::RandF(-w / 2, w / 2)), y + 50.0f, z + (MathHelper::RandF(-d / 2, d / 2))); E->mUseAABOnce = true; E->mUseAAB = true;  mEntities.push_back(E); break;
	case 5: E = new Entity(4, "enemy.mChick3", 60.0f, 80.0f, 1.0f); E->UseTexture(mChick3); E->SetUpAnimation(50, 1, 24, 1.0); E->SetPos(x + (MathHelper::RandF(-w / 2, w / 2)), y + 50.0f, z + (MathHelper::RandF(-d / 2, d / 2))); E->mUseAABOnce = true; E->mUseAAB = true;  mEntities.push_back(E); break;
	case 6: E = new Entity(4, "enemy.mChick4", 60.0f, 80.0f, 1.0f); E->UseTexture(mChick4); E->SetUpAnimation(50, 1, 24, 1.0); E->SetPos(x + (MathHelper::RandF(-w / 2, w / 2)), y + 50.0f, z + (MathHelper::RandF(-d / 2, d / 2))); E->mUseAABOnce = true; E->mUseAAB = true;  mEntities.push_back(E); break;
	default: break;
	}

	

}
void LevelSection::MakeSpinBlock(float x, float y, float z, float w, float h, float d)
{
	Entity* E = new Entity(6, "ground", w, h, d / 4); E->SetToSpin(1.0f, true); E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y, z);  mEntities.push_back(E);
}
void LevelSection::MakeSpinBlock2(float x, float y, float z, float w, float h, float d)
{
	Entity* E = new Entity(6, "ground", w, h, d / 4);  E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y, z);  mEntities.push_back(E);
	MathHelper::RandF() > 0.5 ? E->SetToFlip(1.0f, true) : E->SetToRoll(1.0f, true);
	MathHelper::RandF() > 0.5 ? E->SetToSpin(1.0f, true) : E->SetToSpin(-1.0f, true);
}
void LevelSection::MakeUpDownBlock(float x, float y, float z, float w, float h, float d)
{
	Entity* E = new Entity(6, "ground", w, h, d); E->SetToGoUpAndDown(150.0f, MathHelper::RandF(100.0f, 200.0f), true); E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y, z);  mEntities.push_back(E);
}
void LevelSection::MakeSpinningStairs(float x, float y, float z, float w, float h, float d)
{
	Entity* E = nullptr;
	float currY = y;
	float currRot = 0.0f;
	for (int i = 0; i < 10; i++)
	{
		E = new Entity(6, "ground", w/4, h, d*1.5); E->Yaw(currRot); E->SetToSpin(-0.3f, true); E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, currY, z);  mEntities.push_back(E);
		currY += h*1.5;
		currRot += 0.2f;
	}
	RandomItem(x, currY, z, w, h, d, true);
}
void LevelSection::MakeDoubleStairClimb(float x, float y, float z, float w, float h, float d)
{
	Entity* E = nullptr;
	float currY = y;
	float currRot = 0.0f;
	E = new Entity(6, "ground", w, h, d); E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y, z);  mEntities.push_back(E);  currY += h;//GROUND
	for (int i = 0; i < 10; i++)
	{
		E = new Entity(3, "ground", w / 8, h, d / 2); E->Yaw(currRot); E->SetToSpin(-0.3f, true); E->mBackFaceCull = false; E->UseTexture(mGlassBlock); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, currY, z - w);  mEntities.push_back(E);
		currY += h+1.0f;
		currRot += 0.2f;
	}
	RandomItem( x, currY, z-w, w, h, d,true);

	//RESET Y AND DRAW second spindal 
	currY = y + h;
	for (int i = 0; i < 10; i++)
	{
		E = new Entity(3, "ground", w / 8, h, d / 2); E->Yaw(currRot); E->SetToSpin(-0.3f, true); E->mBackFaceCull = false; E->UseTexture(mGlassBlock); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, currY, z + w);  mEntities.push_back(E);
		currY += h+1.0f;
		currRot += 0.2f;
	}
	RandomItem(x, currY, z + w, w, h, d,true);
}
void LevelSection::MakeFlippingBlock(float x, float y, float z, float w, float h, float d)
{
	Entity* E = new Entity(6, "ground", w, h, d); E->SetToFlip(0.5f, true); E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y, z);  mEntities.push_back(E);

}
void LevelSection::MakeRollingBlock(float x, float y, float z, float w, float h, float d)
{
	Entity* E = new Entity(6, "ground", w, h, d); E->SetToRoll(1.0f, true); E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y, z);  mEntities.push_back(E);
}
void LevelSection::MakeSideToSideBlock(float x, float y, float z, float w, float h, float d)
{
	Entity* E = new Entity(6, "ground", w, h, d); E->SetSideToSide(150.0f, MathHelper::RandF(100.0f, 200.0f), true); E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y, z);  mEntities.push_back(E);
}
void LevelSection::MakeMovingStairs(float x, float y, float z, float w, float h, float d)
{
	Entity* E = nullptr;
	float currY = y+h;
	float currPos = (w/8)*-6.0f;
	for (int i = 0; i < 7; i++)
	{
		E = new Entity(6, "ground", w / 8, h/1.5, d); E->SetPos(x, currY, z); E->Strafe(currPos); E->SetBackAndForth(50.0f, 130.0f, true); E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true;   mEntities.push_back(E);
		currY += h;
		currPos += w/8;
	}
}
void LevelSection::MakeMovingStairs2(float x, float y, float z, float w, float h, float d)
{
	Entity* E = nullptr;
	float currY = y + h;
	float currPos = (d / 8)*-6.0f;
	for (int i = 0; i < 7; i++)
	{
		E = new Entity(6, "ground", w, h / 1.5, d/8); E->SetPos(x, currY, z); E->Walk(currPos); E->SetSideToSide(50.0f, 130.0f, true); E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true;   mEntities.push_back(E);
		currY += h;
		currPos += d / 8;
	}
}
void LevelSection::MakeSpindal(float x, float y, float z, float w, float h, float d)
{
	Entity* E = new Entity(6, "ground", w, h, d); E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y, z);  mEntities.push_back(E);
	Entity* E2 = new Entity(6, "ground", w / 10, h*5, d/1.1); E2->SetToSpin(1.0f, true); E2->UseTexture(mGrass); E2->mUseAABOnce = true; E2->mUseAAB = true; E2->SetPos(x, y + h*2.25, z);  mEntities.push_back(E2);
	Entity* E3 = new Entity(6, "ground", w / 10, h*5, d/1.1); E3->Yaw(XM_PI / 2); E3->SetToSpin(1.0f, true); E3->UseTexture(mGrass); E3->mUseAABOnce = true; E3->mUseAAB = true; E3->SetPos(x, y + h*2.25, z);  mEntities.push_back(E3);
	RandomItem(x, y + h*5+20.0f, z, w, h, d, true);
}
void LevelSection::MakeSideSpindal(float x, float y, float z, float w, float h, float d)
{
	Entity* E = new Entity(6, "ground", w, h, d); E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y, z);  mEntities.push_back(E);
	Entity* E2 = new Entity(6, "ground", w / 4, h , d ); E2->SetToFlip(1.0f, true); E2->UseTexture(mGrass); E2->mUseAABOnce = true; E2->mUseAAB = true; E2->SetPos(x, y + h*5.5, z);  mEntities.push_back(E2);
	Entity* E3 = new Entity(6, "ground", w / 4, h , d ); E3->Pitch(XM_PI / 2); E3->SetToFlip(1.0f, true); E3->UseTexture(mGrass); E3->mUseAABOnce = true; E3->mUseAAB = true; E3->SetPos(x+2.0f, y + h*5.5, z);  mEntities.push_back(E3);
}
void LevelSection::PulseBall(float x, float y, float z, float w, float h, float d)
{
	Entity* E = new Entity(1, "ground", w/10, h, d); E->UseTexture(mBeachBall); E->SetPulse(1.0f, 2.5f, true); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y, z);  mEntities.push_back(E);
	MathHelper::RandF() > 0.5 ? E->SetToFlip(1.0f, true) : E->SetToRoll(1.0f, true);
	E->SetToSpin(1.0f, true);
}
void LevelSection::EarthOrbit(float x, float y, float z, float w, float h, float d)
{
	Entity* E = nullptr;
	E = new Entity(1, "ground", w / 5, h, d);  E->UseTexture(mEarth);  E->SetToSpin(0.2, true); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y+20.0f, z);  mEntities.push_back(E);
	E = new Entity(1, "ground", w / 12, h, d); E->UseTexture(mMoon);  E->SetToOrbit(0.5, true);  E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x-100.0f, y+20.0f, z);  mEntities.push_back(E);
	RandomItem(x, y+70.0f, z, w, h, d, true);
}
void LevelSection::MovingBalls(float x, float y, float z, float w, float h, float d)
{
	Entity* E = nullptr;
	float currX = x - w / 2;
	E = new Entity(6, "ground", w, h, d); E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y, z);  mEntities.push_back(E);//GROUND
	E = new Entity(1, "ground", w / 10, h, d); E->UseTexture(mBeachBall); E->SetSideToSide(150.0f, MathHelper::RandF(100.0f, 200.0f), true); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(currX, y + h*1.5, z); currX += h * 2; mEntities.push_back(E);
	E = new Entity(1, "ground", w / 10, h, d); E->UseTexture(mBeachBall); E->SetSideToSide(150.0f, MathHelper::RandF(100.0f, 200.0f), true); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(currX, y + h*1.5, z); currX += h * 2; mEntities.push_back(E);
	E = new Entity(1, "ground", w / 10, h, d); E->UseTexture(mBeachBall); E->SetSideToSide(150.0f, MathHelper::RandF(100.0f, 200.0f), true); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(currX, y + h*1.5, z); currX += h * 2; mEntities.push_back(E);
	E = new Entity(1, "ground", w / 10, h, d); E->UseTexture(mBeachBall); E->SetSideToSide(150.0f, MathHelper::RandF(100.0f, 200.0f), true); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(currX, y + h*1.5, z); currX += h * 2; mEntities.push_back(E);
	E = new Entity(1, "ground", w / 10, h, d); E->UseTexture(mBeachBall); E->SetSideToSide(150.0f, MathHelper::RandF(100.0f, 200.0f), true); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(currX, y + h*1.5, z); currX += h * 2; mEntities.push_back(E);
	
}
void LevelSection::RandomTwister(float x, float y, float z, float w, float h, float d)
{
	Entity* E = nullptr;
	float currY = y+h*3;
	E = new Entity(6, "ground", w, h, d); E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y, z);  mEntities.push_back(E);//GROUND
	E = new Entity(5, "", w / 10, h * 40, d); E->SetToSpin(5.0, true); E->UseTexture(mTwister);  E->SetPos(x, y + h * 20, z);  mEntities.push_back(E);//TWISTER
	E = new Entity(4, "ground", 340.0f / 4, 240.0f / 4); E->UseTexture(mCow);  E->SetToOrbit(-2, true);   E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x - 100.0f, currY, z);  mEntities.push_back(E); currY += 150.0f;
	E = new Entity(4, "ground", 40.0f, 40.0f); E->UseTexture(mDeadChicken);  E->SetToOrbit(-1.3, true);   E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x - 100.0f, y+50.0f, z);  mEntities.push_back(E); 
	E = new Entity(4, "ground", 340.0f / 5, 240.0f / 5); E->UseTexture(mCow);  E->SetToOrbit(-3, true);   E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x - 100.0f, currY, z);  mEntities.push_back(E);
}
void LevelSection::SkinnyRollBlock(float x, float y, float z, float w, float h, float d)
{
	Entity* E = nullptr;
	float currZ = MathHelper::RandF(z - w / 2, z + w / 2);
	E = new Entity(3, "ground", w, h / 2, d / 20); E->UseTexture(mLog); E->SetToFlip(MathHelper::RandF(1.0f, 3.0f), true); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y, currZ);  mEntities.push_back(E);//GROUND
}
void LevelSection::SkinnyRollBlocks(float x, float y, float z, float w, float h, float d)
{
	Entity* E = nullptr;
	float currZ = z + w/2;
	E = new Entity(3, "ground", w, h / 2, d / 20); E->UseTexture(mLog); E->SetToFlip(MathHelper::RandF(1.0f, 3.0f), true); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y, currZ); currZ -= 50.0f;  mEntities.push_back(E);//GROUND
	E = new Entity(3, "ground", w, h / 2, d / 20); E->UseTexture(mLog); E->SetToFlip(MathHelper::RandF(1.0f, 3.0f), true); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y, currZ); currZ -= 50.0f;  mEntities.push_back(E);//GROUND
	E = new Entity(3, "ground", w, h / 2, d / 20); E->UseTexture(mLog); E->SetToFlip(MathHelper::RandF(1.0f, 3.0f), true); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y, currZ); currZ -= 50.0f;  mEntities.push_back(E);//GROUND
	E = new Entity(3, "ground", w, h / 2, d / 20); E->UseTexture(mLog); E->SetToFlip(MathHelper::RandF(1.0f, 3.0f), true); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y, currZ); currZ -= 50.0f;  mEntities.push_back(E);//GROUND
	E = new Entity(3, "ground", w, h / 2, d / 20); E->UseTexture(mLog); E->SetToFlip(MathHelper::RandF(1.0f, 3.0f), true); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y, currZ); currZ -= 50.0f;  mEntities.push_back(E);//GROUND
}
void LevelSection::SkinnyFlipBlock(float x, float y, float z, float w, float h, float d)
{
	Entity* E = nullptr;
	float currZ = MathHelper::RandF(z - w / 2, z + w / 2);
	E = new Entity(3, "ground", w, h, d / 10); E->UseTexture(mLog); E->SetToRoll(MathHelper::RandF(1.0f, 3.0f), true); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y, currZ);  mEntities.push_back(E);//GROUND
}
void LevelSection::SquishBlockX(float x, float y, float z, float w, float h, float d)
{
	Entity* E = nullptr;
	E = new Entity(3, "ground", w, h, d); E->UseTexture(mAcordian); E->SetToSquish(1.0, 100.0f, true,false,false); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y, z);  mEntities.push_back(E);//GROUND
}
void LevelSection::SquishBlockZ(float x, float y, float z, float w, float h, float d)
{
	Entity* E = nullptr;
	E = new Entity(3, "ground", w, h, d * 3); E->UseTexture(mGlassBlock);  E->SetToSquish(0.5, 0.1f, false, false, true); E->mBackFaceCull = false; E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y, z);  mEntities.push_back(E);//GROUND
}
void LevelSection::SquishBlockY(float x, float y, float z, float w, float h, float d)
{
	Entity* E = nullptr;
	E = new Entity(3, "ground", w / 5, h * 20, d / 2); E->UseTexture(mGlassBlock);  E->SetToSquish(0.5, 0.1f, false, true, false); E->mBackFaceCull = false; E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x - (w / 5) * 2, y + h * 10, z + (d / 4));		  RandomItem(x - (w / 5) * 2,	y + h * 5, z + (d / 4), w, h, d, true); mEntities.push_back(E);//GROUND
	E = new Entity(3, "ground", w / 5, h * 20, d / 2); E->UseTexture(mGlassBlock);  E->SetToSquish(0.45, 0.1f, false, true, false); E->mBackFaceCull = false; E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(	x - (w / 5),		y + h * 10, z - (d / 4)); RandomItem(x - (w / 5),		y + h * 5, z - (d / 4), w, h, d, true); mEntities.push_back(E);//GROUND
	E = new Entity(3, "ground", w / 5, h * 20, d / 2); E->UseTexture(mGlassBlock);  E->SetToSquish(0.4, 0.1f, false, true, false); E->mBackFaceCull = false; E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(	x,					y + h * 10, z + (d / 4)); RandomItem(x,					y + h * 5, z + (d / 4), w, h, d, true); mEntities.push_back(E);//GROUND
	E = new Entity(3, "ground", w / 5, h * 20, d / 2); E->UseTexture(mGlassBlock);  E->SetToSquish(0.35, 0.1f, false, true, false); E->mBackFaceCull = false; E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(	x + (w / 5),		y + h * 10, z - (d / 4)); RandomItem(x + (w / 5),		y + h * 5, z - (d / 4), w, h, d, true); mEntities.push_back(E);//GROUND
	E = new Entity(3, "ground", w / 5, h * 20, d / 2); E->UseTexture(mGlassBlock);  E->SetToSquish(0.55, 0.1f, false, true, false); E->mBackFaceCull = false; E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(	x + (w / 5)*2,		y + h * 10, z + (d / 4)); RandomItem(x + (w / 5)*2,		y + h * 5, z + (d / 4), w, h, d, true); mEntities.push_back(E);//GROUND
}

void LevelSection::EndPortal(float x, float y, float z, float w, float h, float d)
{
	Entity* E = nullptr;
	E = new Entity(6, "ground", w, h, d);	 E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y, z);  mEntities.push_back(E);//GROUND
	E = new Entity(4, "portal", w, w, 1.0f); /*E->Yaw(XM_PI / 1.5);*/ E->UseTexture(mPortal); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y + w/2, z);  mEntities.push_back(E);//END PORTAL
	E->SetPulse(0.3f, 0.5f, true); E->SetToRoll(1.0, true);
}




//ITEMS
void LevelSection::MakeBeer(float x, float y, float z, float w, float h, float d, bool AbsPos)
{
	Entity* B = new Entity(4, "item.beer", 30.0f, 25.0f, 1.0f);  
	B->UseTexture(Inventory::mBeer); 
	AbsPos ? B->SetPos(x,y,z) :
	B->SetPos(x + (MathHelper::RandF(-w / 2, w / 2)), y + h, z + (MathHelper::RandF(-d / 2, d / 2))); 
	B->mUseAABOnce = true; B->mUseAAB = true;  
	mEntities.push_back(B);
}
void LevelSection::MakeMoonshine(float x, float y, float z, float w, float h, float d, bool AbsPos)
{
	Entity* B = new Entity(4, "item.moonshine", 25.0f, 30.0f, 1.0f); 
	B->UseTexture(Inventory::mMoonshine); 
	AbsPos ? B->SetPos(x, y, z) :
	B->SetPos(x + (MathHelper::RandF(-w / 2, w / 2)), y + h + 5.0f, z + (MathHelper::RandF(-d / 2, d / 2))); 
	B->mUseAABOnce = true; B->mUseAAB = true;  
	mEntities.push_back(B);
}
void LevelSection::MakePill(float x, float y, float z, float w, float h, float d, bool AbsPos)
{
	Entity* B = new Entity(4, "item.pill", 25.0f, 25.0f, 1.0f);  
	B->UseTexture(Inventory::mPill);
	AbsPos ? B->SetPos(x, y, z) :
	B->SetPos(x + (MathHelper::RandF(-w / 2, w / 2)), y + h + 5.0f, z + (MathHelper::RandF(-d / 2, d / 2))); 
	B->mUseAABOnce = true; B->mUseAAB = true;  
	mEntities.push_back(B);
}
void LevelSection::MakeApple(float x, float y, float z, float w, float h, float d, bool AbsPos)
{
	Entity* B = new Entity(4, "item.apple", 20.0f, 20.0f, 1.0f);  
	B->UseTexture(Inventory::mApple); 
	AbsPos ? B->SetPos(x, y, z) :
	B->SetPos(x + (MathHelper::RandF(-w / 2, w / 2)), y + h, z + (MathHelper::RandF(-d / 2, d / 2)));
	B->mUseAABOnce = true; B->mUseAAB = true;  
	mEntities.push_back(B);
}
void LevelSection::MakeBush(float x, float y, float z, float w, float h, float d, bool AbsPos)
{
	Entity* B = new Entity(2, "bush", 60.0f, 30.0f); 
	B->UseTexture(mBush); 
	AbsPos ? B->SetPos(x, y, z) :
	B->SetPos(x + (MathHelper::RandF(-w / 2, w / 2)), y + h + 5.0f, z + (MathHelper::RandF(-d / 2, d / 2)));   
	mEntities.push_back(B);
}
void LevelSection::MakeRubix(float x, float y, float z, float w, float h, float d, bool AbsPos)
{
	Entity* B = new Entity(3, "item.rubix", 20.0f, 20.0f, 20.0f);
	B->UseTexture(Inventory::mRubix);
	AbsPos ? B->SetPos(x, y, z) :
	B->SetPos(x + (MathHelper::RandF(-w / 2, w / 2)), y + h, z + (MathHelper::RandF(-d / 2, d / 2)));
	B->mUseAABOnce = true; B->mUseAAB = true;
	mEntities.push_back(B);
}
void LevelSection::MakeFish(float x, float y, float z, float w, float h, float d, bool AbsPos)
{
	Entity* B = new Entity(4, "item.fish", 30.0f, 30.0f, 1.0f);
	B->UseTexture(Inventory::mFish);
	AbsPos ? B->SetPos(x, y, z) :
	B->SetPos(x + (MathHelper::RandF(-w / 2, w / 2)), y + h, z + (MathHelper::RandF(-d / 2, d / 2)));
	B->mUseAABOnce = true; B->mUseAAB = true;
	mEntities.push_back(B);
}
void LevelSection::MakeFishRod(float x, float y, float z, float w, float h, float d, bool AbsPos)
{
	Entity* B = new Entity(4, "item.fishrod", 30.0f, 30.0f, 1.0f);
	B->UseTexture(Inventory::mFishRod);
	AbsPos ? B->SetPos(x, y, z) :
	B->SetPos(x + (MathHelper::RandF(-w / 2, w / 2)), y + h*1.5f, z + (MathHelper::RandF(-d / 2, d / 2)));
	B->mUseAABOnce = true; B->mUseAAB = true;
	mEntities.push_back(B);
}
void LevelSection::MakeBook(float x, float y, float z, float w, float h, float d, bool AbsPos)
{
	Entity* B = new Entity(4, "item.book", 30.0f, 30.0f, 1.0f);
	B->UseTexture(Inventory::mBook);
	AbsPos ? B->SetPos(x, y, z) :
	B->SetPos(x + (MathHelper::RandF(-w / 2, w / 2)), y + h+3.0f, z + (MathHelper::RandF(-d / 2, d / 2)));
	B->mUseAABOnce = true; B->mUseAAB = true;
	mEntities.push_back(B);
}
void LevelSection::MakeLight(float x, float y, float z, float w, float h, float d, bool AbsPos)
{
	Entity* B = new Entity(4, "item.light", 20.0f, 20.0f, 1.0f);
	B->UseTexture(Inventory::mLight);
	AbsPos ? B->SetPos(x, y, z) :
	B->SetPos(x + (MathHelper::RandF(-w / 2, w / 2)), y + h, z + (MathHelper::RandF(-d / 2, d / 2)));
	B->mUseAABOnce = true; B->mUseAAB = true;
	mEntities.push_back(B);
}
void LevelSection::MakeChicken(float x, float y, float z, float w, float h, float d, bool AbsPos)
{
	Entity* B = new Entity(4, "item.chicken", 40.0f, 40.0f, 1.0f);
	B->UseTexture(Inventory::mChicken);
	AbsPos ? B->SetPos(x, y, z) :
	B->SetPos(x + (MathHelper::RandF(-w / 2, w / 2)), y + h + 12.0f, z + (MathHelper::RandF(-d / 2, d / 2)));
	B->mUseAABOnce = true; B->mUseAAB = true;
	mEntities.push_back(B);
}
void LevelSection::MakeBandaid(float x, float y, float z, float w, float h, float d, bool AbsPos)
{
	Entity* B = new Entity(4, "item.bandaid", 30.0f, 30.0f, 1.0f);
	B->UseTexture(Inventory::mBandaid);
	AbsPos ? B->SetPos(x, y, z) :
	B->SetPos(x + (MathHelper::RandF(-w / 2, w / 2)), y + h + 3.0f, z + (MathHelper::RandF(-d / 2, d / 2)));
	B->mUseAABOnce = true; B->mUseAAB = true;
	mEntities.push_back(B);
}
void LevelSection::MakeFire(float x, float y, float z, float w, float h, float d, bool AbsPos)
{
	Entity* B = new Entity(4, "item.fire", 40.0f, 40.0f, 1.0f);
	B->UseTexture(Inventory::mFire);
	AbsPos ? B->SetPos(x, y, z) :
	B->SetPos(x + (MathHelper::RandF(-w / 2, w / 2)), y + h + 15.0f, z + (MathHelper::RandF(-d / 2, d / 2)));
	B->mUseAABOnce = true; B->mUseAAB = true;
	mEntities.push_back(B);
}
void LevelSection::MakeTools(float x, float y, float z, float w, float h, float d, bool AbsPos)
{
	Entity* B = new Entity(4, "item.tools", 40.0f, 40.0f, 1.0f);
	B->UseTexture(Inventory::mTools);
	AbsPos ? B->SetPos(x, y, z) :
	B->SetPos(x + (MathHelper::RandF(-w / 2, w / 2)), y + h + 9.0f, z + (MathHelper::RandF(-d / 2, d / 2)));
	B->mUseAABOnce = true; B->mUseAAB = true;
	mEntities.push_back(B);
}

