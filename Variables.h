#pragma once
#include <set>
#include <map>
#include <unordered_map>
#define STICKERS_COUNT 5

extern void RenderInterface();
extern void RenderDrow();

struct skinInfo
{
	int seed = -1;
	int paintkit;
	std::string tagName;
};

struct WeaponCFG {
	int iItemDefinitionIndex = 0;
	int nFallbackPaintKit = 0;
	int rarity = 0;
	int  flFallbackSeed = 0;
	float flFallbackWear = 0;
	int nFallbackStatTrak = -1;
	int iEntityQuality = 0;
	char ChangerName[32] = "";

};

//Color Feature for Color Changer
struct ColorP
{
public:
	const char* Name;
	float* Ccolor;

	ColorP(const char* name, float* color)
	{
		this->Name = name;
		this->Ccolor = color;
	}
};

struct Variables
{
	Variables()
	{

	}

	struct Ragebot_s
	{
		bool MainSwitch = false;
		bool Enabled = false;
		bool AutoFire = false;
		int TargetSelection = 0;
		float FOV = 0.f;
		bool Silent = false;
		bool AutoPistol = false;
		int KeyPress = 0;
		bool AimStep = false;
		bool EnabledAntiAim = false;
		bool PreAAs = false;
		int	Pitch = 0;
		int	YawTrue = 0;
		int	YawFake = 0;
		float YawTrueAdder = 0.f;
		float YawFakeAdder = 0.f;
		float PitchAdder = 0.f;
		bool AtTarget = false;
		bool Edge = false;
		bool KnifeAA = false;

		bool BuilderAAs = false;
		bool Jitter = false;
		bool FJitter = false;
		bool LBYBreaker = false;
		float BuilderReal = 0.f;
		float BuilderFake = 0.f;
		float BuilderPitch = 0.f;
		float JitterRange = 0.f;
		float FJitterRange = 0.f;
		int flipkey = 0;

		bool FriendlyFire = false;
		int	Hitbox = 0;
		int	Hitscan = 0;
		float Pointscalex = 5.f;
		float Pointscaley = 5.f;
		bool Multipoint = false;
		bool autowall = false;
		bool AimbotAimStep = false;
		bool AntiRecoil = false;
		bool HitScanMode = false;
		bool AutoStop = false;
		bool AutoCrouch = false;
		bool AutoScope = false;
		float MinimumDamageSniper = 0.10;
		float MinimumDamageRifle = 0.10;
		float MinimumDamagePistol = 0.10;
		float MinimumDamageHeavy = 0.10;
		float MinimumDamageSmg = 0.10;
		float MinimumDamageRevolver = 0.10;
		float HitchanceSniper = 0;
		float HitchancePistol = 0;
		float HitchanceRifle = 0;
		float HitchanceHeavy = 0;
		float HitchanceSmgs = 0;
		float HitchanceRevolver = 0;
		bool AIFakeLag = false;
		int	AIFakeLagAmt = 1;
		int FakeLagType = 0;
		bool Hitchance = false;
		bool Resolver = false;
		int Resolver3 = 0;
		bool FakeLagFix = false;
		bool PosAdjust = false;
		bool playerlist = false;
		int BAIMkey = 0;
		float bruteAfterX = 0.f;
		bool AntiSmac = false;
		bool hitscaninvis = false;
		bool RageDraw = false;
		bool LBYCorrection = false;
		int AimbotResolver = 0;
		int SomeShit = 0;
		bool AntiSpread = false;
		bool LBYCorrection2 = false;
		bool	FakeLag = false;
		int		FakeLagAmt = 0;
		int		FakeLagMode = 0;
		bool NextMovingLBYUpdate = false;
		bool NextLBYUpdate = false;
		bool Zeus = false;
		bool FakeLagAA = false;
		int FakeLagAmtAA = 0;
	} Ragebot;

