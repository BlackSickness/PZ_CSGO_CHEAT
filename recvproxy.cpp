#include "SDK.h"


#define SEQUENCE_DEFAULT_DRAW 0
#define SEQUENCE_DEFAULT_IDLE1 1
#define SEQUENCE_DEFAULT_IDLE2 2
#define SEQUENCE_DEFAULT_LIGHT_MISS1 3
#define SEQUENCE_DEFAULT_LIGHT_MISS2 4
#define SEQUENCE_DEFAULT_HEAVY_MISS1 9
#define SEQUENCE_DEFAULT_HEAVY_HIT1 10
#define SEQUENCE_DEFAULT_HEAVY_BACKSTAB 11
#define SEQUENCE_DEFAULT_LOOKAT01 12

#define SEQUENCE_BUTTERFLY_DRAW 0
#define SEQUENCE_BUTTERFLY_DRAW2 1
#define SEQUENCE_BUTTERFLY_LOOKAT01 13
#define SEQUENCE_BUTTERFLY_LOOKAT03 15

#define SEQUENCE_FALCHION_IDLE1 1
#define SEQUENCE_FALCHION_HEAVY_MISS1 8
#define SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP 9
#define SEQUENCE_FALCHION_LOOKAT01 12
#define SEQUENCE_FALCHION_LOOKAT02 13

#define SEQUENCE_DAGGERS_IDLE1 1
#define SEQUENCE_DAGGERS_LIGHT_MISS1 2
#define SEQUENCE_DAGGERS_LIGHT_MISS5 6
#define SEQUENCE_DAGGERS_HEAVY_MISS2 11
#define SEQUENCE_DAGGERS_HEAVY_MISS1 12

#define SEQUENCE_BOWIE_IDLE1 1
using namespace std;
// Helper function to generate a random sequence number.
inline int RandomSequence(int low, int high) {
    return (rand() % (high - low + 1) + low);
}

#define	LIFE_ALIVE 0

#define RandomInt(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);



RecvVarProxyFn fnSequenceProxyFn = nullptr;

RecvVarProxyFn oRecvnModelIndex;
void Hooked_RecvProxy_Viewmodel(CRecvProxyData *pData, void *pStruct, void *pOut)
{
    // Get the knife view model id's
    int default_t = Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_knife_default_t.mdl");
    int default_ct = Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_knife_default_ct.mdl");
    int iBayonet = Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_knife_bayonet.mdl");
    int iButterfly = Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
    int iFlip = Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_knife_flip.mdl");
    int iGut = Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_knife_gut.mdl");
    int iKarambit = Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_knife_karam.mdl");
    int iM9Bayonet = Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
    int iHuntsman = Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_knife_tactical.mdl");
    int iFalchion = Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl");
    int iDagger = Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_knife_push.mdl");
    int iBowie = Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_knife_survival_bowie.mdl");

    int iGunGame = Interfaces::ModelInfo()->GetModelIndex("models/weapons/v_knife_gg.mdl");

    // Get local player (just to stop replacing spectators knifes)
    C_BaseEntity* pLocal = Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());
    if (g_Options.Skinchanger.Enabled && pLocal)
    {
        // If we are alive and holding a default knife(if we already have a knife don't worry about changing)
        if (pLocal->IsAlive() && (
            pData->m_Value.m_Int == default_t ||
            pData->m_Value.m_Int == default_ct ||
            pData->m_Value.m_Int == iBayonet ||
            pData->m_Value.m_Int == iFlip ||
            pData->m_Value.m_Int == iGunGame ||
            pData->m_Value.m_Int == iGut ||
            pData->m_Value.m_Int == iKarambit ||
            pData->m_Value.m_Int == iM9Bayonet ||
            pData->m_Value.m_Int == iHuntsman ||
            pData->m_Value.m_Int == iBowie ||
            pData->m_Value.m_Int == iButterfly ||
            pData->m_Value.m_Int == iFalchion ||
            pData->m_Value.m_Int == iDagger))
        {
            // Set whatever knife we want
            if (g_Options.Skinchanger.knifemodel == 1)
                pData->m_Value.m_Int = iBayonet;
            else if (g_Options.Skinchanger.knifemodel == 2)
                pData->m_Value.m_Int = iFlip;
            else if (g_Options.Skinchanger.knifemodel == 3)
                pData->m_Value.m_Int = iGut;
            else if (g_Options.Skinchanger.knifemodel == 4)
                pData->m_Value.m_Int = iKarambit;
            else if (g_Options.Skinchanger.knifemodel == 5)
                pData->m_Value.m_Int = iM9Bayonet;
            else if (g_Options.Skinchanger.knifemodel == 6)
                pData->m_Value.m_Int = iHuntsman;
            else if (g_Options.Skinchanger.knifemodel == 7)
                pData->m_Value.m_Int = iFalchion;
            else if (g_Options.Skinchanger.knifemodel == 8)
                pData->m_Value.m_Int = iBowie;
            else if (g_Options.Skinchanger.knifemodel == 9)
                pData->m_Value.m_Int = iButterfly;
            else if (g_Options.Skinchanger.knifemodel == 10)
                pData->m_Value.m_Int = iDagger;
        }
    }
    // Carry on the to original proxy
    oRecvnModelIndex(pData, pStruct, pOut);
}





