#include "Configuration.hpp"
#include "SkinChanger.h"
#include "Variables.h"
#include <winerror.h>
#pragma warning( disable : 4091)
#include <ShlObj.h>
#include <string>
#include <sstream>
#include "TEXT.h"


void LoadSettings(std::string szIniFile)
{
	CSX::Cvar::InitPath(szIniFile.c_str());

#pragma region L_Ragebot
	g_Options.Ragebot.MainSwitch = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_MainSwitch, g_Options.Ragebot.MainSwitch) != false;
	g_Options.Ragebot.Enabled = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_Enabled, g_Options.Ragebot.Enabled) != false;
	g_Options.Ragebot.AutoFire = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_AutoFire, g_Options.Ragebot.AutoFire) != false;
	g_Options.Ragebot.TargetSelection = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_TargetSelection, g_Options.Ragebot.TargetSelection);
	g_Options.Ragebot.FOV = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_FOV, g_Options.Ragebot.FOV);
	g_Options.Ragebot.Silent = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_Silent, g_Options.Ragebot.Silent) != false;
	g_Options.Ragebot.AutoPistol = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_AutoPistol, g_Options.Ragebot.AutoPistol) != false;
	g_Options.Ragebot.KeyPress = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_KeyPress, g_Options.Ragebot.KeyPress);
	g_Options.Ragebot.EnabledAntiAim = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_EnabledAntiAim, g_Options.Ragebot.EnabledAntiAim) != false;
	g_Options.Ragebot.PreAAs = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_PreAAs, g_Options.Ragebot.PreAAs) != false;
	g_Options.Ragebot.Pitch = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_Pitch, g_Options.Ragebot.Pitch);
	g_Options.Ragebot.YawTrue = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_YawTrue, g_Options.Ragebot.YawTrue);
	g_Options.Ragebot.YawFake = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_YawFake, g_Options.Ragebot.YawFake);
	g_Options.Ragebot.YawTrueAdder = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_YawTrueAdder, g_Options.Ragebot.YawTrueAdder);
	g_Options.Ragebot.YawFakeAdder = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_YawFakeAdder, g_Options.Ragebot.YawFakeAdder);
	g_Options.Ragebot.PitchAdder = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_PitchAdder, g_Options.Ragebot.PitchAdder);
	g_Options.Ragebot.AtTarget = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_AtTarget, g_Options.Ragebot.AtTarget) != false;
	g_Options.Ragebot.Edge = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_Edge, g_Options.Ragebot.Edge) != false;
	g_Options.Ragebot.KnifeAA = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_KnifeAA, g_Options.Ragebot.KnifeAA) != false;
	g_Options.Ragebot.BuilderAAs = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_BuilderAAs, g_Options.Ragebot.BuilderAAs) != false;
	g_Options.Ragebot.Jitter = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_Jitter, g_Options.Ragebot.Jitter) != false;
	g_Options.Ragebot.FJitter = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_FJitter, g_Options.Ragebot.FJitter) != false;
	g_Options.Ragebot.LBYBreaker = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_LBYBreaker, g_Options.Ragebot.LBYBreaker) != false;
	g_Options.Ragebot.BuilderReal = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_BuilderReal, g_Options.Ragebot.BuilderReal);
	g_Options.Ragebot.BuilderFake = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_BuilderFake, g_Options.Ragebot.BuilderFake);
	g_Options.Ragebot.BuilderPitch = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_BuilderPitch, g_Options.Ragebot.BuilderPitch);
	g_Options.Ragebot.JitterRange = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_JitterRange, g_Options.Ragebot.JitterRange);
	g_Options.Ragebot.FJitterRange = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_FJitterRange, g_Options.Ragebot.FJitterRange);
	g_Options.Ragebot.FriendlyFire = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_FriendlyFire, g_Options.Ragebot.FriendlyFire) != false;
	g_Options.Ragebot.autowall = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_autowall, g_Options.Ragebot.autowall) != false;
	g_Options.Ragebot.Hitbox = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_Hitbox, g_Options.Ragebot.Hitbox);
	g_Options.Ragebot.Hitscan = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_Hitscan, g_Options.Ragebot.Hitscan);
	g_Options.Ragebot.AntiRecoil = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_AntiRecoil, g_Options.Ragebot.AntiRecoil) != false;
	g_Options.Ragebot.AntiSpread = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_AntiSpread, g_Options.Ragebot.AntiSpread) != false;
	g_Options.Ragebot.AutoStop = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_AutoStop, g_Options.Ragebot.AutoStop) != false;
	g_Options.Ragebot.AutoCrouch = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_AutoCrouch, g_Options.Ragebot.AutoCrouch) != false;
	g_Options.Ragebot.AutoScope = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_AutoScope, g_Options.Ragebot.AutoScope) != false;
	g_Options.Ragebot.AIFakeLag = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_AIFakeLag, g_Options.Ragebot.AIFakeLag) != false;
	g_Options.Ragebot.FakeLagAmt = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_AIFakeLagAmt, g_Options.Ragebot.AIFakeLagAmt);
	g_Options.Ragebot.Hitchance = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_Hitchance, g_Options.Ragebot.Hitchance) != false;
	g_Options.Ragebot.Resolver = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_Resolver, g_Options.Ragebot.Resolver) != false;
	g_Options.Ragebot.FakeLagFix = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_FakeLagFix, g_Options.Ragebot.FakeLagFix) != false;
	g_Options.Ragebot.PosAdjust = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_PosAdjust, g_Options.Ragebot.PosAdjust) != false;
	g_Options.Ragebot.playerlist = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_playerlist, g_Options.Ragebot.playerlist) != false;
	g_Options.Ragebot.BAIMkey = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_BAIMkey, g_Options.Ragebot.BAIMkey);
	g_Options.Ragebot.bruteAfterX = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_bruteAfterX, g_Options.Ragebot.bruteAfterX);
	g_Options.Ragebot.AntiSmac = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_AntiSmac, g_Options.Ragebot.AntiSmac) != false;
	g_Options.Ragebot.MinimumDamageSniper = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_MinimumDamageSniper, g_Options.Ragebot.MinimumDamageSniper);
	g_Options.Ragebot.MinimumDamageRifle = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_MinimumDamageRifle, g_Options.Ragebot.MinimumDamageRifle);
	g_Options.Ragebot.MinimumDamagePistol = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_MinimumDamagePistol, g_Options.Ragebot.MinimumDamagePistol);
	g_Options.Ragebot.MinimumDamageHeavy = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_MinimumDamageHeavy, g_Options.Ragebot.MinimumDamageHeavy);
	g_Options.Ragebot.MinimumDamageSmg = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_MinimumDamageSmg, g_Options.Ragebot.MinimumDamageSmg);
	g_Options.Ragebot.MinimumDamageRevolver = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_MinimumDamageRevolver, g_Options.Ragebot.MinimumDamageRevolver);
	g_Options.Ragebot.HitchanceSniper = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_HitchanceSniper, g_Options.Ragebot.HitchanceSniper);
	g_Options.Ragebot.HitchancePistol = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_HitchancePistol, g_Options.Ragebot.HitchancePistol);
	g_Options.Ragebot.HitchanceRifle = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_HitchanceRifle, g_Options.Ragebot.HitchanceRifle);
	g_Options.Ragebot.HitchanceHeavy = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_HitchanceHeavy, g_Options.Ragebot.HitchanceHeavy);
	g_Options.Ragebot.HitchanceSmgs = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_HitchanceSmgs, g_Options.Ragebot.HitchanceSmgs);
	g_Options.Ragebot.HitchanceRevolver = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_HitchanceRevolver, g_Options.Ragebot.HitchanceRevolver);
	g_Options.Ragebot.Multipoint = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_Multipoint, g_Options.Ragebot.Multipoint);
	g_Options.Ragebot.Pointscalex = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_Pointscalex, g_Options.Ragebot.Pointscalex);
	g_Options.Ragebot.Pointscaley = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_Pointscaley, g_Options.Ragebot.Pointscaley);
	g_Options.Ragebot.hitscaninvis = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_HitscanInvis, g_Options.Ragebot.hitscaninvis);
	g_Options.Ragebot.FakeLagType = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_FakeLagType, g_Options.Ragebot.FakeLagType);
	g_Options.Ragebot.FakeLag = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_Fakelag, g_Options.Ragebot.FakeLag) != false;
	g_Options.Ragebot.FakeLagAmt = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_FakeLagAmt, g_Options.Ragebot.FakeLagAmt);
	g_Options.Ragebot.FakeLagMode = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_FakelagMode, g_Options.Ragebot.FakeLagMode);
	g_Options.Ragebot.AimbotResolver = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_AimbotResolver, g_Options.Ragebot.AimbotResolver);
	g_Options.Ragebot.Resolver3 = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_Resolver3, g_Options.Ragebot.Resolver3);
	g_Options.Ragebot.LBYCorrection = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_LBYCorrection, g_Options.Ragebot.LBYCorrection) != false;
	g_Options.Ragebot.LBYCorrection2 = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_LBYCorrection2, g_Options.Ragebot.LBYCorrection2) != false;
	g_Options.Ragebot.RageDraw = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_RageINFO, g_Options.Ragebot.RageDraw) != false;
	g_Options.Ragebot.flipkey = CSX::Cvar::LoadCvar(RAGETBOT_TEXT, R_flipkey, g_Options.Ragebot.flipkey);

