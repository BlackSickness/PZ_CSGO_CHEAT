#pragma once

#include "MiscDefinitions.h"
#include "VMatrix.hpp"
#include "Vector.hpp"
#include "MiscClasses.h"
#include "Vector2D.h"
#define DECLARE_POINTER_HANDLE(name) struct name##__ { int unused; }; typedef struct name##__ *name
#define MAXSTUDIOSKINS		32
#define MAXSTUDIOBONES		128

// These are given to FindMaterial to reference the texture groups that show up on the 
#define TEXTURE_GROUP_LIGHTMAP						        "Lightmaps"
#define TEXTURE_GROUP_WORLD							          "World textures"
#define TEXTURE_GROUP_MODEL							          "Model textures"
#define TEXTURE_GROUP_VGUI							          "VGUI textures"
#define TEXTURE_GROUP_PARTICLE						        "Particle textures"
#define TEXTURE_GROUP_DECAL							          "Decal textures"
#define TEXTURE_GROUP_SKYBOX						          "SkyBox textures"
#define TEXTURE_GROUP_CLIENT_EFFECTS				      "ClientEffect textures"
#define TEXTURE_GROUP_OTHER							          "Other textures"
#define TEXTURE_GROUP_PRECACHED						        "Precached"
#define TEXTURE_GROUP_CUBE_MAP						        "CubeMap textures"
#define TEXTURE_GROUP_RENDER_TARGET					      "RenderTargets"
#define TEXTURE_GROUP_UNACCOUNTED					        "Unaccounted textures"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER		  "Static Vertex"
#define TEXTURE_GROUP_STATIC_INDEX_BUFFER			    "Static Indices"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_DISP		"Displacement Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_COLOR	"Lighting Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_WORLD	"World Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_MODELS	"Model Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_OTHER	"Other Verts"
#define TEXTURE_GROUP_DYNAMIC_INDEX_BUFFER			  "Dynamic Indices"
#define TEXTURE_GROUP_DYNAMIC_VERTEX_BUFFER			  "Dynamic Verts"
#define TEXTURE_GROUP_DEPTH_BUFFER					      "DepthBuffer"
#define TEXTURE_GROUP_VIEW_MODEL					        "ViewModel"
#define TEXTURE_GROUP_PIXEL_SHADERS					      "Pixel Shaders"
#define TEXTURE_GROUP_VERTEX_SHADERS				      "Vertex Shaders"
#define TEXTURE_GROUP_RENDER_TARGET_SURFACE			  "RenderTarget Surfaces"
#define TEXTURE_GROUP_MORPH_TARGETS					      "Morph Targets"
typedef unsigned short MaterialHandle_t;

class IMaterial
{
public:
	const char* GetName()
	{
		typedef const char* (__thiscall* GetNameFn)(void*);
		return call_vfunc<GetNameFn>(this, 0)(this);
	}

	const char* GetTextureGroupName()
	{
		typedef const char* (__thiscall* GetTextureGroupNameFn)(void*);
		return call_vfunc<GetTextureGroupNameFn>(this, 1)(this);
	}

	void IncrementReferenceCount()
	{
		typedef void(__thiscall* IncrementReferenceCountFn)(void*);
		call_vfunc<IncrementReferenceCountFn>(this, 14)(this);
	}

	void AlphaModulate(float alpha)
	{
		typedef void(__thiscall* AlphaModulateFn)(void*, float);
		call_vfunc<AlphaModulateFn>(this, 27)(this, alpha);
	}

	void ColorModulate(float r, float g, float b)
	{
		typedef void(__thiscall* ColorModulateFn)(void*, float, float, float);
		call_vfunc<ColorModulateFn>(this, 28)(this, r, g, b);
	}