void SetViewModelSequence2(const CRecvProxyData *pDataConst, void *pStruct, void *pOut)
{

    // Make the incoming data editable.
    CRecvProxyData* pData = const_cast<CRecvProxyData*>(pDataConst);

    // Confirm that we are replacing our view model and not someone elses.
    CBaseViewModel* pViewModel = static_cast<CBaseViewModel*>(pStruct);

    if (pViewModel)
    {
        C_BaseEntity* pOwner = static_cast<C_BaseEntity*>(Interfaces::EntityList()->GetClientEntity(pViewModel->GetOwner() & 0xFFF));

        // Compare the owner entity of this view model to the local player entity.
        if (pOwner && pOwner->EntIndex() == Interfaces::Engine()->GetLocalPlayer())
        {
            // Get the filename of the current view model.
			const model_t* pModel = Interfaces::ModelInfo()->GetModel(pViewModel->GetModelIndex());
            std::string szModel = Interfaces::ModelInfo()->GetModelName(pModel);

            // Store the current sequence.
            int m_nSequence = pData->m_Value.m_Int;
            if (szModel == "models/weapons/v_knife_butterfly.mdl")
            {
                // Fix animations for the Butterfly Knife.
                switch (m_nSequence)
                {
                case SEQUENCE_DEFAULT_DRAW:
                    m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
                    break;
                case SEQUENCE_DEFAULT_LOOKAT01:
                    m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
                    break;
                default:
                    m_nSequence++;
                }
            }
            else if (szModel == "models/weapons/v_knife_falchion_advanced.mdl")
            {
                // Fix animations for the Falchion Knife.
                switch (m_nSequence)
                {
                case SEQUENCE_DEFAULT_IDLE2:
                    m_nSequence = SEQUENCE_FALCHION_IDLE1; break;
                case SEQUENCE_DEFAULT_HEAVY_MISS1:
                    m_nSequence = RandomInt(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP);
                    break;
                case SEQUENCE_DEFAULT_LOOKAT01:
                    m_nSequence = RandomInt(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02);
                    break;
                case SEQUENCE_DEFAULT_DRAW:
                case SEQUENCE_DEFAULT_IDLE1:
                    break;
                default:
                    m_nSequence--;
                }
            }
            else if (szModel == "models/weapons/v_knife_push.mdl")
            {
                // Fix animations for the Shadow Daggers.
                switch (m_nSequence)
                {
                case SEQUENCE_DEFAULT_IDLE2:
                    m_nSequence = SEQUENCE_DAGGERS_IDLE1; break;
                case SEQUENCE_DEFAULT_LIGHT_MISS1:
                case SEQUENCE_DEFAULT_LIGHT_MISS2:
                    m_nSequence = RandomInt(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5);
                    break;
                case SEQUENCE_DEFAULT_HEAVY_MISS1:
                    m_nSequence = RandomInt(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1);
                    break;
                case SEQUENCE_DEFAULT_HEAVY_HIT1:
                case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
                case SEQUENCE_DEFAULT_LOOKAT01:
                    m_nSequence += 3; break;
                case SEQUENCE_DEFAULT_DRAW:
                case SEQUENCE_DEFAULT_IDLE1:
                    break;
                default:
                    m_nSequence += 2;
                }
            }
            else if (szModel == "models/weapons/v_knife_survival_bowie.mdl")
            {
                // Fix animations for the Bowie Knife.
                switch (m_nSequence)
                {
                case SEQUENCE_DEFAULT_DRAW:
                case SEQUENCE_DEFAULT_IDLE1:
                    break;
                case SEQUENCE_DEFAULT_IDLE2:
                    m_nSequence = SEQUENCE_BOWIE_IDLE1;
                    break;
                default:
                    m_nSequence--;
                }
            }

            // Set the fixed sequence.
            pData->m_Value.m_Int = m_nSequence;
        }
    }

    // Call original function with the modified data.

    fnSequenceProxyFn(pData, pStruct, pOut);
}

