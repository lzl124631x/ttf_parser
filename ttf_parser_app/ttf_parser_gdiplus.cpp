#include "stdafx.h"
using namespace ttf_dll;

void render_glyph(HDC hdc, HBITMAP bmp, Glyph *glyph, size_t width, size_t height, bool render_point){
  if(!glyph) return;

  FWORD glyph_width = ttf.head.x_max - ttf.head.x_min;
  FWORD glyph_height = ttf.head.y_max - ttf.head.y_min;
  float x_ratio = 1.0f * width / glyph_width;
  float y_ratio = 1.0f * height / glyph_height;

  HDC memdc = CreateCompatibleDC(hdc);
  HBITMAP old_bmp = (HBITMAP)SelectObject(memdc, bmp);
  Graphics g(memdc);
  g.Clear(Color::White);
  GraphicsPath path;
  glyph->glyph_to_path(path);

  // perform transformation according to the glyph metrics
  Matrix matrix(1, 0, 0, -1, 0, 0); // flip (x, y) to (x, -y)
  matrix.Translate((float)-ttf.head.x_min, (float)ttf.head.y_max, MatrixOrderAppend); // translate with (-xMin, yMax)
  matrix.Scale(x_ratio, y_ratio, MatrixOrderAppend); // scale with (xRatio, yRatio)
  g.SetTransform(&matrix);
  // render bound
  // FIXME: ugly code!
  g.DrawLine(&Pen(Color(50, 0, 0, 0), 1), (float)glyph->header.x_min, (float)ttf.head.y_max, (float)glyph->header.x_min, (float)-(height / y_ratio - ttf.head.y_max));
  g.DrawLine(&Pen(Color(50, 0, 0, 0), 1), (float)glyph->header.x_max, (float)ttf.head.y_max, (float)glyph->header.x_max, (float)-(height / y_ratio - ttf.head.y_max));
  g.DrawLine(&Pen(Color(50, 0, 0, 0), 1), (float)ttf.head.x_min, (float)glyph->header.y_min, (float)width / x_ratio + ttf.head.x_min, (float)glyph->header.y_min);
  g.DrawLine(&Pen(Color(50, 0, 0, 0), 1), (float)ttf.head.x_min, (float)glyph->header.y_max, (float)width / x_ratio + ttf.head.x_min, (float)glyph->header.y_max);
  // render glyph
  g.SetSmoothingMode(SmoothingModeHighQuality);
  g.FillPath(&SolidBrush(Color::Red), &path);
  g.DrawPath(&Pen(Color::Red, 1), &path);

  // render points
  if(render_point){
    float radius = 1.0f / x_ratio;
    if(radius < 1e-6) radius = 1.0;
    float pen_width = radius;
    if(pen_width - 0.5 > 1e-6) pen_width = 0.5;
    Pen on_pen(Color(200, 0, 255, 0), pen_width);
    Pen off_pen(Color(200, 0, 0, 255), pen_width);

    int all_pt_num = 0, off_pt_num = 0;
    glyph->count_pt_num(all_pt_num, off_pt_num);
    PointF *all_pt = new PointF[all_pt_num];
    int *off_pt = new int[off_pt_num];
    glyph->output_pts(all_pt, off_pt);
    PointF *pt = all_pt;
    for(int i = 0, j = 0; i < all_pt_num; ++i, ++pt){
      if(i == off_pt[j]){   // render off-curve point
        g.DrawRectangle(&off_pen, pt->X - radius, pt->Y - radius, radius * 2.0f, radius * 2.0f);
        ++j;
      }else{                // render on-curve point
        g.DrawEllipse(&on_pen, pt->X - radius, pt->Y - radius, radius * 2.0f, radius * 2.0f);
      }
    }
    DEL_A(all_pt);
    DEL_A(off_pt);
  }

  // copy hdc
  BitBlt(hdc, 0, 0, width, height, memdc, 0, 0, SRCCOPY);
  SelectObject(memdc, old_bmp);
  DeleteDC(memdc);
}