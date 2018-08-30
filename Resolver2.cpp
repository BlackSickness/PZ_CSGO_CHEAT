#include "Resolver2.h"

bool Resolver::didhitHS;

void LowerBodyYawFix(C_BaseEntity* pEntity)
{
	if (g_Options.Ragebot.LBYCorrection)
	{
		if (!pEntity) return;
		if (pEntity->GetClientClass()->m_ClassID != (int)ClassID::CCSPlayer) return;
		if (!pEntity->IsAlive() || !pEntity->GetActiveWeaponHandle()) return;
		if (Interfaces::Engine()->GetLocalPlayer()) return;

		auto EyeAngles = pEntity->GetEyeAngles();
		if (pEntity->GetVelocity().Length() > 1 && (pEntity->GetFlags() & (int)pEntity->GetFlags() & FL_ONGROUND))
			EyeAngles->y = pEntity->GetLowerBodyYaw();
	}
}

void ResolverSetup::Resolve(C_BaseEntity* pEntity)
{
	bool MeetsLBYReq;
	if (pEntity->GetFlags() & FL_ONGROUND)
		MeetsLBYReq = true;
	else
		MeetsLBYReq = false;

	bool IsMoving;
	if (pEntity->GetVelocity().Length2D() >= 0.5)
		IsMoving = true;
	else
		IsMoving = false;

	ResolverSetup::NewANgles[pEntity->EntIndex()] = *pEntity->GetEyeAngles();
	ResolverSetup::newlby[pEntity->EntIndex()] = pEntity->GetLowerBodyYaw();
	ResolverSetup::newsimtime = pEntity->GetSimulationTime();
	ResolverSetup::newdelta[pEntity->EntIndex()] = pEntity->GetEyeAngles()->y;
	ResolverSetup::newlbydelta[pEntity->EntIndex()] = pEntity->GetLowerBodyYaw();
	ResolverSetup::finaldelta[pEntity->EntIndex()] = ResolverSetup::newdelta[pEntity->EntIndex()] - ResolverSetup::storeddelta[pEntity->EntIndex()];
	ResolverSetup::finallbydelta[pEntity->EntIndex()] = ResolverSetup::newlbydelta[pEntity->EntIndex()] - ResolverSetup::storedlbydelta[pEntity->EntIndex()];
	if (newlby == storedlby)
		ResolverSetup::lbyupdated = false;
	else
		ResolverSetup::lbyupdated = true;

	if (g_Options.Ragebot.AimbotResolver == 0)
	{
	}
	else if (g_Options.Ragebot.AimbotResolver == 1)//level 3
	{
		Globals::resolvemode = 2;
		if (Globals::missedshots > 3 && Globals::missedshots < 21)
		{
			if (MeetsLBYReq && lbyupdated)
			{
				pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw();
			}
			else if (!MeetsLBYReq && lbyupdated)
			{
				switch (Globals::Shots % 4)
				{
				case 1:
					pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() - 15;
					break;
				case 2:
					pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() + 40;
					break;
				case 3:
					pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() + 15;
					break;
				case 4:
					pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() - 40;
					break;
				}
			}
			else
				pEntity->GetEyeAngles()->y = rand() % 180 - rand() % 35;
		}

		else if (Globals::missedshots >= 2 && Globals::missedshots <= 3)
		{
			if (MeetsLBYReq && lbyupdated)
			{
				pEntity->GetEyeAngles()->y = ResolverSetup::finallbydelta[pEntity->EntIndex()];
			}
			else
				pEntity->GetEyeAngles()->y = ResolverSetup::finaldelta[pEntity->EntIndex()];
		}
		else
		{
			if (MeetsLBYReq && lbyupdated)
			{
				bool timer = 0;
				if (timer)
					pEntity->GetEyeAngles()->y = ResolverSetup::finallbydelta[pEntity->EntIndex()] + rand() % 35;
				else
					pEntity->GetEyeAngles()->y = ResolverSetup::finallbydelta[pEntity->EntIndex()] - rand() % 35;
				timer = !timer;
			}
			else
			{
				bool timer = 0;
				if (timer)
					pEntity->GetEyeAngles()->y = ResolverSetup::finaldelta[pEntity->EntIndex()] + rand() % 35;
				else
					pEntity->GetEyeAngles()->y = ResolverSetup::finaldelta[pEntity->EntIndex()] - rand() % 35;
				timer = !timer;
			}
		}
	}
	else if (g_Options.Ragebot.AimbotResolver == 2) //level 2 
	{
		Globals::resolvemode = 1;
		if (Resolver::didhitHS)
		{
			if (MeetsLBYReq && lbyupdated)
			{
				pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw();
			}
			switch (Globals::Shots % 30)
			{
			case 1:
				*pEntity->GetEyeAngles() = StoredAngles[pEntity->EntIndex()];
				break;
			case 2:
				*pEntity->GetEyeAngles() = StoredAngles[pEntity->EntIndex()];
				break;
			case 3:
				pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() - 15;
				break;
			case 4:
				pEntity->GetEyeAngles()->y = pEntity->GetEyeAngles()->y + 40;
				break;
			case 5:
				pEntity->GetEyeAngles()->y = pEntity->GetEyeAngles()->y + 15;
				break;
			case 6:
				pEntity->GetEyeAngles()->y = pEntity->GetEyeAngles()->y - 40;
				break;
			case 7:
				pEntity->GetEyeAngles()->y = pEntity->GetEyeAngles()->y - 50;
				break;
			case 8:
				pEntity->GetEyeAngles()->y = pEntity->GetEyeAngles()->y - 60;
				break;
			case 9:
				pEntity->GetEyeAngles()->y = pEntity->GetEyeAngles()->y - 70;
				break;
			case 10:
				pEntity->GetEyeAngles()->y = pEntity->GetEyeAngles()->y - 80;
				break;
			case 11:
				pEntity->GetEyeAngles()->y = pEntity->GetEyeAngles()->y - 90;
				break;
			case 12:
				pEntity->GetEyeAngles()->y = pEntity->GetEyeAngles()->y - 100;
				break;
			case 13:
				pEntity->GetEyeAngles()->y = pEntity->GetEyeAngles()->y - 110;
				break;
			case 14:
				pEntity->GetEyeAngles()->y = pEntity->GetEyeAngles()->y - 120;
				break;
			case 15:
				pEntity->GetEyeAngles()->y = pEntity->GetEyeAngles()->y - 140;
				break;
			case 16:
				pEntity->GetEyeAngles()->y = pEntity->GetEyeAngles()->y - 150;
				break;
			case 17:
				pEntity->GetEyeAngles()->y = pEntity->GetEyeAngles()->y - 160;
				break;
			case 18:
				pEntity->GetEyeAngles()->y = pEntity->GetEyeAngles()->y - 180;
				break;
			case 19:
				pEntity->GetEyeAngles()->y = pEntity->GetEyeAngles()->y - 200;
				break;
			case 20:
				pEntity->GetEyeAngles()->y = pEntity->GetEyeAngles()->y - 220;
				break;
			case 21:
				pEntity->GetEyeAngles()->y = pEntity->GetEyeAngles()->y - 240;
				break;
			case 22:
				pEntity->GetEyeAngles()->y = pEntity->GetEyeAngles()->y - 260;
				break;
			case 23:
				pEntity->GetEyeAngles()->y = pEntity->GetEyeAngles()->y - 280;
				break;
			case 24:
				pEntity->GetEyeAngles()->y = pEntity->GetEyeAngles()->y - 300;
				break;
			case 25:
				pEntity->GetEyeAngles()->y = pEntity->GetEyeAngles()->y - 320;
				break;
			case 26:
				pEntity->GetEyeAngles()->y = pEntity->GetEyeAngles()->y - 330;
				break;
			case 27:
				pEntity->GetEyeAngles()->y = pEntity->GetEyeAngles()->y - 340;
				break;
			case 28:
				pEntity->GetEyeAngles()->y = pEntity->GetEyeAngles()->y - 345;
				break;
			case 29:
				pEntity->GetEyeAngles()->y = pEntity->GetEyeAngles()->y - 350;
				break;
			case 30:
				pEntity->GetEyeAngles()->y = pEntity->GetEyeAngles()->y - 360;
				break;



			}
		}
		else if (MeetsLBYReq && lbyupdated && !Resolver::didhitHS)
		{
			pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw();
		}

		else if (GetAsyncKeyState(g_Options.Ragebot.SomeShit) && g_Options.Ragebot.SomeShit > 0)
		{
			auto local_player = Interfaces::EntityList()->GetClientEntity(
				Interfaces::Engine()->GetLocalPlayer());
			if (!local_player)
				return;

			Vector viewangles;
			Interfaces::Engine()->GetViewAngles(viewangles);
			auto delta = Normalize_y(viewangles.y - pEntity->GetEyeAngles()->y);

			if (delta > 0)
				pEntity->GetEyeAngles()->y + 180;
			else
				pEntity->GetEyeAngles()->y - 180;
		}

		else if (!MeetsLBYReq || !lbyupdated && !Resolver::didhitHS)
		{
			pEntity->GetEyeAngles()->y = rand() % 180 - rand() % 35;
		}
		else
			pEntity->GetEyeAngles()->y = rand() % 180;
	}
	LowerBodyYawFix(pEntity);
}

