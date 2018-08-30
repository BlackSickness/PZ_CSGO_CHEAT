#pragma once
#include "HookIncludes.h"
#include "CSX.h"

static CreateClientClassFn GetWearableCreateFn()
{
	auto clazz = Interfaces::Client()->GetAllClasses();

	while (strcmp(clazz->m_pNetworkName, "CEconWearable"))
		clazz = clazz->m_pNext;

	return clazz->m_pCreateFn;
}
IClientNetworkable* CL_CreateDLLEntity(int iEnt, int iClass, int iSerialNum)
{
	ClientClass* pClient = Interfaces::Client()->GetAllClasses();
	if (!pClient)
		return false;

	while (pClient)
	{
		if (pClient->m_ClassID == iClass)
			return pClient->m_pCreateFn(iEnt, iSerialNum);
		pClient = pClient->m_pNext;
	}
	return false;
}

int GetGloveSkinByMenu(int gloveIndex, int skinIndex)
{
	if (gloveIndex == 0) // bloudhound
	{
		switch (skinIndex)
		{
		case 0:
			return 10006;
		case 1:
			return 10007;
		case 2:
			return 10008;
		case 3:
			return 10039;
		default:
			return 0;
		}
	}
	else if (gloveIndex == 1) // Sport
	{
		switch (skinIndex)
		{
		case 0:
			return 10038;
		case 1:
			return 10037;
		case 2:
			return 10018;
		case 3:
			return 10019;
		case 4:
			return 10048;
		case 5:
			return 10047;
		case 6:
			return 10046;
		case 7:
			return 10045;
		default:
			return 0;
		}
	}
	else if (gloveIndex == 2) // Driver
	{
		switch (skinIndex)
		{
		case 0:
			return 10013;
		case 1:
			return 10015;
		case 2:
			return 10016;
		case 3:
			return 10040;
		case 4:
			return 10044;
		case 5:
			return 10043;
		case 6:
			return 10042;
		case 7:
			return 10041;
		default:
			return 0;
		}
	}
	else if (gloveIndex == 3) // Wraps
	{
		switch (skinIndex)
		{
		case 0:
			return 10009;
		case 1:
			return 10010;
		case 2:
			return 10021;
		case 3:
			return 10036;
		case 4:
			return 10056;
		case 5:
			return 10055;
		case 6:
			return 10054;
		case 7:
			return 10053;
		default:
			return 0;
		}
	}
	else if (gloveIndex == 4) // Moto
	{
		switch (skinIndex)
		{
		case 0:
			return 10024;
		case 1:
			return 10026;
		case 2:
			return 10027;
		case 3:
			return 10028;
		case 4:
			return 10052;
		case 5:
			return 10051;
		case 6:
			return 10050;
		case 7:
			return 10049;
		default:
			return 0;
		}
	}
	else if (gloveIndex == 5) // Specialist
	{
		switch (skinIndex)
		{
		case 0:
			return 10030;
		case 1:
			return 10033;
		case 2:
			return 10034;
		case 3:
			return 10035;
		case 4:
			return 10064;
		case 5:
			return 10063;
		case 6:
			return 10062;
		case 7:
			return 10061;
		default:
			return 0;
		}
	}
	else if (gloveIndex == 6) // Hydra
	{
		switch (skinIndex)
		{
		case 0:
			return 10060;
		case 1:
			return 10059;
		case 2:
			return 10058;
		case 3:
			return 10057;
		case 4:
			return 10064;
		case 5:
			return 10063;
		case 6:
			return 10062;
		case 7:
			return 10061;
		default:
			return 0;
		}
	}
	else
		return 0;
}

bool isKnife(int id)
{
	switch (id)
	{
	case 42:
		return true;
	case 500:
		return true;
	case 506:
		return true;
	case 505:
		return true;
	case 508:
		return true;
	case 507:
		return true;
	case 515:
		return true;
	case 514:
		return true;
	case 512:
		return true;
	case 516:
		return true;
	case 59:
		return true;
	}
	return false;
}


