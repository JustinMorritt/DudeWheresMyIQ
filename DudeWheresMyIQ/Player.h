#ifndef Player_h__
#define Player_h__
#include "Entity.h"
#include <vector>
#include "MathHelper.h"

class Player
{
public:
	Player(ID3D11Device** device);
	~Player();
	void ResetPlayerPos();
	void Update(const Camera& camera, float dt) ;
	void Draw(ID3DX11EffectTechnique** activeTech, ID3D11DeviceContext* context, UINT pass, const Camera& camera, float dt);
	void InsertCollisionItems(std::vector<Entity*> entities);
	void EmptyCollisionItems();
	void CheckCollisions();
	void Applyforces(float dt);
	void Move(float dt);
	void Jump();


	float mMaxSpeed;
	float mAccel;
	XMFLOAT3 mVelocity;
	
	//GO's ANd SLOWS  FW:forward BW:Backward SL:slow
	bool GoFW, GoBW, GoIn, GoOut, SlFW, SlBW, SlIn, SlOut, onGround;

	//COLLISION ITEMS
	std::vector<std::vector<Entity*>> mEntities;

	Entity* mSelf;
	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;

private:
	ID3D11Device* mDevice;

};
#endif // Player_h__

