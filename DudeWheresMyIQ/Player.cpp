#include "Player.h"


Player::Player(ID3D11Device** device) : mDevice(*device),
GoFW(false), GoBW(false), GoIn(false), GoOut(false), SlFW(false), SlBW(false), SlIn(false), SlOut(false),
mMaxSpeed(100.0f), mVelocity(0.0f, 0.0f, 0.0f), mCurrSpeed(0.0f,0.0f,0.0f)
{
	mSelf = new Entity(2, "player", 40.0f, 40.0f);
	mSelf->LoadTexture(*device, L"Textures/invader2.dds");
	mSelf->SetPos(0.0f, 70.0f, 0.0f);
	mSelf->reverseLook = true;
}


Player::~Player()
{
}

void Player::ResetPlayerPos()
{
	mSelf->SetPos(0.0f, 70.0f, 0.0f);
}

void Player::Update(const Camera& camera, float dt)
{
	mSelf->Update(camera, dt);
}

void Player::Draw(ID3DX11EffectTechnique** activeTech, ID3D11DeviceContext* context, UINT pass, const Camera& camera, float dt)
{
	UINT stride = sizeof(Vertex::Basic32);
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	context->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	mSelf->Draw(*activeTech, context, pass, camera, dt);
}

void Player::WalkFW(float dt)
{
	mSelf->Strafe(dt*20.0f);
}

void Player::WalkBW(float dt)
{
	mSelf->Strafe(-dt*20.0f);
}

void Player::WalkTW(float dt)
{
	mSelf->Walk(dt*20.0f);
}

void Player::WalkAW(float dt)
{
	mSelf->Walk(-dt*20.0f);
}
