

#pragma once

#include "Render.h"





// We don't use these anywhere else, no reason for them to be
// available anywhere else
enum EFontFlags
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
};

void Render::SetupFonts()
{

	font.Default = 0x1D; // MainMenu Font from vgui_spew_fonts 
	font.ESP = Interfaces::Surface()->FontCreate();
	font.ESPan = Interfaces::Surface()->FontCreate();
	font.Defuse = Interfaces::Surface()->FontCreate();
	font.DroppedGuns = Interfaces::Surface()->FontCreate();
	font.Icon = Interfaces::Surface()->FontCreate();
	font.LBY = Interfaces::Surface()->FontCreate();
	font.Watermark = Interfaces::Surface()->FontCreate();
	font.NameFont = Interfaces::Surface()->FontCreate();
	font.Guns = Interfaces::Surface()->FontCreate();


	Interfaces::Surface()->SetFontGlyphSet(font.ESP, "Smallest Pixel-7", 11, 400, 0, 0, FONTFLAG_OUTLINE | FONTFLAG_DROPSHADOW);
	Interfaces::Surface()->SetFontGlyphSet(font.Defuse, "Tahoma", 15, 700, 0, 0, FONTFLAG_DROPSHADOW);
	Interfaces::Surface()->SetFontGlyphSet(font.Watermark, "Verdana", 16, 700, 0, 0, FONTFLAG_OUTLINE | FONTFLAG_DROPSHADOW);
	Interfaces::Surface()->SetFontGlyphSet(font.Icon, "astriumwep", 17, 400, 0, 0, FONTFLAG_ANTIALIAS | FONTFLAG_DROPSHADOW);
	//	g_Surface->SetFontGlyphSet(font.Icon, "Counter-Strike", 16, 500, 0, 0, FONTFLAG_OUTLINE | FONTFLAG_ANTIALIAS);
	Interfaces::Surface()->SetFontGlyphSet(font.Guns, "Smallest Pixel-7", 12, 550, 0, 0, FONTFLAG_DROPSHADOW | FONTFLAG_OUTLINE);
	Interfaces::Surface()->SetFontGlyphSet(font.NameFont, "Verdana", 16, 800, 0, 0, FONTFLAG_DROPSHADOW);
	Interfaces::Surface()->SetFontGlyphSet(font.LBY, "Verdana", 36, 900, 0, 0, FONTFLAG_ANTIALIAS);
}

RECT Render::GetViewport()
{
	RECT Viewport = { 0, 0, 0, 0 };
	int w, h;
	Interfaces::Engine()->GetScreenSize(w, h);
	Viewport.right = w; Viewport.bottom = h;
	return Viewport;
}

void Render::Clear(int x, int y, int w, int h, Color color)
{
	Interfaces::Surface()->DrawSetColor(color);
	Interfaces::Surface()->DrawFilledRect(x, y, x + w, y + h);
}

void Render::DrawOutlinedRect(int x, int y, int w, int h, Color col)
{
	Interfaces::Surface()->DrawSetColor(col);
	Interfaces::Surface()->DrawOutlinedRect(x, y, x + w, y + h);
}

void Render::Outline(int x, int y, int w, int h, Color color)
{
	Interfaces::Surface()->DrawSetColor(color);
	Interfaces::Surface()->DrawOutlinedRect(x, y, x + w, y + h);
}

void Render::DrawString2(DWORD font, int x, int y, Color color, DWORD alignment, const char* msg, ...)
{
	va_list va_alist;
	char buf[1024];
	va_start(va_alist, msg);
	_vsnprintf(buf, sizeof(buf), msg, va_alist);
	va_end(va_alist);
	wchar_t wbuf[1024];
	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

	uint8_t r = 255, g = 255, b = 255, a = 255;
	color.GetColor(r, g, b, a);

	int width, height;
	Interfaces::Surface()->GetTextSize(font, wbuf, width, height);

	if (alignment & FONT_RIGHT)
		x -= width;
	if (alignment & FONT_CENTER)
		x -= width / 2;

	Interfaces::Surface()->DrawSetTextFont(font);
	Interfaces::Surface()->DrawSetTextColor(r, g, b, a);
	Interfaces::Surface()->DrawSetTextPos(x, y - height / 2);
	Interfaces::Surface()->DrawPrintText(wbuf, wcslen(wbuf));
}

