#ifndef TTF_PARSER_GDIPLUS
#define TTF_PARSER_GDIPLUS

void render_glyph(HDC hdc, HBITMAP bmp, ttf_dll::USHORT ch, size_t width, size_t height, bool render_point);
#endif