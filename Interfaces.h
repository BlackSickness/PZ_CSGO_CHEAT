#pragma once

#include "CommonIncludes.h"
#include "SDK.h"
#include <iostream>
#include "Codex.h"

class CGui;
class Interfaces
{
public:
	static bool InitialiseInterfaces();
	static IBaseClientDLL* Client();
	static IVEngineClient* Engine();
	static IPanel* Panel();
	static C_BaseEntityList* EntityList();
	static ISurface* Surface();
	static IVDebugOverlay* DebugOverlay();
	static IClientMode* ClientMode();
	static CGlobalVarsBase* Globals();
	static IPrediction *Prediction();
	static CMaterialSystem* MaterialSystem();
	static CVRenderView* RenderView();
	static IVModelRender* ModelRender();
	static CModelInfo* ModelInfo();
	static IEngineTrace* EngineTrace();
	static IPhysicsSurfaceProps* PhysProps();
	static ICVar* CVar();
	static IVEffects* Dlight();
	static CInput* Input();
	static IInputSystem* InputSystem();
	static IGameEventManager2* EventManager();
	static ILocalize*  GetLocalize();
	static CGlowObjectManager* g_GlowObjManager;
	static IViewRenderBeams* g_ViewRenderBeams;
	static CNetworkStringTableContainer* g_ClientStringTableContainer;
	static IMemAlloc* g_MemAlloc;
	static IPrediction *g_Prediction;


private:
	static IBaseClientDLL* g_CHLClient;
	static IVEngineClient* g_Engine;
	static IPanel* g_Panel;
	static C_BaseEntityList* g_EntityList;
	static ISurface* g_Surface;
	static IVDebugOverlay* g_DebugOverlay;
	static IClientMode* g_ClientMode;
	static CGlobalVarsBase* g_Globals;
	static CMaterialSystem* g_MaterialSystem;
	static CVRenderView* g_RenderView;
	static IVModelRender* g_ModelRender;
	static CModelInfo* g_ModelInfo;
	static IEngineTrace* g_EngineTrace;
	static IPhysicsSurfaceProps* g_PhysProps;
	static ICVar* g_CVar;
	static IVEffects* g_Dlight;
	static CInput* g_Input;
	static IInputSystem* g_InputSystem;
	static IGameEventManager2* g_EventManager;
	static ILocalize*   g_pILocalize;

};
extern CGui*		g_pGui;

template<class T>
static T* FindHudElement(const char* name)
{
	static auto pThis = *reinterpret_cast<DWORD**>(CSX::Memory::NewPatternScan(GetModuleHandleW(L"client_panorama.dll"), "B9 ? ? ? ? E8 ? ? ? ? 83 7D F8 ?") + 1);

	static auto find_hud_element = reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(CSX::Memory::NewPatternScan(GetModuleHandleW(L"client_panorama.dll"), "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28"));
	return (T*)find_hud_element(pThis, name);
}
typedef void* (__cdecl* CreateInterface_t)(const char*, int*);
typedef void* (*CreateInterfaceFn)(const char* pName, int* pReturnCode);
inline CreateInterfaceFn CaptureFactory(char* FactoryModule)
{
	CreateInterfaceFn Interface = 0;

	while (!Interface)
	{
		HMODULE hFactoryModule = GetModuleHandleA(FactoryModule);

		if (hFactoryModule)
		{
			Interface = (CreateInterfaceFn)(GetProcAddress(hFactoryModule, "CreateInterface"));
		}

		Sleep(100);
	}

	return Interface;
}

template<typename T>
inline T* CaptureInterface(CreateInterfaceFn Interface, char* InterfaceName)
{
	T* dwPointer = nullptr;

	while (!dwPointer)
	{
		dwPointer = (T*)(Interface(InterfaceName, 0));

		Sleep(100);
	}

	return dwPointer;
}

inline CreateInterfaceFn get_module_factory(HMODULE module)
{
	return reinterpret_cast<CreateInterfaceFn>(GetProcAddress(module, "CreateInterface"));
}