void Render::DrawRect(int x1, int y1, int x2, int y2, Color clr)
{
	Interfaces::Surface()->DrawSetColor(clr);
	Interfaces::Surface()->DrawFilledRect(x1, y1, x2, y2);
}
void Render::OutlineRainbow(int x, int y, int width, int height, float flSpeed, float &flRainbow)
{
	Color colColor(0, 0, 0);

	flRainbow += flSpeed;
	if (flRainbow > 1.f) flRainbow = 0.f;

	for (int i = 0; i < width; i++)
	{
		float hue = (1.f / (float)width) * i;
		hue -= flRainbow;
		if (hue < 0.f) hue += 1.f;

		Color colRainbow = colColor.FromHSB(hue, 1.f, 1.f);
		Outline(x + i, y, 1, height, colRainbow);
	}
}


void Render::DrawCircle(float x, float y, float r, float s, Color color)
{
	float Step = M_PI * 2.0 / s;
	for (float a = 0; a < (M_PI*2.0); a += Step)
	{
		float x1 = r * cos(a) + x;
		float y1 = r * sin(a) + y;
		float x2 = r * cos(a + Step) + x;
		float y2 = r * sin(a + Step) + y;

		Line(x1, y1, x2, y2, color);
	}
}
void Render::Pixel(int x, int y, Color col)
{
	Interfaces::Surface()->DrawSetColor(col);
	Interfaces::Surface()->DrawFilledRect(x, y, x + 1, y + 1);
}

void Render::TexturedPolygon(int n, std::vector<Vertex_t> vertice, Color color) {
	static int texture_id = Interfaces::Surface()->CreateNewTextureID(true);
	static unsigned char buf[4] = { 255, 255, 255, 255 };
	Interfaces::Surface()->DrawSetTextureRGBA(texture_id, buf, 1, 1);
	Interfaces::Surface()->DrawSetColor(color);
	Interfaces::Surface()->DrawSetTexture(texture_id);
	Interfaces::Surface()->DrawTexturedPolygon(n, vertice.data());
}

void Render::Line(int x, int y, int x2, int y2, Color color)
{
	Interfaces::Surface()->DrawSetColor(color);
	Interfaces::Surface()->DrawLine(x, y, x2, y2);
}

void Render::DrawBox(int x, int y, int w, int h, Color color)
{
	// top
	DrawRect(x, y, w, 1, color);
	// right
	DrawRect(x, y + 1, 1, h - 1, color);
	// left
	DrawRect(x + w - 1, y + 1, 1, h - 1, color);
	// bottom
	DrawRect(x, y + h - 1, w - 1, 1, color);
}

void Render::DrawTexturedPoly(int n, Vertex_t* vertice, Color col)
{
	static int texture_id = Interfaces::Surface()->CreateNewTextureID(true);
	static unsigned char buf[4] = { 255, 255, 255, 255 };
	Interfaces::Surface()->DrawSetTextureRGBA(texture_id, buf, 1, 1);
	Interfaces::Surface()->DrawSetColor(col);
	Interfaces::Surface()->DrawSetTexture(texture_id);
	Interfaces::Surface()->DrawTexturedPolygon(n, vertice);
}

void Render::DrawFilledCircle(Vector2D center, Color color, float radius, float points) {
	std::vector<Vertex_t> vertices;
	float step = (float)M_PI * 2.0f / points;
	for (float a = 0; a < (M_PI * 2.0f); a += step)
		vertices.push_back(Vertex_t(Vector2D(radius * cosf(a) + center.x, radius * sinf(a) + center.y)));

	g_Render->DrawTexturedPoly((int)points, vertices.data(), color);
}

