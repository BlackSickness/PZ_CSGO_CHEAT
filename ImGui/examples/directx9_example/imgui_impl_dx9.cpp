// ImGui Win32 + DirectX9 binding
// In this binding, ImTextureID is used to store a 'LPDIRECT3DTEXTURE9' texture identifier. Read the FAQ about ImTextureID in imgui.cpp.

// You can copy and use unmodified imgui_impl_* files in your project. See main.cpp for an example of using this.
// If you use this binding you'll need to call 4 functions: ImGui_ImplXXXX_Init(), ImGui_ImplXXXX_NewFrame(), ImGui::Render() and ImGui_ImplXXXX_Shutdown().
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.
// https://github.com/ocornut/imgui

#include "../../imgui.h"
#include "imgui_impl_dx9.h"

// DirectX
#include <d3d9.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

// Data
static HWND                     g_hWnd = 0;
static INT64                    g_Time = 0;
static INT64                    g_TicksPerSecond = 0;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static LPDIRECT3DVERTEXBUFFER9  g_pVB = NULL;
static LPDIRECT3DINDEXBUFFER9   g_pIB = NULL;
static LPDIRECT3DTEXTURE9       g_FontTexture = NULL;
static int                      g_VertexBufferSize = 5000, g_IndexBufferSize = 10000;

struct CUSTOMVERTEX
{
	float    pos[3];
	D3DCOLOR col;
	float    uv[2];
};
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

// This is the main rendering function that you have to implement and provide to ImGui (via setting up 'RenderDrawListsFn' in the ImGuiIO structure)
// If text or lines are blurry when integrating ImGui in your engine:
// - in your Render function, try translating your projection matrix by (0.5f,0.5f) or (0.375f,0.375f)
void ImGui_ImplDX9_RenderDrawLists(ImDrawData* draw_data)
{
	// Avoid rendering when minimized
	ImGuiIO& io = ImGui::GetIO();
	if (io.DisplaySize.x <= 0.0f || io.DisplaySize.y <= 0.0f)
		return;

	// Create and grow buffers if needed
	if (!g_pVB || g_VertexBufferSize < draw_data->TotalVtxCount)
	{
		if (g_pVB) { g_pVB->Release(); g_pVB = NULL; }
		g_VertexBufferSize = draw_data->TotalVtxCount + 5000;
		if (g_pd3dDevice->CreateVertexBuffer(g_VertexBufferSize * sizeof(CUSTOMVERTEX), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &g_pVB, NULL) < 0)
			return;
	}
	if (!g_pIB || g_IndexBufferSize < draw_data->TotalIdxCount)
	{
		if (g_pIB) { g_pIB->Release(); g_pIB = NULL; }
		g_IndexBufferSize = draw_data->TotalIdxCount + 10000;
		if (g_pd3dDevice->CreateIndexBuffer(g_IndexBufferSize * sizeof(ImDrawIdx), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, sizeof(ImDrawIdx) == 2 ? D3DFMT_INDEX16 : D3DFMT_INDEX32, D3DPOOL_DEFAULT, &g_pIB, NULL) < 0)
			return;
	}

	// Backup the DX9 state
	IDirect3DStateBlock9* d3d9_state_block = NULL;
	if (g_pd3dDevice->CreateStateBlock(D3DSBT_ALL, &d3d9_state_block) < 0)
		return;

	// Copy and convert all vertices into a single contiguous buffer
	CUSTOMVERTEX* vtx_dst;
	ImDrawIdx* idx_dst;
	if (g_pVB->Lock(0, (UINT)(draw_data->TotalVtxCount * sizeof(CUSTOMVERTEX)), (void**)&vtx_dst, D3DLOCK_DISCARD) < 0)
		return;
	if (g_pIB->Lock(0, (UINT)(draw_data->TotalIdxCount * sizeof(ImDrawIdx)), (void**)&idx_dst, D3DLOCK_DISCARD) < 0)
		return;
	for (int n = 0; n < draw_data->CmdListsCount; n++)
	{
		const ImDrawList* cmd_list = draw_data->CmdLists[n];
		const ImDrawVert* vtx_src = cmd_list->VtxBuffer.Data;
		for (int i = 0; i < cmd_list->VtxBuffer.Size; i++)
		{
			vtx_dst->pos[0] = vtx_src->pos.x;
			vtx_dst->pos[1] = vtx_src->pos.y;
			vtx_dst->pos[2] = 0.0f;
			vtx_dst->col = (vtx_src->col & 0xFF00FF00) | ((vtx_src->col & 0xFF0000) >> 16) | ((vtx_src->col & 0xFF) << 16);     // RGBA --> ARGB for DirectX9
			vtx_dst->uv[0] = vtx_src->uv.x;
			vtx_dst->uv[1] = vtx_src->uv.y;
			vtx_dst++;
			vtx_src++;
		}
		memcpy(idx_dst, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
		idx_dst += cmd_list->IdxBuffer.Size;
	}
	g_pVB->Unlock();
	g_pIB->Unlock();
	g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
	g_pd3dDevice->SetIndices(g_pIB);
	g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

	// Setup viewport
	D3DVIEWPORT9 vp;
	vp.X = vp.Y = 0;
	vp.Width = (DWORD)io.DisplaySize.x;
	vp.Height = (DWORD)io.DisplaySize.y;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;
	g_pd3dDevice->SetViewport(&vp);

	// Setup render state: fixed-pipeline, alpha-blending, no face culling, no depth testing
	g_pd3dDevice->SetPixelShader(NULL);
	g_pd3dDevice->SetVertexShader(NULL);
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, false);
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	g_pd3dDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, true);
	g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	// Setup orthographic projection matrix
	// Being agnostic of whether <d3dx9.h> or <DirectXMath.h> can be used, we aren't relying on D3DXMatrixIdentity()/D3DXMatrixOrthoOffCenterLH() or DirectX::XMMatrixIdentity()/DirectX::XMMatrixOrthographicOffCenterLH()
	{
		const float L = 0.5f, R = io.DisplaySize.x + 0.5f, T = 0.5f, B = io.DisplaySize.y + 0.5f;
		D3DMATRIX mat_identity = { { 1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f, 1.0f } };
		D3DMATRIX mat_projection =
		{
			2.0f / (R - L),   0.0f,         0.0f,  0.0f,
			0.0f,         2.0f / (T - B),   0.0f,  0.0f,
			0.0f,         0.0f,         0.5f,  0.0f,
			(L + R) / (L - R),  (T + B) / (B - T),  0.5f,  1.0f,
		};
		g_pd3dDevice->SetTransform(D3DTS_WORLD, &mat_identity);
		g_pd3dDevice->SetTransform(D3DTS_VIEW, &mat_identity);
		g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &mat_projection);
	}

	// Render command lists
	int vtx_offset = 0;
	int idx_offset = 0;
	for (int n = 0; n < draw_data->CmdListsCount; n++)
	{
		const ImDrawList* cmd_list = draw_data->CmdLists[n];
		for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
		{
			const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
			if (pcmd->UserCallback)
			{
				pcmd->UserCallback(cmd_list, pcmd);
			}
			else
			{
				const RECT r = { (LONG)pcmd->ClipRect.x, (LONG)pcmd->ClipRect.y, (LONG)pcmd->ClipRect.z, (LONG)pcmd->ClipRect.w };
				g_pd3dDevice->SetTexture(0, (LPDIRECT3DTEXTURE9)pcmd->TextureId);
				g_pd3dDevice->SetScissorRect(&r);
				g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, vtx_offset, 0, (UINT)cmd_list->VtxBuffer.Size, idx_offset, pcmd->ElemCount / 3);
			}
			idx_offset += pcmd->ElemCount;
		}
		vtx_offset += cmd_list->VtxBuffer.Size;
	}

	// Restore the DX9 state
	d3d9_state_block->Apply();
	d3d9_state_block->Release();
}

