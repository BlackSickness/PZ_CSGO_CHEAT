#include "Color2.h"

Color2 Color2::Black(0, 0, 0, 255);
Color2 Color2::White(255, 255, 255, 255);
Color2 Color2::Red(255, 0, 0, 255);
Color2 Color2::Green(70, 255, 70, 255);
Color2 Color2::Blue(0, 0, 255, 255);

Color2::Color2()
{
	*((int *)this) = 0;
}
Color2::Color2(int _r, int _g, int _b)
{
	SetColor(_r, _g, _b, 255);
}
Color2::Color2(int _r, int _g, int _b, int _a)
{
	SetColor(_r, _g, _b, _a);
}
void Color2::SetRawColor(int color32)
{
	*((int *)this) = color32;
}
int Color2::GetRawColor() const
{
	return *((int *)this);
}
__inline void Color2::SetColor(int _r, int _g, int _b, int _a)
{
	_CColor[0] = (unsigned char)_r;
	_CColor[1] = (unsigned char)_g;
	_CColor[2] = (unsigned char)_b;
	_CColor[3] = (unsigned char)_a;
}
__inline void Color2::SetColor(float _r, float _g, float _b, float _a)
{
	_CColor[0] = static_cast<unsigned char>(_r * 255.0f);
	_CColor[1] = static_cast<unsigned char>(_g * 255.0f);
	_CColor[2] = static_cast<unsigned char>(_b * 255.0f);
	_CColor[3] = static_cast<unsigned char>(_a * 255.0f);
}
void Color2::SetColor(float* color)
{
	if (!color)
		return;

	_CColor[0] = (unsigned char)(color[0] * 255.f);
	_CColor[1] = (unsigned char)(color[1] * 255.f);
	_CColor[2] = (unsigned char)(color[2] * 255.f);
	_CColor[3] = (unsigned char)(color[3] * 255.f);
}
void Color2::GetColor(int &_r, int &_g, int &_b, int &_a) const
{
	_r = _CColor[0];
	_g = _CColor[1];
	_b = _CColor[2];
	_a = _CColor[3];
}
bool Color2::operator== (const Color2 &rhs) const
{
	return (*((int *)this) == *((int *)&rhs));
}
bool Color2::operator!= (const Color2 &rhs) const
{
	return !(operator==(rhs));
}
Color2& Color2::operator=(const Color2 &rhs)
{
	SetRawColor(rhs.GetRawColor());
	return *this;
}
