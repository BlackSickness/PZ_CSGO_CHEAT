#pragma once
#include "VMT.h"
#include "HookIncludes.h"
#include "Codex.h"



namespace hooks
{
	extern bool initialize();
	extern void cleanup();

	typedef void(__stdcall *fsn_t)(ClientFrameStage_t);
	using create_move_t = void(__thiscall *)(IBaseClientDLL *, int, float, bool);
	typedef bool(__thiscall* in_prediction_t)(PVOID);
	typedef void(__thiscall* override_view_t)(void* _this, CViewSetup* setup);
	typedef float(__thiscall* get_fov_t)(void*);
	typedef bool(__thiscall* do_post_screen_space_effects_t)(void*, CViewSetup*);
	typedef void(__thiscall* paint_traverse_t)(PVOID, unsigned int, bool, bool);
	typedef void(__thiscall* dme_t)(void*, void*, void*, const ModelRenderInfo_t&, matrix3x4_t*);
	typedef void(__thiscall* SceneEnd_t)(void *pEcx);
	typedef void(__thiscall* play_sound_t)(void*, const char*);
	typedef bool(__thiscall* svc_get_bool_t)(PVOID);
	typedef void(__thiscall* LockCursor_t)(void*);

	extern fsn_t fsn;
	extern create_move_t create_move;
	extern in_prediction_t original_prediction;
	extern override_view_t original_override_view;
	extern get_fov_t original_get_fov;
	extern do_post_screen_space_effects_t original_do_post_screen_space_effects;
	extern paint_traverse_t original_paint_traverse;
	extern dme_t original_dme;
	extern SceneEnd_t original_SceneEnd;
	extern play_sound_t original_play_sound;
	extern svc_get_bool_t original_svc_get_bool;
	extern LockCursor_t oLockCursor;

	extern VMT::VMTHookManager panel;
	extern VMT::VMTHookManager client;
	extern VMT::VMTHookManager clientmode;
	extern VMT::VMTHookManager modelrender;
	extern VMT::VMTHookManager prediction;
	extern VMT::VMTHookManager surface;
	extern VMT::VMTHookManager eventmanager;
	extern VMT::VMTHookManager d3d;
	extern VMT::VMTHookManager viewrender;
	extern VMT::VMTHookManager engine;
	extern VMT::VMTHookManager sv_cheats;
};
