#include "Entity.h"

//Makes a Square by default 
Entity::Entity(int type, std::string label, float width, float height, float depth) :
mPosition(0.0f, 0.0f, 0.0f),
mShadowScale(0.0f, 0.0f, 0.0f),
mRight(1.0f, 0.0f, 0.0f),
mUp(0.0f, 1.0f, 0.0f),
mLook(0.0f, 0.0f, 1.0f),
prevPitch(0.0f),
rotationY(0.0f),
prevRoll(0.0f),
origTexScale(1.0f, 1.0f, 1.0f),
texTrans(0.0f, 0.0f, 0.0f),
texTransMult(0.0f, 0.0f, 0.0f),
mGoToPos(0.0f, 0.0f, 0.0f),
currProgress(0.0f),
rotationZ(0.0f),
mDistanceLeft(0.0f),
mTexWidth(0.0f),
mTexHeight(0.0f),
mUpDown(false),
mGrowing(false),
mSquishX(false),
mSquishY(false),
mSquishZ(false),
mOrigY(0.0f),
mOrigX(0.0f),
mOrigZ(0.0f),
mGrowOut(false),
mHeightToGo(0.0f),
mScale(1.0f),
mWidth(width),
mHeight(height),
mDepth(depth),
hovering(false),
useTexTrans(false),
progressBar(false),
goToPos(false),
billboard(false),
flipUpright(false),
reverseLook(false),
mDead(false),
mSpinning(false),
mExplode(false),
mBasicTexTrans(false),
mUseAnimation(false),
mUseAAB(false),
mUseAABOnce(false),
mGoUp(true),
mBackFaceCull(true),
mGoDown(false),
mSideToSide(false),
mPulse(false),
mOrbit(false),
turnAngle(0.0f),
explosionDist(0.0f),
mAnim(0),
movementMult(0.0f),
mFlipping(false),
mRolling(false),
mBackAndForth(false),
mGrow(true),
mShrink(false),
mGrowIn(false),
mFlipTexture(false),
mTexRotate(false),
mLabel(label)
{
	//SET MATERIAL
	mMat.Ambient	= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mMat.Diffuse	= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mMat.Specular	= XMFLOAT4(1.0f, 1.0f, 1.0f, 48.0f);

	GeometryGenerator geoGen;

	//FLOOR PLANE
	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mWorld, I); XMStoreFloat4x4(&mShadowTrans, I);

	switch (type)
	{
	case 0: geoGen.CreateGrid(width, height, 2, 2, mGrid);					break;
	case 1: geoGen.CreateSphere(width, height, height, mGrid);/*height is slice count .. width for radius*/ break;
	case 2: geoGen.CreateUprightSquare(width, height, mGrid);				break;
	case 3: geoGen.CreateBox(width, height, depth, mGrid);					break;
	case 4: geoGen.CreateFrontandBackFace(width, height, depth, mGrid);		break;
	case 5: geoGen.CreateCylinder(width, depth, height, 15, 2, mGrid);		break;
	case 6: geoGen.CreateBox2Tex(width, height, depth, mGrid);				break;
	}

	mIndexCount = mGrid.Indices.size();
	mMeshVertices.resize(mGrid.Vertices.size());
}

Entity::~Entity()
{
	//Commented so you have to intentionally release these .. some are sharing textures
// 	mTexSRV->Release();
 	mTexSRV = nullptr;
	if (mAnim){ delete mAnim; mAnim = nullptr; }
}

void Entity::SetPos(float x, float y, float z)
{
	mOrigY = y;
	mOrigX = x;
	mOrigZ = z;
	mPosition = XMFLOAT3(x, y, z);
}

void Entity::SetRot(float x, float y, float z)
{
	XMMATRIX rotX = XMMatrixRotationX(XM_PI*x);
	XMMATRIX rotY = XMMatrixRotationY(XM_PI*y);
	XMMATRIX rotZ = XMMatrixRotationZ(XM_PI*z);
	XMStoreFloat4x4(&mWorld, rotX * rotY * rotZ);
}