	struct
	{
		bool MainSwitch = false;
		bool ManualAA = false;
		bool TriggerbotSwitch = false;
		bool backtrack = false;
		bool checksmoke = false;
		float backtrackTicks = 0.f;
		float backtrack_size = 0.f;
		struct
		{
			bool EnableLegitAA = false;
			bool ALegitAA = false;
			bool Enabled = false;
			bool AutoPistol = false;
			bool AimSniper = false;
			bool fastzoom = false;
			bool fastzoomswitch = false;
			float AAAngle = 0.f;

		} Aimbot;

		int hitboxheavy = 0;
		int heavy_fakelag_mode = 0;
		float heavy_fakelag_value = 0;
		int heavyKey = 6;
		int  heavy_TriggerbotKey = 6;
		float heavy_TriggerbotDelay = 1.f;
		float heavy_Smooth = 1.f;
		float heavyfov = 1.f;
		float heavy_rcs = 1.f;
		int heavy_RcsType = 0;
		float heavy_Triggerbot_hitchance = 1.f;
		bool heavy_Triggerbot_AutoFire = false;
		bool heavy_Triggerbot = false;
		bool heavy_AutoLock = false;
		bool heavy_Silent = false;
		bool heavy_fakelag = false;
		bool heavy_drawfov = false;

		struct
		{
			bool Head = false;
			bool Arms = false;
			bool Chest = false;
			bool Stomach = false;
			bool Legs = false;
		} heavy_TriggerbotFilter;

		int hitboxsmg = 0;
		int smg_fakelag_mode = 0;
		float smg_fakelag_value = 0;
		int smgKey = 6;
		int  smg_TriggerbotKey = 6;
		float smg_TriggerbotDelay = 1.f;
		float smg_Smooth = 1.f;
		float smgfov = 1.f;
		float smg_rcs = 1.f;
		int smg_RcsType = 0;
		float smg_Triggerbot_hitchance = 1.f;
		bool smg_Triggerbot_AutoFire = false;
		bool smg_Triggerbot = false;
		bool smg_AutoLock = false;
		bool smg_Silent = false;
		bool smg_fakelag = false;
		bool smg_drawfov = false;

		struct
		{
			bool Head = false;
			bool Arms = false;
			bool Chest = false;
			bool Stomach = false;
			bool Legs = false;
		} smg_TriggerbotFilter;

		int hitboxrifle = 0;
		int main_fakelag_mode = 0;
		float main_fakelag_value = 0;
		int MainKey = 6;
		int  main_TriggerbotKey = 6;
		float main_TriggerbotDelay = 1.f;
		float Main_Smooth = 1.f;
		float Mainfov = 1.f;
		float main_rcs = 1.f;
		int main_RcsType = 0;
		float main_Triggerbot_hitchance = 1.f;
		bool main_Triggerbot_AutoFire = false;
		bool main_Triggerbot = false;
		bool main_AutoLock = false;
		bool main_Silent = false;
		bool main_fakelag = false;
		bool main_drawfov = false;

		struct
		{
			bool Head = false;
			bool Arms = false;
			bool Chest = false;
			bool Stomach = false;
			bool Legs = false;
		} main_TriggerbotFilter;

		int hitboxpistol = 0;
		int Pistol_fakelag_mode = 0;
		float Pistol_fakelag_value = 0;
		int PistolKey = 6;
		int  Pistol_TriggerbotKey = 6;
		float Pistol_TriggerbotDelay = 1.f;
		float Pistolfov = 1.f;
		float Pistol_Smooth = 1.f;
		float Pistol_rcs = 1.f;
		int Pistol_RcsType = 0;
		float Pistol_Triggerbot_hitchance = 1.f;
		bool Pistol_Triggerbot_AutoFire = false;
		bool Pistol_Triggerbot = false;
		bool Pistol_AutoLock = false;
		bool Pistol_Silent = false;
		bool Pistol_fakelag = false;
		bool Pistol_drawfov = false;

		struct
		{
			bool Head = false;
			bool Arms = false;
			bool Chest = false;
			bool Stomach = false;
			bool Legs = false;
		} Pistol_TriggerbotFilter;