void GloveChanger()
{
	if (!g_Options.Skinchanger.Enabled)
		return;

	C_BaseEntity *local = Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());

	if (!local->IsAlive())
		return;

	UINT* pWeapons = local->GetWeapons();

	if (!pWeapons)
		return;

	UINT* pWareables = local->GetWearables();

	if (!Interfaces::EntityList()->GetClientEntityFromHandle((PVOID)pWareables[0]))
	{
		for (ClientClass* pClass = Interfaces::Client()->GetAllClasses(); pClass; pClass = pClass->m_pNext)
		{
			if (pClass->m_ClassID == (int)ClassID::CEconWearable)
			{
				int iEntry = Interfaces::EntityList()->GetHighestEntityIndex() + 1;
				int iSerial = CSX::Utils::RandomIntRange(0x0, 0xFFF);

				if (pClass->m_pCreateFn(iEntry, iSerial))
				{
					pWareables[0] = iEntry | (iSerial << 16);
					break;
				}
			}
		}

		if (!pWareables[0])
			return;

		CGloves* glove = (CGloves*)Interfaces::EntityList()->GetClientEntityFromHandle((PVOID)pWareables[0]);

		if (!glove)
			return;

		PlayerInfo LocalPlayerInfo;

		if (!Interfaces::Engine()->GetPlayerInfo(Interfaces::Engine()->GetLocalPlayer(), &LocalPlayerInfo))
			return;

		switch (g_Options.Skinchanger.glove)
		{
		case 1: //bloodhound
		{
			if (*glove->GetItemDefinitionIndex() != 5027 || *glove->FallbackPaintKit() != GetGloveSkinByMenu(0, g_Options.Skinchanger.gloveskin))
			{
				*glove->GetItemDefinitionIndex() = 5027;
				*glove->FallbackPaintKit() = GetGloveSkinByMenu(0, g_Options.Skinchanger.gloveskin);
				*glove->GetEntityQuality() = 4;
				*glove->FallbackSeed() = 0;
				*glove->FallbackWear() = 0.00000001f;
				*glove->FallbackStatTrak() = -1;
				*glove->ItemIDHigh() = -1;
				*glove->AccountID() = LocalPlayerInfo.m_nXuidLow;
				glove->SetGloveModelIndex(Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl"));
				glove->PreDataUpdate(DATA_UPDATE_CREATED);
			}
		}
		break;
		case 2: //sporty
		{
			if (*glove->GetItemDefinitionIndex() != 5030 || *glove->FallbackPaintKit() != GetGloveSkinByMenu(1, g_Options.Skinchanger.gloveskin))
			{
				glove->SetGloveModelIndex(Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl"));
				*glove->GetItemDefinitionIndex() = 5030;
				*glove->FallbackPaintKit() = GetGloveSkinByMenu(1, g_Options.Skinchanger.gloveskin);
				*glove->GetEntityQuality() = 4;
				*glove->FallbackSeed() = 0;
				*glove->FallbackWear() = 0.00000001f;
				*glove->FallbackStatTrak() = -1;
				*glove->ItemIDHigh() = -1;
				*glove->AccountID() = LocalPlayerInfo.m_nXuidLow;

				glove->PreDataUpdate(DATA_UPDATE_CREATED);
			}
		}
		break;
		case 3://SLICK_GLOVES
		{
			if (*glove->GetItemDefinitionIndex() != 5031 || *glove->FallbackPaintKit() != GetGloveSkinByMenu(2, g_Options.Skinchanger.gloveskin))
			{
				glove->SetGloveModelIndex(Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl"));
				*glove->GetItemDefinitionIndex() = 5031;
				*glove->FallbackPaintKit() = GetGloveSkinByMenu(2, g_Options.Skinchanger.gloveskin);
				*glove->GetEntityQuality() = 4;
				*glove->FallbackSeed() = 0;
				*glove->FallbackWear() = 0.00000001f;
				*glove->FallbackStatTrak() = -1;
				*glove->ItemIDHigh() = -1;
				*glove->AccountID() = LocalPlayerInfo.m_nXuidLow;

				glove->PreDataUpdate(DATA_UPDATE_CREATED);
			}
		}
		break;
		case 4://LEATHER_HANDWRAPS
		{
			if (*glove->GetItemDefinitionIndex() != 5032 || *glove->FallbackPaintKit() != GetGloveSkinByMenu(3, g_Options.Skinchanger.gloveskin))
			{
				glove->SetGloveModelIndex(Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl"));
				*glove->GetItemDefinitionIndex() = 5032;
				*glove->FallbackPaintKit() = GetGloveSkinByMenu(3, g_Options.Skinchanger.gloveskin);
				*glove->GetEntityQuality() = 4;
				*glove->FallbackSeed() = 0;
				*glove->FallbackWear() = 0.00000001f;
				*glove->FallbackStatTrak() = -1;
				*glove->ItemIDHigh() = -1;
				*glove->AccountID() = LocalPlayerInfo.m_nXuidLow;

				glove->PreDataUpdate(DATA_UPDATE_CREATED);
			}
		}
		break;
		case 5://MOTORCYCLE_GLOVES
		{
			if (*glove->GetItemDefinitionIndex() != 5033 || *glove->FallbackPaintKit() != GetGloveSkinByMenu(4, g_Options.Skinchanger.gloveskin))
			{
				glove->SetGloveModelIndex(Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl"));
				*glove->GetItemDefinitionIndex() = 5033;
				*glove->FallbackPaintKit() = GetGloveSkinByMenu(4, g_Options.Skinchanger.gloveskin);
				*glove->GetEntityQuality() = 4;
				*glove->FallbackSeed() = 0;
				*glove->FallbackWear() = 0.00000001f;
				*glove->FallbackStatTrak() = -1;
				*glove->ItemIDHigh() = -1;
				*glove->AccountID() = LocalPlayerInfo.m_nXuidLow;

				glove->PreDataUpdate(DATA_UPDATE_CREATED);
			}
		}
		break;
		case 6://SPECIALIST_GLOVES
		{
			if (*glove->GetItemDefinitionIndex() != 5034 || *glove->FallbackPaintKit() != GetGloveSkinByMenu(5, g_Options.Skinchanger.gloveskin))
			{
				glove->SetGloveModelIndex(Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl"));
				*glove->GetItemDefinitionIndex() = 5034;
				*glove->FallbackPaintKit() = GetGloveSkinByMenu(5, g_Options.Skinchanger.gloveskin);
				*glove->GetEntityQuality() = 4;
				*glove->FallbackSeed() = 0;
				*glove->FallbackWear() = 0.00000001f;
				*glove->FallbackStatTrak() = -1;
				*glove->ItemIDHigh() = -1;
				*glove->AccountID() = LocalPlayerInfo.m_nXuidLow;

				glove->PreDataUpdate(DATA_UPDATE_CREATED);
			}
		}
		break;
		case 7://HYDRA_GLOVES
		{
			if (*glove->GetItemDefinitionIndex() != 5035 || *glove->FallbackPaintKit() != GetGloveSkinByMenu(6, g_Options.Skinchanger.gloveskin))
			{
				glove->SetGloveModelIndex(Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound_hydra.mdl"));
				*glove->GetItemDefinitionIndex() = 5035;
				*glove->FallbackPaintKit() = GetGloveSkinByMenu(6, g_Options.Skinchanger.gloveskin);
				*glove->GetEntityQuality() = 4;
				*glove->FallbackSeed() = 0;
				*glove->FallbackWear() = 0.00000001f;
				*glove->FallbackStatTrak() = -1;
				*glove->ItemIDHigh() = -1;
				*glove->AccountID() = LocalPlayerInfo.m_nXuidLow;

				glove->PreDataUpdate(DATA_UPDATE_CREATED);
			}
		}
		break;
		}
	}
}
