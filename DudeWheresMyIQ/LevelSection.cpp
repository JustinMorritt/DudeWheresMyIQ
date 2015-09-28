#include "LevelSection.h"

ID3D11ShaderResourceView* LevelSection::mGrass;
ID3D11ShaderResourceView* LevelSection::mBush;
ID3D11Device* LevelSection::mDevice;

LevelSection::LevelSection(std::string seed, float x, float y, float z, float size)
{
	float origX = x;
	float origY = y;
	float currX = x;
	float currY = y;
	float totalWidth = 0.0f;
	float textHeight = size;
	float finalWidth = 0.0f;
	float mult = 10.0f;

	int num = MathHelper::RandI(25.0f, 25.0f);
	for (int i = 0; i < num; i++)
	{
		float rand = MathHelper::RandF();
		char spot = 'm';
		switch (spot)
		{
		case ' ': currX += size * 3; totalWidth += size; break;
		default: 
			MakeChunk(spot, currX, currY, z, size*mult, size, size*mult); 
			rand > 0.8 ? currX += size * mult*1.5 : currX += size * mult;  //MOVE NEXT AND RAND SPACING 
			rand > 0.5f ? currY += size : currY -= size ;				   //UP AND DOWN
			totalWidth += size; 
			break;
		}
	}
}

LevelSection::~LevelSection()
{

}

void LevelSection::Init(ID3D11Device** device)
{
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/grass.dds", 0, 0, &mGrass, 0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/bush.dds", 0, 0, &mBush, 0));
	mDevice = *device;
}

void LevelSection::Update(const Camera& cam, float dt)
{
	for (int i = 0; i < mEntities.size(); i++)
	{
		if (mEntities[i]->mLabel == "beer"){ mEntities[i]->Yaw(dt);}
		mEntities[i]->Update(cam, dt);
		if (mEntities[i]->mDead)
		{
			delete mEntities[i];
			mEntities[i] = nullptr;
			mEntities.erase(mEntities.begin() + i);
		}
	}
}

void LevelSection::MakeChunk(char letter, float x, float y, float z, float w, float h, float d)
{
	Entity* E = nullptr;

	switch (letter)
	{
	default: E = new Entity(3, "ground", w, h, d); E->UseTexture(mGrass); E->mUseAABOnce = true; E->mUseAAB = true; E->SetPos(x, y, z);  mEntities.push_back(E);
		if (MathHelper::RandF() > 0.5){ Entity* B = new Entity(2, "bush", 60.0f, 30.0f); B->UseTexture(mBush); B->SetPos(x + (MathHelper::RandF(-w / 2, w / 2)), y + h + 5.0f, z + (MathHelper::RandF(-d / 2, d / 2)));   mEntities.push_back(B); }
		if (MathHelper::RandF() > 0.2){ Entity* B = new Entity(2, "beer", 30.0f, 25.0f); B->UseTexture(Inventory::mBeer); B->SetPos(x + (MathHelper::RandF(-w / 2, w / 2)), y + h, z + (MathHelper::RandF(-d / 2, d / 2))); B->mUseAABOnce = true; B->mUseAAB = true; B->reverseLook = true;  mEntities.push_back(B); }
		break;
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


