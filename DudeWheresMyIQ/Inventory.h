#ifndef Inventory_h__
#define Inventory_h__

#include "Text.h"

//TODO: Make BG image for inventory with the grid system and a default item block... this will be a button
//Each individual Button Depending on What is in The inventory will have that as its texture.
class Inventory
{
public:
	Inventory();
	~Inventory();
	static void Init(ID3D11Device** device);
	void Update(const Camera& cam, float dt);
	void MakeInv(float x, float y, float size);
	static bool AddToInventory(std::string item);
	void AssignItem(Entity* E, std::string item);
	void HoveringItem(int E);
	void NotHovering(int E);
	void SetDescription(std::string item);
	std::vector<Text*> GetText();
	void UseItem(int E);
	void Draw(ID3DX11EffectTechnique** activeTech, ID3D11DeviceContext* context, UINT pass, const Camera& camera, XMMATRIX& ortho); 
	void ShutDown();
	std::vector<Entity*> GetItems();

	static ID3D11ShaderResourceView* mBG;
	static ID3D11ShaderResourceView* mEmpty;
	static ID3D11ShaderResourceView* mBeer;

	//Then All The items ...

	static ID3D11Device* mDevice;

	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;

	static std::vector<std::string> mItemStrings;
	std::vector<Entity*> mItems;
	std::vector<Entity*> mItemButtons;
	
	std::vector<Text*> mText; // Storage for all the Descriptions.
	Text* mItemDescription; // This will be used for drawing the text apply whatever the description to the item when hovering on it .
	Text* mTitle;

	static int mSlots;
};
#endif // Inventory_h__

