#include "Text.h"
#include "Engine.h"
 

ID3D11ShaderResourceView* Text::mLetters;
ID3D11ShaderResourceView* Text::mBG1;
ID3D11ShaderResourceView* Text::mBG2;
ID3D11ShaderResourceView* Text::mBG3;
ID3D11ShaderResourceView* Text::mBG4;
ID3D11ShaderResourceView* Text::mBG5;
ID3D11ShaderResourceView* Text::mPost;

ID3D11Device* Text::mDevice;

Text::Text(std::string text, float x, float y, float z, float size, int BG, bool ThreeD) : mIB(0), mVB(0), numLetters(39.0f), mOrigPos(x, y, z), mDead(false)
{
	Build(text, x, y, z, size, BG, ThreeD);
}


void Text::Build(std::string text, float x, float y, float z, float size, int BG, bool ThreeD)
{
	float origX = x;
	float origY = y;
	float currX = x;
	float currY = y;
	float textWidth = 0.0f;
	float textHeight = size;
	float finalWidth = 0.0f;
	float letterSpacing = 1.6f;

	for (int i = 0; i < text.size(); i++)
	{
		char spot = text[i];
		switch (spot)
		{
		case ' ': currX += size / 2; textWidth += size / 2; break;
		case '/': currY -= size;   textHeight += size; currX = origX; textWidth += size / 2;
			if (textWidth > finalWidth){ finalWidth = textWidth; } textWidth = 0.0f; break;
		default: MakeLetter(spot, currX, currY, z - 1.0f, size); currX += size / letterSpacing; textWidth += size / letterSpacing; break;
		}
	}
	if (textWidth > finalWidth){ finalWidth = textWidth; }

	if (BG == 2){ Entity* E = new Entity(3, "text", 10.0f, textHeight * 3 + size, 10.0f); E->UseTexture(mPost); E->SetPos(x + ((finalWidth / 2) - (size / 2)), y - textHeight, z + 10.0f); mText.push_back(E); }

	//SET UP TEXT BACKGROUND
	Entity* E = new Entity(4, "text", finalWidth + size, textHeight + size);
	E->SetPos(x + ((finalWidth / 2) - (size / 2)), y - ((textHeight / 2) - (size / 2)), z);
	ThreeD ? mText.push_back(E) : mText.insert(mText.begin(), E);
	switch (BG)
	{
	case 0:	E->UseTexture(mBG1); break;
	case 1: E->UseTexture(mBG2); break;
	case 2: E->UseTexture(mBG3);  break;
	case 3: E->UseTexture(mBG4);  break;
	case 4: E->UseTexture(mBG5);  break;
	}
}

void Text::Rebuild(std::string text, float x, float y, float z, float size, int BG, bool ThreeD)
{
	SafeVecEmpty(mText);
	Build(text, x, y, z, size, BG, ThreeD);
	Engine::BuildVertexAndIndexBuffer(&mIB, &mVB, mText);
}

Text::~Text()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
	SafeVecEmpty(mText);
}

void Text::Init(ID3D11Device** device)
{
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Fonts/FontRaster.dds", 0, 0, &mLetters, 0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Fonts/BG1.dds", 0, 0, &mBG1, 0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Fonts/BG2.dds", 0, 0, &mBG2, 0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Fonts/BG3.dds", 0, 0, &mBG3, 0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Fonts/leftspeech.dds",  0, 0, &mBG4, 0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Fonts/rightspeech.dds", 0, 0, &mBG5, 0));
	HR(D3DX11CreateShaderResourceViewFromFile(*device, L"Fonts/post.dds", 0, 0, &mPost, 0));
	mDevice = *device;
}																			 

void Text::Update(const Camera& cam, float dt)
{
	if (!mDead){ mLife -= dt; if (mLife < 0.0f){ mDead = true; } }
	for (int i = 0; i < mText.size(); i++)
	{
		mText[i]->Update(cam, dt);
	}
}

