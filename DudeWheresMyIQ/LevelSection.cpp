#include "LevelSection.h"
ID3D11ShaderResourceView* LevelSection::mGrass;
ID3D11ShaderResourceView* LevelSection::mBush;
ID3D11ShaderResourceView* LevelSection::mEarth;
ID3D11ShaderResourceView* LevelSection::mMoon;
ID3D11ShaderResourceView* LevelSection::mBeachBall;

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
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/grass.dds", 0, 0,				&mGrass,	0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/bush.dds", 0, 0,				&mBush,		0));
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

void LevelSection::MakeChunk(float x, float y, float z, float w, float h, float d)
{
	float rand = MathHelper::RandF();
	Entity* E = new Entity(3, "ground", w, h, d); E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y, z);  mEntities.push_back(E);
	if (rand > 0.5){ Entity* B = new Entity(2, "bush", 60.0f, 30.0f); B->UseTexture(mBush); B->SetPos(x + (MathHelper::RandF(-w / 2, w / 2)), y + h + 5.0f, z + (MathHelper::RandF(-d / 2, d / 2)));   mEntities.push_back(B); }
	if (rand > 0.2){ Entity* B = new Entity(4, "item.beer", 30.0f, 25.0f, 1.0f);  B->UseTexture(Inventory::mBeer); B->SetPos(x + (MathHelper::RandF(-w / 2, w / 2)), y + h, z + (MathHelper::RandF(-d / 2, d / 2))); B->mUseAABOnce = true; B->mUseAAB = true;  mEntities.push_back(B); }
	if (rand > 0.2){ Entity* B = new Entity(4, "item.pill", 25.0f, 25.0f, 1.0f);  B->UseTexture(Inventory::mPill); B->SetPos(x + (MathHelper::RandF(-w / 2, w / 2)), y + h+5.0f, z + (MathHelper::RandF(-d / 2, d / 2))); B->mUseAABOnce = true; B->mUseAAB = true;  mEntities.push_back(B); }
	if (rand > 0.2){ Entity* B = new Entity(4, "item.moonshine", 25.0f, 30.0f, 1.0f);  B->UseTexture(Inventory::mMoonshine); B->SetPos(x + (MathHelper::RandF(-w / 2, w / 2)), y + h+5.0f, z + (MathHelper::RandF(-d / 2, d / 2))); B->mUseAABOnce = true; B->mUseAAB = true;  mEntities.push_back(B); }
	if (rand > 0.2){ Entity* B = new Entity(4, "item.apple", 20.0f, 20.0f, 1.0f);  B->UseTexture(Inventory::mApple); B->SetPos(x + (MathHelper::RandF(-w / 2, w / 2)), y + h, z + (MathHelper::RandF(-d / 2, d / 2))); B->mUseAABOnce = true; B->mUseAAB = true;  mEntities.push_back(B); }
	if (rand < 0.2){ MakeRandomEnemy(x, y, z, w, h, d); }
}
void LevelSection::MakeObstacle(float x, float y, float z, float w, float h, float d)
{


	float rand = MathHelper::RandF(0.0f,1.5f);
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

void LevelSection::MakeRandomEnemy(float x, float y, float z, float w, float h, float d)
{
	int rand = MathHelper::RandI(0.0f, 6.0f);
	Entity* E = nullptr;

	switch (rand)
	{
	case 0: E = new Entity(4, "enemy.mDude1", 80.0f, 80.0f, 1.0f);  E->UseTexture(mDude1);  E->SetUpAnimation(50, 1, 24, 1.0); E->SetPos(x + (MathHelper::RandF(-w / 2, w / 2)), y + 50.0f, z + (MathHelper::RandF(-d / 2, d / 2))); E->mUseAABOnce = true; E->mUseAAB = true;  mEntities.push_back(E); break;
	case 1: E = new Entity(4, "enemy.mDude2", 80.0f, 80.0f, 1.0f);  E->UseTexture(mDude2);  E->SetUpAnimation(50, 1, 24, 1.0); E->mAnim->Flip(true); E->SetPos(x + (MathHelper::RandF(-w / 2, w / 2)), y + 50.0f, z + (MathHelper::RandF(-d / 2, d / 2))); E->mUseAABOnce = true; E->mUseAAB = true;  mEntities.push_back(E); break;
	case 2: E = new Entity(4, "enemy.mDude3", 80.0f, 80.0f, 1.0f);  E->UseTexture(mDude3);  E->SetUpAnimation(50, 1, 24, 1.0); E->SetPos(x + (MathHelper::RandF(-w / 2, w / 2)), y + 50.0f, z + (MathHelper::RandF(-d / 2, d / 2))); E->mUseAABOnce = true; E->mUseAAB = true;  mEntities.push_back(E); break;
	case 3: E = new Entity(4, "enemy.mChick1", 80.0f, 80.0f, 1.0f); E->UseTexture(mChick1); E->SetUpAnimation(48, 1, 24, 1.0); E->SetPos(x + (MathHelper::RandF(-w / 2, w / 2)), y + 50.0f, z + (MathHelper::RandF(-d / 2, d / 2))); E->mUseAABOnce = true; E->mUseAAB = true;  mEntities.push_back(E); break;
	case 4: E = new Entity(4, "enemy.mChick2", 80.0f, 80.0f, 1.0f); E->UseTexture(mChick2); E->SetUpAnimation(50, 1, 24, 1.0); E->SetPos(x + (MathHelper::RandF(-w / 2, w / 2)), y + 50.0f, z + (MathHelper::RandF(-d / 2, d / 2))); E->mUseAABOnce = true; E->mUseAAB = true;  mEntities.push_back(E); break;
	case 5: E = new Entity(4, "enemy.mChick3", 80.0f, 80.0f, 1.0f); E->UseTexture(mChick3); E->SetUpAnimation(50, 1, 24, 1.0); E->SetPos(x + (MathHelper::RandF(-w / 2, w / 2)), y + 50.0f, z + (MathHelper::RandF(-d / 2, d / 2))); E->mUseAABOnce = true; E->mUseAAB = true;  mEntities.push_back(E); break;
	case 6: E = new Entity(4, "enemy.mChick4", 80.0f, 80.0f, 1.0f); E->UseTexture(mChick4); E->SetUpAnimation(50, 1, 24, 1.0); E->SetPos(x + (MathHelper::RandF(-w / 2, w / 2)), y + 50.0f, z + (MathHelper::RandF(-d / 2, d / 2))); E->mUseAABOnce = true; E->mUseAAB = true;  mEntities.push_back(E); break;
	default: break;
	}

	

}
void LevelSection::MakeSpinBlock(float x, float y, float z, float w, float h, float d)
{
	Entity* E = new Entity(3, "ground", w, h, d / 4); E->SetToSpin(1.0f, true); E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y, z);  mEntities.push_back(E);
}
void LevelSection::MakeSpinBlock2(float x, float y, float z, float w, float h, float d)
{
	Entity* E = new Entity(3, "ground", w, h, d / 4);  E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y, z);  mEntities.push_back(E);
	MathHelper::RandF() > 0.5 ? E->SetToFlip(1.0f, true) : E->SetToRoll(1.0f, true);
	MathHelper::RandF() > 0.5 ? E->SetToSpin(1.0f, true) : E->SetToSpin(-1.0f, true);
}
void LevelSection::MakeUpDownBlock(float x, float y, float z, float w, float h, float d)
{
	Entity* E = new Entity(3, "ground", w, h, d); E->SetToGoUpAndDown(150.0f, MathHelper::RandF(100.0f, 200.0f), true); E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y, z);  mEntities.push_back(E);
}
void LevelSection::MakeSpinningStairs(float x, float y, float z, float w, float h, float d)
{
	Entity* E = nullptr;
	float currY = y;
	float currRot = 0.0f;
	for (int i = 0; i < 10; i++)
	{
		E = new Entity(3, "ground", w/4, h, d*1.5); E->Yaw(currRot); E->SetToSpin(-0.3f, true); E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, currY, z);  mEntities.push_back(E);
		currY += h*1.5;
		currRot += 0.2f;
	}
}
void LevelSection::MakeDoubleStairClimb(float x, float y, float z, float w, float h, float d)
{
	Entity* E = nullptr;
	float currY = y;
	float currRot = 0.0f;
	E = new Entity(3, "ground", w, h, d); E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y, z);  mEntities.push_back(E);  currY += h;//GROUND
	for (int i = 0; i < 10; i++)
	{
		E = new Entity(3, "ground", w / 8, h, d/2); E->Yaw(currRot); E->SetToSpin(-0.3f, true); E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, currY, z-w);  mEntities.push_back(E);
		currY += h;
		currRot += 0.2f;
	}
	E = new Entity(4, "beer", 30.0f, 25.0f, 1.0f); E->SetToSpin(MathHelper::RandF(1.0f, 10.0f), true); E->UseTexture(Inventory::mBeer); E->SetPos(x, currY, z - w ); E->mUseAABOnce = true; E->mUseAAB = true;  mEntities.push_back(E);

	//RESET Y AND DRAW second spindal 
	currY = y + h;
	for (int i = 0; i < 10; i++)
	{
		E = new Entity(3, "ground", w / 8, h, d / 2); E->Yaw(currRot); E->SetToSpin(-0.3f, true); E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, currY, z + w);  mEntities.push_back(E);
		currY += h;
		currRot += 0.2f;
	}
	E = new Entity(4, "beer", 30.0f, 25.0f, 1.0f); E->SetToSpin(MathHelper::RandF(1.0f, 10.0f), true); E->UseTexture(Inventory::mBeer); E->SetPos(x, currY, z + w); E->mUseAABOnce = true; E->mUseAAB = true;  mEntities.push_back(E);
}
void LevelSection::MakeFlippingBlock(float x, float y, float z, float w, float h, float d)
{
	Entity* E = new Entity(3, "ground", w, h, d); E->SetToFlip(0.5f, true); E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y, z);  mEntities.push_back(E);

}
void LevelSection::MakeRollingBlock(float x, float y, float z, float w, float h, float d)
{
	Entity* E = new Entity(3, "ground", w, h, d); E->SetToRoll(1.0f, true); E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y, z);  mEntities.push_back(E);
}
void LevelSection::MakeSideToSideBlock(float x, float y, float z, float w, float h, float d)
{
	Entity* E = new Entity(3, "ground", w, h, d); E->SetSideToSide(150.0f, MathHelper::RandF(100.0f, 200.0f), true); E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y, z);  mEntities.push_back(E);
}
void LevelSection::MakeMovingStairs(float x, float y, float z, float w, float h, float d)
{
	Entity* E = nullptr;
	float currY = y+h;
	float currPos = (w/8)*-6.0f;
	for (int i = 0; i < 7; i++)
	{
		E = new Entity(3, "ground", w / 8, h/1.5, d); E->SetPos(x, currY, z); E->Strafe(currPos); E->SetBackAndForth(50.0f, 130.0f, true); E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true;   mEntities.push_back(E);
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
		E = new Entity(3, "ground", w, h / 1.5, d/8); E->SetPos(x, currY, z); E->Walk(currPos); E->SetSideToSide(50.0f, 130.0f, true); E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true;   mEntities.push_back(E);
		currY += h;
		currPos += d / 8;
	}
}
void LevelSection::MakeSpindal(float x, float y, float z, float w, float h, float d)
{
	Entity* E = new Entity(3, "ground", w, h, d); E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y, z);  mEntities.push_back(E);
	Entity* E2 = new Entity(3, "ground", w / 10, h*10, d/1.1); E2->SetToSpin(1.0f, true); E2->UseTexture(mGrass); E2->mUseAABOnce = true; E2->mUseAAB = true; E2->SetPos(x, y + h*5.5, z);  mEntities.push_back(E2);
	Entity* E3 = new Entity(3, "ground", w / 10, h*10, d/1.1); E3->Yaw(XM_PI / 2); E3->SetToSpin(1.0f, true); E3->UseTexture(mGrass); E3->mUseAABOnce = true; E3->mUseAAB = true; E3->SetPos(x, y + h*5.5, z);  mEntities.push_back(E3);
}
void LevelSection::MakeSideSpindal(float x, float y, float z, float w, float h, float d)
{
	Entity* E = new Entity(3, "ground", w, h, d); E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y, z);  mEntities.push_back(E);
	Entity* E2 = new Entity(3, "ground", w / 4, h , d ); E2->SetToFlip(1.0f, true); E2->UseTexture(mGrass); E2->mUseAABOnce = true; E2->mUseAAB = true; E2->SetPos(x, y + h*5.5, z);  mEntities.push_back(E2);
	Entity* E3 = new Entity(3, "ground", w / 4, h , d ); E3->Pitch(XM_PI / 2); E3->SetToFlip(1.0f, true); E3->UseTexture(mGrass); E3->mUseAABOnce = true; E3->mUseAAB = true; E3->SetPos(x+2.0f, y + h*5.5, z);  mEntities.push_back(E3);
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
}
void LevelSection::MovingBalls(float x, float y, float z, float w, float h, float d)
{
	Entity* E = nullptr;
	float currX = x - w / 2;
	float currRot = 0.0f;
	E = new Entity(3, "ground", w, h, d); E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y, z);  mEntities.push_back(E);//GROUND
	E = new Entity(1, "ground", w / 10, h, d); E->UseTexture(mBeachBall); E->SetSideToSide(150.0f, MathHelper::RandF(100.0f, 200.0f), true); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(currX, y + h*1.5, z); currX += h * 2; mEntities.push_back(E);
	E = new Entity(1, "ground", w / 10, h, d); E->UseTexture(mBeachBall); E->SetSideToSide(150.0f, MathHelper::RandF(100.0f, 200.0f), true); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(currX, y + h*1.5, z); currX += h * 2; mEntities.push_back(E);
	E = new Entity(1, "ground", w / 10, h, d); E->UseTexture(mBeachBall); E->SetSideToSide(150.0f, MathHelper::RandF(100.0f, 200.0f), true); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(currX, y + h*1.5, z); currX += h * 2; mEntities.push_back(E);
	E = new Entity(1, "ground", w / 10, h, d); E->UseTexture(mBeachBall); E->SetSideToSide(150.0f, MathHelper::RandF(100.0f, 200.0f), true); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(currX, y + h*1.5, z); currX += h * 2; mEntities.push_back(E);
	E = new Entity(1, "ground", w / 10, h, d); E->UseTexture(mBeachBall); E->SetSideToSide(150.0f, MathHelper::RandF(100.0f, 200.0f), true); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(currX, y + h*1.5, z); currX += h * 2; mEntities.push_back(E);
	
}

