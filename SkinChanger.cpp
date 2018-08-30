#pragma once
#include "SkinChanger.h"
#include "CSX.h"

std::unordered_map<char*, char*> killIcons = {};
#define INVALID_EHANDLE_INDEX 0xFFFFFFFF
HANDLE worldmodel_handle;
CBaseCombatWeapon* worldmodel;
std::deque<Kit_t> k_skins;
std::deque<Kit_t> k_gloves;
std::deque<Kit_t> k_stickers;
class CCStrike15ItemSchema;

unordered_map<int, const char*> g_ViewModelCfg;
unordered_map<const char*, const char*> g_KillIconCfg;




char* pWeaponData[44] =
{
	// ÏÈÑÒÎËÅÒÛ - 0 - 9
	WEAPON_DEAGLE_STR,WEAPON_ELITE_STR,WEAPON_FIVESEVEN_STR,
	WEAPON_GLOCK_STR,WEAPON_AK47_STR,WEAPON_AUG_STR,
	WEAPON_AWP_STR,WEAPON_FAMAS_S_STR,WEAPON_G3SG1_STR,
	WEAPON_GALILAR_STR,
	// ÀÂÒÎÌÀÒÛ - 10 - 30
	WEAPON_M249_STR,WEAPON_M4A1S_STR,WEAPON_M4A41_STR,WEAPON_MAC10_STR,
	WEAPON_P90_STR,WEAPON_MP5SD_STR,WEAPON_UMP45_STR,WEAPON_XM1014_STR,WEAPON_BIZON_STR,
	WEAPON_MAG7_STR,WEAPON_NEGEV_STR,WEAPON_SAWOFF_STR,WEAPON_TEC9_STR,
	WEAPON_HKP2000_STR,WEAPON_MP7_STR,WEAPON_MP9_STR,
	WEAPON_NOVA_STR,WEAPON_P250_STR,WEAPON_SCAR20_STR,WEAPON_SG556_STR,
	WEAPON_SSG08_STR,WEAPON_USP_S_STR,
	// ÑÍÀÉÏÅÐÊÈ  - 31 - 32
	WEAPON_CZ75A_STR,WEAPON_REVOLVER_STR,WEAPON_KM9B_STR,WEAPON_KBAYONET_STR,
	WEAPON_KFLIP_STR,WEAPON_KGUT_STR,WEAPON_KARAMBIT_STR,WEAPON_KTACTICAL_STR,
	WEAPON_KFALCHION_STR,WEAPON_KSBOWIE_STR,WEAPON_KBUTTERFLY_STR,WEAPON_KPUSH_STR
};

char* pStickersData[34] =
{
	// ÏÈÑÒÎËÅÒÛ - 0 - 9
	Sticker_DEAGLE_STR,Sticker_ELITE_STR,Sticker_FIVESEVEN_STR,
	Sticker_GLOCK_STR,Sticker_AK47_STR,Sticker_AUG_STR,
	Sticker_AWP_STR,Sticker_FAMAS_S_STR,Sticker_G3SG1_STR,
	Sticker_GALILAR_STR,
	// ÀÂÒÎÌÀÒÛ - 10 - 30
	Sticker_M249_STR,Sticker_M4A1S_STR,Sticker_M4A41_STR,Sticker_MAC10_STR,
	Sticker_P90_STR,Sticker_MP5SD_STR,Sticker_UMP45_STR,Sticker_XM1014_STR,Sticker_BIZON_STR,
	Sticker_MAG7_STR,Sticker_NEGEV_STR,Sticker_SAWOFF_STR,Sticker_TEC9_STR,
	Sticker_HKP2000_STR,Sticker_MP7_STR,Sticker_MP9_STR,
	Sticker_NOVA_STR,Sticker_P250_STR,Sticker_SCAR20_STR,Sticker_SG556_STR,
	Sticker_SSG08_STR,Sticker_USP_S_STR,
	// ÑÍÀÉÏÅÐÊÈ  - 31 - 32
	Sticker_CZ75A_STR,Sticker_REVOLVER_STR,
};