void Entity::Update(const Camera& camera, float dt)
{
	if (mSpinning)		{Yaw(dt*movementMult);}
	if (mUpDown)		{GoUpDown(dt);}
	if (mFlipping)		{Pitch(dt*movementMult);}
	if (mRolling)		{Roll(dt*movementMult);}
	if (mSideToSide)	{GoSideToSide(dt);}
	if (mBackAndForth)	{GoBackAndForth(dt);}
	if (mOrbit)			{ Yaw(dt*movementMult); Walk(dt*movementMult*100); }

	XMVECTOR R = XMLoadFloat3(&mRight);
	XMVECTOR U = XMLoadFloat3(&mUp);
	XMVECTOR L = XMLoadFloat3(&mLook);
	XMVECTOR P = XMLoadFloat3(&mPosition);

	
	// Keep axes orthogonal to each other and of unit length.
	L = XMVector3Normalize(L);
	U = XMVector3Normalize(XMVector3Cross(L, R));

	// U, L already ortho-normal, so no need to normalize cross product.
	R = XMVector3Cross(U, L);

	
	// Fill in the world matrix entries.
// 	float x = XMVectorGetX(XMVector3Dot(P, R));
// 	float y = XMVectorGetX(XMVector3Dot(P, U));
// 	float z = XMVectorGetX(XMVector3Dot(P, L));

	float x = XMVectorGetX(P);
	float y = XMVectorGetY(P);
	float z = XMVectorGetZ(P);

	XMStoreFloat3(&mRight, R);
	XMStoreFloat3(&mUp, U);
	XMStoreFloat3(&mLook, L);

	mWorld(0, 0) = mRight.x;
	mWorld(1, 0) = mRight.y;
	mWorld(2, 0) = mRight.z;
	mWorld(3, 0) = x;

	mWorld(0, 1) = mUp.x;
	mWorld(1, 1) = mUp.y;
	mWorld(2, 1) = mUp.z;
	mWorld(3, 1) = y;

	if (reverseLook){
	mWorld(0, 2) = -mLook.x;
	mWorld(1, 2) = -mLook.y;
	mWorld(2, 2) = -mLook.z;
	mWorld(3, 2) = z;}
	else
	{	mWorld(0, 2) = mLook.x;
		mWorld(1, 2) = mLook.y;
		mWorld(2, 2) = mLook.z;
		mWorld(3, 2) = z;}


	mWorld(0, 3) = 0.0f;
	mWorld(1, 3) = 0.0f;
	mWorld(2, 3) = 0.0f;
	mWorld(3, 3) = 1.0f;

	if (hovering)
	{
		XMMATRIX M = XMLoadFloat4x4(&mWorld);
		XMMATRIX scaling = XMMatrixScaling(1.3f, 1.3f, 1.3f);
		XMStoreFloat4x4(&mWorld, scaling * M);
	}


	//GROWING MOVEMENTS
	if (mPulse)	{ Pulse(dt); }
	if (mGrowIn){ GrowIn(dt); }
	if (mGrowOut){ GrowOut(dt); }
	if (mSquishX || mSquishY || mSquishZ){ Squish(dt); 
	if (mSquishX){  ScaleX(currProgress); }
	if (mSquishY){  ScaleY(currProgress); }
	if (mSquishZ){  ScaleZ(currProgress); }}


	if (progressBar)
	{
		ScaleX(currProgress);
	}

	if (billboard)
	{
		XMMATRIX M		= XMLoadFloat4x4(&mWorld);
		XMVECTOR L		= XMVector3Normalize(XMVectorSubtract(camera.GetPositionXM(), GetPositionXM()));
		XMVECTOR Look	= XMLoadFloat3(&mLook);
		XMVECTOR angle	= XMVector3AngleBetweenNormals(L, Look);
		float theAngle	= XMVectorGetY(angle);

		XMMATRIX rotY;
		camera.GetPosition().x < mPosition.x ? rotY = XMMatrixRotationY(-theAngle) : rotY = XMMatrixRotationY(theAngle);

		XMStoreFloat4x4(&mWorld, rotY * M); 
	}

	if (goToPos)
	{
		if (mDistanceLeft <= 0){ goToPos = false; }
	}

	if (mUseAnimation){ mAnim->Update(dt);}

	//update sphere collider
	mSphereCollider.Center	= mPosition;
	if (mUseAAB){ UpdateAAB(); }
	if (mUseAABOnce){ UpdateAAB(); mUseAABOnce = false; }
}

