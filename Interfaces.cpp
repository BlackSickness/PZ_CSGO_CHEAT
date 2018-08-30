#include "Interfaces.h"
#include "SkinChanger.h"


IVDebugOverlay* Interfaces::g_DebugOverlay = nullptr;
IPanel* Interfaces::g_Panel = nullptr;
IBaseClientDLL* Interfaces::g_CHLClient = nullptr;
IVEngineClient* Interfaces::g_Engine = nullptr;
C_BaseEntityList* Interfaces::g_EntityList = nullptr;
ISurface* Interfaces::g_Surface = nullptr;
IClientMode* Interfaces::g_ClientMode = nullptr;
CGlobalVarsBase* Interfaces::g_Globals = nullptr;
IPrediction *Interfaces::g_Prediction = nullptr;
CMaterialSystem* Interfaces::g_MaterialSystem = nullptr;
CVRenderView* Interfaces::g_RenderView = nullptr;
IVModelRender* Interfaces::g_ModelRender = nullptr;
CModelInfo* Interfaces::g_ModelInfo = nullptr;
IEngineTrace* Interfaces::g_EngineTrace = nullptr;
IPhysicsSurfaceProps* Interfaces::g_PhysProps = nullptr;
ICVar* Interfaces::g_CVar = nullptr;
IVEffects* Interfaces::g_Dlight = nullptr;
CInput* Interfaces::g_Input = nullptr;
IInputSystem*		Interfaces::g_InputSystem = nullptr;
IGameEventManager2* Interfaces::g_EventManager = nullptr;
CGlowObjectManager* Interfaces::g_GlowObjManager = nullptr;
ILocalize*  Interfaces::g_pILocalize = nullptr;
IMemAlloc* Interfaces::g_MemAlloc = nullptr;
IViewRenderBeams* Interfaces::g_ViewRenderBeams = nullptr;
CNetworkStringTableContainer* Interfaces::g_ClientStringTableContainer = nullptr;
CGui*		g_pGui = nullptr;



IVEngineClient* Interfaces::Engine()
{
	if (!g_Engine)
	{
		CreateInterfaceFn pfnFactory = CaptureFactory(ENGINE_DLL);
		g_Engine = CaptureInterface<IVEngineClient>(pfnFactory, VENGINE_CLIENT_INTERFACE_VERSION);
#if ENABLE_DEBUG_FILE == 1
		CSX::Log::Add("::g_Engine = %X", g_Engine);
#endif
	}

	return g_Engine;
}

IBaseClientDLL* Interfaces::Client()
{
	if (!g_CHLClient)
	{
		CreateInterfaceFn pfnFactory = CaptureFactory(CLIENT_DLL);
		g_CHLClient = CaptureInterface<IBaseClientDLL>(pfnFactory, CLIENT_DLL_INTERFACE_VERSION);

		while (!((DWORD)**(IClientMode***)((*(uint32_t**)Client())[10] + 0x5)) || !((DWORD)**(CGlobalVarsBase***)((*(uint32_t**)Client())[0] + 0x1B)))
		{
			Sleep(1000);
		}

#if ENABLE_DEBUG_FILE == 1
		CSX::Log::Add("::g_CHLClient = %X", g_CHLClient);
#endif
	}

	return g_CHLClient;
}

C_BaseEntityList* Interfaces::EntityList()
{
	if (!g_EntityList)
	{
		CreateInterfaceFn pfnFactory = CaptureFactory(CLIENT_DLL);
		g_EntityList = CaptureInterface<C_BaseEntityList>(pfnFactory, VCLIENTENTITYLIST_INTERFACE_VERSION);
#if ENABLE_DEBUG_FILE == 1
		CSX::Log::Add("::g_EntityList = %X", g_EntityList);
#endif
	}

	return g_EntityList;
}

CGlobalVarsBase* Interfaces::Globals()
{
	if (!g_Globals)
	{
		auto pClientVFTable = *(uint32_t**)Client();
		g_Globals = **(CGlobalVarsBase***)(pClientVFTable[0] + 0x1B);
#if ENABLE_DEBUG_FILE == 1
		CSX::Log::Add("::g_Globals = %X", g_Globals);
#endif
	}

	return g_Globals;
}

CInput* Interfaces::Input()
{
	if (!g_Input)
	{
		g_Input = *(CInput**)(CSX::Memory::NewPatternScan(GetModuleHandleW(L"client_panorama.dll"), "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10") + 1);
#if ENABLE_DEBUG_FILE == 1
		CSX::Log::Add("::g_Input = %X", g_Input);
#endif
	}

	return g_Input;
}

IInputSystem* Interfaces::InputSystem()
{
	if (!g_InputSystem)
	{
		CreateInterfaceFn pfnFactory = CaptureFactory(INPUTSYSTEM_DLL);
		g_InputSystem = CaptureInterface<IInputSystem>(pfnFactory, INPUTSYSTEM_INTERFACE_VERSION);
#if ENABLE_DEBUG_FILE == 1
		CSX::Log::Add("::g_InputSystem = %X", g_InputSystem);
#endif
	}

	return g_InputSystem;
}


