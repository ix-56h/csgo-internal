#include "utils.h"

extern IClient eClient;
extern IEngine eEngine;

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

    screen.x = (eEngine.width / 2 * NDC.x) + (NDC.x + eEngine.width / 2);
    screen.y = -(eEngine.height / 2 * NDC.y) + (NDC.y + eEngine.height / 2);
    return true;
}

Vec3 calcAngle(Vec3 src, Vec3 dst)
{
    Vec3 delta = dst - src;
    float hyp = delta.Length();
    float pitch = -asinf(delta.z / hyp) * (180 / PI);
    float yaw = atan2f(delta.y, delta.x) * (180 / PI);
    delta.z = 0;

    if (pitch >= -89 && pitch <= 89 && yaw >= -180 && yaw <= 180)
    {
        delta.x = pitch;
        delta.y = yaw;
    }
    return (delta);
}

float getDistance(Vec3 src, Vec3 dst)
{
    Vec3 delta = dst - src;

    return (delta.Length());
}

bool betterTarget(Vec3 src, pEntity* entity, float* curClosestDistance, Vec3* curClosestAngle)
{
    //here we can check viewangle, distance, if walls etc
    //actually just check de distance between the Current closest enemy and another enemy


    //Vec3 screenPos = WorldToScreen(src, pClientState + hazedumper::signatures::dwViewMatrix);
    Vec3 newAngle = calcAngle(src, entity->vecOrigin);
    Vec3 diff = newAngle - *eEngine.viewAngle;
    diff.Normalize();
    float dist = getDistance(src, entity->vecOrigin);
    if (((diff.y <= curClosestAngle->y && diff.y >= -(curClosestAngle->y))
        && (diff.x <= curClosestAngle->x && diff.x >= -(curClosestAngle->x)))
        || (dist < *curClosestDistance && (diff.y <= 7 && diff.y >= -7)
            && (diff.x <= 7 && diff.x >= -7))
        )
    {
        //if (dist < *curClosestDistance)
        //{
        *curClosestAngle = diff;
        *curClosestDistance = dist;
        return (true);
        //}
    }
    return false;

    //REALLY NEED TO REFACTOR, can't access to clientstate width etc without global
    // this is not possible, tomorow just refacto and select target if closest to cursor with world2screen

    //return (getDistance(src, entity->vecOrigin) < curClosestDistance);
}

pEntity* closestOne = nullptr;

pEntity* getClosestEntity()
{
    uint32_t i = 0;
    pEntity* closestPtr = nullptr;
    float closestDistance = 500;
    Vec3 closestAngle = { 15, 15, 0 };
    for (size_t i = 0; i < eClient.maxPlayers; i++)
        //for (size_t i = 1; i < 4; i++)
    {
        pEntity* entity = *reinterpret_cast<pEntity**>(eClient.pEntityList + (i * 0x10));
        if (!entity || eClient.localPlayer->iTeamNum == entity->iTeamNum || entity->isDormant || closestOne == entity)
            continue;
        if ((uintptr_t)entity == (uintptr_t)(eClient.localPlayer))
            continue;
        if (eClient.localPlayer->iHealth < 1 || entity->iHealth < 1)
            continue;
        if (betterTarget(eClient.localPlayer->vecOrigin, entity, &closestDistance, &closestAngle))
            closestPtr = entity;
    }
    return (closestPtr);

}
/*
pEntity* getClosestEntity()
{
    uint32_t i = 0;
    pEntity* closestPtr = nullptr;
    float closestDistance = 500;
    Vec3 closestAngle = { 15, 15, 0 };
    for (size_t i = 1; i < eClient.maxPlayers; i++)
        //for (size_t i = 1; i < 4; i++)
    {
        pEntity* entity = *reinterpret_cast<pEntity**>(eClient.pEntityList + (i * 0x10));
        if (!entity || eClient.localPlayer->iTeamNum == entity->iTeamNum || entity->isDormant)
            continue;
        //if ((uintptr_t)entity == (uintptr_t)(eClient.localPlayer))
        //    continue;
        if (eClient.localPlayer->iHealth < 1 || entity->iHealth < 1)
            continue;
        if (betterTarget(eClient.localPlayer->vecOrigin, entity, &closestDistance, &closestAngle))
            closestPtr = entity;
    }
    return (closestPtr);
}*/

DWORD WINAPI aimbotTH(void* smooth)
{
    while (1)
    {
        eClient.aimSmooth = *(int*)smooth;
        eClient.localPlayer->AimAt(getClosestEntity());
    }
}

pEntity* getClosestEntityFOV()
{
    int aimFOV = 10;
    float lowestHypot = 999;
    pEntity* bestTarget = nullptr;

    int midX = eEngine.width / 2;
    int midY = eEngine.height / 2;
    float multX = midX / 90;
    float size = (aimFOV * multX);

    for (size_t i = 0; i < eClient.maxPlayers; i++)
    {
        pEntity* curTarget = *reinterpret_cast<pEntity**>(eClient.pEntityList + (i * 0x10));

        if (!curTarget || curTarget == nullptr)
            continue;

        if (curTarget->iTeamNum == eClient.localPlayer->iTeamNum || curTarget->iHealth < 1 || curTarget->isDormant)
            continue;

        Vec3 entPos = GetBonePos(curTarget, 8);
        Vec2 screenPos;
        if (WorldToScreen(entPos, screenPos))
        {

            float hypotenuse = sqrtf((midX - screenPos.x) * (midX - screenPos.x) + (midY - screenPos.y) * (midY - screenPos.y));
            if (hypotenuse < size && hypotenuse < lowestHypot)
            {
                lowestHypot = hypotenuse;
                bestTarget = curTarget;
            }
        }
    }
    return bestTarget;
}

DWORD WINAPI aimbotFOV(void* smooth)
{
    while (1)
    {
        eClient.aimSmooth = *(int*)smooth;
        eClient.localPlayer->AimAt(getClosestEntityFOV());
    }
}