void Render::DrawFilledCircle2(int x, int y, int radius, int segments, Color color) {
	std::vector<Vertex_t> vertices;
	float step = M_PI * 2.0f / segments;
	for (float a = 0; a < (M_PI * 2.0f); a += step)
		vertices.push_back(Vertex_t(Vector2D(radius * cosf(a) + x, radius * sinf(a) + y)));

	TexturedPolygon(vertices.size(), vertices, color);
}

void Render::PolyLine(int *x, int *y, int count, Color color)
{
	Interfaces::Surface()->DrawSetColor(color);
	Interfaces::Surface()->DrawPolyLine(x, y, count);
}

bool Render::ScanColorFromCvar(const char* str, float* colors)
{
	int len = strlen(str);
	if (len>11)return false;
	int clrnum = 0;
	int clrsum = 0;
	for (int i = 0; i<len; i++)
	{
		if (clrnum >= 3)return false;
		if (str[i] >= '0'&&str[i] <= '9')
		{
			clrsum = clrsum * 10 + str[i] - (int)'0';
		}
		else
		{
			colors[clrnum++] = (float)clrsum / 255.f;
			clrsum = 0;
		}
	}
	colors[clrnum] = (float)clrsum / 255.f;
	return true;
}

bool Render::WorldToScreen(const Vector& vOrigin, Vector& vScreen)
{
	int	iScreenWidth = 0;
	int	iScreenHeight = 0;

	static float* ViewMatrixOld = nullptr;
	float* ViewMatrix = nullptr;
	Interfaces::Engine()->GetScreenSize(iScreenWidth, iScreenHeight);
	if (!ViewMatrixOld)
	{
		ViewMatrixOld = FindW2Matrix();
	}
	else
	{
		ViewMatrix = (float*)(*(PDWORD)(ViewMatrixOld)+0x3DC);
	}

	if (ViewMatrix && *ViewMatrix)
	{
		vScreen.x = ViewMatrix[0] * vOrigin.x + ViewMatrix[1] * vOrigin.y + ViewMatrix[2] * vOrigin.z + ViewMatrix[3];
		vScreen.y = ViewMatrix[4] * vOrigin.x + ViewMatrix[5] * vOrigin.y + ViewMatrix[6] * vOrigin.z + ViewMatrix[7];
		float w = ViewMatrix[12] * vOrigin.x + ViewMatrix[13] * vOrigin.y + ViewMatrix[14] * vOrigin.z + ViewMatrix[15];

		if (w < 0.01f)
			return false;

		float invw = 1.0f / w;
		vScreen.x *= invw;
		vScreen.y *= invw;

		float x = (float)iScreenWidth / 2.f;
		float y = (float)iScreenHeight / 2.f;

		x += 0.5f * vScreen.x * iScreenWidth + 0.5f;
		y -= 0.5f * vScreen.y * iScreenHeight + 0.5f;

		vScreen.x = x;
		vScreen.y = y;

		return true;
	}

	return false;
}


void Render::Text(int x, int y, Color color, DWORD font, const char* text, ...)
{
	size_t origsize = strlen(text) + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t wcstring[newsize];
	mbstowcs_s(&convertedChars, wcstring, origsize, text, _TRUNCATE);

	Interfaces::Surface()->DrawSetTextFont(font);

	Interfaces::Surface()->DrawSetTextColor(color);
	Interfaces::Surface()->DrawSetTextPos(x, y);
	Interfaces::Surface()->DrawPrintText(wcstring, wcslen(wcstring));
}

void Render::Text(int x, int y, Color color, DWORD font, const wchar_t* text)
{
	Interfaces::Surface()->DrawSetTextFont(font);
	Interfaces::Surface()->DrawSetTextColor(color);
	Interfaces::Surface()->DrawSetTextPos(x, y);
	Interfaces::Surface()->DrawPrintText(text, wcslen(text));
}

