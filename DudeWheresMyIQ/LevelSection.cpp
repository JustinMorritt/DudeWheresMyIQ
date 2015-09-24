#include "LevelSection.h"

ID3D11ShaderResourceView* LevelSection::mGrass;
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

	int num = MathHelper::RandI(10.0f, 10.0f);
	for (int i = 0; i < num; i++)
	{
		float rand = MathHelper::RandF();
		char spot = 'm';
		switch (spot)
		{
		case ' ': currX += size * 3; totalWidth += size; break;
		default: 
			MakeChunk(spot, currX, currY, z, size*5, size, size*5); 
			currX += size*5; 
			rand > 0.5f ? currY += size : currY-= size ; 
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
	mDevice = *device;
}

void LevelSection::Update(const Camera& cam, float dt)
{
	for (int i = 0; i < mEntities.size(); i++)
	{
		mEntities[i]->Update(cam, dt);
	}
}

void LevelSection::MakeChunk(char letter, float x, float y, float z, float w, float h, float d)
{
	Entity* E = nullptr;

	switch (letter)
	{
	default: E = new Entity(3, "ground", w, h, d); E->UseTexture(mGrass); E->mUseAABOnce = true; E->SetPos(x, y, z); E->mUseAAB = true; mEntities.push_back(E); break;
	}
}

void LevelSection::Draw(ID3DX11EffectTechnique** activeTech, ID3D11DeviceContext* context, UINT pass, const Camera& camera, float dt)
{
	UINT stride = sizeof(Vertex::Basic32);
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	context->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

	for (int i = 0; i < mEntities.size(); i++)
	{
		mEntities[i]->Draw(*activeTech, context, pass, camera, dt);
	}
}

void LevelSection::ShutDown()
{
	if (mGrass){ mGrass->Release(); mGrass = 0; }
}


