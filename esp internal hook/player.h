#ifndef PLAYER_H
#define PLAYER_H

struct Vec2 {
	float x, y;
};
struct Vec3 {
	float x, y, z;
};
struct Vec4 {
	float x, y, z, w;
};

class pEntity
{
public:
	char pad_0000[236]; //0x0000
	int8_t N0000008A; //0x00EC
	bool bDormant; //0x00ED
	char pad_00EE[6]; //0x00EE
	int32_t iTeamNum; //0x00F4
	char pad_00F8[8]; //0x00F8
	int32_t iHealth; //0x0100
	char pad_0104[16]; //0x0104
	Vec3 vecVelocity; //0x0114
	char pad_0120[12]; //0x0120
	Vec3 vecViewOffset; //0x012C
	Vec3 vecOrigin; //0x0138
	char pad_0144[9572]; //0x0144
	int32_t boneMatrix; //0x26A8
	char pad_26AC[2432]; //0x26AC
	Vec3 aimPunchAngle; //0x302C
	char pad_3038[29528]; //0x3038
	uint32_t shotFired; //0xA390
	char pad_A394[4052]; //0xA394
	uint32_t hasHelmet; //0xB368
	char pad_B36C[12]; //0xB36C
	uint32_t armorValue; //0xB378
	char pad_B37C[16]; //0xB37C
	uint32_t hasDefuse; //0xB38C
	char pad_B390[1068]; //0xB390
}; //Size: 0xB7BC

#endif#pragma once
