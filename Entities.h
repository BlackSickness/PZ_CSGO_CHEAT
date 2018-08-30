

#pragma once
#include "MiscDefinitions.h"
#include "VMatrix.hpp"
#include "Vector.hpp"
#include "CSX.h"
#include "Ray_t.hpp"
#include <map>



#define BONE_USED_BY_HITBOX			0x00000100
#define BONE_USED_BY_ATTACHMENT      0x00000200    // bone (or child) is used by an attachment point


#define TICK_INTERVAL			(Interfaces::Globals()->interval_per_tick)

#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
#define TICKS_TO_TIME( t )		( Interfaces::Globals()->interval_per_tick *( t ) )
enum class TeamID : int
{
	TEAM_UNASSIGNED,
	TEAM_SPECTATOR,
	TEAM_TERRORIST,
	TEAM_COUNTER_TERRORIST,
};
enum PLAYER_TEAM
{
	TEAM_SPEC = 1,
	TEAM_TT,
	TEAM_CT
};
class IClientRenderable;
class IClientNetworkable;
class IClientUnknown;
class IClientThinkable;
class C_BaseEntity;
class CSWeaponInfo;

typedef unsigned long CBaseHandle;

struct RecvProp;

class DVariant
{
public:
	union
	{
		float m_Float;
		long m_Int;
		char* m_pString;
		void* m_pData;
		float m_Vector[3];
	};
};

class CRecvProxyData
{
public:
	const RecvProp* m_pRecvProp;
	char _pad[4];//csgo ( for l4d keep it commented out :) )
	DVariant m_Value;
	int m_iElement;
	int m_ObjectID;
};

typedef void(*RecvVarProxyFn)(const CRecvProxyData* pData, void* pStruct, void* pOut);

struct RecvTable
{
	RecvProp* m_pProps;
	int m_nProps;
	void* m_pDecoder;
	char* m_pNetTableName;
	bool m_bInitialized;
	bool m_bInMainList;
};

struct RecvProp
{
	char* m_pVarName;
	int m_RecvType;
	int m_Flags;
	int m_StringBufferSize;
	bool m_bInsideArray;
	const void* m_pExtraData;
	RecvProp* m_pArrayProp;
	void* m_ArrayLengthProxy;
	void* m_ProxyFn;
	void* m_DataTableProxyFn;
	RecvTable* m_pDataTable;
	int m_Offset;
	int m_ElementStride;
	int m_nElements;
	const char* m_pParentArrayPropName;
};



typedef IClientNetworkable*	(*CreateClientClassFn)(int entnum, int serialNum);
typedef IClientNetworkable* (*CreateEventFn)();

class ClientClass
{
public:
	CreateClientClassFn		m_pCreateFn;
	CreateEventFn m_pCreateEventFn;
	char			*m_pNetworkName;
	RecvTable		*m_pRecvTable;
	ClientClass		*m_pNext;
	int				m_ClassID;
};

#include "memesets.h"
#include "NetVarManager.h"


struct CHudTexture
{
	char	szShortName[64];	//0x0000
	char	szTextureFile[64];	//0x0040
	bool	bRenderUsingFont;	//0x0080
	bool	bPrecached;			//0x0081
	char	cCharacterInFont;	//0x0082
	BYTE	pad_0x0083;			//0x0083
	int		hFont;				//0x0084
	int		iTextureId;			//0x0088
	float	afTexCoords[4];		//0x008C
	int		iPosX[4];			//0x009C
}; //Size=0x00AC


class CSWeaponInfo {
public:
	char _0x0000[20];
	__int32 max_clip;			//0x0014 
	char _0x0018[12];
	__int32 max_reserved_ammo;	//0x0024 
	char _0x0028[96];
	char* hud_name;				//0x0088 
	char* weapon_name;
	char _0x0090[56];
	__int32 type;
	char _0x00CC[4];
	__int32 price;
	__int32 reward;
	char _0x00D8[20];
	BYTE full_auto;				//0x00EC 
	char _0x00ED[3];
	__int32 damage;				//0x00F0 
	float armor_ratio;			//0x00F4 
	__int32 bullets;			//0x00F8 
	float penetration;			//0x00FC 
	char _0x0100[8];
	float range;				//0x0108 
	float range_modifier;		//0x010C 
	char _0x0110[16];
	BYTE silencer;				//0x0120 
	char _0x0121[15];
	float max_speed;			//0x0130 
	float max_speed_alt;		//0x0134 
	char _0x0138[76];
	__int32 recoil_seed;		//0x0184 
	char _0x0188[32];
};// Size=0x01A8

enum class ClassID
{
	CAK47 = 1,
	CBaseAnimating = 2,
	CBaseAnimatingOverlay = 3,
	CBaseAttributableItem = 4,
	CBaseButton = 5,
	CBaseCombatCharacter = 6,
	CBaseCombatWeapon = 7,
	CBaseCSGrenade = 8,
	CBaseCSGrenadeProjectile = 9,
	CBaseDoor = 10,
	CBaseEntity = 11,
	CBaseFlex = 12,
	CBaseGrenade = 13,
	CBaseParticleEntity = 14,
	CBasePlayer = 15,
	CBasePropDoor = 16,
	CBaseTeamObjectiveResource = 17,
	CBaseTempEntity = 18,
	CBaseToggle = 19,
	CBaseTrigger = 20,
	CBaseViewModel = 21,
	CBaseVPhysicsTrigger = 22,
	CBaseWeaponWorldModel = 23,
	CBeam = 24,
	CBeamSpotlight = 25,
	CBoneFollower = 26,
	CBreakableProp = 27,
	CBreakableSurface = 28,
	CC4 = 29,
	CCascadeLight = 30,
	CChicken = 31,
	CColorCorrection = 32,
	CColorCorrectionVolume = 33,
	CCSGameRulesProxy = 34,
	CCSPlayer = 35,
	CCSPlayerResource = 36,
	CCSRagdoll = 37,
	CCSTeam = 38,
	CDEagle = 39,
	CDecoyGrenade = 40,
	CDecoyProjectile = 41,
	CDynamicLight = 42,
	CDynamicProp = 43,
	CEconEntity = 44,
	CEconWearable = 45,
	CEmbers = 46,
	CEntityDissolve = 47,
	CEntityFlame = 48,
	CEntityFreezing = 49,
	CEntityParticleTrail = 50,
	CEnvAmbientLight = 51,
	CEnvDetailController = 52,
	CEnvDOFController = 53,
	CEnvParticleScript = 54,
	CEnvProjectedTexture = 55,
	CEnvQuadraticBeam = 56,
	CEnvScreenEffect = 57,
	CEnvScreenOverlay = 58,
	CEnvTonemapController = 59,
	CEnvWind = 60,
	CFEPlayerDecal = 61,
	CFireCrackerBlast = 62,
	CFireSmoke = 63,
	CFireTrail = 64,
	CFish = 65,
	CFlashbang = 66,
	CFogController = 67,
	CFootstepControl = 68,
	CFunc_Dust = 69,
	CFunc_LOD = 70,
	CFuncAreaPortalWindow = 71,
	CFuncBrush = 72,
	CFuncConveyor = 73,
	CFuncLadder = 74,
	CFuncMonitor = 75,
	CFuncMoveLinear = 76,
	CFuncOccluder = 77,
	CFuncReflectiveGlass = 78,
	CFuncRotating = 79,
	CFuncSmokeVolume = 80,
	CFuncTrackTrain = 81,
	CGameRulesProxy = 82,
	CHandleTest = 83,
	CHEGrenade = 84,
	CHostage = 85,
	CHostageCarriableProp = 86,
	CIncendiaryGrenade = 87,
	CInferno = 88,
	CInfoLadderDismount = 89,
	CInfoOverlayAccessor = 90,
	CItem_Healthshot = 91,
	CItemDogtags = 92,
	CKnife = 93,
	CKnifeGG = 94,
	CLightGlow = 95,
	CMaterialModifyControl = 96,
	CMolotovGrenade = 97,
	CMolotovProjectile = 98,
	CMovieDisplay = 99,
	CParticleFire = 100,
	CParticlePerformanceMonitor = 101,
	CParticleSystem = 102,
	CPhysBox = 103,
	CPhysBoxMultiplayer = 104,
	CPhysicsProp = 105,
	CPhysicsPropMultiplayer = 106,
	CPhysMagnet = 107,
	CPlantedC4 = 108,
	CPlasma = 109,
	CPlayerResource = 110,
	CPointCamera = 111,
	CPointCommentaryNode = 112,
	CPointWorldText = 113,
	CPoseController = 114,
	CPostProcessController = 115,
	CPrecipitation = 116,
	CPrecipitationBlocker = 117,
	CPredictedViewModel = 118,
	CProp_Hallucination = 119,
	CPropDoorRotating = 120,
	CPropJeep = 121,
	CPropVehicleDriveable = 122,
	CRagdollManager = 123,
	CRagdollProp = 124,
	CRagdollPropAttached = 125,
	CRopeKeyframe = 126,
	CSCAR17 = 127,
	CSceneEntity = 128,
	CSensorGrenade = 129,
	CSensorGrenadeProjectile = 130,
	CShadowControl = 131,
	CSlideshowDisplay = 132,
	CSmokeGrenade = 133,
	CSmokeGrenadeProjectile = 134,
	CSmokeStack = 135,
	CSpatialEntity = 136,
	CSpotlightEnd = 137,
	CSprite = 138,
	CSpriteOriented = 139,
	CSpriteTrail = 140,
	CStatueProp = 141,
	CSteamJet = 142,
	CSun = 143,
	CSunlightShadowControl = 144,
	CTeam = 145,
	CTeamplayRoundBasedRulesProxy = 146,
	CTEArmorRicochet = 147,
	CTEBaseBeam = 148,
	CTEBeamEntPoint = 149,
	CTEBeamEnts = 150,
	CTEBeamFollow = 151,
	CTEBeamLaser = 152,
	CTEBeamPoints = 153,
	CTEBeamRing = 154,
	CTEBeamRingPoint = 155,
	CTEBeamSpline = 156,
	CTEBloodSprite = 157,
	CTEBloodStream = 158,
	CTEBreakModel = 159,
	CTEBSPDecal = 160,
	CTEBubbles = 161,
	CTEBubbleTrail = 162,
	CTEClientProjectile = 163,
	CTEDecal = 164,
	CTEDust = 165,
	CTEDynamicLight = 166,
	CTEEffectDispatch = 167,
	CTEEnergySplash = 168,
	CTEExplosion = 169,
	CTEFireBullets = 170,
	CTEFizz = 171,
	CTEFootprintDecal = 172,
	CTEFoundryHelpers = 173,
	CTEGaussExplosion = 174,
	CTEGlowSprite = 175,
	CTEImpact = 176,
	CTEKillPlayerAttachments = 177,
	CTELargeFunnel = 178,
	CTEMetalSparks = 179,
	CTEMuzzleFlash = 180,
	CTEParticleSystem = 181,
	CTEPhysicsProp = 182,
	CTEPlantBomb = 183,
	CTEPlayerAnimEvent = 184,
	CTEPlayerDecal = 185,
	CTEProjectedDecal = 186,
	CTERadioIcon = 187,
	CTEShatterSurface = 188,
	CTEShowLine = 189,
	CTesla = 190,
	CTESmoke = 191,
	CTESparks = 192,
	CTESprite = 193,
	CTESpriteSpray = 194,
	CTest_ProxyToggle_Networkable = 194,
	CTestTraceline = 196,
	CTEWorldDecal = 197,
	CTriggerPlayerMovement = 198,
	CTriggerSoundOperator = 199,
	CVGuiScreen = 200,
	CVoteController = 201,
	CWaterBullet = 202,
	CWaterLODControl = 203,
	CWeaponAug = 204,
	CWeaponAWP = 205,
	CWeaponBaseItem = 206,
	CWeaponBizon = 207,
	CWeaponCSBase = 208,
	CWeaponCSBaseGun = 209,
	CWeaponCycler = 210,
	CWeaponElite = 211,
	CWeaponFamas = 212,
	CWeaponFiveSeven = 213,
	CWeaponG3SG1 = 214,
	CWeaponGalil = 215,
	CWeaponGalilAR = 216,
	CWeaponGlock = 217,
	CWeaponHKP2000 = 218,
	CWeaponM249 = 219,
	CWeaponM3 = 220,
	CWeaponM4A1 = 221,
	CWeaponMAC10 = 222,
	CWeaponMag7 = 223,
	CWeaponMP5Navy = 224,
	CWeaponMP7 = 225,
	CWeaponMP9 = 226,
	CWeaponNegev = 227,
	CWeaponNOVA = 228,
	CWeaponP228 = 229,
	CWeaponP250 = 230,
	CWeaponP90 = 231,
	CWeaponSawedoff = 232,
	CWeaponSCAR20 = 233,
	CWeaponScout = 234,
	CWeaponSG550 = 235,
	CWeaponSG552 = 236,
	CWeaponSG556 = 237,
	CWeaponSSG08 = 238,
	CWeaponTaser = 239,
	CWeaponTec9 = 240,
	CWeaponTMP = 241,
	CWeaponUMP45 = 242,
	CWeaponUSP = 243,
	CWeaponXM1014 = 244,
	CWorld = 245,
	DustTrail = 246,
	MovieExplosion = 247,
	ParticleSmokeGrenade = 248,
	RocketTrail = 249,
	SmokeTrail = 250,
	SporeExplosion = 251,
	SporeTrail = 252
};