#pragma endregion
#pragma region L_Ligetbot
	g_Options.Legitbot.MainSwitch = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_MainSwitch, g_Options.Legitbot.MainSwitch) != false;
	g_Options.Legitbot.backtrack = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_backtrack, g_Options.Legitbot.backtrack) != false;
	g_Options.Legitbot.backtrackTicks = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_backtrackTicks, g_Options.Legitbot.backtrackTicks);
	g_Options.Legitbot.backtrack_size = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_backtrackSize, g_Options.Legitbot.backtrack_size);
	g_Options.Legitbot.Aimbot.Enabled = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_Enabled, g_Options.Legitbot.Aimbot.Enabled) != false;
	g_Options.Legitbot.Aimbot.ALegitAA = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_ALegitAA, g_Options.Legitbot.Aimbot.ALegitAA) != false;
	g_Options.Legitbot.checksmoke = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_checksmoke, g_Options.Legitbot.checksmoke) != false;
	g_Options.Legitbot.Aimbot.EnableLegitAA = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_LAA, g_Options.Legitbot.Aimbot.EnableLegitAA) != false;
	g_Options.Legitbot.Aimbot.AAAngle = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_AAAngle, g_Options.Legitbot.Aimbot.AAAngle);

	g_Options.Legitbot.PistolKey = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_PistolKey, g_Options.Legitbot.PistolKey);
	g_Options.Legitbot.Pistolfov = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_Pistolfov, g_Options.Legitbot.Pistolfov);
	g_Options.Legitbot.Pistol_fakelag = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_Pistol_FakeLag, g_Options.Legitbot.Pistol_fakelag) != false;
	g_Options.Legitbot.Pistol_fakelag_mode = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_Pistol_FakeLag_Mode, g_Options.Legitbot.Pistol_fakelag_mode);
	g_Options.Legitbot.Pistol_fakelag_value = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_Pistol_FakeLag_Value, g_Options.Legitbot.Pistol_fakelag_value);
	g_Options.Legitbot.Pistol_Smooth = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_PistolSmooth, g_Options.Legitbot.Pistol_Smooth);
	g_Options.Legitbot.hitboxpistol = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_PistolHitbox, g_Options.Legitbot.hitboxpistol);
	g_Options.Legitbot.Pistol_rcs = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_pistol_rcs, g_Options.Legitbot.Pistol_rcs);
	g_Options.Legitbot.Pistol_RcsType = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_pistol_mode, g_Options.Legitbot.Pistol_RcsType);
	g_Options.Legitbot.Aimbot.AutoPistol = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_pistol_AutoPistol, g_Options.Legitbot.Aimbot.AutoPistol) != false;
	g_Options.Legitbot.Pistol_AutoLock = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_pistol_AutoLock, g_Options.Legitbot.Pistol_AutoLock) != false;
	g_Options.Legitbot.Pistol_Silent = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_pistol_AutoLock, g_Options.Legitbot.Pistol_Silent) != false;
	g_Options.Legitbot.Pistol_TriggerbotKey = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_pistol_TriggerbotKey, g_Options.Legitbot.Pistol_TriggerbotKey);
	g_Options.Legitbot.Pistol_Triggerbot_hitchance = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_pistol_Triggerbot_hitchance, g_Options.Legitbot.Pistol_Triggerbot_hitchance);
	g_Options.Legitbot.Pistol_Triggerbot_AutoFire = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_Pistol_Triggerbot_AutoFire, g_Options.Legitbot.Pistol_Triggerbot_AutoFire) != false;
	g_Options.Legitbot.Pistol_Triggerbot = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_Pistol_Triggerbot, g_Options.Legitbot.Pistol_Triggerbot) != false;
	g_Options.Legitbot.Pistol_TriggerbotFilter.Arms = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_Pistol_Triggerbot_Arms, g_Options.Legitbot.Pistol_TriggerbotFilter.Arms) != false;
	g_Options.Legitbot.Pistol_TriggerbotFilter.Chest = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_Pistol_Triggerbot_Chest, g_Options.Legitbot.Pistol_TriggerbotFilter.Chest) != false;
	g_Options.Legitbot.Pistol_TriggerbotFilter.Head = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_Pistol_Triggerbot_Head, g_Options.Legitbot.Pistol_TriggerbotFilter.Head) != false;
	g_Options.Legitbot.Pistol_TriggerbotFilter.Legs = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_Pistol_Triggerbot_Legs, g_Options.Legitbot.Pistol_TriggerbotFilter.Legs) != false;
	g_Options.Legitbot.Pistol_TriggerbotFilter.Stomach = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_Pistol_Triggerbot_Stomach, g_Options.Legitbot.Pistol_TriggerbotFilter.Stomach) != false;

	g_Options.Legitbot.heavyKey = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_heavyKey, g_Options.Legitbot.heavyKey);
	g_Options.Legitbot.heavyfov = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_heavyfov, g_Options.Legitbot.heavyfov);
	g_Options.Legitbot.heavy_fakelag = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_heavy_FakeLag, g_Options.Legitbot.Pistol_fakelag) != false;
	g_Options.Legitbot.heavy_fakelag_mode = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_heavy_FakeLag_Mode, g_Options.Legitbot.heavy_fakelag_mode);
	g_Options.Legitbot.heavy_fakelag_value = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_heavy_FakeLag_Value, g_Options.Legitbot.heavy_fakelag_value);
	g_Options.Legitbot.heavy_Smooth = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_heavySmooth, g_Options.Legitbot.heavy_Smooth);
	g_Options.Legitbot.hitboxheavy = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_heavyHitbox, g_Options.Legitbot.hitboxheavy);
	g_Options.Legitbot.heavy_rcs = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_heavy_rcs, g_Options.Legitbot.heavy_rcs);
	g_Options.Legitbot.heavy_RcsType = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_heavy_rcs, g_Options.Legitbot.heavy_RcsType);
	g_Options.Legitbot.heavy_AutoLock = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_heavy_AutoLock, g_Options.Legitbot.heavy_AutoLock) != false;
	g_Options.Legitbot.heavy_Silent = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_heavy_Silent, g_Options.Legitbot.heavy_Silent) != false;
	g_Options.Legitbot.heavy_TriggerbotKey = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_heavy_TriggerbotKey, g_Options.Legitbot.heavy_TriggerbotKey);
	g_Options.Legitbot.heavy_Triggerbot_hitchance = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_heavy_Triggerbot_hitchance, g_Options.Legitbot.heavy_Triggerbot_hitchance);
	g_Options.Legitbot.heavy_Triggerbot_AutoFire = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_heavy_Triggerbot_AutoFire, g_Options.Legitbot.heavy_Triggerbot_AutoFire) != false;
	g_Options.Legitbot.heavy_Triggerbot = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_heavy_Triggerbot, g_Options.Legitbot.heavy_Triggerbot) != false;
	g_Options.Legitbot.heavy_TriggerbotFilter.Arms = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_heavy_Triggerbot_Arms, g_Options.Legitbot.heavy_TriggerbotFilter.Arms) != false;
	g_Options.Legitbot.heavy_TriggerbotFilter.Chest = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_heavy_Triggerbot_Chest, g_Options.Legitbot.heavy_TriggerbotFilter.Chest) != false;
	g_Options.Legitbot.heavy_TriggerbotFilter.Head = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_heavy_Triggerbot_Head, g_Options.Legitbot.heavy_TriggerbotFilter.Head) != false;
	g_Options.Legitbot.heavy_TriggerbotFilter.Legs = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_heavy_Triggerbot_Legs, g_Options.Legitbot.heavy_TriggerbotFilter.Legs) != false;
	g_Options.Legitbot.heavy_TriggerbotFilter.Stomach = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_heavy_Triggerbot_Stomach, g_Options.Legitbot.heavy_TriggerbotFilter.Stomach) != false;

	g_Options.Legitbot.smgKey = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_smgKey, g_Options.Legitbot.smgKey);
	g_Options.Legitbot.smgfov = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_smgfov, g_Options.Legitbot.smgfov);
	g_Options.Legitbot.smg_fakelag = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_smg_FakeLag, g_Options.Legitbot.smg_fakelag) != false;
	g_Options.Legitbot.smg_fakelag_mode = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_smg_FakeLag_Mode, g_Options.Legitbot.smg_fakelag_mode);
	g_Options.Legitbot.smg_fakelag_value = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_smg_FakeLag_Value, g_Options.Legitbot.smg_fakelag_value);
	g_Options.Legitbot.smg_Smooth = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_smgSmooth, g_Options.Legitbot.smg_Smooth);
	g_Options.Legitbot.hitboxsmg = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_smgHitbox, g_Options.Legitbot.hitboxsmg);
	g_Options.Legitbot.smg_rcs = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_smg_rcs, g_Options.Legitbot.smg_rcs);
	g_Options.Legitbot.smg_RcsType = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_smg_mode, g_Options.Legitbot.smg_RcsType);
	g_Options.Legitbot.smg_AutoLock = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_smg_AutoLock, g_Options.Legitbot.smg_AutoLock) != false;
	g_Options.Legitbot.smg_Silent = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_smg_Silent, g_Options.Legitbot.smg_Silent) != false;
	g_Options.Legitbot.smg_TriggerbotKey = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_smg_TriggerbotKey, g_Options.Legitbot.smg_TriggerbotKey);
	g_Options.Legitbot.smg_Triggerbot_hitchance = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_smg_Triggerbot_hitchance, g_Options.Legitbot.smg_Triggerbot_hitchance);
	g_Options.Legitbot.smg_Triggerbot_AutoFire = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_smg_Triggerbot_AutoFire, g_Options.Legitbot.smg_Triggerbot_AutoFire) != false;
	g_Options.Legitbot.smg_Triggerbot = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_smg_Triggerbot, g_Options.Legitbot.smg_Triggerbot) != false;
	g_Options.Legitbot.smg_TriggerbotFilter.Arms = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_smg_Triggerbot_Arms, g_Options.Legitbot.smg_TriggerbotFilter.Arms) != false;
	g_Options.Legitbot.smg_TriggerbotFilter.Chest = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_smg_Triggerbot_Chest, g_Options.Legitbot.smg_TriggerbotFilter.Chest) != false;
	g_Options.Legitbot.smg_TriggerbotFilter.Head = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_smg_Triggerbot_Head, g_Options.Legitbot.smg_TriggerbotFilter.Head) != false;
	g_Options.Legitbot.smg_TriggerbotFilter.Legs = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_smg_Triggerbot_Legs, g_Options.Legitbot.smg_TriggerbotFilter.Legs) != false;
	g_Options.Legitbot.smg_TriggerbotFilter.Stomach = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_smg_Triggerbot_Stomach, g_Options.Legitbot.smg_TriggerbotFilter.Stomach) != false;

	g_Options.Legitbot.MainKey = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_MainKey, g_Options.Legitbot.MainKey);
	g_Options.Legitbot.Mainfov = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_Mainfov, g_Options.Legitbot.Mainfov);
	g_Options.Legitbot.main_fakelag = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_main_FakeLag, g_Options.Legitbot.main_fakelag) != false;
	g_Options.Legitbot.main_fakelag_mode = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_main_FakeLag_Mode, g_Options.Legitbot.main_fakelag_mode);
	g_Options.Legitbot.main_fakelag_value = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_main_FakeLag_Value, g_Options.Legitbot.main_fakelag_value);
	g_Options.Legitbot.Main_Smooth = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_MainSmooth, g_Options.Legitbot.Main_Smooth);
	g_Options.Legitbot.hitboxrifle = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_MainHitbox, g_Options.Legitbot.hitboxrifle);
	g_Options.Legitbot.main_rcs = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_main_rcs, g_Options.Legitbot.main_rcs);
	g_Options.Legitbot.main_RcsType = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_main_mode, g_Options.Legitbot.main_RcsType);
	g_Options.Legitbot.main_AutoLock = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_main_AutoLock, g_Options.Legitbot.main_AutoLock) != false;
	g_Options.Legitbot.main_Silent = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_main_Silent, g_Options.Legitbot.main_Silent) != false;
	g_Options.Legitbot.main_TriggerbotKey = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_main_TriggerbotKey, g_Options.Legitbot.main_TriggerbotKey);
	g_Options.Legitbot.main_Triggerbot_hitchance = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_main_Triggerbot_hitchance, g_Options.Legitbot.main_Triggerbot_hitchance);
	g_Options.Legitbot.main_Triggerbot_AutoFire = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_main_Triggerbot_AutoFire, g_Options.Legitbot.main_Triggerbot_AutoFire) != false;
	g_Options.Legitbot.main_Triggerbot = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_main_Triggerbot, g_Options.Legitbot.main_Triggerbot) != false;
	g_Options.Legitbot.main_TriggerbotFilter.Arms = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_main_Triggerbot_Arms, g_Options.Legitbot.main_TriggerbotFilter.Arms) != false;
	g_Options.Legitbot.main_TriggerbotFilter.Chest = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_main_Triggerbot_Chest, g_Options.Legitbot.main_TriggerbotFilter.Chest) != false;
	g_Options.Legitbot.main_TriggerbotFilter.Head = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_main_Triggerbot_Head, g_Options.Legitbot.main_TriggerbotFilter.Head) != false;
	g_Options.Legitbot.main_TriggerbotFilter.Legs = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_main_Triggerbot_Legs, g_Options.Legitbot.main_TriggerbotFilter.Legs) != false;
	g_Options.Legitbot.main_TriggerbotFilter.Stomach = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_main_Triggerbot_Stomach, g_Options.Legitbot.main_TriggerbotFilter.Stomach) != false;

	g_Options.Legitbot.SniperKey = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_SniperKey, g_Options.Legitbot.SniperKey);
	g_Options.Legitbot.Sniperfov = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_Sniperfov, g_Options.Legitbot.Sniperfov);
	g_Options.Legitbot.Sniper_fakelag = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_sniper_FakeLag, g_Options.Legitbot.Sniper_fakelag) != false;
	g_Options.Legitbot.Sniper_fakelag_mode = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_sniper_FakeLag_Mode, g_Options.Legitbot.Sniper_fakelag_mode);
	g_Options.Legitbot.Sniper_fakelag_value = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_sniper_FakeLag_Value, g_Options.Legitbot.Sniper_fakelag_value);
	g_Options.Legitbot.Sniper_Smooth = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_SniperSmooth, g_Options.Legitbot.Sniper_Smooth);
	g_Options.Legitbot.hitboxsniper = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_SniperHitbox, g_Options.Legitbot.hitboxsniper);
	g_Options.Legitbot.sniper_rcs = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_sniper_rcs, g_Options.Legitbot.sniper_rcs);
	g_Options.Legitbot.sniper_RcsType = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_sniper_mode, g_Options.Legitbot.sniper_RcsType);
	g_Options.Legitbot.Aimbot.fastzoom = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_sniper_fastzoom, g_Options.Legitbot.Aimbot.fastzoom) != false;
	g_Options.Legitbot.Aimbot.fastzoomswitch = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_sniper_fastzoomswitch, g_Options.Legitbot.Aimbot.fastzoomswitch) != false;
	g_Options.Legitbot.sniper_AutoLock = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_sniper_AutoLock, g_Options.Legitbot.sniper_AutoLock) != false;
	g_Options.Legitbot.sniper_Silent = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_sniper_Silent, g_Options.Legitbot.sniper_Silent) != false;
	g_Options.Legitbot.sniper_TriggerbotKey = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_sniper_TriggerbotKey, g_Options.Legitbot.sniper_TriggerbotKey);
	g_Options.Legitbot.sniper_Triggerbot_hitchance = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_sniper_Triggerbot_hitchance, g_Options.Legitbot.sniper_Triggerbot_hitchance);
	g_Options.Legitbot.sniper_Triggerbot_AutoFire = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_sniper_Triggerbot_AutoFire, g_Options.Legitbot.sniper_Triggerbot_AutoFire) != false;
	g_Options.Legitbot.sniper_Triggerbot = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_sniper_Triggerbot, g_Options.Legitbot.sniper_Triggerbot) != false;
	g_Options.Legitbot.sniper_TriggerbotFilter.Arms = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_sniper_Triggerbot_Arms, g_Options.Legitbot.sniper_TriggerbotFilter.Arms) != false;
	g_Options.Legitbot.sniper_TriggerbotFilter.Chest = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_sniper_Triggerbot_Chest, g_Options.Legitbot.sniper_TriggerbotFilter.Chest) != false;
	g_Options.Legitbot.sniper_TriggerbotFilter.Head = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_sniper_Triggerbot_Head, g_Options.Legitbot.sniper_TriggerbotFilter.Head) != false;
	g_Options.Legitbot.sniper_TriggerbotFilter.Legs = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_sniper_Triggerbot_Legs, g_Options.Legitbot.sniper_TriggerbotFilter.Legs) != false;
	g_Options.Legitbot.sniper_TriggerbotFilter.Stomach = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_sniper_Triggerbot_Stomach, g_Options.Legitbot.sniper_TriggerbotFilter.Stomach) != false;

	g_Options.Legitbot.RevolverKey = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_RevolverKey, g_Options.Legitbot.RevolverKey);
	g_Options.Legitbot.Revolverfov = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_Revolverfov, g_Options.Legitbot.Revolverfov);
	g_Options.Legitbot.Revolver_fakelag = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_Revolver_FakeLag, g_Options.Legitbot.Revolver_fakelag) != false;
	g_Options.Legitbot.Revolver_fakelag_mode = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_Revolver_FakeLag_Mode, g_Options.Legitbot.Revolver_fakelag_mode);
	g_Options.Legitbot.Revolver_fakelag_value = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_Revolver_FakeLag_Value, g_Options.Legitbot.Revolver_fakelag_value);
	g_Options.Legitbot.Revolver_Smooth = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_RevolverSmooth, g_Options.Legitbot.Revolver_Smooth);
	g_Options.Legitbot.hitboxRevolver = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_RevolverHitbox, g_Options.Legitbot.hitboxRevolver);
	g_Options.Legitbot.Revolver_rcs = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_Revolver_rcs, g_Options.Legitbot.Revolver_rcs);
	g_Options.Legitbot.Revolver_RcsType = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_Revolver_mode, g_Options.Legitbot.Revolver_RcsType);
	g_Options.Legitbot.Revolver_AutoLock = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_Revolver_AutoLock, g_Options.Legitbot.Revolver_AutoLock) != false;
	g_Options.Legitbot.Revolver_Silent = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_Revolver_Silent, g_Options.Legitbot.Revolver_Silent) != false;
	g_Options.Legitbot.Revolver_TriggerbotKey = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_Revolver_TriggerbotKey, g_Options.Legitbot.Revolver_TriggerbotKey);
	g_Options.Legitbot.Revolver_Triggerbot_hitchance = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_Revolver_Triggerbot_hitchance, g_Options.Legitbot.Revolver_Triggerbot_hitchance);
	g_Options.Legitbot.Revolver_Triggerbot_AutoFire = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_Revolver_Triggerbot_AutoFire, g_Options.Legitbot.Revolver_Triggerbot_AutoFire) != false;
	g_Options.Legitbot.Revolver_Triggerbot = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_Revolver_Triggerbot, g_Options.Legitbot.Revolver_Triggerbot) != false;
	g_Options.Legitbot.Revolver_TriggerbotFilter.Arms = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_Revolver_Triggerbot_Arms, g_Options.Legitbot.Revolver_TriggerbotFilter.Arms) != false;
	g_Options.Legitbot.Revolver_TriggerbotFilter.Chest = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_Revolver_Triggerbot_Chest, g_Options.Legitbot.Revolver_TriggerbotFilter.Chest) != false;
	g_Options.Legitbot.Revolver_TriggerbotFilter.Head = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_Revolver_Triggerbot_Head, g_Options.Legitbot.Revolver_TriggerbotFilter.Head) != false;
	g_Options.Legitbot.Revolver_TriggerbotFilter.Legs = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_Revolver_Triggerbot_Legs, g_Options.Legitbot.Revolver_TriggerbotFilter.Legs) != false;
	g_Options.Legitbot.Revolver_TriggerbotFilter.Stomach = CSX::Cvar::LoadCvar(LIGETBOT_TEXT, L_Revolver_Triggerbot_Stomach, g_Options.Legitbot.Revolver_TriggerbotFilter.Stomach) != false;

