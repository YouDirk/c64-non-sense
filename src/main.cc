 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
struct __vic2 {
union {
struct {
unsigned char spr0_x; 
unsigned char spr0_y; 
unsigned char spr1_x; 
unsigned char spr1_y; 
unsigned char spr2_x; 
unsigned char spr2_y; 
unsigned char spr3_x; 
unsigned char spr3_y; 
unsigned char spr4_x; 
unsigned char spr4_y; 
unsigned char spr5_x; 
unsigned char spr5_y; 
unsigned char spr6_x; 
unsigned char spr6_y; 
unsigned char spr7_x; 
unsigned char spr7_y; 
};
struct {
unsigned char x; 
unsigned char y; 
} spr_pos[8];
};
unsigned char spr_hi_x; 
unsigned char ctrl1; 
unsigned char rasterline; 
union {
struct {
unsigned char strobe_x; 
unsigned char strobe_y; 
};
struct {
unsigned char x; 
unsigned char y; 
} strobe;
};
unsigned char spr_ena; 
unsigned char ctrl2; 
unsigned char spr_exp_y; 
unsigned char addr; 
unsigned char irr; 
unsigned char imr; 
unsigned char spr_bg_prio; 
unsigned char spr_mcolor; 
unsigned char spr_exp_x; 
unsigned char spr_coll; 
unsigned char spr_bg_coll; 
unsigned char bordercolor; 
union {
struct {
unsigned char bgcolor0; 
unsigned char bgcolor1; 
unsigned char bgcolor2; 
unsigned char bgcolor3; 
};
unsigned char bgcolor[4]; 
};
union {
struct {
unsigned char spr_mcolor0; 
unsigned char spr_mcolor1; 
};
 
unsigned char spr_mcolors[2]; 
};
union {
struct {
unsigned char spr0_color; 
unsigned char spr1_color; 
unsigned char spr2_color; 
unsigned char spr3_color; 
unsigned char spr4_color; 
unsigned char spr5_color; 
unsigned char spr6_color; 
unsigned char spr7_color; 
};
unsigned char spr_color[8]; 
};
 
unsigned char x_kbd; 
unsigned char clock; 
};
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
struct __sid_voice {
unsigned freq; 
unsigned pw; 
unsigned char ctrl; 
unsigned char ad; 
unsigned char sr; 
};
struct __sid {
struct __sid_voice v1; 
struct __sid_voice v2; 
struct __sid_voice v3; 
unsigned flt_freq; 
unsigned char flt_ctrl; 
unsigned char amp; 
unsigned char ad1; 
unsigned char ad2; 
unsigned char noise; 
unsigned char read3; 
};
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
struct __6526 {
unsigned char pra; 
unsigned char prb; 
unsigned char ddra; 
unsigned char ddrb; 
unsigned char ta_lo; 
unsigned char ta_hi; 
unsigned char tb_lo; 
unsigned char tb_hi; 
unsigned char tod_10; 
unsigned char tod_sec; 
unsigned char tod_min; 
unsigned char tod_hour; 
unsigned char sdr; 
unsigned char icr; 
unsigned char cra; 
unsigned char crb; 
};
 
 
 
 
 
 
 
extern void c64_65816_emd[];
extern void c64_c256k_emd[];
extern void c64_dqbb_emd[];
extern void c64_georam_emd[];
extern void c64_isepic_emd[];
extern void c64_ram_emd[];
extern void c64_ramcart_emd[];
extern void c64_reu_emd[];
extern void c64_vdc_emd[];
extern void dtv_himem_emd[];
extern void c64_hitjoy_joy[];
extern void c64_numpad_joy[];
extern void c64_ptvjoy_joy[];
extern void c64_stdjoy_joy[]; 
extern void c64_1351_mou[]; 
extern void c64_joy_mou[];
extern void c64_inkwell_mou[];
extern void c64_pot_mou[];
extern void c64_swlink_ser[];
extern void c64_hi_tgi[]; 
 
 
 
unsigned char get_ostype (void);
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
typedef unsigned char* va_list;
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
typedef unsigned char _Bool;
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
typedef signed char int8_t;
typedef int int16_t;
typedef long int32_t;
typedef unsigned char uint8_t;
typedef unsigned uint16_t;
typedef unsigned long uint32_t;
 
typedef signed char int_least8_t;
typedef int int_least16_t;
typedef long int_least32_t;
typedef unsigned char uint_least8_t;
typedef unsigned uint_least16_t;
typedef unsigned long uint_least32_t;
 
typedef signed char int_fast8_t;
typedef int int_fast16_t;
typedef long int_fast32_t;
typedef unsigned char uint_fast8_t;
typedef unsigned uint_fast16_t;
typedef unsigned long uint_fast32_t;
 
typedef int intptr_t;
typedef unsigned uintptr_t;
 
typedef long intmax_t;
typedef unsigned long uintmax_t;
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
typedef int ptrdiff_t;
typedef char wchar_t;
typedef unsigned size_t;
 
 
 
