#include "HookIncludes.h"
#include "HooksList.h"
#include "Listener.h"
#include "Interface.h"

namespace hooks
{
	VMT::VMTHookManager panel;
	VMT::VMTHookManager client;
	VMT::VMTHookManager clientmode;
	VMT::VMTHookManager modelrender;
	VMT::VMTHookManager prediction;
	VMT::VMTHookManager surface;
	VMT::VMTHookManager eventmanager;
	VMT::VMTHookManager d3d;
	VMT::VMTHookManager viewrender;
	VMT::VMTHookManager engine;
	VMT::VMTHookManager sv_cheats;
	IDirect3DDevice9* g_pDevice = nullptr;


	fsn_t fsn;
	create_move_t create_move;
	in_prediction_t original_prediction;
	override_view_t original_override_view;
	get_fov_t original_get_fov;
    do_post_screen_space_effects_t original_do_post_screen_space_effects;
	paint_traverse_t original_paint_traverse;
	dme_t original_dme;
    SceneEnd_t original_SceneEnd;
    play_sound_t original_play_sound;
	svc_get_bool_t original_svc_get_bool;
	LockCursor_t oLockCursor;

	void theme()
	{
		switch (g_Options.Menu.Theme)
		{
		case 0:
			g_pGui->purple();
		break;
		case 1:
			g_pGui->DefaultSheme1();
			break;
		case 2:
			g_pGui->RedSheme();
			break;
		case 3:
			g_pGui->darkblue();
			break;
		case 4:
			g_pGui->MidNightSheme();
			break;
		case 5:
			g_pGui->NightSheme();
			break;
		case 6:
			g_pGui->DunnoSheme();
			break;
		case 7:
			g_pGui->BlueSheme();
			break;
		case 8:
			g_pGui->MidNight2();
			break;
		case 9:
			g_pGui->BlackSheme2();
			break;
		case 10:
			g_pGui->green();
			break;
		case 11:
			g_pGui->pink();
			break;
		case 12:
			g_pGui->blue();
			break;
		case 13:
			g_pGui->yellow();
			break;
		case 14:
			g_pGui->BlackSheme();
			break;
		default:
			break;
		}
	}
	void	__stdcall hkLockCursor()
	{
		if (bIsGuiVisible)
			Interfaces::Surface()->UnlockCursor();
		else oLockCursor(Interfaces::Surface());
	}
	bool InitializeDX(IDirect3DDevice9* pDevice)
	{
		g_pGui = new CGui();

		g_pGui->GUI_Init(pDevice);

		CreateDirectoryW(L"C:\\DARKHOOK", NULL);
		GuiFile = "C:/DARKHOOK/gui.ini";
		IniFile = "C:/DARKHOOK/settings.ini";
		LoadSettings(IniFile);
		RefreshConfigs();

		return true;
	}
	typedef HRESULT(WINAPI* Present_t)(IDirect3DDevice9* pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion);
	Present_t Present_o;

	typedef HRESULT(WINAPI* Reset_t)(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
	Reset_t Reset_o;

	HRESULT WINAPI Hook_Present(IDirect3DDevice9* pDevice, CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
	{

		if (g_Render && !Interfaces::Engine()->IsTakingScreenshot() && Interfaces::Engine()->IsActiveApp())
		{
			DWORD colorwrite, srgbwrite;
			pDevice->GetRenderState(D3DRS_COLORWRITEENABLE, &colorwrite);
			pDevice->GetRenderState(D3DRS_SRGBWRITEENABLE, &srgbwrite);
			pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
			pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, false);

			if (g_pGui)
				g_pGui->GUI_Draw_Elements();


			g_pGui->MenuColor();

			theme();

			pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, colorwrite);
			pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, srgbwrite);
		}
		return Present_o(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
	}

	HRESULT WINAPI Hook_Reset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
	{

		if (g_pGui)
			ImGui_ImplDX9_InvalidateDeviceObjects();

		HRESULT result = Reset_o(pDevice, pPresentationParameters);
		if (g_Render)
			g_Render->SetupFonts();

		if (g_pGui)
			ImGui_ImplDX9_CreateDeviceObjects();

		return result;
	}