#pragma endregion
#pragma region L_ESP
	g_Options.Visuals.Enabled = CSX::Cvar::LoadCvar(ESP_TEXT, E_Enabled, g_Options.Visuals.Enabled) != false;
	g_Options.Visuals.Box = CSX::Cvar::LoadCvar(ESP_TEXT, E_Box, g_Options.Visuals.Box) != false;
	g_Options.Visuals.AimSpot = CSX::Cvar::LoadCvar(ESP_TEXT, E_AimSpot, g_Options.Visuals.AimSpot) != false;
	g_Options.Visuals.DLight = CSX::Cvar::LoadCvar(ESP_TEXT, E_DLight, g_Options.Visuals.DLight) != false;
	g_Options.Visuals.glowplayer = CSX::Cvar::LoadCvar(ESP_TEXT, E_glowplayer, g_Options.Visuals.glowplayer) != false;
	g_Options.Visuals.Weapons = CSX::Cvar::LoadCvar(ESP_TEXT, E_Weapons, g_Options.Visuals.Weapons);
	g_Options.Visuals.LBYIndicator = CSX::Cvar::LoadCvar(ESP_TEXT, E_LBYIndicator, g_Options.Visuals.LBYIndicator) != false;
	g_Options.Visuals.Chams = CSX::Cvar::LoadCvar(ESP_TEXT, E_Chams, g_Options.Visuals.Chams) != false;
	g_Options.Visuals.Teamchams = CSX::Cvar::LoadCvar(ESP_TEXT, E_Teamchams, g_Options.Visuals.Teamchams) != false;
	g_Options.Visuals.XQZ = CSX::Cvar::LoadCvar(ESP_TEXT, E_XQZ, g_Options.Visuals.XQZ) != false;
	g_Options.Visuals.weaponglow = CSX::Cvar::LoadCvar(ESP_TEXT, E_weaponglow, g_Options.Visuals.weaponglow) != false;
	g_Options.Visuals.teamglow = CSX::Cvar::LoadCvar(ESP_TEXT, E_teamglow, g_Options.Visuals.teamglow) != false;
	g_Options.Visuals.bombglow = CSX::Cvar::LoadCvar(ESP_TEXT, E_bombglow, g_Options.Visuals.bombglow) != false;
	g_Options.Visuals.BoxType = CSX::Cvar::LoadCvar(ESP_TEXT, E_BoxType, g_Options.Visuals.BoxType);
	g_Options.Visuals.TeamESP = CSX::Cvar::LoadCvar(ESP_TEXT, E_TeamESP, g_Options.Visuals.TeamESP) != false;
	g_Options.Visuals.AimLine = CSX::Cvar::LoadCvar(ESP_TEXT, E_AimLine, g_Options.Visuals.AimLine) != false;
	g_Options.Visuals.health = CSX::Cvar::LoadCvar(ESP_TEXT, E_health, g_Options.Visuals.health) != false;
	g_Options.Visuals.barrel = CSX::Cvar::LoadCvar(ESP_TEXT, E_barrel, g_Options.Visuals.barrel) != false;
	g_Options.Visuals.DrawAwall = CSX::Cvar::LoadCvar(ESP_TEXT, E_DrawAwall, g_Options.Visuals.DrawAwall) != false;
	g_Options.Visuals.barrelL = CSX::Cvar::LoadCvar(ESP_TEXT, E_barrelL, g_Options.Visuals.barrelL);
	g_Options.Visuals.Playermatierial = CSX::Cvar::LoadCvar(ESP_TEXT, E_Playermatierial, g_Options.Visuals.Playermatierial);
	g_Options.Visuals.Weaponsdroppedmatierial = CSX::Cvar::LoadCvar(ESP_TEXT, E_Weaponsdroppedmatierial, g_Options.Visuals.Weaponsdroppedmatierial);
	g_Options.Visuals.WeaponsDroppedChams = CSX::Cvar::LoadCvar(ESP_TEXT, E_droppedchams, g_Options.Visuals.WeaponsDroppedChams) != false;
	g_Options.Visuals.angleLines = CSX::Cvar::LoadCvar(ESP_TEXT, E_angleLines, g_Options.Visuals.angleLines) != false;
	g_Options.Visuals.esp_Visible = CSX::Cvar::LoadCvar(ESP_TEXT, E_esp_Visible, g_Options.Visuals.esp_Visible);
	g_Options.Visuals.corpses = CSX::Cvar::LoadCvar(ESP_TEXT, E_esp_corpses, g_Options.Visuals.corpses) != false;
	g_Options.Visuals.fill = CSX::Cvar::LoadCvar(ESP_TEXT, E_esp_fillbox, g_Options.Visuals.fill) != false;
	g_Options.Visuals.armor = CSX::Cvar::LoadCvar(ESP_TEXT, E_esp_armor, g_Options.Visuals.armor) != false;
	g_Options.Visuals.bulletshow = CSX::Cvar::LoadCvar(ESP_TEXT, E_esp_bulletshow, g_Options.Visuals.bulletshow) != false;
	g_Options.Visuals.flTracersDuration = CSX::Cvar::LoadCvar(ESP_TEXT, E_esp_flTracersDuration, g_Options.Visuals.flTracersDuration);
	g_Options.Visuals.flTracersWidth = CSX::Cvar::LoadCvar(ESP_TEXT, E_esp_flTracersWidth, g_Options.Visuals.flTracersWidth);
	g_Options.Visuals.healthtype = CSX::Cvar::LoadCvar(ESP_TEXT, E_esp_healthtype, g_Options.Visuals.healthtype);
	g_Options.Visuals.chamsHp = CSX::Cvar::LoadCvar(ESP_TEXT, E_esp_chamsHp, g_Options.Visuals.chamsHp);
	g_Options.Visuals.esp_fill_amount = CSX::Cvar::LoadCvar(ESP_TEXT, E_esp_fill_amount, g_Options.Visuals.esp_fill_amount);
	g_Options.Visuals.esp_player_chams_fakeangle = CSX::Cvar::LoadCvar(ESP_TEXT, E_chams_fakeangle, g_Options.Visuals.esp_player_chams_fakeangle);
	g_Options.Visuals.AWallCrosshair = CSX::Cvar::LoadCvar(ESP_TEXT, E_AWallCrosshair, g_Options.Visuals.AWallCrosshair) != false;
	g_Options.Visuals.SniperCrosshair = CSX::Cvar::LoadCvar(ESP_TEXT, E_SniperCrosshair, g_Options.Visuals.SniperCrosshair) != false;
	g_Options.Visuals.SniperCrosshairType = CSX::Cvar::LoadCvar(ESP_TEXT, E_SniperCrosshairType, g_Options.Visuals.SniperCrosshairType);
	g_Options.Visuals.CrosshairOn = CSX::Cvar::LoadCvar(ESP_TEXT, E_CrosshairOn, g_Options.Visuals.CrosshairOn);
	g_Options.Visuals.CrosshairSpread = CSX::Cvar::LoadCvar(ESP_TEXT, E_CrosshairSpread, g_Options.Visuals.CrosshairSpread);
	g_Options.Visuals.CrosshairType = CSX::Cvar::LoadCvar(ESP_TEXT, E_CrosshairType, g_Options.Visuals.CrosshairType);
	g_Options.Visuals.CrosshairStyle = CSX::Cvar::LoadCvar(ESP_TEXT, E_CrosshairStyle, g_Options.Visuals.CrosshairStyle);
	g_Options.Visuals.CrosshairRainbow = CSX::Cvar::LoadCvar(ESP_TEXT, E_CrosshairRainbow, g_Options.Visuals.CrosshairRainbow);

	g_Options.Visuals.Name = CSX::Cvar::LoadCvar(ESP_TEXT, E_Name, g_Options.Visuals.Name) != false;
	g_Options.Visuals.skeletonenbl = CSX::Cvar::LoadCvar(ESP_TEXT, E_Skeleton, g_Options.Visuals.skeletonenbl) != false;
	g_Options.Visuals.OffscreenIndicator = CSX::Cvar::LoadCvar(ESP_TEXT, E_OffscreenIndicator, g_Options.Visuals.OffscreenIndicator) != false;
	g_Options.Visuals.GrenadeBox = CSX::Cvar::LoadCvar(ESP_TEXT, E_GrenadeBox, g_Options.Visuals.GrenadeBox) != false;
	g_Options.Visuals.Chicken = CSX::Cvar::LoadCvar(ESP_TEXT, E_Chicken, g_Options.Visuals.Chicken) != false;
	g_Options.Visuals.DroppedGuns = CSX::Cvar::LoadCvar(ESP_TEXT, E_DroppedGuns, g_Options.Visuals.DroppedGuns) != false;
	g_Options.Visuals.C4World = CSX::Cvar::LoadCvar(ESP_TEXT, E_C4World, g_Options.Visuals.C4World) != false;
	g_Options.Visuals.Weapon = CSX::Cvar::LoadCvar(ESP_TEXT, E_Weapon, g_Options.Visuals.Weapon);
	g_Options.Visuals.skeletonopts = CSX::Cvar::LoadCvar(ESP_TEXT, E_skeletonopts, g_Options.Visuals.skeletonopts);
	g_Options.Visuals.Grenades = CSX::Cvar::LoadCvar(ESP_TEXT, E_Grenades, g_Options.Visuals.Grenades);
	g_Options.Visuals.OffscreenIndicatorSize = CSX::Cvar::LoadCvar(ESP_TEXT, E_OffscreenIndicatorSize, g_Options.Visuals.OffscreenIndicatorSize);
	g_Options.Visuals.IsHasDefuser = CSX::Cvar::LoadCvar(ESP_TEXT, E_IsHasDefuser, g_Options.Visuals.IsHasDefuser) != false;
	g_Options.Visuals.Scoped = CSX::Cvar::LoadCvar(ESP_TEXT, E_Scoped, g_Options.Visuals.Scoped) != false;
	g_Options.Visuals.BombCarrier = CSX::Cvar::LoadCvar(ESP_TEXT, E_BombCarrier, g_Options.Visuals.BombCarrier) != false;
	g_Options.Visuals.Flashed = CSX::Cvar::LoadCvar(ESP_TEXT, E_Flashed, g_Options.Visuals.Flashed) != false;
	g_Options.Visuals.Distance = CSX::Cvar::LoadCvar(ESP_TEXT, E_Distance, g_Options.Visuals.Distance) != false;
	g_Options.Visuals.Money = CSX::Cvar::LoadCvar(ESP_TEXT, E_Money, g_Options.Visuals.Money) != false;
	g_Options.Visuals.IsReloading = CSX::Cvar::LoadCvar(ESP_TEXT, E_IsReloading, g_Options.Visuals.IsReloading) != false;
	g_Options.Visuals.Callout = CSX::Cvar::LoadCvar(ESP_TEXT, E_Callout, g_Options.Visuals.Callout) != false;
	g_Options.Visuals.Hitbox = CSX::Cvar::LoadCvar(ESP_TEXT, E_Hitbox, g_Options.Visuals.Hitbox) != false;
	g_Options.Visuals.HitboxDuration = CSX::Cvar::LoadCvar(ESP_TEXT, E_HitboxDuration, g_Options.Visuals.HitboxDuration);
	g_Options.Visuals.DroppedGunsType = CSX::Cvar::LoadCvar(ESP_TEXT, E_DroppedGunsType, g_Options.Visuals.DroppedGunsType);
	g_Options.Visuals.Armor2 = CSX::Cvar::LoadCvar(ESP_TEXT, E_Armor2, g_Options.Visuals.Armor2);