void Entity::Draw(ID3DX11EffectTechnique* activeTech, ID3D11DeviceContext* context, UINT pass, const Camera& camera, float dt)
{
	XMMATRIX world = XMLoadFloat4x4(&mWorld);
	XMMATRIX shad = XMLoadFloat4x4(&mShadowTrans);
	XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
	XMMATRIX worldViewProj = world*camera.View()*camera.Proj();
	Effects::BasicFX->SetWorld(world);
	Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
	Effects::BasicFX->SetWorldViewProj(worldViewProj);
	Effects::BasicFX->SetShadowTransform(world*shad);
	if (!useTexTrans){ Effects::BasicFX->SetTexTransform(XMMatrixScaling(origTexScale.x, origTexScale.y, origTexScale.z)); }
	if (useTexTrans)
	{
		Effects::BasicFX->SetTexTransform(XMMatrixTranslation(texTrans.x, texTrans.y, texTrans.z)*XMMatrixScaling(origTexScale.x, origTexScale.y, origTexScale.z));
		texTrans.x += dt*texTransMult.x;
		texTrans.y += dt*texTransMult.y;
		texTrans.z += dt*texTransMult.z;
	}
	if (mBasicTexTrans)
	{
		XMMATRIX Scale;
		mFlipTexture ? Scale = XMMatrixScaling(-origTexScale.x, origTexScale.y, origTexScale.z) : Scale = XMMatrixScaling(origTexScale.x, origTexScale.y, origTexScale.z);
		Effects::BasicFX->SetTexTransform(XMMatrixTranslation(texTrans.x, texTrans.y, texTrans.z)*Scale);
	}
	if (mUseAnimation)
	{
		XMMATRIX Scale;
		mAnim->Flipped() ? Scale = XMMatrixScaling(-origTexScale.x, origTexScale.y, origTexScale.z) : Scale = XMMatrixScaling(origTexScale.x, origTexScale.y, origTexScale.z);
		Effects::BasicFX->SetTexTransform(XMMatrixTranslation(mAnim->GetX(), mAnim->GetY(), texTrans.z)*Scale);
	}

	Effects::BasicFX->SetMaterial(mMat);
	Effects::BasicFX->SetDiffuseMap(mTexSRV);

	if (mExplode)
	{
		ID3DX11EffectTechnique* ExploadTech;
		ExploadTech = Effects::BasicFX->ExplosionTech;
		Effects::BasicFX->SetDT(explosionDist);
		ExploadTech->GetPassByIndex(pass)->Apply(0, context);
	}
	else
	{
		activeTech->GetPassByIndex(pass)->Apply(0, context);
	}

	mBackFaceCull ? context->RSSetState(0) : context->RSSetState(RenderStates::NoCullRS);

	context->DrawIndexed(mIndexCount, mIndexOffset, mVertexOffset);
}

