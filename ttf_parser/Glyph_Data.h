#ifndef GLYPH_DATA_H
#define GLYPH_DATA_H
#include "Type.h"
#include "TTF_Table.h"
#include "Mem_Stream.h"
#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")

namespace ttf_dll{
/************************************************************************/
/*                         glyf - Glyf Data                             */
/* Spec: https://www.microsoft.com/typography/otspec/glyf.htm           */
/************************************************************************/
  class Glyph;

  class DLL_API Glyph_Data{
  public:
    void      *data;                      // The whole chunk of data in glyph data table.
    size_t    length;                     // Length of data in bytes.

    void load_table(Table_Directory_Entry *entry, ifstream &fin);
    ~Glyph_Data(){
      DEL_A(data);
    }
    void dump_info(FILE *fp, size_t indent);
    Glyph *load_glyph(USHORT glyph_index);
    void clear();
  };

  class Glyph_Loader{
  public:
    USHORT  *end_contours;       // Array of last points of each contour; its length is the number of contours.
    // FIXME: Currently I don't care about instructions
    BYTE    *flags;                     // [number_of_points]
    SHORT   *x_coordinates;             // [number_of_points]
    SHORT   *y_coordinates;             // [number_of_points]
    // Actually the type of coordinates is either BYTE or SHORT.
    // Each coordinate might has different type according to the corresponding flag.
    // Here I choose to store all the coordinates in a SHORT array.
    USHORT  pt_num;                     // the number of points in this glyph
    // Points are indexed from 0. end_pts_of_contours stores the index of each contour's end point.
    // The last contour's end point has the largest index which equals pt_num - 1.
    Glyph_Loader(Glyph &glyph){
      end_contours = glyph.end_contours;
      flags = glyph.flags;
      x_coordinates = glyph.x_coordinates;
      y_coordinates = glyph.y_coordinates;
    }
	void load_glyph(Mem_Stream &msm, USHORT glyph_index);
  void load_simple_glyph(Mem_Stream &msm);
  void load_composite_glyph(Mem_Stream &msm);
  };

  class DLL_API Glyph_Header{
  public:
	  SHORT  num_contours;
	  // If the 'number_of_contours' is greater than zero, this is a single glyph;
	  // if that field equals -1, this is a composite glyph.
	  FWORD  x_min;
	  FWORD  y_min;
	  FWORD  x_max;
	  FWORD  y_max;
	  bool loaded;
	  void init();
	  bool load_glyph_header(Mem_Stream &msm);
  };

  bool is_simply_glyph(SHORT num_contours){ return num_contours > 0; }
  bool is_composite_glyph(SHORT num_contours){ return num_contours == -1; }

  class DLL_API Glyph{
  public:
	Glyph_Header header;

    USHORT  *end_contours;       // Array of last points of each contour; its length is the number of contours.
    USHORT  instr_len;
    BYTE    *flags;                     // [number_of_points]
    SHORT   *x_coordinates;             // [number_of_points]
    SHORT   *y_coordinates;             // [number_of_points]
    // Actually the type of coordinates is either BYTE or SHORT.
    // Each coordinate might has different type according to the corresponding flag.
    // Here I choose to store all the coordinates in a SHORT array.
    USHORT  pt_num;                     // the number of points in this glyph
    // Points are indexed from 0. end_pts_of_contours stores the index of each contour's end point.
    // The last contour's end point has the largest index which equals pt_num - 1.

    void init();
    Glyph *Glyph::load(Mem_Stream &msm, Glyph_Loader &loader);
    Glyph *load_simple_glyph(Mem_Stream &msm, Glyph_Loader &loader);
    Glyph *load_composite_glyph(Mem_Stream &msm, Glyph_Loader &loader);
    bool load_glyph_header(Mem_Stream &msm);
    Glyph *load_glyph(Mem_Stream &msm);
    void dump_coordinates();
    void dump_svg_outline(FILE *fp);
    void glyph_to_path(GraphicsPath &path);
    void dump_info(FILE *fp, size_t indent);
    void count_pt_num(int &all_pt_num, int &off_pt_num);
    void output_pts(PointF *all_pt, int *off_pt);
  };
}
#endif