int pWeaponItemIndexData[44] =
{
	WEAPON_DEAGLE,WEAPON_DUALS,WEAPON_FIVE7,WEAPON_GLOCK,
	WEAPON_AK47 ,WEAPON_AUG ,WEAPON_AWP ,WEAPON_FAMAS,
	WEAPON_G3SG1 ,WEAPON_GALIL,WEAPON_M249,WEAPON_M4A1S,
	WEAPON_M4A1 ,WEAPON_MAC10,WEAPON_P90,WEAPON_MP5SD,WEAPON_UMP45,
	WEAPON_XM1014,WEAPON_BIZON,WEAPON_MAG7 ,
	WEAPON_NEGEV,WEAPON_SAWEDOFF,WEAPON_TEC9 ,WEAPON_P2000,WEAPON_MP7 ,
	WEAPON_MP9 ,WEAPON_NOVA ,WEAPON_P250,WEAPON_SCAR20,
	WEAPON_SG553,WEAPON_SCOUT,WEAPON_USPS,WEAPON_CZ75,WEAPON_REVOLVER,
	WEAPON_KNIFE_M9BAYONET,WEAPON_KNIFE_BAYONET,WEAPON_KNIFE_FLIP,
	WEAPON_KNIFE_GUT,WEAPON_KNIFE_KARAMBIT,WEAPON_KNIFE_HUNTSMAN,
	WEAPON_KNIFE_FALCHION,WEAPON_KNIFE_BOWIE,WEAPON_KNIFE_BUTTERFLY,
	WEAPON_KNIFE_DAGGER,
};

int pStickersItemIndexData[34] =
{
	WEAPON_DEAGLE,WEAPON_DUALS,WEAPON_FIVE7,WEAPON_GLOCK,
	WEAPON_AK47 ,WEAPON_AUG ,WEAPON_AWP ,WEAPON_FAMAS,
	WEAPON_G3SG1 ,WEAPON_GALIL,WEAPON_M249,WEAPON_M4A1S,
	WEAPON_M4A1 ,WEAPON_MAC10,WEAPON_P90,WEAPON_MP5SD,WEAPON_UMP45,
	WEAPON_XM1014,WEAPON_BIZON,WEAPON_MAG7 ,
	WEAPON_NEGEV,WEAPON_SAWEDOFF,WEAPON_TEC9 ,WEAPON_P2000,WEAPON_MP7 ,
	WEAPON_MP9 ,WEAPON_NOVA ,WEAPON_P250,WEAPON_SCAR20,
	WEAPON_SG553,WEAPON_SCOUT,WEAPON_USPS,WEAPON_CZ75,WEAPON_REVOLVER,
};

bool isWeaponKnife(int weaponid)
{
	if (weaponid == 0 || weaponid == 42 || weaponid == 59 || weaponid == 41 || weaponid == 500 || weaponid == 515 || weaponid == 512 || weaponid == 505 || weaponid == 506 || weaponid == 507 || weaponid == 508 || weaponid == 516 || weaponid == 514 || weaponid == 509)
		return true;

	return false;
}

bool CSkin::ApplyCustomKillIcon(IGameEvent* pEvent)
{
	int nUserID = pEvent->GetInt("attacker");

	if (!nUserID)
		return false;

	if (Interfaces::Engine()->GetPlayerForUserID(nUserID) != Interfaces::Engine()->GetLocalPlayer())
		return false;

	const char* szWeapon = pEvent->GetString("weapon");

	for (auto ReplacementIcon : g_KillIconCfg)
	{
		if (!strcmp(szWeapon, ReplacementIcon.first))
		{
			pEvent->SetString("weapon", ReplacementIcon.second);
			break;
		}
	}

	return true;
}

