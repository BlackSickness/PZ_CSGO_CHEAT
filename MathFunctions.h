


#pragma once

#include "Vector.hpp"
#include "MiscDefinitions.h"
#include "VMatrix.hpp"
#include <algorithm>
#include <stdint.h>
#define M_PI		3.14159265358979323846f
#define M_PI_F		((float)(M_PI))
#define RAD2DEG( x )  ( static_cast<float>(x) * (180.f / M_PI) )
#define DEG2RAD( x )  ( static_cast<float>(x) * (M_PI / 180.f) )
typedef void(*LPSEARCHFUNC)(LPCTSTR lpszFileName);

void SinCos(float radians, float *sine, float *cosine);
void ClampAngles(QAngle &angles);
void VectorAngles1(const Vector & forward, QAngle & angles);
void VectorAngles(const Vector& forward, QAngle &angles);

 void AngleVectors(const QAngle &angles, Vector *forward);
 Vector	AngleVector(QAngle meme);
void Normalize(Vector &vIn, Vector &vOut);
void AngleNormalize(QAngle& angle);
void CalcAngle(Vector src, Vector dst, Vector &angles);
void AverageDifference(const Vector& a, const Vector& b, float& result);
QAngle CalcAngle(Vector src, Vector dst);
void AngleVectors(const QAngle &angles, Vector *forward, Vector *right, Vector *up);
void SmoothAngles(Vector MyViewAngles, Vector AimAngles, Vector &OutAngles, float Smoothing);
bool LineGoesThroughSmoke(Vector vStartPos, Vector vEndPos);
float DistancePointToLine(Vector Point, Vector LineOrigin, Vector Dir);
bool sanitize_angles(QAngle & angles);

#define M_RADPI 57.295779513082f
inline void compute_angle(const Vector &source, const Vector &destination, QAngle& angles)
{
	Vector delta = source - destination;
	angles.x = static_cast< float >(asin(delta.z / delta.Length()) * M_RADPI);
	angles.y = static_cast< float >(atan(delta.y / delta.x) * M_RADPI);
	angles.z = 0.0f;

	if (delta.x >= 0.0f)
		angles.y += 180.0f;
}
inline QAngle compute_angle(const Vector &source, const Vector &destination)
{
	QAngle angles;

	Vector delta = source - destination;
	angles.x = static_cast< float >(asin(delta.z / delta.Length()) * M_RADPI);
	angles.y = static_cast< float >(atan(delta.y / delta.x) * M_RADPI);
	angles.z = 0.0f;

	if (delta.x >= 0.0f)
		angles.y += 180.0f;

	return angles;
}
inline float get_distance(const Vector &start, const Vector &end)
{
	float distance = sqrt((start - end).Length());

	if (distance < 1.0f)
		distance = 1.0f;

	return distance;
}


inline BOOL SearchFiles(LPCTSTR lpszFileName, LPSEARCHFUNC lpSearchFunc, BOOL bInnerFolders = FALSE)
{
	LPTSTR part;
	char tmp[MAX_PATH];
	char name[MAX_PATH];

	HANDLE hSearch = NULL;
	WIN32_FIND_DATA wfd;
	memset(&wfd, 0, sizeof(WIN32_FIND_DATA));

	if (bInnerFolders)
	{
		if (GetFullPathName(lpszFileName, MAX_PATH, tmp, &part) == 0) return FALSE;
		strcpy(name, part);
		strcpy(part, "*.*");
		wfd.dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;
		if (!((hSearch = FindFirstFile(tmp, &wfd)) == INVALID_HANDLE_VALUE))
			do
			{
				if (!strncmp(wfd.cFileName, ".", 1) || !strncmp(wfd.cFileName, "..", 2))
					continue;

				if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					char next[MAX_PATH];
					if (GetFullPathName(lpszFileName, MAX_PATH, next, &part) == 0) return FALSE;
					strcpy(part, wfd.cFileName);
					strcat(next, "\\");
					strcat(next, name);

					SearchFiles(next, lpSearchFunc, TRUE);
				}
			} while (FindNextFile(hSearch, &wfd));
			FindClose(hSearch);
	}

	if ((hSearch = FindFirstFile(lpszFileName, &wfd)) == INVALID_HANDLE_VALUE)
		return TRUE;
	do
		if (!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			char file[MAX_PATH];
			if (GetFullPathName(lpszFileName, MAX_PATH, file, &part) == 0) return FALSE;
			strcpy(part, wfd.cFileName);

			lpSearchFunc(wfd.cFileName);
		}
	while (FindNextFile(hSearch, &wfd));
	FindClose(hSearch);
	return TRUE;
}


class CTimer
{
public:
	ULONGLONG dwTime;
	bool bEnable;
	CTimer()
	{
		dwTime = 0;
		bEnable = true;
	}
	bool delay(DWORD dwMsec);
	void reset();
	void stop();
};