RecvVarProxyFn fnNoSmoke;
void NoSmoke(const CRecvProxyData *pData, void *pStruct, void *pOut)
{
	if(g_Options.Misc.NoSmoke) *(bool*)((DWORD)pOut + 0x1) = true;

	fnNoSmoke(pData, pStruct, pOut);
}
void AnimationFixHook()
{
    for (ClientClass* pClass = Interfaces::Client()->GetAllClasses(); pClass; pClass = pClass->m_pNext) {
        if (!strcmp(pClass->m_pNetworkName, "CBaseViewModel")) {
            // Search for the 'm_nModelIndex' property.
            RecvTable* pClassTable = pClass->m_pRecvTable;

            for (int nIndex = 0; nIndex < pClassTable->m_nProps; nIndex++) {
                RecvProp* pProp = &pClassTable->m_pProps[nIndex];

                if (!pProp || strcmp(pProp->m_pVarName, "m_nSequence"))
                    continue;

                // Store the original proxy function.
                fnSequenceProxyFn = static_cast<RecvVarProxyFn>(pProp->m_ProxyFn);

                // Replace the proxy function with our sequence changer.
                pProp->m_ProxyFn = static_cast<RecvVarProxyFn>(SetViewModelSequence2);

                break;
            }

            break;
        }
    }
}

void AnimationFixUnhook()
{
    for (ClientClass* pClass = Interfaces::Client()->GetAllClasses(); pClass; pClass = pClass->m_pNext) {
        if (!strcmp(pClass->m_pNetworkName, "CBaseViewModel")) {
            // Search for the 'm_nModelIndex' property.
            RecvTable* pClassTable = pClass->m_pRecvTable;

            for (int nIndex = 0; nIndex < pClassTable->m_nProps; nIndex++) {
                RecvProp* pProp = &pClassTable->m_pProps[nIndex];

                if (!pProp || strcmp(pProp->m_pVarName, "m_nSequence"))
                    continue;

                // Replace the proxy function with our sequence changer.
                pProp->m_ProxyFn = fnSequenceProxyFn;

                break;
            }

            break;
        }
    }
}
void NetvarHook()
{
    AnimationFixHook();
    ClientClass *pClass = Interfaces::Client()->GetAllClasses();
    while (pClass)
    {
        const char *pszName = pClass->m_pRecvTable->m_pNetTableName;
        if (!strcmp(pszName, "DT_SmokeGrenadeProjectile"))
        {
            for (int i = 0; i < pClass->m_pRecvTable->m_nProps; i++)
            {
                RecvProp *pProp = &(pClass->m_pRecvTable->m_pProps[i]);
                const char *name = pProp->m_pVarName;
                if (!strcmp(name, "m_bDidSmokeEffect"))
                {
                    fnNoSmoke = (RecvVarProxyFn)pProp->m_ProxyFn;
                    pProp->m_ProxyFn = NoSmoke;
                }

            }
        }
        else if (!strcmp(pszName, "DT_BaseViewModel"))
        {
            for (int i = 0; i < pClass->m_pRecvTable->m_nProps; i++)
            {
                RecvProp *pProp = &(pClass->m_pRecvTable->m_pProps[i]);
                const char *name = pProp->m_pVarName;
                if (!strcmp(name, "m_nModelIndex"))
                {
                    oRecvnModelIndex = (RecvVarProxyFn)pProp->m_ProxyFn;
                    pProp->m_ProxyFn = Hooked_RecvProxy_Viewmodel;
                }
            }
        }
        pClass = pClass->m_pNext;
    }
}
void UnloadProxy()
{
    AnimationFixUnhook();
    ClientClass *pClass = Interfaces::Client()->GetAllClasses();
    while (pClass)
    {
        const char *pszName = pClass->m_pRecvTable->m_pNetTableName;
        if (!strcmp(pszName, "DT_SmokeGrenadeProjectile"))
        {
            for (int i = 0; i < pClass->m_pRecvTable->m_nProps; i++)
            {
                RecvProp *pProp = &(pClass->m_pRecvTable->m_pProps[i]);
                const char *name = pProp->m_pVarName;
                if (!strcmp(name, "m_bDidSmokeEffect"))
                {
                    pProp->m_ProxyFn = fnNoSmoke;
                }

            }
        }
        else if (!strcmp(pszName, "DT_BaseViewModel"))
        {
            for (int i = 0; i < pClass->m_pRecvTable->m_nProps; i++)
            {
                RecvProp *pProp = &(pClass->m_pRecvTable->m_pProps[i]);
                const char *name = pProp->m_pVarName;


                // Knives
                if (!strcmp(name, "m_nModelIndex"))
                {
                    pProp->m_ProxyFn = oRecvnModelIndex;
                }
            }
        }
        pClass = pClass->m_pNext;
    }
}