enum CSGOHitboxID : int
{
	Head = 0,
	Neck,
	Pelvis,
	Stomach,
	LowerChest,
	Chest,
	UpperChest,
	RightThigh,
	LeftThigh,
	RightShin,
	LeftShin,
	RightFoot,
	LeftFoot,
	RightHand,
	LeftHand,
	RightUpperArm,
	RightLowerArm,
	LeftUpperArm,
	LeftLowerArm
};

enum class ItemDefinitionIndexx : short
{
	ITEM_NONE = 0,
	WEAPON_DEAGLE = 1,
	WEAPON_DUALS = 2,
	WEAPON_FIVE7 = 3,
	WEAPON_GLOCK = 4,
	WEAPON_AK47 = 7,
	WEAPON_AUG = 8,
	WEAPON_AWP = 9,
	WEAPON_FAMAS = 10,
	WEAPON_G3SG1 = 11,
	WEAPON_GALIL = 13,
	WEAPON_M249 = 14,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10 = 17,
	WEAPON_P90 = 19,
	WEAPON_MP5SD = 23,
	WEAPON_UMP45 = 24,
	WEAPON_XM1014 = 25,
	WEAPON_BIZON = 26,
	WEAPON_MAG7 = 27,
	WEAPON_NEGEV = 28,
	WEAPON_SAWEDOFF = 29,
	WEAPON_TEC9 = 30,
	WEAPON_TASER = 31,
	WEAPON_P2000 = 32,
	WEAPON_MP7 = 33,
	WEAPON_MP9 = 34,
	WEAPON_NOVA = 35,
	WEAPON_P250 = 36,
	WEAPON_SCAR20 = 38,
	WEAPON_SG553 = 39,
	WEAPON_SCOUT = 40,
	WEAPON_KNIFE_T = 42,
	WEAPON_FLASH = 43,
	WEAPON_HE = 44,
	WEAPON_SMOKE = 45,
	WEAPON_MOLOTOV = 46,
	WEAPON_DECOY = 47,
	WEAPON_INC = 48,
	WEAPON_C4 = 49,
	WEAPON_KNIFE_CT = 59,
	WEAPON_M4A1S = 60,
	WEAPON_USPS = 61,
	WEAPON_CZ75 = 63,
	WEAPON_REVOLVER = 64,
	WEAPON_KNIFE_BAYONET = 500,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT = 506,
	WEAPON_KNIFE_KARAMBIT = 507,
	WEAPON_KNIFE_M9BAYONET = 508,
	WEAPON_KNIFE_HUNTSMAN = 509,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY = 515,
	WEAPON_KNIFE_DAGGER = 516,
	GLOVE_STUDDED_BLOODHOUND = 5027,
	GLOVE_T_SIDE = 5028,
	GLOVE_CT_SIDE = 5029,
	GLOVE_SPORTY = 5030,
	GLOVE_SLICK = 5031,
	GLOVE_LEATHER_WRAP = 5032,
	GLOVE_MOTORCYCLE = 5033,
	GLOVE_SPECIALIST = 5034,
	GLOVE_HYDRA = 5035,
	WEAPON_MAX
};

struct DefItem_t
{
	DefItem_t(const char* displayName,
		const char* entityName,
		const char* entityModel,
		const char* killIcon = nullptr)
	{
		this->displayName = displayName;
		this->entityName = entityName;
		this->entityModel = entityModel;
		this->killIcon = killIcon;
	}

	const char* displayName = nullptr;
	const char* entityName = nullptr;
	const char* entityModel = nullptr;
	const char* killIcon = nullptr;
};