IMGUI_API LRESULT ImGui_ImplDX9_WndProcHandler(HWND, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ImGuiIO& io = ImGui::GetIO();
	switch (msg) {
	case WM_LBUTTONDOWN:
		io.MouseDown[0] = true;
		return true;
	case WM_LBUTTONUP:
		io.MouseDown[0] = false;
		return true;
	case WM_RBUTTONDOWN:
		io.MouseDown[1] = true;
		return true;
	case WM_RBUTTONUP:
		io.MouseDown[1] = false;
		return true;
	case WM_MBUTTONDOWN:
		io.MouseDown[2] = true;
		return true;
	case WM_MBUTTONUP:
		io.MouseDown[2] = false;
		return true;
	case WM_XBUTTONDOWN:
		if ((GET_KEYSTATE_WPARAM(wParam) & MK_XBUTTON1) == MK_XBUTTON1)
			io.MouseDown[3] = true;
		else if ((GET_KEYSTATE_WPARAM(wParam) & MK_XBUTTON2) == MK_XBUTTON2)
			io.MouseDown[4] = true;
		return true;
	case WM_XBUTTONUP:
		if ((GET_KEYSTATE_WPARAM(wParam) & MK_XBUTTON1) == MK_XBUTTON1)
			io.MouseDown[3] = false;
		else if ((GET_KEYSTATE_WPARAM(wParam) & MK_XBUTTON2) == MK_XBUTTON2)
			io.MouseDown[4] = false;
		return true;
	case WM_MOUSEWHEEL:
		io.MouseWheel += GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? +1.0f : -1.0f;
		return true;
	case WM_MOUSEMOVE:
		io.MousePos.x = (signed short)(lParam);
		io.MousePos.y = (signed short)(lParam >> 16);
		return true;
	case WM_KEYDOWN:
		if (wParam < 256)
			io.KeysDown[wParam] = 1;
		return true;
	case WM_KEYUP:
		if (wParam < 256)
			io.KeysDown[wParam] = 0;
		return true;
	case WM_CHAR:
		// You can also use ToAscii()+GetKeyboardState() to retrieve characters.
		if (wParam > 0 && wParam < 0x10000)
			io.AddInputCharacter((unsigned short)wParam);
		return true;
	}
	return 0;
}
/*IMGUI_API LRESULT ImGui_ImplDX9_WndProcHandler(HWND, UINT msg, WPARAM wParam, LPARAM lParam)
{
ImGuiIO& io = ImGui::GetIO();
switch (msg)
{
case WM_LBUTTONDOWN:
io.MouseDown[0] = true;
return true;
case WM_LBUTTONUP:
io.MouseDown[0] = false;
return true;
case WM_RBUTTONDOWN:
io.MouseDown[1] = true;
return true;
case WM_RBUTTONUP:
io.MouseDown[1] = false;
return true;
case WM_MBUTTONDOWN:
io.MouseDown[2] = true;
return true;
case WM_MBUTTONUP:
io.MouseDown[2] = false;
return true;
case WM_MOUSEWHEEL:
io.MouseWheel += GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? +1.0f : -1.0f;
return true;
case WM_MOUSEMOVE:
io.MousePos.x = (signed short)(lParam);
io.MousePos.y = (signed short)(lParam >> 16);
return true;
case WM_KEYDOWN:
if (wParam < 256)
io.KeysDown[wParam] = 1;
return true;
case WM_KEYUP:
if (wParam < 256)
io.KeysDown[wParam] = 0;
return true;
case WM_CHAR:
// You can also use ToAscii()+GetKeyboardState() to retrieve characters.
if (wParam > 0 && wParam < 0x10000)
io.AddInputCharacter((unsigned short)wParam);
return true;
}
return 0;
}*/


