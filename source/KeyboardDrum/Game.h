#pragma once

#include "TypeDef.h"


//using namespace MapleDrum;
using namespace std;
namespace MapleDrum
{
    class Game
    {
    public:
        Game(int screenWidth,int screenHeight);
        virtual ~Game();
        		   
		

	protected:
		void InitializeDirectX();



		static const UINT DefaultScreenWidth;
		static const UINT DefaultScreenHeight;
		static const UINT DefaultFrameRate;
		static const UINT DefaultMultiSamplingCount;


		HINSTANCE mInstance;
		HWND mWindow;


		UINT mScreenWidth;
		UINT mScreenHeight;

		ID3D11Device1* mDirect3DDevice;
		ID3D11DeviceContext1* mDirect3DDeviceContext;
		IDXGISwapChain1* mSwapChain;

		UINT mFrameRate;
		bool mIsFullScreen;
		UINT mMultiSamplingCount;
		UINT mMultiSamplingQualityLevels;

		ID3D11Texture2D* mDepthStencilBuffer;
		ID3D11RenderTargetView* mRenderTargetView;
		ID3D11DepthStencilView* mDepthStencilView;
		void Release();
    private:
		
        Game(const Game& rhs);
        Game& operator=(const Game& rhs);

    };
}