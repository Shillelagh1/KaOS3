#define VGA_BLACK       0
#define VGA_BLUE        1
#define VGA_GREEN       2
#define VGA_CYAN        3
#define VGA_RED         4
#define VGA_MAGENTA     5
#define VGA_BROWN       6
#define VGA_LGRAY       7
#define VGA_GRAY        8
#define VGA_LBLUE       9
#define VGA_LGREEN      10
#define VGA_LCYAN       11
#define VGA_LRED        12
#define VGA_LMAGENTA    13
#define VGA_LBROWN      14
#define VGA_WHITE       15

#define VGA_W 80
#define VGA_H 25
#define VGA_OFFSET 0xB8000

void VGAT_put(int x, int y, char c);
void VGAT_write(int x, int y, char* str);
void VGAT_glyph(int x, int y, char* str);
void VGAT_print(char* str);
void VGAT_fill(char c);
void VGAT_color(int fg, int bg);