#pragma endregion
#pragma region L_Misc
	g_Options.Misc.FOVChanger = CSX::Cvar::LoadCvar(MISC_TEXT, M_FOVChanger, g_Options.Misc.FOVChanger);
	g_Options.Misc.viewmodelChanger = CSX::Cvar::LoadCvar(MISC_TEXT, M_viewmodelChanger, g_Options.Misc.viewmodelChanger);
	g_Options.Misc.NoFlash = CSX::Cvar::LoadCvar(MISC_TEXT, M_NoFlash, g_Options.Misc.NoFlash) != false;
	g_Options.Misc.NoSmoke = CSX::Cvar::LoadCvar(MISC_TEXT, M_NoSmoke, g_Options.Misc.NoSmoke) != false;
	g_Options.Misc.Hands = CSX::Cvar::LoadCvar(MISC_TEXT, M_Hands, g_Options.Misc.Hands);
	g_Options.Misc.ThirdPerson = CSX::Cvar::LoadCvar(MISC_TEXT, M_ThirdPerson, g_Options.Misc.ThirdPerson) != false;
	g_Options.Misc.Time = CSX::Cvar::LoadCvar(MISC_TEXT, M_Time, g_Options.Misc.Time) != false;
	g_Options.Misc.noscopeborder = CSX::Cvar::LoadCvar(MISC_TEXT, M_noscopeborder, g_Options.Misc.noscopeborder) != false;
	g_Options.Visuals.GrenadePrediction = CSX::Cvar::LoadCvar(MISC_TEXT, M_GrenadePrediction, g_Options.Visuals.GrenadePrediction) != false;
	g_Options.Misc.Hitmarker = CSX::Cvar::LoadCvar(MISC_TEXT, M_Hitmarker, g_Options.Misc.Hitmarker) != false;
	g_Options.Misc.silentstealer = CSX::Cvar::LoadCvar(MISC_TEXT, M_silentstealer, g_Options.Misc.silentstealer) != false;
	g_Options.Misc.hidename = CSX::Cvar::LoadCvar(MISC_TEXT, M_hidename, g_Options.Misc.hidename) != false;
	g_Options.Misc.Bhop = CSX::Cvar::LoadCvar(MISC_TEXT, M_Bhop, g_Options.Misc.Bhop) != false;
	g_Options.Misc.AutoStrafe = CSX::Cvar::LoadCvar(MISC_TEXT, M_AutoStrafe, g_Options.Misc.AutoStrafe) != false;
	g_Options.Misc.Ranks = CSX::Cvar::LoadCvar(MISC_TEXT, M_Ranks, g_Options.Misc.Ranks) != false;
	g_Options.Misc.NameSpammer = CSX::Cvar::LoadCvar(MISC_TEXT, M_NameSpammer, g_Options.Misc.NameSpammer) != false;
	g_Options.Misc.ChatSpamMode = CSX::Cvar::LoadCvar(MISC_TEXT, M_ChatSpamMode, g_Options.Misc.ChatSpamMode);
	g_Options.Misc.syncclantag = CSX::Cvar::LoadCvar(MISC_TEXT, M_syncclantag, g_Options.Misc.syncclantag);
	g_Options.Misc.SpecList = CSX::Cvar::LoadCvar(MISC_TEXT, M_SpecList, g_Options.Misc.SpecList) != false;
	g_Options.Misc.namespam = CSX::Cvar::LoadCvar(MISC_TEXT, M_namespam, g_Options.Misc.namespam) != false;
	g_Options.Misc.spammer = CSX::Cvar::LoadCvar(MISC_TEXT, M_spammer, g_Options.Misc.spammer);
	g_Options.Misc.AutoAccept = CSX::Cvar::LoadCvar(MISC_TEXT, M_AutoAccept, g_Options.Misc.AutoAccept) != false;
	g_Options.Misc.namespeed = CSX::Cvar::LoadCvar(MISC_TEXT, M_namespeed, g_Options.Misc.namespeed);
	g_Options.Misc.Crosshair = CSX::Cvar::LoadCvar(MISC_TEXT, M_Crosshair, g_Options.Misc.Crosshair) != false;
	g_Options.Misc.Crosshair2 = CSX::Cvar::LoadCvar(MISC_TEXT, M_Crosshair2, g_Options.Misc.Crosshair2) != false;
	g_Options.Misc.SkyShit = CSX::Cvar::LoadCvar(MISC_TEXT, M_SkyShit, g_Options.Misc.SkyShit) != false;
	g_Options.Misc.NoSky = CSX::Cvar::LoadCvar(MISC_TEXT, M_NoSky, g_Options.Misc.NoSky) != false;
	g_Options.Misc.AirlockKey = CSX::Cvar::LoadCvar(MISC_TEXT, M_AirlockKey, g_Options.Misc.AirlockKey);
	g_Options.Misc.ASUSWALL = CSX::Cvar::LoadCvar(MISC_TEXT, M_ASUSWALL, g_Options.Misc.ASUSWALL) != false;
	g_Options.Misc.handmat = CSX::Cvar::LoadCvar(MISC_TEXT, M_handmat, g_Options.Misc.handmat);
	g_Options.Misc.weaponhandmat = CSX::Cvar::LoadCvar(MISC_TEXT, M_weaponhandmat, g_Options.Misc.weaponhandmat);
	g_Options.Misc.Weaponslog = CSX::Cvar::LoadCvar(MISC_TEXT, M_WeaponsLog, g_Options.Misc.Weaponslog) != false;
	g_Options.Misc.DMGlog = CSX::Cvar::LoadCvar(MISC_TEXT, M_DMGLog, g_Options.Misc.DMGlog) != false;
	g_Options.Misc.TPKey = CSX::Cvar::LoadCvar(MISC_TEXT, M_TPKey, g_Options.Misc.TPKey);
	g_Options.Misc.Snow = CSX::Cvar::LoadCvar(MISC_TEXT, M_Snow, g_Options.Misc.Snow) != false;
	g_Options.Misc.postprocess = CSX::Cvar::LoadCvar(MISC_TEXT, M_postprocess, g_Options.Misc.postprocess) != false;
	g_Options.Misc.RCS = CSX::Cvar::LoadCvar(MISC_TEXT, M_RCS, g_Options.Misc.RCS) != false;
	g_Options.Misc.RCS_Distortion = CSX::Cvar::LoadCvar(MISC_TEXT, M_RCS_Distortion, g_Options.Misc.RCS_Distortion) != false;
	g_Options.Misc.NRCS = CSX::Cvar::LoadCvar(MISC_TEXT, M_NRCS, g_Options.Misc.NRCS);
	g_Options.Misc.RCS_NDistortion = CSX::Cvar::LoadCvar(MISC_TEXT, M_RCS_NDistortion, g_Options.Misc.RCS_NDistortion);
	g_Options.Misc.HandMat = CSX::Cvar::LoadCvar(MISC_TEXT, M_HandMat, g_Options.Misc.HandMat) != false;
	g_Options.Misc.WeaponsHandMat = CSX::Cvar::LoadCvar(MISC_TEXT, M_WeaponsHandMat, g_Options.Misc.WeaponsHandMat) != false;
	g_Options.Misc.Hitsound = CSX::Cvar::LoadCvar(MISC_TEXT, M_Hitsound, g_Options.Misc.Hitsound);
	g_Options.Misc.fakewalk = CSX::Cvar::LoadCvar(MISC_TEXT, M_FakeWalk, g_Options.Misc.fakewalk) != false;
	g_Options.Misc.fakewalkkey = CSX::Cvar::LoadCvar(MISC_TEXT, M_FakeWalkkey, g_Options.Misc.fakewalkkey);
	g_Options.Misc.RecoilCrosshair = CSX::Cvar::LoadCvar(MISC_TEXT, M_RecoilCrosshair, g_Options.Misc.RecoilCrosshair) != false;
	g_Options.Misc.bFreeCam = CSX::Cvar::LoadCvar(MISC_TEXT, M_bFreeCam, g_Options.Misc.bFreeCam) != false;
	g_Options.Misc.iFreeCamKey = CSX::Cvar::LoadCvar(MISC_TEXT, M_iFreeCamKey, g_Options.Misc.iFreeCamKey);
	g_Options.Misc.flFreeCamSpeed = CSX::Cvar::LoadCvar(MISC_TEXT, M_flFreeCamSpeed, g_Options.Misc.flFreeCamSpeed);
	g_Options.Misc.bBlockbot = CSX::Cvar::LoadCvar(MISC_TEXT, M_bBlockbot, g_Options.Misc.bBlockbot) != false;
	g_Options.Misc.iBlockbotKey = CSX::Cvar::LoadCvar(MISC_TEXT, M_iBlockbotKey, g_Options.Misc.iBlockbotKey) != false;
	g_Options.Misc.customodelsct = CSX::Cvar::LoadCvar(MISC_TEXT, M_customodelsct, g_Options.Misc.customodelsct);
	g_Options.Misc.customodelsct = CSX::Cvar::LoadCvar(MISC_TEXT, M_customodelst, g_Options.Misc.customodelst);
	g_Options.Misc.moonwalk = CSX::Cvar::LoadCvar(MISC_TEXT, M_moonwalk, g_Options.Misc.moonwalk) != false;
	g_Options.Misc.moonwalkkey = CSX::Cvar::LoadCvar(MISC_TEXT, M_moonwalkkey, g_Options.Misc.moonwalkkey);
	g_Options.Knifebot.knf_Active = CSX::Cvar::LoadCvar(MISC_TEXT, M_Knf_Active, g_Options.Knifebot.knf_Active) != false;
	g_Options.Knifebot.knf_Team = CSX::Cvar::LoadCvar(MISC_TEXT, M_knf_Team, g_Options.Knifebot.knf_Team) != false;
	g_Options.Knifebot.knf_Attack = CSX::Cvar::LoadCvar(MISC_TEXT, M_knf_Attack, g_Options.Knifebot.knf_Attack);
	g_Options.Knifebot.knf_DistAttack = CSX::Cvar::LoadCvar(MISC_TEXT, M_knf_DistAttack, g_Options.Knifebot.knf_DistAttack);
	g_Options.Knifebot.knf_DistAttack2 = CSX::Cvar::LoadCvar(MISC_TEXT, M_knf_DistAttack2, g_Options.Knifebot.knf_DistAttack2);

#pragma endregion
#pragma region L_Radar
	g_Options.Radar.radaringame = CSX::Cvar::LoadCvar(RADAR_TEXT, R_radaringame, g_Options.Radar.radaringame) != false;
	g_Options.Radar.radarwindow = CSX::Cvar::LoadCvar(RADAR_TEXT, R_radarwindow, g_Options.Radar.radarwindow) != false;
	g_Options.Radar.radrzoom = CSX::Cvar::LoadCvar(RADAR_TEXT, R_radrzoom, g_Options.Radar.radrzoom);
	g_Options.Radar.radralpha = CSX::Cvar::LoadCvar(RADAR_TEXT, R_radralpha, g_Options.Radar.radralpha);
	g_Options.Radar.radrsize = CSX::Cvar::LoadCvar(RADAR_TEXT, R_radrsize, g_Options.Radar.radrsize);

#pragma endregion

#pragma region L_Skins
	g_Options.Skinchanger.Enabled = CSX::Cvar::LoadCvar(SKINS_TEXT, S_Enabled, g_Options.Skinchanger.Enabled) != false;
	g_Options.Skinchanger.knifemodel = CSX::Cvar::LoadCvar(SKINS_TEXT, S_knifemodel, g_Options.Skinchanger.knifemodel);
	g_Options.Skinchanger.glove = CSX::Cvar::LoadCvar(SKINS_TEXT, S_glove, g_Options.Skinchanger.glove);
	g_Options.Skinchanger.gloveskin = CSX::Cvar::LoadCvar(SKINS_TEXT, S_gloveskin, g_Options.Skinchanger.gloveskin);
	g_Options.Menu.currentWeapon = CSX::Cvar::LoadCvar(SKINS_TEXT, CW_currentWeapon, g_Options.Menu.currentWeapon);
	g_Options.Skinchanger.EnabledSticker = CSX::Cvar::LoadCvar(SKINS_TEXT, S_EnabledSticker, g_Options.Skinchanger.EnabledSticker);

	for (DWORD i = 0; i < WEAPON_DATA_SIZE; i++)
	{
		g_Options.Skinchanger.W[pWeaponItemIndexData[i]].nFallbackPaintKit = CSX::Cvar::LoadCvar(SKINS_TEXT, pWeaponData[i], 0);
		g_Options.Skinchanger.W[pWeaponItemIndexData[i]].flFallbackWear = CSX::Cvar::LoadCvar(SKINS_Wear_TEXT, pWeaponData[i], 0);
		g_Options.Skinchanger.W[pWeaponItemIndexData[i]].iEntityQuality = CSX::Cvar::LoadCvar(SKINS_Quality_TEXT, pWeaponData[i], 0);
		g_Options.Skinchanger.W[pWeaponItemIndexData[i]].nFallbackStatTrak = CSX::Cvar::LoadCvar(SKINS_StatTrak_TEXT, pWeaponData[i], 0);
		g_Options.Skinchanger.W[pWeaponItemIndexData[i]].flFallbackSeed = CSX::Cvar::LoadCvar(SKINS_Seed_TEXT, pWeaponData[i], 0);
	}
	
	for (DWORD i = 0; i < Sticker_DATA_SIZE; i++)
	{
		g_Options.Sticker.Weapons[pStickersItemIndexData[i]].Sticker[0].ID = CSX::Cvar::LoadCvar(Sticker_Slot1_TEXT, pStickersData[i], 0);
		g_Options.Sticker.Weapons[pStickersItemIndexData[i]].Sticker[1].ID = CSX::Cvar::LoadCvar(Sticker_Slot2_TEXT, pStickersData[i], 0);
		g_Options.Sticker.Weapons[pStickersItemIndexData[i]].Sticker[2].ID = CSX::Cvar::LoadCvar(Sticker_Slot3_TEXT, pStickersData[i], 0);
		g_Options.Sticker.Weapons[pStickersItemIndexData[i]].Sticker[3].ID = CSX::Cvar::LoadCvar(Sticker_Slot4_TEXT, pStickersData[i], 0);
		g_Options.Sticker.Weapons[pStickersItemIndexData[i]].Sticker[4].ID = CSX::Cvar::LoadCvar(Sticker_Slot5_TEXT, pStickersData[i], 0);

	}
	
#pragma endregion

