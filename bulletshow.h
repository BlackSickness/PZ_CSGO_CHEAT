#include "Render.h"
#include "SDK.h"
#include <vector>


class cbullet_tracer_info
{
public:
	cbullet_tracer_info(Vector src, Vector dst, float time, Color color)
	{
		this->src = src;
		this->dst = dst;
		this->time = time;
		this->color = color;

	}
	Vector src, dst;
	float time;
	Color color;
	C_BaseEntity*	pPlayer = nullptr;
};