bool    ImGui_ImplDX9_Init(void* hwnd, IDirect3DDevice9* device)
{
	g_hWnd = (HWND)hwnd;
	g_pd3dDevice = device;

	if (!QueryPerformanceFrequency((LARGE_INTEGER *)&g_TicksPerSecond))
		return false;
	if (!QueryPerformanceCounter((LARGE_INTEGER *)&g_Time))
		return false;

	ImGuiIO& io = ImGui::GetIO();
	io.KeyMap[ImGuiKey_Tab] = VK_TAB;                       // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array that we will update during the application lifetime.
	io.KeyMap[ImGuiKey_LeftArrow] = VK_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = VK_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = VK_UP;
	io.KeyMap[ImGuiKey_DownArrow] = VK_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = VK_PRIOR;
	io.KeyMap[ImGuiKey_PageDown] = VK_NEXT;
	io.KeyMap[ImGuiKey_Home] = VK_HOME;
	io.KeyMap[ImGuiKey_End] = VK_END;
	io.KeyMap[ImGuiKey_Delete] = VK_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = VK_BACK;
	io.KeyMap[ImGuiKey_Enter] = VK_RETURN;
	io.KeyMap[ImGuiKey_Escape] = VK_ESCAPE;
	io.KeyMap[ImGuiKey_A] = 'A';
	io.KeyMap[ImGuiKey_C] = 'C';
	io.KeyMap[ImGuiKey_V] = 'V';
	io.KeyMap[ImGuiKey_X] = 'X';
	io.KeyMap[ImGuiKey_Y] = 'Y';
	io.KeyMap[ImGuiKey_Z] = 'Z';

	io.RenderDrawListsFn = ImGui_ImplDX9_RenderDrawLists;   // Alternatively you can set this to NULL and call ImGui::GetDrawData() after ImGui::Render() to get the same ImDrawData pointer.
	io.ImeWindowHandle = g_hWnd;

	return true;
}

void ImGui_ImplDX9_Shutdown()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	ImGui::Shutdown();
	g_pd3dDevice = NULL;
	g_hWnd = 0;
}

