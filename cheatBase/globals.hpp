#pragma once
#include "memory/memory.h"
#include "minhook/include/MinHook.h"


Memory mem = Memory();

namespace settings
{
	namespace menu
	{
		bool bShow = false;
	}

	namespace aimbot
	{
		const char* workTypeString[2] = { "ANGLE", "DISTANCE" };

		bool bEnabled = false;
		float maxFov = 50;
		bool bOnlyVisible = false;
		bool bIgnoreTeam = false;
		int aimType = 0;
		bool bAutoFire = 0;

		int selfPredict = 0;
		int targetPredict = 0;
	};

	namespace esp
	{
		bool bEnabled = false;
		bool bDrawTeam = false;

		bool bDrawHead = false;
		bool bDrawName = false;
	};

	namespace misc
	{
		bool bNoGunWait = false;
		bool bNoRecoil = false;
		bool bInfiniteAmmo = false;
		bool bInfiniteHealth = false;
	}

	namespace movement
	{
		bool bNoAcceleration = false;
		float velHack = 1;
		bool bBhopEnabled = false;
		bool bBrakeAnims = false;
	}
};