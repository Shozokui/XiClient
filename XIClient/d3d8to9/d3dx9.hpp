#pragma once

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <d3d9.h>
#ifndef D3DXINLINE
#ifdef _MSC_VER
#if (_MSC_VER >= 1200)
#define D3DXINLINE __forceinline
#else
#define D3DXINLINE __inline
#endif
#else
#ifdef __cplusplus
#define D3DXINLINE inline
#else
#define D3DXINLINE
#endif
#endif
#endif
#define D3DX_FILTER_NONE 1

#define D3DXASM_DEBUG 0x0001
#define D3DXASM_SKIPVALIDATION  0x0010

#ifdef NDEBUG
#define D3DXASM_FLAGS  0
#else
#define D3DXASM_FLAGS D3DXASM_DEBUG
#endif // NDEBUG

struct D3DXMACRO
{
	LPCSTR Name;
	LPCSTR Definition;
};

typedef interface ID3DXBuffer *LPD3DXBUFFER;
typedef interface ID3DXInclude *LPD3DXINCLUDE;

DECLARE_INTERFACE_(ID3DXBuffer, IUnknown)
{
	// IUnknown
	STDMETHOD(QueryInterface)(THIS_ REFIID iid, LPVOID *ppv) PURE;
	STDMETHOD_(ULONG, AddRef)(THIS) PURE;
	STDMETHOD_(ULONG, Release)(THIS) PURE;

	// ID3DXBuffer
	STDMETHOD_(LPVOID, GetBufferPointer)(THIS) PURE;
	STDMETHOD_(DWORD, GetBufferSize)(THIS) PURE;
};

typedef HRESULT(WINAPI *PFN_D3DXAssembleShader)(LPCSTR pSrcData, UINT SrcDataLen, const D3DXMACRO *pDefines, LPD3DXINCLUDE pInclude, DWORD Flags, LPD3DXBUFFER *ppShader, LPD3DXBUFFER *ppErrorMsgs);
typedef HRESULT(WINAPI *PFN_D3DXDisassembleShader)(const DWORD *pShader, BOOL EnableColorCode, LPCSTR pComments, LPD3DXBUFFER *ppDisassembly);
typedef HRESULT(WINAPI *PFN_D3DXLoadSurfaceFromSurface)(LPDIRECT3DSURFACE9 pDestSurface, const PALETTEENTRY *pDestPalette, const RECT *pDestRect, LPDIRECT3DSURFACE9 pSrcSurface, const PALETTEENTRY *pSrcPalette, const RECT *pSrcRect, DWORD Filter, D3DCOLOR ColorKey);
typedef HRESULT(WINAPI *PFN_D3DXLoadSurfaceFromMemory)(LPDIRECT3DSURFACE9 pDestSurface, const PALETTEENTRY *pDestPalette, const RECT *pDestRect, LPCVOID pSrcMemory, D3DFORMAT SrcFormat, UINT SrcPitch, const PALETTEENTRY *pSrcPalette, const RECT *pSrcRect, DWORD Filter, D3DCOLOR ColorKey);
typedef HRESULT(WINAPI* PFN_D3DXLoadSurfaceFromFileInMemory)(LPDIRECT3DSURFACE9, PALETTEENTRY*, RECT*, LPCVOID, UINT, RECT*, DWORD, D3DCOLOR, void*);
typedef HRESULT(WINAPI* PFN_D3DXCreateTexture)(LPDIRECT3DDEVICE9 pDevice, UINT Width, UINT Height, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, LPDIRECT3DTEXTURE9* ppTexture);
typedef HRESULT(WINAPI* PFN_D3DXCheckTextureRequirements)(LPDIRECT3DDEVICE9 pDevice, UINT* pWidth, UINT* pHeight, UINT* pNumMipLevels, DWORD Usage, D3DFORMAT* pFormat, D3DPOOL Pool);
typedef HRESULT(WINAPI* PFN_D3DXCheckCubeTextureRequirements)(LPDIRECT3DDEVICE9 pDevice, UINT* pSize, UINT* pNumMipLevels, DWORD Ussage, D3DFORMAT* pFormat, D3DPOOL Pool);
typedef HRESULT(WINAPI* PFN_D3DXCreateCubeTexture)(LPDIRECT3DDEVICE9 pDevice, UINT Size, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, LPDIRECT3DCUBETEXTURE9* ppCubeTexture);

extern PFN_D3DXAssembleShader D3DXAssembleShader;
extern PFN_D3DXDisassembleShader D3DXDisassembleShader;
extern PFN_D3DXLoadSurfaceFromSurface D3DXLoadSurfaceFromSurface;
extern PFN_D3DXLoadSurfaceFromMemory D3DXLoadSurfaceFromMemory;
extern PFN_D3DXLoadSurfaceFromFileInMemory D3DXLoadSurfaceFromFileInMemory;
extern PFN_D3DXCreateTexture D3DXCreateTexture;
extern PFN_D3DXCheckTextureRequirements D3DXCheckTextureRequirements;
extern PFN_D3DXCheckCubeTextureRequirements D3DXCheckCubeTextureRequirements;
extern PFN_D3DXCreateCubeTexture D3DXCreateCubeTexture;