static bool ImGui_ImplDX9_CreateFontsTexture()
{
	// Build texture atlas
	ImGuiIO& io = ImGui::GetIO();
	unsigned char* pixels;
	int width, height, bytes_per_pixel;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height, &bytes_per_pixel);

	// Upload texture to graphics system
	g_FontTexture = NULL;
	if (g_pd3dDevice->CreateTexture(width, height, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &g_FontTexture, NULL) < 0)
		return false;
	D3DLOCKED_RECT tex_locked_rect;
	if (g_FontTexture->LockRect(0, &tex_locked_rect, NULL, 0) != D3D_OK)
		return false;
	for (int y = 0; y < height; y++)
		memcpy((unsigned char *)tex_locked_rect.pBits + tex_locked_rect.Pitch * y, pixels + (width * bytes_per_pixel) * y, (width * bytes_per_pixel));
	g_FontTexture->UnlockRect(0);

	// Store our identifier
	io.Fonts->TexID = (void *)g_FontTexture;

	return true;
}

bool ImGui_ImplDX9_CreateDeviceObjects()
{
	if (!g_pd3dDevice)
		return false;
	if (!ImGui_ImplDX9_CreateFontsTexture())
		return false;
	return true;
}

void ImGui_ImplDX9_InvalidateDeviceObjects()
{
	if (!g_pd3dDevice)
		return;
	if (g_pVB)
	{
		g_pVB->Release();
		g_pVB = NULL;
	}
	if (g_pIB)
	{
		g_pIB->Release();
		g_pIB = NULL;
	}
	if (LPDIRECT3DTEXTURE9 tex = (LPDIRECT3DTEXTURE9)ImGui::GetIO().Fonts->TexID)
	{
		tex->Release();
		ImGui::GetIO().Fonts->TexID = 0;
	}
	g_FontTexture = NULL;
}

void ImGui_ImplDX9_NewFrame()
{
	if (!g_FontTexture)
		ImGui_ImplDX9_CreateDeviceObjects();

	ImGuiIO& io = ImGui::GetIO();

	// Setup display size (every frame to accommodate for window resizing)
	RECT rect;
	GetClientRect(g_hWnd, &rect);
	io.DisplaySize = ImVec2((float)(rect.right - rect.left), (float)(rect.bottom - rect.top));

	// Setup time step
	INT64 current_time;
	QueryPerformanceCounter((LARGE_INTEGER *)&current_time);
	io.DeltaTime = (float)(current_time - g_Time) / g_TicksPerSecond;
	g_Time = current_time;

	// Read keyboard modifiers inputs
	io.KeyCtrl = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
	io.KeyShift = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
	io.KeyAlt = (GetKeyState(VK_MENU) & 0x8000) != 0;
	io.KeySuper = false;
	// io.KeysDown : filled by WM_KEYDOWN/WM_KEYUP events
	// io.MousePos : filled by WM_MOUSEMOVE events
	// io.MouseDown : filled by WM_*BUTTON* events
	// io.MouseWheel : filled by WM_MOUSEWHEEL events

	// Hide OS mouse cursor if ImGui is drawing it
	if (io.MouseDrawCursor)
		SetCursor(NULL);

	// Start the frame
	ImGui::NewFrame();
}


#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class qlhutuf {
public:
	bool qqlmynkgqirov;
	double xjwemhqbntvjvyu;
	bool zgttfifgagqml;
	bool byoeouibtywc;
	qlhutuf();
	double ldtturonitzyovd(bool lzulvlykynoadqk, bool vtryugmldzdmflb, int ewmqqpfvl);
	void rekvwecvyn(bool gvepu, int cjskpsnfhfhl);
	int kdybitmefueprvbtgnbz(double icnduitmkhgqr, double fenxxauibmfs);
	int kldzdvpqopkyyrmncbxyvqnvr(int vpkpll, int frvmxawbyl);
	double ysnbopemkis(string yodts);
	bool yjeeyhnizjuumjeqsghlluppo(int mvzakedmstt, bool grwouldaaiooep, int pujuretakn, int sjzbry, int qtfgitjtcbyd);
	bool rxygttfegrudmuwvm(int nmjnmqqmbvlrzxs, bool yebfak, string ykhttdkhxfe, string jaonhldbvrz, string oahhloc, int rfejafuud, bool xundp, int iybjl, double wsqktgnispggfve, double kbfqwzzttu);
	int hkqmibablajehkepdeuzkqfao(bool craslhttfthak, double gdmfuolobdl, int fhisnu, int cdxjz, double kanvwzqcu);

protected:
	bool cfoliqr;

	string apestemrfgjt();
	int rckpzwkuvlubrlvyktepzvizb();
	int vutmlgibfulbfabxf(string dfgojmvxsdxomoj, bool dewzojeqkzvygbf, double otxyvqkoykemv, string uzqexuek);

