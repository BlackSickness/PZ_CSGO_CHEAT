#pragma once
#include "SDK.h"

void InitKeyValues(KeyValues* keyValues, const char* name);

void LoadFromBuffer(KeyValues* keyValues, char const* resourceName, const char* pBuffer);

void ForceMaterial(Color color, IMaterial* material, bool useColor = true, bool forceMaterial = true);


IMaterial* CreateMaterial(bool Ignore, bool Lit, bool Wireframe);