#ifndef Text_h__
#define Text_h__

#include <vector>
#include <iostream>
#include "Entity.h"
#include "d3dUtil.h"


class Text
{
public:
	Text(std::string text, float x, float y, float z, float size, int BG, bool ThreeD);
	~Text();
	static void Init(ID3D11Device** device);
	void Update(const Camera& cam, float dt);
	void MakeLetter(char letter, float x, float y, float z, float size);
	void DrawText2D(ID3DX11EffectTechnique** activeTech, ID3D11DeviceContext* context, UINT pass, const Camera& camera, XMMATRIX& ortho);
	void DrawText3D(ID3DX11EffectTechnique** activeTech, ID3D11DeviceContext* context, UINT pass, const Camera& camera, float dt);
	void ShutDown();

	static ID3D11ShaderResourceView* mLetters;
	static ID3D11ShaderResourceView* mBG1;

	static ID3D11Device* mDevice;
	float numLetters;

	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;
	std::vector<Entity*> mText;
};

#endif // Text_h__
