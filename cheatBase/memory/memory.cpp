#include "memory.h"

void Memory::init()
{
	baseAddress = (uintptr_t)GetModuleHandle("ac_client.exe");
	playerBase = *(ent**)(baseAddress + 0x18AC00);
	entities = *(entList**)(baseAddress + 0x18AC04);
	playerCount = (int*)(baseAddress + 0x18AC0C);
	matrix = (Matrix*)(baseAddress + 0x17DFD0);
	intersectFunc = baseAddress + 0xCCA80;
}

entList* Memory::getEntListPtr()
{
	return entities;
}

int* Memory::getPlayerCount()
{
	return playerCount;
}

ent* Memory::getLocalPlayer()
{
	return playerBase;
}

weapon* Memory::getCurrentWeapon()
{
	return playerBase->cweapon;
}

Matrix* Memory::getMatrix()
{
	return matrix;
}

void Memory::AcIntersectGeometry(Vector3& from, Vector3& to) {
	DWORD fIntersectGeometry = intersectFunc;

	void* pFrom = (void*)&from;
	void* pTo = (void*)&to;

	__asm {
		mov ecx, pFrom
		mov edx, pTo
		call fIntersectGeometry
	}
}