IEngineTrace* Interfaces::EngineTrace()
{
	if (!g_EngineTrace)
	{
		CreateInterfaceFn pfnFactory = CaptureFactory(ENGINE_DLL);
		g_EngineTrace = CaptureInterface<IEngineTrace>(pfnFactory, INTERFACEVERSION_ENGINETRACE_CLIENT);
#if ENABLE_DEBUG_FILE == 1
		CSX::Log::Add("::g_EngineTrace = %X", g_EngineTrace);
#endif
	}

	return g_EngineTrace;
}

IClientMode* Interfaces::ClientMode()
{
	if (!g_ClientMode)
	{
		auto pClientVFTable = *(uint32_t**)Client();
		g_ClientMode = **(IClientMode***)(pClientVFTable[10] + 0x5);

#if ENABLE_DEBUG_FILE == 1
		CSX::Log::Add("::g_ClientMode = %X", g_ClientMode);
#endif
	}

	return g_ClientMode;
}

CModelInfo* Interfaces::ModelInfo()
{
	if (!g_ModelInfo)
	{
		CreateInterfaceFn pfnFactory = CaptureFactory(ENGINE_DLL);
		g_ModelInfo = CaptureInterface<CModelInfo>(pfnFactory, VMODELINFO_CLIENT_INTERACE_VERSION);
#if ENABLE_DEBUG_FILE == 1
		CSX::Log::Add("::g_ModelInfo = %X", g_ModelInfo);
#endif
	}

	return g_ModelInfo;
}

IVModelRender* Interfaces::ModelRender()
{
	if (!g_ModelRender)
	{
		CreateInterfaceFn pfnFactory = CaptureFactory(ENGINE_DLL);
		g_ModelRender = CaptureInterface<IVModelRender>(pfnFactory, VENGINE_HUDMODEL_INTERFACE_VERSION);
#if ENABLE_DEBUG_FILE == 1
		CSX::Log::Add("::g_ModelRender = %X", g_ModelRender);
#endif
	}

	return g_ModelRender;
}

CVRenderView* Interfaces::RenderView()
{
	if (!g_RenderView)
	{
		CreateInterfaceFn pfnFactory = CaptureFactory(ENGINE_DLL);
		g_RenderView = CaptureInterface<CVRenderView>(pfnFactory, VENGINE_RENDERVIEW_INTERFACE_VERSION);
#if ENABLE_DEBUG_FILE == 1
		CSX::Log::Add("::g_RenderView = %X", g_RenderView);
#endif
	}

	return g_RenderView;
}

CMaterialSystem* Interfaces::MaterialSystem()
{
	if (!g_MaterialSystem)
	{
		CreateInterfaceFn pfnFactory = CaptureFactory(MATERIAL_DLL);
		g_MaterialSystem = CaptureInterface<CMaterialSystem>(pfnFactory, MATERIAL_SYSTEM_INTERFACE_VERSION);
#if ENABLE_DEBUG_FILE == 1
		CSX::Log::Add("::g_MaterialSystem = %X", g_MaterialSystem);
#endif
	}

	return g_MaterialSystem;
}

ISurface* Interfaces::Surface()
{
	if (!g_Surface)
	{
		CreateInterfaceFn pfnFactory = CaptureFactory(VGUIMT_DLL);
		g_Surface = CaptureInterface<ISurface>(pfnFactory, VGUI_SURFACE_INTERFACE_VERSION);
#if ENABLE_DEBUG_FILE == 1
		CSX::Log::Add("::g_Surface = %X", g_Surface);
#endif
	}

	return g_Surface;
}

IGameEventManager2*	Interfaces::EventManager()
{
	if (!g_EventManager)
	{
		CreateInterfaceFn pfnFactory = CaptureFactory(ENGINE_DLL);
		g_EventManager = CaptureInterface<IGameEventManager2>(pfnFactory, GAMEEVENTMANAGER_INTERFACE_VERSION);
#if ENABLE_DEBUG_FILE == 1
		CSX::Log::Add("::g_EventManager = %X", g_EventManager);
#endif
	}

	return g_EventManager;
}

ICVar* Interfaces::CVar()
{
	if (!g_CVar)
	{
		CreateInterfaceFn pfnFactory = CaptureFactory(VSTDLIB_DLL);
		g_CVar = CaptureInterface<ICVar>(pfnFactory, VENGINEVCAR_INTERFACE_VERSION);

	}

	return g_CVar;
}

IVEffects* Interfaces::Dlight()
{
	if (!g_Dlight)
	{
		CreateInterfaceFn pfnFactory = CaptureFactory(ENGINE_DLL);
		g_Dlight = CaptureInterface<IVEffects>(pfnFactory, VENGINEVEFFECTS_INTERFACE_VERSION);
	}
	return g_Dlight;
}

IPhysicsSurfaceProps* Interfaces::PhysProps()
{
	if (!g_PhysProps)
	{
		CreateInterfaceFn pfnFactory = CaptureFactory(VPHYSICS_DLL);
		g_PhysProps = CaptureInterface<IPhysicsSurfaceProps>(pfnFactory, VPhysics_Surface_Props001);
#if ENABLE_DEBUG_FILE == 1
		CSX::Log::Add("::g_PhysProps = %X", g_PhysProps);
#endif
	}

	return g_PhysProps;
}

