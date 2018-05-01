#pragma once

#include "TypeDef.h"
#include "Game.h"


using namespace MapleDrum;
class GraphicsEngine:public Game
{
public:
	GraphicsEngine(wstring shaderPath, vector<XMFLOAT3> vertexes, int windowWidth, int windowHeight);
	~GraphicsEngine();
	
	void Initialize(HINSTANCE instance, HWND window,
		const vector<Particle>* particles, const XMFLOAT4X4* view, const XMFLOAT4X4* proj);
	void Draw();
	void Release();
private:
	const static int MaxInstanceNum = 8096;
	void InitDx();
	

	vector<XMFLOAT3> mVertexes;
	const vector<Particle>* mParticles;
	const XMFLOAT4X4* mView;
	const XMFLOAT4X4* mProj;
	wstring mShaderPath;
	ID3D11InputLayout* mInputLayout;
	ID3D11ShaderResourceView* mVertexSRV;
	ID3D11ShaderResourceView * mScreenOffSRV;
	ID3D11RenderTargetView* mScreenOffRTV;
	ID3D11Buffer* mParticleBuffer;
	ID3DX11Effect* mEffect;
	ID3DX11EffectTechnique* mTechnique;
	ID3DX11EffectPass* mPass;
	ID3DX11EffectMatrixVariable* mVarView;
	ID3DX11EffectMatrixVariable* mVarProj;
	ID3DX11EffectShaderResourceVariable* mVarVertexBuffer;
	ID3DX11EffectShaderResourceVariable* mVarScreenOff;


};
