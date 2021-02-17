
#include "b3d_types.h"

B3L_Mesh_t b3d_box[] ={
0x000C0008,//vect num, tri num, normal num, 16bit DUMMY
0x3f000000,0x3f000000,0x3f000000,0xbf000000,0xbf000000,0xbf000000,//boundbox
0x3f000000,0xbf000000,0xbf000000,0xbf000000,0xbf000000,0xbf000000,0x3f000000,0x3f000000,//vects
0xbf000000,0xbf000000,0x3f000000,0xbf000000,0x3f000000,0xbf000000,0x3f000000,0xbf000000,
0xbf000000,0x3f000000,0x3f000000,0x3f000000,0x3f000000,0xbf000000,0x3f000000,0x3f000000,
0x00020003,0x00010000,0x00000003,0x00020000,0x00020004,0x00040006,0x00060004,0x00070005,//triIdx
0x00060005,0x00010003,0x00010007,0x00070005,0x00020006,0x00070003,0x00030006,0x00000001,
0x00050004,0x00040001,
0x003f0000,0x3f003f3f,0x3f3f0000,0x3f003f3f,0x3f00003f,0x003f0000,0x3f3f003f,0x3f000000,//uv
0x3f3f0000,0x003f0000,0x003f3f00,0x3f003f3f,0x003f0000,0x3f003f3f,0x3f3f0000,0x3f3f003f,
0x00003f00,0x3f00003f,
0x00000000,0x00000000,0xbf800000,0x00000000,0x00000000,0xbf800000,
0x3f800000,0x00000000,0x00000000,0x3f800000,0x00000000,0x00000000,
0x00000000,0x00000000,0x3f800000,0x00000000,0x00000000,0x3f800000,
0xbf800000,0x00000000,0x00000000,0xbf800000,0x00000000,0x00000000,
0x00000000,0x3f800000,0x00000000,0x00000000,0x3f800000,0x00000000,
0x00000000,0xbf800000,0x00000000,0x00000000,0xbf800000,0x00000000,

} ;

const u32 pal[256] = {
  0XFF000000, 0XFF0A0A0A, 0XFF171717, 0XFF212121, 0XFF2E2E2E, 0XFF3B3B3B, 0XFF484848, 0XFF595959,
  0XFF6A6A6A, 0XFF7B7B7B, 0XFF909090, 0XFFA3A3A3, 0XFFB8B8B8, 0XFFCECECE, 0XFFE5E5E5, 0XFFFFFEFF,
  0XFF000055, 0XFF00005D, 0XFF00006B, 0XFF0A007A, 0XFF19008D, 0XFF2400A0, 0XFF3100B3, 0XFF4000CA,
  0XFF5000E0, 0XFF6000F7, 0XFF742CFF, 0XFF8549FF, 0XFF9A65FF, 0XFFAF7EFF, 0XFFC597FF, 0XFFDEB2FF,
  0XFF370052, 0XFF3E0059, 0XFF4A0067, 0XFF560076, 0XFF650088, 0XFF74009A, 0XFF8300AC, 0XFF9600C2,
  0XFFA800D7, 0XFFBC00ED, 0XFFD21AFF, 0XFFE63FFF, 0XFFFD5DFF, 0XFFFF78FF, 0XFFFF91FF, 0XFFFFAEFF,
  0XFF6E002E, 0XFF750036, 0XFF830045, 0XFF900055, 0XFFA00068, 0XFFB1007A, 0XFFC2008C, 0XFFD600A1,
  0XFFEA00B6, 0XFFFF01CC, 0XFFFF35E4, 0XFFFF4FFA, 0XFFFF6AFF, 0XFFFF83FF, 0XFFFF9BFF, 0XFFFFB6FF,
  0XFF920000, 0XFF9A0000, 0XFFA80000, 0XFFB60000, 0XFFC70000, 0XFFD8002F, 0XFFEA004B, 0XFFFF0066,
  0XFFFF257E, 0XFFFF3D96, 0XFFFF55B0, 0XFFFF69C6, 0XFFFF80E0, 0XFFFF96FA, 0XFFFFADFF, 0XFFFFC7FF,
  0XFFA10000, 0XFFA90000, 0XFFB70000, 0XFFC60000, 0XFFD70000, 0XFFE90000, 0XFFFB0000, 0XFFFF1B39,
  0XFFFF325C, 0XFFFF4678, 0XFFFF5C95, 0XFFFF6FAD, 0XFFFF86C8, 0XFFFF9CE3, 0XFFFFB2FD, 0XFFFFCCFF,
  0XFF780000, 0XFF800900, 0XFF8D1500, 0XFF9B1C00, 0XFFAB2700, 0XFFBC3200, 0XFFCD3F00, 0XFFE14E00,
  0XFFF65F00, 0XFFFF6F2C, 0XFFFF8353, 0XFFFF956E, 0XFFFFAB8A, 0XFFFFC0A5, 0XFFFFD6BF, 0XFFFFF0DC,
  0XFF361400, 0XFF3D1A00, 0XFF492300, 0XFF552C00, 0XFF633900, 0XFF724700, 0XFF825500, 0XFF946600,
  0XFFA77800, 0XFFBA8A00, 0XFFD09F00, 0XFFE4B200, 0XFFFBC800, 0XFFFFDE3B, 0XFFFFF55E, 0XFFFFFF7F,
  0XFF001F00, 0XFF002300, 0XFF012900, 0XFF113100, 0XFF1D3D00, 0XFF2A4C00, 0XFF375B00, 0XFF466D00,
  0XFF578000, 0XFF689300, 0XFF7CA900, 0XFF8EBD00, 0XFFA3D300, 0XFFB9EB00, 0XFFCFFF15, 0XFFE8FF4D,
  0XFF002400, 0XFF002800, 0XFF002D00, 0XFF003300, 0XFF003F00, 0XFF004D00, 0XFF005D00, 0XFF007000,
  0XFF008300, 0XFF009600, 0XFF10AD00, 0XFF2AC000, 0XFF41D800, 0XFF57EF00, 0XFF6DFF24, 0XFF86FF54,
  0XFF002600, 0XFF002900, 0XFF002E00, 0XFF003500, 0XFF004000, 0XFF004E00, 0XFF005E00, 0XFF007100,
  0XFF008400, 0XFF009700, 0XFF00AE00, 0XFF00C200, 0XFF00D900, 0XFF00F000, 0XFF1CFF29, 0XFF42FF59,
  0XFF002300, 0XFF002600, 0XFF002C00, 0XFF003200, 0XFF003E00, 0XFF004C00, 0XFF005C00, 0XFF006F00,
  0XFF008100, 0XFF009500, 0XFF00AB00, 0XFF00BE00, 0XFF00D52A, 0XFF00ED4F, 0XFF13FF6C, 0XFF3EFF8B,
  0XFF001700, 0XFF001D00, 0XFF002300, 0XFF002B00, 0XFF003900, 0XFF004706, 0XFF005618, 0XFF00672B,
  0XFF00793D, 0XFF008B50, 0XFF00A067, 0XFF00B37C, 0XFF00C994, 0XFF00E0AC, 0XFF00F7C4, 0XFF2DFFDF,
  0XFF00001D, 0XFF000424, 0XFF00132D, 0XFF001D36, 0XFF002A44, 0XFF003752, 0XFF004462, 0XFF005574,
  0XFF006687, 0XFF00779C, 0XFF008BB4, 0XFF009DC9, 0XFF00B2E2, 0XFF00C8FB, 0XFF29DEFF, 0XFF49F8FF,
  0XFF000043, 0XFF000049, 0XFF000056, 0XFF000064, 0XFF000076, 0XFF000088, 0XFF00009A, 0XFF001EAF,
  0XFF0035C6, 0XFF0049DD, 0XFF005FF7, 0XFF1672FF, 0XFF2F88FF, 0XFF469FFF, 0XFF5BB5FF, 0XFF74CFFF,
  0XFF000055, 0XFF00005C, 0XFF00006B, 0XFF00007B, 0XFF00008E, 0XFF0000A1, 0XFF0000B4, 0XFF0000CB,
  0XFF0000E2, 0XFF0000FA, 0XFF1730FF, 0XFF2A4BFF, 0XFF3F66FF, 0XFF537FFF, 0XFF6897FF, 0XFF7FB2FF,};