		int hitboxsniper = 0;
		int  Sniper_fakelag_mode = 0;
		float  Sniper_fakelag_value = 0;
		int SniperKey = 6;
		int  sniper_TriggerbotKey = 6;
		float sniper_TriggerbotDelay = 1.f;
		float Sniperfov = 1.f;
		float Sniper_Smooth = 1.f;
		float sniper_rcs = 1.f;
		int sniper_RcsType = 0;
		float sniper_Triggerbot_hitchance = 1;
		bool sniper_Triggerbot_AutoFire = false;
		bool sniper_Triggerbot = false;
		bool sniper_AutoLock = false;
		bool sniper_Silent = false;
		bool Sniper_fakelag = false;
		bool Sniper_drawfov = false;
		struct
		{
			bool Head = false;
			bool Arms = false;
			bool Chest = false;
			bool Stomach = false;
			bool Legs = false;
		} sniper_TriggerbotFilter;


		int hitboxRevolver = 0;
		int  Revolver_fakelag_mode = 0;
		float  Revolver_fakelag_value = 0;
		int RevolverKey = 6;
		int  Revolver_TriggerbotKey = 6;
		float Revolver_TriggerbotDelay = 1.f;
		float Revolver_Smooth = 1.f;
		float Revolverfov = 1.f;
		float Revolver_rcs = 1.f;
		int Revolver_RcsType = 0;
		float Revolver_Triggerbot_hitchance = 1.f;
		bool Revolver_Triggerbot_AutoFire = false;
		bool Revolver_Triggerbot = false;
		bool Revolver_AutoLock = false;
		bool Revolver_Silent = false;
		bool Revolver_fakelag = false;
		bool Revolver_drawfov = false;

		struct
		{
			bool Head = false;
			bool Arms = false;
			bool Chest = false;
			bool Stomach = false;
			bool Legs = false;
		} Revolver_TriggerbotFilter;

	} Legitbot;


	struct
	{
		bool 	Enabled;

		bool Box = false;
		bool fill = false;
		bool AimSpot = false;
		bool DLight = false;
		bool glowplayer = false;
		int Weapons = 0;
		bool LBYIndicator = false;
		bool Chams = false;
		bool Teamchams = false;
		bool XQZ = false;
		bool weaponglow = false;
		bool teamglow = false;
		bool bombglow = false;
		int BoxType = 0;
		bool TeamESP = false;
		bool angleLines = false;
		bool DrawAwall = false;
		bool AimLine = false;
		bool health = false;
		bool barrel = false;
		int barrelL = 0;
		int Playermatierial = 0;
		int Weaponsdroppedmatierial = 0;
		bool WeaponsDroppedChams = false;
		bool GrenadePrediction = false;
		int esp_Visible = 1;
		bool corpses = false;
		float esp_fill_amount = 0.f;
		int healthtype = 0;
		bool bulletshow = false;
		float flTracersDuration = 0.f;
		float flTracersWidth = 0.f;
		bool chamsHp = false;
		bool esp_player_chams_fakeangle = false;
		bool SniperCrosshair = false;
		int SniperCrosshairType = 0;
		bool AWallCrosshair =false;

		bool CrosshairOn = false;
		bool CrosshairSpread = false;
		bool CrosshairType = false;
		int	CrosshairStyle = 0;
		bool CrosshairRainbow = false;

		bool Hitbox;
		float HitboxDuration;
		bool GrenadeBox;
		int DroppedGunsType;
		int Grenades;
		bool Chicken = false;
		bool Hostage = false;
		bool DroppedGuns = false;
		bool C4World = false;
		int OffscreenIndicatorSize = 0;
		bool OffscreenIndicator = false;
		bool skeletonenbl;
		int skeletonopts;
		int Weapon;
		bool resolveMode;
		bool armor;
		int Armor2;
		bool Money;
		bool Name;
		//Info Enemy
		bool BombCarrier;
		bool Flashed;
		bool Distance;
		bool Scoped;
		bool IsHasDefuser;
		bool IsDefusing;
		bool Callout;
		bool IsReloading;

	} Visuals;

