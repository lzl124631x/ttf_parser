#ifndef GLYPH_DATA_H
#define GLYPH_DATA_H
#include "Type.h"
#include "TTF_Table.h"
#include "Mem_Stream.h"
#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")

namespace ttf_dll {
/****************************************************************************/
/*                         glyf - Glyf Data                                 */
/* Spec: https://www.microsoft.com/typography/otspec/glyf.htm               */
/****************************************************************************/
class Glyph;
class Glyph_Loader;

class DLL_API Glyph_Data {
 public:
  ~Glyph_Data();
  // Reads the table from the file stream. The `entry` provides some
  // information needed for loading.
  void load_table(Table_Record_Entry *entry, ifstream &fin);
  // Dumps the information of this table to an XML file.
  void dump_info(Xml_Logger &logger) const;
  // Loads the glyph labeled by `glyph_index`.
  Glyph *load_glyph(const GLYPH_ID glyph_index);

  // The whole chunk of data in glyph data table.
  void      *data;
  // Length of data in bytes.
  size_t    length;
};

class DLL_API Glyph_Header {
 public:
  void reset();
  bool load_glyph_header(Mem_Stream &msm);

  SHORT  num_contours;
  // If the 'number_of_contours' is greater than zero, this is a single glyph;
  // if that field equals -1, this is a composite glyph.
  FWORD  x_min;
  FWORD  y_min;
  FWORD  x_max;
  FWORD  y_max;
};

class DLL_API Glyph {
 public:
  void prepare();
  void destroy();
  void reset();
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

  Glyph_Header    header;
  bool            root;
  GLYPH_ID        glyph_index;

  USHORT          *end_contours;        // Array of last points of each contour; its length is the number of contours.
  USHORT          num_instr;
  BYTE            *instrs;
  BYTE            *flags/*[number_of_points]*/;
  // Actually the type of coordinates is either BYTE or SHORT.
  // Each coordinate might has different type according to the corresponding flag.
  // Here I choose to store all the coordinates in a SHORT array.
  PointF          *coordinates/*[number_of_points]*/;

  // the number of points in this glyph
  USHORT          pt_num;
  // Points are indexed from 0. end_pts_of_contours stores the index of each contour's end point.
  // The last contour's end point has the largest index which equals pt_num - 1.
};

class DLL_API Glyph_Loader {
 public:
  Glyph_Loader(Glyph &glyph) {
    header.reset();
    end_contours = glyph.end_contours;
    num_instr = 0;
    instrs = glyph.instrs;
    flags = glyph.flags;
    coordinates = glyph.coordinates;
    pt_num = 0;
  }
  void load_glyph(GLYPH_ID glyph_index, const Matrix &mtx = Matrix());
  void load_simple_glyph(Mem_Stream &msm);
  void load_composite_glyph(Mem_Stream &msm);

  Glyph_Header  header;
  USHORT        *end_contours;       // Array of last points of each contour; its length is the number of contours.
  USHORT        num_instr;
  BYTE          *instrs;
  BYTE          *flags/*[number_of_points]*/;
  PointF        *coordinates/*[number_of_points]*/;
  USHORT        pt_num;                     // the number of points in this glyph
  // Points are indexed from 0. end_pts_of_contours stores the index of each contour's end point.
  // The last contour's end point has the largest index which equals pt_num - 1.
 private:
  void read_flags(Mem_Stream &msm);
  void read_coordinates(Mem_Stream &msm, PointF *ptr, bool read_x);
};

} // namespace ttf_dll
#endif