	void SetMaterialVarFlag(MaterialVarFlags_t flag, bool on)
	{
		typedef void(__thiscall* SetMaterialVarFlagFn)(void*, MaterialVarFlags_t, bool);
		call_vfunc<SetMaterialVarFlagFn>(this, 29)(this, flag, on);
	}
};
enum ShaderStencilOp_t
{
#if !defined( _X360 )
    SHADER_STENCILOP_KEEP = 1,
    SHADER_STENCILOP_ZERO = 2,
    SHADER_STENCILOP_SET_TO_REFERENCE = 3,
    SHADER_STENCILOP_INCREMENT_CLAMP = 4,
    SHADER_STENCILOP_DECREMENT_CLAMP = 5,
    SHADER_STENCILOP_INVERT = 6,
    SHADER_STENCILOP_INCREMENT_WRAP = 7,
    SHADER_STENCILOP_DECREMENT_WRAP = 8,
#else
    SHADER_STENCILOP_KEEP = D3DSTENCILOP_KEEP,
    SHADER_STENCILOP_ZERO = D3DSTENCILOP_ZERO,
    SHADER_STENCILOP_SET_TO_REFERENCE = D3DSTENCILOP_REPLACE,
    SHADER_STENCILOP_INCREMENT_CLAMP = D3DSTENCILOP_INCRSAT,
    SHADER_STENCILOP_DECREMENT_CLAMP = D3DSTENCILOP_DECRSAT,
    SHADER_STENCILOP_INVERT = D3DSTENCILOP_INVERT,
    SHADER_STENCILOP_INCREMENT_WRAP = D3DSTENCILOP_INCR,
    SHADER_STENCILOP_DECREMENT_WRAP = D3DSTENCILOP_DECR,
#endif
    SHADER_STENCILOP_FORCE_DWORD = 0x7fffffff
};

enum ShaderStencilFunc_t
{
#if !defined( _X360 )
    SHADER_STENCILFUNC_NEVER = 1,
    SHADER_STENCILFUNC_LESS = 2,
    SHADER_STENCILFUNC_EQUAL = 3,
    SHADER_STENCILFUNC_LEQUAL = 4,
    SHADER_STENCILFUNC_GREATER = 5,
    SHADER_STENCILFUNC_NOTEQUAL = 6,
    SHADER_STENCILFUNC_GEQUAL = 7,
    SHADER_STENCILFUNC_ALWAYS = 8,
#else
    SHADER_STENCILFUNC_NEVER = D3DCMP_NEVER,
    SHADER_STENCILFUNC_LESS = D3DCMP_LESS,
    SHADER_STENCILFUNC_EQUAL = D3DCMP_EQUAL,
    SHADER_STENCILFUNC_LEQUAL = D3DCMP_LESSEQUAL,
    SHADER_STENCILFUNC_GREATER = D3DCMP_GREATER,
    SHADER_STENCILFUNC_NOTEQUAL = D3DCMP_NOTEQUAL,
    SHADER_STENCILFUNC_GEQUAL = D3DCMP_GREATEREQUAL,
    SHADER_STENCILFUNC_ALWAYS = D3DCMP_ALWAYS,
#endif

    SHADER_STENCILFUNC_FORCE_DWORD = 0x7fffffff
};

#if defined( _X360 )
enum ShaderHiStencilFunc_t
{
    SHADER_HI_STENCILFUNC_EQUAL = D3DHSCMP_EQUAL,
    SHADER_HI_STENCILFUNC_NOTEQUAL = D3DHSCMP_NOTEQUAL,

    SHADER_HI_STENCILFUNC_FORCE_DWORD = 0x7fffffff
};
#endif
struct ShaderStencilState_t
{
    bool m_bEnable;
    ShaderStencilOp_t m_FailOp;
    ShaderStencilOp_t m_ZFailOp;
    ShaderStencilOp_t m_PassOp;
    ShaderStencilFunc_t m_CompareFunc;
    int m_nReferenceValue;
    uint32_t m_nTestMask;
    uint32_t m_nWriteMask;

#if defined( _X360 )
    bool m_bHiStencilEnable;
    bool m_bHiStencilWriteEnable;
    ShaderHiStencilFunc_t m_HiStencilCompareFunc;
    int m_nHiStencilReferenceValue;
#endif