void Entity::Draw2D(ID3DX11EffectTechnique* activeTech, ID3D11DeviceContext* context, UINT pass, const Camera& camera, XMMATRIX& ortho)
{
	XMMATRIX world = XMLoadFloat4x4(&mWorld);
	XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);

	XMMATRIX worldViewProj = world*ortho;
	Effects::BasicFX->SetWorld(world);
	Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
	Effects::BasicFX->SetWorldViewProj(worldViewProj);

	if (!useTexTrans){ Effects::BasicFX->SetTexTransform(XMMatrixScaling(origTexScale.x, origTexScale.y, origTexScale.z)); }
	if (mBasicTexTrans)
	{
		XMMATRIX Scale;
		mFlipTexture ? Scale = XMMatrixScaling(-origTexScale.x, origTexScale.y, origTexScale.z) : Scale = XMMatrixScaling(origTexScale.x, origTexScale.y, origTexScale.z);
		Effects::BasicFX->SetTexTransform(XMMatrixTranslation(texTrans.x, texTrans.y, texTrans.z)*Scale);
	}
	if (mUseAnimation)
	{
		XMMATRIX Scale;
		mAnim->Flipped() ? Scale = XMMatrixScaling(-origTexScale.x, origTexScale.y, origTexScale.z) : Scale = XMMatrixScaling(origTexScale.x, origTexScale.y, origTexScale.z);
		Effects::BasicFX->SetTexTransform(XMMatrixTranslation(mAnim->GetX(), mAnim->GetY(), texTrans.z)*Scale);
	}
	Effects::BasicFX->SetMaterial(mMat);
	Effects::BasicFX->SetDiffuseMap(mTexSRV);

	activeTech->GetPassByIndex(pass)->Apply(0, context);
	context->DrawIndexed(mIndexCount, mIndexOffset, mVertexOffset);
}

void Entity::DrawShadow(ID3DX11EffectTechnique* activeTech, ID3D11DeviceContext* context, const Camera& camera, XMFLOAT4X4 lightView, XMFLOAT4X4 lightProj)
{
	XMMATRIX view		= XMLoadFloat4x4(&lightView);
	XMMATRIX proj		= XMLoadFloat4x4(&lightProj);
	XMMATRIX viewProj	= XMMatrixMultiply(view, proj);

	ID3DX11EffectTechnique* smapTech = Effects::BuildShadowMapFX->BuildShadowMapAlphaClipTech;
	D3DX11_TECHNIQUE_DESC techDesc;
	smapTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		ID3DX11EffectPass* pass = activeTech->GetPassByIndex(p);
		XMMATRIX world = XMLoadFloat4x4(&mWorld);
		XMMATRIX worldInvTranspose = MathHelper::InverseTranspose(world);
		XMMATRIX worldViewProj = world*view*proj;
		Effects::BuildShadowMapFX->SetWorld(world);
		Effects::BuildShadowMapFX->SetWorldInvTranspose(worldInvTranspose);
		Effects::BuildShadowMapFX->SetWorldViewProj(worldViewProj);
		Effects::BuildShadowMapFX->SetDiffuseMap(mTexSRV);

		if (!useTexTrans){ Effects::BuildShadowMapFX->SetTexTransform(XMMatrixScaling(origTexScale.x, origTexScale.y, origTexScale.z)); }
		if (mBasicTexTrans)
		{
			Effects::BuildShadowMapFX->SetTexTransform(XMMatrixTranslation(texTrans.x, texTrans.y, texTrans.z)*XMMatrixScaling(origTexScale.x, origTexScale.y, origTexScale.z));
		}
		if (mUseAnimation)
		{
			XMMATRIX Scale;
			mAnim->Flipped() ? Scale = XMMatrixScaling(-origTexScale.x, origTexScale.y, origTexScale.z) : Scale = XMMatrixScaling(origTexScale.x, origTexScale.y, origTexScale.z);
			Effects::BuildShadowMapFX->SetTexTransform(XMMatrixTranslation(mAnim->GetX(), mAnim->GetY(), texTrans.z)*Scale);
		}

		pass->Apply(0, context);
		context->DrawIndexed(mIndexCount, mIndexOffset, mVertexOffset);
	}
}

void Entity::SetVertexOffset(int offSet)
{
	mVertexOffset = offSet;
}

void Entity::SetIndexOffset(int offSet)
{
	mIndexOffset = offSet;
}

