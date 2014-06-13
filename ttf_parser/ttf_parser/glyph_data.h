#ifndef GLYPH_DATA_H
#define GLYPH_DATA_H
#include "type.h"
#include "ttf_table.h"
#include "mem_stream.h"
#include <objidl.h> // For some names used in <gdiplus.h>
#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")

namespace ttf_dll {
/****************************************************************************/
/*                         glyf - Glyf Data                                 */
/* Spec: https://www.microsoft.com/typography/otspec/glyf.htm               */
/****************************************************************************/
class Glyph;
class GlyphLoader;

// This table contains information that describes the glyphs in the font in
// the TrueType outline format.
class DLL_API GlyphData {
 public:
  // Reads the table from the file stream. The `entry` provides some
  // information needed for loading.
  void LoadTable(const TableRecordEntry *entry, std::ifstream &fin);
  // Deallocates the memory allocated in `LoadTable`, if any.
  void Destroy();
  // Dumps the information of this table to an XML file.
  void DumpInfo(XmlLogger &logger) const;
  // Loads the glyph labeled by `glyph_index`.
  Glyph *LoadGlyph(GlyphId glyph_index);
  // Accessors
  void *data() const { return data_; }
  size_t length() const { return length_; }

 private:
  // The whole chunk of data in glyph data table.
  void      *data_;
  // Length of data in bytes.
  size_t    length_;
};

class DLL_API GlyphHeader {
 public:
  void Reset();
  bool LoadGlyphHeader(MemStream &msm);
  // Accessors
  Short num_contours() const { return num_contours_; }
  FWord x_min() const { return x_min_; }
  FWord y_min() const { return y_min_; }
  FWord x_max() const { return x_max_; }
  FWord y_max() const { return y_max_; }
  // Mutators
  void set_num_contours(Short num_contours) { num_contours_ = num_contours; }

 private:
  // If the `numberOfContours` is greater than zero, this is a single glyph;
  // if that field equals -1, this is a composite glyph.
  Short  num_contours_;
  // Minimum x for coordinate data.
  FWord  x_min_;
  // Minimum y for coordinate data.
  FWord  y_min_;
  // Maximum x for coordinate data.
  FWord  x_max_;
  // Maximum y for coordinate data.
  FWord  y_max_;
};

class DLL_API Glyph {
 public:
  // Allocates memory (end_contours, instructions, flags and coordinates)
  // for holding data of glyphs. This function is called only when a new ttf
  // is loaded, i.e., when `GlyphData::LoadTable` is called.
  void Prepare();
  // Deallocates the memory allocated by `Glyph::Prepare`.
  void Destroy();
  // Reset fields of this glyph.
  void Reset();
  Glyph *LoadSimpleGlyph(MemStream &msm, GlyphLoader &loader);
  Glyph *LoadCompositeGlyph(MemStream &msm, GlyphLoader &loader);
  bool LoadGlyphHeader(MemStream &msm);
  Glyph *LoadGlyph(MemStream &msm);
  // void DumpCoordinate();
  // void DumpSvgOutline(FILE *fp);
  void GlyphToPath(Gdiplus::GraphicsPath &path) const;
  void DumpInfo(FILE *fp, size_t indent);
  // Counts the point number needed to render glyph points.
  // `all_pt_num` counts all the points, including those implicit points.
  // `off_pt_num` counts all the off-curve points.
  void CountPointNum(int *all_pt_num, int *off_pt_num) const;
  // Outputs all the points of this glyph to the array `all_pt`. The output
  // parameter `off_pt` stores the index of each off-curve points.
  void OutputPoints(Gdiplus::PointF *all_pt, int *off_pt) const;
  // Set the `num_contours` field in `header`.
  void SetNumContours(Short num_contours) { header_.set_num_contours(num_contours); }
  // Accessors
  const GlyphHeader &header() const { return header_; }
  bool root() const { return root_; }
  GlyphId glyph_index() const { return glyph_index_; }
  UShort *end_contours() const { return end_contours_; };
  Byte *instructions() const { return instructions_; }
  Byte *flags() const { return flags_; }
  Gdiplus::PointF *coordinates() const { return coordinates_; }
  UShort pt_num() const { return pt_num_; }
  // Mutators
  void set_header(const GlyphHeader &header) { header_ = header; }
  void set_root(bool root) { root_ = root; }
  void set_glyph_index(GlyphId glyph_index) { glyph_index_ = glyph_index; }
  void set_pt_num(UShort pt_num) { pt_num_ = pt_num; }

 private:
  GlyphHeader       header_;
  bool              root_;
  GlyphId           glyph_index_;
  // Array of last points of each contour; its length is the number of
  // contours.
  UShort            *end_contours_;
  UShort            num_instructions_;
  Byte              *instructions_;
  Byte              *flags_/*[number_of_points]*/;
  // Actually the type of coordinates is either BYTE or SHORT. Each
  // coordinate might has different type according to its corresponding flag.
  // Here I choose to store all the coordinates in a SHORT array for
  // simplicity.
  Gdiplus::PointF   *coordinates_/*[number_of_points]*/;

  // The number of points in this glyph.
  UShort            pt_num_;
  // Points are indexed from 0. end_pts_of_contours stores the index of each
  // contour's end point. The last contour's end point has the largest index
  // which equals pt_num - 1.
};

class DLL_API GlyphLoader {
 public:
  GlyphLoader(Glyph &glyph);
  void LoadGlyph(GlyphId glyph_index,
                 const Gdiplus::Matrix &mtx = Gdiplus::Matrix());
  void LoadSimpleGlyph(MemStream &msm);
  void LoadCompositeGlyph(MemStream &msm);

 private:
  void ReadFlags(MemStream &msm);
  void ReadCoordinates(MemStream &msm, Gdiplus::PointF *ptr, bool read_x);
  
  GlyphHeader   header_;
  // Array of last points of each contour; its length is the number of
  // contours.
  UShort            *end_contours_;
  UShort            num_instructions_;
  Byte              *instructions_;
  Byte              *flags_/*[number_of_points]*/;
  Gdiplus::PointF   *coordinates_/*[number_of_points]*/;
  // The number of points in this glyph.
  UShort            pt_num_;
  // Points are indexed from 0. end_pts_of_contours stores the index of each
  // contour's end point. The last contour's end point has the largest index
  // which equals pt_num - 1.
};

} // namespace ttf_dll
#endif