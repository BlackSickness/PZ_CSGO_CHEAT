#pragma once
#include "singleton.hpp"
#include "MiscClasses.h"
#include "Interfaces.h"
#include "Sounds.h"
#include "Logging.h"

std::vector<cbullet_tracer_info> logs;

#pragma comment(lib, "winmm.lib")
class item_purchase

	: public singleton<item_purchase>
{
	class item_purchase_listener
		: public IGameEventListener2
	{
	public:
		void start()
		{
			Interfaces::EventManager()->AddListener(this, "item_purchase", false);
			Interfaces::EventManager()->AddListener(this, "player_hurt", false);
			Interfaces::EventManager()->AddListener(this, "player_death", false);
			Interfaces::EventManager()->AddListener(this, "game_newmap", false);
			Interfaces::EventManager()->AddListener(this, "player_connect_full", false);
			Interfaces::EventManager()->AddListener(this, "round_start", false);
			Interfaces::EventManager()->AddListener(this, "cs_game_disconnected", false);
			Interfaces::EventManager()->AddListener(this, "bullet_impact", false);

		}
		void stop()
		{
			Interfaces::EventManager()->RemoveListener(this);
		}
		void FireGameEvent(IGameEvent *event) override
		{
			singleton()->on_fire_weapons(event, false);
			singleton()->on_fire_DMG(event, false);
		if(g_Options.Skinchanger.Enabled)singleton()->Killicon(event);
			singleton()->beam(event);
			//singleton()->LCH(event);

		}
		int GetEventDebugID(void) override
		{
			return 42 /*0x2A*/;
		}
	};

public:

	static item_purchase* singleton()
	{
		static item_purchase* instance = new item_purchase;
		return instance;
	}

	void initialize()
	{
		listener.start();
	}

	void remove()
	{
		listener.stop();
	}

	void on_fire_weapons(IGameEvent* event, bool drawing)
	{
		static AppLog wlog;
		if (drawing)
		{
			wlog.Draw("Weapons Log");
		}

		if (!drawing)
		{
			C_BaseEntity* local = Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());
			if (!strcmp(event->GetName(), "item_purchase"))
			{
				auto buyer = event->GetInt("userid");
				std::string gun = event->GetString("weapon");

				if (strstr(gun.c_str(), "molotov")
					|| strstr(gun.c_str(), "nade")
					|| strstr(gun.c_str(), "kevlar")
					|| strstr(gun.c_str(), "decoy")
					|| strstr(gun.c_str(), "suit")
					|| strstr(gun.c_str(), "flash")
					|| strstr(gun.c_str(), "vest")
					|| strstr(gun.c_str(), "cutter")
					|| strstr(gun.c_str(), "defuse")
					)  return;
				auto player_index = Interfaces::Engine()->GetPlayerForUserID(buyer);
				C_BaseEntity* player = (C_BaseEntity*)Interfaces::EntityList()->GetClientEntity(player_index);
				PlayerInfo pinfo;

				if (player && local && Interfaces::Engine()->GetPlayerInfo(player_index, &pinfo))
				{
					gun.erase(gun.find("weapon_"), 7);
					if (player->GetTeamNum() == local->GetTeamNum())
					{
						wlog.AddLog("[Team] ""%s bought a %s\n", pinfo.m_szPlayerName, gun.c_str());

					}
					else
					{
						wlog.AddLog("[Enemy] ""%s bought a %s\n", pinfo.m_szPlayerName, gun.c_str());
					}

				}
			}

		}
	}
	void on_fire_DMG(IGameEvent* event, bool drawing)
	{
		static AppLog dlog;
		if (drawing)
		{
			dlog.Draw("DMG Log");
		}

		if (!drawing)
		{
			C_BaseEntity* local = Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());

			if (!strcmp(event->GetName(), "player_hurt"))
			{
				if (Interfaces::Engine()->GetPlayerForUserID(event->GetInt("attacker")) == Interfaces::Engine()->GetLocalPlayer() &&
					Interfaces::Engine()->GetPlayerForUserID(event->GetInt("userid")) != Interfaces::Engine()->GetLocalPlayer())
				{
					if (g_Options.Visuals.Hitbox)
						visuals::Hitbox(Interfaces::Engine()->GetPlayerForUserID(event->GetInt("userid")));
				}

				int deadfag = event->GetInt("userid");
				int attackingfag = event->GetInt("attacker");
				C_BaseEntity* pLocal = Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());
				if (Interfaces::Engine()->GetPlayerForUserID(deadfag) != Interfaces::Engine()->GetLocalPlayer() && Interfaces::Engine()->GetPlayerForUserID(attackingfag) == Interfaces::Engine()->GetLocalPlayer())
				{
					C_BaseEntity* hittedplayer = (C_BaseEntity*)(Interfaces::Engine()->GetPlayerForUserID(deadfag));
					int hit = event->GetInt("hitgroup");
					if (hit >= 0 && hittedplayer && deadfag && attackingfag)
					{
						Globals::missedshots++;
					}
					else
					{
						Globals::missedshots++;
					}
				}
			}

			if (!strcmp(event->GetName(), "player_hurt"))
			{
				auto bitch = event->GetInt("userid");
				auto coolguy49 = event->GetInt("attacker");
				int dmg = event->GetInt("dmg_health");


				auto bitch_index = Interfaces::Engine()->GetPlayerForUserID(bitch);
				auto coolguy49_index = Interfaces::Engine()->GetPlayerForUserID(coolguy49);
				C_BaseEntity* bitch_ = (C_BaseEntity*)Interfaces::EntityList()->GetClientEntity(bitch_index);
				C_BaseEntity* coolguy49_ = (C_BaseEntity*)Interfaces::EntityList()->GetClientEntity(coolguy49_index);
				PlayerInfo bitch_info;
				PlayerInfo coolguy49_info;

				if (coolguy49_ == local)
				{
					G::hitmarkeralpha = 1.f;
					switch (g_Options.Misc.Hitsound)
					{
					case 0: break;
					case 1: PlaySoundA(rawData, NULL, SND_ASYNC | SND_MEMORY); break;
					case 2: PlaySoundA(pew, NULL, SND_ASYNC | SND_MEMORY); break;
					case 3: PlaySoundA(roblox, NULL, SND_ASYNC | SND_MEMORY); break;
					case 4: PlaySoundA(hitler_wav, NULL, SND_ASYNC | SND_MEMORY); break;
					case 5: PlaySoundA(headmeme, NULL, SND_ASYNC | SND_MEMORY); break;
					case 6: PlaySoundA(FadeCSGO, NULL, SND_ASYNC | SND_MEMORY); break;
					case 7: PlaySoundA(inject_wav, NULL, SND_ASYNC | SND_MEMORY); break;
					}

				}

				if (bitch && local && Interfaces::Engine()->GetPlayerInfo(bitch_index, &bitch_info), Interfaces::Engine()->GetPlayerInfo(coolguy49_index, &coolguy49_info) && bitch_ && coolguy49_)
				{
					dlog.AddLog("[Dmg] ""%s did %i to %s\n", coolguy49_info.m_szPlayerName, dmg, bitch_info.m_szPlayerName);
				}
			}

		}
	}


	void beam(IGameEvent* event)
	{
		if (!strcmp(event->GetName(), "bullet_impact"))
		{
			C_BaseEntity* LocalPlayer = Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());

			if (LocalPlayer)
			{
				auto index = Interfaces::Engine()->GetPlayerForUserID(event->GetInt("userid"));

				if (index != Interfaces::Engine()->GetLocalPlayer())
					return;

				auto local = static_cast<C_BaseEntity*>(Interfaces::EntityList()->GetClientEntity(index));
				if (!local)
					return;

				Vector position(event->GetFloat("x"), event->GetFloat("y"), event->GetFloat("z"));

				Ray_t ray;
				ray.Init(local->GetEyePosition(), position);

				CTraceFilter filter;
				filter.pSkip = local;

				trace_t tr;
				Interfaces::EngineTrace()->TraceRay(ray, MASK_SHOT, &filter, &tr);

				logs.push_back(cbullet_tracer_info(local->GetEyePosition(), position, Interfaces::Globals()->curtime, Color(int(g_Options.Color.flTracers[0] * 255.f), int(g_Options.Color.flTracers[1] * 255.f), int(g_Options.Color.flTracers[2] * 255.f))));


				if (!local)
					return;

				for (size_t i = 0; i < logs.size(); i++)
				{
					auto current = logs.at(i);
					current.color = Color(int(g_Options.Color.flTracers[0] * 255.f), int(g_Options.Color.flTracers[1] * 255.f), int(g_Options.Color.flTracers[2] * 255.f)); //color of local player's tracers
					BeamInfo_t beamInfo;
					beamInfo.m_nType = TE_BEAMPOINTS;
					beamInfo.m_pszModelName = "sprites/physbeam.vmt";
					beamInfo.m_nModelIndex = -1;
					beamInfo.m_flHaloScale = 0.0f;
					beamInfo.m_flLife = g_Options.Visuals.flTracersDuration;
					beamInfo.m_flWidth = g_Options.Visuals.flTracersWidth;
					beamInfo.m_flEndWidth = g_Options.Visuals.flTracersWidth;
					beamInfo.m_flFadeLength = 0.0f;
					beamInfo.m_flAmplitude = 2.0f;
					beamInfo.m_flBrightness = 255.f;
					beamInfo.m_flSpeed = 0.2f;
					beamInfo.m_nStartFrame = 0;
					beamInfo.m_flFrameRate = 0.f;
					beamInfo.m_flRed = current.color.r();
					beamInfo.m_flGreen = current.color.g();
					beamInfo.m_flBlue = current.color.b();
					beamInfo.m_nSegments = 2;
					beamInfo.m_bRenderable = true;
					beamInfo.m_nFlags = FBEAM_ONLYNOISEONCE | FBEAM_NOTILE | FBEAM_HALOBEAM;

					beamInfo.m_vecStart = LocalPlayer->GetEyePosition();
					beamInfo.m_vecEnd = current.dst;
					if (g_Options.Visuals.bulletshow)
					{
						auto beam = Interfaces::g_ViewRenderBeams->CreateBeamPoints(beamInfo);
						if (beam)
							Interfaces::g_ViewRenderBeams->DrawBeam(beam);
					}

					logs.erase(logs.begin() + i);
				}
			}

		}

	}
	void Killicon(IGameEvent* pEvent)
	{
		const char* szEventName = pEvent->GetName();

		if (!strcmp(szEventName, "player_death"))
			CSkin::instance().ApplyCustomKillIcon(pEvent);

		if (!strcmp(szEventName, "game_newmap"))
		{
			if (g_ViewModelCfg.size() >= 1)
				g_ViewModelCfg.clear();
			CSkin::instance().SetKillIconCfg();
		}
	}

private:
	item_purchase_listener  listener;
};

item_purchase purchase;