#define grid_width 8
#define grid_height 11
static unsigned char grid_bits[] U8X8_PROGMEM = {
    0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55};

#define tm_logo_width 96
#define tm_logo_height 68
static unsigned char tm_logo_bits[] U8X8_PROGMEM = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x80, 0xff, 0x3f, 0x1e, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x14, 0x00,
    0x80, 0xff, 0x3f, 0x1e, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x1e, 0x00,
    0x80, 0xff, 0x3f, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x1c, 0x00,
    0x00, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x1e, 0x00,
    0x00, 0xf0, 0x01, 0x1e, 0xe0, 0x0f, 0x0f, 0x1e, 0xf0, 0x03, 0xff, 0x01,
    0x00, 0xf0, 0x00, 0x1e, 0xf8, 0x1f, 0x0f, 0x0f, 0xfc, 0x0f, 0xff, 0x01,
    0x00, 0xf0, 0x00, 0x1e, 0xfc, 0x1e, 0x8f, 0x0f, 0x1e, 0x1f, 0xff, 0x01,
    0x00, 0xf0, 0x00, 0x1e, 0x3e, 0x10, 0xcf, 0x07, 0x0e, 0x1e, 0x1e, 0x00,
    0x00, 0xf0, 0x00, 0x1e, 0x1e, 0x00, 0xcf, 0x03, 0x0f, 0x1c, 0x1e, 0x00,
    0x00, 0xf0, 0x01, 0x1e, 0x1e, 0x00, 0xff, 0x01, 0xff, 0x3f, 0x1c, 0x00,
    0x00, 0xf0, 0x00, 0x1e, 0x1e, 0x00, 0xff, 0x01, 0xff, 0x1f, 0x1e, 0x00,
    0x00, 0xf0, 0x00, 0x1e, 0x1e, 0x00, 0xff, 0x03, 0x0f, 0x00, 0x1c, 0x00,
    0x00, 0xf0, 0x00, 0x1e, 0x1e, 0x00, 0xcf, 0x07, 0x0f, 0x00, 0x1e, 0x00,
    0x00, 0xf0, 0x00, 0x1e, 0x3e, 0x10, 0x8f, 0x07, 0x1f, 0x10, 0x1c, 0x00,
    0x00, 0xf0, 0x00, 0x1e, 0x7c, 0x18, 0x0f, 0x0f, 0xbe, 0x18, 0x3c, 0x00,
    0x00, 0xf0, 0x01, 0x1e, 0xfc, 0x1f, 0x0f, 0x1f, 0xfc, 0x1f, 0xfc, 0x01,
    0x00, 0xf0, 0x00, 0x1e, 0xf0, 0x0f, 0x0f, 0x1e, 0xf8, 0x1f, 0xf8, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x80, 0x02, 0x00, 0x00, 0x40, 0x01, 0x20, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x50, 0x01, 0x2a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xf0, 0x01, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00,
    0xf0, 0x03, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00,
    0xf0, 0x83, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00,
    0xf8, 0x83, 0x3f, 0xe0, 0x05, 0x40, 0x03, 0x3e, 0x00, 0x2e, 0x00, 0x00,
    0xf0, 0xc7, 0x1f, 0xf8, 0x1f, 0xf0, 0x1f, 0xff, 0x83, 0x7f, 0xe0, 0x39,
    0xf0, 0xc7, 0x3f, 0xf8, 0x3f, 0xf8, 0x3f, 0xff, 0xc1, 0xfb, 0xe1, 0x7f,
    0xf0, 0xee, 0x1c, 0x00, 0x3c, 0x3c, 0x10, 0xbc, 0xe0, 0xe0, 0xe1, 0x3f,
    0x78, 0xee, 0x3e, 0x00, 0x3c, 0x3c, 0x00, 0x3c, 0xf0, 0xc0, 0xe1, 0x03,
    0xf0, 0xfe, 0x3e, 0x80, 0x3e, 0xfc, 0x01, 0x3c, 0xf0, 0xe4, 0xe1, 0x01,
    0x70, 0x7c, 0x1c, 0xf8, 0x3f, 0xf8, 0x0f, 0x3c, 0xf0, 0xff, 0xe3, 0x01,
    0x70, 0x7c, 0x3e, 0x7c, 0x3d, 0xf0, 0x3f, 0x3c, 0xf0, 0xff, 0xe1, 0x01,
    0xf8, 0x38, 0x1c, 0x3c, 0x38, 0xc0, 0x3f, 0x1c, 0xf0, 0x00, 0xe0, 0x01,
    0x70, 0x10, 0x3e, 0x1c, 0x38, 0x00, 0x3c, 0x3c, 0xf0, 0x00, 0xe0, 0x01,
    0xf0, 0x00, 0x1c, 0x3e, 0x3c, 0x0c, 0x3c, 0x3c, 0xe0, 0x01, 0xe1, 0x01,
    0x70, 0x00, 0x3e, 0x7c, 0x7f, 0x7c, 0x3f, 0xfc, 0xe3, 0xef, 0xe1, 0x01,
    0xf8, 0x00, 0x1c, 0xfc, 0x3b, 0xfc, 0x1f, 0xf8, 0x81, 0xff, 0xe1, 0x01,
    0x20, 0x00, 0x1a, 0xf0, 0x28, 0xf0, 0x03, 0xf0, 0x01, 0xfe, 0x40, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};