IPrediction* Interfaces::Prediction()
{
	if (!g_Prediction)
	{
		CreateInterfaceFn pfnFactory = CaptureFactory(CLIENT_DLL);
		g_Prediction = CaptureInterface<IPrediction>(pfnFactory, VClientPrediction001);
#if ENABLE_DEBUG_FILE == 1
		CSX::Log::Add("::g_Prediction = %X", g_Prediction);
#endif
	}

	return g_Prediction;
}

IVDebugOverlay* Interfaces::DebugOverlay()
{
	if (!g_DebugOverlay)
	{
		CreateInterfaceFn pfnFactory = CaptureFactory(ENGINE_DLL);
		g_DebugOverlay = CaptureInterface<IVDebugOverlay>(pfnFactory, VDEBUGOVERLAY004);
#if ENABLE_DEBUG_FILE == 1
		CSX::Log::Add("::g_DebugOverlay = %X", g_DebugOverlay);
#endif
	}

	return g_DebugOverlay;
}

IPanel* Interfaces::Panel()
{
	if (!g_Panel)
	{
		CreateInterfaceFn pfnFactory = CaptureFactory(VGUI2_DLL);
		g_Panel = CaptureInterface<IPanel>(pfnFactory, VGUI_PANEL009);
#if ENABLE_DEBUG_FILE == 1
		CSX::Log::Add("::g_Panel = %X", g_Panel);
#endif
	}

	return g_Panel;
}

ILocalize* Interfaces::GetLocalize()
{
	if (!g_pILocalize)
	{
		CreateInterfaceFn pfnFactory = CaptureFactory("localize.dll");
		g_pILocalize = CaptureInterface<ILocalize>(pfnFactory, "Localize_001");

	}

	return g_pILocalize;
}



bool Interfaces::InitialiseInterfaces()
{
	if (!CSX::Utils::IsModuleLoad(CLIENT_DLL, 45000))
		return false;

	if (!CSX::Utils::IsModuleLoad(ENGINE_DLL))
		return false;

	if (!CSX::Utils::IsModuleLoad(VGUI2_DLL))
		return false;

	if (!CSX::Utils::IsModuleLoad(VGUIMAT_DLL))
		return false;

	if (!CSX::Utils::IsModuleLoad(VSTDLIB_DLL))
		return false;

	if (!CSX::Utils::IsModuleLoad(MATERIAL_DLL))
		return false;



	if (!Interfaces::Engine())
	{
		return false;
	}

	if (!Interfaces::Client())
	{
		return false;
	}

	if (!Interfaces::EntityList())
	{
		return false;
	}

	if (!Interfaces::Globals())
	{
		return false;
	}


	if (!Interfaces::PhysProps())
	{

		return false;

	}


	if (!Interfaces::EngineTrace())
	{
		return false;
	}

	if (!Interfaces::ClientMode())
	{
		return false;
	}

	if (!Interfaces::ModelInfo())
	{
		return false;
	}


	if (!Interfaces::ModelRender())
	{
		return false;
	}

	if (!Interfaces::RenderView())
	{
		return false;
	}

	if (!Interfaces::MaterialSystem())
	{
		return false;
	}

	if (!Interfaces::Surface())
	{
		return false;
	}

	if (!Interfaces::EventManager())
	{
		return false;
	}

	if (!Interfaces::CVar())
	{
		return false;
	}


	if (!Interfaces::Prediction())
	{
		return false;
	}

	/*
	if (!Interfaces::Input())
	{
		return false;
	}
	
	if (Interfaces::InputSystem())
	{
		return false;
	}
	*/
	if (!Interfaces::Dlight())
	{
		return false;
	}

	if (!Interfaces::Panel())
	{
		return false;
	}

	if (!Interfaces::DebugOverlay())
	{
		return false;
	}

	auto EnginePointer = get_module_factory(GetModuleHandleW(L"engine.dll"));


	Interfaces::g_GlowObjManager = *(CGlowObjectManager**)(CSX::Memory::NewPatternScan(GetModuleHandleW(L"client_panorama.dll"), "0F 11 05 ? ? ? ? 83 C8 01 C7 05 ? ? ? ? 00 00 00 00") + 3);
	Interfaces::g_MemAlloc = *(IMemAlloc**)(GetProcAddress(GetModuleHandleW(L"tier0.dll"), "g_pMemAlloc"));
	Interfaces::g_ViewRenderBeams = *reinterpret_cast< IViewRenderBeams** > (CSX::Memory::FindPatternV2(("client_panorama.dll"), ("A1 ? ? ? ? 56 8B F1 B9 ? ? ? ? FF 50 08")) + 0x1);
	Interfaces::g_ClientStringTableContainer = (CNetworkStringTableContainer*)EnginePointer("VEngineClientStringTable001", NULL);

	InitializeKits();

	return true;
}