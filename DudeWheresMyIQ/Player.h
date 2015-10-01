#ifndef Player_h__
#define Player_h__
#include "Entity.h"
#include <vector>
#include "MathHelper.h"
#include "Inventory.h"
#include "StateMachine.h"
#include "Battle.h"

class Engine;

class Player
{
public:
	Player(ID3D11Device** device);
	~Player();
	void ResetPlayerPos();
	void ResetPlayerStats();
	void Update(const Camera& camera, float dt) ;
	void Draw(ID3DX11EffectTechnique** activeTech, ID3D11DeviceContext* context, UINT pass, const Camera& camera, float dt, XMMATRIX& shadow);
	void DrawShad(ID3DX11EffectTechnique** activeTech, ID3D11DeviceContext* context, const Camera& camera, XMFLOAT4X4 lightView, XMFLOAT4X4 lightProj);
	void InsertCollisionItems(std::vector<Entity*> entities);
	void EmptyCollisionItems();
	void CheckCollisions();
	void SetItemDescription(int i);
	void Applyforces(float dt);
	void SetAnimation();
	void Move(float dt);
	void Jump();
	static Entity* GetSelf();

	static int   mLevel;
	static float mIQ;
	static float mMaxSpeed;
	static float mJumpHeight;
	float mAccel;
	XMFLOAT3 mVelocity;
	
	//GO's ANd SLOWS  FW:forward BW:Backward SL:slow
	bool GoFW, GoBW, GoIn, GoOut, SlFW, SlBW, SlIn, SlOut, onGround;

	//COLLISION ITEMS
	std::vector<std::vector<Entity*>> mEntities;

	std::vector<Text*> mText; // Storage for all the Descriptions.
	Text* mItemDescription; // This will be used for drawing the text apply whatever the description to the item when hovering on it .

	static Entity* mSelf;
	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;

private:
	ID3D11Device* mDevice;

};
#endif // Player_h__

