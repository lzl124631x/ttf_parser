#ifndef TTF_PARSER_GDIPLUS
#define TTF_PARSER_GDIPLUS

void RenderGlyph(HDC hdc, HBITMAP bmp, ttf_dll::Glyph *glyph, size_t width, size_t height, bool render_point);

#endif