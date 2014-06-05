#include "stdafx.h"
#include "glyph_data.h"
#include "true_type_font.h"
using namespace Gdiplus;

namespace ttf_dll {

// Bit Order of Simply Glyph Description Flag
enum SimpleGlyphDescriptionFlag {
// FIXME: This enum should be defined in the declaration of class.
  // If set, the point is on the curve; otherwise, it is off the curve.
  kOnCurve              = BIT(0),
  // If set, the corresponding x-coordinate is 1 byte long.
  // If not set, 2 bytes.
  kXShortVector         = BIT(1),
  // Correspondingly.
  kYShortVector         = BIT(2),
  // If set, the next byte specifies the number of additional times this set
  // of flags is to be repeated. In this way, the number of flags listed can
  // be smaller than the number of points in a character.
  kRepeat               = BIT(3),
  // This flag has two meanings, depending on how the x-Short Vector flag is
  // set.
  // * If x-Short Vector is set, this bit describes the sign of the value,
  // with 1 equaling positive and 0 negative.
  // * If the x-Short Vector bit is not set and this bit is set, then the
  // current x-coordinate is the same as the previous x-coordinate.
  // * If the x-Short Vector bit is not set and this bit is also not set, the
  // current x-coordinate is a signed 16-bit delta vector.
  kThisXIsSame          = BIT(4),
  // Correspondingly.
  kThisYIsSame          = BIT(5)
};

// Bit Order of Composite Glyph Description Flag
enum CompositeGlyphDescriptionFlag {
  // If this is set, the arguments are words; otherwise, they are bytes.
  kArg1And2AreWords           = BIT(0),
  // If this is set, the arguments are xy values; otherwise, they are points.
  kArgsAreXyValues            = BIT(1),
  // For the xy values if the preceding is true.
  kRoundXyToGrid              = BIT(2),
  // This indicates that there is a simple scale for the component.
  // Otherwise, scale = 1.0.
  kWeHaveAScale               = BIT(3),
  // Bit 4 is reserved and set to 0.
  // Indicates at least one more glyph after this one.
  kMoreComponents             = BIT(5),
  // The x direction will use a different scale from the y direction.
  kWeHaveAnXAndYScale         = BIT(6),
  // There is a 2 by 2 transformation that will be used to scale the
  // component.
  kWeHaveATwoByTwo            = BIT(7),
  // Following the last component are instructions for the composite
  // character.
  kWeHaveInstructions         = BIT(8),
  // If set, this forces the aw and lsb (and rsb) for the composite to be
  // equal to those from this original glyph. This works for hinted and
  // unhinted characters.
  kUseMyMetrics               = BIT(9),
  // Used by Apple in GX fonts.
  kOverlapCompound            = BIT(10),
  // Composite designed to have the component offset scaled (designed for
  // Apple rasterizer).
  kScaledComponentOffset      = BIT(11),
  // Composite designed not to have the component offset scaled (designed for
  // the Microsoft TrueType rasterizer).
  kUnscaledComponentOffset    = BIT(12)
};

// Adds a quadratic Bezier to the `path`. `q0`, `q1` and `q2` stand for the
// start, control and end point of the quadratic Bezier respectively.
static void AddQuadraticBezier(GraphicsPath &path,const PointF &q0,
                               const PointF &q1, const PointF &q2);
// Returns true if the `num_contours` belongs to simple glyph.
static inline bool IsSimpleGlyph(const Short num_contours);
// Returns true if the `num_contours` belongs to composite glyph.
static inline bool IsCompositeGlyph(const Short num_contours);

static GlyphData *glyf;  // FIXME: Do I really need this pointer?
static Glyph glyph;
/****************************************************************************/
/*                               Glyph Data                                 */
/****************************************************************************/
void GlyphData::LoadTable(const TableRecordEntry *entry, ifstream &fin) {
  data_ = NULL;
  fin.seekg(entry->offset_, ios::beg);
  length_ = entry->length_;
  data_ = new Byte[length_];
  fin.read((char*)data_, length_);
  glyph.Prepare();
  glyf = this;
}

void GlyphData::Destroy() {
  DEL_A(data_);
  glyph.Destroy();
}

Glyph *GlyphData::LoadGlyph(const GlyphId glyph_index) {
  glyph.Reset();
  GlyphLoader loader(glyph);
  loader.LoadGlyph(glyph_index);
  return &glyph;
}

void GlyphData::DumpInfo(XmlLogger &logger) const {
  logger.Println("<glyf>");
  // FIXME: not finished.
  //for (int i = 0; i < maxp.num_glyphs; ++i){
  //Glyph *glyph = create_glyph()
  //glyph_data_array[i]->dump_info(fp, indent + 1);
  //}
  logger.Println("</glyf>");
}
/****************************************************************************/
/*                               GlyphLoader                                */
/****************************************************************************/
void GlyphLoader::LoadGlyph(const GlyphId glyph_index, const Matrix &mtx) {
  ULong offset = 0, length = 0;
  g_ttf->loca_.GetGlyphOffsetAndLength(glyph_index, &offset, &length);
  if (!length) {
    // This glyph does not have outline.
    glyph.pt_num_ = 0;
    return;
  }
  // FIXME: The bound in the following line could be more tight.
  MemStream msm(glyf->data_, glyf->length_);
  msm.Seek(offset);
  if (!header_.LoadGlyphHeader(msm)) {
    // Invalid header. Return directly.
    return;
  }
  if (glyph.root_ == true) {
    // If this is the root node of current glyph...
    glyph.root_ = false;
    glyph.glyph_index_ = glyph_index;
    glyph.header_ = header_;
    // Clear 'num_contours', increase this field after each subglyph is
    // parsed.
    glyph.header_.num_contours_ = 0;
  }
  if (IsSimpleGlyph(header_.num_contours_)) {
    LoadSimpleGlyph(msm);
    // Shift the end_contours with the pt_num.
    for (int i = 0; i < header_.num_contours_; ++i) {
      end_contours_[i] += glyph.pt_num_;
    }
    // Transform the points.
    mtx.TransformPoints(coordinates_, pt_num_);
    // Forward the pointers.
    end_contours_ += header_.num_contours_;
    instructions_ += num_instructions_;
    flags_ += pt_num_;
    coordinates_ += pt_num_;
    // Update the fields in glyph.
    glyph.pt_num_ += pt_num_;
    glyph.header_.num_contours_ += header_.num_contours_;
  } else if (IsCompositeGlyph(header_.num_contours_)) {
    LoadCompositeGlyph(msm);
  }
}

void GlyphLoader::ReadFlags(MemStream &msm) {
  Byte flag = 0;
  for (int i = 0; i < pt_num_;) {
    MREAD(msm, &flag);
    flags_[i++] = flag;
    if (flag & kRepeat) {
      Byte repeat_num = 0;
      MREAD(msm, &repeat_num);
      while (repeat_num-- > 0) {
        flags_[i++] = flag;
      }
    }
  }
}

void GlyphLoader::ReadCoordinates(MemStream &msm, PointF *ptr, bool read_x) {
  Byte flag = 0;
  Byte SHORT_VECTOR = kXShortVector << (read_x ? 0: 1);
  Byte IS_SAME = kThisXIsSame << (read_x ? 0: 1);
  Short val = 0, last = 0;
  for (int i = 0; i < pt_num_; ++i, ++ptr) {
    flag = flags_[i];
    val = 0;
    if (flag & SHORT_VECTOR) {
      MREAD(msm, (Byte*)&val);         // ATTENTION: DO NOT omit (BYTE*)!
      if (~flag & IS_SAME) {
        val = -val;
      }
    } else {
      if (~flag & IS_SAME) {
        MREAD(msm, (Short*)&val);
        // ATTENTION: Though (SHORT*) is dispensable here, remember the coordinates are of either BYTE or SHORT!
      }
    }
    val += last;
    (read_x ? ptr->X : ptr->Y) = (float)val;
    last = val;
  }
}
/************************************************************************/
/*                              Glyph                                   */
/************************************************************************/
void GlyphHeader::Reset() {
  num_contours_ = 0;
  x_min_ = y_min_ = x_max_ = y_max_ = 0;
}

void Glyph::Reset() {
  header_.Reset();
  glyph_index_ = 0;
  num_instructions_ = 0;
  pt_num_ = 0;
  root_ = true;
}

#define MAX(a, b) ((a) > (b) ? (a) : (b))
void Glyph::Prepare() {
  end_contours_ = new UShort[MAX(g_ttf->maxp_.max_contours_,
                                 g_ttf->maxp_.max_composite_contours_)];
  instructions_ = new Byte[g_ttf->maxp_.max_size_of_instructions_];
  UShort max_pts = MAX(g_ttf->maxp_.max_points_,
                       g_ttf->maxp_.max_composite_points_);
  flags_ = new Byte[max_pts];
  coordinates_ = new PointF[max_pts];
}
#undef MAX

void Glyph::Destroy() {
  DEL_A(end_contours_);
  DEL_A(instructions_);
  DEL_A(flags_);
  DEL_A(coordinates_);
}

bool GlyphHeader::LoadGlyphHeader(MemStream &msm) {
  MREAD(msm, &num_contours_);
  if (IsSimpleGlyph(num_contours_)
      && num_contours_ > g_ttf->maxp_.max_contours_
      || IsCompositeGlyph(num_contours_)
      && num_contours_ > g_ttf->maxp_.max_composite_contours_) {
    // ERROR: The contour number of this glyph is out-of-bound.
    return false;
  }
  MREAD(msm, &x_min_);
  MREAD(msm, &y_min_);
  MREAD(msm, &x_max_);
  MREAD(msm, &y_max_);
  return true;
}

void GlyphLoader::LoadSimpleGlyph(MemStream &msm) {
  MREAD_N(msm, end_contours_, header_.num_contours_);
  MREAD(msm, &num_instructions_);
  if (num_instructions_ > g_ttf->maxp_.max_size_of_instructions_) {
    return;
  }
  //MREAD_N(msm, instructions, instruction_length);
  msm.Seek(sizeof(Byte) * num_instructions_); // FIXME: skip instructions
  pt_num_ = end_contours_[header_.num_contours_ - 1] + 1;
  ReadFlags(msm);
  ReadCoordinates(msm, coordinates_, true);
  ReadCoordinates(msm, coordinates_, false);
}

void GlyphLoader::LoadCompositeGlyph(MemStream &msm) {
  // component flag
  UShort      flags;
  // glyph index of component
  GlyphId     glyph_index;
  // x/y-offset for component or point number; type depends on bits 0 and 1
  // in component flags
  Short       arg1, arg2;
  // The translate part of matrix transformation
  Short       x, y;
  // The linear part of matrix transformation
  F2Dot14     xx, xy, yx, yy;
  //                          | .------.   |           | .--------.   |
  // Linear Part      ->      | | 1  0 | 0 |   ->      | | xx  xy | 0 |
  //                  ->      | | 0  1 | 0 |   ->      | | yx  yy | 0 |
  //                          | .------.   |           | .--------.   |
  // Translate Part   ->      | | 0  0 | 1 |   ->      | | x   y  | 1 |
  //                          | .------.   |           | .--------.   |
  do {
    x = y = 0;
    xx = yy = 1;
    xy = yx = 0;
    MREAD(msm, &flags);
    MREAD(msm, &glyph_index);
    arg1 = arg2 = 0;
    if (flags & kArg1And2AreWords) {
      MREAD(msm, &arg1);
      MREAD(msm, &arg2);
    } else {
      MREAD(msm, (Byte*)&arg1);
      MREAD(msm, (Byte*)&arg2);
    }
    Matrix mtx;
    if (flags & kArgsAreXyValues) {
      // Translate the points
      x = arg1;
      y = arg2;
    }
    if (flags & kWeHaveAScale) {
      MREAD(msm, &xx);
      yy = xx;
    } else if (flags & kWeHaveAnXAndYScale) {
      MREAD(msm, &xy);
      MREAD(msm, &yy);
    } else if (flags & kWeHaveATwoByTwo) {
      MREAD(msm, &xx);
      MREAD(msm, &xy);
      MREAD(msm, &yx);
      MREAD(msm, &yy);
    }
    mtx.SetElements(xx, xy, yx, yy, x, y);
    LoadGlyph(glyph_index, mtx);
  } while (flags & kMoreComponents);
  if (flags & kWeHaveInstructions) {
    MREAD(msm, &num_instructions_);
    MREAD_N(msm, instructions_, num_instructions_);
  }
}

void Glyph::GlyphToPath(GraphicsPath &path) {
  // Set the fill mode as `FillModeWinding` to prevent white spots from
  // appearing at the overlaps of glyph contours.
  path.SetFillMode(FillModeWinding);

  PointF start_point, prev_point, cur_point;
  Byte flag = 0;
  bool new_contour = true;
  // The index of the last point of the jth contour.
  int last = 0;
  for (int i = 0, j = 0; i < pt_num_; ++i) {
    // `i` stands for the index of `cur_point`, while `j` stands for the
    // index of current contour.
    flag = flags_[i];
    cur_point = coordinates_[i];

    if (new_contour) { // If this is the first point of jth contour.
      path.StartFigure();
      new_contour = false;
      // Calculate the prev_point.
      // <last_flag, flag>
      // <0, 0>: prev_point = mid(last_point, cur_point);
      // <0, 1>: prev_point = cur_point
      // <1, *>: prev_point = last_point
      last = end_contours_[j];
      if (flags_[last] & kOnCurve) {
        // If the last point is on-curve, it'll be the prev_point of the first
        // point.
        prev_point = coordinates_[last];
      } else {
        // If the last point is off-curve...
        if (flag & kOnCurve) {
          // and if the first point is on-curve, no need to draw anything.
          prev_point = cur_point;
        } else {
          // Otherwise, the prev_point is the mid-point between the first and
          // the last point (both off-curve).
          prev_point = PointF(
                           (cur_point.X + coordinates_[last].X) / 2.0f,
                           (cur_point.Y + coordinates_[last].Y) / 2.0f);
        }
      }
      start_point = prev_point; // start_point is not simply the first point.
    }
    if (cur_point.X < header_.x_min_) {
      flag = flag;
    }
    // Draw something.
    // <flag, next_flag>
    // <0, 0>: AddQuadraticBezier(prev_point, cur_point, implicit_next_point)
    // <0, 1>: AddQuadraticBezier(prev_point, cur_point, next_point).
    // <1, *>: AddLine(prev_point, cur_point)
    if (flag & kOnCurve) {
      // If this point is on-curve, just draw a straight line.
      path.AddLine(prev_point, cur_point);
      prev_point = cur_point;
    } else { // Otherwise, draw a quadratic bezier.
      // Calculate the next point.
      PointF next_point;
      if (i == last) {
        // If this is the last point, the `start_point` is served as
        // next_point.
        next_point = start_point;
      } else {
        Byte next_flag = flags_[i + 1];
        if (next_flag & kOnCurve) {
          // If the next point is on-curve, it'll be the next_point.
          next_point = coordinates_[i + 1];
        } else {
          // Otherwise, `next_point` is the `mid-point` between `cur_point`
          // and the next point.
          next_point = PointF(
                         (cur_point.X + coordinates_[i + 1].X) / 2.0f,
                         (cur_point.Y + coordinates_[i + 1].Y) / 2.0f
                       );
        }
      }
      AddQuadraticBezier(path, prev_point, cur_point, next_point);
      prev_point = next_point;
    }
    if (i == last) {
      // If the last point is visited, close figure.
      path.CloseFigure();
      new_contour = true;
      ++j;
    }
  }
}

// Count the point number needed to render glyph points.
// all_pt_num counts all the points, including those implicit points.
// off_pt_num counts all the off-curve points.
void Glyph::CountPointNum(int &all_pt_num, int &off_pt_num) {
  all_pt_num = off_pt_num = 0;
  bool new_contour = true;
  Byte flag = 0, prev_flag = 0;
  int last = 0;
  for (int i = 0, j = 0; i < pt_num_; ++i, ++all_pt_num) {
    flag = flags_[i] & kOnCurve;
    if (new_contour) { // If this is the first point of jth contour.
      new_contour = false;
      last = end_contours_[j];
      prev_flag = flags_[last] & kOnCurve;
    }
    if (!flag) { // If the current point is OFF-curve, count it.
      ++off_pt_num;
      if (!prev_flag) { // If the previous point is also OFF-curve, count the implicit on-curve point.
        ++all_pt_num;
      }
    }
    if (i == last) { // If the last point is visited.
      new_contour = true;
      ++j;
    }
    prev_flag = flag;
  }
}

// FIXME: why not combine 'output_pts' and 'glyph_to_path' together?
void Glyph::OutputPoints(PointF *all_pt, int *off_pt) {
  PointF prev_point, cur_point;
  Byte prev_flag = 0, flag = 0;
  bool new_contour = true;
  int last = 0;
  PointF *pt = all_pt;
  for (int i = 0, j = 0; i < pt_num_; ++i) {
    // i stands for the index of cur_point, while j stands for the index of current contour.
    flag = flags_[i] & kOnCurve;
    cur_point = coordinates_[i];

    if (new_contour) { // If this is the first point of jth contour.
      new_contour = false;
      last = end_contours_[j];
      prev_flag = flags_[last] & kOnCurve;
      prev_point = coordinates_[last];
    }

    if (!prev_flag && !flag) {
      // If the previous and current point are both OFF-curve, add the implicit on-curve point.
      *pt++ = PointF(
                (prev_point.X + cur_point.X) / 2.0f,
                (prev_point.Y + cur_point.Y) / 2.0f
              );
    }
    if (!flag) {
      *off_pt++ = pt - all_pt;    // record the index of the off-curve point
    }
    *pt++ = prev_point = cur_point;
    prev_flag = flag;
    if (i == last) { // If the last point is visited.
      new_contour = true;
      ++j;
    }
  }
}

/****************************************************************************/
static void AddQuadraticBezier(GraphicsPath &path, const PointF &q0, const PointF &q1, const PointF &q2) {
  PointF c1, c2;
  // first control point of cubic bezier: c1 = (q0 + 2 * q1)/3 = q0 + 2 * (q1 - q0)/3
  c1.X = (q0.X + 2 * q1.X) / 3.0f;
  c1.Y = (q0.Y + 2 * q1.Y) / 3.0f;

  // second control point of cubic bezier: c2 = (2 * q1 + q2)/3 = c1 + (q2 - q0)/3
  c2.X = (2 * q1.X + q2.X) / 3.0f;
  c2.Y = (2 * q1.Y + q2.Y) / 3.0f;

  path.AddBezier(q0, c1, c2, q2);
}

static bool IsSimpleGlyph(const Short num_contours) {
  return num_contours > 0;
}

static bool IsCompositeGlyph(const Short num_contours) {
  return num_contours == -1;
}

} // namespace ttf_dll