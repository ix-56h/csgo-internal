#ifndef INTERNAL_H
#define INTERNAL_H
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

//includes
#include <d3d9.h>
#include <d3dx9.h>

#include <inttypes.h>

#include "player.h"
#include "utils.h"
#include "ClientEngine.h"
#include "draw.h"

#include "offsets.h"

#define ABS(x) ((x < 0) ? (-x) : (x))

struct view_matrix_t {
	float matrix[16];
};
#endif
