#pragma once
#include "HookIncludes.h"
#include <xlocbuf>
#include <deque>
#include <codecvt>
#include <algorithm>
void SkinChanger();
void InitializeKits();
void ApplyStickers();

extern int pWeaponItemIndexData[44];
extern int pStickersItemIndexData[34];

extern char* pWeaponData[44];
extern char* pStickersData[34];

extern unordered_map<int, const char*> g_ViewModelCfg;
extern unordered_map<const char*, const char*> g_KillIconCfg;

class CSkin
	:public singleton<CSkin>
{
public:
	void SetKillIconCfg();
	bool ApplyCustomKillIcon(IGameEvent* pEvent);

};

#define WEAPON_DATA_SIZE ( sizeof( pWeaponData ) / sizeof( *pWeaponData ) )

#define WEAPON_DEAGLE_STR		"deagle"				// 0
#define WEAPON_ELITE_STR		"elite"					// 1
#define WEAPON_FIVESEVEN_STR	"fiveseven"				// 2
#define WEAPON_GLOCK_STR		"glock"					// 3
#define WEAPON_AK47_STR			"ak47"					// 4
#define WEAPON_AUG_STR			"aug"					// 5
#define WEAPON_AWP_STR			"awp"					// 6
#define WEAPON_FAMAS_S_STR		"famas"					// 7
#define WEAPON_G3SG1_STR		"g3sg1"					// 8
#define WEAPON_GALILAR_STR		"galilar"				// 9
#define WEAPON_M249_STR			"m249"					// 10
#define WEAPON_M4A1S_STR		"m4a1_silencer"			// 11			
#define WEAPON_M4A41_STR		"m4a1"					// 12
#define WEAPON_MAC10_STR		"mac10"					// 13
#define WEAPON_P90_STR			"p90"					// 14
#define WEAPON_MP5SD_STR		"mp5sd"					// 14
#define WEAPON_UMP45_STR		"ump45"					// 15
#define WEAPON_XM1014_STR		"xm1014"				// 16
#define WEAPON_BIZON_STR		"bizon"					// 17
#define WEAPON_MAG7_STR			"mag7"					// 18
#define WEAPON_NEGEV_STR		"negev"					// 19
#define WEAPON_SAWOFF_STR		"sawedoff"				// 20
#define WEAPON_TEC9_STR			"tec9"					// 21
#define WEAPON_HKP2000_STR		"hkp2000"				// 22
#define WEAPON_MP7_STR			"mp7"					// 23
#define WEAPON_MP9_STR			"mp9"					// 24
#define WEAPON_NOVA_STR			"nova"					// 25
#define WEAPON_P250_STR			"p250"					// 26
#define WEAPON_SCAR20_STR		"scar20"				// 27
#define WEAPON_SG556_STR		"sg556"					// 28
#define WEAPON_SSG08_STR		"ssg08"					// 29
#define WEAPON_USP_S_STR		"usp_silencer"			// 30
#define WEAPON_CZ75A_STR		"cz75a"					// 31
#define WEAPON_REVOLVER_STR		"revolver"				// 32
#define WEAPON_KM9B_STR			"knife_m9_bayonet"		// 33
#define WEAPON_KBAYONET_STR		"bayonet"				// 34
#define WEAPON_KFLIP_STR		"knife_flip"			// 35
#define WEAPON_KGUT_STR			"knife_gut"				// 36
#define WEAPON_KARAMBIT_STR		"knife_karambit"		// 37
#define WEAPON_KTACTICAL_STR	"knife_tactical"		// 38
#define WEAPON_KFALCHION_STR	"knife_falchion"		// 39
#define WEAPON_KSBOWIE_STR		"knife_survival_bowie"	// 40
#define WEAPON_KBUTTERFLY_STR	"knife_butterfly"		// 41
#define WEAPON_KPUSH_STR		"knife_push"			// 42


#define Sticker_DATA_SIZE ( sizeof( pStickersData ) / sizeof( *pStickersData ) )

