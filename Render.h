
#pragma once

#include "Interfaces.h"

#include "Vector2D.h"

void Quad();


class Render
{
public:
	void SetupFonts();

	// Normal Drawing functions
	void Clear(int x, int y, int w, int h, Color color);
	void DrawOutlinedRect(int x, int y, int w, int h, Color col);
	void Outline(int x, int y, int w, int h, Color color);
	void DrawString2(DWORD font, int x, int y, Color color, DWORD alignment, const char* msg, ...);
	void DrawRect(int x1, int y1, int x2, int y2, Color clr);
	void OutlineRainbow(int x, int y, int w, int h, float flSpeed, float &flRainbow);
	void Line(int x, int y, int x2, int y2, Color color);
	void DrawTexturedPoly(int n, Vertex_t* vertice, Color col);
	void DrawCircle(float x, float y, float r, float s, Color color);
	void PolyLine(int *x, int *y, int count, Color color);
	void Polygon(int count, Vertex_t* Vertexs, Color color);
	void PolygonOutline(int count, Vertex_t* Vertexs, Color color, Color colorLine);
	void PolyLine(int count, Vertex_t* Vertexs, Color colorLine);
	void OutlineCircle(int x, int y, int r, int seg, Color color);
	void Pixel(int x, int y, Color col);
	void Draw3DCube(float scalar, QAngle angles, Vector middle_origin, Color outline);
	void DrawFilledCircle2(int x, int y, int radius, int segments, Color color);
	void TexturedPolygon(int n, std::vector<Vertex_t> vertice, Color color);
	void DrawFilledCircle(Vector2D center, Color color, float radius, float points);
	void DrawBox(int x, int y, int w, int h, Color color);
	// Gradient Functions
	void GradientV(int x, int y, int w, int h, Color c1, Color c2);
	void GradientH(int x, int y, int w, int h, Color c1, Color c2);

	// Text functions


	void Text(int x, int y, Color color, DWORD font, const char* text, ...);
	void Textf(int x, int y, Color color, DWORD font, const char* fmt, ...);
	void Text(int x, int y, Color color, DWORD font, const wchar_t* text);
	void Text(int x, int y, DWORD font, const wchar_t* text);
	RECT GetTextSize(DWORD font, const char* text);

	// Other rendering functions
	bool WorldToScreen(const Vector& vOrigin, Vector& vScreen);
	bool ScanColorFromCvar(const char* str, float* colors);
	RECT GetViewport();
    class Fonts
    {
    public:
		DWORD Default;
		DWORD ESP;
		DWORD ESPan;
		DWORD Defuse;
		DWORD DroppedGuns;
		DWORD Icon;
		DWORD LBY;
		DWORD Watermark;
		DWORD Guns;
		DWORD NameFont;
    } font;
	float* FindW2Matrix()
	{
		return (float*)(reinterpret_cast<DWORD>(&Interfaces::Engine()->WorldToScreenMatrix()) + 0x40);
	}

};

extern Render* g_Render;