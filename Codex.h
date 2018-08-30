#pragma once
#pragma warning(disable:4996)

#include <Windows.h>
#include <iostream>
#include <memory>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <TlHelp32.h>
#include <WinInet.h>

using namespace std;

#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib, "wininet.lib")
#include "CSX.h"

#define VClientPrediction001 "VClientPrediction001"
#define VPhysics_Surface_Props001 "VPhysicsSurfaceProps001"
#define CREATEINTERFACE_PROCNAME "CreateInterface"
#define VENGINE_CLIENT_INTERFACE_VERSION "VEngineClient014"
#define CLIENT_DLL_INTERFACE_VERSION "VClient018"
#define VCLIENTENTITYLIST_INTERFACE_VERSION	"VClientEntityList003"
#define INTERFACEVERSION_ENGINETRACE_CLIENT	"EngineTraceClient004"
#define VMODELINFO_CLIENT_INTERACE_VERSION "VModelInfoClient004"
#define IENGINESOUND_CLIENT_INTERFACE_VERSION "IEngineSoundClient003"
#define VENGINE_HUDMODEL_INTERFACE_VERSION "VEngineModel016"
#define VENGINE_RENDERVIEW_INTERFACE_VERSION "VEngineRenderView014"
#define MATERIAL_SYSTEM_INTERFACE_VERSION "VMaterialSystem080"
#define VGUI_SURFACE_INTERFACE_VERSION "VGUI_Surface031"
#define GAMEEVENTMANAGER_INTERFACE_VERSION	"GAMEEVENTSMANAGER002"
#define VENGINEVCAR_INTERFACE_VERSION "VEngineCvar007"
#define INPUTSYSTEM_INTERFACE_VERSION "InputSystemVersion001"
#define VENGINEVEFFECTS_INTERFACE_VERSION "VEngineEffects001"
#define VENGINECLIENTSTRINGTABLE001 "VEngineClientStringTable001"
#define VDEBUGOVERLAY004 "VDebugOverlay004"
#define VGUI_PANEL009 "VGUI_Panel009"
#define VPHYSICS_DLL "vphysics.dll"
#define ENGINE_DLL "engine.dll"
#define CLIENT_DLL "client_panorama.dll"
#define MATERIAL_DLL "materialsystem.dll"
#define VGUIMT_DLL "vguimatsurface.dll"
#define VSTDLIB_DLL	"vstdlib.dll"
#define INPUTSYSTEM_DLL	"inputsystem.dll"
#define CACHEPOINTER "datacache.dll"
#define STDPOINTER "vstdlib.dll"
#define GLOBAL_VARS_PATTERN "\xA1\x00\x00\x00\x00\x8B\x4D\xFC\x8B\x55\x08"
#define GLOBAL_VARS_MASK "x????xxxxxx"

#define CLIENT_MODE_PATTERN "\xB9\x00\x00\x00\x00\x83\x38\x02\x75\x0D\xFF\xB0\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x5D\xC3"
#define CLIENT_MODE_MASK "x????xxxxxxx????x????xx"

#define VGUI2_DLL "vgui2.dll"
#define VGUIMAT_DLL	"vguimatsurface.dll"
#define D3D9_DLL "d3d9.dll"
#define SHADERPIDX9_DLL "shaderapidx9.dll"
#define GAMEOVERLAYRENDERER_DLL "GameOverlayRenderer.dll"
#define D3D9_MASK "x????xxxxxx"
#define GMOR_MASK "x????x????xxxx????xxxxxxx"
#define FORCE_FULL_UPDATE_MASK "x????xxxxxxxxxxxxxxx"

#define D3D9_PATTERN "\xA1\x00\x00\x00\x00\x50\x8B\x08\xFF\x51\x0C"
#define FORCE_FULL_UPDATE_PATTERN "\xA1\x00\x00\x00\x00\xB9\x00\x00\x00\x00\x56\xFF\x50\x14\x8B\x34\x85"
#define GMOR_PATTERN "\xA3\x00\x00\x00\x00\x68\x00\x00\x00\x00\xFF\x76\x54\xE8\x00\x00\x00\x00\x83\xC4\x08\x84\xC0\x75\x17"
#define SMOK_PATTERN "\x55\x8B\xEC\x83\xEC\x08\x8B\x15\x00\x00\x00\x00\x0F\x57\xC0"

#define VGUI2_DLL "vgui2.dll"
#define VGUIMAT_DLL	"vguimatsurface.dll"
#define D3D9_DLL "d3d9.dll"
#define SHADERPIDX9_DLL "shaderapidx9.dll"
#define GAMEOVERLAYRENDERER_DLL "GameOverlayRenderer.dll"
#define D3D9_MASK "x????xxxxxx"
#define GMOR_MASK "x????x????xxxx????xxxxxxx"
#define FORCE_FULL_UPDATE_MASK "x????xxxxxxxxxxxxxxx"

//[enc_string_disable /]

#define D3D9_PATTERN "\xA1\x00\x00\x00\x00\x50\x8B\x08\xFF\x51\x0C"
#define FORCE_FULL_UPDATE_PATTERN "\xA1\x00\x00\x00\x00\xB9\x00\x00\x00\x00\x56\xFF\x50\x14\x8B\x34\x85"
#define GMOR_PATTERN "\xA3\x00\x00\x00\x00\x68\x00\x00\x00\x00\xFF\x76\x54\xE8\x00\x00\x00\x00\x83\xC4\x08\x84\xC0\x75\x17"
#define SMOK_PATTERN "\x55\x8B\xEC\x83\xEC\x08\x8B\x15\x00\x00\x00\x00\x0F\x57\xC0"