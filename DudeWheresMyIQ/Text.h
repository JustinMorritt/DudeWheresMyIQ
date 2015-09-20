#ifndef Text_h__
#define Text_h__

#include <vector>
#include <iostream>
#include "Entity.h"
#include "d3dUtil.h"


class Text
{
public:
	Text(std::string text, float x, float y, float size, int BG);
	~Text();
	static void Init(ID3D11Device** device);
	void Update(const Camera& cam, float dt);
	void MakeLetter(char letter, float x, float y, float size);
	void DrawText2D(ID3DX11EffectTechnique** activeTech, ID3D11DeviceContext* context, UINT pass, const Camera& camera, XMMATRIX& ortho);
	void DrawText3D();
	void ShutDown();

	static ID3D11ShaderResourceView* mLetters;
	static ID3D11ShaderResourceView* mBG1;

	static ID3D11Device* mDevice;
	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;

	std::vector<Entity*> mText;
	float numLetters;
};

#endif // Text_h__
