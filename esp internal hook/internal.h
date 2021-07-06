#pragma once
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

#define IN_ATTACK                    (1 << 0)
#define IN_JUMP                        (1 << 1)
#define IN_DUCK                        (1 << 2)
#define IN_FORWARD                    (1 << 3)
#define IN_BACK                        (1 << 4)
#define IN_USE                        (1 << 5)
#define IN_CANCEL                    (1 << 6)
#define IN_LEFT                        (1 << 7)
#define IN_RIGHT                    (1 << 8)
#define IN_MOVELEFT                    (1 << 9)
#define IN_MOVERIGHT                (1 << 10)
#define IN_ATTACK2                    (1 << 11)
#define IN_RUN                        (1 << 12)
#define IN_RELOAD                    (1 << 13)
#define IN_ALT1                        (1 << 14)
#define IN_ALT2                        (1 << 15)
#define IN_SCORE                    (1 << 16)
#define IN_SPEED                    (1 << 17)
#define IN_WALK                        (1 << 18)
#define IN_ZOOM                        (1 << 19)
#define IN_WEAPON1                    (1 << 20)
#define IN_WEAPON2                    (1 << 21)
#define IN_BULLRUSH                    (1 << 22)

//includes
#include <d3d9.h>
#include <d3dx9.h>

#include <inttypes.h>

#include "vector.h"
#include "player.h"
#include "utils.h"
#include "ClientEngine.h"
#include "draw.h"
#include "offsets.h"

#define PI 3.1415927f
#define ABS(x) ((x < 0) ? (-x) : (x))

struct view_matrix_t {
	float matrix[16];
};