const texture_t tex[4098] = {
  0x40,0x00,0X63, 0X32, 0X76, 0X32, 0X71, 0X80, 0X3, 0X7, 0XC0, 0X1, 0X90, 0X2, 0X90, 0X2, 0XC1, 0X1,
0XC0, 0X1, 0X90, 0X2, 0X1, 0X2, 0X83, 0X1, 0X1, 0X1, 0X80, 0X2, 0XC0, 0X80, 0X6, 0X3,
0XD3, 0X13, 0XF8, 0X17, 0XF8, 0XF8, 0X17, 0XF9, 0XF8, 0XF8, 0XF8, 0XF8, 0X17, 0XF8, 0XF8, 0XF8,
0XF8, 0XF7, 0XF8, 0X6F, 0X6F, 0XE, 0XE, 0X8, 0X66, 0X65, 0X69, 0X65, 0X78, 0X66, 0X8, 0X70,
0X76, 0X33, 0X76, 0X32, 0X75, 0XD2, 0X4, 0X5, 0XC1, 0X2, 0X80, 0XD3, 0XD4, 0X80, 0X1, 0X1,
0X1, 0X2, 0X3, 0X90, 0X1, 0X1, 0X1, 0XD2, 0XC0, 0X1, 0X3, 0X1, 0X1, 0X2, 0XD5, 0XC6,
0X4, 0XE7, 0X17, 0XFA, 0X17, 0XFA, 0XF8, 0X17, 0XF8, 0X17, 0XFA, 0X17, 0XF9, 0XF8, 0X17, 0XF8,
0X17, 0XF8, 0XF7, 0X1B, 0XD, 0XD, 0X6E, 0X6E, 0X65, 0X9, 0X65, 0X9, 0X67, 0X58, 0X78, 0X2,
0X32, 0X65, 0X7, 0X64, 0X31, 0XC5, 0X3, 0X90, 0XD1, 0XD3, 0XB2, 0X4, 0XC6, 0X4, 0XD3, 0X80,
0X1, 0XC0, 0X3, 0XD2, 0X80, 0X1, 0XC0, 0X1, 0XC0, 0X1, 0X3, 0X2, 0X1, 0X80, 0X83, 0X5,
0XC6, 0X17, 0XF8, 0XF8, 0XF8, 0X17, 0XF8, 0XFA, 0XF8, 0X17, 0XF8, 0XF8, 0X17, 0XFA, 0XF8, 0XF8,
0XE8, 0XF8, 0X16, 0XE7, 0X1E, 0XD, 0X5F, 0XD, 0X6E, 0X66, 0X69, 0X65, 0X9, 0X66, 0X65, 0X6,
0X76, 0X32, 0X65, 0X7, 0X63, 0X4, 0XD3, 0XC1, 0X82, 0XD3, 0XC6, 0XD2, 0X83, 0X2, 0XC6, 0X3,
0X3, 0X90, 0X2, 0X1, 0X83, 0XD1, 0XC0, 0X2, 0X80, 0X1, 0X1, 0X1, 0X1, 0X2, 0X1, 0X5,
0XD5, 0XF8, 0X17, 0XFA, 0X16, 0XF8, 0X17, 0XF8, 0X17, 0XF8, 0XFA, 0XF8, 0XF8, 0XF8, 0XF7, 0X17,
0XF8, 0XF8, 0XF8, 0XF7, 0XF7, 0XD, 0XD, 0XE, 0X6E, 0XC, 0X65, 0X69, 0X65, 0X9, 0X67, 0X76,
0X6E, 0XD, 0X64, 0X33, 0X76, 0X5, 0X83, 0XD2, 0X3, 0X7, 0X4, 0XC0, 0X1, 0X1, 0X1, 0XC1,
0X3, 0XD4, 0X5, 0X4, 0X3, 0X80, 0XD2, 0X4, 0X80, 0X1, 0X1, 0X1, 0X3, 0X80, 0XD3, 0X83,
0X14, 0X16, 0XE8, 0XF7, 0XF8, 0XFA, 0XF8, 0X17, 0XFA, 0XF8, 0X17, 0XF8, 0X17, 0XFA, 0XF7, 0XF8,
0XF8, 0X17, 0XF7, 0XF7, 0XF7, 0X17, 0XE, 0XE, 0XE, 0XE, 0X9, 0X66, 0X6A, 0X66, 0X9, 0X66,
0XE, 0X7, 0X64, 0X7, 0X64, 0X31, 0XD2, 0X83, 0XD4, 0X80, 0X90, 0X3, 0X1, 0X1, 0X1, 0X1,
0XC0, 0X4, 0X5, 0X6, 0X7, 0X5, 0XC6, 0X4, 0X3, 0X1, 0X1, 0XC0, 0X1, 0X1, 0X1, 0X80,
0X83, 0X1, 0X2, 0X2, 0X3, 0X20, 0XE7, 0XF7, 0XF7, 0X17, 0XF8, 0XF8, 0XF8, 0X17, 0XF8, 0XFA,
0XF7, 0XF8, 0XE8, 0X17, 0XF7, 0XE7, 0X1B, 0XD, 0X6E, 0X4F, 0X6F, 0X9, 0X66, 0X78, 0X67, 0X65,
0XE, 0X9, 0X64, 0X32, 0X76, 0X32, 0X85, 0XD3, 0XC1, 0X1, 0X2, 0XD3, 0X1, 0X1, 0X1, 0X1,
0XC0, 0XC0, 0X80, 0X3, 0XC6, 0X7, 0X2, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1,
0XD0, 0X1, 0X1, 0X1, 0X1, 0X1, 0X2, 0X2, 0X3, 0XD2, 0X14, 0X15, 0X17, 0XF9, 0X17, 0XF8,
0X17, 0XF8, 0XF8, 0XF7, 0XF7, 0XF7, 0XF7, 0X1C, 0X6E, 0XD, 0X6E, 0XE, 0X8, 0X69, 0X8, 0X67,
0X6C, 0X5D, 0X75, 0X32, 0X76, 0X32, 0X5, 0X1, 0X1, 0X1, 0X4, 0XC0, 0X1, 0X1, 0X1, 0X1,
0X2, 0X1, 0X1, 0X1, 0XD0, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1,
0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0XC0, 0XC0, 0X1, 0XE5, 0XFA, 0XF7,
0XF8, 0X17, 0XF8, 0XF7, 0X17, 0XF7, 0XE7, 0XF7, 0X1E, 0XE, 0XE, 0X6E, 0X6E, 0X66, 0X6A, 0X65,
0X63, 0X32, 0X76, 0X33, 0X76, 0X7, 0X6B, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X3,
0X1, 0XD2, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X2, 0X2, 0X2,
0X1, 0X2, 0X2, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0XC0, 0XE1, 0X17,
0XF8, 0XFA, 0XF7, 0XF7, 0XE8, 0XF7, 0X17, 0XF7, 0XE7, 0X1F, 0XD, 0X5F, 0XE, 0XD, 0X8, 0X67,
0X76, 0X33, 0X65, 0X7, 0X65, 0X32, 0X74, 0X1, 0X1, 0X83, 0X2, 0X1, 0X1, 0X1, 0X83, 0X1,
0XC0, 0X4, 0X6, 0XC0, 0X1, 0X1, 0X2, 0X1, 0X1, 0X1, 0X1, 0X2, 0X2, 0XD4, 0X5, 0XD5,
0X5, 0X5, 0XD4, 0X4, 0X2, 0X1, 0X2, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0XF7,
0XF7, 0X17, 0XF7, 0XF8, 0X17, 0XF7, 0XF7, 0XF7, 0X16, 0XE7, 0X4F, 0X6E, 0XE, 0XE, 0X6D, 0X65,
0X32, 0X78, 0X33, 0X65, 0X33, 0X76, 0X32, 0X70, 0X1, 0X8, 0X3, 0X90, 0X2, 0X80, 0XD3, 0X1,
0X1, 0X4, 0X3, 0X1, 0X1, 0X1, 0X1, 0X2, 0X1, 0X3, 0X4, 0XD6, 0X7, 0XFE, 0XA, 0XFD,
0X8, 0XFD, 0XA, 0X9, 0XD9, 0X5, 0XD2, 0X2, 0X1, 0X1, 0X3, 0X2, 0X1, 0X1, 0X1, 0X13,
0XFA, 0XF7, 0XF8, 0XE8, 0XF8, 0XF7, 0X17, 0XF8, 0XF7, 0X17, 0X16, 0XE, 0X6F, 0XD, 0XE, 0X6C,
0X76, 0X33, 0X76, 0X7, 0X64, 0X33, 0X76, 0X5, 0X2, 0X8, 0X8, 0X2, 0X2, 0XD2, 0X1, 0X1,
0X1, 0X3, 0XC0, 0X1, 0X1, 0X2, 0X1, 0XD2, 0X6, 0XD9, 0XFE, 0XFD, 0XA, 0XFD, 0XFD, 0XA,
0XED, 0XFE, 0XFD, 0XFD, 0XFE, 0X9, 0XD8, 0X4, 0XD2, 0X2, 0X1, 0X1, 0X1, 0X1, 0X1, 0X13,
0X17, 0XF8, 0X17, 0XF8, 0X16, 0XF8, 0XE8, 0XF7, 0XF7, 0XE8, 0XF8, 0XFD, 0X6E, 0XE, 0X5F, 0X6E,
0X64, 0X7, 0X45, 0X76, 0X33, 0X76, 0X33, 0X62, 0X1, 0X1, 0X1, 0X1, 0XC0, 0X80, 0X1, 0X1,
0XC0, 0X3, 0X1, 0X1, 0X2, 0X1, 0X2, 0X9, 0XFD, 0X9, 0XFE, 0X9, 0XFE, 0XA, 0XFF, 0XFE,
0XB, 0XA, 0XFF, 0XA, 0XFD, 0XFD, 0XFD, 0X9, 0X9, 0XD6, 0X2, 0X1, 0X1, 0X1, 0X1, 0X13,
0XF8, 0XF7, 0XF8, 0XF7, 0XE8, 0X17, 0XF8, 0X17, 0XF8, 0X17, 0XF7, 0XF7, 0X1D, 0XE, 0XE, 0X6F,
0X33, 0X65, 0X32, 0X76, 0X34, 0X65, 0X7, 0X63, 0X1, 0X1, 0X1, 0X6, 0X4, 0X80, 0X2, 0X1,
0X1, 0X1, 0X1, 0X1, 0X1, 0X2, 0XD6, 0XFD, 0X9, 0XFD, 0X9, 0XFD, 0XFE, 0XFF, 0XA, 0XFF,
0XFF, 0XFF, 0XB, 0XED, 0XA, 0XA, 0XFD, 0XFC, 0X9, 0XFC, 0X5, 0XD0, 0X1, 0X1, 0X1, 0X14,
0XFA, 0X17, 0XE8, 0XF8, 0X17, 0XF8, 0XF7, 0XF7, 0XE8, 0XF7, 0X17, 0XF7, 0XF7, 0XD, 0XE, 0XE,
0X9, 0X6A, 0X76, 0X33, 0X77, 0X33, 0X76, 0X32, 0X3, 0X1, 0XC0, 0X4, 0X5, 0X3, 0X3, 0XC1,
0X1, 0X1, 0X1, 0X1, 0X2, 0X4, 0X7, 0X9, 0XFC, 0XA, 0XEC, 0XFE, 0XA, 0XFE, 0XB, 0XC,
0XC, 0XFF, 0XFE, 0XA, 0XFD, 0XFD, 0X9, 0XEC, 0X9, 0XFC, 0XD8, 0X4, 0X1, 0X1, 0X1, 0X15,
0XF8, 0XF8, 0X17, 0XF8, 0XF8, 0X17, 0XF7, 0XF7, 0X17, 0XF8, 0XF7, 0X17, 0XF8, 0XF7, 0X1F, 0XE,
0X6F, 0X31, 0X76, 0X33, 0X76, 0X7, 0X65, 0X7, 0X62, 0X2, 0X5, 0X1, 0X5, 0XC7, 0X3, 0XD3,
0X2, 0XC0, 0X1, 0X2, 0X2, 0X4, 0XD8, 0XFD, 0XDA, 0XFD, 0XA, 0XFD, 0XA, 0XFF, 0XFF, 0XFF,
0XC, 0XFF, 0XB, 0XFE, 0XA, 0XFD, 0XA, 0XFC, 0XDA, 0X9, 0X9, 0XD5, 0X2, 0X1, 0X1, 0X15,
0XF8, 0X17, 0XF8, 0X17, 0XE8, 0XF8, 0XE8, 0X17, 0XF8, 0XF7, 0XE8, 0XF8, 0X17, 0XF8, 0X16, 0XE,
0X6D, 0X32, 0X76, 0X34, 0X65, 0X33, 0X65, 0X7, 0X9, 0X5, 0X3, 0X1, 0X1, 0X6, 0X3, 0XB2,
0XD2, 0X90, 0X1, 0X1, 0X2, 0X5, 0X9, 0XFD, 0X9, 0XFD, 0XA, 0XED, 0XFE, 0XB, 0XFF, 0XB,
0XFF, 0XB, 0XFE, 0XFE, 0XFE, 0XA, 0XFC, 0XA, 0XFC, 0X9, 0XFB, 0X6, 0XD3, 0X1, 0X1, 0XE7,
0X18, 0XF8, 0XFA, 0XF7, 0X17, 0XF8, 0X17, 0XF8, 0XF7, 0X17, 0XF8, 0XF7, 0XF7, 0XE8, 0XF8, 0X17,
0XE, 0X9, 0X65, 0X7, 0X65, 0X7, 0X64, 0X9, 0X5D, 0X70, 0X1, 0X1, 0X1, 0X1, 0X3, 0X4,
0X82, 0XD2, 0X80, 0X3, 0X5, 0XE8, 0XA, 0XFC, 0X9, 0XED, 0XA, 0XFE, 0XB, 0XFF, 0XC, 0XFF,
0XC, 0XFF, 0XC, 0XB, 0XFE, 0XA, 0XDB, 0X9, 0XFC, 0XA, 0XD9, 0X9, 0XD7, 0X20, 0X1, 0XF8,
0XF8, 0X17, 0XF7, 0XF8, 0XF7, 0X17, 0XF8, 0XE8, 0XF8, 0XF7, 0XE8, 0X17, 0XF8, 0X17, 0XFA, 0XF7,
0X7, 0X6B, 0X32, 0X65, 0X7, 0X65, 0X6E, 0X5F, 0XD, 0X8, 0X1, 0X1, 0X1, 0X1, 0XC0, 0X4,
0X3, 0X83, 0XD2, 0X4, 0X7, 0XFD, 0XA, 0XD9, 0XFD, 0X9, 0X9, 0X9, 0XFE, 0XC, 0XD, 0X1F,
0XD, 0X1E, 0XD, 0X1E, 0XB, 0XFD, 0X9, 0XFD, 0XD9, 0XFD, 0X9, 0XFC, 0X9, 0XD6, 0XD1, 0X17,
0XFA, 0XF8, 0X17, 0XE8, 0X17, 0XF8, 0X17, 0XF7, 0X17, 0XF8, 0X17, 0XF8, 0XF7, 0XF8, 0X17, 0XF8,
0X64, 0X32, 0X76, 0X33, 0X65, 0X33, 0X76, 0X9, 0X6E, 0XB, 0X1, 0X1, 0X1, 0X1, 0X1, 0X2,
0XD2, 0X2, 0X5, 0XD8, 0X9, 0XD8, 0X21, 0X5, 0X6, 0X9, 0XFC, 0X9, 0X9, 0X9, 0XFE, 0XC,
0XFF, 0XC, 0XFF, 0X9, 0XD6, 0X7, 0X5, 0X5, 0X7, 0X8, 0XFC, 0X8, 0XD9, 0X9, 0X22, 0XF8,
0XF8, 0X17, 0XF8, 0XF8, 0XF8, 0XE8, 0X17, 0XFA, 0XF7, 0XF7, 0XE8, 0X17, 0XF8, 0XE8, 0X17, 0XF8,
0X76, 0X33, 0X65, 0X7, 0X65, 0X7, 0X64, 0X34, 0X6F, 0X7, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1,
0XC0, 0X1, 0X5, 0X7, 0XF1, 0XD2, 0X4, 0XFD, 0X9, 0XE9, 0X8, 0XFC, 0X9, 0XFD, 0XD8, 0X5,
0X7, 0XD6, 0X5, 0X9, 0XFD, 0XA, 0XFC, 0X9, 0XE8, 0XD6, 0XE0, 0X4, 0XFC, 0X7, 0XD8, 0X18,
0XFA, 0XF8, 0X17, 0XFA, 0X17, 0XF8, 0XF7, 0XF7, 0X17, 0XF8, 0X17, 0XF8, 0X17, 0XF8, 0XF8, 0X17,
0X64, 0X7, 0X65, 0X33, 0X65, 0X33, 0X76, 0X65, 0X9, 0X64, 0X2, 0X1, 0X80, 0X2, 0X1, 0X1,
0X1, 0X2, 0X7, 0X10, 0X7, 0X8, 0X15, 0XD8, 0XD8, 0X9, 0XFC, 0X9, 0XD9, 0X9, 0X8, 0X12,
0X14, 0XD2, 0X15, 0XD9, 0X9, 0XFE, 0X9, 0XE8, 0X7, 0X8, 0X12, 0X2, 0X5, 0X6, 0X14, 0X17,
0XF8, 0XFA, 0XF7, 0X17, 0XF7, 0X17, 0XF8, 0X17, 0XE8, 0XF8, 0XE8, 0XF8, 0XF7, 0X17, 0XE8, 0XF8,
0X64, 0X34, 0X77, 0X7, 0X65, 0X7, 0X65, 0X33, 0X76, 0X32, 0X72, 0X2, 0X2, 0XC1, 0X1, 0XC1,
0X1, 0XD0, 0X7, 0X7, 0XE7, 0XD8, 0X4, 0X14, 0XD6, 0X20, 0XD1, 0XD3, 0XD6, 0XFB, 0X9, 0XD6,
0XFE, 0X4, 0XA, 0XE8, 0X5, 0XD1, 0XD4, 0XD6, 0X14, 0XD8, 0X20, 0X9, 0XD6, 0X20, 0X5, 0X17,
0XF8, 0X17, 0XF8, 0XF8, 0XE8, 0X17, 0XE8, 0XF8, 0X17, 0XF8, 0X17, 0XF7, 0X17, 0XFA, 0XF7, 0XF8,
0X76, 0X34, 0X65, 0X76, 0X33, 0X65, 0X7, 0X65, 0X33, 0X76, 0X31, 0XC0, 0X1, 0X1, 0X1, 0X1,
0X2, 0XC0, 0XD7, 0X8, 0XFD, 0X7, 0X14, 0X9, 0XD7, 0X8, 0X9, 0XFD, 0X9, 0XB, 0XFC, 0X9,
0XB, 0XE8, 0X9, 0XA, 0X9, 0XD8, 0X9, 0X23, 0XD6, 0XD7, 0XD6, 0XE8, 0X7, 0XD7, 0X21, 0XFA,
0XF8, 0X17, 0XF8, 0X17, 0XF8, 0XF8, 0X17, 0XF8, 0XF8, 0X17, 0XE8, 0XF8, 0XF7, 0X17, 0XF8, 0XF8,
0X64, 0X7, 0X65, 0X7, 0X65, 0X33, 0X65, 0X7, 0X65, 0X7, 0X63, 0X2, 0X3, 0X1, 0X1, 0X1,
0XD7, 0X9, 0X8, 0XFB, 0X9, 0XFD, 0XD9, 0XFD, 0X9, 0XFD, 0XFE, 0XA, 0XFE, 0XC, 0XD8, 0XFE,
0XB, 0XFD, 0X8, 0XFE, 0XFE, 0X1C, 0XA, 0XFC, 0X9, 0XFD, 0X7, 0X15, 0X7, 0X8, 0X14, 0XE8,
0XFA, 0X17, 0XFA, 0XF7, 0X17, 0XE8, 0XF8, 0X17, 0XE8, 0XF8, 0X17, 0XF8, 0X17, 0XFA, 0XF7, 0XF8,
0X6F, 0X32, 0X65, 0X33, 0X65, 0X8, 0X65, 0X34, 0X65, 0X32, 0X76, 0X2, 0X1, 0X1, 0X1, 0X2,
0X14, 0XE7, 0X7, 0X9, 0XFC, 0XA, 0X9, 0XE9, 0XA, 0XFE, 0XB, 0XFF, 0XB, 0X7, 0XFD, 0X1C,
0XFE, 0XA, 0XD8, 0XB, 0XB, 0XB, 0XFE, 0XFE, 0XA, 0X15, 0XD9, 0X9, 0XFB, 0XD8, 0X6, 0X8,
0X16, 0XE8, 0X17, 0XF8, 0XF8, 0X17, 0XF8, 0X17, 0XF8, 0X17, 0XF8, 0XFA, 0XF7, 0XF7, 0X17, 0XF8,
0X8, 0X65, 0X7, 0X65, 0X7, 0X65, 0X7, 0X6B, 0X9, 0X76, 0X33, 0X70, 0X4, 0X1, 0X1, 0X2,
0XD7, 0X7, 0X16, 0XD9, 0X9, 0XFC, 0XFD, 0X9, 0XFD, 0X9, 0XFC, 0X9, 0XFC, 0XFE, 0X9, 0XFE,
0XA, 0XFD, 0X9, 0XFC, 0X7, 0XFD, 0X9, 0XFE, 0X9, 0XE8, 0X9, 0XE9, 0X7, 0X16, 0XD7, 0X14,
0XD7, 0X18, 0XF8, 0X17, 0XFA, 0XF7, 0X17, 0XE8, 0XF8, 0XFA, 0X16, 0XF7, 0X17, 0XF8, 0XFA, 0X17,
0X6B, 0X32, 0X65, 0X8, 0X45, 0X65, 0X32, 0XA, 0X6E, 0XB, 0X6D, 0X4, 0X1, 0X1, 0X1, 0X3,
0XFD, 0X9, 0XD9, 0X9, 0XFB, 0X9, 0XFD, 0X9, 0XFE, 0XB, 0XFF, 0XC, 0XB, 0XFD, 0XA, 0XB,
0XFF, 0XFD, 0X9, 0XFE, 0XC, 0XFF, 0X9, 0XFD, 0X8, 0XFC, 0X9, 0X9, 0XE9, 0X8, 0X22, 0X9,
0XFC, 0XFD, 0XFD, 0X17, 0XE8, 0X17, 0XF8, 0XF8, 0X17, 0XF8, 0X17, 0XF8, 0XFA, 0X16, 0XF8, 0XF7,
0XC, 0X8, 0X65, 0X64, 0X7, 0X65, 0XA, 0X6E, 0X6F, 0X6E, 0X7, 0X50, 0X2, 0X1, 0X1, 0X2,
0XFE, 0X9, 0XFC, 0X7, 0X8, 0XFC, 0X9, 0XFE, 0XFE, 0XB, 0X1E, 0XFF, 0XA, 0XFD, 0X9, 0XFD,
0XFD, 0X9, 0XFD, 0X9, 0XFE, 0X1E, 0XC, 0XFE, 0X9, 0XFC, 0X9, 0XE9, 0X7, 0X7, 0XE7, 0XD9,
0XB, 0X6E, 0X6E, 0XE, 0XE, 0XB, 0XFC, 0X16, 0XF8, 0X17, 0XE8, 0X17, 0XF8, 0XF7, 0X17, 0XF8,
0X64, 0X64, 0X7, 0X65, 0X33, 0X65, 0XB, 0X6E, 0XE, 0X5F, 0X6B, 0X31, 0X83, 0X1, 0X1, 0X1,
0X9, 0XFD, 0X8, 0X8, 0XFB, 0X9, 0XFC, 0X9, 0XFE, 0XC, 0XFF, 0XB, 0XFD, 0XD7, 0X20, 0XD5,
0XD6, 0XE0, 0XD7, 0XFD, 0XA, 0XFF, 0XB, 0XFD, 0X9, 0XFC, 0X9, 0XE8, 0X8, 0XE8, 0X7, 0X8,
0XFD, 0XE, 0X5F, 0X6F, 0X4F, 0XE, 0X4F, 0XD, 0XFD, 0X16, 0XF8, 0XF8, 0X17, 0XF8, 0XE8, 0XF8,
0X34, 0X65, 0X33, 0X65, 0X8, 0X65, 0X76, 0X7, 0X6E, 0XA, 0XB, 0X32, 0X73, 0X2, 0XC1, 0XD0,
0XD7, 0XFD, 0X7, 0X17, 0X9, 0XFC, 0XA, 0XFD, 0XA, 0XFE, 0XB, 0XFE, 0XB, 0X8, 0XFD, 0XB,
0X1B, 0X9, 0X16, 0XA, 0XFE, 0XB, 0XFF, 0X1C, 0XA, 0XD9, 0X9, 0XFC, 0X7, 0X14, 0X7, 0XFC,
0XD, 0X6E, 0XE, 0XE, 0X6E, 0XE, 0X6E, 0XE, 0XE, 0X1F, 0XFC, 0X16, 0XF8, 0X17, 0XF8, 0XFA,
0X76, 0X8, 0X65, 0X7, 0X66, 0X33, 0X34, 0X65, 0XA, 0X43, 0X65, 0X7, 0X62, 0XD2, 0XC1, 0X1,
0X4, 0X9, 0XE8, 0X9, 0XD9, 0X9, 0XFD, 0XA, 0XFD, 0XB, 0XFE, 0XB, 0XFF, 0X1C, 0XFE, 0XA,
0XFE, 0XA, 0XFF, 0XFE, 0XB, 0XFF, 0XB, 0XFD, 0X9, 0XFC, 0X8, 0X16, 0XD8, 0X8, 0XE8, 0X9,
0XD, 0X5F, 0XE, 0XE, 0X5F, 0XE, 0XE, 0X5F, 0X6E, 0XD, 0X6E, 0XFD, 0X16, 0XFA, 0XF8, 0X17,
0X65, 0X33, 0X65, 0X34, 0X65, 0X77, 0X7, 0X65, 0X33, 0X76, 0X34, 0X76, 0X32, 0X83, 0X1, 0X1,
0XD4, 0XFE, 0X7, 0X8, 0XFC, 0X9, 0XFC, 0XA, 0XFD, 0XFE, 0XB, 0XFF, 0XA, 0XB, 0XFD, 0X9,
0XFD, 0X9, 0XFE, 0XA, 0XB, 0XFE, 0XB, 0XFD, 0X9, 0XFC, 0X8, 0XE8, 0X8, 0X14, 0X9, 0XFD,
0X6F, 0XE, 0XE, 0X6E, 0XE, 0XE, 0X6E, 0XE, 0XE, 0XE, 0X6F, 0X6F, 0X1D, 0X16, 0XF8, 0X17,
0X76, 0X34, 0X78, 0X76, 0X7, 0X65, 0X33, 0X78, 0X34, 0X78, 0X7, 0X66, 0X32, 0X4, 0X80, 0XD3,
0X3, 0X9, 0XE8, 0X9, 0X8, 0XFC, 0XA, 0XFD, 0X9, 0XFE, 0XA, 0XFE, 0X9, 0X16, 0XD6, 0X14,
0XD6, 0X14, 0X9, 0XFD, 0XA, 0XFE, 0X9, 0XFD, 0X9, 0XFC, 0X8, 0X8, 0XD8, 0X7, 0X9, 0X1E,
0XE, 0X6F, 0X5F, 0XE, 0XE, 0XE, 0X5F, 0X6F, 0XE, 0X6E, 0X5F, 0XD, 0XE, 0XC, 0XF8, 0X17,
0X65, 0X7, 0X65, 0X34, 0X65, 0X8, 0X65, 0X7, 0X65, 0X33, 0X65, 0X7, 0X65, 0X5, 0X83, 0X3,
0X90, 0XD2, 0X6, 0X8, 0XFB, 0X9, 0XFC, 0XA, 0XFD, 0XA, 0XFE, 0X9, 0XE8, 0X9, 0X9, 0XFD,
0XA, 0X8, 0X15, 0XD8, 0X1C, 0XA, 0XFE, 0XFD, 0X9, 0XD9, 0XFC, 0XD7, 0X24, 0XD7, 0XB, 0XE,
0X6F, 0XE, 0X6F, 0X6E, 0X5F, 0X6F, 0X6F, 0X6E, 0X5F, 0XE, 0X6F, 0X6E, 0XE, 0X5F, 0X1D, 0XE7,
0X6D, 0X32, 0X65, 0X8, 0X65, 0X33, 0X65, 0X33, 0X78, 0X7, 0X66, 0X33, 0X65, 0X31, 0XC6, 0XD3,
0X83, 0XD0, 0X5, 0X16, 0XD9, 0X9, 0XFC, 0X9, 0XFD, 0XA, 0XFE, 0X9, 0XFE, 0X8, 0XFB, 0X7,
0XFB, 0X8, 0XFC, 0X9, 0XA, 0XFE, 0XA, 0XFD, 0X9, 0XFC, 0X8, 0X24, 0XD8, 0X9, 0X5F, 0XE,
0X5F, 0XE, 0XE, 0XE, 0X6F, 0XE, 0X4F, 0X6F, 0XE, 0X6E, 0XE, 0XE, 0X4F, 0XC, 0XB, 0X1C,
0X76, 0X8, 0X65, 0X33, 0X65, 0X7, 0X65, 0XC, 0X64, 0X33, 0X65, 0X7, 0X65, 0X7, 0X3, 0X83,
0X1, 0XD0, 0XD3, 0X8, 0X8, 0XE8, 0X9, 0XD9, 0X9, 0XFD, 0XA, 0XFD, 0X9, 0XFB, 0XD6, 0X14,
0XD6, 0X15, 0XA, 0XFD, 0XFD, 0XA, 0XFD, 0X9, 0XD9, 0X9, 0XE8, 0X7, 0X23, 0XC, 0XE, 0XE,
0X6F, 0XE, 0XE, 0X5F, 0X6E, 0XE, 0X6E, 0XE, 0XE, 0X5F, 0XE, 0XD, 0X1B, 0XF6, 0XE, 0XE,
0X64, 0X34, 0X65, 0X8, 0X65, 0X8, 0X65, 0X5F, 0X6C, 0XA, 0X6C, 0X63, 0X7, 0X64, 0X4, 0X1,
0X1, 0XC0, 0X2, 0XE8, 0X7, 0X16, 0XD9, 0X9, 0XFB, 0X9, 0XD9, 0XFD, 0XA, 0XFE, 0XA, 0XB,
0XFD, 0XA, 0XFD, 0X9, 0XFD, 0X9, 0XD9, 0X9, 0XFC, 0X22, 0XD7, 0X7, 0X14, 0XE, 0XE, 0XF,
0X5F, 0XE, 0X6F, 0X6F, 0XE, 0X5F, 0X6F, 0XE, 0X6E, 0XE, 0X1D, 0X16, 0XF7, 0XB, 0X5F, 0XE,
0X6E, 0X76, 0X34, 0X65, 0X8, 0X65, 0X9, 0X6E, 0XE, 0X6E, 0X7, 0X65, 0X33, 0X65, 0X30, 0X1,
0XC0, 0X1, 0X4, 0X7, 0XE8, 0X7, 0X16, 0XD8, 0X7, 0XFC, 0X9, 0XFD, 0XA, 0XFE, 0XC, 0XFE,
0XC, 0XFF, 0XB, 0XFD, 0X9, 0XFC, 0XD8, 0X24, 0XD8, 0X7, 0X14, 0X6, 0X6, 0XE, 0X5F, 0XE,
0X6F, 0XE, 0X5F, 0X6E, 0XE, 0X6F, 0XE, 0X4F, 0X1E, 0X17, 0XE8, 0XF8, 0X17, 0X1F, 0XE, 0XE,
0X32, 0X65, 0X77, 0X34, 0X65, 0X9, 0X5D, 0X6F, 0XE, 0X6E, 0X33, 0X76, 0X8, 0X65, 0X6, 0X1,
0X1, 0X1, 0XD1, 0X7, 0X15, 0X7, 0XD7, 0X23, 0XD8, 0X7, 0XE8, 0X9, 0XFD, 0XA, 0XB, 0XFE,
0XB, 0XFE, 0X9, 0XFC, 0XD8, 0X8, 0XD8, 0XD6, 0X22, 0XD6, 0XD6, 0X22, 0X8, 0XE, 0XE, 0XE,
0X5F, 0X6F, 0XE, 0XE, 0XE, 0X4F, 0XD, 0XFC, 0X16, 0XE8, 0X17, 0XF8, 0XFC, 0X6E, 0XE, 0XE,
0X65, 0X7, 0X34, 0X78, 0X64, 0X65, 0X76, 0X5D, 0XE, 0XD, 0X6E, 0X64, 0X33, 0X65, 0X33, 0X3,
0X2, 0XC0, 0X1, 0XD3, 0X7, 0XD7, 0X15, 0XD7, 0X22, 0XD7, 0X7, 0XD7, 0X7, 0XFC, 0X9, 0XFC,
0X9, 0X9, 0XFC, 0X7, 0X7, 0X15, 0X5, 0X14, 0XD6, 0X5, 0XD6, 0X14, 0XFA, 0X1B, 0XE, 0X6F,
0XE, 0XE, 0XE, 0X5F, 0X6E, 0XFD, 0X16, 0XF7, 0X17, 0XF8, 0XF8, 0X17, 0XD, 0XE, 0X6F, 0XE,
0X65, 0X8, 0X66, 0X7, 0X33, 0X8, 0X65, 0XC, 0X8, 0X65, 0X7, 0X64, 0X8, 0X65, 0X7, 0X7,
0X1, 0X1, 0X1, 0X1, 0X5, 0X14, 0X8, 0XD8, 0XD8, 0X23, 0XD8, 0X22, 0XD6, 0XD6, 0X7, 0XE8,
0X7, 0X15, 0XD6, 0XD5, 0XD5, 0XD6, 0XD6, 0X5, 0XD6, 0X22, 0XD6, 0X23, 0XFA, 0X17, 0X1D, 0XE,
0X6F, 0X5F, 0X6F, 0X1D, 0X16, 0X17, 0XF8, 0X17, 0XF8, 0X17, 0XE8, 0X17, 0X6F, 0XE, 0X4F, 0X6F,
0X64, 0X34, 0X77, 0X66, 0X65, 0X33, 0X65, 0X8, 0X66, 0X33, 0X65, 0X33, 0X65, 0X34, 0X65, 0X74,
0X1, 0X1, 0X1, 0X1, 0XD0, 0XD5, 0XD6, 0X7, 0X8, 0XE8, 0X8, 0XD8, 0X21, 0XE2, 0X10, 0X4,
0XD5, 0XD4, 0X20, 0XD4, 0X23, 0XD7, 0X24, 0XD7, 0X14, 0X6, 0XD6, 0X16, 0XFA, 0XF8, 0XF8, 0X1E,
0XE, 0X1F, 0X17, 0XE8, 0XE8, 0XF8, 0X17, 0XFA, 0XF7, 0XFA, 0XF7, 0XE, 0X5F, 0XD, 0X6F, 0XD,
0X65, 0X7, 0X66, 0X34, 0X7, 0X65, 0X8, 0X66, 0X33, 0X65, 0X7, 0X65, 0X7, 0X65, 0X7, 0X64,
0XD0, 0X1, 0X2, 0X1, 0X1, 0X1, 0X20, 0X14, 0XD8, 0X7, 0XE8, 0X7, 0XE8, 0X8, 0XD7, 0X15,
0X7, 0XE7, 0X7, 0XD8, 0X7, 0XD8, 0X14, 0X6, 0XD6, 0X20, 0XD4, 0XE0, 0X17, 0X18, 0XFA, 0X17,
0XFC, 0X16, 0XFA, 0X17, 0XF8, 0X17, 0XF8, 0XF7, 0X17, 0XF8, 0X17, 0XE, 0X6F, 0XE, 0X5F, 0XE,
0X65, 0X34, 0X76, 0X8, 0X66, 0X33, 0X65, 0X34, 0X78, 0X34, 0X78, 0X33, 0X65, 0X7, 0X30, 0X5,
0X70, 0X1, 0X2, 0X2, 0X2, 0X1, 0XC0, 0X2, 0X20, 0XD7, 0X23, 0XD7, 0X6, 0XE7, 0X7, 0XD8,
0XD7, 0X15, 0XD7, 0X15, 0XD6, 0X22, 0XD6, 0X4, 0X13, 0XD4, 0X1, 0X1, 0X15, 0XF8, 0XF8, 0XF8,
0XF8, 0XF8, 0XF8, 0XF8, 0X17, 0XF8, 0X17, 0XFA, 0XF7, 0XFA, 0XD, 0X5F, 0XD, 0X6F, 0X6E, 0XE,
0X9, 0X65, 0X34, 0X66, 0X7, 0X66, 0X7, 0X65, 0X7, 0X74, 0X4, 0X4, 0X3, 0X2, 0X2, 0X1,
0X1, 0X2, 0X1, 0X2, 0X2, 0X2, 0X2, 0X1, 0X1, 0XD2, 0XD4, 0X20, 0XD5, 0X10, 0X5, 0X13,
0X5, 0XD5, 0X20, 0X5, 0X10, 0XD4, 0XE0, 0XD3, 0X2, 0X1, 0X1, 0X1, 0XD0, 0X17, 0XFA, 0X17,
0XF9, 0X17, 0XF8, 0XE8, 0XF8, 0XFA, 0XF7, 0X17, 0XF8, 0X17, 0X6F, 0XE, 0X6F, 0X4F, 0XE, 0XE,
0X65, 0X7, 0X66, 0X7, 0X64, 0X6, 0X30, 0X73, 0X3, 0X3, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2,
0X1, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X1, 0XD1, 0XD2, 0XD2, 0XE1, 0XE1,
0X10, 0XD4, 0XE1, 0XD4, 0XD3, 0X20, 0X2, 0XD0, 0X1, 0X1, 0X1, 0X1, 0X1, 0X13, 0X16, 0XF8,
0XFA, 0XF8, 0X17, 0XF8, 0X17, 0XF8, 0X17, 0XFA, 0XE7, 0XD, 0X6F, 0XE, 0X6E, 0XE, 0X4F, 0XE,
0X33, 0X76, 0X30, 0X2, 0X3, 0X70, 0X2, 0X2, 0X2, 0X2, 0X2, 0X3, 0X70, 0X3, 0X2, 0X1,
0X1, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X1, 0X1, 0X1, 0X2, 0XD2,
0XD2, 0XE0, 0XD2, 0XE0, 0X4, 0X9, 0X2, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X3, 0X2,
0X3, 0X20, 0XD5, 0X16, 0XF8, 0XFA, 0XF7, 0XF8, 0X18, 0X6F, 0XE, 0X5F, 0XE, 0XE, 0X6E, 0XE,
0X83, 0XD2, 0X2, 0X2, 0X2, 0X2, 0X3, 0X2, 0X2, 0X2, 0X2, 0X3, 0X3, 0X2, 0X2, 0X2,
0X1, 0X2, 0X2, 0X2, 0X2, 0X2, 0X3, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X3, 0X9,
0X7, 0X8, 0X8, 0XFC, 0XA, 0XD2, 0X2, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X3,
0X2, 0X2, 0X2, 0X3, 0X4, 0X13, 0X17, 0XF8, 0XD, 0X5F, 0XE, 0X6F, 0X6E, 0X5F, 0XE, 0XE,
0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2,
0X2, 0X2, 0X1, 0X70, 0X2, 0X3, 0X2, 0X3, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X3,
0X8, 0XFB, 0XD8, 0X9, 0X3, 0X2, 0X1, 0X2, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X2,
0X3, 0X3, 0X3, 0X2, 0X2, 0X3, 0X4, 0X14, 0X6F, 0XE, 0X6F, 0XE, 0XE, 0XE, 0X4F, 0X6F,
0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X3, 0X2, 0X2, 0X2, 0X2, 0X2,
0X2, 0X2, 0X2, 0X1, 0X3, 0X2, 0X3, 0X3, 0X70, 0X4, 0X2, 0X2, 0X70, 0X3, 0X2, 0XD1,
0X2, 0X5, 0X9, 0X3, 0X1, 0X2, 0X1, 0X2, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X2,
0X2, 0X3, 0X2, 0X3, 0X2, 0X2, 0X3, 0X3, 0X4, 0X5, 0X7, 0XE, 0X4F, 0X6E, 0XE, 0X6E,
0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X3, 0X2, 0X71, 0X3, 0X2, 0X2, 0X2, 0X70,
0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X3, 0X3, 0X2, 0X3, 0X3, 0X3, 0X2, 0X2, 0X2,
0X3, 0X4, 0X20, 0X1, 0X2, 0X2, 0X1, 0X2, 0X1, 0X2, 0X1, 0X1, 0X1, 0X1, 0X1, 0X2,
0X2, 0X3, 0X2, 0X2, 0X3, 0X3, 0X3, 0X83, 0X3, 0X2, 0X2, 0X2, 0X6, 0XD, 0X5F, 0X6F,
0X2, 0X2, 0X2, 0X3, 0X2, 0X2, 0X2, 0X2, 0X2, 0X3, 0X3, 0X3, 0X2, 0X2, 0X3, 0X2,
0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X3, 0X2, 0X3, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2,
0X2, 0X2, 0X2, 0X1, 0X1, 0X2, 0X1, 0X2, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X2, 0X2,
0X3, 0X2, 0X3, 0X2, 0X2, 0X2, 0X2, 0X3, 0X2, 0X2, 0X2, 0X2, 0X70, 0X3, 0X5, 0XE,
0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X70, 0X3, 0X2, 0X3, 0X2, 0X2, 0X2, 0X2, 0X2,
0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X3, 0X2, 0X3, 0X2, 0X3, 0X2, 0X2, 0X2,
0X1, 0X1, 0X2, 0X1, 0X1, 0X2, 0X1, 0X2, 0X1, 0X2, 0X1, 0X70, 0X1, 0X1, 0X2, 0X2,
0X2, 0X2, 0X3, 0X2, 0X3, 0X2, 0X2, 0X2, 0X2, 0X2, 0X3, 0X2, 0X3, 0X3, 0X2, 0X3,
0X2, 0X2, 0X3, 0X2, 0X2, 0X2, 0X3, 0X3, 0X2, 0X2, 0X2, 0X2, 0X3, 0X2, 0X2, 0X3,
0X70, 0X3, 0X2, 0X3, 0X2, 0X70, 0X1, 0X2, 0X3, 0X2, 0X3, 0X70, 0X3, 0X3, 0X2, 0X2,
0X1, 0X1, 0X1, 0X3, 0X1, 0X1, 0X1, 0X2, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X2, 0X2,
0X3, 0X2, 0X70, 0X3, 0X2, 0X3, 0X2, 0X3, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2,
0X2, 0X3, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X3, 0X2,
0X3, 0X2, 0X2, 0X70, 0X3, 0X3, 0X3, 0X1, 0X2, 0X70, 0X3, 0X3, 0X3, 0X2, 0X2, 0X2,
0X2, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X70, 0X1, 0X1, 0X1, 0X1, 0X2, 0X2, 0X2,
0X3, 0X2, 0X3, 0X2, 0X2, 0X2, 0X3, 0X2, 0X70, 0X3, 0X3, 0X2, 0X2, 0X2, 0X2, 0X3,
0X2, 0X2, 0X2, 0X2, 0X70, 0X3, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X3, 0X2, 0X70, 0X3,
0X2, 0X2, 0X3, 0X3, 0X3, 0X2, 0X2, 0X2, 0X1, 0X3, 0X3, 0X2, 0X2, 0X3, 0X2, 0X1,
0X2, 0X2, 0X2, 0X1, 0X2, 0X2, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X1, 0X2, 0X2, 0X3,
0X70, 0X3, 0X3, 0X3, 0X2, 0X3, 0X70, 0X3, 0X3, 0X3, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2,
0X2, 0X2, 0X2, 0X3, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X70, 0X3, 0X3, 0X3, 0X3,
0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X3, 0X2, 0X2, 0X1, 0X2, 0X2, 0X2, 0X2, 0X1, 0X70,
0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X1, 0X1, 0X1, 0X1, 0X1, 0X2, 0X2, 0X3, 0X2,
0X3, 0X3, 0X2, 0X2, 0X3, 0X2, 0X3, 0X3, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X3,
0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X3, 0X2, 0X2, 0X2, 0X2,
0X3, 0X4, 0X2, 0X2, 0X3, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X1, 0X1, 0X1, 0X1, 0X1,
0X2, 0X2, 0X3, 0X2, 0X2, 0X2, 0X2, 0X2, 0X1, 0X1, 0X1, 0X2, 0X3, 0X2, 0X3, 0X2,
0X2, 0X3, 0X2, 0X3, 0X2, 0X3, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2,
0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2,
0X2, 0X2, 0X2, 0X2, 0X3, 0X2, 0X3, 0X2, 0X70, 0X3, 0X2, 0X2, 0X2, 0X2, 0X2, 0X1,
0X2, 0X2, 0X2, 0X70, 0X3, 0X2, 0X2, 0X70, 0X3, 0X2, 0X2, 0X2, 0X2, 0X3, 0X2, 0X3,
0X2, 0X3, 0X2, 0X2, 0X70, 0X3, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2,
0X2, 0X3, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X3, 0X2, 0X2,
0X3, 0X2, 0X3, 0X70, 0X3, 0X2, 0X70, 0X3, 0X3, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2,
0X2, 0X2, 0X3, 0X3, 0X2, 0X3, 0X3, 0X3, 0X2, 0X2, 0X3, 0X2, 0X3, 0X70, 0X3, 0X2,
0X3, 0X2, 0X3, 0X3, 0X3, 0X3, 0X2, 0X3, 0X2, 0X2, 0X70, 0X3, 0X2, 0X2, 0X2, 0X2,
0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X70, 0X3, 0X2, 0X2, 0X70,
0X3, 0X2, 0X3, 0X3, 0X3, 0X3, 0X3, 0X2, 0X3, 0X2, 0X2, 0X2, 0X2, 0X70, 0X3, 0X2,
0X2, 0X2, 0X2, 0X3, 0X2, 0X2, 0X2, 0X2, 0X3, 0X2, 0X2, 0X3, 0X2, 0X3, 0X2, 0X3,
0X2, 0X70, 0X3, 0X2, 0X2, 0X2, 0X3, 0X2, 0X2, 0X3, 0X3, 0X2, 0X2, 0X2, 0X2, 0X2,
0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X70, 0X2, 0X3, 0X3, 0X2, 0X3, 0X3,
0X2, 0X2, 0X3, 0X2, 0X2, 0X3, 0X3, 0X2, 0X2, 0X2, 0X3, 0X2, 0X3, 0X3, 0X2, 0X2,
0X2, 0X2, 0X3, 0X2, 0X3, 0X3, 0X3, 0X2, 0X2, 0X3, 0X3, 0X2, 0X3, 0X2, 0X3, 0X2,
0X3, 0X3, 0X3, 0X2, 0X3, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2,
0X2, 0X2, 0X2, 0X2, 0X2, 0X70, 0X2, 0X2, 0X1, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2, 0X2,
0X3, 0X2, 0X2, 0X3, 0X2, 0X3, 0X2, 0X3, 0X3, 0X2, 0X2, 0X2, 0X2, 0X2, 0X3, 0X2,
0X2, 0X2, 0X3, 0X70, 0X3, 0X2, 0X2, 0X3, 0X70, 0X3, 0X2, 0X3, 0X2, 0X3, 0X2, 0X3,
0X2, 0X2, 0X2, 0X3, 0X2, 0X3, 0X3, 0X2, 0X70, 0X3, 0X2, 0X2, 0X2, 0X2, 0X1, 0X2, };
///////////////////