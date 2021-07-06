#include "draw.h"

extern LPDIRECT3DDEVICE9 gDevice;

void DrawLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color) {
    ID3DXLine* LineL;
    D3DXCreateLine(gDevice, &LineL);

    D3DXVECTOR2 Line[2];
    Line[0] = D3DXVECTOR2(x1, y1);
    Line[1] = D3DXVECTOR2(x2, y2);
    LineL->SetWidth(thickness);
    LineL->Draw(Line, 2, color);
    LineL->Release();
}

void DrawLineVec(Vec2 src, Vec2 dst, int thickness, D3DCOLOR color) {
	DrawLine(src.x, src.y, dst.x, dst.y, thickness, color);
}

void DrawEspBox2D(Vec2 top, Vec2 bot, int thickness, D3DCOLOR color) {
	int height = ABS(top.y - bot.y);
	Vec2 tl, tr;
	tl.x = top.x - height / 4;
	tr.x = top.x + height / 4;
	tl.y = tr.y = top.y;

	Vec2 bl, br;
	bl.x = bot.x - height / 4;
	br.x = bot.x + height / 4;
	bl.y = br.y = bot.y;

	DrawLineVec(tl, tr, thickness, color);
	DrawLineVec(bl, br, thickness, color);
	DrawLineVec(tl, bl, thickness, color);
	DrawLineVec(tr, br, thickness, color);
}
#include <stdio.h>
void DrawHealthArmor(Vec2 Foot2d, Vec2 Head2d, int thickness, pEntity* entity)
{
    int height = ABS(Foot2d.y - Head2d.y);
    int dX = (Foot2d.x - Head2d.x);

    float healthPerc = entity->iHealth / 100.f;
    float armorPerc = entity->ArmorValue / 100.f;
    int  r, g = 0;
    r = (1 - healthPerc) * 255;
    g = (healthPerc * 255);

    Vec2 botHealth, topHealth, botArmor, topArmor;
    int healthHeight = height * healthPerc;

    botHealth.y = botArmor.y = Foot2d.y;
    botHealth.x = Foot2d.x - (height / 4) - 2;
    topHealth.y = Head2d.y + height - healthHeight;
    topHealth.x = Foot2d.x - (height / 4) - 2 - (dX * healthPerc);
    
    DrawLineVec(botHealth, topHealth, thickness, D3DCOLOR_ARGB(255, r, g, 0));
    if (entity->ArmorValue > 1)
    {
        float armorPerc = entity->ArmorValue / 100.f;
        int armorHeight = height * armorPerc;
        botArmor.x = Foot2d.x + (height / 4) + 2;
        topArmor.y = Head2d.y + height - armorHeight;
        topArmor.x = Foot2d.x + (height / 4) + 2 - (dX * armorPerc);

        DrawLineVec(botArmor, topArmor, thickness, D3DCOLOR_ARGB(255, 30, 144, 255));
    }
}
/*
*/