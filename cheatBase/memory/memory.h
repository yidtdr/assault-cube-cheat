#pragma once
#include <iostream>
#include <windows.h>
#include "../ac.h"

class Memory
{
public:
	Memory() {};
	void init();
	entList* getEntListPtr();
	int* getPlayerCount();
	ent* getLocalPlayer();
	weapon* getCurrentWeapon();
	Matrix* getMatrix();
	void AcIntersectGeometry(Vector3& from, Vector3& to);
	DWORD intersectFunc;
private:
	uintptr_t baseAddress;
	ent* playerBase;
	entList* entities;
	int* playerCount;
	Matrix* matrix;
};