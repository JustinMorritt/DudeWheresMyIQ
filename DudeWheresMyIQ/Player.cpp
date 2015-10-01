#include "Player.h"
#include "Engine.h"

Entity* Player::mSelf;

int Player::mLevel;
float Player::mIQ;
float Player::mMaxSpeed;
float Player::mJumpHeight;


Player::Player(ID3D11Device** device) : mDevice(*device),
GoFW(false), GoBW(false), GoIn(false), GoOut(false), SlFW(false), SlBW(false), SlIn(false), SlOut(false), onGround(false),
 mVelocity(0.0f, 0.0f, 0.0f), mAccel(180.0f), mItemDescription(0)
{
	mSelf = new Entity(2, "player", 80.0f, 80.0f);
	mSelf->LoadTexture(*device, L"Textures/Guy/guy.dds");
	mSelf->SetUpAnimation(24, 8, 30, 0.9f);
	mSelf->reverseLook = true;
	mSelf->mUseAAB = true;
	ResetPlayerPos();

	mMaxSpeed = 100.0f;
	mLevel = 1;
	mIQ = 110.0f;
	mJumpHeight = 200.0f;

	//MAKE TEXT DESCRIPTIONS
	Text* t = new Text(" mmm Beer ../", mSelf->mPosition.x, mSelf->mPosition.y, mSelf->mPosition.z, 10.0f, 1, true); mText.push_back(t); // 0 BEER
}


Player::~Player()
{
}

void Player::ResetPlayerPos()
{
	mSelf->SetPos(0.0f, 350.0f, 0.0f);
}

void Player::ResetPlayerStats()
{
	mMaxSpeed = 100.0f;
	mLevel = 1;
	mIQ = 110.0f;
	mJumpHeight = 200.0f;
}

void Player::Update(const Camera& camera, float dt)
{
	Applyforces(dt);
	Move(dt);
	mSelf->Update(camera, dt);
	
	//triming the AABB
	mSelf->mMeshBox.Extents.z = 5.0f; // Makes the Dude a bit wider then 0 to prevent Z fighting ..
	mSelf->mMeshBox.Extents.x -= 23.0f; 
	//mSelf->mMeshBox.Extents.y -= 1.0f; 
	
	CheckCollisions();

	if (mItemDescription){ mItemDescription->Update(camera, dt); if (mItemDescription->mDead){ mItemDescription = nullptr; } }
}

void Player::Draw(ID3DX11EffectTechnique** activeTech, ID3D11DeviceContext* context, UINT pass, const Camera& camera, float dt, XMMATRIX& shadow)
{
	UINT stride = sizeof(Vertex::Basic32);
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	context->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	mSelf->SetShadTrans(shadow);
	mSelf->Draw(*activeTech, context, pass, camera, dt);
	if (mItemDescription){ mItemDescription->SetShadowTran(shadow); mItemDescription->DrawText3D(activeTech, context, pass, camera, dt); }
}

void Player::DrawShad(ID3DX11EffectTechnique** activeTech, ID3D11DeviceContext* context, const Camera& camera, XMFLOAT4X4 lightView, XMFLOAT4X4 lightProj)
{
	UINT stride = sizeof(Vertex::Basic32);
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	context->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	mSelf->DrawShadow(*activeTech, context, camera, lightView, lightProj);
	if (mItemDescription){ mItemDescription->DrawTextShad(activeTech, context, camera, lightView, lightProj); }
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
					SetItemDescription(0);
					mEntities[i][j]->mDead = true; Inventory::AddToInventory("beer");
					mEntities[i].erase(mEntities[i].begin() + j);
				}
			}
			if (mEntities[i][j]->mLabel.size() > 7)
			{
				std::string str(mEntities[i][j]->mLabel.begin(), mEntities[i][j]->mLabel.begin() + 5);
				if (str == "enemy")
				{
					if (!mEntities[i][j]->mDead)
					{
						if (XNA::IntersectAxisAlignedBoxAxisAlignedBox(&mSelf->mMeshBox, &mEntities[i][j]->mMeshBox)) //Broad phase quick check
						{
							std::string str2(mEntities[i][j]->mLabel.begin() + 6, mEntities[i][j]->mLabel.end()); // Minus one or you get an Iterator past the end of the string 
							Battle::SetBattleType(str2);
							Engine::NewBattle();
							*StateMachine::pGameState = GameState::BATTLE;
							mEntities[i].erase(mEntities[i].begin() + j);
						}
					}

				}
			}

		}
	}
}

void Player::SetItemDescription(int i)
{
	mItemDescription = mText[i]; 
	mItemDescription->SetPosition(mSelf->mPosition.x-70.0f, mSelf->mPosition.y+60.0f, mSelf->mPosition.z);
	mItemDescription->SetLife(false, 3.0f);
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
		mVelocity.y = mJumpHeight; onGround = false;
		GoBW ? mSelf->mAnim->SetAnim(5, true, true) : mSelf->mAnim->SetAnim(5, true, false); mSelf->mAnim->ResetFrame();
	}
}

Entity* Player::GetSelf()
{
	return mSelf;
}


