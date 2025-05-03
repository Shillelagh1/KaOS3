#include "VGAText.h"
#include <stdint.h>

static unsigned int vgarow = 0;
static unsigned int vgacol = 0;
static uint8_t vgacolor = 0;
volatile uint16_t* vgabuf = (uint16_t*)VGA_OFFSET;

void VGAT_put(int x, int y, char c){
    if(x < 0 || x > VGA_W || y < 0 || y > VGA_H){
        return;
    }
    vgabuf[y * VGA_W + x] = c | vgacolor << 8;
}

void VGAT_glyph(int x, int y, char* str){
    int i = 0;
    int c = x;
    int r = y;
    while(str[i] != 0){
        switch(str[i]){
            case '\n':
                c=x;
                r++;
                break;
            default:
                VGAT_put(c, r, str[i]);
                if(c++>=VGA_W-1){
                    c=x;
                    r++;
                }
                break;
        }
        i++;
    }
    
    vgarow = r;
    vgacol = c;
}

void VGAT_write(int x, int y, char* str){
    int i = 0;
    int c = x;
    int r = y;
    while(str[i] != 0){
        switch(str[i]){
            case '\n':
                c=0;
                r++;
                break;
            default:
                VGAT_put(c, r, str[i]);
                if(c++>=VGA_W-1){
                    c=0;
                    r++;
                }
                break;
        }
        i++;
    }

    vgarow = r;
    vgacol = c;
}

void VGAT_print(char* str){
    VGAT_write(vgacol, vgarow, str);
}

void VGAT_color(int fg, int bg){
    vgacolor = fg | bg << 4;
}

void VGAT_fill(char c){
    for(int i = 0; i < VGA_W; i++){
        for(int j = 0; j < VGA_H; j++){
            VGAT_put(i, j, c);
        }
    }
}

char* itoa_dec(int i)
{
	   static char text[12]; 
	   int loc = 11; 
	   text[11] = 0; 
	   char neg = 1;
	   if (i >= 0)
	   {
		   neg = 0;
		   i = -i;
	   }
	   while (i) 
	   { 
	       text[--loc] = '0' - (i%10); 
	       i/=10; 
	   }
	   if (loc==11) 
	       text[--loc] = '0'; 
	   if (neg)
		   text[--loc] = '-';	   
	   return &text[loc]; 
}