const std::map<ItemDefinitionIndexx, DefItem_t> ItemDefinitionIndexMap = {
	{ ItemDefinitionIndexx::ITEM_NONE,{ "<-Default->", "DEFAULT", "", "" } },
	{ ItemDefinitionIndexx::WEAPON_DEAGLE,{ "#SFUI_WPNHUD_DesertEagle", "weapon_deagle", "models/weapons/v_pist_deagle.mdl", "deagle" } },
	{ ItemDefinitionIndexx::WEAPON_DUALS,{ "#SFUI_WPNHUD_Elites", "weapon_elite", "models/weapons/v_pist_elite.mdl", "elite" } },
	{ ItemDefinitionIndexx::WEAPON_FIVE7,{ "#SFUI_WPNHUD_FiveSeven", "weapon_fiveseven", "models/weapons/v_pist_fiveseven.mdl", "fiveseven" } },
	{ ItemDefinitionIndexx::WEAPON_GLOCK,{ "#SFUI_WPNHUD_Glock18", "weapon_glock", "models/weapons/v_pist_glock18.mdl", "glock" } },
	{ ItemDefinitionIndexx::WEAPON_AK47,{ "#SFUI_WPNHUD_AK47", "weapon_ak47", "models/weapons/v_rif_ak47.mdl", "ak47" } },
	{ ItemDefinitionIndexx::WEAPON_AUG,{ "#SFUI_WPNHUD_Aug", "weapon_aug", "models/weapons/v_rif_aug.mdl", "aug" } },
	{ ItemDefinitionIndexx::WEAPON_AWP,{ "#SFUI_WPNHUD_AWP", "weapon_awp", "models/weapons/v_snip_awp.mdl", "awp" } },
	{ ItemDefinitionIndexx::WEAPON_FAMAS,{ "#SFUI_WPNHUD_Famas", "weapon_famas", "models/weapons/v_rif_famas.mdl", "famas" } },
	{ ItemDefinitionIndexx::WEAPON_G3SG1,{ "#SFUI_WPNHUD_G3SG1", "weapon_g3sg1", "models/weapons/v_snip_g3sg1.mdl", "g3sg1" } },
	{ ItemDefinitionIndexx::WEAPON_GALIL,{ "#SFUI_WPNHUD_GalilAR", "weapon_galilar", "models/weapons/v_rif_galilar.mdl", "galilar" } },
	{ ItemDefinitionIndexx::WEAPON_M249,{ "#SFUI_WPNHUD_M249", "weapon_m249", "models/weapons/v_mach_m249para.mdl", "m249" } },
	{ ItemDefinitionIndexx::WEAPON_M4A1,{ "#SFUI_WPNHUD_M4A1", "weapon_m4a1", "models/weapons/v_rif_m4a1.mdl", "m4a1" } },
	{ ItemDefinitionIndexx::WEAPON_MAC10,{ "#SFUI_WPNHUD_MAC10", "weapon_mac10", "models/weapons/v_smg_mac10.mdl", "mac10" } },
	{ ItemDefinitionIndexx::WEAPON_P90,{ "#SFUI_WPNHUD_P90", "weapon_p90", "models/weapons/v_smg_p90.mdl", "p90" } },
	{ ItemDefinitionIndexx::WEAPON_MP5SD,{ "#SFUI_WPNHUD_MP5SD", "weapon_mp5sd", "models/weapons/v_smg_mp5sd.mdl", "mp5sd" } },
	{ ItemDefinitionIndexx::WEAPON_UMP45,{ "#SFUI_WPNHUD_UMP45", "weapon_ump45", "models/weapons/v_smg_ump45.mdl", "ump45" } },
	{ ItemDefinitionIndexx::WEAPON_XM1014,{ "#SFUI_WPNHUD_xm1014", "weapon_xm1014", "models/weapons/v_shot_xm1014.mdl", "xm1014" } },
	{ ItemDefinitionIndexx::WEAPON_BIZON,{ "#SFUI_WPNHUD_Bizon", "weapon_bizon", "models/weapons/v_smg_bizon.mdl", "bizon" } },
	{ ItemDefinitionIndexx::WEAPON_MAG7,{ "#SFUI_WPNHUD_Mag7", "weapon_mag7", "models/weapons/v_shot_mag7.mdl", "mag7" } },
	{ ItemDefinitionIndexx::WEAPON_NEGEV,{ "#SFUI_WPNHUD_Negev", "weapon_negev", "models/weapons/v_mach_negev.mdl", "negev" } },
	{ ItemDefinitionIndexx::WEAPON_SAWEDOFF,{ "#SFUI_WPNHUD_Sawedoff", "weapon_sawedoff", "models/weapons/v_shot_sawedoff.mdl", "sawedoff" } },
	{ ItemDefinitionIndexx::WEAPON_TEC9,{ "#SFUI_WPNHUD_Tec9", "weapon_tec9", "models/weapons/v_pist_tec9.mdl", "tec9" } },
	{ ItemDefinitionIndexx::WEAPON_TASER,{ "#SFUI_WPNHUD_Taser", "weapon_taser", "models/weapons/v_eq_taser.mdl", "taser" } },
	{ ItemDefinitionIndexx::WEAPON_P2000,{ "#SFUI_WPNHUD_HKP2000", "weapon_hkp2000", "models/weapons/v_pist_hkp2000.mdl", "hkp2000" } },
	{ ItemDefinitionIndexx::WEAPON_MP7,{ "#SFUI_WPNHUD_MP7", "weapon_mp7", "models/weapons/v_smg_mp7.mdl", "mp7" } },
	{ ItemDefinitionIndexx::WEAPON_MP9,{ "#SFUI_WPNHUD_MP9", "weapon_mp9", "models/weapons/v_smg_mp9.mdl", "mp9" } },
	{ ItemDefinitionIndexx::WEAPON_NOVA,{ "#SFUI_WPNHUD_Nova", "weapon_nova", "models/weapons/v_shot_nova.mdl", "nova" } },
	{ ItemDefinitionIndexx::WEAPON_P250,{ "#SFUI_WPNHUD_P250", "weapon_p250", "models/weapons/v_pist_p250.mdl", "p250" } },
	{ ItemDefinitionIndexx::WEAPON_SCAR20,{ "#SFUI_WPNHUD_SCAR20", "weapon_scar20", "models/weapons/v_snip_scar20.mdl", "scar20" } },
	{ ItemDefinitionIndexx::WEAPON_SG553,{ "#SFUI_WPNHUD_SG556", "weapon_sg556", "models/weapons/v_rif_sg556.mdl", "sg556" } },
	{ ItemDefinitionIndexx::WEAPON_SCOUT,{ "#SFUI_WPNHUD_SSG08", "weapon_ssg08", "models/weapons/v_snip_ssg08.mdl", "ssg08" } },
	{ ItemDefinitionIndexx::WEAPON_KNIFE_CT,{ "#SFUI_WPNHUD_Knife", "weapon_knife", "models/weapons/v_knife_default_ct.mdl", "knife_default_ct" } },
	{ ItemDefinitionIndexx::WEAPON_FLASH,{ "#SFUI_WPNHUD_FLASHBANG", "weapon_flashbang", "models/weapons/v_eq_flashbang.mdl", "flashbang" } },
	{ ItemDefinitionIndexx::WEAPON_HE,{ "#SFUI_WPNHUD_HE_Grenade", "weapon_hegrenade", "models/weapons/v_eq_fraggrenade.mdl", "hegrenade" } },
	{ ItemDefinitionIndexx::WEAPON_SMOKE,{ "#SFUI_WPNHUD_Smoke_Grenade", "weapon_smokegrenade", "models/weapons/v_eq_smokegrenade.mdl", "smokegrenade" } },
	{ ItemDefinitionIndexx::WEAPON_MOLOTOV,{ "#SFUI_WPNHUD_MOLOTOV", "weapon_molotov", "models/weapons/v_eq_molotov.mdl", "inferno" } },
	{ ItemDefinitionIndexx::WEAPON_DECOY,{ "#SFUI_WPNHUD_DECOY", "weapon_decoy", "models/weapons/v_eq_decoy.mdl", "decoy" } },
	{ ItemDefinitionIndexx::WEAPON_INC,{ "#SFUI_WPNHUD_IncGrenade", "weapon_incgrenade", "models/weapons/v_eq_incendiarygrenade.mdl", "inferno" } },
	{ ItemDefinitionIndexx::WEAPON_C4,{ "#SFUI_WPNHUD_C4", "weapon_c4", "models/weapons/v_ied.mdl" } },
	{ ItemDefinitionIndexx::WEAPON_KNIFE_T,{ "#SFUI_WPNHUD_Knife", "weapon_knife_t", "models/weapons/v_knife_default_t.mdl", "knife_t" } },
	{ ItemDefinitionIndexx::WEAPON_M4A1S,{ "#SFUI_WPNHUD_M4_SILENCER", "weapon_m4a1_silencer", "models/weapons/v_rif_m4a1_s.mdl", "m4a1_silencer" } },
	{ ItemDefinitionIndexx::WEAPON_USPS,{ "#SFUI_WPNHUD_USP_SILENCER", "weapon_usp_silencer", "models/weapons/v_pist_223.mdl", "usp_silencer" } },
	{ ItemDefinitionIndexx::WEAPON_CZ75,{ "#SFUI_WPNHUD_CZ75", "weapon_cz75a", "models/weapons/v_pist_cz_75.mdl", "cz75a" } },
	{ ItemDefinitionIndexx::WEAPON_REVOLVER,{ "#SFUI_WPNHUD_REVOLVER", "weapon_revolver", "models/weapons/v_pist_revolver.mdl", "revolver" } },
	{ ItemDefinitionIndexx::WEAPON_KNIFE_BAYONET,{ "#SFUI_WPNHUD_KnifeBayonet", "weapon_knife_bayonet", "models/weapons/v_knife_bayonet.mdl", "bayonet" } },
	{ ItemDefinitionIndexx::WEAPON_KNIFE_FLIP,{ "#SFUI_WPNHUD_KnifeFlip", "weapon_knife_flip", "models/weapons/v_knife_flip.mdl", "knife_flip" } },
	{ ItemDefinitionIndexx::WEAPON_KNIFE_GUT,{ "#SFUI_WPNHUD_KnifeGut", "weapon_knife_gut", "models/weapons/v_knife_gut.mdl", "knife_gut" } },
	{ ItemDefinitionIndexx::WEAPON_KNIFE_KARAMBIT,{ "#SFUI_WPNHUD_KnifeKaram", "weapon_knife_karambit", "models/weapons/v_knife_karam.mdl", "knife_karambit" } },
	{ ItemDefinitionIndexx::WEAPON_KNIFE_M9BAYONET,{ "#SFUI_WPNHUD_KnifeM9", "weapon_knife_m9_bayonet", "models/weapons/v_knife_m9_bay.mdl", "knife_m9_bayonet" } },
	{ ItemDefinitionIndexx::WEAPON_KNIFE_HUNTSMAN,{ "#SFUI_WPNHUD_KnifeTactical", "weapon_knife_tactical", "models/weapons/v_knife_tactical.mdl", "knife_tactical" } },
	{ ItemDefinitionIndexx::WEAPON_KNIFE_FALCHION,{ "#SFUI_WPNHUD_knife_falchion_advanced", "weapon_knife_falchion", "models/weapons/v_knife_falchion_advanced.mdl", "knife_falchion" } },
	{ ItemDefinitionIndexx::WEAPON_KNIFE_BOWIE,{ "#SFUI_WPNHUD_knife_survival_bowie", "weapon_knife_survival_bowie", "models/weapons/v_knife_survival_bowie.mdl", "knife_survival_bowie" } },
	{ ItemDefinitionIndexx::WEAPON_KNIFE_BUTTERFLY,{ "#SFUI_WPNHUD_Knife_Butterfly", "weapon_knife_butterfly", "models/weapons/v_knife_butterfly.mdl", "knife_butterfly" } },
	{ ItemDefinitionIndexx::WEAPON_KNIFE_DAGGER,{ "#SFUI_WPNHUD_knife_push", "weapon_knife_push", "models/weapons/v_knife_push.mdl", "knife_push" } },
	{ ItemDefinitionIndexx::GLOVE_STUDDED_BLOODHOUND,{ "#CSGO_Wearable_t_studdedgloves", "studded_bloodhound_gloves", "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl" } },
	{ ItemDefinitionIndexx::GLOVE_T_SIDE,{ "#CSGO_Wearable_t_defaultgloves", "t_gloves", "models/weapons/v_models/arms/glove_fingerless/v_glove_fingerless.mdl" } },
	{ ItemDefinitionIndexx::GLOVE_CT_SIDE,{ "#CSGO_Wearable_ct_defaultgloves", "ct_gloves", "models/weapons/v_models/arms/glove_hardknuckle/v_glove_hardknuckle.mdl" } },
	{ ItemDefinitionIndexx::GLOVE_SPORTY,{ "#CSGO_Wearable_v_sporty_glove", "sporty_gloves", "models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl" } },
	{ ItemDefinitionIndexx::GLOVE_SLICK,{ "#CSGO_Wearable_v_slick_glove", "slick_gloves", "models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl" } },
	{ ItemDefinitionIndexx::GLOVE_LEATHER_WRAP,{ "#CSGO_Wearable_v_leather_handwrap", "leather_handwraps", "models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl" } },
	{ ItemDefinitionIndexx::GLOVE_MOTORCYCLE,{ "#CSGO_Wearable_v_motorcycle_glove", "motorcycle_gloves", "models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl" } },
	{ ItemDefinitionIndexx::GLOVE_SPECIALIST,{ "#CSGO_Wearable_v_specialist_glove", "specialist_gloves", "models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl" } },
    { ItemDefinitionIndexx::GLOVE_HYDRA,{ "#CSGO_Wearable_v_bloodhound_hydra_glove", "bloodhound_hydra_gloves", "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound_hydra.mdl" } }

};