#pragma region L_Colors

	string esp_Ambient = CSX::Cvar::LoadCvar(COLORS_TEXT, C_esp_Ambient, "0,0,0");
	string backtrack = CSX::Cvar::LoadCvar(COLORS_TEXT, C_backtrack, "0,255,0");
	string skeleton = CSX::Cvar::LoadCvar(COLORS_TEXT, C_skeleton, "0,255,0");
	string hitbox = CSX::Cvar::LoadCvar(COLORS_TEXT, C_hitbox, "0,255,0");

	string esp_player_fakechams_color = CSX::Cvar::LoadCvar(COLORS_TEXT, C_fakechams, "255,0,0");

	string Crosshair = CSX::Cvar::LoadCvar(COLORS_TEXT, C_Crosshair, "255,0,0");
	string Scope = CSX::Cvar::LoadCvar(COLORS_TEXT, C_Scope, "255,0,0");
	string CrsSpread = CSX::Cvar::LoadCvar(COLORS_TEXT, C_CrsSpread, "255,255,255");
	string TeamSkeleton = CSX::Cvar::LoadCvar(COLORS_TEXT, C_TeamSkeleton, "255,255,255");
	string EnemySkeleton = CSX::Cvar::LoadCvar(COLORS_TEXT, C_EnemySkeleton, "255,255,0");
	string TeamSkeletonINVIS = CSX::Cvar::LoadCvar(COLORS_TEXT, C_TeamSkeletonINVIS, "255,0,0");
	string EnemySkeletonInVIS = CSX::Cvar::LoadCvar(COLORS_TEXT, C_EnemySkeletonInVIS, "255,0,0");
	string esp_Color_VCT = CSX::Cvar::LoadCvar(COLORS_TEXT, C_ESP_Color_VCT, "0,255,255");
	string esp_Color_VTT = CSX::Cvar::LoadCvar(COLORS_TEXT, C_ESP_Color_VTT, "0,255,0");
	string esp_Color_CT = CSX::Cvar::LoadCvar(COLORS_TEXT, C_ESP_Color_CT, "255,0,0");
	string esp_Color_TT = CSX::Cvar::LoadCvar(COLORS_TEXT, C_ESP_Color_TT, "255,0,0");
	string fill_color_team = CSX::Cvar::LoadCvar(COLORS_TEXT, C_fill_color_team, "255,0,0");
	string fill_color_enemy = CSX::Cvar::LoadCvar(COLORS_TEXT, C_fill_color_enemy, "255,0,0");

	string color_sniper = CSX::Cvar::LoadCvar(COLORS_TEXT, C_color_sniper, "255,0,0");
	string GrenadeCollision = CSX::Cvar::LoadCvar(COLORS_TEXT, C_GrenadeCollision, "255,0,0");
	string color_grenadeprediction_circle = CSX::Cvar::LoadCvar(COLORS_TEXT, C_color_grenadeprediction_circle, "255,0,0");
	string color_grenadeprediction = CSX::Cvar::LoadCvar(COLORS_TEXT, C_color_grenadeprediction, "255,255,255");


	string EnemyGlow = CSX::Cvar::LoadCvar(COLORS_TEXT, C_EnemyGlow, "255,0,0");
	string TeamGlow = CSX::Cvar::LoadCvar(COLORS_TEXT, C_TeamGlow, "0,255,0");
	string WeaponGlow = CSX::Cvar::LoadCvar(COLORS_TEXT, C_WeaponGlow, "255,0,0");
	string C4glow = CSX::Cvar::LoadCvar(COLORS_TEXT, C_C4glow, "0,255,0");
	string EnemyChamsVis = CSX::Cvar::LoadCvar(COLORS_TEXT, C_EnemyChamsVis, "255,0,0");
	string EnemyChamsNVis = CSX::Cvar::LoadCvar(COLORS_TEXT, C_EnemyChamsNVis, "255,0,0");
	string TeamChamsVis = CSX::Cvar::LoadCvar(COLORS_TEXT, C_TeamChamsVis, "0,255,0");
	string TeamChamsNVis = CSX::Cvar::LoadCvar(COLORS_TEXT, C_TeamChamsNVis, "255,0,0");
	string WeaponDropped = CSX::Cvar::LoadCvar(COLORS_TEXT, C_WeaponDropped, "255,0,0");
	string HandCham = CSX::Cvar::LoadCvar(COLORS_TEXT, C_HandCham, "255,0,0");
	string WeaponHandCham = CSX::Cvar::LoadCvar(COLORS_TEXT, C_WeaponHandCham, "255,0,0");
	string rad_Color_CT = CSX::Cvar::LoadCvar(COLORS_TEXT, C_rad_Color_CT, "0,255,255");
	string rad_Color_TT = CSX::Cvar::LoadCvar(COLORS_TEXT, C_rad_Color_TT, "255,0,0");
	string rad_Color_VCT = CSX::Cvar::LoadCvar(COLORS_TEXT, C_rad_Color_VCT, "255,0,0");
	string rad_Color_VTT = CSX::Cvar::LoadCvar(COLORS_TEXT, C_rad_Color_VTT, "255,0,0");
	string Light_TT = CSX::Cvar::LoadCvar(COLORS_TEXT, C_Light_TT, "255,0,0");
	string Light_CT = CSX::Cvar::LoadCvar(COLORS_TEXT, C_Light_CT, "0,0,255");
	string WorldMode = CSX::Cvar::LoadCvar(COLORS_TEXT, C_WorldMode, "255,255,255");
	string flTracers = CSX::Cvar::LoadCvar(COLORS_TEXT, C_flTracers, "255,0,0");

	g_Options.Color.rad_Alpha = CSX::Cvar::LoadCvar(COLORS_TEXT, C_rad_Alpha, g_Options.Color.rad_Alpha);
	g_Options.Color.champlayeralpha = CSX::Cvar::LoadCvar(COLORS_TEXT, C_champlayeralpha, g_Options.Color.champlayeralpha);
	g_Options.Color.chamhandalpha = CSX::Cvar::LoadCvar(COLORS_TEXT, C_chamhandalpha, g_Options.Color.chamhandalpha);
	g_Options.Color.weaponhandalpha = CSX::Cvar::LoadCvar(COLORS_TEXT, C_weaponhandalpha, g_Options.Color.weaponhandalpha);
	g_Options.Color.weaponglowalpha = CSX::Cvar::LoadCvar(COLORS_TEXT, C_weaponglowalpha, g_Options.Color.weaponglowalpha);
	g_Options.Color.playerglowalpha = CSX::Cvar::LoadCvar(COLORS_TEXT, C_playerglowalpha, g_Options.Color.playerglowalpha);
	g_Options.Color.bombglowalpha = CSX::Cvar::LoadCvar(COLORS_TEXT, C_bombglowalpha, g_Options.Color.bombglowalpha);
	g_Options.Color.weaponsworldalph = CSX::Cvar::LoadCvar(COLORS_TEXT, C_weaponsworldalph, g_Options.Color.weaponsworldalph);
	g_Options.Color.weponchamsalph = CSX::Cvar::LoadCvar(COLORS_TEXT, C_weponchamsalph, g_Options.Color.weponchamsalph);

	g_Render->ScanColorFromCvar(esp_Ambient.c_str(), g_Options.Color.esp_Ambient);
	g_Render->ScanColorFromCvar(backtrack.c_str(), g_Options.Color.backtrack);
	g_Render->ScanColorFromCvar(skeleton.c_str(), g_Options.Color.color_skeleton);
	g_Render->ScanColorFromCvar(hitbox.c_str(), g_Options.Color.hitbox);

	g_Render->ScanColorFromCvar(esp_player_fakechams_color.c_str(), g_Options.Color.esp_player_fakechams_color);

	g_Render->ScanColorFromCvar(Crosshair.c_str(), g_Options.Color.Crosshair);
	g_Render->ScanColorFromCvar(Scope.c_str(), g_Options.Color.Scope);
	g_Render->ScanColorFromCvar(CrsSpread.c_str(), g_Options.Color.CrsSpread);
	g_Render->ScanColorFromCvar(TeamSkeleton.c_str(), g_Options.Color.TeamSkeleton);
	g_Render->ScanColorFromCvar(EnemySkeleton.c_str(), g_Options.Color.EnemySkeleton);
	g_Render->ScanColorFromCvar(TeamSkeletonINVIS.c_str(), g_Options.Color.TeamSkeletonINVIS);
	g_Render->ScanColorFromCvar(EnemySkeletonInVIS.c_str(), g_Options.Color.EnemySkeletonInVIS);
	g_Render->ScanColorFromCvar(esp_Color_VCT.c_str(), g_Options.Color.esp_Color_VCT);
	g_Render->ScanColorFromCvar(esp_Color_VTT.c_str(), g_Options.Color.esp_Color_VTT);
	g_Render->ScanColorFromCvar(esp_Color_CT.c_str(), g_Options.Color.esp_Color_CT);
	g_Render->ScanColorFromCvar(esp_Color_TT.c_str(), g_Options.Color.esp_Color_TT);
	g_Render->ScanColorFromCvar(fill_color_team.c_str(), g_Options.Color.fill_color_team);
	g_Render->ScanColorFromCvar(fill_color_enemy.c_str(), g_Options.Color.fill_color_enemy);
	g_Render->ScanColorFromCvar(color_sniper.c_str(), g_Options.Color.color_sniper);
	g_Render->ScanColorFromCvar(GrenadeCollision.c_str(), g_Options.Color.GrenadeCollision);
	g_Render->ScanColorFromCvar(color_grenadeprediction_circle.c_str(), g_Options.Color.color_grenadeprediction_circle);
	g_Render->ScanColorFromCvar(color_grenadeprediction.c_str(), g_Options.Color.color_grenadeprediction);

	g_Render->ScanColorFromCvar(EnemyGlow.c_str(), g_Options.Color.EnemyGlow);
	g_Render->ScanColorFromCvar(TeamGlow.c_str(), g_Options.Color.TeamGlow);
	g_Render->ScanColorFromCvar(WeaponGlow.c_str(), g_Options.Color.WeaponGlow);
	g_Render->ScanColorFromCvar(C4glow.c_str(), g_Options.Color.C4glow);
	g_Render->ScanColorFromCvar(EnemyChamsVis.c_str(), g_Options.Color.EnemyChamsVis);
	g_Render->ScanColorFromCvar(EnemyChamsNVis.c_str(), g_Options.Color.EnemyChamsNVis);
	g_Render->ScanColorFromCvar(TeamChamsVis.c_str(), g_Options.Color.TeamChamsVis);
	g_Render->ScanColorFromCvar(TeamChamsNVis.c_str(), g_Options.Color.TeamChamsNVis);
	g_Render->ScanColorFromCvar(WeaponDropped.c_str(), g_Options.Color.WeaponDropped);
	g_Render->ScanColorFromCvar(HandCham.c_str(), g_Options.Color.HandCham);
	g_Render->ScanColorFromCvar(WeaponHandCham.c_str(), g_Options.Color.WeaponHandCham);
	g_Render->ScanColorFromCvar(rad_Color_CT.c_str(), g_Options.Color.rad_Color_CT);
	g_Render->ScanColorFromCvar(rad_Color_TT.c_str(), g_Options.Color.rad_Color_TT);
	g_Render->ScanColorFromCvar(rad_Color_VCT.c_str(), g_Options.Color.rad_Color_VCT);
	g_Render->ScanColorFromCvar(rad_Color_VTT.c_str(), g_Options.Color.rad_Color_VTT);
	g_Render->ScanColorFromCvar(Light_TT.c_str(), g_Options.Color.Light_TT);
	g_Render->ScanColorFromCvar(Light_CT.c_str(), g_Options.Color.Light_CT);
	g_Render->ScanColorFromCvar(WorldMode.c_str(), g_Options.Color.worldmode);
	g_Render->ScanColorFromCvar(flTracers.c_str(), g_Options.Color.flTracers);


#pragma endregion

#pragma region L_Settings
	g_Options.Menu.Theme = CSX::Cvar::LoadCvar(SETTINGS_TEXT, St_Themes, g_Options.Menu.Theme);