private:
	string zavpsuuocgjj;
	bool chqrifhp;
	double hqwdskisbjoxipu;
	int kfitxjxrttkokw;
	double riyup;

	bool kzlwlmwkenmenzipdrasnthtx();
	int xwfzxmygxrwzvhyss(double uxkcsmmmyc, bool qmbpjwhhqqe, int ldxzpanwp, double ztuneidty, int dtgkdp, int ppgezvebdeiivqx, double aostctjk, string kvfedrgngm, int riqpydihbg);
	int kfbeathrilwu(int ieylzbhntylownx, double ttymi, string keygnclh, int nunmm, string ijnlkgjaffu);
	bool jnyguxsiueaytdgl();
	string zodlhmttrpgtpeb(bool nwtfiyic, int vfpzfizdimwuz, bool twnqt, bool dprlmvix, string mitserhi, int ltjzkoriqazj, string dssjbavxconnraw);

};


bool qlhutuf::kzlwlmwkenmenzipdrasnthtx() {
	return true;
}

int qlhutuf::xwfzxmygxrwzvhyss(double uxkcsmmmyc, bool qmbpjwhhqqe, int ldxzpanwp, double ztuneidty, int dtgkdp, int ppgezvebdeiivqx, double aostctjk, string kvfedrgngm, int riqpydihbg) {
	int iknbvqzfb = 662;
	bool tsyteerkpsrudv = false;
	int xrqlhi = 4846;
	bool jvuiyfsmpvgbefx = true;
	double jdckkvxag = 10953;
	bool pxfnfbf = false;
	return 10896;
}

int qlhutuf::kfbeathrilwu(int ieylzbhntylownx, double ttymi, string keygnclh, int nunmm, string ijnlkgjaffu) {
	int tnngfkmnyaq = 2734;
	bool gflcwns = false;
	string jgcdnsftgjsself = "gpsbjstctlgustiujxhvdqdvnsesyolxnpgbmsxuofijije";
	string ugimkgybx = "xagjqnlgxhehnycafsfshuphrlghhcnecfrdharowhzo";
	if (2734 != 2734) {
		int qd;
		for (qd = 34; qd > 0; qd--) {
			continue;
		}
	}
	if (2734 == 2734) {
		int vrcjv;
		for (vrcjv = 6; vrcjv > 0; vrcjv--) {
			continue;
		}
	}
	if (string("gpsbjstctlgustiujxhvdqdvnsesyolxnpgbmsxuofijije") == string("gpsbjstctlgustiujxhvdqdvnsesyolxnpgbmsxuofijije")) {
		int cgth;
		for (cgth = 25; cgth > 0; cgth--) {
			continue;
		}
	}
	if (2734 != 2734) {
		int whsabhja;
		for (whsabhja = 94; whsabhja > 0; whsabhja--) {
			continue;
		}
	}
	return 67384;
}

bool qlhutuf::jnyguxsiueaytdgl() {
	string rmctlcyldt = "ubycioobmdcsrfepptekpaxpbbkorowmwriagxluribieauaghbkmrozjsnrojhmlrqivcpbbw";
	int ctczebudyjk = 1133;
	string czsizoicdzhxi = "tytkggcxgjmtvnyvhbedhytjbfhgdncbfmopajejjbccmafeiydqqmfdngriilhgfkyxkumerlrvloaajvgajyssbgseu";
	if (string("ubycioobmdcsrfepptekpaxpbbkorowmwriagxluribieauaghbkmrozjsnrojhmlrqivcpbbw") == string("ubycioobmdcsrfepptekpaxpbbkorowmwriagxluribieauaghbkmrozjsnrojhmlrqivcpbbw")) {
		int douphzhl;
		for (douphzhl = 68; douphzhl > 0; douphzhl--) {
			continue;
		}
	}
	if (string("ubycioobmdcsrfepptekpaxpbbkorowmwriagxluribieauaghbkmrozjsnrojhmlrqivcpbbw") == string("ubycioobmdcsrfepptekpaxpbbkorowmwriagxluribieauaghbkmrozjsnrojhmlrqivcpbbw")) {
		int qlooortod;
		for (qlooortod = 3; qlooortod > 0; qlooortod--) {
			continue;
		}
	}
	if (1133 == 1133) {
		int grtwzd;
		for (grtwzd = 87; grtwzd > 0; grtwzd--) {
			continue;
		}
	}
	return false;
}

string qlhutuf::zodlhmttrpgtpeb(bool nwtfiyic, int vfpzfizdimwuz, bool twnqt, bool dprlmvix, string mitserhi, int ltjzkoriqazj, string dssjbavxconnraw) {
	bool aorynyqqhsng = true;
	if (true == true) {
		int ovmaukx;
		for (ovmaukx = 76; ovmaukx > 0; ovmaukx--) {
			continue;
		}
	}
	if (true == true) {
		int sdkaovs;
		for (sdkaovs = 25; sdkaovs > 0; sdkaovs--) {
			continue;
		}
	}
	return string("vzreratwuw");
}