class CBaseAttributableItem
{
public:


	short* GetItemDefinitionIndex()
	{
		// DT_BaseAttributableItem -> m_AttributeManager -> m_Item -> m_iItemDefinitionIndex
		return (short*)((DWORD)this + offsetz.DT_BaseAttributableItem.m_iItemDefinitionIndex);
	}
	int* ItemIDHigh()
	{
		// DT_BaseAttributableItem -> m_AttributeManager -> m_Item -> m_iItemIDHigh
		return (int*)((DWORD)this + offsetz.DT_BaseAttributableItem.m_iItemIDHigh);
	}
	int* ItemIDLow()
	{
		return (int*)((uintptr_t)this + 0x1F4);
	}

	int* FallbackPaintKit()
	{
		return (int*)((uintptr_t)this + offsetz.DT_BaseAttributableItem.m_nFallbackPaintKit);
	}

	int* FallbackSeed()
	{
		return (int*)((uintptr_t)this + offsetz.DT_BaseAttributableItem.m_nFallbackSeed);
	}

	float* FallbackWear()
	{
		return (float*)((uintptr_t)this + offsetz.DT_BaseAttributableItem.m_flFallbackWear);
	}

	int* FallbackStatTrak()
	{
		return (int*)((uintptr_t)this + offsetz.DT_BaseAttributableItem.m_nFallbackStatTrak);
	}

	int* OwnerXuidLow()
	{
		return (int*)((uintptr_t)this + offsetz.DT_BaseAttributableItem.m_OriginalOwnerXuidLow);
	}
	int* OwnerXuidHigh()
	{
		return (int*)((uintptr_t)this + offsetz.DT_BaseAttributableItem.m_OriginalOwnerXuidHigh);
	}
	char* GetCustomName()
	{
		return (char*)((uintptr_t)this + offsetz.DT_BaseAttributableItem.m_szCustomName);
	}
	int* GetEntityQuality() {
		// DT_BaseAttributableItem -> m_AttributeManager -> m_Item -> m_iEntityQuality
		return (int*)((DWORD)this + offsetz.DT_BaseAttributableItem.m_iEntityQuality);

	}

};


typedef CSWeaponInfo& (__thiscall* GetCSWpnDataFn)(void*);
class CBaseViewModel {
public:
	inline int GetModelIndex() {

		return *(int*)((DWORD)this + offsetz.DT_BaseViewModel.m_nModelIndex);
	}

	inline void SetModelIndex(int nModelIndex) {
		*(int*)((DWORD)this + offsetz.DT_BaseViewModel.m_nModelIndex) = nModelIndex;
	}

	inline DWORD GetOwner() {

		return *(PDWORD)((DWORD)this + offsetz.DT_BaseViewModel.m_hOwner);
	}

	inline DWORD GetWeapon() {

		return *(PDWORD)((DWORD)this + offsetz.DT_BaseViewModel.m_hWeapon);
	}
};

enum DataUpdateType_t
{
	DATA_UPDATE_CREATED = 0,
	//	DATA_UPDATE_ENTERED_PVS,
	DATA_UPDATE_DATATABLE_CHANGED,
	//	DATA_UPDATE_LEFT_PVS,
	//	DATA_UPDATE_DESTROYED,
};

class CBaseCombatWeapon : public CBaseAttributableItem
{
public:
	float m_flPostponeFireReadyTime()
	{
		return *(float*)((uintptr_t)this + offsetz.DT_WeaponCSBase.m_flPostponeFireReadyTime);
	}

	HANDLE m_hWeaponWorldModel()
	{
		return *(HANDLE*)((uintptr_t)this + offsetz.DT_BaseCombatWeapon.m_hWeaponWorldModel);
	}
	int ammo()
	{
		return *(int*)((uintptr_t)this + offsetz.DT_BaseCombatWeapon.m_iClip1);
	}
	int ammo2()
	{
		return *(int*)((uintptr_t)this + offsetz.DT_BaseCombatWeapon.m_iReserve);
	}
	int* AccountID()
	{
		return (int*)((uintptr_t)this + offsetz.DT_BaseAttributableItem.m_iAccountID);
	}
	float GetNextPrimaryAttack()
	{
		return *(float*)((uintptr_t)this + offsetz.DT_BaseCombatWeapon.m_flNextPrimaryAttack);
	}

	int GetOwnerHandle()
	{
		return *(int*)((uintptr_t)this + offsetz.DT_BaseCombatWeapon.m_hOwner);
	}

	Vector GetOrigin() // GetVecOrigin
	{
		return *(Vector*)((uintptr_t)this + offsetz.DT_BaseEntity.m_vecOrigin);
	}


	int* ViewModelIndex()
	{
		return (int*)((uintptr_t)this + offsetz.DT_BaseCombatWeapon.m_iViewModelIndex);
	}

	int* WorldModelIndex()
	{
		return (int*)((uintptr_t)this + offsetz.DT_BaseCombatWeapon.m_iWorldModelIndex);
	}

	int* ModelIndex()
	{
		return (int*)((uintptr_t)this + offsetz.DT_BaseViewModel.m_nModelIndex);
	}

	//new
	void SetModelIndex(int model)
	{
		typedef void(__thiscall* oSetModelIndex)(PVOID, int);
		return call_vfunc< oSetModelIndex >(this, 75)(this, model);
	}


	CBaseAttributableItem* GeteAttributableItem()
	{
		return (CBaseAttributableItem*)(DWORD)this;
	}


	float GetInaccuracy() {
		if (!this) return 0;
		typedef float(__thiscall* tFunc)(void*);
		return call_vfunc<tFunc>(this, 467)(this);
	}


	float GetSpread() {
		if (!this) return 0;
		typedef float(__thiscall* tFunc)(void*);
		return call_vfunc<tFunc>(this, 436)(this);
	}


	void UpdateAccuracyPenalty(CBaseCombatWeapon *pWeapon) {
		DWORD dwUpdateVMT = (*reinterpret_cast< PDWORD_PTR* >(pWeapon))[468];
		return ((void(__thiscall*)(CBaseCombatWeapon*)) dwUpdateVMT)(pWeapon);
	}


	CSWeaponInfo* GetCSWpnData()
	{
		if (!this)
			return nullptr;
		typedef CSWeaponInfo*(__thiscall* tGetCSWpnData)(void*);
		return call_vfunc<tGetCSWpnData>(this, 444)(this);
	}

	int	GetWeaponType()
	{
		typedef int(__thiscall* GetWeaponTypeFn)(void*);
		return call_vfunc<GetWeaponTypeFn>(this, 453)(this);
	}

	bool IsInReload()
	{
		return *(bool*)((uintptr_t)this + offsetz.DT_BaseCombatWeapon.m_bInReload);
	}

	void Release()
	{
		PVOID pNetworkable = (PVOID)((DWORD)(this) + 0x8);
		typedef void(__thiscall* OriginalFn)(PVOID);
		return call_vfunc<OriginalFn>(pNetworkable, 1)(pNetworkable);
	}
	void SetDestroyedOnRecreateEntities(void)
	{
		PVOID pNetworkable = (PVOID)((DWORD)(this) + 0x8);
		typedef void(__thiscall* OriginalFn)(PVOID);
		return call_vfunc<OriginalFn>(pNetworkable, 13)(pNetworkable);
	}