void ResolverSetup::StoreFGE(C_BaseEntity* pEntity)
{
	ResolverSetup::storedanglesFGE = pEntity->GetEyeAngles()->y;
	ResolverSetup::storedlbyFGE = pEntity->GetLowerBodyYaw();
	ResolverSetup::storedsimtimeFGE = pEntity->GetSimulationTime();
}
void ResolverSetup::StoreThings(C_BaseEntity* pEntity)
{
	ResolverSetup::StoredAngles[pEntity->EntIndex()] = *pEntity->GetEyeAngles();
	ResolverSetup::storedlby[pEntity->EntIndex()] = pEntity->GetLowerBodyYaw();
	ResolverSetup::storedsimtime = pEntity->GetSimulationTime();
	ResolverSetup::storeddelta[pEntity->EntIndex()] = pEntity->GetEyeAngles()->y;
	ResolverSetup::storedlby[pEntity->EntIndex()] = pEntity->GetLowerBodyYaw();
}

void ResolverSetup::CM(C_BaseEntity* pEntity)
{
	for (int x = 1; x < Interfaces::Engine()->GetMaxClients(); x++)
	{

		pEntity = (C_BaseEntity*)Interfaces::EntityList()->GetClientEntity(x);
		C_BaseEntity *pLocal = Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());

		if (!pEntity
			|| pEntity == pLocal
			|| pEntity->IsDormant()
			|| !pEntity->IsAlive())
			continue;

		ResolverSetup::StoreThings(pEntity);
	}
}