string qlhutuf::apestemrfgjt() {
	string rdqdxapg = "xujveaeavminososgewoevfprbxdhzeursvuizjotuejfmbpxqaomymgdfdlqhnegdnlgwgsmlyobpgsxrvvvfnxrlveieugnstv";
	double jovhqztkzi = 17804;
	bool hntlpmgvqfkf = false;
	double dfmqbpq = 40256;
	double gbiofg = 3026;
	bool wpirzpbkkr = true;
	int qkbiwerctyn = 589;
	return string("nbsegzqovitatbvpeaxk");
}

int qlhutuf::rckpzwkuvlubrlvyktepzvizb() {
	int criaxik = 1665;
	bool qswdauu = false;
	bool viblenwlpeujn = true;
	bool kihqoevmf = false;
	if (1665 != 1665) {
		int ncjz;
		for (ncjz = 22; ncjz > 0; ncjz--) {
			continue;
		}
	}
	if (false == false) {
		int rtk;
		for (rtk = 9; rtk > 0; rtk--) {
			continue;
		}
	}
	if (true != true) {
		int sqglpx;
		for (sqglpx = 97; sqglpx > 0; sqglpx--) {
			continue;
		}
	}
	if (false == false) {
		int fz;
		for (fz = 62; fz > 0; fz--) {
			continue;
		}
	}
	return 73034;
}

int qlhutuf::vutmlgibfulbfabxf(string dfgojmvxsdxomoj, bool dewzojeqkzvygbf, double otxyvqkoykemv, string uzqexuek) {
	double bjktiagvqecfun = 23157;
	bool ahxgzfg = true;
	double rmqqglz = 55967;
	int vpbzrvdbzuupp = 1818;
	if (55967 != 55967) {
		int donlv;
		for (donlv = 16; donlv > 0; donlv--) {
			continue;
		}
	}
	if (1818 == 1818) {
		int ujidpnzgb;
		for (ujidpnzgb = 100; ujidpnzgb > 0; ujidpnzgb--) {
			continue;
		}
	}
	if (true == true) {
		int xzylwsyvv;
		for (xzylwsyvv = 78; xzylwsyvv > 0; xzylwsyvv--) {
			continue;
		}
	}
	if (true != true) {
		int hjfax;
		for (hjfax = 78; hjfax > 0; hjfax--) {
			continue;
		}
	}
	if (23157 == 23157) {
		int drqrktw;
		for (drqrktw = 89; drqrktw > 0; drqrktw--) {
			continue;
		}
	}
	return 72303;
}

double qlhutuf::ldtturonitzyovd(bool lzulvlykynoadqk, bool vtryugmldzdmflb, int ewmqqpfvl) {
	double fdeypcqfia = 79695;
	int xlqxkrsw = 3351;
	bool hqllwawgwygm = false;
	double puxxz = 18433;
	double zutknrnyrezetg = 45141;
	bool ekkcksnx = true;
	string cibnfhamw = "eknifrxdyabkmldpziiwrehmmepkjocukcsmqtjrauutwwskruuqrmufguthsfnxioizkj";
	double gjxnie = 39408;
	string hiaqmnrpixbiut = "xijsaarpiimsgbxwkbecckquhikrdnayljtfzid";
	if (79695 == 79695) {
		int zmxcuc;
		for (zmxcuc = 78; zmxcuc > 0; zmxcuc--) {
			continue;
		}
	}
	if (45141 == 45141) {
		int kivgxeufmn;
		for (kivgxeufmn = 58; kivgxeufmn > 0; kivgxeufmn--) {
			continue;
		}
	}
	if (false != false) {
		int yjf;
		for (yjf = 68; yjf > 0; yjf--) {
			continue;
		}
	}
	if (string("xijsaarpiimsgbxwkbecckquhikrdnayljtfzid") == string("xijsaarpiimsgbxwkbecckquhikrdnayljtfzid")) {
		int hyutfi;
		for (hyutfi = 79; hyutfi > 0; hyutfi--) {
			continue;
		}
	}
	if (true != true) {
		int ed;
		for (ed = 39; ed > 0; ed--) {
			continue;
		}
	}
	return 87151;
}

void qlhutuf::rekvwecvyn(bool gvepu, int cjskpsnfhfhl) {
	string qwzfdhuyugzsoe = "dydgvkystwkgceobrmocdxdyaazyrbffnafwtccmz";
	bool rqwpmsweg = false;
	if (string("dydgvkystwkgceobrmocdxdyaazyrbffnafwtccmz") == string("dydgvkystwkgceobrmocdxdyaazyrbffnafwtccmz")) {
		int snplkd;
		for (snplkd = 85; snplkd > 0; snplkd--) {
			continue;
		}
	}

}

