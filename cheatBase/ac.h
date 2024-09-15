#pragma once
#include "Vectors/Vectors.h"

class ent;
class weapon;

struct acVec
{
	union
	{
		struct { float x, y, z; };
		float v[3];
		int i[3];
	};
};

inline acVec vecToAc(Vector3 v)
{
	acVec t = acVec{};
	t.i[0] = 0;
	t.i[1] = 0;
	t.i[2] = 0;


	t.v[0] = 0.0f;
	t.v[1] = 0.0f;
	t.v[2] = 0.0f;

	t.x = v.x;
	t.y = v.y;
	t.z = v.z;

	return t;
}

class weapon
{
public:
	void* N000001AF; //0x0000
	int32_t weaponId; //0x0004
	ent* pOwner; //0x0008
	void* N000001B2; //0x000C
	int* ammoReserv; //0x0010
	int* ammo; //0x0014
	int* gunWait; //0x0018
	int32_t shotsInRow; //0x001C
	bool bReload; //0x0020
};

class ent
{
public:
	char pad_0000[4]; //0x0000
	Vector3 head; //0x0004
	Vector3 vel; //0x0010
	Vector3 vel_2; //0x001C
	Vector3 pos; //0x0028
	Vector2 viewAngle; //0x0034
	Vector2 viewVel; //0x003C
	char pad_0044[4]; //0x0044
	int32_t fallUnits; //0x0048
	Vector3 unknown; //0x004C
	char pad_0058[5]; //0x0058
	bool onGround; //0x005D
	char pad_005E[1]; //0x005E
	bool jumpnext; //0x005F
	bool jumpdone; //0x0060
	bool crouch; //0x0061
	bool crouchAir; //0x0062
	bool crouchTry; //0x0063
	char pad_0064[2]; //0x0064
	bool scoping; //0x0066
	char pad_0067[25]; //0x0067
	int32_t strafe; //0x0080
	char pad_0084[104]; //0x0084
	int32_t hp; //0x00EC
	char pad_00F0[276]; //0x00F0
	bool attacking;
	char name[16]; //0x0205
	char pad_0215[247]; //0x0215
	int8_t team; //0x030C
	char pad_030D[87]; //0x030D
	weapon* curweapon; //0x0364
	weapon* cweapon; //0x0368
	char pad_036C[140]; //0x036C
	Vector3 posIfSeen; //0x03F8
	char pad_037C[500]; //0x036
};

struct entList {
	ent* ents[31];
};