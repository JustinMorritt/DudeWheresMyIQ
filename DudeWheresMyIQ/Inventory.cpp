#include "Inventory.h"

std::vector<Entity*> Inventory::GetItems()
{
	return mItemButtons;
}

ID3D11ShaderResourceView* Inventory::mBG;
ID3D11ShaderResourceView* Inventory::mBeer;
ID3D11ShaderResourceView* Inventory::mEmpty;
ID3D11Device* Inventory::mDevice;
std::vector<std::string> Inventory::mItemStrings;
int Inventory::mSlots;

Inventory::Inventory()
{
	
	float size = 200.0f;
	float baseX = -100.0f - (size * 2);
	float currX = baseX;
	float currY = 200.0f;


	for (int i = 0; i < mSlots; i++)
	{
		if (currX > size * 3){ currX = baseX;  currY -= size; } // 5 per row 
		MakeInv(currX, currY, size/1.2);
		currX += size;
	}
}

Inventory::~Inventory()
{
}

void Inventory::Update(const Camera& cam, float dt)
{
	for (int i = 0; i < mText.size(); i++)
	{
		mText[i]->Update(cam, dt);
	}

	int ItemSpot = 0;
	for (int i = 0; i < mItems.size(); i++)
	{
		if (mItems[i]->mLabel != "inv")
		{
			AssignItem(mItems[i], mItemStrings[ItemSpot]); ItemSpot++;
		}
	
		mItems[i]->Update(cam, dt);
	}
}

void Inventory::MakeInv(float x, float y, float size)
{
	Entity* E = new Entity(2, "inv", size, size);  E->SetPos(x, y, -90.0f); E->UseTexture(mBG);  E->reverseLook   = true; mItems.push_back(E);
	Entity* B = new Entity(2, "empty", size, size); B->SetPos(x, y, -90.0f); B->UseTexture(mEmpty); B->reverseLook = true; mItems.push_back(B); mItemButtons.push_back(B);
	mItemStrings.push_back("");
}

bool Inventory::AddToInventory(std::string item)
{
	for (int i = 0; i < mItemStrings.size(); i++)
	{
		if (mItemStrings[i] == ""){ mItemStrings[i] = item; return true;}
	}
	return false;
}

void Inventory::AssignItem(Entity* E, std::string item)
{
	if (item == "beer"){ E->UseTexture(mBeer); E->mLabel = "beer"; }
	else if (item == "water"){ }
	else{ E->UseTexture(mEmpty); }
}

void Inventory::UseItem(int E)
{
	if (mItemButtons[E]->mLabel == "beer"){/*Beer Has Been Used*/}

	
	if (mItemStrings.size() > E)
	{
		mItemButtons[E]->UseTexture(mEmpty);
		mItemStrings[E] = "";
	}
}

void Inventory::Draw(ID3DX11EffectTechnique** activeTech, ID3D11DeviceContext* context, UINT pass, const Camera& camera, XMMATRIX& ortho)
{
	UINT stride = sizeof(Vertex::Basic32);
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	context->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

	for (int i = 0; i < mItems.size(); i++)
	{
		mItems[i]->Draw2D(*activeTech, context, pass, camera, ortho);
	}
}

void Inventory::Init(ID3D11Device** device)
{
	mSlots = 18;
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/inventory.dds", 0, 0, &mBG, 0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/beer.dds", 0, 0, &mBeer, 0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Textures/empty.dds", 0, 0, &mEmpty, 0));
	mDevice = *device;
}