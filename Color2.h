#pragma once
#include <cstdint>
class Color2
{
public:
	Color2();
	Color2(int _r, int _g, int _b);
	Color2(int _r, int _g, int _b, int _a);
	Color2(float _r, float _g, float _b) : Color2(_r, _g, _b, 1.0f) {}
	Color2(float _r, float _g, float _b, float _a)
		: Color2(
			static_cast<int>(_r * 255.0f),
			static_cast<int>(_g * 255.0f),
			static_cast<int>(_b * 255.0f),
			static_cast<int>(_a * 255.0f))
	{
	}
	explicit Color2(float* rgb) : Color2(rgb[0], rgb[1], rgb[2], 1.0f) {}
	explicit Color2(unsigned long argb)
	{
		_CColor[2] = (unsigned char)((argb & 0x000000FF) >> (0 * 8));
		_CColor[1] = (unsigned char)((argb & 0x0000FF00) >> (1 * 8));
		_CColor[0] = (unsigned char)((argb & 0x00FF0000) >> (2 * 8));
		_CColor[3] = (unsigned char)((argb & 0xFF000000) >> (3 * 8));
	}

	void    SetRawColor(int color32);
	int     GetRawColor() const;
	void    SetColor(int _r, int _g, int _b, int _a = 0);
	void    SetColor(float _r, float _g, float _b, float _a = 0);
	void	SetColor(float* color);
	void    GetColor(int &_r, int &_g, int &_b, int &_a) const;

	int r() const { return _CColor[0]; }
	int g() const { return _CColor[1]; }
	int b() const { return _CColor[2]; }
	int a() const { return _CColor[3]; }

	void SetAlpha(int alpha) { _CColor[3] = alpha; }

	unsigned char &operator[](int index)
	{
		return _CColor[index];
	}
	const unsigned char &operator[](int index) const
	{
		return _CColor[index];
	}

	bool operator==(const Color2 &rhs) const;
	bool operator!=(const Color2 &rhs) const;
	Color2 &operator=(const Color2 &rhs);

	static Color2 Black;
	static Color2 White;
	static Color2 Red;
	static Color2 Green;
	static Color2 Blue;


	unsigned char _CColor[4];
};