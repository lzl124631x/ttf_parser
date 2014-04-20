#include "stdafx.h"
using namespace ttf_dll;

// FIXME: the following definition should not be defined here.
#define ON_CURVE			0x1 // FIXME: the following code is in test mode.
void render_glyph(HDC hdc, HBITMAP bmp, ttf_dll::USHORT ch, size_t width, size_t height){
	ttf_dll::USHORT glyph_index = ttf.cmap.get_glyph_index(Windows, Unicode_BMP, ch);
	Glyph *glyph = ttf.load_glyph(glyph_index);   // FIXME: the 'get_glyph' function should be member of ttf.
  if(!glyph){ // Error: no such glyph.
    return;
  }

	FWORD glyph_width = ttf.head.x_max - ttf.head.x_min;
	FWORD glyph_height = ttf.head.y_max - ttf.head.y_min;
	double x_ratio = 1.0 * width / glyph_width;
	double y_ratio = 1.0 * height / glyph_height;

  HDC memdc = CreateCompatibleDC(hdc);
  HBITMAP old_bmp = (HBITMAP)SelectObject(memdc, bmp);
	Graphics g(memdc);
	g.Clear(Color::White);
	GraphicsPath path;
	glyph->glyph_to_path(path);

	// perform transformation according to the glyph metrics
	Matrix matrix(1, 0, 0, -1, 0, 0); // flip (x, y) to (x, -y)
	matrix.Translate(-ttf.head.x_min, ttf.head.y_max, MatrixOrderAppend); // translate with (-xMin, yMax)
	matrix.Scale(x_ratio, y_ratio, MatrixOrderAppend); // scale with (xRatio, yRatio)
	g.SetTransform(&matrix);
	// render bound
	// FIXME: ugly code!
	g.DrawLine(&Pen(Color(50, 0, 0, 0), 1), glyph->x_min, ttf.head.y_max, glyph->x_min, -(height / y_ratio - ttf.head.y_max));
	g.DrawLine(&Pen(Color(50, 0, 0, 0), 1), glyph->x_max, ttf.head.y_max, glyph->x_max, -(height / y_ratio - ttf.head.y_max));
	g.DrawLine(&Pen(Color(50, 0, 0, 0), 1), ttf.head.x_min, glyph->y_min, width / x_ratio + ttf.head.x_min, glyph->y_min);
	g.DrawLine(&Pen(Color(50, 0, 0, 0), 1), ttf.head.x_min, glyph->y_max, width / x_ratio + ttf.head.x_min, glyph->y_max);
	// render glyph
	g.SetSmoothingMode(SmoothingModeHighQuality);
	g.FillPath(&SolidBrush(Color::Red), &path);
	g.DrawPath(&Pen(Color::Red, 1), &path);

  // render hint
  int all_pt_num = 0, off_pt_num = 0;
  glyph->count_pt_num(all_pt_num, off_pt_num);
  PointF *all_pt = new PointF[all_pt_num];
  int *off_pt = new int[off_pt_num];
  glyph->output_pts(all_pt, off_pt);
  PointF *pt = all_pt;
  for(int i = 0; i < all_pt_num; ++i, ++pt){
    g.FillEllipse(&SolidBrush(Color(80, 0, 0, 0)), pt->X, pt->Y, 15.0, 15.0);
  }
  DEL_A(all_pt);
  DEL_A(off_pt);

	BitBlt(hdc, 0, 0, width, height, memdc, 0, 0, SRCCOPY);
	SelectObject(memdc, old_bmp);
	DeleteDC(memdc);
}