	struct
	{
		bool antiuntrusted = true;
		float FOVChanger = 0.f;
		float viewmodelChanger = 75;
		bool NoFlash = false;
		bool NoSmoke = false;
		int Hands = 0;
		bool ThirdPerson = false;
		bool Time = false;
		bool noscopeborder = false;
		bool Hitmarker = false;
		bool silentstealer = false;
		bool hidename = false;
		bool Bhop = false;
		bool AutoStrafe = false;
		bool Ranks = false;
		float MinVel = 0;
		int	AirStuckKey = 0;
		int lagexploit = 0;
		int lagexploitmultiplier = 3;
		float lagexploitvalue = 0.f;
		int NameSpammer = 0;
		int	ChatSpamMode = 0;
		int syncclantag = 0;
		float NRCS = 0.f;
		float RCS_NDistortion = 0.f;
		bool RCS_Distortion = false;
		bool SpecList = false;
		bool namespam = false;
		int spammer = 0;
		bool AutoAccept = false;
		bool SpoofConfirmation = false;
		bool animatedclantag = false;
		int namespeed = 100;
		bool Crosshair = false;
		bool RecoilCrosshair = false;
		int SkyShit = 0;
		bool NoSky = false;
		int AirlockKey = 0;
		bool RCS = false;
		bool ASUSWALL = false;
		int handmat = 0;
		int weaponhandmat = 0;
		bool Weaponslog = false;
		bool DMGlog = false;
		int TPKey = 0;
		const char* misc_SkyName;
		bool Snow = false;
		bool postprocess = false;
		bool HandMat = false;
		bool WeaponsHandMat = false;
		bool Crosshair2 = false;
		bool isReplaying = false;
		bool isRecording = false;
		bool NightMode = false;
		int Hitsound = 0;
		int Edgekey = 0;
		bool Edgjump = false;
		bool fakewalk = false;
		int fakewalkkey= 0;
		int slowmovekey = 0;
		bool Watermark = true;
		int flashlight = 0;
		bool lsdmode = false;
		bool chromemode = false;
		bool minecraftmode = false;
		bool ambientlight = false;
		bool afkbot = false;
		bool bFreeCam = false;
		int iFreeCamKey = 0;
		float flFreeCamSpeed = 0.f;
		bool bBlockbot = false;
		int iBlockbotKey = 0;
		int customodelst = 0;
		int customodelsct = 0;
		bool moonwalk = false;
		int moonwalkkey = 0;

		bool	RemovalsVisualRecoil;

	} Misc;

	struct
	{
		bool radaringame;
		bool radarwindow;
		int radrsize = 200;
		float radralpha = 1.f;
		float radrzoom = 2.f;


	}Radar;


	struct
	{

		struct
		{
			int   ID;
			float Wear;
			int   Seed;

			struct
			{
				bool Enabled;

				int  Kills;
			} StatTrak;

			struct
			{
				int ID;
			} Sticker[5];
		} Weapons[517];
	}Sticker;



	struct
	{
		bool Enabled = false;
		bool EnabledSticker = false;
		int knifemodel = 0;
		int glove = 0;
		int gloveskin = 0;

		WeaponCFG* W = new WeaponCFG[520];

		std::unordered_map<std::string, std::set<std::string>> weaponSkins;
		std::unordered_map<std::string, skinInfo> skinMap;
		std::unordered_map<std::string, std::string> skinNames;
		std::vector<std::string> weaponNames;
	} Skinchanger;

	struct
	{
		bool knf_Active = false;
		bool knf_Team = false;
		int knf_Attack = 2;
		int knf_DistAttack = 72;
		int knf_DistAttack2 = 64;
	}Knifebot;
	struct
	{
		bool	Opened = false;
		float	flAlpha;
		float	flTabPos;
		int   flSubTabsAlpha;
		int   flTabsAlpha;
		bool    bShowSubTabs;
		int		iTab = -1;
		int		iLegitSubTab;
		int		iRageSubTab;
		int		iVisualsSubTab;
		int		iSettingsSubTab;
		int		iColorsSubTab;
		int		iMiscSubTab;
		int		iSkinSubTab;
		int      skinstap;
		bool    bSeeSubTabs;