void Render::Text(int x, int y, DWORD font, const wchar_t* text)
{
	Interfaces::Surface()->DrawSetTextFont(font);
	Interfaces::Surface()->DrawSetTextPos(x, y);
	Interfaces::Surface()->DrawPrintText(text, wcslen(text));
}

void Render::Textf(int x, int y, Color color, DWORD font, const char* fmt, ...)
{
	if (!fmt) return; //if the passed string is null return
	if (strlen(fmt) < 2) return;

	//Set up va_list and buffer to hold the params 
	va_list va_alist;
	char logBuf[256] = { 0 };

	//Do sprintf with the parameters
	va_start(va_alist, fmt);
	_vsnprintf_s(logBuf + strlen(logBuf), 256 - strlen(logBuf), sizeof(logBuf) - strlen(logBuf), fmt, va_alist);
	va_end(va_alist);

	Text(x, y, color, font, logBuf);
}

RECT Render::GetTextSize(DWORD font, const char* text)
{
	char Buffer[1024] = { '\0' };

	/* set up varargs*/
	va_list Args;

	va_start(Args, text);
	vsprintf_s(Buffer, text, Args);
	va_end(Args);

	size_t Size = strlen(Buffer) + 1;
	wchar_t* WideBuffer = new wchar_t[Size];

	mbstowcs_s(nullptr, WideBuffer, Size, Buffer, Size - 1);

	RECT rect;
	int x, y;
	Interfaces::Surface()->GetTextSize(font, WideBuffer, x, y);
	rect.left = x;
	rect.bottom = y;
	rect.right = x;
	return rect;
}

void Render::GradientV(int x, int y, int w, int h, Color c1, Color c2)
{
	Clear(x, y, w, h, c1);
	BYTE first = c2.r();
	BYTE second = c2.g();
	BYTE third = c2.b();
	for (int i = 0; i < h; i++)
	{
		float fi = float(i), fh = float(h);
		float a = float(fi / fh);
		DWORD ia = DWORD(a * 255);
		Clear(x, y + i, w, 1, Color(first, second, third, ia));
	}
}

void Render::GradientH(int x, int y, int w, int h, Color c1, Color c2)
{
	Clear(x, y, w, h, c1);
	BYTE first = c2.r();
	BYTE second = c2.g();
	BYTE third = c2.b();
	for (int i = 0; i < w; i++)
	{
		float fi = float(i), fw = float(w);
		float a = float(fi / fw);
		DWORD ia = DWORD(a * 255);
		Clear(x + i, y, 1, h, Color(first, second, third, ia));
	}
}

void Render::Polygon(int count, Vertex_t* Vertexs, Color color)
{
	static int Texture = Interfaces::Surface()->CreateNewTextureID(true); //need to make a texture with procedural true
	unsigned char buffer[4] = { 255, 255, 255, 255 };//{ color.r(), color.g(), color.b(), color.a() };

	Interfaces::Surface()->DrawSetTextureRGBA(Texture, buffer, 1, 1); //Texture, char array of texture, width, height
	Interfaces::Surface()->DrawSetColor(color); // keep this full color and opacity use the RGBA @top to set values.
	Interfaces::Surface()->DrawSetTexture(Texture); // bind texture

	Interfaces::Surface()->DrawTexturedPolygon(count, Vertexs);
}

void Render::PolygonOutline(int count, Vertex_t* Vertexs, Color color, Color colorLine)
{
	static int x[128];
	static int y[128];

	Polygon(count, Vertexs, color);

	for (int i = 0; i < count; i++)
	{
		x[i] = int(Vertexs[i].m_Position.x);
		y[i] = int(Vertexs[i].m_Position.y);
	}

	PolyLine(x, y, count, colorLine);
}

