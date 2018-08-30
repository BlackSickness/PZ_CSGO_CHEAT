#include "HookIncludes.h"
#include "Global.h"


void __fastcall Hooked_SceneEnd(void *pEcx, void *pEdx) {

	C_BaseEntity *pLocal = Interfaces::EntityList()->GetClientEntity(Interfaces::Engine()->GetLocalPlayer());

	// Ghost Chams
	if (g_Options.Visuals.esp_player_chams_fakeangle)
	{
		if (pLocal)
		{
			static  IMaterial* ghost_mat = CreateMaterial(false, true, false); //creating a material identical to wireframe
			if (ghost_mat)
			{
				Vector OriginalAngle;
				OriginalAngle = *pLocal->GetEyeAngles();
				pLocal->SetAngle2(Vector(0, Globals::FakeAngle, 0));

			Interfaces::RenderView()->SetColorModulation(g_Options.Color.esp_player_fakechams_color);
			Interfaces::ModelRender()->ForcedMaterialOverride(ghost_mat);
				pLocal->draw_model(1, 200);
				Interfaces::ModelRender()->ForcedMaterialOverride(nullptr);
				pLocal->SetAngle2(OriginalAngle);
			}
		}
	}
	hooks::original_SceneEnd(pEcx);

}