void CSkin::SetKillIconCfg()
{
	//[enc_string_disable /]
	char* pszKnifeModelsIcon[10] =
	{
		"bayonet","knife_flip","knife_gut","knife_karambit",
		"knife_m9_bayonet","knife_tactical","knife_falchion",
		"knife_survival_bowie","knife_butterfly","knife_push"
	};
	//[enc_string_enable /]
	if (g_Options.Skinchanger.knifemodel >= 1 && g_Options.Skinchanger.knifemodel <= 10)
	{
		g_KillIconCfg["knife_default_ct"] = pszKnifeModelsIcon[g_Options.Skinchanger.knifemodel - 1];
	}
	else
	{
		g_KillIconCfg["knife_default_ct"] = "knife_default_ct";
	}
	if (g_Options.Skinchanger.knifemodel >= 1 && g_Options.Skinchanger.knifemodel <= 10)
	{
		g_KillIconCfg["knife_t"] = pszKnifeModelsIcon[g_Options.Skinchanger.knifemodel - 1];
	}
	else
	{
		g_KillIconCfg["knife_t"] = "knife_t";
	}
}
enum class StickerAttributeType_t
{
	Index,
	Wear,
	Scale,
	Rotation
};
DWORD dwEconItemInterfaceWrapper = 0x2DB0 + 0xC;

typedef float(__thiscall* GetStickerAttributeBySlotIndexFloatFn)(void*, int, StickerAttributeType_t, float);
GetStickerAttributeBySlotIndexFloatFn oGetStickerAttributeBySlotIndexFloat;

float __fastcall Hooked_GetStickerAttributeBySlotIndexFloat(void* thisptr, void* edx, int iSlot, StickerAttributeType_t iAttribute, float flUnknown)
{
	auto pItem = reinterpret_cast<CBaseCombatWeapon*>(uintptr_t(thisptr) - dwEconItemInterfaceWrapper);

	if (!pItem)
	{
		return oGetStickerAttributeBySlotIndexFloat(thisptr, iSlot, iAttribute, flUnknown);
	}

	int iID = *pItem->GetItemDefinitionIndex();

	switch (iAttribute)
	{
	case StickerAttributeType_t::Wear:
		return 0.0000000001f;
	case StickerAttributeType_t::Scale:
		return 1.f;
	case StickerAttributeType_t::Rotation:
		return 0;
	default:
		break;
	}

	return oGetStickerAttributeBySlotIndexFloat(thisptr, iSlot, iAttribute, flUnknown);
}

typedef UINT(__thiscall* GetStickerAttributeBySlotIndexIntFn)(void*, int, StickerAttributeType_t, float);
GetStickerAttributeBySlotIndexIntFn oGetStickerAttributeBySlotIndexInt;

UINT __fastcall Hooked_GetStickerAttributeBySlotIndexInt(void* thisptr, void* edx, int iSlot, StickerAttributeType_t iAttribute, UINT iUnknown)
{
	auto pItem = reinterpret_cast<CBaseCombatWeapon*>(uintptr_t(thisptr) - dwEconItemInterfaceWrapper);

	if (!pItem)
	{
		return oGetStickerAttributeBySlotIndexInt(thisptr, iSlot, iAttribute, iUnknown);
	}

	int iID = *pItem->GetItemDefinitionIndex();

	if (iAttribute == StickerAttributeType_t::Index)
	{
		return (g_Options.Sticker.Weapons[iID].Sticker[iSlot].ID);
	}

	return oGetStickerAttributeBySlotIndexInt(thisptr, iSlot, iAttribute, iUnknown);
}

bool IsCodePtr(void* ptr)
{
	constexpr const DWORD protect_flags = PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY;

	MEMORY_BASIC_INFORMATION Out;
	VirtualQuery(ptr, &Out, sizeof Out);

	return Out.Type && !(Out.Protect & (PAGE_GUARD | PAGE_NOACCESS)) && Out.Protect & protect_flags;
}