void ResolverSetup::OverrideResolver(C_BaseEntity* pEntity)
{

	bool MeetsLBYReq;
	if (pEntity->GetFlags() & FL_ONGROUND)
		MeetsLBYReq = true;
	else
		MeetsLBYReq = false;

	int OverrideKey = g_Options.Ragebot.SomeShit;

	if (g_Options.Ragebot.SomeShit);
	{
		if (GetAsyncKeyState(g_Options.Ragebot.SomeShit))
		{
			pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() - 180.f;

			{
				Globals::resolvemode = 2;
				if (Globals::missedshots > 4 && Globals::missedshots < 5)
				{
					if (MeetsLBYReq && lbyupdated)
					{
						pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw();
					}
					else if (!MeetsLBYReq && lbyupdated)
					{
						switch (Globals::Shots % 4)
						{
						case 1:
							pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() + 45;
							break;
						case 2:
							pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() + 90;
							break;
						case 3:
							pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() + 135;
							break;
						case 4:
							pEntity->GetEyeAngles()->y = pEntity->GetLowerBodyYaw() + 180;
							break;
						}
					}
					else
						pEntity->GetEyeAngles()->y = rand() % 180 - rand() % 35;
				}

			}
		}
	}
}

void ResolverSetup::FSN(ClientFrameStage_t stage)
{
	if (stage == ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		for (int i = 1; i < Interfaces::EntityList()->GetHighestEntityIndex(); i++)
		{
			C_BaseEntity *pLocal = Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());

				C_BaseEntity* pEnt = Interfaces::EntityList()->GetClientEntity(i);

				if (pEnt == nullptr) continue;
					
				if (!pEnt) continue;

				if (pEnt == pLocal) continue;

				if (pEnt->IsDormant()) continue;

				PlayerInfo pTemp;

				if (!Interfaces::Engine()->GetPlayerInfo(i, &pTemp))
					continue;

				pEnt = (C_BaseEntity*)Interfaces::EntityList()->GetClientEntity(i);

				

			if (!pEnt
				|| pEnt == pLocal
				|| pEnt->IsDormant()
				|| !pEnt->IsAlive())
				continue;

			ResolverSetup::Resolve(pEnt);
			ResolverSetup::OverrideResolver(pEnt);
		}
	}
}