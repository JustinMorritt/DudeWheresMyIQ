#include "Player.h"


Player::Player(ID3D11Device** device) : mDevice(*device), mLevel(1), mIQ(110.0f),
GoFW(false), GoBW(false), GoIn(false), GoOut(false), SlFW(false), SlBW(false), SlIn(false), SlOut(false), onGround(false),
mMaxSpeed(100.0f), mVelocity(0.0f, 0.0f, 0.0f), mAccel(180.0f)
{
	mSelf = new Entity(2, "player", 80.0f, 80.0f);
	mSelf->LoadTexture(*device, L"Textures/Guy/guy.dds");
	mSelf->SetUpAnimation(24, 8, 30, 0.9f);
	mSelf->reverseLook = true;
	mSelf->mUseAAB = true;
	ResetPlayerPos();
}


Player::~Player()
{
}

void Player::ResetPlayerPos()
{
	mSelf->SetPos(0.0f, 150.0f, 0.0f);
}

void Player::Update(const Camera& camera, float dt)
{
	Applyforces(dt);
	Move(dt);
	mSelf->Update(camera, dt);
	
	//triming the AABB
	mSelf->mMeshBox.Extents.z = 5.0f; // Makes the Dude a bit wider then 0 to prevent Z fighting ..
	mSelf->mMeshBox.Extents.x -= 23.0f; 
	mSelf->mMeshBox.Extents.y += 2.0f; 

	CheckCollisions();
}

void Player::Draw(ID3DX11EffectTechnique** activeTech, ID3D11DeviceContext* context, UINT pass, const Camera& camera, float dt, XMMATRIX& shadow)
{
	UINT stride = sizeof(Vertex::Basic32);
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	context->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	mSelf->SetShadTrans(shadow);
	mSelf->Draw(*activeTech, context, pass, camera, dt);
}

void Player::DrawShad(ID3DX11EffectTechnique** activeTech, ID3D11DeviceContext* context, const Camera& camera, XMFLOAT4X4 lightView, XMFLOAT4X4 lightProj)
{
	UINT stride = sizeof(Vertex::Basic32);
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	context->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	mSelf->DrawShadow(*activeTech, context, camera, lightView, lightProj);
}

void Player::InsertCollisionItems(std::vector<Entity*> entities)
{
	mEntities.push_back(entities);
}

void Player::EmptyCollisionItems()
{
	mEntities.clear();
}

//Checking Collisionable Objects
void Player::CheckCollisions()
{
	for (int i = 0; i < mEntities.size(); i++)
	{
		for (int j = 0; j < mEntities[i].size(); j++)
		{
			if (mEntities[i][j]->mLabel == "ground")
			{
				if (XNA::IntersectAxisAlignedBoxAxisAlignedBox(&mSelf->mMeshBox, &mEntities[i][j]->mMeshBox)) //Broad phase quick check
				{
					XMFLOAT3 Correction = MathHelper::AABBCollision(mSelf->mMeshBox, mEntities[i][j]->mMeshBox);
					if(		 Correction.x != 0.0f){ mSelf->Strafe(Correction.x);}
					else if (Correction.z != 0.0f){ mSelf->Walk(  Correction.z);}
					else if (Correction.y != 0.0f)
					{
						mSelf->Jump(Correction.y);
					
						if (!onGround)
						{
							mSelf->mAnim->OverRidePlayOnce(false); (GoBW) ? mSelf->mAnim->SetAnim(6, true, true) : mSelf->mAnim->SetAnim(6, true, false); 
							mSelf->mAnim->ResetFrame();
						}//LANDING ANIMATION
					
						mVelocity.y = 0.0f; onGround = true; 
					}//Means it Hit Ground Reset Y Velocity So it Doesnt Go To Much And Jitter
				}
			}
			if (mEntities[i][j]->mLabel == "beer")
			{
				if (XNA::IntersectAxisAlignedBoxAxisAlignedBox(&mSelf->mMeshBox, &mEntities[i][j]->mMeshBox)) //Broad phase quick check
				{
					mEntities[i][j]->mDead = true; Inventory::AddToInventory("beer");
					mEntities[i].erase(mEntities[i].begin() + j);
				}
			}

		}
	}
}

void Player::Applyforces(float dt)
{
	//ACCELERATION
	if (GoIn){  if (mVelocity.z != -mMaxSpeed){ mVelocity.z -= mAccel*dt;  if (mVelocity.z < -mMaxSpeed){ mVelocity.z = -mMaxSpeed; }}}
	if (GoOut){ if (mVelocity.z != mMaxSpeed){  mVelocity.z += mAccel*dt;  if (mVelocity.z > mMaxSpeed){  mVelocity.z = mMaxSpeed;  }}}
	if (GoBW){  if (mVelocity.x != -mMaxSpeed){ mVelocity.x -= mAccel*dt;  if (mVelocity.x < -mMaxSpeed){ mVelocity.x = -mMaxSpeed; }}}
	if (GoFW){  if (mVelocity.x != mMaxSpeed){  mVelocity.x += mAccel*dt;  if (mVelocity.x > mMaxSpeed){  mVelocity.x = mMaxSpeed;  }}}

	//DECELERATION
	if (SlIn) { mVelocity.z += mAccel*dt; if (mVelocity.z > 0){ SlIn  = false; mVelocity.z = 0; }}
	if (SlOut){ mVelocity.z -= mAccel*dt; if (mVelocity.z < 0){ SlOut = false; mVelocity.z = 0; }}
	if (SlBW){  mVelocity.x += mAccel*dt; if (mVelocity.x > 0){ SlBW  = false; mVelocity.x = 0; }}
	if (SlFW){  mVelocity.x -= mAccel*dt; if (mVelocity.x < 0){ SlFW  = false; mVelocity.x = 0; }}

	//GRAVITY
	
	mVelocity.y -= mAccel*dt*2; if (mVelocity.y > mMaxSpeed * 3){ mVelocity.y = -mMaxSpeed * 3; }
	SetAnimation();
}

void Player::SetAnimation()
{
	if (GoIn){ mSelf->mAnim->SetAnim(4, false, false); }
	if (GoOut){mSelf->mAnim->SetAnim(3, false, false); }
	if (GoBW){ mSelf->mAnim->SetAnim(2, false, true); }
	if (GoFW){ mSelf->mAnim->SetAnim(2, false, false); }

	if (GoIn && GoFW){ mSelf->mAnim->SetAnim(1, false, false);}
	if (GoIn && GoBW){ mSelf->mAnim->SetAnim(1, false, true);}
	if (GoOut && GoFW){ mSelf->mAnim->SetAnim(7, false, false);}
	if (GoOut && GoBW){ mSelf->mAnim->SetAnim(7, false, true);}

	if (!GoIn && !GoOut && !GoBW && !GoFW){ mSelf->mAnim->Flipped() ? mSelf->mAnim->SetAnim(0, false, true) : mSelf->mAnim->SetAnim(0, false, false); }
}

void Player::Move(float dt)
{
	//TODO:: EVENTUALLY JUMP IN HERE TOO
	mSelf->Strafe(dt*mVelocity.x);		
	mSelf->Walk(	 dt*mVelocity.z);
	mSelf->Jump(	 dt*mVelocity.y);
}

void Player::Jump()
{
	if (onGround)
	{
		mVelocity.y = 200.0f; onGround = false;
		GoBW ? mSelf->mAnim->SetAnim(5, true, true) : mSelf->mAnim->SetAnim(5, true, false); mSelf->mAnim->ResetFrame();
	}
}