#pragma endregion

}
void SaveSettings(std::string szIniFile)
{
	CSX::Cvar::InitPath(szIniFile.c_str());

#pragma region S_Ragebot


	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_MainSwitch, g_Options.Ragebot.MainSwitch);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_Enabled, g_Options.Ragebot.Enabled);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_AutoFire, g_Options.Ragebot.AutoFire);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_TargetSelection, g_Options.Ragebot.TargetSelection);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_FOV, g_Options.Ragebot.FOV);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_Silent, g_Options.Ragebot.Silent);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_AutoPistol, g_Options.Ragebot.AutoPistol);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_KeyPress, g_Options.Ragebot.KeyPress);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_EnabledAntiAim, g_Options.Ragebot.EnabledAntiAim);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_PreAAs, g_Options.Ragebot.PreAAs);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_Pitch, g_Options.Ragebot.Pitch);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_YawTrue, g_Options.Ragebot.YawTrue);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_YawFake, g_Options.Ragebot.YawFake);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_YawTrueAdder, g_Options.Ragebot.YawTrueAdder);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_YawFakeAdder, g_Options.Ragebot.YawFakeAdder);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_PitchAdder, g_Options.Ragebot.PitchAdder);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_AtTarget, g_Options.Ragebot.AtTarget);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_Edge, g_Options.Ragebot.Edge);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_KnifeAA, g_Options.Ragebot.KnifeAA);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_BuilderAAs, g_Options.Ragebot.BuilderAAs);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_Jitter, g_Options.Ragebot.Jitter);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_FJitter, g_Options.Ragebot.FJitter);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_LBYBreaker, g_Options.Ragebot.LBYBreaker);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_BuilderReal, g_Options.Ragebot.BuilderReal);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_BuilderFake, g_Options.Ragebot.BuilderFake);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_BuilderPitch, g_Options.Ragebot.BuilderPitch);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_JitterRange, g_Options.Ragebot.JitterRange);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_FJitterRange, g_Options.Ragebot.FJitterRange);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_FriendlyFire, g_Options.Ragebot.FriendlyFire);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_autowall, g_Options.Ragebot.autowall);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_Hitbox, g_Options.Ragebot.Hitbox);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_Hitscan, g_Options.Ragebot.Hitscan);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_AntiRecoil, g_Options.Ragebot.AntiRecoil);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_AntiSpread, g_Options.Ragebot.AntiSpread);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_AutoStop, g_Options.Ragebot.AutoStop);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_AutoCrouch, g_Options.Ragebot.AutoCrouch);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_AutoScope, g_Options.Ragebot.AutoScope);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_AIFakeLag, g_Options.Ragebot.AIFakeLag);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_AIFakeLagAmt, g_Options.Ragebot.AIFakeLagAmt);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_Hitchance, g_Options.Ragebot.Hitchance);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_Resolver, g_Options.Ragebot.Resolver);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_FakeLagFix, g_Options.Ragebot.FakeLagFix);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_PosAdjust, g_Options.Ragebot.PosAdjust);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_playerlist, g_Options.Ragebot.playerlist);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_BAIMkey, g_Options.Ragebot.BAIMkey);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_bruteAfterX, g_Options.Ragebot.bruteAfterX);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_AntiSmac, g_Options.Ragebot.AntiSmac);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_MinimumDamageSniper, g_Options.Ragebot.MinimumDamageSniper);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_MinimumDamageRifle, g_Options.Ragebot.MinimumDamageRifle);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_MinimumDamagePistol, g_Options.Ragebot.MinimumDamagePistol);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_MinimumDamageHeavy, g_Options.Ragebot.MinimumDamageHeavy);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_MinimumDamageSmg, g_Options.Ragebot.MinimumDamageSmg);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_MinimumDamageRevolver, g_Options.Ragebot.MinimumDamageRevolver);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_HitchanceSniper, g_Options.Ragebot.HitchanceSniper);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_HitchancePistol, g_Options.Ragebot.HitchancePistol);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_HitchanceRifle, g_Options.Ragebot.HitchanceRifle);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_HitchanceHeavy, g_Options.Ragebot.HitchanceHeavy);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_HitchanceSmgs, g_Options.Ragebot.HitchanceSmgs);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_HitchanceRevolver, g_Options.Ragebot.HitchanceRevolver);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_Multipoint, g_Options.Ragebot.Multipoint);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_Pointscalex, g_Options.Ragebot.Pointscalex);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_Pointscaley, g_Options.Ragebot.Pointscaley);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_HitscanInvis, g_Options.Ragebot.hitscaninvis);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_FakeLagType, g_Options.Ragebot.FakeLagType);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_Fakelag, g_Options.Ragebot.FakeLag);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_FakeLagAmt, g_Options.Ragebot.FakeLagAmt);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_FakelagMode, g_Options.Ragebot.FakeLagMode);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_AimbotResolver, g_Options.Ragebot.AimbotResolver);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_Resolver3, g_Options.Ragebot.Resolver3);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_LBYCorrection, g_Options.Ragebot.LBYCorrection);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_LBYCorrection2, g_Options.Ragebot.LBYCorrection2);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_RageINFO, g_Options.Ragebot.RageDraw);
	CSX::Cvar::SaveCvar(RAGETBOT_TEXT, R_flipkey, g_Options.Ragebot.flipkey);


#pragma endregion
#pragma region S_Ligetbot

	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_MainSwitch, g_Options.Legitbot.MainSwitch);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_backtrack, g_Options.Legitbot.backtrack);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_backtrackSize, g_Options.Legitbot.backtrack_size);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_backtrackTicks, g_Options.Legitbot.backtrackTicks);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_Enabled, g_Options.Legitbot.Aimbot.Enabled);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_ALegitAA, g_Options.Legitbot.Aimbot.ALegitAA);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_checksmoke, g_Options.Legitbot.checksmoke);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_LAA, g_Options.Legitbot.Aimbot.EnableLegitAA);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_AAAngle, g_Options.Legitbot.Aimbot.AAAngle);

	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_PistolKey, g_Options.Legitbot.PistolKey);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_Pistolfov, g_Options.Legitbot.Pistolfov);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_Pistol_FakeLag, g_Options.Legitbot.Pistol_fakelag);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_Pistol_FakeLag_Mode, g_Options.Legitbot.Pistol_fakelag_mode);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_Pistol_FakeLag_Value, g_Options.Legitbot.Pistol_fakelag_value);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_PistolSmooth, g_Options.Legitbot.Pistol_Smooth);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_PistolHitbox, g_Options.Legitbot.hitboxpistol);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_pistol_rcs, g_Options.Legitbot.Pistol_rcs);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_pistol_mode, g_Options.Legitbot.Pistol_RcsType);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_pistol_AutoPistol, g_Options.Legitbot.Aimbot.AutoPistol);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_pistol_AutoLock, g_Options.Legitbot.Pistol_AutoLock);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_Pistol_Silent, g_Options.Legitbot.Pistol_Silent);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_pistol_TriggerbotKey, g_Options.Legitbot.Pistol_TriggerbotKey);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_pistol_Triggerbot_hitchance, g_Options.Legitbot.Pistol_Triggerbot_hitchance);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_Pistol_Triggerbot_AutoFire, g_Options.Legitbot.Pistol_Triggerbot_AutoFire);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_Pistol_Triggerbot, g_Options.Legitbot.Pistol_Triggerbot);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_Pistol_Triggerbot_Arms, g_Options.Legitbot.Pistol_TriggerbotFilter.Arms);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_Pistol_Triggerbot_Chest, g_Options.Legitbot.Pistol_TriggerbotFilter.Chest);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_Pistol_Triggerbot_Head, g_Options.Legitbot.Pistol_TriggerbotFilter.Head);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_Pistol_Triggerbot_Legs, g_Options.Legitbot.Pistol_TriggerbotFilter.Legs);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_Pistol_Triggerbot_Stomach, g_Options.Legitbot.Pistol_TriggerbotFilter.Stomach);

	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_heavyKey, g_Options.Legitbot.heavyKey);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_heavyfov, g_Options.Legitbot.heavyfov);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_heavy_FakeLag, g_Options.Legitbot.heavy_fakelag);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_heavy_FakeLag_Mode, g_Options.Legitbot.heavy_fakelag_mode);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_heavy_FakeLag_Value, g_Options.Legitbot.heavy_fakelag_value);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_heavySmooth, g_Options.Legitbot.heavy_Smooth);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_heavyHitbox, g_Options.Legitbot.hitboxheavy);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_heavy_rcs, g_Options.Legitbot.heavy_rcs);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_heavy_mode, g_Options.Legitbot.heavy_RcsType);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_heavy_AutoLock, g_Options.Legitbot.heavy_AutoLock);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_heavy_Silent, g_Options.Legitbot.heavy_Silent);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_heavy_TriggerbotKey, g_Options.Legitbot.heavy_TriggerbotKey);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_heavy_Triggerbot_hitchance, g_Options.Legitbot.heavy_Triggerbot_hitchance);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_heavy_Triggerbot_AutoFire, g_Options.Legitbot.heavy_Triggerbot_AutoFire);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_heavy_Triggerbot, g_Options.Legitbot.heavy_Triggerbot);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_heavy_Triggerbot_Arms, g_Options.Legitbot.heavy_TriggerbotFilter.Arms);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_heavy_Triggerbot_Chest, g_Options.Legitbot.heavy_TriggerbotFilter.Chest);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_heavy_Triggerbot_Head, g_Options.Legitbot.heavy_TriggerbotFilter.Head);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_heavy_Triggerbot_Legs, g_Options.Legitbot.heavy_TriggerbotFilter.Legs);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_heavy_Triggerbot_Stomach, g_Options.Legitbot.heavy_TriggerbotFilter.Stomach);

	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_smgKey, g_Options.Legitbot.smgKey);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_smgfov, g_Options.Legitbot.smgfov);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_smg_FakeLag, g_Options.Legitbot.smg_fakelag);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_smg_FakeLag_Mode, g_Options.Legitbot.smg_fakelag_mode);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_smg_FakeLag_Value, g_Options.Legitbot.smg_fakelag_value);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_smgSmooth, g_Options.Legitbot.smg_Smooth);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_smgHitbox, g_Options.Legitbot.hitboxsmg);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_smg_rcs, g_Options.Legitbot.smg_rcs);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_smg_mode, g_Options.Legitbot.smg_RcsType);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_smg_AutoLock, g_Options.Legitbot.smg_AutoLock);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_smg_Silent, g_Options.Legitbot.smg_Silent);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_smg_TriggerbotKey, g_Options.Legitbot.smg_TriggerbotKey);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_smg_Triggerbot_hitchance, g_Options.Legitbot.smg_Triggerbot_hitchance);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_smg_Triggerbot_AutoFire, g_Options.Legitbot.smg_Triggerbot_AutoFire);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_smg_Triggerbot, g_Options.Legitbot.smg_Triggerbot);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_smg_Triggerbot_Arms, g_Options.Legitbot.smg_TriggerbotFilter.Arms);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_smg_Triggerbot_Chest, g_Options.Legitbot.smg_TriggerbotFilter.Chest);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_smg_Triggerbot_Head, g_Options.Legitbot.smg_TriggerbotFilter.Head);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_smg_Triggerbot_Legs, g_Options.Legitbot.smg_TriggerbotFilter.Legs);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_smg_Triggerbot_Stomach, g_Options.Legitbot.smg_TriggerbotFilter.Stomach);

	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_MainKey, g_Options.Legitbot.MainKey);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_Mainfov, g_Options.Legitbot.Mainfov);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_main_FakeLag, g_Options.Legitbot.main_fakelag);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_main_FakeLag_Mode, g_Options.Legitbot.main_fakelag_mode);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_main_FakeLag_Value, g_Options.Legitbot.main_fakelag_value);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_MainSmooth, g_Options.Legitbot.Main_Smooth);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_MainHitbox, g_Options.Legitbot.hitboxrifle);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_main_rcs, g_Options.Legitbot.main_rcs);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_main_mode, g_Options.Legitbot.main_RcsType);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_main_AutoLock, g_Options.Legitbot.main_AutoLock);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_main_Silent, g_Options.Legitbot.main_Silent);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_main_TriggerbotKey, g_Options.Legitbot.main_TriggerbotKey);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_main_Triggerbot_hitchance, g_Options.Legitbot.main_Triggerbot_hitchance);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_main_Triggerbot_AutoFire, g_Options.Legitbot.main_Triggerbot_AutoFire);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_main_Triggerbot, g_Options.Legitbot.main_Triggerbot);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_main_Triggerbot_Arms, g_Options.Legitbot.main_TriggerbotFilter.Arms);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_main_Triggerbot_Chest, g_Options.Legitbot.main_TriggerbotFilter.Chest);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_main_Triggerbot_Head, g_Options.Legitbot.main_TriggerbotFilter.Head);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_main_Triggerbot_Legs, g_Options.Legitbot.main_TriggerbotFilter.Legs);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_main_Triggerbot_Stomach, g_Options.Legitbot.main_TriggerbotFilter.Stomach);

	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_SniperKey, g_Options.Legitbot.SniperKey);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_Sniperfov, g_Options.Legitbot.Sniperfov);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_sniper_FakeLag, g_Options.Legitbot.Sniper_fakelag);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_sniper_FakeLag_Mode, g_Options.Legitbot.Sniper_fakelag_mode);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_sniper_FakeLag_Value, g_Options.Legitbot.Sniper_fakelag_value);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_SniperSmooth, g_Options.Legitbot.Sniper_Smooth);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_SniperHitbox, g_Options.Legitbot.hitboxsniper);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_sniper_rcs, g_Options.Legitbot.sniper_rcs);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_sniper_mode, g_Options.Legitbot.sniper_RcsType);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_sniper_fastzoom, g_Options.Legitbot.Aimbot.fastzoom);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_sniper_fastzoomswitch, g_Options.Legitbot.Aimbot.fastzoomswitch);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_sniper_AutoLock, g_Options.Legitbot.sniper_AutoLock);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_sniper_Silent, g_Options.Legitbot.sniper_Silent);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_sniper_TriggerbotKey, g_Options.Legitbot.sniper_TriggerbotKey);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_sniper_Triggerbot_hitchance, g_Options.Legitbot.sniper_Triggerbot_hitchance);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_sniper_Triggerbot_AutoFire, g_Options.Legitbot.sniper_Triggerbot_AutoFire);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_sniper_Triggerbot, g_Options.Legitbot.sniper_Triggerbot);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_sniper_Triggerbot_Arms, g_Options.Legitbot.sniper_TriggerbotFilter.Arms);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_sniper_Triggerbot_Chest, g_Options.Legitbot.sniper_TriggerbotFilter.Chest);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_sniper_Triggerbot_Head, g_Options.Legitbot.sniper_TriggerbotFilter.Head);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_sniper_Triggerbot_Legs, g_Options.Legitbot.sniper_TriggerbotFilter.Legs);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_sniper_Triggerbot_Stomach, g_Options.Legitbot.sniper_TriggerbotFilter.Stomach);

	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_RevolverKey, g_Options.Legitbot.RevolverKey);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_Revolverfov, g_Options.Legitbot.Revolverfov);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_Revolver_FakeLag, g_Options.Legitbot.Revolver_fakelag);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_Revolver_FakeLag_Mode, g_Options.Legitbot.Revolver_fakelag_mode);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_Revolver_FakeLag_Value, g_Options.Legitbot.Revolver_fakelag_value);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_RevolverSmooth, g_Options.Legitbot.Revolver_Smooth);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_RevolverHitbox, g_Options.Legitbot.hitboxRevolver);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_Revolver_rcs, g_Options.Legitbot.Revolver_rcs);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_Revolver_mode, g_Options.Legitbot.Revolver_RcsType);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_Revolver_AutoLock, g_Options.Legitbot.Revolver_AutoLock);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_Revolver_Silent, g_Options.Legitbot.Revolver_Silent);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_Revolver_TriggerbotKey, g_Options.Legitbot.Revolver_TriggerbotKey);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_Revolver_Triggerbot_hitchance, g_Options.Legitbot.Revolver_Triggerbot_hitchance);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_Revolver_Triggerbot_AutoFire, g_Options.Legitbot.Revolver_Triggerbot_AutoFire);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_Revolver_Triggerbot, g_Options.Legitbot.Revolver_Triggerbot);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_Revolver_Triggerbot_Arms, g_Options.Legitbot.Revolver_TriggerbotFilter.Arms);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_Revolver_Triggerbot_Chest, g_Options.Legitbot.Revolver_TriggerbotFilter.Chest);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_Revolver_Triggerbot_Head, g_Options.Legitbot.Revolver_TriggerbotFilter.Head);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_Revolver_Triggerbot_Legs, g_Options.Legitbot.Revolver_TriggerbotFilter.Legs);
	CSX::Cvar::SaveCvar(LIGETBOT_TEXT, L_Revolver_Triggerbot_Stomach, g_Options.Legitbot.Revolver_TriggerbotFilter.Stomach);