		int 	Key = 0;
		int		rageSubtab = 0;
		int		ConfigFile = 0;
		int		Theme = 9;
		bool	bShowTabs = false;
		int currentWeapon = 0;
	} Menu;
	struct
	{
		int champlayeralpha = 0;
		int chamhandalpha = 0;
		int weaponhandalpha = 0;
		int weaponglowalpha = 0;
		int playerglowalpha = 0;
		int bombglowalpha = 0;
		int weaponsworldalph = 0;
		int weponchamsalph = 0;
		int rad_Alpha = 255;

		float worldmode[3] = { 0.f, 0.f, 0.f };
		float rad_Color_CT[3] = { 0.f, 0.f, 0.f };
		float rad_Color_TT[3] = { 0.f, 0.f, 0.f };
		float rad_Color_VCT[3] = { 0.f, 0.f, 0.f };
		float rad_Color_VTT[3] = { 0.f, 0.f, 0.f };

		float esp_Ambient[3] = { 0.f, 0.f, 0.f };
		float backtrack[3] = { 0.f, 0.f, 0.f };
		float Crosshair[3] = { 0.f, 0.f, 0.f };
		float Scope[3] = { 0.f, 0.f, 0.f };
		float CrsSpread[3] = { 0.f, 0.f, 0.f };
		float color_recoil[3] = { 0, 0, 1.f };
		float hitmarker_color[3] = { 1.f, 1.f, 1.f };

		float Light_CT[3] = { 0.f, 0.f, 0.f };
		float Light_TT[3] = { 0.f, 0.f, 0.f };

		float TeamSkeleton[3] = { 0.f, 0.f, 0.f };
		float EnemySkeleton[3] = { 0.f, 0.f, 0.f };


		float TeamSkeletonINVIS[3] = { 0.f, 0.f, 0.f };
		float EnemySkeletonInVIS[3] = { 0.f, 0.f, 0.f };

		float fill_color_enemy[3] = { 1.f, 1.f, 0.0f };
		float fill_color_team[3] = { 0.f, 0.2f, 1.f };

		float esp_Color_CT[3] = { 0.f,0.0f,0.f };
		float esp_Color_TT[3] = { 0.f,0.0f,0.f };
		float esp_Color_VCT[3] = { 0.f,0.0f,0.f };
		float esp_Color_VTT[3] = { 0.f,0.0f,0.f };

		float flTracers[3] = { 0.f,0.0f,0.f };


		float EnemyGlow[3] = { 0.f, 0.f, 0.f };
		float TeamGlow[3] = { 0.f, 0.f, 0.f };
		float WeaponGlow[3] = { 0.f, 0.f, 0.f };
		float C4glow[3] = { 0.f, 0.f, 0.f };

		float EnemyChamsVis[3] = { 0.f, 0.f, 0.f };
		float EnemyChamsNVis[3] = { 0.f, 0.f, 0.f };
		float TeamChamsVis[3] = { 0.f, 0.f, 0.f };
		float TeamChamsNVis[3] = { 0.f, 0.f, 0.f };
		float WeaponDropped[3] = { 0.f, 0.f, 0.f };
		float HandCham[3] = { 0.f, 0.f, 0.f };
		float WeaponHandCham[3] = { 0.f, 0.f, 0.f };
		float esp_player_fakechams_color[3] = { 0.f, 0.f, 0.f };
		float color_grenadeprediction[3] = { 1.f, 1.f, 1.f };
		float color_grenadeprediction_circle[3] = { 1.f, 1.f, 1.f };
		float GrenadeCollision[3] = { 1.f,0.f,0.f };
		float color_sniper[3] = { 1.f, 1.f, 1.f };
		float color_skeleton[3] = { 1.f, 1.f, 1.f };
		float droppedguns[3] = { 1.f,1.f,1.f };
		float hitbox[3] = { 1.f, 0.f, 0.f };

	} Color;

};

extern Variables g_Options;

