#ifndef Entity_h__
#define Entity_h__


#include "Sky.h"
#include "GeometryGenerator.h"
#include "Camera.h"
#include "Vertex.h"
#include "Effects.h"
#include "xnacollision.h"
#include "SpriteAnimation.h"
#include <iostream>

//CURRENTLY around 600Bytes

class Entity
{
public:
	//TYPES 0:grid 1:sphere 2:URSquare 3:Box 4:FR/B Squares
	Entity(int type, std::string label, float width, float height, float depth = 0.0f);
	~Entity();
	void SetPos(float x, float y, float z);
	void SetRot(float x, float y, float z);
	void Update(const Camera& camera, float dt);
	void Draw(ID3DX11EffectTechnique* activeTech, ID3D11DeviceContext* context, UINT pass, const Camera& camera, float dt);
	void Draw2D(ID3DX11EffectTechnique* activeTech, ID3D11DeviceContext* context, UINT pass, const Camera& camera, XMMATRIX& ortho);
	void DrawShadow(ID3DX11EffectTechnique* activeTech, ID3D11DeviceContext* context, const Camera& camera, XMFLOAT4X4 lightView, XMFLOAT4X4 lightProj);
	void SetVertexOffset(int offSet);
	void SetIndexOffset(int offSet);
	void LoadVertData(std::vector<Vertex::Basic32>& verts, UINT& k);
	void LoadTexture(ID3D11Device* device, std::wstring texFilename);
	void UseTexture(ID3D11ShaderResourceView* tex);
	void SetSphereCollider(float radius);
	void ResetLookUpRight();
	void SetUpAnimation(float cols, float rows, float FPS, float animSpeed = 1.0f, bool isLooping = true);
	void UpdateAAB();
	void SetShadTrans(XMMATRIX& shadow);
	void SetShadowScale(float x, float y, float z);
	
	//CUSTOM PERMANENT MOVEMENTS
	void SetToSpin(float mult, bool spin);
	void SetToGoUpAndDown(float mult,float height, bool updown);
	void SetToFlip(float mult, bool flip);
	void SetToRoll(float mult, bool roll);
	void SetToOrbit(float mult, bool orbit);
	void SetSideToSide(float mult, float dist, bool b);
	void SetBackAndForth(float mult, float dist, bool b);
	void SetPulse(float mult, float dist, bool b);
	void SetGrowIn(float mult, bool b);
	void SetGrowOut(float mult, bool b);
	void GrowIn(float dt);
	void GrowOut(float dt);
	void GoUpDown(float dt);
	void GoSideToSide(float dt);
	void GoBackAndForth(float dt);
	void Pulse(float dt);


	// Strafe/Walk
	void Strafe(float d);
	void Jump(float d);
	void Roll(float d);
	void Walk(float d);

	// Rotate 
	void Pitch(float angle);
	void Yaw(float angle);
	void RotateY(float angle);
	void RotateX(float angle);
	void RotateZ(float angle);
	void SetOrbitPos(float x, float y, float z, float dt);

	//Scale
	void ScaleWhole(float scale);
	void ScaleX(float scale);

	void LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp);
	void LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up);
	void SetGoToPoint(float x, float y, float z);


	//Getters
	int GetVertOffset();
	int GetIndOffset();

	XMVECTOR GetRightXM()const;
	XMFLOAT3 GetRight()const;
	XMVECTOR GetUpXM()const;
	XMFLOAT3 GetUp()const;
	XMVECTOR GetLookXM()const;
	XMFLOAT3 GetLook()const;
	XMVECTOR GetPositionXM()const;


	GeometryGenerator::MeshData GetMeshData();

	ID3D11ShaderResourceView*	mTexSRV;
	Material					mMat;
	XMFLOAT4X4					mWorld;
	int							mVertexOffset;
	UINT						mIndexOffset;
	UINT						mIndexCount;
	GeometryGenerator::MeshData mGrid;


	XMFLOAT4X4 mShadowTrans;
	XMFLOAT3 mShadowScale;
	XMFLOAT3 mPosition;
	XMFLOAT3 mGoToPos;
	XMFLOAT3 mRight;
	XMFLOAT3 mUp;
	XMFLOAT3 mLook;

	int mWidth;
	int mHeight;
	float mTexWidth;
	float mTexHeight;

	XMFLOAT3 texTrans;
	XMFLOAT3 texTransMult;
	XMFLOAT3 origTexScale;

	float rotationY;
	float rotationZ;
	float prevPitch;
	float prevRoll;
	float currProgress;
	float mDistanceLeft;
	float turnAngle;
	float explosionDist;

	bool hovering;
	bool clicked;
	bool useTexTrans;
	bool progressBar;
	bool goToPos;
	bool billboard;
	bool flipUpright;
	bool reverseLook;
	bool mDead;
	bool mExplode;
	bool mBasicTexTrans;
	bool mUseAnimation;
	bool mUseAAB;
	bool mUseAABOnce;

	//CUSTOM MOVEMENTS
	bool mSpinning;
	float mSpinMult;
	bool  mUpDown;
	float mHeightToGo;
	bool  mGoUp;
	bool  mGoDown;
	float mOrigY;
	float mOrigX;
	float mOrigZ;
	float mUpDownMult;
	float movementMult;
	bool  mFlipping;
	bool  mRolling;
	bool  mSideToSide;
	bool  mBackAndForth;
	bool mGrowIn;
	bool mPulse;
	bool mGrow;
	bool mShrink;
	bool mOrbit;
	bool mGrowOut;
	bool mGrowing;
	bool mFlipTexture;
	float mScale;
	
	std::string mLabel;

	//SYSTEM COPIES OF MESH FOR PICKING
	std::vector<Vertex::Basic32> mMeshVertices;
	std::vector<UINT> mMeshIndices;

	XNA::AxisAlignedBox mMeshBox;
	XNA::Sphere			mSphereCollider;
	SpriteAnimation*	mAnim;
};

#endif // Button_h__
