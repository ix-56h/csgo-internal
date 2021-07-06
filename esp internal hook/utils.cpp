#include "utils.h"

extern IClient eClient;
extern size_t windowHeight, windowWidth;

uintptr_t GetModuleBaseAddress(const wchar_t* moduleName) {
    return (uintptr_t)GetModuleHandleW(moduleName);
}

Vec3 GetBonePos(pEntity* ent, int bone) {
	uintptr_t bonePtr = ent->boneMatrix;
	Vec3 bonePos;
	bonePos.x = *(float*)(bonePtr + 0x30 * bone + 0x0C);
	bonePos.y = *(float*)(bonePtr + 0x30 * bone + 0x1C);
	bonePos.z = *(float*)(bonePtr + 0x30 * bone + 0x2C);
	return bonePos;
}

bool WorldToScreen(Vec3 pos, Vec2& screen) {
    Vec4 clipCoords;
    clipCoords.x = pos.x * eClient.viewMatrix[0] + pos.y * eClient.viewMatrix[1] + pos.z * eClient.viewMatrix[2] + eClient.viewMatrix[3];
    clipCoords.y = pos.x * eClient.viewMatrix[4] + pos.y * eClient.viewMatrix[5] + pos.z * eClient.viewMatrix[6] + eClient.viewMatrix[7];
    clipCoords.z = pos.x * eClient.viewMatrix[8] + pos.y * eClient.viewMatrix[9] + pos.z * eClient.viewMatrix[10] + eClient.viewMatrix[11];
    clipCoords.w = pos.x * eClient.viewMatrix[12] + pos.y * eClient.viewMatrix[13] + pos.z * eClient.viewMatrix[14] + eClient.viewMatrix[15];

    if (clipCoords.w < 0.1f)
        return false;

    Vec3 NDC;
    NDC.x = clipCoords.x / clipCoords.w;
    NDC.y = clipCoords.y / clipCoords.w;
    NDC.z = clipCoords.z / clipCoords.w;

    screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
    screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
    return true;
}