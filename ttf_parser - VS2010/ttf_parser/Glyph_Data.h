#ifndef GLYPH_DATA_H
#define GLYPH_DATA_H
#include "type.h"
#include "ttf_table.h"
#include "mem_stream.h"
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
class GlyphLoader;

class DLL_API GlyphData {
 public:
  ~GlyphData();
  // Reads the table from the file stream. The `entry` provides some
  // information needed for loading.
  void LoadTable(TableRecordEntry *entry, ifstream &fin);
  // Dumps the information of this table to an XML file.
  void DumpInfo(XmlLogger &logger) const;
  // Loads the glyph labeled by `glyph_index`.
  Glyph *LoadGlyph(const GlyphID glyph_index);

  // The whole chunk of data in glyph data table.
  void      *data_;
  // Length of data in bytes.
  size_t    length_;
};

class DLL_API GlyphHeader {
 public:
  void Reset();
  bool LoadGlyphHeader(MemStream &msm);

  Short  num_contours_;
  // If the 'number_of_contours' is greater than zero, this is a single glyph;
  // if that field equals -1, this is a composite glyph.
  FWord  x_min_;
  FWord  y_min_;
  FWord  x_max_;
  FWord  y_max_;
};

class DLL_API Glyph {
 public:
  void Prepare();
  void Destroy();
  void Reset();
  Glyph *LoadSimpleGlyph(MemStream &msm, GlyphLoader &loader);
  Glyph *LoadCompositeGlyph(MemStream &msm, GlyphLoader &loader);
  bool LoadGlyphHeader(MemStream &msm);
  Glyph *LoadGlyph(MemStream &msm);
  void DumpCoordinate();
  void DumpSVGOutline(FILE *fp);
  void GlyphToPath(GraphicsPath &path);
  void DumpInfo(FILE *fp, size_t indent);
  void CountPointNum(int &all_pt_num, int &off_pt_num);
  void OutputPoints(PointF *all_pt, int *off_pt);

  GlyphHeader     header_;
  bool            root_;
  GlyphID         glyph_index_;

  UShort          *end_contours_;        // Array of last points of each contour; its length is the number of contours.
  UShort          num_instructions_;
  Byte            *instructions_;
  Byte            *flags_/*[number_of_points]*/;
  // Actually the type of coordinates is either BYTE or SHORT.
  // Each coordinate might has different type according to the corresponding flag.
  // Here I choose to store all the coordinates in a SHORT array.
  PointF          *coordinates_/*[number_of_points]*/;

  // the number of points in this glyph
  UShort          pt_num_;
  // Points are indexed from 0. end_pts_of_contours stores the index of each contour's end point.
  // The last contour's end point has the largest index which equals pt_num - 1.
};

class DLL_API GlyphLoader {
 public:
  GlyphLoader(Glyph &glyph) {
    header_.Reset();
    end_contours_ = glyph.end_contours_;
    num_instructions_ = 0;
    instructions_ = glyph.instructions_;
    flags_ = glyph.flags_;
    coordinates_ = glyph.coordinates_;
    pt_num_ = 0;
  }
  void LoadGlyph(GlyphID glyph_index, const Matrix &mtx = Matrix());
  void LoadSimpleGlyph(MemStream &msm);
  void LoadCompositeGlyph(MemStream &msm);

  GlyphHeader   header_;
  UShort        *end_contours_;       // Array of last points of each contour; its length is the number of contours.
  UShort        num_instructions_;
  Byte          *instructions_;
  Byte          *flags_/*[number_of_points]*/;
  PointF        *coordinates_/*[number_of_points]*/;
  UShort        pt_num_;                     // the number of points in this glyph
  // Points are indexed from 0. end_pts_of_contours stores the index of each contour's end point.
  // The last contour's end point has the largest index which equals pt_num - 1.
 private:
  void ReadFlags(MemStream &msm);
  void ReadCoordinates(MemStream &msm, PointF *ptr, bool read_x);
};

} // namespace ttf_dll
#endif