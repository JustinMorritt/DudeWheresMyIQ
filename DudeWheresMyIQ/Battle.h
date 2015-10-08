#ifndef Battle_h__
#define Battle_h__
#include "Text.h"
#include "LevelSection.h"
#include "Player.h"
#include "Inventory.h"

class Battle
{
public:
	Battle();
	~Battle();
	static void Init(ID3D11Device** device);
	void ShutDown();
	void Draw(ID3DX11EffectTechnique** activeTech, ID3D11DeviceContext* context, UINT pass, const Camera& camera, XMMATRIX& ortho);
	void SetDescription(std::string item);
	void UsedAbility(std::string ability);
	void CPUUsedAbility();
	void AssignTitle();
	void AssignState();
	void UpdateIQs();
	void MakeMove();
	void Wait(int i);

	void DrawAbilitys(ID3DX11EffectTechnique** activeTech, ID3D11DeviceContext* context, UINT pass, const Camera& camera, XMMATRIX& ortho);
	void Update(const Camera& cam, float dt);
	void InsertAllText();
	static void SetBattleType(std::string type);

	static ID3D11ShaderResourceView* mBG;
	static ID3D11ShaderResourceView* mBrainP;
	static ID3D11ShaderResourceView* mBrainCPU;
	static ID3D11ShaderResourceView* mBrainBar;
	static ID3D11ShaderResourceView* mBrainBarOL;


	std::vector<Entity*> mEntities;
	std::vector<Entity*> mButtons;
	
	static std::vector<Text*> mTitleText;	//Different Titles .. depending on the State the battle is in .
	static std::vector<Text*> mAbilityUsedText;	//Different Titles .. depending on the State the battle is in .
	static std::vector<Text*> mDumbAssUsedText;	//Different Titles .. depending on the State the battle is in .

	static std::vector<Text*> mText;		// Storage for all the Ability Descriptions.

	static Text* mAbilityUsed;
	static Text* mDumbyUsed;
	static Text* mTitle;
	static Text* mLeaveMsg;

	static int mWaitTimer;
	int mWaitAndAssign;
	float mTickTimer;
	static bool mMove;
	static bool mState;
	bool CpuMove;
	float mPIQ;
	float mCPIQ;

	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;
	static ID3D11Device* mDevice;

	static std::string mType;
};
#endif // Battle_h__

