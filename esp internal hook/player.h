#pragma once
#include "offsets.h"

#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}

class pEntity {
public:
	union {
		DEFINE_MEMBER_N(bool, isDormant, ofs::st::m_bDormant);
		DEFINE_MEMBER_N(uint32_t, iTeamNum, ofs::nv::m_iTeamNum);
		DEFINE_MEMBER_N(uint32_t, iHealth, ofs::nv::m_iHealth);
		DEFINE_MEMBER_N(uint32_t, fFlags, ofs::nv::m_fFlags);
		DEFINE_MEMBER_N(Vec3, vecVelocity, ofs::nv::m_vecVelocity);
		DEFINE_MEMBER_N(Vec3, vecViewOffset, ofs::nv::m_vecViewOffset);
		DEFINE_MEMBER_N(Vec3, vecOrigin, ofs::nv::m_vecOrigin);
		DEFINE_MEMBER_N(uint32_t, boneMatrix, ofs::nv::m_dwBoneMatrix);
		DEFINE_MEMBER_N(Vec3, aimPunchAngle, ofs::nv::m_aimPunchAngle);
		DEFINE_MEMBER_N(uint32_t, shotFired, ofs::nv::m_iShotsFired);
		DEFINE_MEMBER_N(uint32_t, hasHelmet, ofs::nv::m_bHasHelmet);
		DEFINE_MEMBER_N(uint32_t, ArmorValue, ofs::nv::m_ArmorValue);
		DEFINE_MEMBER_N(uint32_t, hasDefuse, ofs::nv::m_bHasDefuser);
	};
	void AimAt(pEntity* target);
};