	bool IsKnife()
	{
		int iWeaponID = *this->GetItemDefinitionIndex();
		return (iWeaponID == (int)ItemDefinitionIndexx::WEAPON_KNIFE_T || iWeaponID == (int)ItemDefinitionIndexx::WEAPON_KNIFE_CT
			|| iWeaponID == 59 || iWeaponID == 41
			|| iWeaponID == 500 || iWeaponID == 505 || iWeaponID == 506
			|| iWeaponID == 507 || iWeaponID == 508 || iWeaponID == 509
			|| iWeaponID == 515);
	}
	bool IsGrenade()
	{
		int iWeaponID = *this->GetItemDefinitionIndex();
		return (iWeaponID == (int)ItemDefinitionIndexx::WEAPON_HE
			|| (int)ItemDefinitionIndexx::WEAPON_INC
			|| (int)ItemDefinitionIndexx::WEAPON_DECOY
			|| (int)ItemDefinitionIndexx::WEAPON_SMOKE
			|| (int)ItemDefinitionIndexx::WEAPON_FLASH);
	}
	bool AWall()
	{
		int iWeaponID = *this->GetItemDefinitionIndex();
		return (iWeaponID ==
			(int)ItemDefinitionIndexx::WEAPON_AK47
			|| (int)ItemDefinitionIndexx::WEAPON_AUG
			|| (int)ItemDefinitionIndexx::WEAPON_REVOLVER
			|| (int)ItemDefinitionIndexx::WEAPON_BIZON
			|| (int)ItemDefinitionIndexx::WEAPON_CZ75
			|| (int)ItemDefinitionIndexx::WEAPON_DEAGLE
			|| (int)ItemDefinitionIndexx::WEAPON_DUALS
			|| (int)ItemDefinitionIndexx::WEAPON_FAMAS
			|| (int)ItemDefinitionIndexx::WEAPON_FIVE7
			|| (int)ItemDefinitionIndexx::WEAPON_GALIL
			|| (int)ItemDefinitionIndexx::WEAPON_GLOCK
			|| (int)ItemDefinitionIndexx::WEAPON_M249
			|| (int)ItemDefinitionIndexx::WEAPON_M4A1
			|| (int)ItemDefinitionIndexx::WEAPON_MAC10
			|| (int)ItemDefinitionIndexx::WEAPON_MAG7
			|| (int)ItemDefinitionIndexx::WEAPON_MP7
			|| (int)ItemDefinitionIndexx::WEAPON_MP9
			|| (int)ItemDefinitionIndexx::WEAPON_NOVA
			|| (int)ItemDefinitionIndexx::WEAPON_NEGEV
			|| (int)ItemDefinitionIndexx::WEAPON_P250
			|| (int)ItemDefinitionIndexx::WEAPON_P90
			|| (int)ItemDefinitionIndexx::WEAPON_MP5SD
			|| (int)ItemDefinitionIndexx::WEAPON_SAWEDOFF
			|| (int)ItemDefinitionIndexx::WEAPON_SG553
			|| (int)ItemDefinitionIndexx::WEAPON_TEC9
			|| (int)ItemDefinitionIndexx::WEAPON_P2000
			|| (int)ItemDefinitionIndexx::WEAPON_USPS
			|| (int)ItemDefinitionIndexx::WEAPON_UMP45
			|| (int)ItemDefinitionIndexx::WEAPON_XM1014
			|| (int)ItemDefinitionIndexx::WEAPON_TASER
			|| (int)ItemDefinitionIndexx::WEAPON_M4A1S);
	}
	bool IsValidWeapon()
	{
		int iWeaponID = *this->GetItemDefinitionIndex();
		return (iWeaponID ==
			(int)ItemDefinitionIndexx::WEAPON_AK47
			|| (int)ItemDefinitionIndexx::WEAPON_AUG
			|| (int)ItemDefinitionIndexx::WEAPON_REVOLVER
			|| (int)ItemDefinitionIndexx::WEAPON_AWP
			|| (int)ItemDefinitionIndexx::WEAPON_BIZON
			|| (int)ItemDefinitionIndexx::WEAPON_CZ75
			|| (int)ItemDefinitionIndexx::WEAPON_DEAGLE
			|| (int)ItemDefinitionIndexx::WEAPON_DUALS
			|| (int)ItemDefinitionIndexx::WEAPON_FAMAS
			|| (int)ItemDefinitionIndexx::WEAPON_FIVE7
			|| (int)ItemDefinitionIndexx::WEAPON_G3SG1
			|| (int)ItemDefinitionIndexx::WEAPON_GALIL
			|| (int)ItemDefinitionIndexx::WEAPON_GLOCK
			|| (int)ItemDefinitionIndexx::WEAPON_SCOUT
			|| (int)ItemDefinitionIndexx::WEAPON_M249
			|| (int)ItemDefinitionIndexx::WEAPON_M4A1
			|| (int)ItemDefinitionIndexx::WEAPON_MAC10
			|| (int)ItemDefinitionIndexx::WEAPON_MAG7
			|| (int)ItemDefinitionIndexx::WEAPON_MP7
			|| (int)ItemDefinitionIndexx::WEAPON_MP9
			|| (int)ItemDefinitionIndexx::WEAPON_NOVA
			|| (int)ItemDefinitionIndexx::WEAPON_NEGEV
			|| (int)ItemDefinitionIndexx::WEAPON_P250
			|| (int)ItemDefinitionIndexx::WEAPON_P90
			|| (int)ItemDefinitionIndexx::WEAPON_MP5SD
			|| (int)ItemDefinitionIndexx::WEAPON_SAWEDOFF
			|| (int)ItemDefinitionIndexx::WEAPON_SCAR20
			|| (int)ItemDefinitionIndexx::WEAPON_SG553
			|| (int)ItemDefinitionIndexx::WEAPON_TEC9
			|| (int)ItemDefinitionIndexx::WEAPON_P2000
			|| (int)ItemDefinitionIndexx::WEAPON_USPS
			|| (int)ItemDefinitionIndexx::WEAPON_UMP45
			|| (int)ItemDefinitionIndexx::WEAPON_XM1014
			|| (int)ItemDefinitionIndexx::WEAPON_TASER
			|| (int)ItemDefinitionIndexx::WEAPON_M4A1S);
	}
	char* GetGunIcon()
	{
		int WeaponId = *this->GetItemDefinitionIndex();
		switch (WeaponId)
		{
			//case WEAPON_KNIFE:
		case 500:
		case 505:
		case 506:
		case 507:
		case 508:
		case 509:
		case 512:
		case 514:
		case 515:
		case 516:
			return "]";
		case (int)ItemDefinitionIndexx::WEAPON_DEAGLE:
			return "A";
		case (int)ItemDefinitionIndexx::WEAPON_DUALS:
			return "B";
		case (int)ItemDefinitionIndexx::WEAPON_FIVE7:
			return "C";
		case (int)ItemDefinitionIndexx::WEAPON_GLOCK:
			return "D";
		case (int)ItemDefinitionIndexx::WEAPON_P2000:
			return "E";
		case (int)ItemDefinitionIndexx::WEAPON_P250:
			return "F";
		case (int)ItemDefinitionIndexx::WEAPON_USPS:
			return "G";
		case (int)ItemDefinitionIndexx::WEAPON_TEC9:
			return "H";
		case (int)ItemDefinitionIndexx::WEAPON_CZ75:
			return "I";
		case (int)ItemDefinitionIndexx::WEAPON_REVOLVER:
			return "J";
		case (int)ItemDefinitionIndexx::WEAPON_MAC10:
			return "K";
		case (int)ItemDefinitionIndexx::WEAPON_UMP45:
			return "L";
		case (int)ItemDefinitionIndexx::WEAPON_BIZON:
			return "M";
		case (int)ItemDefinitionIndexx::WEAPON_MP7:
			return "N";
		case (int)ItemDefinitionIndexx::WEAPON_MP9:
			return "O";
		case (int)ItemDefinitionIndexx::WEAPON_P90:
			return "P";
		case (int)ItemDefinitionIndexx::WEAPON_GALIL:
			return "Q";
		case (int)ItemDefinitionIndexx::WEAPON_FAMAS:
			return "R";
		case (int)ItemDefinitionIndexx::WEAPON_M4A1S:
			return "S";
		case (int)ItemDefinitionIndexx::WEAPON_M4A1:
			return "T";
		case (int)ItemDefinitionIndexx::WEAPON_AUG:
			return "U";
		case (int)ItemDefinitionIndexx::WEAPON_SG553:
			return "V";
		case (int)ItemDefinitionIndexx::WEAPON_AK47:
			return "W";
		case (int)ItemDefinitionIndexx::WEAPON_G3SG1:
			return "X";
		case (int)ItemDefinitionIndexx::WEAPON_SCAR20:
			return "Y";
		case (int)ItemDefinitionIndexx::WEAPON_AWP:
			return "Z";
		case (int)ItemDefinitionIndexx::WEAPON_SCOUT:
			return "a";
		case (int)ItemDefinitionIndexx::WEAPON_XM1014:
			return "b";
		case (int)ItemDefinitionIndexx::WEAPON_SAWEDOFF:
			return "c";
		case (int)ItemDefinitionIndexx::WEAPON_MAG7:
			return "d";
		case (int)ItemDefinitionIndexx::WEAPON_NOVA:
			return "e";
		case (int)ItemDefinitionIndexx::WEAPON_NEGEV:
			return "f";
		case (int)ItemDefinitionIndexx::WEAPON_M249:
			return "g";
		case (int)ItemDefinitionIndexx::WEAPON_TASER:
			return "h";
		case (int)ItemDefinitionIndexx::WEAPON_FLASH:
			return "i";
		case (int)ItemDefinitionIndexx::WEAPON_HE:
			return "j";
		case (int)ItemDefinitionIndexx::WEAPON_SMOKE:
			return "k";
		case (int)ItemDefinitionIndexx::WEAPON_MOLOTOV:
			return "l";
		case (int)ItemDefinitionIndexx::WEAPON_DECOY:
			return "m";
		case (int)ItemDefinitionIndexx::WEAPON_INC:
			return "n";
		case (int)ItemDefinitionIndexx::WEAPON_C4:
			return "o";
		default:
			return " ";
		}
	}

	char* GetGunName()
	{
		int WeaponId = *this->GetItemDefinitionIndex();
		if (this->IsKnife())
			return "Knife";
		switch (WeaponId)
		{
		case (int)ItemDefinitionIndexx::WEAPON_AK47:
			return "AK47";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_AUG:
			return "AUG";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_REVOLVER:
			return "Revolver";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_AWP:
			return "AWP";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_BIZON:
			return "Bizon";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_C4:
			return "Bomb";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_CZ75:
			return "CZ75";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_DEAGLE:
			return "Deagle";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_DECOY:
			return "Decoy";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_DUALS:
			return "Elites";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_FAMAS:
			return "Famas";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_FIVE7:
			return "FiveSeven";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_FLASH:
			return "Flash";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_G3SG1:
			return "G3SG1";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_GALIL:
			return "Galil";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_GLOCK:
			return "Glock";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_INC:
			return "Molotov";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_MOLOTOV:
			return "Molotov";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_SCOUT:
			return "Scout";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_HE:
			return "Grenade";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_M249:
			return "M249";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_M4A1:
			return "M4A1";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_MAC10:
			return "MAC10";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_MAG7:
			return "MAG7";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_MP7:
			return "MP7";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_MP9:
			return "MP9";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_NOVA:
			return "Nova";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_NEGEV:
			return "Negev";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_P250:
			return "P250";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_P90:
			return "P90";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_SAWEDOFF:
			return "SawedOff";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_SCAR20:
			return "SCAR20";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_SMOKE:
			return "Smoke";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_SG553:
			return "SG556";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_TEC9:
			return "TEC9";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_P2000:
			return "P2000";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_USPS:
			return "USP";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_UMP45:
			return "UMP";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_XM1014:
			return "XM1014";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_TASER:
			return "Zeus";
			break;
		case (int)ItemDefinitionIndexx::WEAPON_M4A1S:
			return "M4A1-S";
		}
		return "";
	}
};

