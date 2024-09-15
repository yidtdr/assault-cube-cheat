#include "globals.hpp"
#include "ac.h"

namespace Movement
{
	Vector3 GetDirection(float rotation) {
		float dx = -sin(rotation * 0.0174533);
		float dy = cos(rotation * 0.0174533);

		return Vector3(-dx, -dy, 0.0f);
	}

	void cycle()
	{
		ent* lp = mem.getLocalPlayer();

		if (settings::movement::bNoAcceleration)
		{
			bool strafeLeft = (lp->strafe & 0x1);
			bool strafeRight = (lp->strafe & 0x100);
			bool strafeForward = (lp->strafe & 0x10000);
			bool strafeBackwards = (lp->strafe & 0x1000000);

			Vector3 forwComp = GetDirection(lp->viewAngle.x) * strafeForward;
			Vector3 backComp = GetDirection(lp->viewAngle.x + 180) * strafeBackwards;
			Vector3 leftComp = GetDirection(lp->viewAngle.x - 90) * strafeLeft;
			Vector3 rightComp = GetDirection(lp->viewAngle.x + 90) * strafeRight;

			Vector3 newVel = forwComp + backComp + leftComp + rightComp;
			newVel.z = lp->vel.z;

			lp->vel = newVel;
			lp->vel.x *= settings::movement::velHack;
			lp->vel.y *= settings::movement::velHack;
		}

		if (settings::movement::bBhopEnabled)
		{
			if (GetAsyncKeyState(VK_SPACE))
			{
				lp->jumpnext = 1;
			}
		}

		bool crouchOff = false;

		if (settings::movement::bBrakeAnims)
		{
			if (!lp->onGround)
			{
				lp->crouch = true;
				lp->crouchTry = true;
				lp->crouchAir = true;
				crouchOff = true;
			} else if (crouchOff) {
				lp->crouch = false;
				lp->crouchTry = false;
				lp->crouchAir = false;
				crouchOff = false;
			}
		}
	}
}