    ShaderStencilState_t()
    {
        m_bEnable = false;
        m_PassOp = m_FailOp = m_ZFailOp = SHADER_STENCILOP_KEEP;
        m_CompareFunc = SHADER_STENCILFUNC_ALWAYS;
        m_nReferenceValue = 0;
        m_nTestMask = m_nWriteMask = 0xFFFFFFFF;

#if defined( _X360 )
        m_bHiStencilEnable = false;
        m_bHiStencilWriteEnable = false;
        m_HiStencilCompareFunc = SHADER_HI_STENCILFUNC_EQUAL;
        m_nHiStencilReferenceValue = 0;
#endif
    }
};
#define VirtualFn( cast )typedef cast( __thiscall* OriginalFn )
class CMaterialSystem
{
public:
	IMaterial* CreateMaterial(const char* pMaterialName, KeyValues* pVMTKeyValues)
	{
		VirtualFn(IMaterial*)(PVOID, const char*, KeyValues*);
		return call_vfunc< OriginalFn >(this, 83)(this, pMaterialName, pVMTKeyValues);
	}
	IMaterial* FindMaterial(const char* pMaterialName, const char* pTextureGroupName, bool complain = true, const char* pComplainPrefix = 0)
	{
		VirtualFn(IMaterial*)(PVOID, const char*, const char*, bool, const char*);
		return call_vfunc< OriginalFn >(this, 84)(this, pMaterialName, pTextureGroupName, complain, pComplainPrefix);
	}
	MaterialHandle_t FirstMaterial()
	{
		VirtualFn(MaterialHandle_t)(PVOID);
		return call_vfunc< OriginalFn >(this, 86)(this);
	}
	MaterialHandle_t NextMaterial(MaterialHandle_t h)
	{
		VirtualFn(MaterialHandle_t)(PVOID, MaterialHandle_t);
		return call_vfunc< OriginalFn >(this, 87)(this, h);
	}
	MaterialHandle_t InvalidMaterial()
	{
		VirtualFn(MaterialHandle_t)(PVOID);
		return call_vfunc< OriginalFn >(this, 88)(this);
	}
	IMaterial* GetMaterial(MaterialHandle_t h)
	{
		VirtualFn(IMaterial*)(PVOID, MaterialHandle_t);
		return call_vfunc< OriginalFn >(this, 89)(this, h);
	}
	int	GetNumMaterials()
	{
		VirtualFn(int)(PVOID);
		return call_vfunc< OriginalFn >(this, 90)(this);
	}
    void SetStencilState(const ShaderStencilState_t &state)
    {
        typedef void(__thiscall* OriginalFn)(void*, const ShaderStencilState_t&);
        call_vfunc<OriginalFn>(this, 127)(this, state);
    }
};

class IVModelRender
{
public:
	void ForcedMaterialOverride(IMaterial* material, OverrideType_t type = OVERRIDE_NORMAL, int idk = NULL)
	{
		typedef void (__thiscall* Fn)(void*, IMaterial*, OverrideType_t, int);
		return call_vfunc<Fn>(this, 1)(this, material, type, idk);
	}
};


enum RenderableTranslucencyType_t
{
	RENDERABLE_IS_OPAQUE = 0,
	RENDERABLE_IS_TRANSLUCENT,
	RENDERABLE_IS_TWO_PASS,	// has both translucent and opaque sub-partsa
};
class CUtlBuffer;
class CPhysCollide;
class CModelInfo
{
public:

	virtual							~CModelInfo(void) { }
	virtual const model_t			*GetModel(int modelindex) const = 0;
	// Returns index of model by name
	virtual int						GetModelIndex(const char *name) const = 0;
	// Returns name of model
	virtual const char				*GetModelName(const model_t *model) const = 0;
	virtual void					UNUSED() {};
	virtual vcollide_t				*GetVCollide(const model_t *model) const = 0;
	virtual vcollide_t				*GetVCollide(int modelindex) const = 0;
	virtual void					GetModelBounds(const model_t *model, Vector& mins, Vector& maxs) const = 0;
	virtual	void					GetModelRenderBounds(const model_t *model, Vector& mins, Vector& maxs) const = 0;
	virtual int						GetModelFrameCount(const model_t *model) const = 0;
	virtual int						GetModelType(const model_t *model) const = 0;
	virtual void					*GetModelExtraData(const model_t *model) = 0;
	virtual bool					ModelHasMaterialProxy(const model_t *model) const = 0;
	virtual bool					IsTranslucent(model_t const* model) const = 0;
	virtual bool					IsTranslucentTwoPass(const model_t *model) const = 0;
	virtual void					Unused0() {};
	virtual RenderableTranslucencyType_t ComputeTranslucencyType(const model_t *model, int nSkin, int nBody) = 0;
	virtual int						GetModelMaterialCount(const model_t* model) const = 0;
	virtual void					GetModelmaterials(const model_t *model, int count, IMaterial** ppMaterials) = 0;
	virtual bool					IsModelVertexLit(const model_t *model) const = 0;
	virtual const char				*GetModelKeyValueText(const model_t *model) = 0;
	virtual bool					GetModelKeyValue(const model_t *model, CUtlBuffer &buf) = 0; // supports keyvalue blocks in submodels
	virtual float					GetModelRadius(const model_t *model) = 0;