class CGloves : public CBaseCombatWeapon {
public:

	void PreDataUpdate(int updateType)
	{
		PVOID pNetworkable = (PVOID)((DWORD)(this) + 0x8);
		typedef void(__thiscall* OriginalFn)(PVOID, int);
		return call_vfunc<OriginalFn>(pNetworkable, 6)(pNetworkable, updateType);
	}

	void SetGloveModelIndex(int modelIndex)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int);
		call_vfunc<OriginalFn>(this, 75)(this, modelIndex);
	}
};



class CCSBomb
{
public:

	HANDLE GetOwnerHandle()
	{
		return *(HANDLE*)((uintptr_t)this + offsetz.DT_BaseCombatWeapon.m_hOwner);
	}

	float GetC4BlowTime()
	{
		return *(float*)((uintptr_t)this + offsetz.DT_PlantedC4.m_flC4Blow);
	}

	float GetC4DefuseCountDown()
	{
		return *(float*)((uintptr_t)this + offsetz.DT_PlantedC4.m_flDefuseCountDown);
	}

	int GetBombDefuser()
	{
		return *(int*)((uintptr_t)this + offsetz.DT_PlantedC4.m_hBombDefuser);
	}

	bool IsBombDefused()
	{
		return *(bool*)((uintptr_t)this + offsetz.DT_PlantedC4.m_bBombDefused);
	}
};


class CCSGrenade
{
public:

	float GetThrowTime()
	{
		return *(float*)((uintptr_t)this + offsetz.DT_BaseCSGrenade.m_fThrowTime);
	}
};

class CLocalPlayerExclusive
{
public:

	Vector GetViewPunchAngle()
	{
		return *(Vector*)((uintptr_t)this + 0x64);
	}
	Vector GetAimPunchAngle()
	{
		return *(Vector*)((uintptr_t)this + 0x70);
	}
	Vector GetAimPunchAngleVel()
	{
		return *(Vector*)((uintptr_t)this + 0x7C);
	}
};

class CollisionProperty
{
public:

	Vector GetMins()
	{
		return *(Vector*)((uintptr_t)this + offsetz.DT_BaseEntity.m_vecMins);
	}
	Vector GetMaxs()
	{
		return *(Vector*)((uintptr_t)this + offsetz.DT_BaseEntity.m_vecMaxs);
	}

	unsigned char GetSolidType()
	{
		return *(unsigned char*)((uintptr_t)this + offsetz.DT_BaseEntity.m_nSolidType);
	}

	unsigned short GetSolidFlags()
	{
		return *(unsigned short*)((uintptr_t)this + offsetz.DT_BaseEntity.m_usSolidFlags);
	}

	int GetSurroundsType()
	{
		return *(int*)((uintptr_t)this + offsetz.DT_BaseEntity.m_nSurroundType);
	}


	bool IsSolid()
	{
		return (GetSolidType() != SOLID_NONE) && ((GetSolidFlags() & FSOLID_NOT_SOLID) == 0);
	}
};

typedef unsigned short ClientShadowHandle_t;
typedef unsigned short ClientRenderHandle_t;
typedef unsigned short ModelInstanceHandle_t;
typedef unsigned char uint8;

class matrix3x4_t;
class IClientUnknown;
struct model_t;

enum ShadowType_t
{
	SHADOWS_NONE = 0,
	SHADOWS_SIMPLE,
	SHADOWS_RENDER_TO_TEXTURE,
	SHADOWS_RENDER_TO_TEXTURE_DYNAMIC,	// the shadow is always changing state
	SHADOWS_RENDER_TO_DEPTH_TEXTURE,
	SHADOWS_RENDER_TO_TEXTURE_DYNAMIC_CUSTOM,	// changing, and entity uses custom rendering code for shadow
};

class IPVSNotify
{
public:
	virtual void OnPVSStatusChanged(bool bInPVS) = 0;
};

struct RenderableInstance_t
{
	uint8 m_nAlpha;
};

class IClientRenderable {
public:
	virtual IClientUnknown*            GetIClientUnknown() = 0;
	virtual Vector const&              GetRenderOrigin(void) = 0;
	virtual QAngle const&              GetRenderAngles(void) = 0;
	virtual bool                       ShouldDraw(void) = 0;
	virtual int                        GetRenderFlags(void) = 0;
	virtual void                       Unused(void) const {}
	virtual ClientShadowHandle_t       GetShadowHandle() const = 0;
	virtual ClientRenderHandle_t&      RenderHandle() = 0;
	virtual const model_t*             GetModel() const = 0;
	virtual int                        DrawModel(int flags, const int /*RenderableInstance_t*/ &instance) = 0;
	virtual int                        GetBody() = 0;
	virtual void                       GetColorModulation(float* color) = 0;
	virtual bool                       LODTest() = 0;
	virtual bool                       SetupBones(matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime) = 0;
	virtual void                       SetupWeights(const matrix3x4_t *pBoneToWorld, int nFlexWeightCount, float *pFlexWeights, float *pFlexDelayedWeights) = 0;
	virtual void					   DoAnimationEvents(void) = 0;
	virtual IPVSNotify*				   GetPVSNotifyInterface() = 0;
	virtual void                       GetRenderBounds(Vector& mins, Vector& maxs) = 0;
	virtual void                       GetRenderBoundsWorldspace(Vector& mins, Vector& maxs) = 0;
	virtual void                       GetShadowRenderBounds(Vector &mins, Vector &maxs, ShadowType_t shadowType) = 0;
	virtual bool                       ShouldReceiveProjectedTextures(int flags) = 0;
	virtual bool                       GetShadowCastDistance(float *pDist, ShadowType_t shadowType) const = 0;
	virtual bool                       GetShadowCastDirection(Vector *pDirection, ShadowType_t shadowType) const = 0;
	virtual bool                       IsShadowDirty() = 0;
	virtual void                       MarkShadowDirty(bool bDirty) = 0;
	virtual IClientRenderable*         GetShadowParent() = 0;
	virtual IClientRenderable*         FirstShadowChild() = 0;
	virtual IClientRenderable*         NextShadowPeer() = 0;
	virtual ShadowType_t			   ShadowCastType() = 0;
	virtual void                       CreateModelInstance() = 0;
	virtual ModelInstanceHandle_t      GetModelInstance() = 0;
	virtual const matrix3x4_t&         RenderableToWorldTransform() = 0;
	virtual int                        LookupAttachment(const char *pAttachmentName) = 0;
	virtual bool                       GetAttachment(int number, Vector &origin, QAngle &angles) = 0;
	virtual bool                       GetAttachment(int number, matrix3x4_t &matrix) = 0;
	virtual float*                     GetRenderClipPlane(void) = 0;
	virtual int                        GetSkin() = 0;
	virtual void                       OnThreadedDrawSetup() = 0;
	virtual bool                       UsesFlexDelayedWeights() = 0;
	virtual void                       RecordToolMessage() = 0;
	virtual bool                       ShouldDrawForSplitScreenUser(int nSlot) = 0;
	virtual uint8                      OverrideAlphaModulation(uint8 nAlpha) = 0;
	virtual uint8                      OverrideShadowAlphaModulation(uint8 nAlpha) = 0;
};

class IClientUnknown;
class ClientClass;
class bf_read;
class IClientNetworkable {
public:
	virtual IClientUnknown*  GetIClientUnknown() = 0;
	virtual void             Release() = 0;
	virtual ClientClass*     GetClientClass() = 0;
	virtual void             NotifyShouldTransmit(int state) = 0;
	virtual void             OnPreDataChanged(DataUpdateType_t updateType) = 0;
	virtual void             OnDataChanged(DataUpdateType_t updateType) = 0;
	virtual void             PreDataUpdate(DataUpdateType_t updateType) = 0;
	virtual void             PostDataUpdate(DataUpdateType_t updateType) = 0;
	virtual void             unknown(void) = 0;
	virtual bool             IsDormant(void) = 0;
	virtual int              EntIndex(void) const = 0;
	virtual void             ReceiveMessage(int classID, bf_read& msg) = 0;
	virtual void*            GetDataTableBasePtr() = 0;
	virtual void             SetDestroyedOnRecreateEntities(void) = 0;
};

class ICollideable
{
public:
	// Gets at the entity handle associated with the collideable
	virtual C_BaseEntity	*GetEntityHandle() = 0;

	// These methods return the bounds of an OBB measured in "collision" space
	// which can be retreived through the CollisionToWorldTransform or
	// GetCollisionOrigin/GetCollisionAngles methods
	virtual const Vector&	OBBMins() const = 0;
	virtual const Vector&	OBBMaxs() const = 0;

	// Returns the bounds of a world-space box used when the collideable is being traced
	// against as a trigger. It's only valid to call these methods if the solid flags
	// have the FSOLID_USE_TRIGGER_BOUNDS flag set.
	virtual void			WorldSpaceTriggerBounds(Vector *pVecWorldMins, Vector *pVecWorldMaxs) const = 0;

	// custom collision test
	virtual bool			TestCollision(const Ray_t &ray, unsigned int fContentsMask, trace_t& tr) = 0;