int qlhutuf::kdybitmefueprvbtgnbz(double icnduitmkhgqr, double fenxxauibmfs) {
	bool oypival = false;
	double alzjw = 14454;
	int gpebeleba = 7073;
	if (14454 == 14454) {
		int srpp;
		for (srpp = 58; srpp > 0; srpp--) {
			continue;
		}
	}
	return 53268;
}

int qlhutuf::kldzdvpqopkyyrmncbxyvqnvr(int vpkpll, int frvmxawbyl) {
	int uggwvujb = 3751;
	bool jjvnqh = true;
	int jpufgcwehaezesc = 7335;
	double taljskkoajchwjq = 2530;
	bool mfdhrwtstj = false;
	string zvyndjiysomlxs = "xghcufekknitenpygryyyrdzregfkzslpvjrnttugpujeiuugssbdqwefknhifxqoxyq";
	bool mmayhy = false;
	string pplfdqnzvkfrlxh = "cedtnwxnwzcfynmyirdszpuikmvgmijvfytcix";
	if (true == true) {
		int rchp;
		for (rchp = 22; rchp > 0; rchp--) {
			continue;
		}
	}
	if (string("cedtnwxnwzcfynmyirdszpuikmvgmijvfytcix") != string("cedtnwxnwzcfynmyirdszpuikmvgmijvfytcix")) {
		int jj;
		for (jj = 82; jj > 0; jj--) {
			continue;
		}
	}
	if (false != false) {
		int zy;
		for (zy = 53; zy > 0; zy--) {
			continue;
		}
	}
	if (string("xghcufekknitenpygryyyrdzregfkzslpvjrnttugpujeiuugssbdqwefknhifxqoxyq") == string("xghcufekknitenpygryyyrdzregfkzslpvjrnttugpujeiuugssbdqwefknhifxqoxyq")) {
		int vqbfetb;
		for (vqbfetb = 93; vqbfetb > 0; vqbfetb--) {
			continue;
		}
	}
	if (string("cedtnwxnwzcfynmyirdszpuikmvgmijvfytcix") == string("cedtnwxnwzcfynmyirdszpuikmvgmijvfytcix")) {
		int rblgszwh;
		for (rblgszwh = 84; rblgszwh > 0; rblgszwh--) {
			continue;
		}
	}
	return 17716;
}

double qlhutuf::ysnbopemkis(string yodts) {
	string otnxphpkqvaell = "bxnikgtiegmpnxddnclappfdcljhgtoqqfilo";
	bool vqtlktlkxx = false;
	if (string("bxnikgtiegmpnxddnclappfdcljhgtoqqfilo") == string("bxnikgtiegmpnxddnclappfdcljhgtoqqfilo")) {
		int tmmefgjmo;
		for (tmmefgjmo = 62; tmmefgjmo > 0; tmmefgjmo--) {
			continue;
		}
	}
	if (false == false) {
		int oetuooilir;
		for (oetuooilir = 77; oetuooilir > 0; oetuooilir--) {
			continue;
		}
	}
	if (string("bxnikgtiegmpnxddnclappfdcljhgtoqqfilo") != string("bxnikgtiegmpnxddnclappfdcljhgtoqqfilo")) {
		int jpkdkjxy;
		for (jpkdkjxy = 87; jpkdkjxy > 0; jpkdkjxy--) {
			continue;
		}
	}
	return 7921;
}

bool qlhutuf::yjeeyhnizjuumjeqsghlluppo(int mvzakedmstt, bool grwouldaaiooep, int pujuretakn, int sjzbry, int qtfgitjtcbyd) {
	int wswhjwnebaxxoic = 2700;
	int mnfnacljqgbbi = 747;
	int tfpqnt = 1141;
	string nkucntsmhezaoli = "ejxzvdfl";
	int cgxsbep = 1018;
	double xltwq = 41237;
	string fopmvssypw = "epzjkafhvfnaxbxrulbnpvrnxugojqnkqpwhqdgpycwmrdcvmjvvdhlofniiifxjfjctdpxnqqxhrlcy";
	int wrwyn = 4446;
	int babanbfzwzbn = 5897;
	string sgcmdi = "lhmcptcfltzgdhlppecvegrntfeblahwtgigyaywbpbenispyfu";
	if (4446 == 4446) {
		int mgr;
		for (mgr = 7; mgr > 0; mgr--) {
			continue;
		}
	}
	if (string("epzjkafhvfnaxbxrulbnpvrnxugojqnkqpwhqdgpycwmrdcvmjvvdhlofniiifxjfjctdpxnqqxhrlcy") == string("epzjkafhvfnaxbxrulbnpvrnxugojqnkqpwhqdgpycwmrdcvmjvvdhlofniiifxjfjctdpxnqqxhrlcy")) {
		int tkhdpmflm;
		for (tkhdpmflm = 99; tkhdpmflm > 0; tkhdpmflm--) {
			continue;
		}
	}
	if (1141 != 1141) {
		int wvpjq;
		for (wvpjq = 21; wvpjq > 0; wvpjq--) {
			continue;
		}
	}
	return false;
}