void ApplyStickers(CBaseCombatWeapon* Weapon)
{
	void**& vmt = *reinterpret_cast<void***>(uintptr_t(Weapon) + dwEconItemInterfaceWrapper);

	static void** hooked_vmt = nullptr;
	if (!hooked_vmt)
	{
		size_t size = 0;

		while (IsCodePtr(vmt[size]))
		{
			++size;
		}

		hooked_vmt = new void*[size];
		memcpy(hooked_vmt, vmt, size * sizeof(void*));

		oGetStickerAttributeBySlotIndexFloat = (GetStickerAttributeBySlotIndexFloatFn)hooked_vmt[4];
		hooked_vmt[4] = reinterpret_cast<void*>(&Hooked_GetStickerAttributeBySlotIndexFloat);

		oGetStickerAttributeBySlotIndexInt = (GetStickerAttributeBySlotIndexIntFn)hooked_vmt[5];
		hooked_vmt[5] = reinterpret_cast<void*>(&Hooked_GetStickerAttributeBySlotIndexInt);
	}

	vmt = hooked_vmt;
}

int GetKnifeDefinitionIndex(int si)
{
	// 0 - Bayonet, 1 - Flip, 2 - Gut, 3 - Karambit, 4 - M9Bayonet, 5 - Huntsman, 6 - Falchion, 7 - Bowie, 8 - Butterfly, 9 - Daggers, 10 - Golden
	switch (si)
	{
	case 0:
		return 500;
	case 1:
		return 505;
	case 2:
		return 506;
	case 3:
		return 507;
	case 4:
		return 508;
	case 5:
		return 509;
	case 6:
		return 512;
	case 7:
		return 514;
	case 8:
		return 515;
	case 9:
		return 516;
	default:
		return -1;
	}
}

int GetCurrentKnifeModel(int currentKnife)
{
	// 0 - Bayonet, 1 - Flip, 2 - Gut, 3 - Karambit, 4 - M9Bayonet, 5 - Huntsman, 6 - Falchion, 7 - Bowie, 8 - Butterfly, 9 - Daggers, 10 - Golden
	switch (currentKnife)
	{
	case 0:
		return Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_knife_bayonet.mdl"); //Bayo
	case 1:
		return Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_knife_flip.mdl"); //Flip
	case 2:
		return Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_knife_gut.mdl"); //Gut
	case 3:
		return Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_knife_karam.mdl"); //Karambit
	case 4:
		return Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_knife_m9_bay.mdl"); //M9 Bayo
	case 5:
		return Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_knife_tactical.mdl"); //Huntsman
	case 6:
		return Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl"); //Falchion
	case 7:
		return Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_knife_survival_bowie.mdl"); //Bowie
	case 8:
		return Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_knife_butterfly.mdl"); //Butterfly
	case 9:
		return Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_knife_push.mdl"); //Daggers
	case 10:
		return Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_knife_gg.mdl"); //GunGame
	default:
		return 0;
	}
}