#define Sticker_DEAGLE_STR		"deagle"				// 0
#define Sticker_ELITE_STR		"elite"					// 1
#define Sticker_FIVESEVEN_STR	"fiveseven"				// 2
#define Sticker_GLOCK_STR		"glock"					// 3
#define Sticker_AK47_STR		"ak47"					// 4
#define Sticker_AUG_STR			"aug"					// 5
#define Sticker_AWP_STR			"awp"					// 6
#define Sticker_FAMAS_S_STR		"famas"					// 7
#define Sticker_G3SG1_STR		"g3sg1"					// 8
#define Sticker_GALILAR_STR		"galilar"				// 9
#define Sticker_M249_STR		"m249"					// 10
#define Sticker_M4A1S_STR		"m4a1_silencer"			// 11			
#define Sticker_M4A41_STR		"m4a1"					// 12
#define Sticker_MAC10_STR		"mac10"					// 13
#define Sticker_P90_STR			"p90"					// 14
#define Sticker_MP5SD_STR		"mp5sd"					// 14
#define Sticker_UMP45_STR		"ump45"					// 15
#define Sticker_XM1014_STR		"xm1014"				// 16
#define Sticker_BIZON_STR		"bizon"					// 17
#define Sticker_MAG7_STR		"mag7"					// 18
#define Sticker_NEGEV_STR		"negev"					// 19
#define Sticker_SAWOFF_STR		"sawedoff"				// 20
#define Sticker_TEC9_STR		"tec9"					// 21
#define Sticker_HKP2000_STR		"hkp2000"				// 22
#define Sticker_MP7_STR			"mp7"					// 23
#define Sticker_MP9_STR			"mp9"					// 24
#define Sticker_NOVA_STR		"nova"					// 25
#define Sticker_P250_STR		"p250"					// 26
#define Sticker_SCAR20_STR		"scar20"				// 27
#define Sticker_SG556_STR		"sg556"					// 28
#define Sticker_SSG08_STR		"ssg08"					// 29
#define Sticker_USP_S_STR		"usp_silencer"			// 30
#define Sticker_CZ75A_STR		"cz75a"					// 31
#define Sticker_REVOLVER_STR	"revolver"				// 32



struct Kit_t
{
	int id;
	std::string name;

	bool operator < (const Kit_t& other) const { return (name < other.name); }
};
extern std::deque<Kit_t> k_skins;
extern std::deque<Kit_t> k_gloves;
extern std::deque<Kit_t> k_stickers;


class CCStrike15ItemSchema;
class CCStrike15ItemSystem;

template <typename Key, typename Value>
struct Node_t
{
	int previous_id;		//0x0000
	int next_id;			//0x0004
	void* _unknown_ptr;		//0x0008
	int _unknown;			//0x000C
	Key key;				//0x0010
	Value value;			//0x0014
};

template <typename Key, typename Value>
struct Head_t
{
	Node_t<Key, Value>* memory;		//0x0000
	int allocation_count;			//0x0004
	int grow_size;					//0x0008
	int start_element;				//0x000C
	int next_available;				//0x0010
	int _unknown;					//0x0014
	int last_element;				//0x0018
}; //Size=0x001C

   // could use CUtlString but this is just easier and CUtlString isn't needed anywhere else
struct String_t
{
	char* buffer;	//0x0000
	int capacity;	//0x0004
	int grow_size;	//0x0008
	int length;		//0x000C
}; //Size=0x0010

struct CPaintKit
{
	int id;						//0x0000

	String_t name;				//0x0004
	String_t description;		//0x0014
	String_t item_name;			//0x0024
	String_t material_name;		//0x0034
	String_t image_inventory;	//0x0044

	char pad_0x0054[0x8C];		//0x0054
}; //Size=0x00E0

struct CStickerKit
{
	int id;

	int item_rarity;

	String_t name;
	String_t description;
	String_t item_name;
	String_t material_name;
	String_t image_inventory;

	int tournament_event_id;
	int tournament_team_id;
	int tournament_player_id;
	bool is_custom_sticker_material;

	float rotate_end;
	float rotate_start;

	float scale_min;
	float scale_max;

	float wear_min;
	float wear_max;

	String_t image_inventory2;
	String_t image_inventory_large;

	uint32_t pad0[4];
};
