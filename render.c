#include "share.h"


int keyButtons[4] = {KEY_D, KEY_F, KEY_J, KEY_K};
Texture2D background;

//on windows you will need a 64 bit compiler

bool initiate_render ()
{
    InitWindow(screenx, screeny, "osuc");
    InitAudioDevice();
    //SetTargetFPS(60);
    BeginDrawing();

	return true;
}

void update_key ()
{
		float time = GetTime() * 1000.0f;
		for(int i = 0; i < amountKeys; i++)
		{
        	if(IsKeyPressed(keyButtons[i])) keys[i] = time;
		}
    /*
        keys[1] = IsKeyDown(KEY_F);

        keys[2] = IsKeyDown(KEY_J);

        keys[3] = IsKeyDown(KEY_K);

        keys[4] = IsKeyDown(KEY_SPACE);
        keys[5] = IsKeyDown(KEY_LEFT_SHIFT);
        keys[6] = IsKeyDown(KEY_ESCAPE);
        */
}

void drawBackground ()
{
	draw_texture( vec_create(-0.4, -1), vec_create(1, 1), background);
}


bool frame_update ()
{
    //glFlush();
    int new_time = GetTime() * 1000.0;
    if(!paused)delta = (float)(new_time-millitotal) / 1000.0f;
    millitotal = new_time;
    if(!paused) millitimer += millitotal - new_time;
    //DrawRectangle(0, 0, 500, 500, (Color){.r=0,.g=0,.b=0,.a=255});
    DrawFPS(5,5);
	//printf("end Drawing\n");
    EndDrawing();
    BeginDrawing();
    ClearBackground(BLACK);
	//drawBackground();
	//printf("\nBegin Drawing");
    update_key();

    return !WindowShouldClose();
}

void draw_rectangle (vec pos1, vec pos2, vec4 color)
{
	//printf(" %f %f, %f %f", pos1.x, pos1.y, pos2.x, pos2.y);
    pos1 = vec_multi(vec_addf(vec_devf(pos1,2),0.5f), (vec){.x=screenx,.y=screeny});
    pos2 = vec_multi(vec_addf(vec_devf(pos2,2),0.5f), (vec){.x=screenx,.y=screeny});
	if(pos2.y < pos1.y)
	{
		float tmp = pos2.y;
		pos2.y = pos1.y;
		pos1.y = tmp;
	}
	//printf("    %f %f, %f %f", pos1.x, pos1.y, pos2.x, pos2.y);
    DrawRectangle(pos1.x, pos1.y, pos2.x - pos1.x, pos2.y - pos1.y, (Color){.r=color.r*255,.g=color.g*255,.b=color.b*255, .a=color.a*255});
    //printf("begin drawing\n");
    //DrawRectangle(0, 0, 500, 500, (Color){.r=1,.g=1,.b=1,.a=255});
}

void draw_texture (vec pos1, vec pos2, Texture2D tex)
{
	//printf("\njust a hard days work!");
	pos1 = vec_multi(vec_addf(vec_devf(pos1,2),0.5f), (vec){.x=screenx,.y=screeny});
    pos2 = vec_multi(vec_addf(vec_devf(pos2,2),0.5f), (vec){.x=screenx,.y=screeny});
	if(pos2.y < pos1.y)
	{
		float tmp = pos2.y;
		pos2.y = pos1.y;
		pos1.y = tmp;
	}	
	DrawTextureEx(tex, (Vector2){.x=pos1.x, .y=pos1.y}, 0, (pos2.x - pos1.x)/tex.width, WHITE);
}

void draw_text (vec pos, float size, vec4 color, const char * text)
{
	pos = vec_multi(vec_addf(vec_devf(pos,2),0.5f), (vec){.x=screenx,.y=screeny});
	DrawText(text, pos.x, pos.y, size, (Color){.r=color.r*255,.g=color.g*255,.b=color.b*255, .a=color.a*255});
}

void draw_gradient (vec pos1, vec pos2, vec4 color1, vec4 color2, vec4 color3, vec4 color4)
{

}