void Entity::LoadVertData(std::vector<Vertex::Basic32>& verts, UINT& k)
{
	XMFLOAT3 vMinf3(+MathHelper::Infinity, +MathHelper::Infinity, +MathHelper::Infinity);
	XMFLOAT3 vMaxf3(-MathHelper::Infinity, -MathHelper::Infinity, -MathHelper::Infinity);

	XMVECTOR vMin = XMLoadFloat3(&vMinf3);
	XMVECTOR vMax = XMLoadFloat3(&vMaxf3);

	for (size_t i = 0; i < mGrid.Vertices.size(); ++i, ++k)
	{
		verts[k].Pos	= mGrid.Vertices[i].Position;
		verts[k].Normal = mGrid.Vertices[i].Normal;
		verts[k].Tex	= mGrid.Vertices[i].TexC;

		//Copy Into The Buttons Messh For Future Collision Check 
		mMeshVertices[i].Pos	= mGrid.Vertices[i].Position;
		mMeshVertices[i].Normal = mGrid.Vertices[i].Normal;

		XMVECTOR P = XMLoadFloat3(&mMeshVertices[i].Pos);

		vMin = XMVectorMin(vMin, P);
		vMax = XMVectorMax(vMax, P);
	}
	XMStoreFloat3(&mMeshBox.Center, 0.5f*(vMin + vMax));
	XMStoreFloat3(&mMeshBox.Extents, 0.5f*(vMax - vMin));

}

void Entity::LoadTexture(ID3D11Device* device , std::wstring texFilename)
{
	HR(D3DX11CreateShaderResourceViewFromFile(device, texFilename.c_str(), 0, 0, &mTexSRV, 0));
	
	//GRAB TEXTURES WIDTH AND HEIGHT .. USED LATER FOR ANIMATION PURPOSES
	ID3D11Resource*  resource  = 0;
	ID3D11Texture2D* texture2D = 0;

	mTexSRV->GetResource(&resource);
	resource->QueryInterface(&texture2D);
	D3D11_TEXTURE2D_DESC desc;

	texture2D->GetDesc(&desc);
	mTexWidth = desc.Width;
	mTexHeight = desc.Height;

	ReleaseCOM(resource);
	ReleaseCOM(texture2D);
}

void Entity::UseTexture(ID3D11ShaderResourceView* tex)
{
	mTexSRV = tex;
}

void Entity::SetSphereCollider(float radius)
{
	mSphereCollider.Center = mPosition;
	mSphereCollider.Radius = radius;
}

void Entity::ResetLookUpRight()
{
	mRight	= { 1.0f, 0.0f, 0.0f };
	mUp		= { 0.0f, 1.0f, 0.0f };
	mLook	= { 0.0f, 0.0f, 1.0f };
}

void Entity::SetUpAnimation(float cols, float rows, float FPS, float animSpeed /*= 1.0f*/, bool isLooping /*= true*/)
{
	mAnim	= new SpriteAnimation(cols, rows, FPS,animSpeed,isLooping);
	mUseAnimation		= true;
	origTexScale		= { 1.0f / cols, 1.0f / rows, 1.0f };
}

void Entity::UpdateAAB()
{
	XMFLOAT3 vMinf3(+MathHelper::Infinity, +MathHelper::Infinity, +MathHelper::Infinity);
	XMFLOAT3 vMaxf3(-MathHelper::Infinity, -MathHelper::Infinity, -MathHelper::Infinity);
	XMVECTOR vMin = XMLoadFloat3(&vMinf3);
	XMVECTOR vMax = XMLoadFloat3(&vMaxf3);

	for (size_t i = 0; i < mGrid.Vertices.size(); ++i)
	{
		XMVECTOR P = XMLoadFloat3(&mMeshVertices[i].Pos);
		P = XMVector3TransformCoord(P, XMLoadFloat4x4(&mWorld));
		vMin = XMVectorMin(vMin, P);
		vMax = XMVectorMax(vMax, P);
	}
	XMStoreFloat3(&mMeshBox.Center, 0.5f*(vMin + vMax));
	XMStoreFloat3(&mMeshBox.Extents, 0.5f*(vMax - vMin));
}

