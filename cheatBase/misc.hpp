#include "globals.hpp"
#include "ac.h"

namespace Misc
{
	void cycle()
	{
		if (settings::misc::bNoGunWait)
		{
			*mem.getLocalPlayer()->curweapon->gunWait = 0;
		}
		if (settings::misc::bNoRecoil)
		{
			mem.getLocalPlayer()->viewVel = Vector2{ 0, 0};
		}
		if (settings::misc::bInfiniteAmmo)
		{
			*mem.getLocalPlayer()->curweapon->ammo = 1000;
		}
		if (settings::misc::bInfiniteHealth)
		{
			mem.getLocalPlayer()->hp = 1000;
		}
	}
}