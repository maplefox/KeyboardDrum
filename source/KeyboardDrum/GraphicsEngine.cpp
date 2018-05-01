#include "Common.h"
#include "GraphicsEngine.h"
#include "GameException.h"


;
using namespace MapleDrum;
GraphicsEngine::GraphicsEngine(wstring shaderPath,vector<XMFLOAT3> vertexes,int windowWidth, int windowHeight):
	Game(windowWidth,windowHeight),mShaderPath(shaderPath),mVertexes(vertexes)
{

}


GraphicsEngine::~GraphicsEngine()
{
	Release();	
}

void GraphicsEngine::Initialize(HINSTANCE instance, HWND window, const vector<Particle>* particles, const XMFLOAT4X4* view, const XMFLOAT4X4* proj)
{
	mInstance = instance;
	mWindow = window;
	InitializeDirectX();
	InitDx();
	mParticles = particles;
	mView = view;
	mProj = proj;
}
void GraphicsEngine::Release()
{
	ReleaseObject(mInputLayout);
	ReleaseObject(mVertexSRV);
	ReleaseObject(mParticleBuffer);
	ReleaseObject(mEffect);
	Game::Release();
}

void GraphicsEngine::InitDx()
{
	// Compile the shader
	UINT shaderFlags = 0;

#if defined( DEBUG ) || defined( _DEBUG )
	shaderFlags |= D3DCOMPILE_DEBUG;
	shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3D10Blob* compiledShader = nullptr;
	ID3D10Blob* errorMessages = nullptr;
	HRESULT hr = D3DCompileFromFile(mShaderPath.c_str(), nullptr, nullptr, nullptr, "fx_5_0", shaderFlags, 0, &compiledShader, &errorMessages);
	if (FAILED(hr))
	{
		char* errorMessage = (errorMessages != nullptr ? (char*)errorMessages->GetBufferPointer() : "D3DX11CompileFromFile() failed");
		string err = errorMessage;
		GameException ex(errorMessage, hr);
		ReleaseObject(errorMessages);
		
		throw ex;
	}

	// Create an effect object from the compiled shader
	hr = D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), 0, mDirect3DDevice, &mEffect);
	if (FAILED(hr))
	{
		throw GameException("D3DX11CreateEffectFromMemory() failed.", hr);
	}

	ReleaseObject(compiledShader);

	// Look up the technique, pass, and WVP variable from the effect
	mTechnique = mEffect->GetTechniqueByName("main11");
	if (mTechnique == nullptr)
	{
		throw GameException("ID3DX11Effect::GetTechniqueByName() could not find the specified technique.", hr);
	}

	mPass = mTechnique->GetPassByName("p0");
	if (mPass == nullptr)
	{
		throw GameException("ID3DX11EffectTechnique::GetPassByName() could not find the specified pass.", hr);
	}


	ID3DX11EffectVariable* variable = mEffect->GetVariableByName("View");
	if (variable == nullptr)
	{
		throw GameException("ID3DX11Effect::GetVariableByName() could not find the specified variable.", hr);
	}

	mVarView = variable->AsMatrix();
	variable->Release();
	if (mVarView->IsValid() == false)
	{
		throw GameException("Invalid effect variable cast.");
	}


	variable = mEffect->GetVariableByName("Projection");
	if (variable == nullptr)
	{
		throw GameException("ID3DX11Effect::GetVariableByName() could not find the specified variable.", hr);
	}

	mVarProj = variable->AsMatrix();
	variable->Release();
	if (mVarProj->IsValid() == false)
	{
		throw GameException("Invalid effect variable cast.");
	}


	variable = mEffect->GetVariableByName("Vertexes");
	mVarVertexBuffer = variable->AsShaderResource();
	variable->Release();

	


	// Create the input layout
	D3DX11_PASS_DESC passDesc;
	mPass->GetDesc(&passDesc);

	D3D11_INPUT_ELEMENT_DESC inputElementDescriptions[] =
	{
		{ "VERSTART", 0, DXGI_FORMAT_R32_SINT,       0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "VERNUM",    0, DXGI_FORMAT_R32_SINT     , 0, 4, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WORLD",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 32,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WORLD",    1, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA	, 0 },
		{ "WORLD",    2, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 64, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WORLD",    3, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 80, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	if (FAILED(hr = mDirect3DDevice->CreateInputLayout(inputElementDescriptions, ARRAYSIZE(inputElementDescriptions), passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &mInputLayout)))
	{
		throw GameException("ID3D11Device::CreateInputLayout() failed.", hr);
	}




	// Create the vertex buffer

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.ByteWidth = sizeof(XMFLOAT3)*mVertexes.size();
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	//vertexBufferDesc.StructureByteStride = sizeof(XMFLOAT3);
	D3D11_SUBRESOURCE_DATA vertexSubResourceData;
	ZeroMemory(&vertexSubResourceData, sizeof(vertexSubResourceData));
	vertexSubResourceData.pSysMem = &mVertexes[0];

	ID3D11Buffer* vertexBuffer;

	if (FAILED(mDirect3DDevice->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, &vertexBuffer)))
	{
		throw GameException("ID3D11Device::CreateBuffer() failed.");
	}
	D3D11_SHADER_RESOURCE_VIEW_DESC vertexSRVDesc;
	ZeroMemory(&vertexSRVDesc, sizeof(vertexSRVDesc));
	vertexSRVDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	vertexSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	vertexSRVDesc.Buffer.ElementOffset = 0;
	vertexSRVDesc.Buffer.NumElements = mVertexes.size();
	if (FAILED(mDirect3DDevice->CreateShaderResourceView(vertexBuffer, &vertexSRVDesc, &mVertexSRV)))
	{
		throw GameException("ID3D11Device::CreateShaderResourceView failed.");
	}
	vertexBuffer->Release();
	
	

	// Create the particle buffer

	D3D11_BUFFER_DESC particleBufferDesc;
	ZeroMemory(&particleBufferDesc, sizeof(particleBufferDesc));
	particleBufferDesc.ByteWidth = sizeof(Particle) * MaxInstanceNum;
	particleBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	particleBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	particleBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	if (FAILED(mDirect3DDevice->CreateBuffer(&particleBufferDesc, nullptr, &mParticleBuffer)))
	{
		throw GameException("ID3D11Device::CreateBuffer() failed.");
	}


	//Create OffScreen texture


}