void Entity::SetShadTrans(XMMATRIX& shadow)
{
	XMStoreFloat4x4(&mShadowTrans, shadow);
}

void Entity::SetShadowScale(float x, float y, float z)
{
	mShadowScale.x = x; mShadowScale.y = y; mShadowScale.z = z;
}

int Entity::GetVertOffset()
{
	return mVertexOffset;
}

int Entity::GetIndOffset()
{
	return mIndexOffset;
}

GeometryGenerator::MeshData Entity::GetMeshData()
{
	return mGrid;
}

XMVECTOR Entity::GetRightXM()const
{
	return XMLoadFloat3(&mRight);
}

XMFLOAT3 Entity::GetRight()const
{
	return mRight;
}

XMVECTOR Entity::GetUpXM()const
{
	return XMLoadFloat3(&mUp);
}

XMFLOAT3 Entity::GetUp()const
{
	return mUp;
}

XMVECTOR Entity::GetLookXM()const
{
	return XMLoadFloat3(&mLook);
}

XMFLOAT3 Entity::GetLook()const
{
	return mLook;
}



void Entity::Strafe(float d)
{
	// mPosition += d*mRight
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR r = XMLoadFloat3(&mRight);
	XMVECTOR p = XMLoadFloat3(&mPosition);
	XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, r, p));
}

void Entity::Jump(float d)
{
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR u = XMLoadFloat3(&mUp);
	XMVECTOR p = XMLoadFloat3(&mPosition);
	XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, u, p));
}

void Entity::Walk(float d)
{
	if (goToPos){ mDistanceLeft -= d; }
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR l = XMLoadFloat3(&mLook);
	XMVECTOR p = XMLoadFloat3(&mPosition);
	XMStoreFloat3(&mPosition, XMVectorMultiplyAdd(s, l, p));
}

void Entity::Pitch(float angle)
{
	prevPitch = angle;
	// Rotate up and look vector about the right vector.

	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&mRight), angle);

	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));
}

void Entity::Yaw(float angle)
{
	// Rotate right and look vector about the up vector.
	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&mUp), angle);
	XMStoreFloat3(&mRight, XMVector3TransformNormal(XMLoadFloat3(&mRight), R));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));
}

void Entity::Roll(float angle)
{
	//Rotate Up and Right on Look Vector
	prevRoll = angle;

	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&mLook), angle);

	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mRight, XMVector3TransformNormal(XMLoadFloat3(&mRight), R));
}

void Entity::RotateY(float angle)
{
	rotationY = angle;
	// Rotate the basis vectors about the world y-axis.

	XMMATRIX R = XMMatrixRotationY(angle);

	XMStoreFloat3(&mRight, XMVector3TransformNormal(XMLoadFloat3(&mRight), R));
	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));
}

void Entity::RotateX(float angle)
{
	//rotationY = angle;
	// Rotate the basis vectors about the world y-axis.

	XMMATRIX R = XMMatrixRotationX(angle);

	XMStoreFloat3(&mRight, XMVector3TransformNormal(XMLoadFloat3(&mRight), R));
	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));
}

void Entity::RotateZ(float angle)
{
	XMVECTOR P = XMLoadFloat3(&mPosition);
	XMMATRIX R = XMMatrixRotationZ(angle);

	XMStoreFloat3(&mRight, XMVector3TransformNormal(XMLoadFloat3(&mRight), R));
	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));

	XMStoreFloat3(&mPosition, P);

}

void Entity::ScaleWhole(float scale)
{
	XMMATRIX W = XMLoadFloat4x4(&mWorld);
	XMMATRIX scaling = XMMatrixScaling(scale, scale, scale);
	XMStoreFloat4x4(&mWorld, scaling * W);
}