	bool initialize()
	{

		if (!CSX::Utils::IsModuleLoad(D3D9_DLL))
			return false;

		if (!CSX::Utils::IsModuleLoad(SHADERPIDX9_DLL))
			return false;

		if (!CSX::Utils::IsModuleLoad(GAMEOVERLAYRENDERER_DLL))
			return false;

		DWORD d3d9TablePtrPtr = CSX::Memory::FindPattern(SHADERPIDX9_DLL, D3D9_PATTERN, D3D9_MASK, 1);
		DWORD_PTR** dwPresent_o = (DWORD_PTR**)CSX::Memory::FindPattern(GAMEOVERLAYRENDERER_DLL, GMOR_PATTERN, GMOR_MASK, 1);

		if (d3d9TablePtrPtr && dwPresent_o)
		{
			g_pDevice = (IDirect3DDevice9*)(**(PDWORD*)d3d9TablePtrPtr);

			if (g_pDevice)
			{
				d3d.Init(g_pDevice);
				DWORD_PTR* dwAddress = *dwPresent_o;
				Present_o = (Present_t)(*dwAddress);
				*dwAddress = (DWORD_PTR)(&Hook_Present);

				Reset_o = reinterpret_cast<Reset_t>(d3d.HookFunction<Reset_t>(16, Hook_Reset));

				client.Init(Interfaces::Client());
				fsn = reinterpret_cast<fsn_t>(client.HookFunction<fsn_t>(37, hkFrameStageNotify));

				create_move = reinterpret_cast<create_move_t>(client.HookFunction<create_move_t>(22, hkCreateMove));

				prediction.Init(Interfaces::Prediction());
				original_prediction = reinterpret_cast<in_prediction_t>(prediction.HookFunction<in_prediction_t>(14, hkInPrediction));

				clientmode.Init(Interfaces::ClientMode());
				original_override_view = reinterpret_cast<override_view_t>(clientmode.HookFunction<override_view_t>(18, hkOverrideView));
				original_get_fov = reinterpret_cast<get_fov_t>(clientmode.HookFunction<get_fov_t>(35, hkGetViewModelFOV));
				original_do_post_screen_space_effects = reinterpret_cast<do_post_screen_space_effects_t>(clientmode.HookFunction<do_post_screen_space_effects_t>(44, hkDoPostScreenSpaceEffects));


				panel.Init(Interfaces::Panel());
				original_paint_traverse = reinterpret_cast<paint_traverse_t>(panel.HookFunction<paint_traverse_t>(41, hkPaintTraverse));

				modelrender.Init(Interfaces::ModelRender());
				original_dme = reinterpret_cast<dme_t>(modelrender.HookFunction<dme_t>(21, hkDrawModelExecute));


				viewrender.Init(Interfaces::RenderView());
				original_SceneEnd = reinterpret_cast<SceneEnd_t>(viewrender.HookFunction<SceneEnd_t>(9, Hooked_SceneEnd));

				surface.Init(Interfaces::Surface());
				original_play_sound = reinterpret_cast<play_sound_t>(surface.HookFunction<play_sound_t>(82, hkPlaySound));
				oLockCursor = reinterpret_cast<LockCursor_t>(surface.HookFunction<LockCursor_t>(67, hkLockCursor));

				ConVar* sv_cheats_con = Interfaces::CVar()->FindVar("sv_cheats");
				sv_cheats.Init(sv_cheats_con);
				original_svc_get_bool = reinterpret_cast<svc_get_bool_t>(sv_cheats.HookFunction<svc_get_bool_t>(13, hkSvCheatsGetBool));

				Interfaces::Engine()->ClientCmd_Unrestricted2("toggleconsole");
				Sleep(50);
				Interfaces::Engine()->ClientCmd_Unrestricted2("clear");
				Sleep(50);

				Interfaces::CVar()->ConsoleColorPrintf(Color(255, 0, 0), "PZ-> ");
				Sleep(50);
				Interfaces::Engine()->ClientCmd_Unrestricted2("echo has successfully injected.\n");
				Sleep(50);
				Interfaces::CVar()->ConsoleColorPrintf(Color(255, 0, 0), "PZ-> ");
				Sleep(50);
				Interfaces::Engine()->ClientCmd_Unrestricted2("echo Build Date: " __DATE__ __TIME__);

				initializeskins();
				item_purchase::singleton()->initialize();
				if (InitializeDX(g_pDevice))
					return true;
			}
		}
		return false;
	}
	void cleanup()
	{
		/*
		IDirect3DDevice9Table.UnHook();
		item_purchase::singleton()->remove();
		client.UnHookFunction();
		prediction.unhook_all();
		clientmode.unhook_all();
		panel.unhook_all();
		modelrender.unhook_all();
		surface.unhook_all();
		//EventManager.Unhook();
		viewrender.unhook_all();
		//d3d.Unhook();
		sv_cheats.unhook_all();
		engine.unhook_all();
		renderer->~Renderer();
		*/
	}

}

