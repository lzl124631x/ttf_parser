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
    USHORT    max_points;                 // Maximum points in a non-composite glyph. (From 'maxp')
    USHORT    max_contours;               // Maximum contours in a non-composite glyph. (From 'maxp')
    USHORT    max_size_of_instructions;   // Maximum byte count for glyph instructions. (From 'maxp')

    void      *data;                      // The whole chunk of data in glyph data table.
    size_t    length;                     // Length of data in bytes.

    void load_table(Table_Directory_Entry *entry, ifstream &fin, USHORT max_p, USHORT max_c, USHORT max_instr);
    ~Glyph_Data(){
      delete[] data;
      data = NULL;
    }
    void dump_info(FILE *fp, size_t indent);
    Glyph *load_glyph(USHORT offset);
    void clear();
  };

  class DLL_API Glyph{
  public:
    SHORT  number_of_contours;
    // If the 'number_of_contours' is greater than or equal to zero, this is a single glyph;
    // if negative, this is a composite glyph.
    // FIXME: There is a discrepancy in this spec.
    // The spec also says the numberOfContours of Simple Glyph is greater than zero,
    // while that of Composite Glyph is -1.
    FWORD  x_min;
    FWORD  y_min;
    FWORD  x_max;
    FWORD  y_max;

    Glyph();
    void load_glyph_header(Mem_Stream &msm);
    virtual Glyph *load_glyph(Mem_Stream &msm) = 0;
    virtual void dump_coordinates() = 0;
    virtual void dump_svg_outline(FILE *fp) = 0;
    virtual void glyph_to_path(GraphicsPath &path) = 0;
    virtual void dump_info(FILE *fp, size_t indent) = 0;
  };

  class DLL_API Simple_Glyph_Description : public Glyph{
  private:
    void read_flags(Mem_Stream &msm);
    void read_coordinates(Mem_Stream &msm, SHORT *ptr, bool read_x);
    void dump_flags();
  public:
    USHORT  *end_pts_of_contours;       // Array of last points of each contour; its length is the number of contours.
    USHORT  instruction_length;
    BYTE    *instructions;              // [instruction_length]
    BYTE    *flags;                     // [variable_length]
    SHORT   *x_coordinates;             // [number_of_points]
    SHORT   *y_coordinates;             // [number_of_points]
    // Actually the type of coordinates is either BYTE or SHORT.
    // Each coordinate might has different type according to the corresponding flag.
    // Here I choose to store all the coordinates in a SHORT array.
    // FIXME: Reading glyph data is the culprit of slow parsing speed. Consider read right before rendering.
    USHORT  pt_num;                     // the number of points in this glyph
    // Points are indexed from 0. end_pts_of_contours stores the index of each contour's end point.
    // The last contour's end point has the largest index which equals pt_num - 1.
    
    Simple_Glyph_Description();
    ~Simple_Glyph_Description(){}       // FIXME: destructor should be private
    void init(USHORT max_contours, USHORT max_points, USHORT max_size_of_instructions);
    void Simple_Glyph_Description::destroy();
    Glyph *load_glyph(Mem_Stream &msm);
    void dump_coordinates();
    void dump_svg_outline(FILE *fp);
    void glyph_to_path(GraphicsPath &path);
    void dump_info(FILE *fp, size_t indent);
  };
  
  class Composite_Glyph_Description : public Glyph{
  public:
    USHORT      flags;                  // component flag
    USHORT      glyph_index;            // glyph index of component
    SHORT       argument1;              // x-offset for component or point number; type depends on bits 0 and 1 in component flags
    SHORT       argument2;              // y-offset for component or point number; type depends on bits 0 and 1 in component flags
    F2DOT14     x_scale;
    F2DOT14     scale01;
    F2DOT14     scale10;
    F2DOT14     y_scale;
    USHORT      number_of_instructions;
    BYTE        *instructions;
    // Some transformation options might follow.

    Composite_Glyph_Description();
    ~Composite_Glyph_Description(){}
    Glyph *load_glyph(Mem_Stream &msm);
    void dump_coordinates(){}
    void dump_svg_outline(FILE *fp){}
    void glyph_to_path(GraphicsPath &path){}
    void dump_info(FILE *fp, size_t indent);
  };
}
#endif