char* __fastcall__ strcat (char* dest, const char* src);
char* __fastcall__ strchr (const char* s, int c);
int __fastcall__ strcmp (const char* s1, const char* s2);
int __fastcall__ strcoll (const char* s1, const char* s2);
char* __fastcall__ strcpy (char* dest, const char* src);
size_t __fastcall__ strcspn (const char* s1, const char* s2);
char* __fastcall__ strerror (int errcode);
size_t __fastcall__ strlen (const char* s);
char* __fastcall__ strncat (char* s1, const char* s2, size_t count);
int __fastcall__ strncmp (const char* s1, const char* s2, size_t count);
char* __fastcall__ strncpy (char* dest, const char* src, size_t count);
char* __fastcall__ strpbrk (const char* str, const char* set);
char* __fastcall__ strrchr (const char* s, int c);
size_t __fastcall__ strspn (const char* s1, const char* s2);
char* __fastcall__ strstr (const char* str, const char* substr);
char* __fastcall__ strtok (char* s1, const char* s2);
size_t __fastcall__ strxfrm (char* s1, const char* s2, size_t count);
void* __fastcall__ memchr (const void* mem, int c, size_t count);
int __fastcall__ memcmp (const void* p1, const void* p2, size_t count);
void* __fastcall__ memcpy (void* dest, const void* src, size_t count);
void* __fastcall__ memmove (void* dest, const void* src, size_t count);
void* __fastcall__ memset (void* s, int c, size_t count);
 
void* __fastcall__ _bzero (void* ptr, size_t n);
 
void __fastcall__ bzero (void* ptr, size_t n); 
char* __fastcall__ strdup (const char* s); 
int __fastcall__ stricmp (const char* s1, const char* s2); 
int __fastcall__ strcasecmp (const char* s1, const char* s2); 
int __fastcall__ strnicmp (const char* s1, const char* s2, size_t count); 
int __fastcall__ strncasecmp (const char* s1, const char* s2, size_t count); 
char* __fastcall__ strlwr (char* s);
char* __fastcall__ strlower (char* s);
char* __fastcall__ strupr (char* s);
char* __fastcall__ strupper (char* s);
char* __fastcall__ strqtok (char* s1, const char* s2);
const char* __fastcall__ _stroserror (unsigned char errcode);
 
 
typedef char string_t[256];
 
 
extern void Graphix_init(void);
extern void Graphix_release(void);
int
main(void)
{
unsigned char* x;
 
 
unsigned char d020_border = *(unsigned char*) 0xd020;
unsigned char dd00_cia2 = *(unsigned char*) 0xdd00;
unsigned char d011_mode = *(unsigned char*) 0xd011;
unsigned char d016_mode2 = *(unsigned char*) 0xd016;
unsigned char d018_buffer = *(unsigned char*) 0xd018;
Graphix_init();
 
memset((((void*) 0xc000 - 0x00*0x4000) + (void*) 0x00*0x04), 0x50, 40*25);
 
memset((void*) 0xe000, 0x18, 8*40*25);
for (x = (void*) 0xe000; x < (void*) (0xe000 + 8*40*25); x+=8)
*x = 0xff;
 
*(unsigned char*) 0xd011 = d011_mode | 0x20;
 
{
unsigned char scroll_x = d016_mode2 & 0x07;
unsigned char scroll_y = d011_mode & 0x07;
unsigned char joy_cntrl = 0x1f;
int prev_time = -1;
while (joy_cntrl & (1<<4)) {
while (prev_time >= 0
|| !(~(*(unsigned char*) 0xdc00) & 0x1f)) {
if (prev_time >= 0
&& (prev_time + 1) % 256 == *(unsigned char*) 0x00a2)
prev_time = -1;
}
prev_time = *(unsigned char*) 0x00a2;
joy_cntrl = *(unsigned char*) 0xdc00;
 
if (~joy_cntrl & (1<<0)) {
scroll_y = (scroll_y + 1) & 7;
*(unsigned char*) 0xd011
= ((*(unsigned char*) 0xd011) & 0xf8) | scroll_y;
}
 
if (~joy_cntrl & (1<<1)) {
scroll_y = (scroll_y - 1) & 7;
*(unsigned char*) 0xd011
= ((*(unsigned char*) 0xd011) & 0xf8) | scroll_y;
}
 
if (~joy_cntrl & (1<<2)) {
scroll_x = (scroll_x + 1) & 7;
*(unsigned char*) 0xd016
= ((*(unsigned char*) 0xd016) & 0xf8) | scroll_x;
}
 
if (~joy_cntrl & (1<<3)) {
scroll_x = (scroll_x - 1) & 7;
*(unsigned char*) 0xd016
= ((*(unsigned char*) 0xd016) & 0xf8) | scroll_x;
}
}
}
 
*(unsigned char*) 0xd016 = d016_mode2;
 
*(unsigned char*) 0xd011 = (d011_mode & 0xef) | 0x20;
Graphix_release();
return 0;
}