void Text::MakeLetter(char letter, float x, float y, float z, float size)
{
	Entity* E = nullptr;

	XMFLOAT3 base = { 0.0f, 0.0f, 0.0f };
	
	switch (letter)
	{
	case 'A': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base;  E->SetPos(x, y, z); mText.push_back(E); break;
	case 'B': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 1.0f;  E->SetPos(x, y, z); mText.push_back(E); break;
	case 'C': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 2.0f;  E->SetPos(x, y, z); mText.push_back(E); break;
	case 'D': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 3.0f;  E->SetPos(x, y, z); mText.push_back(E); break;
	case 'E': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 4.0f;  E->SetPos(x, y, z); mText.push_back(E); break;
	case 'F': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 5.0f;  E->SetPos(x, y, z); mText.push_back(E); break;
	case 'G': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 6.0f;  E->SetPos(x, y, z); mText.push_back(E); break;
	case 'H': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 7.0f;  E->SetPos(x, y, z); mText.push_back(E); break;
	case 'I': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 8.0f;  E->SetPos(x, y, z); mText.push_back(E); break;
	case 'J': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 9.0f;  E->SetPos(x, y, z); mText.push_back(E); break;
	case 'K': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 10.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case 'L': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 11.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case 'M': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 12.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case 'N': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 13.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case 'O': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 14.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case 'P': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 15.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case 'Q': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 16.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case 'R': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 17.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case 'S': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 18.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case 'T': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 19.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case 'U': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 20.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case 'V': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 21.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case 'W': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 22.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case 'X': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 23.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case 'Y': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 24.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case 'Z': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 25.0f; E->SetPos(x, y, z); mText.push_back(E); break;
		
	case 'a': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 0.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case 'b': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 1.0f;  E->SetPos(x, y, z); mText.push_back(E); break;
	case 'c': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 2.0f;  E->SetPos(x, y, z); mText.push_back(E); break;
	case 'd': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 3.0f;  E->SetPos(x, y, z); mText.push_back(E); break;
	case 'e': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 4.0f;  E->SetPos(x, y, z); mText.push_back(E); break;
	case 'f': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 5.0f;  E->SetPos(x, y, z); mText.push_back(E); break;
	case 'g': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 6.0f;  E->SetPos(x, y, z); mText.push_back(E); break;
	case 'h': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 7.0f;  E->SetPos(x, y, z); mText.push_back(E); break;
	case 'i': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 8.0f;  E->SetPos(x, y, z); mText.push_back(E); break;
	case 'j': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 9.0f;  E->SetPos(x, y, z); mText.push_back(E); break;
	case 'k': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 10.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case 'l': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 11.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case 'm': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 12.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case 'n': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 13.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case 'o': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 14.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case 'p': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 15.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case 'q': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 16.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case 'r': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 17.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case 's': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 18.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case 't': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 19.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case 'u': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 20.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case 'v': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 21.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case 'w': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 22.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case 'x': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 23.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case 'y': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 24.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case 'z': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 25.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	

	case '0': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 26.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case '1': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 27.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case '2': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 28.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case '3': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 29.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case '4': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 30.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case '5': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 31.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case '6': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 32.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case '7': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 33.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case '8': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 34.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case '9': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 35.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case '!': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 36.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case '?': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 37.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	case '.': E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 38.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	default : E = new Entity(2, "text", size, size); E->UseTexture(mLetters); E->texTrans = base; E->texTrans.x = 39.0f; E->SetPos(x, y, z); mText.push_back(E); break;
	}
	E->mBasicTexTrans = true;
	E->origTexScale = { 1.0f /numLetters, 1.0f, 1.0f };
}


void Text::DrawText2D(ID3DX11EffectTechnique** activeTech, ID3D11DeviceContext* context, UINT pass, const Camera& camera, XMMATRIX& ortho)
{
	UINT stride = sizeof(Vertex::Basic32);
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	context->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

	for (int i = 0; i < mText.size(); i++)
	{
		mText[i]->Draw2D(*activeTech, context, pass, camera, ortho);
	}
}

void Text::DrawText3D(ID3DX11EffectTechnique** activeTech, ID3D11DeviceContext* context, UINT pass, const Camera& camera, float dt)
{
	UINT stride = sizeof(Vertex::Basic32);
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	context->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

	for (int i = 0; i < mText.size(); i++)
	{
		mText[i]->Draw(*activeTech, context, pass, camera, dt);
	}
}

void Text::DrawTextShad(ID3DX11EffectTechnique** activeTech, ID3D11DeviceContext* context, const Camera& camera, XMFLOAT4X4 lightView, XMFLOAT4X4 lightProj)
{
	UINT stride = sizeof(Vertex::Basic32);
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	context->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

	mText[mText.size()-1]->DrawShadow(*activeTech, context, camera, lightView, lightProj);
}

void Text::SetShadowTran(XMMATRIX& shad)
{
	mText[mText.size() - 1]->SetShadTrans(shad);
}

void Text::SetGrowIn(float mult, bool on)
{
	for (int i = 0; i < mText.size(); i++)
	{
		mText[i]->SetGrowIn(mult, on);
	}
}

void Text::SetGrowOut(float mult, bool on)
{
	for (int i = 0; i < mText.size(); i++)
	{
		mText[i]->SetGrowOut(mult, on);
	}
}

void Text::SetPosition(float x, float y, float z)
{
	//Need To Get Difference From Original Posision And Go Through Each Entity += the difference
	XMFLOAT3 to = { x, y, z };
	XMFLOAT3 from = mOrigPos;
	XMFLOAT3 diff = { to.x - from.x, to.y - from.y, to.z - from.z };

	
	for (int i = 0; i < mText.size(); i++)
	{
		mText[i]->SetPos(mText[i]->mPosition.x += diff.x, mText[i]->mPosition.y += diff.y, mText[i]->mPosition.z += diff.z);
	}
	mOrigPos = to;
}

void Text::ShutDown()
{ 

	if (mLetters){ mLetters->Release(); mLetters = 0; }
}

void Text::SetLife(bool b, float life)
{
	mLife = life;
	mDead = b;
}