void SkinChanger() {

	if (!g_Options.Skinchanger.Enabled)
		return;

	int nLocalPlayerID = Interfaces::Engine()->GetLocalPlayer();

	C_BaseEntity* pLocal = (C_BaseEntity*)Interfaces::EntityList()->GetClientEntity(nLocalPlayerID);

	if (!pLocal || pLocal->IsDead())
		return;

	UINT* hWeapons = pLocal->GetWeapons();

	if (!hWeapons)
		return;

	PlayerInfo LocalPlayerInfo;

	Interfaces::Engine()->GetPlayerInfo(nLocalPlayerID, &LocalPlayerInfo);

	for (size_t i = 0; hWeapons[i] != INVALID_EHANDLE_INDEX; i++)
	{
		CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntityList()->GetClientEntity(hWeapons[i] & 0xFFF);


			if (!pWeapon)
				continue;

			int weapon = *pWeapon->GetItemDefinitionIndex();
		
			if (weapon)
			{
				if (g_Options.Skinchanger.W[weapon].nFallbackPaintKit != 0)
					*pWeapon->FallbackPaintKit() = g_Options.Skinchanger.W[weapon].nFallbackPaintKit;

			}

			if (g_Options.Skinchanger.knifemodel > 0)
			{
				
					if (isWeaponKnife(weapon))
					{
						worldmodel_handle = pWeapon->m_hWeaponWorldModel();
						if (worldmodel_handle) worldmodel = (CBaseCombatWeapon*)Interfaces::EntityList()->GetClientEntityFromHandle(worldmodel_handle);

						*pWeapon->ModelIndex() = GetCurrentKnifeModel(g_Options.Skinchanger.knifemodel - 1); // m_nModelIndex
						*pWeapon->ViewModelIndex() = GetCurrentKnifeModel(g_Options.Skinchanger.knifemodel - 1);
						if (worldmodel) *worldmodel->ModelIndex() = GetCurrentKnifeModel(g_Options.Skinchanger.knifemodel - 1) + 1;
						*pWeapon->GetItemDefinitionIndex() = GetKnifeDefinitionIndex(g_Options.Skinchanger.knifemodel - 1);

						//killIcons.clear();
						//killIcons["knife_default_ct"] = "bayonet";
						//killIcons["knife_t"] = "bayonet";

						*pWeapon->FallbackPaintKit() = g_Options.Skinchanger.W[GetKnifeDefinitionIndex(g_Options.Skinchanger.knifemodel - 1)].nFallbackPaintKit; // Lore 
					}
				
			}

			if (LocalPlayerInfo.m_nXuidLow != *pWeapon->OwnerXuidLow())
				continue;

			if (LocalPlayerInfo.m_nXuidHigh != *pWeapon->OwnerXuidHigh())
				continue;
			*pWeapon->GetEntityQuality() = g_Options.Skinchanger.W[weapon].iEntityQuality;
			*pWeapon->FallbackWear() = min(1.f, g_Options.Skinchanger.W[weapon].flFallbackWear + 0.0000000001f);
			*pWeapon->FallbackSeed() = g_Options.Skinchanger.W[weapon].flFallbackSeed;
			*pWeapon->FallbackStatTrak() = g_Options.Skinchanger.W[weapon].nFallbackStatTrak;
			snprintf(pWeapon->GetCustomName(), 32, g_Options.Skinchanger.W[weapon].ChangerName);
			if (g_Options.Skinchanger.W[weapon].iItemDefinitionIndex)
			{
				*pWeapon->GetItemDefinitionIndex() = g_Options.Skinchanger.W[weapon].iItemDefinitionIndex;
			}
			*pWeapon->ItemIDHigh() = -1;
			*pWeapon->AccountID() = LocalPlayerInfo.m_nXuidLow;
			if (g_Options.Skinchanger.EnabledSticker)
			{
				ApplyStickers(pWeapon);

			}

		}
	}



