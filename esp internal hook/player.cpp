#include "internal.h"
extern IEngine eEngine;
extern IClient eClient;

void pEntity::AimAt(pEntity *target)
{
    if (target != nullptr)
    {
        Vec3 dst = GetBonePos(target, 8);

        Vec3 vecOrigin = eClient.localPlayer->vecOrigin;
        Vec3 viewOffset = eClient.localPlayer->vecViewOffset;

        // Need to get the eye position
        if (eClient.localPlayer->fFlags & IN_DUCK)
            vecOrigin.z += 44.f;
        else
            vecOrigin.z += 64.f;

        Vec3 newAngle = calcAngle(vecOrigin, dst);
        Vec3 diff = newAngle - *eEngine.viewAngle;
        diff.Normalize();
        eEngine.viewAngle->x += diff.x / eClient.aimSmooth;
        eEngine.viewAngle->y += diff.y / eClient.aimSmooth;
    }
}