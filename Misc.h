

#pragma once

#include "entities.h"
#include "SDK.h"
#include "singleton.hpp"

class misc
	: public singleton<misc>
{
public:
	void OnCreateMove(CInput::CUserCmd *cmd, C_BaseEntity *local, bool& bSendPacket);
	void StandAloneRCS(C_BaseEntity* pLocal, CInput::CUserCmd* pCmd);
	static void AutoStrafe(CInput::CUserCmd *cmd, C_BaseEntity *local, QAngle oldangles);
	void EdgeJump(CInput::CUserCmd* pCmd);
	void blockbot(CInput::CUserCmd * m_pcmd);
	static void FakeWalk(CInput::CUserCmd *pCmd);
	static void moonwalk(CInput::CUserCmd *cmd);
	static void SlowMove(CInput::CUserCmd* pCmd, bool &bSendPacket, C_BaseEntity *local);
	static void Knifebot(CInput::CUserCmd* pCmd);
	//void Slowmo(CUserCmd *pCmd, bool &bSendPacket);

	static Vector RotatePoint(Vector EntityPos, Vector LocalPlayerPos, int posX, int posY, int sizeX, int sizeY, float angle, float zoom, bool* viewCheck)
	{
		float r_1, r_2;
		float x_1, y_1;

		r_1 = -(EntityPos.y - LocalPlayerPos.y);
		r_2 = EntityPos.x - LocalPlayerPos.x;
		float Yaw = angle - 90.0f;

		float yawToRadian = Yaw * (float)(M_PI / 180.0F);
		x_1 = (float)(r_2 * (float)cos((double)(yawToRadian)) - r_1 * sin((double)(yawToRadian))) / 20;
		y_1 = (float)(r_2 * (float)sin((double)(yawToRadian)) + r_1 * cos((double)(yawToRadian))) / 20;

		*viewCheck = y_1 < 0;

		x_1 *= zoom;
		y_1 *= zoom;

		int sizX = sizeX / 2;
		int sizY = sizeY / 2;

		x_1 += sizX;
		y_1 += sizY;

		if (x_1 < 5)
			x_1 = 5;

		if (x_1 > sizeX - 5)
			x_1 = sizeX - 5;

		if (y_1 < 5)
			y_1 = 5;

		if (y_1 > sizeY - 5)
			y_1 = sizeY - 5;


		x_1 += posX;
		y_1 += posY;


		return Vector(x_1, y_1, 0);
	}

};
template<class T, class U>
inline T clamp(T in, U low, U high)
{
	if (in <= low)
		return low;
	else if (in >= high)
		return high;
	else
		return in;
}

inline float bitsToFloat(unsigned long i)
{
	return *reinterpret_cast<float*>(&i);
}

inline static vec_t Normalize_y(vec_t ang)
{
	while (ang < -180.0f)
		ang += 360.0f;
	while (ang > 180.0f)
		ang -= 360.0f;
	return ang;
}

inline float FloatNegate(float f)
{
	return bitsToFloat(FloatBits(f) ^ 0x80000000);
}

inline void setclantag(const char* tag)
{
	static auto ClanTagOffset = CSX::Memory::FindPattern(ENGINE_DLL, "\x53\x56\x57\x8B\xDA\x8B\xF9\xFF\x15", "xxxxxxxxx",0);
	if (ClanTagOffset)
	{
		auto tag_ = std::string(tag);
		if (strlen(tag) > 0) {
			auto newline = tag_.find("\\n");
			auto tab = tag_.find("\\t");
			if (newline != std::string::npos) {
				tag_.replace(newline, newline + 2, "\n");
			}
			if (tab != std::string::npos) {
				tag_.replace(tab, tab + 2, "\t");
			}
		}
		static auto dankesttSetClanTag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>(ClanTagOffset);
		dankesttSetClanTag(tag_.data(), tag_.data());
	}
}
static __declspec(naked) void __cdecl Invoke_NET_SetConVar(void* pfn, const char* cvar, const char* value)
{
	__asm
	{
		push    ebp
		mov     ebp, esp
		and     esp, 0FFFFFFF8h
		sub     esp, 44h
		push    ebx
		push    esi
		push    edi
		mov     edi, cvar
		mov     esi, value
		jmp     pfn
	}
}

inline void DECLSPEC_NOINLINE NET_SetConVar(const char* value, const char* cvar)
{
	static DWORD setaddr = CSX::Memory::FindPattern(ENGINE_DLL, "\x8D\x4C\x24\x1C\xE8\x00\x00\x00\x00\x56", "xxxxx????x",0);
	if (setaddr != 0)
	{
		void* pvSetConVar = (char*)setaddr;
		Invoke_NET_SetConVar(pvSetConVar, cvar, value);
	}
}
inline void change_name(const char* name)
{
	if (Interfaces::Engine()->IsInGame() && Interfaces::Engine()->IsConnected())
		NET_SetConVar(name, "name");
}
inline void RankReveal(CInput::CUserCmd* cmd)
{
	if (cmd->buttons & IN_SCORE && g_Options.Misc.Ranks)
	{
		using ServerRankRevealAll = char(__cdecl*)(int*);
		typedef bool(__cdecl *RankRevealFn)(float*);
		static uint8_t* fnServerRankRevealAll = CSX::Memory::NewPatternScan(GetModuleHandleW(L"client_panorama.dll"), "55 8B EC 8B 0D ? ? ? ? 85 C9 75 ? A1 ? ? ? ? 68 ? ? ? ? 8B 08 8B 01 FF 50 ? 85 C0 74 ? 8B C8 E8 ? ? ? ? 8B C8 EB ? 33 C9 89 0D ? ? ? ? 8B 45 ? FF 70 ? E8 ? ? ? ? B0 ? 5D");
		int v[3] = { 0,0,0 };
		reinterpret_cast<ServerRankRevealAll>(fnServerRankRevealAll)(v);
	}
}
char* const tuxlist[] =
{
	"AimTux owns me and all",
	"Your Windows p2c sucks my AimTux dry",
	"It's free as in FREEDOM!",
	"Tux only let me out so I could play this game, please be nice!",
	"Tux nutted but you keep sucken",
	">tfw no vac on Linux",
};
char* const ezfragslist[] =
{
	"Think you could do better? Not without www.EZFrags.co.uk",
	"If I was cheating, I'd use www.EZFrags.co.uk",
	"I'm not using www.EZFrags.co.uk, you're just bad",
	"Visit www.EZFrags.co.uk for the finest public & private CS:GO cheats",
	"Stop being a noob! Get good with www.EZFrags.co.uk",
	"You just got pwned by www.EZFrags.co.uk, the #1 CS:GO cheat",
};
char* const DHlist[] =
{
	"You just got rekt by Dark Hook",
	"Git Gud! Git Dark Hook!",
	"I'm not cheating, but If I was I would recommend Dark Hook",
	"Dark Hook > all",
	"You think your ayypaste is better than Dark Hook?",
	"Stop being a noob nigger, git Dark Hook"
};