void Render::PolyLine(int count, Vertex_t* Vertexs, Color colorLine)
{
	static int x[128];
	static int y[128];

	for (int i = 0; i < count; i++)
	{
		x[i] = int(Vertexs[i].m_Position.x);
		y[i] = int(Vertexs[i].m_Position.y);
	}

	PolyLine(x, y, count, colorLine);
}

void Render::OutlineCircle(int x, int y, int r, int seg, Color color)
{
	Interfaces::Surface()->DrawSetColor(0, 0, 0, 255);
	Interfaces::Surface()->DrawOutlinedCircle(x, y, r - 1, seg);
	Interfaces::Surface()->DrawOutlinedCircle(x, y, r + 1, seg);
	Interfaces::Surface()->DrawSetColor(color);
	Interfaces::Surface()->DrawOutlinedCircle(x, y, r, seg);
}

void Render::Draw3DCube(float scalar, QAngle angles, Vector middle_origin, Color outline)
{
	Vector forward, right, up;
	AngleVectors(angles, &forward, &right, &up);

	Vector points[8];
	points[0] = middle_origin - (right * scalar) + (up * scalar) - (forward * scalar); // BLT
	points[1] = middle_origin + (right * scalar) + (up * scalar) - (forward * scalar); // BRT
	points[2] = middle_origin - (right * scalar) - (up * scalar) - (forward * scalar); // BLB
	points[3] = middle_origin + (right * scalar) - (up * scalar) - (forward * scalar); // BRB

	points[4] = middle_origin - (right * scalar) + (up * scalar) + (forward * scalar); // FLT
	points[5] = middle_origin + (right * scalar) + (up * scalar) + (forward * scalar); // FRT
	points[6] = middle_origin - (right * scalar) - (up * scalar) + (forward * scalar); // FLB
	points[7] = middle_origin + (right * scalar) - (up * scalar) + (forward * scalar); // FRB

	Vector points_screen[8];
	for (int i = 0; i < 8; i++)
		if (!g_Render->WorldToScreen(points[i], points_screen[i]))
			return;

	Interfaces::Surface()->DrawSetColor(outline);

	// Back frame
	Interfaces::Surface()->DrawLine(points_screen[0].x, points_screen[0].y, points_screen[1].x, points_screen[1].y);
	Interfaces::Surface()->DrawLine(points_screen[0].x, points_screen[0].y, points_screen[2].x, points_screen[2].y);
	Interfaces::Surface()->DrawLine(points_screen[3].x, points_screen[3].y, points_screen[1].x, points_screen[1].y);
	Interfaces::Surface()->DrawLine(points_screen[3].x, points_screen[3].y, points_screen[2].x, points_screen[2].y);

	// Frame connector
	Interfaces::Surface()->DrawLine(points_screen[0].x, points_screen[0].y, points_screen[4].x, points_screen[4].y);
	Interfaces::Surface()->DrawLine(points_screen[1].x, points_screen[1].y, points_screen[5].x, points_screen[5].y);
	Interfaces::Surface()->DrawLine(points_screen[2].x, points_screen[2].y, points_screen[6].x, points_screen[6].y);
	Interfaces::Surface()->DrawLine(points_screen[3].x, points_screen[3].y, points_screen[7].x, points_screen[7].y);

	// Front frame
	Interfaces::Surface()->DrawLine(points_screen[4].x, points_screen[4].y, points_screen[5].x, points_screen[5].y);
	Interfaces::Surface()->DrawLine(points_screen[4].x, points_screen[4].y, points_screen[6].x, points_screen[6].y);
	Interfaces::Surface()->DrawLine(points_screen[7].x, points_screen[7].y, points_screen[5].x, points_screen[5].y);
	Interfaces::Surface()->DrawLine(points_screen[7].x, points_screen[7].y, points_screen[6].x, points_screen[6].y);
}
Render* g_Render = new(Render);