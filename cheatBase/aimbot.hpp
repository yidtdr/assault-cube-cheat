#include "globals.hpp"
#include <string>
#include <math.h>

namespace AimBot
{
    int framesTillFire = -1;
    ent* lastTarget = nullptr;

    Vector3 calculateDirection(float yaw, float pitch)
    {
        // Convert player's view angles to a direction vector
        yaw = yaw * 0.0174533;
        pitch = pitch * 0.0174533;

        Vector3 direction;
        direction.x = cosf(pitch) * cosf(yaw);
        direction.y = cosf(pitch) * sinf(yaw);
        direction.z = sinf(pitch);

        return direction;
    }

    bool isTargetVisible(ent* target)
    {
        ent fakeLP = ent();
        fakeLP = *mem.getLocalPlayer();
        Vector3 tmpTo = target->head;
        Vector3 from = Vector3(fakeLP.head.x, fakeLP.head.y, fakeLP.head.z);
        Vector3 to = Vector3(tmpTo.x, tmpTo.y, tmpTo.z);

        mem.AcIntersectGeometry(from, to);
        
        return (to.x == tmpTo.x && to.y == tmpTo.y && to.z == tmpTo.z);
    }

    void SetViewAngles(Vector2 newAngles);
    ent* FindClosestEnemy(entList* entities, Vector3 playerPos, int entCount);
    ent* FindNearestEnemy(entList* entities, Vector3 playerPos, int entCount);
    inline Vector3 predictHeadPos(ent* target);


    void cycle() {
        if (settings::aimbot::bEnabled)
        {
            int* playerCount = mem.getPlayerCount();
            entList* entities = mem.getEntListPtr();
            ent* localPlayer = mem.getLocalPlayer();
            Vector3 playerPos = localPlayer->head + (localPlayer->vel * settings::aimbot::selfPredict / 1000);
            Vector2 viewAngles = localPlayer->viewAngle;

            if (settings::aimbot::bAutoFire)
            {
                localPlayer->attacking = false;
            }

            if (settings::aimbot::aimType == 0)
            {
                ent* target = FindNearestEnemy(entities, playerPos, *playerCount);

                if (target != NULL)
                {
                    Vector2 aimAngles = CalcAngle(playerPos, predictHeadPos(target)  /* target->head + (target->vel * settings::aimbot::targetPredict / 1000)*/);

                    if (settings::aimbot::bAutoFire)
                    {
                        if (v2Dist(aimAngles, localPlayer->viewAngle) < 0.3f)
                        {
                            localPlayer->attacking = true;
                        }
                    }

                    SetViewAngles(aimAngles);

                }
            }
            else
            {
                ent* target = FindClosestEnemy(entities, playerPos, *playerCount);

                if (target != NULL)
                {
                    Vector2 aimAngles = CalcAngle(playerPos, predictHeadPos(target) /*target->head + (target->vel * settings::aimbot::targetPredict / 1000)*/);

                    if (settings::aimbot::bAutoFire)
                    {
                        if (v2Dist(aimAngles, localPlayer->viewAngle) < 0.3f)
                        {
                            localPlayer->attacking = true;
                        }
                    }

                    SetViewAngles(aimAngles);
                }
            }
        }
    }

    bool isEntityTeammate(ent* target, ent* localplayer)
    {
        return (localplayer->team == target->team);
    }

    bool isEntityVisible(ent* target)
    {
        return ((target->posIfSeen.x != target->posIfSeen.y) && (target->posIfSeen.y != target->posIfSeen.z));
    }

    bool isEntityAlive(ent* target)
    {
        return ((target->hp > 0) && (target->hp <= 100));
    }

    bool checkEntity(ent* target, ent* localplayer)
    {
        if (target != NULL)
        {
            if (!isEntityAlive(target)) return false;
            if (settings::aimbot::bIgnoreTeam && isEntityTeammate(target, localplayer)) return false;
            if (settings::aimbot::bOnlyVisible && !isTargetVisible(target)) return false;
            if (settings::aimbot::maxFov < v2Dist(CalcAngle(localplayer->head, target->head), localplayer->viewAngle)) return false;
            return true;
        }
        return false;
    }

	ent* FindNearestEnemy(entList* entities, Vector3 playerPos, int entCount) {
		ent* nearest = nullptr;
		float nearestAngle = FLT_MAX;
        ent* localPlayer = mem.getLocalPlayer();

		for (int i = 0; i < entCount; i++) {
			ent* player = entities->ents[i];
			if (checkEntity(player, localPlayer)) {
				float deltaAngle = v2Dist(CalcAngle(localPlayer->head, player->head), localPlayer->viewAngle);
				if (deltaAngle < nearestAngle) {
                    nearestAngle = deltaAngle;
					nearest = player;
				}
			}
		}

		return nearest;
	}

    ent* FindClosestEnemy(entList* entities, Vector3 playerPos, int entCount) {
        ent* nearest = nullptr;
        float nearestDist = FLT_MAX;
        ent* localPlayer = mem.getLocalPlayer();


        for (int i = 0; i < entCount; i++) {
            ent* player = entities->ents[i];
            if (checkEntity(player, localPlayer)) {
                float dist = (player->pos - playerPos).Length();
                if (dist < nearestDist) {
                    nearestDist = dist;
                    nearest = player;
                }
            }
        }

        return nearest;
    }

    void SetViewAngles(Vector2 newAngles) {
        ent* localPlayer = mem.getLocalPlayer();
        localPlayer->viewAngle.x = newAngles.x;
        localPlayer->viewAngle.y = newAngles.y;
    }


    Vector3 predictHeadPos(ent* target) {
        float rotation = target->viewAngle.x;
        float dx = -sin(rotation * 0.0174533);
        float dy = cos(rotation * 0.0174533);

        return (target->head + Vector3(-dx * 0.3 + target->vel.x * settings::aimbot::targetPredict / 1000.0f, -dy * 0.3 + target->vel.y * settings::aimbot::targetPredict / 1000.0f, 0.2f));
    }
}