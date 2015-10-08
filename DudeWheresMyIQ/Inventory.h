#ifndef Inventory_h__
#define Inventory_h__

#include "Text.h"

class Player;
class Engine;
//TODO: Make BG image for inventory with the grid system and a default item block... this will be a button
//Each individual Button Depending on What is in The inventory will have that as its texture.
class Inventory
{
public:
	Inventory();
	~Inventory();
	static void Init(ID3D11Device** device);
	static void Update(const Camera& cam, float dt);
	void UpdateStats();
	static void UpdateAbilitys(const Camera& cam, float dt);
	void MakeInv(float x, float y, float size);
	void MakeAbility(float x, float y, float size);
	static bool AddToInventory(std::string item);
	static bool AddToAbilitys(std::string item);
	static void AssignItem(Entity* E, std::string item);
	void InventoryOn();
	void HoveringItem(int E);
	void HoveringAbility(int E);
	void NotHovering(int E);
	void AddToIQ(float num);
	void AddToJump(float num);
	void AddToSpeed(float num);
	void NotHoveringAbility(int E);
	static void BuildBattleAbilitys();
	void SetDescription(std::string item);
	void SetAbilityDescription(std::string item);
	static int NumAbilitys();
	std::vector<Text*> GetText();
	void UseItem(int E);
	void UseAbility(int E);
	void Draw(ID3DX11EffectTechnique** activeTech, ID3D11DeviceContext* context, UINT pass, const Camera& camera, XMMATRIX& ortho);
	static void DrawAbilitys(ID3DX11EffectTechnique** activeTech, ID3D11DeviceContext* context, UINT pass, const Camera& camera, XMMATRIX& ortho); //Used in Battle
	void ShutDown();
	std::vector<Entity*> GetItems();

	static ID3D11ShaderResourceView* mBG;
	static ID3D11ShaderResourceView* mEmpty;
	static ID3D11ShaderResourceView* mBeer;
	static ID3D11ShaderResourceView* mPill;
	static ID3D11ShaderResourceView* mMoonshine;
	static ID3D11ShaderResourceView* mApple;
	static ID3D11ShaderResourceView* mRubix;
	static ID3D11ShaderResourceView* mFish;
	static ID3D11ShaderResourceView* mFishRod;
	static ID3D11ShaderResourceView* mLight;
	static ID3D11ShaderResourceView* mBook;
	static ID3D11ShaderResourceView* mChicken;
	static ID3D11ShaderResourceView* mBandaid;
	static ID3D11ShaderResourceView* mFire;
	static ID3D11ShaderResourceView* mTools;

	//Then All The items ...

	static ID3D11Device* mDevice;

	ID3D11Buffer* mVB; 	static ID3D11Buffer* mVB2; //For abilitys 
	ID3D11Buffer* mIB;	static ID3D11Buffer* mIB2;

	static std::vector<std::string> mItemStrings;
	static std::vector<std::string> mAbilityStrings; // permanent copies of the current Learned abilities
	static std::vector<std::string> mBattleStrings;  // insert all them into here during a battle then they can be emptied as they are used ..

	static std::vector<Entity*> mItems;
	static std::vector<Entity*> mItemButtons;

	static std::vector<Entity*> mAbilitys;
	static std::vector<Entity*> mAbilityButtons;
	static std::vector<Entity*> mBattleButtons;
	
	static std::vector<Text*> mTitles; // Storage for all the Descriptions.
	static std::vector<Text*> mText; // Storage for all the Descriptions.
	static Text* mItemDescription; // This will be used for drawing the text apply whatever the description to the item when hovering on it .
	Text* mTitle;

	static int mSlots;
};
#endif // Inventory_h__