	virtual const studiohdr_t		*FindModel(const studiohdr_t *pStudioHdr, void **cache, const char *modelname) const = 0;
	virtual const studiohdr_t		*FindModel(void *cache) const = 0;
	virtual	virtualmodel_t			*GetVirtualModel(const studiohdr_t *pStudioHdr) const = 0;
	virtual byte					*GetAnimBlock(const studiohdr_t *pStudioHdr, int iBlock) const = 0;
	virtual bool					HasAnimBlockBeenPreloaded(studiohdr_t const*, int) const = 0;

	// Available on client only!!!
	virtual void					GetModelMaterialColorAndLighting(const model_t *model, Vector const& origin,
		QAngle const& angles, trace_t* pTrace,
		Vector& lighting, Vector& matColor) = 0;
	virtual void					GetIlluminationPoint(const model_t *model, IClientRenderable *pRenderable, Vector const& origin,
		QAngle const& angles, Vector* pLightingCenter) = 0;

	virtual int						GetModelContents(int modelIndex) const = 0;
	virtual void					UNUSED11() {};
	virtual studiohdr_t				*GetStudioModel(const model_t *mod) = 0;
	virtual int						GetModelSpriteWidth(const model_t *model) const = 0;
	virtual int						GetModelSpriteHeight(const model_t *model) const = 0;

	// Sets/gets a map-specified fade range (client only)
	virtual void					SetLevelScreenFadeRange(float flMinSize, float flMaxSize) = 0;
	virtual void					GetLevelScreenFadeRange(float *pMinArea, float *pMaxArea) const = 0;

	// Sets/gets a map-specified per-view fade range (client only)
	virtual void					SetViewScreenFadeRange(float flMinSize, float flMaxSize) = 0;

	// Computes fade alpha based on distance fade + screen fade (client only)
	virtual unsigned char			ComputeLevelScreenFade(const Vector &vecAbsOrigin, float flRadius, float flFadeScale) const = 0;
	virtual unsigned char			ComputeViewScreenFade(const Vector &vecAbsOrigin, float flRadius, float flFadeScale) const = 0;

	// both client and server
	virtual int						GetAutoplayList(const studiohdr_t *pStudioHdr, unsigned short **pAutoplayList) const = 0;

	// Gets a virtual terrain collision model (creates if necessary)
	// NOTE: This may return NULL if the terrain model cannot be virtualized
	virtual CPhysCollide			*GetCollideForVirtualTerrain(int index) = 0;
	virtual bool					IsUsingFBTexture(const model_t *model, int nSkin, int nBody, void /*IClientRenderable*/ *pClientRenderable) const = 0;
	virtual const model_t			*FindOrLoadModel(const char *name) const = 0;
	virtual MDLHandle_t				GetCacheHandle(const model_t *model) const = 0;
	// Returns planes of non-nodraw brush model surfaces
	virtual int						GetBrushModelPlaneCount(const model_t *model) const = 0;
	virtual void					GetBrushModelPlane(const model_t *model, int nIndex, cplane_t &plane, Vector *pOrigin) const = 0;
	virtual int						GetSurfacepropsForVirtualTerrain(int index) = 0;
	virtual bool					UsesEnvCubemap(const model_t *model) const = 0;
	virtual bool					UsesStaticLighting(const model_t *model) const = 0;

	studiohdr_t* GetStudiomodel(const model_t* mod)
	{
		typedef studiohdr_t*(__stdcall* oGetStudiomodel)(const model_t*);
		return call_vfunc<oGetStudiomodel>(this, 32)(mod);
	}

