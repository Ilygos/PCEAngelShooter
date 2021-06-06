#include <huc.h>
#include <st.h>

int VSyncCnt;
int TimerCnt;
int MainCnt;
int SubCnt;

//ANIM STATIC VARIABLES
int tics, animWay, frame;

#incspr(fae, "fae.pcx", 0, 0, 2, 16);
#incpal(palette_fae, "fae.pcx");
#incspr(bullet, "bullet.pcx", 0, 0, 1, 1)
#incpal(bulletpal, "bullet.pcx")


#incchr(tiles_decor, "background.pcx", 32, 29);
#incbat(map_decor, "background.pcx", 0x1000, 32, 29);
#incpal(palette_decor, "background.pcx");

#define SPEED_BULLET 4
#define MAX_BULLETS 10
#define BULLET_SPRITE 1

struct bullet
{
	int x, y;
	char active;
};

struct bullet bullets[MAX_BULLETS];


void spr_make(spriteno,spritex,spritey,spritepattern,ctrl1,ctrl2,sprpal,sprpri)
int spriteno,spritex,spritey,spritepattern,ctrl1,ctrl2,sprpal,sprpri;
{
	spr_set(spriteno);
	spr_x(spritex);
	spr_y(spritey);
	spr_pattern(spritepattern);
	spr_ctrl(ctrl1,ctrl2);
	spr_pal(sprpal);
	spr_pri(sprpri);
}

void run_anim_character(void)
{
    if (tics > 2)
    {
        tics = 0;
        if (animWay > 0)
        {
             frame = frame + 1;
             if (frame > 7) frame = 7;
        }
        else if (animWay < 0) 
        {
            frame = frame - 1;
            if (frame < 0) frame = 0;
        }   
        else if (frame > 3)
            frame--;
        else if (frame < 3)
            frame++; 
        
        if (frame == 0) spr_pattern(0x5000);
        if (frame == 1) spr_pattern(0x5100);
        if (frame == 2) spr_pattern(0x5200);
        if (frame == 3) spr_pattern(0x5300);
        if (frame == 4) spr_pattern(0x5400);
        if (frame == 5) spr_pattern(0x5500);
        if (frame == 6) spr_pattern(0x5600);
        if (frame == 7) spr_pattern(0x5700);
    }
}

void main(void)
{
	unsigned char dead;
	int faeX, faeY, j1, j2;
	struct bullet *bp;
	unsigned char shootCooldown;
	unsigned char i, j;
	char isFacing;

    for (;;)
    {
		dead = 0;
		faeX = 100;
		faeY = 100;
		frame = 0;
		tics = 0;
		shootCooldown = 0;
		isFacing = 1;
		

		set_xres(256);

		//SPRITES INIT
		init_satb();

		for (i = 0; i < MAX_BULLETS; i++)
		{
			spr_make(BULLET_SPRITE + i, -16, 0, 0x5800, FLIP_MAS | SIZE_MAS, NO_FLIP | SZ_16x16, 1, 1);
			bullets[i].active = 0;
		}

		spr_make(0, faeX, faeY, 0x5000, FLIP_MAS|SIZE_MAS, NO_FLIP|SZ_32x32, 0, 1);

		load_palette(16, palette_fae, 1);
		load_palette(17, bulletpal, 1);

		load_vram(0x5000, fae, 0x800);
		load_vram(0x5800, bullet, 0x40);
		satb_update();

		load_background(tiles_decor, palette_decor, map_decor, 32, 29);

		while(!dead)
		{
			vsync();
			j1 = joy(0);
			j2 = joytrg(0);
			animWay = 0;
			tics++;

			//CONTROLS
			if (j1 & JOY_LEFT)
			{
				spr_ctrl(FLIP_X_MASK, FLIP_X);
				isFacing = -1;
				if (faeX > -8) faeX--;
			}
			if (j1 & JOY_RGHT)
			{
				spr_ctrl(FLIP_X_MASK, NO_FLIP_X);
				isFacing = 1;
				if (faeX < 232) faeX++;
			}
			if (j1 & JOY_UP)
			{
				animWay = 1;
				if (faeY > 0) faeY--;
			}
			if (j1 & JOY_DOWN)
			{
				animWay = -1;
				if (faeY < 210) faeY++;
			}

			if (j2 & JOY_A && !shootCooldown) 
			{
				for (bp = bullets, i = 0; i < MAX_BULLETS; ++i, ++bp)
				{
					if (!bp->active)
					{
						bp->active = isFacing;
						bp->x = faeX + 8 + isFacing * 16;
						bp->y = faeY + 10;
						shootCooldown = 10;
						spr_set(BULLET_SPRITE + i);
						if (isFacing > 0)
							spr_ctrl(FLIP_X_MASK, FLIP_X);
						else
							spr_ctrl(FLIP_X_MASK, NO_FLIP_X);
						break;
					}
				}
			}

			if (shootCooldown)
				shootCooldown--;

			bp = bullets;
			for (i = 0; i < MAX_BULLETS; i++)
			{
				if (bp->active)
				{
					spr_set(BULLET_SPRITE + i);
					spr_x(bp->x);
					spr_y(bp->y);
					bp->x += bp->active * SPEED_BULLET;
					//put_number(bp->x, 4, 0, 0);
					if (bp->x > 256 || bp->x < -16)
					{
						bp->active = 0;
						spr_x(-16);
						spr_y(0);
					}
				}
				++bp;
			}


			//SETTING SPRITE POS
			spr_set(0);
			spr_x(faeX);
        	spr_y(faeY);

			run_anim_character();

			/* END UPDATE*/
			satb_update();
			disp_on();
		}
    }
}