void GraphicsEngine::Draw()
{
	float backgroundColor[] = { 0,0,0,1.0 };
	mDirect3DDeviceContext->ClearRenderTargetView(mRenderTargetView, backgroundColor);
	mDirect3DDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	float backBlendColor[] = { 0,0,0,0 };
	
	
	mDirect3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	mDirect3DDeviceContext->IASetInputLayout(mInputLayout);
	
	D3D11_MAPPED_SUBRESOURCE mappedData;
	auto hr = mDirect3DDeviceContext->Map(mParticleBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	if (FAILED(hr))
		throw GameException("map failed", hr);
	auto particlePtr = (Particle*)mappedData.pData;
	auto& particles = *mParticles;
	if(particles.size()>0)
		CopyMemory(particlePtr, &particles[0], particles.size() * sizeof(Particle));
	

	mDirect3DDeviceContext->Unmap(mParticleBuffer, 0);
	
	UINT stride = sizeof(Particle);
	UINT offset = 0;
	mDirect3DDeviceContext->IASetVertexBuffers(0, 1, &mParticleBuffer, &stride, &offset);
 	//auto mat = XMLoadFloat4x4(mViewProj);
	
	mVarView->SetMatrix(mView->m[0]);
	mVarProj->SetMatrix(mProj->m[0]);
	mVarVertexBuffer->SetResource(mVertexSRV);
	mPass->Apply(0, mDirect3DDeviceContext);
	
	mDirect3DDeviceContext->Draw(particles.size(), 0);
	mSwapChain->Present(1, 0);
}