	// Perform hitbox test, returns true *if hitboxes were tested at all*!!
	virtual bool			TestHitboxes(const Ray_t &ray, unsigned int fContentsMask, trace_t& tr) = 0;

	// Returns the BRUSH model index if this is a brush model. Otherwise, returns -1.
	virtual int				GetCollisionModelIndex() = 0;

	// Return the model, if it's a studio model.
	virtual const model_t*	GetCollisionModel() = 0;

	// Get angles and origin.
	virtual const Vector&	GetCollisionOrigin() const = 0;
	virtual const QAngle&	GetCollisionAngles() const = 0;
	virtual const matrix3x4_t&	CollisionToWorldTransform() const = 0;

	// Return a SOLID_ define.
	virtual SolidType_t		GetSolid() const = 0;
	virtual int				GetSolidFlags() const = 0;

	// Gets at the containing class...
	virtual IClientUnknown*	GetIClientUnknown() = 0;

	// We can filter out collisions based on collision group
	virtual int				GetCollisionGroup() const = 0;

	// Returns a world-aligned box guaranteed to surround *everything* in the collision representation
	// Note that this will surround hitboxes, trigger bounds, physics.
	// It may or may not be a tight-fitting box and its volume may suddenly change
	virtual void			WorldSpaceSurroundingBounds(Vector *pVecMins, Vector *pVecMaxs) = 0;

	virtual bool			ShouldTouchTrigger(int triggerSolidFlags) const = 0;

	// returns NULL unless this collideable has specified FSOLID_ROOT_PARENT_ALIGNED
	virtual const matrix3x4_t	*GetRootParentToWorldTransform() const = 0;
};
class ICollideable;
class IClientNetworkable;
class IClientRenderable;
class IClientEntity;
class C_BaseEntity;
class IClientThinkable;
class IClientAlphaProperty;
class IClientUnknown
{
public:
	ICollideable* GetCollideable()
	{
		return (ICollideable*)((DWORD)this + 0x318);
	}
	virtual IClientNetworkable*        GetClientNetworkable() = 0;
	virtual IClientRenderable*         GetClientRenderable() = 0;
	virtual IClientEntity*             GetIClientEntity() = 0;
	virtual C_BaseEntity*              GetBaseEntity() = 0;
	virtual IClientThinkable*          GetClientThinkable() = 0;
	//virtual IClientModelRenderable*  GetClientModelRenderable() = 0;
	virtual IClientAlphaProperty*      GetClientAlphaProperty() = 0;
};


class IClientUnknown;
class CClientThinkHandlePtr;
typedef CClientThinkHandlePtr* ClientThinkHandle_t;

class IClientThinkable {
public:
	virtual IClientUnknown*		GetIClientUnknown() = 0;
	virtual void				ClientThink() = 0;
	virtual ClientThinkHandle_t	GetThinkHandle() = 0;
	virtual void				SetThinkHandle(ClientThinkHandle_t hThink) = 0;
	virtual void				Release() = 0;
};

enum MoveType_t
{
	MOVETYPE_NONE = 0,
	MOVETYPE_ISOMETRIC,
	MOVETYPE_WALK,
	MOVETYPE_STEP,
	MOVETYPE_FLY,
	MOVETYPE_FLYGRAVITY,
	MOVETYPE_VPHYSICS,
	MOVETYPE_PUSH,
	MOVETYPE_NOCLIP,
	MOVETYPE_LADDER,
	MOVETYPE_OBSERVER,
	MOVETYPE_CUSTOM,
	MOVETYPE_LAST = MOVETYPE_CUSTOM,
	MOVETYPE_MAX_BITS = 4
};



enum class ObserverMode_t : int
{
	OBS_MODE_NONE = 0,
	OBS_MODE_DEATHCAM = 1,
	OBS_MODE_FREEZECAM = 2,
	OBS_MODE_FIXED = 3,
	OBS_MODE_IN_EYE = 4,
	OBS_MODE_CHASE = 5,
	OBS_MODE_ROAMING = 6
};

enum EntityShape_t : int
{
	SHAPE_CIRCLE,
	SHAPE_SQUARE,
	SHAPE_TRIANGLE,
	SHAPE_TRIANGLE_UPSIDEDOWN
};

class animation_layer
{
public:
	char  pad_0000[20];
	uint32_t m_nOrder; //0x0014
	uint32_t m_nSequence; //0x0018
	float_t m_flPrevCycle; //0x001C
	float_t m_flWeight; //0x0020
	float_t m_flWeightDeltaRate; //0x0024
	float_t m_flPlaybackRate; //0x0028
	float_t m_flCycle; //0x002C
	void *m_pOwner; //0x0030 // player's thisptr
	char  pad_0038[4]; //0x0034
}; //Size: 0x0038


class __declspec (novtable)C_BaseEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable
{
public:
public:
	virtual void			Release(void) = 0;
	virtual void			blahblahpad(void) = 0;
	virtual Vector&	GetAbsOrigin(void) const = 0;
	virtual const Vector&	GetAbsAngles(void) const = 0;
	CBaseCombatWeapon*	m_pWeaponEntity;

	Vector GetAbsOrigin3() {
		__asm {
			MOV ECX, this
			MOV EAX, DWORD PTR DS : [ECX]
			CALL DWORD PTR DS : [EAX + 0x28]
		}
	}

	CUtlVector<animation_layer> GetAnimationOverlay() {
		return (CUtlVector<animation_layer>)((DWORD)this + 0x2970);
	}

	void SetAbsAngles(const QAngle &angles)
	{
		using SetAbsAnglesFn = void(__thiscall*)(void*, const QAngle &angles);
		static SetAbsAnglesFn SetAbsAngles = (SetAbsAnglesFn)CSX::Memory::NewPatternScan(GetModuleHandle("client_panorama.dll"), "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8");
		SetAbsAngles(this, angles);
	}
	CLocalPlayerExclusive* localPlayerExclusive()
	{
		return (CLocalPlayerExclusive*)((uintptr_t)this + 0x2FAC);
	}
	CollisionProperty* collisionProperty()
	{
		return (CollisionProperty*)((uintptr_t)this + 0x318);
	}
	void SetModelIndex(int model)
	{
		typedef void(__thiscall* oSetModelIndex)(PVOID, int);
		return call_vfunc< oSetModelIndex >(this, 75)(this, model);
	}
	float GetSimulationTime()
	{
		return *(float*)((uintptr_t)this + offsetz.DT_BaseEntity.m_flSimulationTime);
	}

	float GetAnimTime()
	{
		return *(float*)((uintptr_t)this + offsetz.DT_BaseEntity.m_flAnimTime);
	}

	IClientRenderable* GetRenderable()
	{
		return reinterpret_cast<IClientRenderable*>((DWORD)this + 0x4);
	}

	HANDLE m_hViewModel()
	{
		return *(HANDLE*)((uintptr_t)this + offsetz.DT_BasePlayer.m_hViewModel);
	}

	int draw_model(int flags, uint8_t alpha) {
		using fn = int(__thiscall*)(void*, int, uint8_t);
		return call_vfunc< fn >(GetRenderable(), 9)(GetRenderable(), flags, alpha);
	}
	void SetAngle2(Vector wantedang) {
		typedef void(__thiscall* oSetAngle)(void*, const Vector &);
		static oSetAngle _SetAngle = (oSetAngle)((uintptr_t)CSX::Memory::FindPatternV2("client_panorama.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1"));
		_SetAngle(this, wantedang);
	}

	int m_iShotsFired()
	{
		static int meme = NetVarManager->GetOffset("DT_CSPlayer", "m_iShotsFired");
		return *(int*)((uintptr_t)this + meme);
	}
	float GetNextAttack()
	{
		return *(float*)((DWORD)this + 0x2D60);
	}
	Vector GetAbsOrigin2()
	{
		return *(Vector*)((DWORD)this + 0x84 + 0x1C);
	}
	int GetHitboxSet()
	{
		return *(int*)((DWORD)this + offsetz.DT_BaseAnimating.m_nHitboxSet);
	}

	bool hasDefuser()
	{
		return *(bool*)((DWORD)this + offsetz.DT_CSPlayer.m_bHasDefuser);
	}
	bool m_bHasHeavyArmor()
	{
		return *(bool*)((DWORD)this + offsetz.DT_CSPlayer.m_bHasHeavyArmor);
	}
	int GetFlags()
	{
		return *(int*)((uintptr_t)this + offsetz.DT_BasePlayer.m_fFlags);
	}

	Vector GetOrigin()
	{
		return *(Vector*)((uintptr_t)this + offsetz.DT_BaseEntity.m_vecOrigin);
	}



	int GetOwnerHandle()
	{
		return *(int*)((uintptr_t)this + offsetz.DT_BaseEntity.m_hOwner);
	}

	Vector GetRotation()
	{
		return *(Vector*)((uintptr_t)this + offsetz.DT_BaseEntity.m_angRotation);
	}

	int GetTeamNum()
	{
		return *(int*)((uintptr_t)this + offsetz.DT_BaseEntity.m_iTeamNum);
	}

	int GetMaxHealth()
	{
		return *(int*)((uintptr_t)this + offsetz.DT_BasePlayer.m_iMaxHealth);
	}

	bool IsValid()
	{
		return (!IsDead() && GetHealth() > 0 && !IsDormant());
	}

	bool IsDead()
	{
		BYTE LifeState = *(PBYTE)((DWORD)this + offsetz.DT_BasePlayer.m_lifeState);
		return (LifeState != LIFE_ALIVE);
	}
	int GetHealth()
	{
		return *(int*)((uintptr_t)this + offsetz.DT_BasePlayer.m_iHealth);
	}

	unsigned char GetLifeState()
	{
		return *(unsigned char*)((uintptr_t)this + offsetz.DT_BasePlayer.m_lifeState);
	}

	char* GetCallout()
	{
		return (char*)((uintptr_t)this + offsetz.DT_BasePlayer.m_szLastPlaceName);
	}