	void GetModelMaterials(const model_t *model, int count, IMaterial** ppMaterial)
	{
		typedef void *(__thiscall* oGetModelMaterials)(PVOID, const model_t*, int, IMaterial**);
		call_vfunc<oGetModelMaterials>(this, 19)(this, model, count, ppMaterial);
	}
};

class CVRenderView
{
public:
	void SetBlend(float alpha)
	{
		typedef void (__thiscall* oDrawModelExecute)(PVOID, float);
		return call_vfunc<oDrawModelExecute>(this, 4)(this, alpha);
	}

	void SetColorModulation(float const* colors)
	{
		typedef void (__thiscall* oDrawModelExecute)(PVOID, float const*);
		return call_vfunc<oDrawModelExecute>(this, 6)(this, colors);
	}
};
struct vrect_t;
enum DrawFlags_t
{
	DF_RENDER_REFRACTION = 0x1,
	DF_RENDER_REFLECTION = 0x2,
	DF_CLIP_Z = 0x4,
	DF_CLIP_BELOW = 0x8,
	DF_RENDER_UNDERWATER = 0x10,
	DF_RENDER_ABOVEWATER = 0x20,
	DF_RENDER_WATER = 0x40,
	DF_UNUSED1 = 0x100,
	DF_WATERHEIGHT = 0x200,
	DF_UNUSED2 = 0x400,
	DF_DRAWSKYBOX = 0x800,
	DF_FUDGE_UP = 0x1000,
	DF_DRAW_ENTITITES = 0x2000,
	DF_UNUSED3 = 0x4000,
	DF_UNUSED4 = 0x8000,
	DF_UNUSED5 = 0x10000,
	DF_SAVEGAMESCREENSHOT = 0x20000,
	DF_CLIP_SKYBOX = 0x40000,
	DF_SHADOW_DEPTH_MAP = 0x100000    // Currently rendering a shadow depth map
};
class IViewRender
{
public:
	virtual void                Init(void) = 0;
	virtual void                LevelInit(void) = 0;
	virtual void                LevelShutdown(void) = 0;
	virtual void                Shutdown(void) = 0;
	virtual void                OnRenderStart() = 0;
	virtual    void                Render(vrect_t *rect) = 0;
	virtual void                RenderView(const CViewSetup &view, int nClearFlags, int whatToDraw) = 0;
	virtual int                 GetDrawFlags() = 0;
	virtual void                StartPitchDrift(void) = 0;
	virtual void                StopPitchDrift(void) = 0;
	virtual void*               GetFrustum() = 0;
	virtual bool                ShouldDrawBrushModels(void) = 0;
	virtual const CViewSetup*   GetPlayerViewSetup(void) const = 0;
	virtual const CViewSetup*   GetViewSetup(void) const = 0;
	virtual void                DisableVis(void) = 0;
	virtual int                 BuildWorldListsNumber() const = 0;
	virtual void                SetCheapWaterStartDistance(float flCheapWaterStartDistance) = 0;
	virtual void                SetCheapWaterEndDistance(float flCheapWaterEndDistance) = 0;
	virtual void                GetWaterLODParams(float &flCheapWaterStartDistance, float &flCheapWaterEndDistance) = 0;
	virtual void                DriftPitch(void) = 0;
	virtual void                SetScreenOverlayMaterial(IMaterial *pMaterial) = 0;
	virtual IMaterial*          GetScreenOverlayMaterial() = 0;
	virtual void                WriteSaveGameScreenshot(const char *pFilename) = 0;
	virtual void                WriteSaveGameScreenshotOfSize(const char *pFilename, int width, int height) = 0;
	virtual void                QueueOverlayRenderView(const CViewSetup &view, int nClearFlags, int whatToDraw) = 0;
	virtual float               GetZNear() = 0;
	virtual float               GetZFar() = 0;
	virtual void                GetScreenFadeDistances(float *min, float *max) = 0;
	virtual C_BaseEntity*       GetCurrentlyDrawingEntity() = 0;
	virtual void                SetCurrentlyDrawingEntity(C_BaseEntity *pEnt) = 0;
};