//USED FOR THE PROGRESS BARS
void Entity::ScaleX(float scale)
{
	//Progress gets offset on the X depending on the currProgress
	XMMATRIX trans = XMMatrixTranslation(mWorld.m[3][0]-(mWidth/2 - (mWidth/2*currProgress)), mWorld.m[3][1], mWorld.m[3][2]); // ORIGINAL TRANSLATION
	XMMATRIX scaling = XMMatrixScaling(scale, 1.0f, 1.0f);
	XMStoreFloat4x4(&mWorld, scaling * trans); //Scaled Then sent Back To Original Position;
}

void Entity::ScaleY(float scale)
{
	//Progress gets offset on the X depending on the currProgress
	XMMATRIX trans = XMMatrixTranslation(mWorld.m[3][0], mWorld.m[3][1] - (mHeight / 2 - (mHeight / 2 * currProgress)), mWorld.m[3][2]); // ORIGINAL TRANSLATION
	XMMATRIX scaling = XMMatrixScaling(1.0f, scale, 1.0f);
	XMStoreFloat4x4(&mWorld, scaling * trans); //Scaled Then sent Back To Original Position;
}

void Entity::ScaleZ(float scale)
{
	//Progress gets offset on the X depending on the currProgress
	XMMATRIX trans = XMMatrixTranslation(mWorld.m[3][0], mWorld.m[3][1], mWorld.m[3][2] - (mDepth / 2 - (mDepth / 2 * currProgress))); // ORIGINAL TRANSLATION
	XMMATRIX scaling = XMMatrixScaling(1.0f, 1.0f, scale);
	XMStoreFloat4x4(&mWorld, scaling * trans); //Scaled Then sent Back To Original Position;
}

XMVECTOR Entity::GetPositionXM()const
{
	return XMLoadFloat3(&mPosition);
}

void Entity::LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp)
{
	XMVECTOR L = XMVector3Normalize(XMVectorSubtract(target, pos));
	XMVECTOR R = XMVector3Normalize(XMVector3Cross(worldUp, L));
	XMVECTOR U = XMVector3Cross(L, R);


	XMStoreFloat3(&mPosition, pos);
	XMStoreFloat3(&mLook, L);
	XMStoreFloat3(&mRight, R);
	XMStoreFloat3(&mUp, U);
}

void Entity::LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up)
{
	XMVECTOR P = XMLoadFloat3(&pos);
	XMVECTOR T = XMLoadFloat3(&target);
	XMVECTOR U = XMLoadFloat3(&up);

	LookAt(P, T, U);
}

void Entity::SetGoToPoint(float x, float y, float z)
{
	mGoToPos.x = x; mGoToPos.y = y, mGoToPos.z = z;
	goToPos = true;

	XMVECTOR first = XMLoadFloat3(&mGoToPos);
	XMVECTOR second = XMLoadFloat3(&mPosition);
	XMVECTOR end = XMVectorSubtract(first, second);

	XMVECTOR length = XMVector3Length(end);
	mDistanceLeft = XMVectorGetX(length);
	
	XMStoreFloat3(&mLook, XMVector3Normalize(end));
	XMStoreFloat3(&mRight, XMVector3Cross(XMLoadFloat3(&mUp), XMLoadFloat3(&mLook)));
}

void Entity::SetOrbitPos(float x, float y, float z, float dt)
{
	rotationY += dt;
	mPosition.x = x; mPosition.y = y; mPosition.z = z;


}


//MOVEMENT 
void Entity::SetToSpin(float mult, bool spin)
{
	movementMult = mult;
	mSpinning = spin;
}

void Entity::SetToGoUpAndDown(float mult, float height, bool updown)
{
	mHeightToGo = height;
	movementMult = mult;
	mUpDown = updown;
}

void Entity::SetToFlip(float mult, bool flip)
{
	movementMult = mult;
	mFlipping = flip;
}

void Entity::SetToRoll(float mult, bool roll)
{
	movementMult = mult;
	mRolling     = roll;
}