	HANDLE GetActiveWeaponHandle()
	{
		return *(HANDLE*)((uintptr_t)this + offsetz.DT_BaseCombatCharacter.m_hActiveWeapon);
	}

	UINT* GetWeapons()
	{
		// DT_BasePlayer -> m_hMyWeapons
		return (UINT*)((DWORD)this + offsetz.DT_BaseCombatCharacter.m_hMyWeapons);
	}

	int GetTickBase()
	{
		return *(int*)((uintptr_t)this + offsetz.DT_BasePlayer.m_nTickBase);
	}

	Vector GetViewOffset()
	{
		return *(Vector*)((uintptr_t)this + offsetz.DT_BasePlayer.m_vecViewOffset);
	}

	Vector GetVelocity()
	{
		return *(Vector*)((uintptr_t)this + offsetz.DT_BasePlayer.m_vecVelocity);
	}

	bool HasGunGameImmunity()
	{
		return *(bool*)((uintptr_t)this + offsetz.DT_CSPlayer.m_bGunGameImmunity);
	}

	bool IsDefusing()
	{
		return *(bool*)((uintptr_t)this + offsetz.DT_CSPlayer.m_bIsDefusing);
	}

	int ArmorValue()
	{
		return *(int*)((uintptr_t)this + offsetz.DT_CSPlayer.m_ArmorValue);
	}

	int iAccount()
	{
		return *(int*)((uintptr_t)this + offsetz.DT_CSPlayer.m_iAccount);
	}

	char* GetArmorName()
	{
		if (ArmorValue() > 0)
		{
			if (HasHelmet())
				return "K+H";
			else
				return "K";
		}
		else
			return "";
	}

	char* GetArmorIcon()
	{
		if (ArmorValue() > 0)
		{
			if (HasHelmet())
				return "q";
			else
				return "p";
		}
		else
			return "";
	}

	bool HasHelmet()
	{
		return *(bool*)((uintptr_t)this + offsetz.DT_CSPlayer.m_bHasHelmet);
	}

	bool IsScoped()
	{
		return *(bool*)((uintptr_t)this + offsetz.DT_CSPlayer.m_bIsScoped);
	}

	float GetFlashDuration() {
		return *(float*)((DWORD)this + offsetz.DT_CSPlayer.m_flFlashDuration);
	}

	bool IsFlashed()
	{
		return GetFlashDuration() > 0;
	}

	int GetMoney()
	{
		return *(int*)((uintptr_t)this + offsetz.DT_CSPlayer.m_iAccount);
	}

	HANDLE GetObserverTargetHandle()
	{
		return *(HANDLE*)((uintptr_t)this + offsetz.DT_BasePlayer.m_hObserverTarget);
	}






	void* GetObserverTarget()
	{
		return (void*)((uintptr_t)this + offsetz.DT_BasePlayer.m_hObserverTarget);
	}

	ObserverMode_t* GetObserverMode()
	{
		return (ObserverMode_t*)((uintptr_t)this + offsetz.DT_BasePlayer.m_iObserverMode);
	}

	UINT* GetWearables()
	{
		return (UINT*)((DWORD)this + offsetz.DT_BaseCombatCharacter.m_hMyWearables);
	}
	int GetMoveType() {
		return *(int*)((DWORD)this + offsetz.DT_BaseEntity.m_MoveType);
	}

	float GetLowerBodyYaw() {
		return *(float*)((DWORD)this + offsetz.DT_CSPlayer.m_flLowerBodyYawTarget);
	}

	QAngle* GetEyeAngles()
	{
		return (QAngle*)((uintptr_t)this + offsetz.DT_CSPlayer.m_angEyeAngles[0]);
	}

	bool IsAlive()
	{
		return (GetLifeState() == LIFE_ALIVE && GetHealth() > 0);
	}

	Vector WorldSpaceCenter() {
		Vector Max = collisionProperty()->GetMaxs() + this->GetOrigin();
		Vector Min = collisionProperty()->GetMins() + this->GetOrigin();
		Vector Size = Max - Min;
		Size /= 2;
		Size += Min;
		return Size;
	}

	bool IsPlayer()
	{
		typedef bool(__thiscall *IsPlayer_t)(PVOID);
		return ((IsPlayer_t)(*(PDWORD)(*(PDWORD)(this) + 0x260)))(this);
	}
	Vector GetBonePos(int i)
	{
		matrix3x4_t boneMatrix[128];
		if (this->SetupBones(boneMatrix, 128, BONE_USED_BY_HITBOX, static_cast<float>(GetTickCount64())))
		{
			return Vector(boneMatrix[i][0][3], boneMatrix[i][1][3], boneMatrix[i][2][3]);
		}
		return Vector(0, 0, 0);
	}

	Vector GetHeadPos()
	{
		return this->GetBonePos(8);
	}
	Vector GetEyePosition()
	{
		return GetRenderOrigin() + GetViewOffset();
	}

};



class CPlayerResource {};

class C_CSPlayerResource : public CPlayerResource
{
public:
	int GetPing(int index)
	{
		return *(int*)((uintptr_t)this + offsetz.DT_PlayerResource.m_iPing + index * 4);
	}

	int GetKills(int index)
	{
		return *(int*)((uintptr_t)this + offsetz.DT_PlayerResource.m_iKills + index * 4);
	}

	int GetAssists(int index)
	{
		return *(int*)((uintptr_t)this + offsetz.DT_PlayerResource.m_iAssists + index * 4);
	}

	int GetDeaths(int index)
	{
		return *(int*)((uintptr_t)this + offsetz.DT_PlayerResource.m_iDeaths + index * 4);
	}

	bool GetConnected(int index)
	{
		return *(bool*)((uintptr_t)this + offsetz.DT_PlayerResource.m_bConnected + index);
	}

	TeamID GetTeam(int index)
	{
		return *(TeamID*)((uintptr_t)this + offsetz.DT_PlayerResource.m_iTeam + index * 4);
	}

	int GetPendingTeam(int index)
	{
		return *(int*)((uintptr_t)this + offsetz.DT_PlayerResource.m_iPendingTeam + index * 4);
	}

	bool GetAlive(int index)
	{
		return *(bool*)((uintptr_t)this + offsetz.DT_PlayerResource.m_bAlive + index);
	}

	int GetHealth(int index)
	{
		return *(int*)((uintptr_t)this + offsetz.DT_PlayerResource.m_iHealth + index * 4);
	}

	int GetPlayerC4()
	{
		return *(int*)((uintptr_t)this + offsetz.DT_CSPlayerResource.m_iPlayerC4);
	}

	int GetMVPs(int index)
	{
		return *(int*)((uintptr_t)this + offsetz.DT_CSPlayerResource.m_iMVPs + index * 4);
	}

	int GetArmor(int index)
	{
		return *(int*)((uintptr_t)this + offsetz.DT_CSPlayerResource.m_iArmor + index * 4);
	}

	int GetScore(int index)
	{
		return *(int*)((uintptr_t)this + offsetz.DT_CSPlayerResource.m_iScore + index * 4);
	}

	int* GetCompetitiveRanking(int index)
	{
		return (int*)((uintptr_t)this + offsetz.DT_CSPlayerResource.m_iCompetitiveRanking + index * 4);
	}

	int* GetCompetitiveWins(int index)
	{
		return (int*)((uintptr_t)this + offsetz.DT_CSPlayerResource.m_iCompetitiveWins + index * 4);
	}

	int GetCompTeammateColor(int index)
	{
		return *(int*)((uintptr_t)this + offsetz.DT_CSPlayerResource.m_iCompTeammateColor + index * 4);
	}

	const char* GetClan(int index)
	{
		return (const char*)((uintptr_t)this + offsetz.DT_CSPlayerResource.m_szClan + index * 16);
	}

	int* GetActiveCoinRank(int index)
	{
		return (int*)((uintptr_t)this + offsetz.DT_CSPlayerResource.m_nActiveCoinRank + index * 4);
	}

	int* GetMusicID(int index)
	{
		return (int*)((uintptr_t)this + offsetz.DT_CSPlayerResource.m_nMusicID + index * 4);
	}

	int* GetPersonaDataPublicCommendsLeader(int index)
	{
		return (int*)((uintptr_t)this + offsetz.DT_CSPlayerResource.m_nPersonaDataPublicCommendsLeader + index * 4);
	}

	int* GetPersonaDataPublicCommendsTeacher(int index)
	{
		return (int*)((uintptr_t)this + offsetz.DT_CSPlayerResource.m_nPersonaDataPublicCommendsTeacher + index * 4);
	}

	int* GetPersonaDataPublicCommendsFriendly(int index)
	{
		return (int*)((uintptr_t)this + offsetz.DT_CSPlayerResource.m_nPersonaDataPublicCommendsFriendly + index * 4);
	}
};

class C_CSGameRules
{
public:
	bool IsValveDS()
	{
		return *(bool*)((uintptr_t)this + offsetz.DT_CSGameRulesProxy.m_bIsValveDS);
	}

	bool IsBombDropped()
	{
		return *(bool*)((uintptr_t)this + offsetz.DT_CSGameRulesProxy.m_bBombDropped);
	}

	bool IsBombPlanted()
	{
		return *(bool*)((uintptr_t)this + offsetz.DT_CSGameRulesProxy.m_bBombPlanted);
	}

	bool IsFreezeTime()
	{
		return *(bool*)((uintptr_t)this + offsetz.DT_CSGameRulesProxy.m_bFreezePeriod);
	}
};

class IBaseInterface
{
public:
	virtual	~IBaseInterface() {}
};

class IGameConsole : public IBaseInterface
{
public:
	// activates the console, makes it visible and brings it to the foreground
	virtual void Activate() = 0;

	virtual void Initialize() = 0;

	// hides the console
	virtual void Hide() = 0;

	// clears the console
	virtual void Clear() = 0;

	// return true if the console has focus
	virtual bool IsConsoleVisible() = 0;

	virtual void SetParent(int parent) = 0;
};