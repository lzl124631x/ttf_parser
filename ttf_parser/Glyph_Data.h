#ifndef GLYPH_DATA_H
#define GLYPH_DATA_H
#include "Type.h"
#include "TTF_Table.h"
#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")
/******************************* glyf ***********************************/
// https://www.microsoft.com/typography/otspec/glyf.htm
namespace ttf_dll{
  class DLL_API Glyph_Data{ // FIXME: consider to change this class into a pure vitual class
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
    Glyph_Data(ifstream &fin, ULONG offset);
    static Glyph_Data* create_glyph_data(ifstream &fin, ULONG offset, USHORT max_contours);
    virtual ~Glyph_Data(){};
    static bool is_simply_glyph(SHORT number_of_contours){ return number_of_contours >= 0; } // FIXME: does this function need to be member function?
    virtual void dump_coordinates() = 0;
    virtual void dump_svg_outline(FILE *fp) = 0;
    virtual void glyph_to_path(GraphicsPath &path) = 0;
    virtual void dump_info(FILE *fp, size_t indent) = 0;
  };

  class DLL_API Simple_Glyph_Description : public Glyph_Data{
  private:
    void read_flags(ifstream &fin);
    void read_coordinates(ifstream& fin, SHORT *ptr, bool read_x);
    void dump_flags();
  public:
    USHORT  *end_pts_of_contours;
    USHORT  instruction_length;
    BYTE    *instructions;              // [instruction_length]
    BYTE    *flags;                     // [number_of_points]
    SHORT   *x_coordinates;             // [number_of_points]
    SHORT   *y_coordinates;             // [number_of_points]
    // Actually the type of coordinates is either BYTE or SHORT.
    // Each coordinate might has different type according to the corresponding flag.
    // Here I choose to store all the coordinates in a SHORT array.
    // FIXME: Reading glyph data is the culprit of slow parsing speed. Consider read right before rendering.
    USHORT  pt_num;                     // the number of points in this glyph
    // Points are indexed from 0. end_pts_of_contours stores the index of each contour's end point.
    // The last contour's end point has the largest index which equals pt_num - 1.
    Simple_Glyph_Description(ifstream &fin, ULONG offset);
    ~Simple_Glyph_Description(){
      delete[] end_pts_of_contours;
      delete[] instructions;
      delete[] flags;
      delete[] x_coordinates;
      delete[] y_coordinates;
    }
    void dump_coordinates();
    void dump_svg_outline(FILE *fp);
    void glyph_to_path(GraphicsPath &path);
    void dump_info(FILE *fp, size_t indent);
  };
  
  class Composite_Glyph_Description : public Glyph_Data{
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
    Composite_Glyph_Description(ifstream &fin, ULONG offset);
    ~Composite_Glyph_Description(){
      delete[] instructions;
    }
    void dump_coordinates(){}
    void dump_svg_outline(FILE *fp){}
    void glyph_to_path(GraphicsPath &path){}
    void dump_info(FILE *fp, size_t indent);
  };
}
#endif