#pragma once
#include "internal.h"

void DrawLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color);
void DrawLineVec(Vec2 src, Vec2 dst, int thickness, D3DCOLOR color);
void DrawEspBox2D(Vec2 top, Vec2 bot, int thickness, D3DCOLOR color);