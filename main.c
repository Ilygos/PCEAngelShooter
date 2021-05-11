#include <huc.h>
#include <st.h>

int VSyncCnt;
int TimerCnt;
int MainCnt;
int SubCnt;


#incchr(tiles_decor, "background.pcx", 32, 29);
#incbat(map_decor, "background.pcx", 0x1000, 32, 29);
#incpal(palette_decor, "background.pcx");

void main(void)
{
    vsync(1);    
    set_xres(256);
    init_satb();
    load_background(tiles_decor, palette_decor, map_decor, 32, 29);
    satb_update();

    for (;;)
    {
        
    }
}