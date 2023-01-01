#pragma once
#define WIN32_LEAN_AND_MEAN
#include "StorageListObject.h"
#include "CAcc.h"
#include "CAcc2.h"
#include "DxGamma.h"
#include "d3d8to9/d3d8to9.hpp"
#include "Values.h"
#include "FVF44Vertex.h"
#include <wtypes.h>
namespace FFXI {
	namespace CYy {
		class CDx : public StorageListObject
		{
		public:
			static CDx* instance;
			static bool Initialize(IDirect3D8*, Direct3DDevice8*);
			static void Uninitialize();
			const static CYyClass CDxClass;
			virtual const CYyClass* GetRuntimeClass() override final;
			CDx(IDirect3D8*, Direct3DDevice8*);
			virtual ~CDx();
			static FVF44Vertex SomeBasicVertices[4];
			char CheckDeviceCaps();
			bool SetSurface(IDirect3DSurface8*, IDirect3DSurface8*);
			bool CheckSurface(IDirect3DTexture8**, IDirect3DSurface8**);
			bool DeviceGood();
			int BeginScene();
			int EndScene();
			void SetFromCursors(unsigned char);
			void SetSomeValues(int, D3DCOLOR, float, int);
			void SetViewport();
			int AddViewportAtOrigin(IDirect3DTexture8*, IDirect3DTexture8*, IDirect3DSurface8*);
			int AddViewport(IDirect3DTexture8*, IDirect3DTexture8*, IDirect3DSurface8*, short*, short*, int, int);
			D3DVIEWPORT8* GetTopViewport();
			void AddSetViewport(D3DVIEWPORT8*);
			int ClearFullViewport(DWORD, D3DCOLOR, float, DWORD);
			int ClearViewport(DWORD, D3DRECT*, DWORD, D3DCOLOR, float, DWORD);;
			void RevertStage();
			void RewindStage();
			void ShowCursor(bool);
			IDirect3DTexture8* CreateTexture(unsigned short, unsigned short, IDirect3DTexture8**, int, D3DPOOL);
			IDirect3DSurface8* CreateDepthStencilSurface(unsigned short, unsigned short);
			void SetRenderStates();
			bool StateBlockFunc(D3DSTATEBLOCKTYPE, DWORD*);
			void StateBlockFunc2(int, DWORD*, char);
			void SomeFunc(float, float, int);
			void SetSomeColor(D3DCOLOR);
			void GetTransform(D3DTRANSFORMSTATETYPE, D3DMATRIX*);
			HRESULT SetTransform(D3DTRANSFORMSTATETYPE, D3DMATRIX*);
			HRESULT SetLight(int, D3DLIGHT8*);
			bool GammaAffectLight(D3DLIGHT8*);
			void DrawVerts(D3DPRIMITIVETYPE, int, const void*, int);
			void DoViewportRender(IDirect3DBaseTexture8*, D3DCOLOR, int);
			void RenderTexOnViewport(IDirect3DBaseTexture8*, D3DCOLOR, DWORD*, float*, int);
			void DrawViewportWithZedd(IDirect3DBaseTexture8*, float);
			void SomeCAcc2Render(CAcc2*, D3DCOLOR, DWORD*);
			void DrawSomeVerts(int, int);
			void DrawStream(D3DPRIMITIVETYPE, int, int);
			void DrawStreamIndexed(D3DPRIMITIVETYPE, int, int, int, int);
			void SetActiveVertexShader(DWORD);
			char field_4;
			char field_5;
			char field_6;
			char field_7;
			IDirect3D8* DXInterface;
			Direct3DDevice8* DXDevice;
			IDirect3DTexture8* Textures2[8];
			IDirect3DSurface8* RenderTargets[8];
			IDirect3DTexture8* Textures[8];
			IDirect3DSurface8* Stencils[8];
			IDirect3DSurface8* BackbufferSurface;
			IDirect3DSurface8* StencilDepthSurface;
			D3DVIEWPORT8 Viewports[8];
			char ViewportIndex;
			char field_159;
			char field_15A;
			char field_15B;
			int AppWidth;
			int AppHeight;
			int field_164;
			D3DCOLOR SomeColor;
			float field_16C;
			int field_170;
			float field_174;
			float field_178;
			float field_17C;
			float field_180;
			float field_184;
			float field_188;
			int field_18C;
			CAcc2 CAcc2;
			CAcc CAcc_1;
			CAcc CAcc_2;
			CAcc CAcc_3;
			CAcc CAcc_4;
			DWORD StateBlock1;
			DWORD StateBlock2;
			DxGamma Gamma;
			int field_810;
			int field_814;
			int field_818;
			int field_81C;
			int field_820;
			int field_824;
			char field_828;
			char field_829;
			char field_82A;
			char field_82B;
			int field_82C;
			int field_830;
			D3DCAPS8 DeviceCaps;
			char field_908;
			char field_909;
			char field_90A;
			char field_90B;
			__int16 field_90C;
			__int16 field_90E;
			char field_910;
			char field_911;
			char field_912;
			char field_913;
			char field_914;
			char field_915;
			char field_916;
			char field_917;
			int field_918;
			int FullscreenMultisampleCount;
			char GPUVendor;
			char field_921;
			char field_922;
			char field_923;
			DWORD VertexBufferUsage;
			D3DPOOL VertexBufferPool;
			char field_92C;
			char field_92D;
			char field_92E;
			char field_92F;
			char field_930;
			char field_931;
			char field_932;
			char field_933;
			int field_934;
			int field_938;
			D3DMATRIX WorldTransform;
			D3DMATRIX ViewTransform;
			D3DMATRIX ProjectionTransform;
			int field_9FC;
			int field_A00;
			HCURSOR Cursors[18];
			int field_A4C;
			
		};
	}
}