#pragma endregion
#pragma region S_ESP
	CSX::Cvar::SaveCvar(ESP_TEXT, E_Enabled, g_Options.Visuals.Enabled);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_Box, g_Options.Visuals.Box);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_AimSpot, g_Options.Visuals.AimSpot);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_DLight, g_Options.Visuals.DLight);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_glowplayer, g_Options.Visuals.glowplayer);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_Weapons, g_Options.Visuals.Weapons);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_LBYIndicator, g_Options.Visuals.LBYIndicator);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_Chams, g_Options.Visuals.Chams);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_Teamchams, g_Options.Visuals.Teamchams);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_XQZ, g_Options.Visuals.XQZ);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_weaponglow, g_Options.Visuals.weaponglow);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_teamglow, g_Options.Visuals.teamglow);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_bombglow, g_Options.Visuals.bombglow);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_BoxType, g_Options.Visuals.BoxType);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_TeamESP, g_Options.Visuals.TeamESP);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_AimLine, g_Options.Visuals.AimLine);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_health, g_Options.Visuals.health);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_barrel, g_Options.Visuals.barrel);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_DrawAwall, g_Options.Visuals.DrawAwall);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_droppedchams, g_Options.Visuals.WeaponsDroppedChams);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_barrelL, g_Options.Visuals.barrelL);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_Playermatierial, g_Options.Visuals.Playermatierial);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_Weaponsdroppedmatierial, g_Options.Visuals.Weaponsdroppedmatierial);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_angleLines, g_Options.Visuals.angleLines);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_esp_Visible, g_Options.Visuals.esp_Visible);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_esp_corpses, g_Options.Visuals.corpses);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_esp_fillbox, g_Options.Visuals.fill);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_esp_armor, g_Options.Visuals.armor);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_esp_bulletshow, g_Options.Visuals.bulletshow);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_esp_flTracersDuration, g_Options.Visuals.flTracersDuration);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_esp_flTracersWidth, g_Options.Visuals.flTracersWidth);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_esp_healthtype, g_Options.Visuals.healthtype);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_esp_chamsHp, g_Options.Visuals.chamsHp);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_esp_fill_amount, g_Options.Visuals.esp_fill_amount);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_chams_fakeangle, g_Options.Visuals.esp_player_chams_fakeangle);
    CSX::Cvar::SaveCvar(ESP_TEXT, E_AWallCrosshair, g_Options.Visuals.AWallCrosshair);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_SniperCrosshair, g_Options.Visuals.SniperCrosshair);
    CSX::Cvar::SaveCvar(ESP_TEXT, E_SniperCrosshairType, g_Options.Visuals.SniperCrosshairType);
    CSX::Cvar::SaveCvar(ESP_TEXT, E_CrosshairOn, g_Options.Visuals.CrosshairOn);
    CSX::Cvar::SaveCvar(ESP_TEXT, E_CrosshairSpread, g_Options.Visuals.CrosshairSpread);
    CSX::Cvar::SaveCvar(ESP_TEXT, E_CrosshairType, g_Options.Visuals.CrosshairType);
    CSX::Cvar::SaveCvar(ESP_TEXT, E_CrosshairStyle, g_Options.Visuals.CrosshairStyle);
    CSX::Cvar::SaveCvar(ESP_TEXT, E_CrosshairRainbow, g_Options.Visuals.CrosshairRainbow);

	CSX::Cvar::SaveCvar(ESP_TEXT, E_Name, g_Options.Visuals.Name);
    CSX::Cvar::SaveCvar(ESP_TEXT, E_Skeleton, g_Options.Visuals.skeletonenbl);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_OffscreenIndicator, g_Options.Visuals.OffscreenIndicator);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_GrenadeBox, g_Options.Visuals.GrenadeBox);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_Chicken, g_Options.Visuals.Chicken);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_DroppedGuns, g_Options.Visuals.DroppedGuns);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_C4World, g_Options.Visuals.C4World);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_Weapon, g_Options.Visuals.Weapon);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_skeletonopts, g_Options.Visuals.skeletonopts);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_Grenades, g_Options.Visuals.Grenades);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_OffscreenIndicatorSize, g_Options.Visuals.OffscreenIndicatorSize);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_IsHasDefuser, g_Options.Visuals.IsHasDefuser);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_Scoped, g_Options.Visuals.Scoped);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_BombCarrier, g_Options.Visuals.BombCarrier);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_Flashed, g_Options.Visuals.Flashed);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_Distance, g_Options.Visuals.Distance);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_Money, g_Options.Visuals.Money);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_IsReloading, g_Options.Visuals.IsReloading);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_Callout, g_Options.Visuals.Callout);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_Hitbox, g_Options.Visuals.Hitbox);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_HitboxDuration, g_Options.Visuals.HitboxDuration);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_DroppedGunsType, g_Options.Visuals.DroppedGunsType);
	CSX::Cvar::SaveCvar(ESP_TEXT, E_Armor2, g_Options.Visuals.Armor2);

#pragma endregion
#pragma region S_Misc
	CSX::Cvar::SaveCvar(MISC_TEXT, M_FOVChanger, g_Options.Misc.FOVChanger);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_viewmodelChanger, g_Options.Misc.viewmodelChanger);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_NoFlash, g_Options.Misc.NoFlash);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_NoSmoke, g_Options.Misc.NoSmoke);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_Hands, g_Options.Misc.Hands);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_ThirdPerson, g_Options.Misc.ThirdPerson);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_Time, g_Options.Misc.Time);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_noscopeborder, g_Options.Misc.noscopeborder);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_GrenadePrediction, g_Options.Visuals.GrenadePrediction);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_Hitmarker, g_Options.Misc.Hitmarker);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_silentstealer, g_Options.Misc.silentstealer);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_hidename, g_Options.Misc.hidename);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_Bhop, g_Options.Misc.Bhop);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_AutoStrafe, g_Options.Misc.AutoStrafe);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_Ranks, g_Options.Misc.Ranks);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_NameSpammer, g_Options.Misc.NameSpammer);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_ChatSpamMode, g_Options.Misc.ChatSpamMode);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_syncclantag, g_Options.Misc.syncclantag);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_SpecList, g_Options.Misc.SpecList);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_namespam, g_Options.Misc.namespam);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_spammer, g_Options.Misc.spammer);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_AutoAccept, g_Options.Misc.AutoAccept);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_namespeed, g_Options.Misc.namespeed);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_Crosshair, g_Options.Misc.Crosshair);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_Crosshair2, g_Options.Misc.Crosshair2);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_SkyShit, g_Options.Misc.SkyShit);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_NoSky, g_Options.Misc.NoSky);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_AirlockKey, g_Options.Misc.AirlockKey);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_ASUSWALL, g_Options.Misc.ASUSWALL);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_handmat, g_Options.Misc.handmat);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_weaponhandmat, g_Options.Misc.weaponhandmat);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_WeaponsLog, g_Options.Misc.Weaponslog);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_DMGLog, g_Options.Misc.DMGlog);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_TPKey, g_Options.Misc.TPKey);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_Snow, g_Options.Misc.Snow);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_postprocess, g_Options.Misc.postprocess);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_RCS, g_Options.Misc.RCS);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_RCS_Distortion, g_Options.Misc.RCS_Distortion);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_NRCS, g_Options.Misc.NRCS);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_RCS_NDistortion, g_Options.Misc.RCS_NDistortion);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_HandMat, g_Options.Misc.HandMat);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_WeaponsHandMat, g_Options.Misc.WeaponsHandMat);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_Hitsound, g_Options.Misc.Hitsound);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_FakeWalk, g_Options.Misc.fakewalk);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_FakeWalkkey, g_Options.Misc.fakewalkkey);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_RecoilCrosshair, g_Options.Misc.RecoilCrosshair);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_bFreeCam, g_Options.Misc.bFreeCam);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_iFreeCamKey, g_Options.Misc.iFreeCamKey);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_flFreeCamSpeed, g_Options.Misc.flFreeCamSpeed);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_bBlockbot, g_Options.Misc.bBlockbot);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_iBlockbotKey, g_Options.Misc.iBlockbotKey);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_customodelsct, g_Options.Misc.customodelsct);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_customodelst, g_Options.Misc.customodelst);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_moonwalk, g_Options.Misc.moonwalk);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_moonwalkkey, g_Options.Misc.moonwalkkey);
    CSX::Cvar::SaveCvar(MISC_TEXT, M_Knf_Active, g_Options.Knifebot.knf_Active);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_knf_Team, g_Options.Knifebot.knf_Team);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_knf_Attack, g_Options.Knifebot.knf_Attack);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_knf_DistAttack, g_Options.Knifebot.knf_DistAttack);
	CSX::Cvar::SaveCvar(MISC_TEXT, M_knf_DistAttack2, g_Options.Knifebot.knf_DistAttack2);

#pragma endregion
#pragma region S_Radar
	 CSX::Cvar::SaveCvar(RADAR_TEXT, R_radaringame, g_Options.Radar.radaringame);
	 CSX::Cvar::SaveCvar(RADAR_TEXT, R_radarwindow, g_Options.Radar.radarwindow);
	 CSX::Cvar::SaveCvar(RADAR_TEXT, R_radrzoom, g_Options.Radar.radrzoom);
	 CSX::Cvar::SaveCvar(RADAR_TEXT, R_radralpha, g_Options.Radar.radralpha);
	 CSX::Cvar::SaveCvar(RADAR_TEXT, R_radrsize, g_Options.Radar.radrsize);
#pragma endregion
#pragma region S_Skins
	CSX::Cvar::SaveCvar(SKINS_TEXT, S_Enabled, g_Options.Skinchanger.Enabled);
	CSX::Cvar::SaveCvar(SKINS_TEXT, S_knifemodel, g_Options.Skinchanger.knifemodel);
	CSX::Cvar::SaveCvar(SKINS_TEXT, S_glove, g_Options.Skinchanger.glove);
	CSX::Cvar::SaveCvar(SKINS_TEXT, S_gloveskin, g_Options.Skinchanger.gloveskin);
	CSX::Cvar::SaveCvar(SKINS_TEXT, CW_currentWeapon, g_Options.Menu.currentWeapon);
	CSX::Cvar::SaveCvar(SKINS_TEXT, S_EnabledSticker, g_Options.Skinchanger.EnabledSticker);

	for (DWORD i = 0; i < WEAPON_DATA_SIZE; i++)
	{
		CSX::Cvar::SaveCvar(SKINS_TEXT, pWeaponData[i], g_Options.Skinchanger.W[pWeaponItemIndexData[i]].nFallbackPaintKit);
		CSX::Cvar::SaveCvar(SKINS_Wear_TEXT, pWeaponData[i], g_Options.Skinchanger.W[pWeaponItemIndexData[i]].flFallbackWear);
		CSX::Cvar::SaveCvar(SKINS_Quality_TEXT, pWeaponData[i], g_Options.Skinchanger.W[pWeaponItemIndexData[i]].iEntityQuality);
		CSX::Cvar::SaveCvar(SKINS_StatTrak_TEXT, pWeaponData[i], g_Options.Skinchanger.W[pWeaponItemIndexData[i]].nFallbackStatTrak);

	}
	
	for (DWORD i = 0; i < Sticker_DATA_SIZE; i++)
	{
		CSX::Cvar::SaveCvar(Sticker_Slot1_TEXT, pStickersData[i], g_Options.Sticker.Weapons[pStickersItemIndexData[i]].Sticker[0].ID);
		CSX::Cvar::SaveCvar(Sticker_Slot2_TEXT, pStickersData[i], g_Options.Sticker.Weapons[pStickersItemIndexData[i]].Sticker[1].ID );
		CSX::Cvar::SaveCvar(Sticker_Slot3_TEXT, pStickersData[i], g_Options.Sticker.Weapons[pStickersItemIndexData[i]].Sticker[2].ID );
		CSX::Cvar::SaveCvar(Sticker_Slot4_TEXT, pStickersData[i], g_Options.Sticker.Weapons[pStickersItemIndexData[i]].Sticker[3].ID );
		CSX::Cvar::SaveCvar(Sticker_Slot5_TEXT, pStickersData[i], g_Options.Sticker.Weapons[pStickersItemIndexData[i]].Sticker[4].ID );

	}