void InitializeKits()
{
	static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	// Search the relative calls

	// call    ItemSystem
	// push    dword ptr [esi+0Ch]
	// lea     ecx, [eax+4]
	// call    CEconItemSchema::GetPaintKitDefinition

	auto sig_address = CSX::Memory::FindPBYTEPattern("client_panorama.dll", (PBYTE)"\xE8\x00\x00\x00\x00\xFF\x76\x0C\x8D\x48\x04\xE8", "x????xxxxxxx", NULL, NULL);

	// Skip the opcode, read rel32 address
	auto item_system_offset = *reinterpret_cast<int32_t*>(sig_address + 1);

	// Add the offset to the end of the instruction
	auto item_system_fn = reinterpret_cast<CCStrike15ItemSystem* (*)()>(sig_address + 5 + item_system_offset);

	// Skip VTable, first member variable of ItemSystem is ItemSchema
	auto item_schema = reinterpret_cast<CCStrike15ItemSchema*>(uintptr_t(item_system_fn()) + sizeof(void*));

	// Dump paint kits
	{
		// Skip the instructions between, skip the opcode, read rel32 address
		auto get_paint_kit_definition_offset = *reinterpret_cast<int32_t*>(sig_address + 11 + 1);

		// Add the offset to the end of the instruction
		auto get_paint_kit_definition_fn = reinterpret_cast<CPaintKit* (__thiscall *)(CCStrike15ItemSchema*, int)>(sig_address + 11 + 5 + get_paint_kit_definition_offset);

		// The last offset is head_element, we need that

		// push    ebp
		// mov     ebp, esp
		// sub     esp, 0Ch
		// mov     eax, [ecx+298h]

		// Skip instructions, skip opcode, read offset
		auto start_element_offset = *reinterpret_cast<intptr_t*>(uintptr_t(get_paint_kit_definition_fn) + 8 + 2);

		// Calculate head base from start_element's offset
		auto head_offset = start_element_offset - 12;

		auto map_head = reinterpret_cast<Head_t<int, CPaintKit*>*>(uintptr_t(item_schema) + head_offset);

		for (int i = 0; i <= map_head->last_element; ++i)
		{
			auto paint_kit = map_head->memory[i].value;

			if (paint_kit->id == 9001)
				continue;

			const wchar_t* wide_name = Interfaces::GetLocalize()->Find(paint_kit->item_name.buffer + 1);
			auto name = converter.to_bytes(wide_name);

			if (paint_kit->id < 10000)
				k_skins.push_back({ paint_kit->id, name });
			else
				k_gloves.push_back({ paint_kit->id, name });
		}

		std::sort(k_skins.begin(), k_skins.end());
		std::sort(k_gloves.begin(), k_gloves.end());
	}

	// Dump sticker kits
	{
		auto sticker_sig = CSX::Memory::FindPBYTEPattern("client_panorama.dll", (PBYTE)"\x53\x8D\x48\x04\xE8\x00\x00\x00\x00\x8B\x4D\x10", "xxxxx????xxx", NULL, NULL) + 4;

		// Skip the opcode, read rel32 address
		auto get_sticker_kit_definition_offset = *reinterpret_cast<intptr_t*>(sticker_sig + 1);

		// Add the offset to the end of the instruction
		auto get_sticker_kit_definition_fn = reinterpret_cast<CPaintKit* (__thiscall *)(CCStrike15ItemSchema*, int)>(sticker_sig + 5 + get_sticker_kit_definition_offset);

		// The last offset is head_element, we need that

		//	push    ebp
		//	mov     ebp, esp
		//	push    ebx
		//	push    esi
		//	push    edi
		//	mov     edi, ecx
		//	mov     eax, [edi + 2BCh]

		// Skip instructions, skip opcode, read offset
		auto start_element_offset = *reinterpret_cast<intptr_t*>(uintptr_t(get_sticker_kit_definition_fn) + 8 + 2);

		// Calculate head base from start_element's offset
		auto head_offset = start_element_offset - 12;

		auto map_head = reinterpret_cast<Head_t<int, CStickerKit*>*>(uintptr_t(item_schema) + head_offset);

		for (int i = 0; i <= map_head->last_element; ++i)
		{
			auto sticker_kit = map_head->memory[i].value;

			char sticker_name_if_valve_fucked_up_their_translations[64];

			auto sticker_name_ptr = sticker_kit->item_name.buffer + 1;

			if (strstr(sticker_name_ptr, "StickerKit_dhw2014_dignitas"))
			{
				strcpy_s(sticker_name_if_valve_fucked_up_their_translations, "StickerKit_dhw2014_teamdignitas");
				strcat_s(sticker_name_if_valve_fucked_up_their_translations, sticker_name_ptr + 27);
				sticker_name_ptr = sticker_name_if_valve_fucked_up_their_translations;
			}

			const wchar_t* wide_name = Interfaces::GetLocalize()->Find(sticker_name_ptr);
			auto name = converter.to_bytes(wide_name);

			k_stickers.push_back({ sticker_kit->id, name });
		}

		std::sort(k_stickers.begin(), k_stickers.end());

		k_stickers.insert(k_stickers.begin(), { 0, "None" });
	}
}
