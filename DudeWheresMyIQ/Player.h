#ifndef Player_h__
#define Player_h__
#include "Entity.h"

class Player
{
public:
	Player(ID3D11Device** device);
	~Player();
	void ResetPlayerPos();
	void Update(const Camera& camera, float dt) ;
	void Draw(ID3DX11EffectTechnique** activeTech, ID3D11DeviceContext* context, UINT pass, const Camera& camera, float dt);

	//Forward on X
	void WalkFW(float dt);
	//Backward on X
	void WalkBW(float dt);
	//Toward on Z
	void WalkTW(float dt);
	//Away on Z
	void WalkAW(float dt);

	float mMaxSpeed;
	XMFLOAT3 mVelocity;
	XMFLOAT3 mCurrSpeed;
	
	//GO's ANd SLOWS  FW:forward BW:Backward SL:slow
	bool GoFW, GoBW, GoIn, GoOut, SlFW, SlBW, SlIn, SlOut;



	Entity* mSelf;
	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;

private:
	ID3D11Device* mDevice;
};
#endif // Player_h__

