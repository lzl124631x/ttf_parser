#ifndef GLYPH_DATA_H
#define GLYPH_DATA_H
#include "type.h"
#include "ttf_table.h"
#include "index_to_location.h"
#include "mem_stream.h"
#include <objidl.h> // For some names used in <gdiplus.h>
#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")

namespace ttf_dll {
/****************************************************************************/
/*                         glyf - Glyf Data                                 */
/* Spec: https://www.microsoft.com/typography/otspec/glyf.htm               */
/****************************************************************************/
class GlyphData;

class DLL_API Glyph {
  friend class GlyphData;
public:
  Glyph();
  // Allocates memory (end_contours, instructions, flags and coordinates)
  // for holding data of glyphs.
  void Init(UShort num_contours, UShort num_instructions, UShort num_points);
  // Deallocates the memory allocated by `Init`.
  void Destroy();
  // Reset fields of this glyph.
  void Reset();
  // Turns the outline of this glyph into a `GraphicsPath`.
  void GlyphToPath(Gdiplus::GraphicsPath &path) const;
  // Counts the point number needed to render glyph points.
  // `all_pt_num` counts all the points, including those implicit points.
  // `off_pt_num` counts all the off-curve points.
  void CountPointNum(int *all_pt_num, int *off_pt_num) const;
  // Outputs all the points of this glyph to the array `all_pt`. The output
  // parameter `off_pt` stores the index of each off-curve points.
  void OutputPoints(Gdiplus::PointF *all_pt, int *off_pt) const;
  // Accessors
  GlyphId glyph_index() const { return glyph_index_; }
  Short num_contours() const { return num_contours_; }
  FWord x_min() const { return x_min_; };
  FWord y_min() const { return y_min_; };
  FWord x_max() const { return x_max_; };
  FWord y_max() const { return y_max_; };

private:
  // Returns true if this is a simple glyph.
  bool IsSimpleGlyph() const { return num_contours_ > 0; }
  // Returns true if this is a composite glyph.
  bool IsCompositeGlyph() const { return num_contours_ == -1; }
  
  // The glyph index of this glyph.
  GlyphId           glyph_index_;
  // If the `numberOfContours` is greater than zero, this is a single glyph;
  // if that field equals -1, this is a composite glyph.
  Short             num_contours_;
  // Minimum x for coordinate data.
  FWord             x_min_;
  // Minimum y for coordinate data.
  FWord             y_min_;
  // Maximum x for coordinate data.
  FWord             x_max_;
  // Maximum y for coordinate data.
  FWord             y_max_;

  // Array of last points of each contour.
  UShort            *end_contours_/*[num_contours_]*/;
  // Total number of bytes for instructions.
  UShort            num_instructions_;
  // Array of instructions for each glyph.
  Byte              *instructions_/*[num_instructions_]*/;
  // Array of flags for each coordinate in outline.
  Byte              *flags_/*[num_points_]*/;
  // The type of coordinates is either BYTE or SHORT. Each coordinate might
  // has different type according to its corresponding flag.
  Gdiplus::PointF   *coordinates_/*[num_points_]*/;
  // The number of points in this glyph.
  // Points are indexed from 0. `end_contours_` stores the index of each
  // contour's end point. The last contour's end point has the largest index
  // which equals `num_points_ - 1`.
  UShort            num_points_;
};

// This table contains information that describes the glyphs in the font in
// the TrueType outline format.
class DLL_API GlyphData : public TtfSubtable {
 public:
  explicit GlyphData(const TrueTypeFont &ttf);
  // Overrides
  void Init(const TableRecordEntry *entry, std::ifstream &fin);
  void Destroy();
  void DumpInfo(XmlLogger &logger) const;
  // Loads the glyph labeled by `glyph_index`.
  const Glyph &LoadGlyph(GlyphId glyph_index);

 private:
  void LoadSubglyph(GlyphId glyph_index,
                    const Gdiplus::Matrix &mtx = Gdiplus::Matrix(),
                    UShort depth = 0);
  void LoadSimpleGlyph(MemStream &msm);
  void LoadCompositeGlyph(MemStream &msm, const Gdiplus::Matrix &mtx,
                          UShort depth);
  void ReadFlags(MemStream &msm);
  void ReadCoordinates(MemStream &msm, Gdiplus::PointF *ptr, bool read_x);
  // The whole chunk of data in glyph data table.
  void      *data_;
  // The length of data in bytes.
  size_t    length_;
  // `LoadGlyph` uses this field to store a specific glyph.
  Glyph     glyph_;

  // Loader
  Glyph     subglyph_;
  bool      root_;
};

} // namespace ttf_dll
#endif