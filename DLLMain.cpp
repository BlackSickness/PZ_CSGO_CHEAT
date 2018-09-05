#include "Hooks.h"
#include "Render.h"
#include "recvproxy.h"
#include <windows.h>
#include "SetupColors.h"


UCHAR
szFileSys[255],
szVolNameBuff[255];

DWORD
dwMFL,
dwSysFlags,
dwSerial;
#define My_HWID (123)
#define David (123)
#define Rayan (123)
#define Tayeb (123)
#define Saeed (123)
#define Waleed (123)
#define Hasan (123)
#define Nikushaa (123)


bool chk()
{
	UCHAR
		szFileSys[255],
		szVolNameBuff[255];
	DWORD
		dwMFL,
		dwSysFlags,
		dwSerial;
	GetVolumeInformation("C:\\", (LPTSTR)szVolNameBuff, 255, &dwSerial, &dwMFL, &dwSysFlags, (LPTSTR)szFileSys, 255);

	if (dwSerial == My_HWID ||
		dwSerial == David   ||
		dwSerial == Rayan   ||
		dwSerial == Tayeb   ||
		dwSerial == Saeed   ||
		dwSerial == Waleed  ||
		dwSerial == Hasan   ||
		dwSerial == Nikushaa)
	{
		return true;
	}

	return false;
}

bool on_dll_attach(void* base)
{
	
	//if (!chk())
	//{

		//MessageBox(0, "Cheat Expired", "INFORMATION", MB_OK | MB_ICONERROR);
		//ExitProcess(1);

	//}
	
	Interfaces::InitialiseInterfaces();
	g_Netvars->GetNetvars();
	g_Render->SetupFonts();
	hooks::initialize();
	SetupColors();
	NetvarHook();
	Sleep(6000);
	FreeLibraryAndExitThread((HMODULE)base, 0);
}

bool on_dll_detach()
{
	UnloadProxy();
	hooks::cleanup();
	return 1;
}

BOOL WINAPI DllMain(
	_In_      HINSTANCE hinstDll,
	_In_      DWORD     fdwReason,
	_In_opt_	LPVOID    lpvReserved
)
{
	switch (fdwReason) {
	case DLL_PROCESS_ATTACH:
		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)on_dll_attach, hinstDll, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		if (lpvReserved == nullptr)
			return on_dll_detach();
	}
	return TRUE;
}
