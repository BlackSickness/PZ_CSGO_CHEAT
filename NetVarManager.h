#pragma once

class CNetVarManager
{
public:
	void Initialize();
	void GrabOffsets();
	int GetOffset(const char *tableName, const char *propName);
	bool HookProp(const char *tableName, const char *propName, RecvVarProxyFn fun);
	DWORD hookProp(const char* tableName, const char* propName, void* hkFunc, void* oldFn = 0);
private:
	int Get_Prop(const char *tableName, const char *propName, RecvProp **prop = 0);
	int Get_Prop(RecvTable *recvTable, const char *propName, RecvProp **prop = 0);

	RecvTable *GetTable(const char *tableName);
	std::vector<RecvTable*> m_tables;
	void DumpTable(RecvTable *table, int depth);
};

extern CNetVarManager* NetVarManager;