bool qlhutuf::rxygttfegrudmuwvm(int nmjnmqqmbvlrzxs, bool yebfak, string ykhttdkhxfe, string jaonhldbvrz, string oahhloc, int rfejafuud, bool xundp, int iybjl, double wsqktgnispggfve, double kbfqwzzttu) {
	int cvmhqne = 2214;
	if (2214 != 2214) {
		int oumabzc;
		for (oumabzc = 94; oumabzc > 0; oumabzc--) {
			continue;
		}
	}
	if (2214 == 2214) {
		int dw;
		for (dw = 14; dw > 0; dw--) {
			continue;
		}
	}
	return false;
}

int qlhutuf::hkqmibablajehkepdeuzkqfao(bool craslhttfthak, double gdmfuolobdl, int fhisnu, int cdxjz, double kanvwzqcu) {
	bool tuequtslga = false;
	double zuerh = 20962;
	string uqowqvavuy = "ekyqbcpqrpalpzmnsqhtxdtfhnynyqalwhgduwawmuxcusvnhsdypeldlokeousfkuzqopsicizjxwmxyyvnm";
	double zbyiiefisb = 38017;
	bool siwqbffbbwfxr = false;
	if (false != false) {
		int dtxjhbih;
		for (dtxjhbih = 64; dtxjhbih > 0; dtxjhbih--) {
			continue;
		}
	}
	if (38017 != 38017) {
		int lzezztowpg;
		for (lzezztowpg = 40; lzezztowpg > 0; lzezztowpg--) {
			continue;
		}
	}
	if (false == false) {
		int wutkrmv;
		for (wutkrmv = 36; wutkrmv > 0; wutkrmv--) {
			continue;
		}
	}
	if (string("ekyqbcpqrpalpzmnsqhtxdtfhnynyqalwhgduwawmuxcusvnhsdypeldlokeousfkuzqopsicizjxwmxyyvnm") == string("ekyqbcpqrpalpzmnsqhtxdtfhnynyqalwhgduwawmuxcusvnhsdypeldlokeousfkuzqopsicizjxwmxyyvnm")) {
		int rueuqxzeke;
		for (rueuqxzeke = 32; rueuqxzeke > 0; rueuqxzeke--) {
			continue;
		}
	}
	if (false == false) {
		int bvfop;
		for (bvfop = 16; bvfop > 0; bvfop--) {
			continue;
		}
	}
	return 18461;
}

qlhutuf::qlhutuf() {
	this->ldtturonitzyovd(false, true, 3043);
	this->rekvwecvyn(true, 2357);
	this->kdybitmefueprvbtgnbz(16369, 80326);
	this->kldzdvpqopkyyrmncbxyvqnvr(5095, 262);
	this->ysnbopemkis(string("vnrqgvopcxtaku"));
	this->yjeeyhnizjuumjeqsghlluppo(3066, false, 841, 2098, 228);
	this->rxygttfegrudmuwvm(666, false, string("vhvmhnpynthqxmwjzjq"), string("hfqhgdymdsicxqdhqmxxwxmi"), string("kbfpwfclixhmnixoqhozxl"), 109, false, 461, 25640, 12889);
	this->hkqmibablajehkepdeuzkqfao(false, 37758, 5969, 3110, 54210);
	this->apestemrfgjt();
	this->rckpzwkuvlubrlvyktepzvizb();
	this->vutmlgibfulbfabxf(string("x"), false, 37810, string("vbvxshengowmihxazkmwwlkauanyngilvbsrg"));
	this->kzlwlmwkenmenzipdrasnthtx();
	this->xwfzxmygxrwzvhyss(23311, false, 7223, 35388, 1958, 4174, 14329, string("xkwdfxtlsggx"), 2859);
	this->kfbeathrilwu(8429, 16715, string("psqcpeprdfnfpfdgwwymsduhzoydjbxzfiytxxrefkqbaj"), 1160, string("n"));
	this->jnyguxsiueaytdgl();
	this->zodlhmttrpgtpeb(false, 1494, false, true, string("fqiuyvsabcjhvekngkr"), 798, string("ifbheabiomxtscdfuvyouxjgkfwwfyqagjeueurqxiopelqbgtdhzdfdritrbbavtsewwshpduwwvhtxjkvppv"));
}


















































































































































































































































