#pragma endregion
#pragma region S_Colors

	string esp_Ambient =
		to_string(int(g_Options.Color.esp_Ambient[0] * 255.f)) + "," +
		to_string(int(g_Options.Color.esp_Ambient[1] * 255.f)) + "," +
		to_string(int(g_Options.Color.esp_Ambient[2] * 255.f));

	string skeleton =
		to_string(int(g_Options.Color.color_skeleton[0] * 255.f)) + "," +
		to_string(int(g_Options.Color.color_skeleton[1] * 255.f)) + "," +
		to_string(int(g_Options.Color.color_skeleton[2] * 255.f));

	string hitbox =
		to_string(int(g_Options.Color.hitbox[0] * 255.f)) + "," +
		to_string(int(g_Options.Color.hitbox[1] * 255.f)) + "," +
		to_string(int(g_Options.Color.hitbox[2] * 255.f));


	string backtrack =
		to_string(int(g_Options.Color.backtrack[0] * 255.f)) + "," +
		to_string(int(g_Options.Color.backtrack[1] * 255.f)) + "," +
		to_string(int(g_Options.Color.backtrack[2] * 255.f));

	string color_sniper =
		to_string(int(g_Options.Color.color_sniper[0] * 255.f)) + "," +
		to_string(int(g_Options.Color.color_sniper[1] * 255.f)) + "," +
		to_string(int(g_Options.Color.color_sniper[2] * 255.f));

	string color_grenadeprediction_circle =
		to_string(int(g_Options.Color.color_grenadeprediction_circle[0] * 255.f)) + "," +
		to_string(int(g_Options.Color.color_grenadeprediction_circle[1] * 255.f)) + "," +
		to_string(int(g_Options.Color.color_grenadeprediction_circle[2] * 255.f));

	string GrenadeCollision =
		to_string(int(g_Options.Color.GrenadeCollision[0] * 255.f)) + "," +
		to_string(int(g_Options.Color.GrenadeCollision[1] * 255.f)) + "," +
		to_string(int(g_Options.Color.GrenadeCollision[2] * 255.f));

	string color_grenadeprediction =
		to_string(int(g_Options.Color.color_grenadeprediction[0] * 255.f)) + "," +
		to_string(int(g_Options.Color.color_grenadeprediction[1] * 255.f)) + "," +
		to_string(int(g_Options.Color.color_grenadeprediction[2] * 255.f));

	string esp_player_fakechams_color =
		to_string(int(g_Options.Color.esp_player_fakechams_color[0] * 255.f)) + "," +
		to_string(int(g_Options.Color.esp_player_fakechams_color[1] * 255.f)) + "," +
		to_string(int(g_Options.Color.esp_player_fakechams_color[2] * 255.f));


	string fill_color_team =
		to_string(int(g_Options.Color.fill_color_team[0] * 255.f)) + "," +
		to_string(int(g_Options.Color.fill_color_team[1] * 255.f)) + "," +
		to_string(int(g_Options.Color.fill_color_team[2] * 255.f));

	string fill_color_enemy =
		to_string(int(g_Options.Color.fill_color_enemy[0] * 255.f)) + "," +
		to_string(int(g_Options.Color.fill_color_enemy[1] * 255.f)) + "," +
		to_string(int(g_Options.Color.fill_color_enemy[2] * 255.f));

	string Crosshair =
		to_string(int(g_Options.Color.Crosshair[0] * 255.f)) + "," +
		to_string(int(g_Options.Color.Crosshair[1] * 255.f)) + "," +
		to_string(int(g_Options.Color.Crosshair[2] * 255.f));

	string Scope =
		to_string(int(g_Options.Color.Scope[0] * 255.f)) + "," +
		to_string(int(g_Options.Color.Scope[1] * 255.f)) + "," +
		to_string(int(g_Options.Color.Scope[2] * 255.f));

	string CrsSpread =
		to_string(int(g_Options.Color.CrsSpread[0] * 255.f)) + "," +
		to_string(int(g_Options.Color.CrsSpread[1] * 255.f)) + "," +
		to_string(int(g_Options.Color.CrsSpread[2] * 255.f));



	string flTracers =
		to_string(int(g_Options.Color.flTracers[0] * 255.f)) + "," +
		to_string(int(g_Options.Color.flTracers[1] * 255.f)) + "," +
		to_string(int(g_Options.Color.flTracers[2] * 255.f));

	string TeamSkeleton =
		to_string(int(g_Options.Color.TeamSkeleton[0] * 255.f)) + "," +
		to_string(int(g_Options.Color.TeamSkeleton[1] * 255.f)) + "," +
		to_string(int(g_Options.Color.TeamSkeleton[2] * 255.f));

	string EnemySkeleton =
		to_string(int(g_Options.Color.EnemySkeleton[0] * 255.f)) + "," +
		to_string(int(g_Options.Color.EnemySkeleton[1] * 255.f)) + "," +
		to_string(int(g_Options.Color.EnemySkeleton[2] * 255.f));

	string TeamSkeletonINVIS =
		to_string(int(g_Options.Color.TeamSkeletonINVIS[0] * 255.f)) + "," +
		to_string(int(g_Options.Color.TeamSkeletonINVIS[1] * 255.f)) + "," +
		to_string(int(g_Options.Color.TeamSkeletonINVIS[2] * 255.f));


	string EnemySkeletonInVIS =
		to_string(int(g_Options.Color.EnemySkeletonInVIS[0] * 255.f)) + "," +
		to_string(int(g_Options.Color.EnemySkeletonInVIS[1] * 255.f)) + "," +
		to_string(int(g_Options.Color.EnemySkeletonInVIS[2] * 255.f));

	string ESP_Color_CT =
		to_string(int(g_Options.Color.esp_Color_CT[0] * 255.f)) + "," +
		to_string(int(g_Options.Color.esp_Color_CT[1] * 255.f)) + "," +
		to_string(int(g_Options.Color.esp_Color_CT[2] * 255.f));

	string ESP_Color_TT =
		to_string(int(g_Options.Color.esp_Color_TT[0] * 255.f)) + "," +
		to_string(int(g_Options.Color.esp_Color_TT[1] * 255.f)) + "," +
		to_string(int(g_Options.Color.esp_Color_TT[2] * 255.f));

	string ESP_Color_VCT =
		to_string(int(g_Options.Color.esp_Color_VCT[0] * 255.f)) + "," +
		to_string(int(g_Options.Color.esp_Color_VCT[1] * 255.f)) + "," +
		to_string(int(g_Options.Color.esp_Color_VCT[2] * 255.f));

	string ESP_Color_VTT =
		to_string(int(g_Options.Color.esp_Color_VTT[0] * 255.f)) + "," +
		to_string(int(g_Options.Color.esp_Color_VTT[1] * 255.f)) + "," +
		to_string(int(g_Options.Color.esp_Color_VTT[2] * 255.f));

	string EnemyGlow =
		to_string(int(g_Options.Color.EnemyGlow[0] * 255.f)) + "," +
		to_string(int(g_Options.Color.EnemyGlow[1] * 255.f)) + "," +
		to_string(int(g_Options.Color.EnemyGlow[2] * 255.f));

	string TeamGlow =
		to_string(int(g_Options.Color.TeamGlow[0] * 255.f)) + "," +
		to_string(int(g_Options.Color.TeamGlow[1] * 255.f)) + "," +
		to_string(int(g_Options.Color.TeamGlow[2] * 255.f));

	string WeaponGlow =
		to_string(int(g_Options.Color.WeaponGlow[0] * 255.f)) + "," +
		to_string(int(g_Options.Color.WeaponGlow[1] * 255.f)) + "," +
		to_string(int(g_Options.Color.WeaponGlow[2] * 255.f));

	string C4glow =
		to_string(int(g_Options.Color.C4glow[0] * 255.f)) + "," +
		to_string(int(g_Options.Color.C4glow[1] * 255.f)) + "," +
		to_string(int(g_Options.Color.C4glow[2] * 255.f));

	string EnemyChamsVis =
		to_string(int(g_Options.Color.EnemyChamsVis[0] * 255.f)) + "," +
		to_string(int(g_Options.Color.EnemyChamsVis[1] * 255.f)) + "," +
		to_string(int(g_Options.Color.EnemyChamsVis[2] * 255.f));

	string EnemyChamsNVis =
		to_string(int(g_Options.Color.EnemyChamsNVis[0] * 255.f)) + "," +
		to_string(int(g_Options.Color.EnemyChamsNVis[1] * 255.f)) + "," +
		to_string(int(g_Options.Color.EnemyChamsNVis[2] * 255.f));

	string TeamChamsVis =
		to_string(int(g_Options.Color.TeamChamsVis[0] * 255.f)) + "," +
		to_string(int(g_Options.Color.TeamChamsVis[1] * 255.f)) + "," +
		to_string(int(g_Options.Color.TeamChamsVis[2] * 255.f));

	string TeamChamsNVis =
		to_string(int(g_Options.Color.TeamChamsNVis[0] * 255.f)) + "," +
		to_string(int(g_Options.Color.TeamChamsNVis[1] * 255.f)) + "," +
		to_string(int(g_Options.Color.TeamChamsNVis[2] * 255.f));

	string WeaponDropped =
		to_string(int(g_Options.Color.WeaponDropped[0] * 255.f)) + "," +
		to_string(int(g_Options.Color.WeaponDropped[1] * 255.f)) + "," +
		to_string(int(g_Options.Color.WeaponDropped[2] * 255.f));

	string HandCham =
		to_string(int(g_Options.Color.HandCham[0] * 255.f)) + "," +
		to_string(int(g_Options.Color.HandCham[1] * 255.f)) + "," +
		to_string(int(g_Options.Color.HandCham[2] * 255.f));

	string WeaponHandCham =
		to_string(int(g_Options.Color.WeaponHandCham[0] * 255.f)) + "," +
		to_string(int(g_Options.Color.WeaponHandCham[1] * 255.f)) + "," +
		to_string(int(g_Options.Color.WeaponHandCham[2] * 255.f));

	string rad_Color_CT =
		to_string(int(g_Options.Color.rad_Color_CT[0] * 255.f)) + "," +
		to_string(int(g_Options.Color.rad_Color_CT[1] * 255.f)) + "," +
		to_string(int(g_Options.Color.rad_Color_CT[2] * 255.f));

	string rad_Color_TT =
		to_string(int(g_Options.Color.rad_Color_TT[0] * 255.f)) + "," +
		to_string(int(g_Options.Color.rad_Color_TT[1] * 255.f)) + "," +
		to_string(int(g_Options.Color.rad_Color_TT[2] * 255.f));

	string rad_Color_VCT =
		to_string(int(g_Options.Color.rad_Color_VCT[0] * 255.f)) + "," +
		to_string(int(g_Options.Color.rad_Color_VCT[1] * 255.f)) + "," +
		to_string(int(g_Options.Color.rad_Color_VCT[2] * 255.f));

	string rad_Color_VTT =
		to_string(int(g_Options.Color.rad_Color_VTT[0] * 255.f)) + "," +
		to_string(int(g_Options.Color.rad_Color_VTT[1] * 255.f)) + "," +
		to_string(int(g_Options.Color.rad_Color_VTT[2] * 255.f));

	string Light_CT =
		to_string(int(g_Options.Color.Light_CT[0] * 255.f)) + "," +
		to_string(int(g_Options.Color.Light_CT[1] * 255.f)) + "," +
		to_string(int(g_Options.Color.Light_CT[2] * 255.f));

	string Light_TT =
		to_string(int(g_Options.Color.Light_TT[0] * 255.f)) + "," +
		to_string(int(g_Options.Color.Light_TT[1] * 255.f)) + "," +
		to_string(int(g_Options.Color.Light_TT[2] * 255.f));

	string WorldMode =
		to_string(int(g_Options.Color.worldmode[0] * 255.f)) + "," +
		to_string(int(g_Options.Color.worldmode[1] * 255.f)) + "," +
		to_string(int(g_Options.Color.worldmode[2] * 255.f));

	CSX::Cvar::SaveCvar(COLORS_TEXT, C_champlayeralpha, g_Options.Color.champlayeralpha);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_chamhandalpha, g_Options.Color.chamhandalpha);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_weaponhandalpha, g_Options.Color.weaponhandalpha);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_weaponglowalpha, g_Options.Color.weaponglowalpha);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_playerglowalpha, g_Options.Color.playerglowalpha);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_bombglowalpha, g_Options.Color.bombglowalpha);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_weaponsworldalph, g_Options.Color.weaponsworldalph);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_weponchamsalph, g_Options.Color.weponchamsalph);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_rad_Alpha, g_Options.Color.rad_Alpha);


	CSX::Cvar::SaveCvar(COLORS_TEXT, C_esp_Ambient, esp_Ambient);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_backtrack, backtrack);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_hitbox, hitbox);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_skeleton, skeleton);


	CSX::Cvar::SaveCvar(COLORS_TEXT, C_color_sniper, color_sniper);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_GrenadeCollision, GrenadeCollision);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_color_grenadeprediction_circle, color_grenadeprediction_circle);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_color_grenadeprediction, color_grenadeprediction);

	CSX::Cvar::SaveCvar(COLORS_TEXT, C_fakechams, esp_player_fakechams_color);

	CSX::Cvar::SaveCvar(COLORS_TEXT, C_fill_color_team, fill_color_team);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_fill_color_enemy, fill_color_enemy);

	CSX::Cvar::SaveCvar(COLORS_TEXT, C_Crosshair, Crosshair);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_Scope, Scope);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_CrsSpread, CrsSpread);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_flTracers, flTracers);

	CSX::Cvar::SaveCvar(COLORS_TEXT, C_TeamSkeleton, TeamSkeleton);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_EnemySkeleton, EnemySkeleton);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_TeamSkeletonINVIS, TeamSkeletonINVIS);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_EnemySkeletonInVIS, EnemySkeletonInVIS);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_ESP_Color_CT, ESP_Color_CT);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_ESP_Color_TT, ESP_Color_TT);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_ESP_Color_VCT, ESP_Color_VCT);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_ESP_Color_VTT, ESP_Color_VTT);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_EnemyGlow, EnemyGlow);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_TeamGlow, TeamGlow);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_WeaponGlow, WeaponGlow);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_C4glow, C4glow);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_EnemyChamsVis, EnemyChamsVis);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_EnemyChamsNVis, EnemyChamsNVis);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_TeamChamsVis, TeamChamsVis);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_TeamChamsNVis, TeamChamsNVis);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_WeaponDropped, WeaponDropped);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_HandCham, HandCham);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_WeaponHandCham, WeaponHandCham);

	CSX::Cvar::SaveCvar(COLORS_TEXT, C_rad_Color_CT, rad_Color_CT);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_rad_Color_TT, rad_Color_TT);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_rad_Color_VCT, rad_Color_VCT);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_rad_Color_VTT, rad_Color_VTT);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_Light_CT, Light_CT);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_Light_TT, Light_TT);
	CSX::Cvar::SaveCvar(COLORS_TEXT, C_WorldMode, WorldMode);


#pragma endregion

#pragma region S_Settings
	CSX::Cvar::SaveCvar(SETTINGS_TEXT, St_Themes, g_Options.Menu.Theme);

#pragma endregion


}

Variables g_Options;