void Entity::SetToOrbit(float mult, bool orbit)
{
	movementMult	= mult;
	mOrbit			= orbit;
}

void Entity::SetSideToSide(float mult, float dist, bool b)
{
	mHeightToGo  = dist;
	movementMult = mult;
	mSideToSide  = b;
}

void Entity::SetBackAndForth(float mult, float dist, bool b)
{
	mHeightToGo   = dist;
	movementMult  = mult;
	mBackAndForth = b;
}

void Entity::SetToSquish(float mult, float dist, bool x, bool y, bool z)//dist out of a 100 Normalized.
{
	mSquishX = x;
	mSquishY = y;
	mSquishZ = z;
	currProgress = 1.0f;
	mHeightToGo = dist;
	movementMult = mult;
}

void Entity::SetPulse(float mult, float dist, bool b)
{
	mHeightToGo = dist;
	movementMult = mult;
	mPulse = b;
}

void Entity::SetGrowIn(float mult, bool b)
{
	mScale = 0.0f;
	mGrowIn = b;
	movementMult = mult;
	mGrowing = true;
}

void Entity::SetGrowOut(float mult, bool b)
{
	mGrowOut = b;
	movementMult = mult;
	mGrowing = true;
}

void Entity::SetTextureRotate(float mult)
{
	mTexRotate = true;
	mSpinMult = mult;
}

void Entity::GrowIn(float dt)
{
	mScale += dt*movementMult;
	if (mScale > 1.0f){ mScale = 1.0; mGrowIn = false; mGrowing = false; }
	ScaleWhole(mScale);
}

void Entity::GrowOut(float dt)
{
	mScale -= dt*movementMult;
	if (mScale < 0.0f){ mScale = 0.0; mGrowOut = false; mGrowing = false; }
	ScaleWhole(mScale);
}

void Entity::GoUpDown(float dt)
{
	if (mGoUp)
	{
		Jump(dt*movementMult);
		if (mPosition.y > mOrigY + mHeightToGo){ mGoUp = false; mGoDown = true; }
	}
	if (mGoDown)
	{
		Jump(-dt*movementMult);
		if (mPosition.y < mOrigY - mHeightToGo){ mGoUp = true; mGoDown = false; }
	}
}

void Entity::Squish(float dt)
{
	if (mGoUp)
	{
		currProgress += dt * movementMult;
		if (currProgress > 1.0){ mGoUp = false; mGoDown = true; }
	}
	if (mGoDown)
	{
		currProgress -= dt * movementMult;
		if (currProgress < mHeightToGo){ currProgress = 0.1f; mGoUp = true; mGoDown = false; }
	}
}

void Entity::GoSideToSide(float dt)
{
	if (mGoUp)
	{
		Walk(dt*movementMult);
		if (mPosition.z > mOrigZ + mHeightToGo){ mGoUp = false; mGoDown = true; }
	}
	if (mGoDown)
	{
		Walk(-dt*movementMult);
		if (mPosition.z < mOrigZ - mHeightToGo){ mGoUp = true; mGoDown = false; }
	}
}

void Entity::GoBackAndForth(float dt)
{
	if (mGoUp)
	{
		Strafe(dt*movementMult);
		if (mPosition.x > mOrigX + mHeightToGo){ mGoUp = false; mGoDown = true; }
	}
	if (mGoDown)
	{
		Strafe(-dt*movementMult);
		if (mPosition.x < mOrigX - mHeightToGo){ mGoUp = true; mGoDown = false; }
	}
}

void Entity::Pulse(float dt)
{
	if (mGrow)
	{
		mScale += dt*movementMult;
		ScaleWhole(mScale);
		if (mScale > mHeightToGo * 2){ mGrow = false; mShrink = true; }
	}
	else if (mShrink)
	{
		mScale += -dt*movementMult;
		ScaleWhole(mScale);
		if (mScale < mHeightToGo / 2){ mGrow = true; mShrink = false; }
	}
}
