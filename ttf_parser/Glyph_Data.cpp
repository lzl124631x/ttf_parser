#include "stdafx.h"
#include "Glyph_Data.h"
#include "True_Type_Font.h"

namespace ttf_dll{
    static void add_quadratic_bezier(GraphicsPath &path, const PointF &q0, const PointF &q1, const PointF &q2){
    PointF c1, c2;
    // first control point of cubic bezier: c1 = (q0 + 2 * q1)/3 = q0 + 2 * (q1 - q0)/3
    c1.X = (q0.X + 2 * q1.X) / 3.0f;
    c1.Y = (q0.Y + 2 * q1.Y) / 3.0f;

    // second control point of cubic bezier: c2 = (2 * q1 + q2)/3 = c1 + (q2 - q0)/3
    c2.X = (2 * q1.X + q2.X) / 3.0f;
    c2.Y = (2 * q1.Y + q2.Y) / 3.0f;

    path.AddBezier(q0, c1, c2, q2);
  }
  bool is_simply_glyph(SHORT num_contours){ return num_contours > 0; }
  bool is_composite_glyph(SHORT num_contours){ return num_contours == -1; }
  // Bit Order of Simply Glyph Description Flag
  enum Simple_Glyph_Description_Flag{ // FIXME: This enum should be defined in the declaration of class.
    ON_CURVE            = BIT(0),
    // If set, the point is on the curve; otherwise, it is off the curve.
    X_SHORT_VECTOR      = BIT(1),
    // If set, the corresponding x-coordinate is 1 byte long. If not set, 2 bytes.
    Y_SHORT_VECTOR      = BIT(2),
    // Correspondingly.
    REPEAT              = BIT(3),
    // If set, the next byte specifies the number of additional times this set of flags is to be repeated.
    // In this way, the number of flags listed can be smaller than the number of points in a character.
    THIS_X_IS_SAME      = BIT(4),
    // This flag has two meanings, depending on how the x-Short Vector flag is set.
    // If x-Short Vector is set, this bit describes the sign of the value, with 1 equalling positive and 0 negative.
    // If the x-Short Vector bit is not set and this bit is set,
    // then the current x-coordinate is the same as the previous x-coordinate.
    // If the x-Short Vector bit is not set and this bit is also not set,
    // the current x-coordinate is a signed 16-bit delta vector.
    THIS_Y_IS_SAME      = BIT(5)
    // Correspondingly.
  };

  // Bit Order of Composite Glyph Description Flag
  enum Composite_Glyph_Description_Flag{
    ARG_1_AND_2_ARE_WORDS         = BIT(0),
    // If this is set, the arguments are words; otherwise, they are bytes.
    ARGS_ARE_XY_VALUES            = BIT(1),
    // If this is set, the arguments are xy values; otherwise, they are points.
    ROUND_XY_TO_GRID              = BIT(2),
    // For the xy values if the preceding is true.
    WE_HAVE_A_SCALE               = BIT(3),
    // This indicates that there is a simple scale for the component. Otherwise, scale = 1.0.
    // Bit 4 is reserved and set to 0.
    MORE_COMPONENTS               = BIT(5),
    // Indicates at least one more glyph after this one.
    WE_HAVE_AN_X_AND_Y_SCALE      = BIT(6),
    // The x direction will use a different scale from the y direction.
    WE_HAVE_A_TWO_BY_TWO          = BIT(7),
    // There is a 2 by 2 transformation that will be used to scale the component.
    WE_HAVE_INSTRUCTIONS          = BIT(8),
    // Following the last component are instructions for the composite character.
    USE_MY_METRICS                = BIT(9),
    // If set, this forces the aw and lsb (and rsb) for the composite to be equal to those from this original glyph.
    // This works for hinted and unhinted characters.
    OVERLAP_COMPOUND              = BIT(10),
    // Used by Apple in GX fonts.
    SCALED_COMPONENT_OFFSET       = BIT(11),
    // Composite designed to have the component offset scaled (designed for Apple rasterizer).
    UNSCALED_COMPONENT_OFFSET     = BIT(12)
    // Composite designed not to have the component offset scaled (designed for the Microsoft TrueType rasterizer).
  };
  static Glyph_Data *glyf;  // FIXME: Do I really need this pointer?
  static Glyph glyph;
/************************************************************************/
/*                                                                      */
/************************************************************************/
  void Glyph_Loader::read_flags(Mem_Stream &msm){
    BYTE flag = 0;  
    for(int i = 0; i < pt_num;){
      MREAD(msm, &flag);
      flags[i++] = flag;
      if(flag & REPEAT){
        BYTE repeat_num = 0;
        MREAD(msm, &repeat_num);
        while(repeat_num-- > 0){
          flags[i++] = flag;
        }
      }
    }
  }

  void Glyph_Loader::read_coordinates(Mem_Stream &msm, PointF *ptr, bool read_x){
    BYTE flag = 0;
    BYTE SHORT_VECTOR = X_SHORT_VECTOR << (read_x ? 0: 1);
    BYTE IS_SAME = THIS_X_IS_SAME << (read_x ? 0: 1);
    SHORT val = 0, last = 0;
    for(int i = 0; i < pt_num; ++i, ++ptr){
      flag = flags[i];
      val = 0;
      if(flag & SHORT_VECTOR){
        MREAD(msm, (BYTE*)&val);         // ATTENTION: DO NOT omit (BYTE*)! 
        if(~flag & IS_SAME){
          val = -val;
        }
      }else{
        if(~flag & IS_SAME){
          MREAD(msm, (SHORT*)&val);
          // ATTENTION: Though (SHORT*) is dispensable here, remember the coordinates are of either BYTE or SHORT!
        }
      }
      val += last;
      (read_x ? ptr->X : ptr->Y) = (float)val;
      last = val;
    }
  }
/************************************************************************/
/*                             Glyph Data                               */
/************************************************************************/
  void Glyph_Data::load_table(Table_Directory_Entry *entry, ifstream &fin){
    data = NULL;
    fin.seekg(entry->offset, ios::beg);
    length = entry->length;
    data = new BYTE[length];
    fin.read((char*)data, length);
    glyph.prepare();
    glyf = this;
  }

  Glyph_Data::~Glyph_Data(){
    DEL_A(data);
    glyph.destroy();
  }

  Glyph *Glyph_Data::load_glyph(GLYPH_ID glyph_index){
    glyph.reset();
    Glyph_Loader loader(glyph);
    loader.load_glyph(glyph_index);
    return &glyph;
  }

  void Glyph_Data::dump_info(FILE *fp, size_t indent){
    IND_FP("<glyf>\n");
    //for(int i = 0; i < maxp.num_glyphs; ++i){
      //Glyph *glyph = create_glyph()
      //glyph_data_array[i]->dump_info(fp, indent + 1);
    //}
    IND_FP("</glyf>\n");
  }
/************************************************************************/
/*                           Glyph Loader                               */
/************************************************************************/
  void Glyph_Loader::load_glyph(GLYPH_ID glyph_index, const Matrix &mtx){
    ULONG offset = g_ttf->glyph_index_to_offset(glyph_index);
    Mem_Stream msm(glyf->data, glyf->length);
    msm.seek(offset);
    if(!header.load_glyph_header(msm)){
      return; // Invalid header. Return directly.
    }
    if(glyph.root == true){ // If this is the root node of current glyph
      glyph.root = false;
      glyph.glyph_index = glyph_index;
      glyph.header = header;
      glyph.header.num_contours = 0; // Clear 'num_contours', increase this field after each subglyph is parsed.
    }
    if(is_simply_glyph(header.num_contours)){
      load_simple_glyph(msm);
      // Shift the end_contours with the pt_num.
      for(int i = 0; i < header.num_contours; ++i){
        end_contours[i] += glyph.pt_num;
      }
      // Transform the points.
      mtx.TransformPoints(coordinates, pt_num);
      // Forward the pointers.
      end_contours += header.num_contours;
      instrs += num_instr;
      flags += pt_num;
      coordinates += pt_num;
      // Update the fields in glyph.
      glyph.pt_num += pt_num;
      glyph.header.num_contours += header.num_contours;
    }else if(is_composite_glyph(header.num_contours)){
      load_composite_glyph(msm);
    }
  }
/************************************************************************/
/*                              Glyph                                   */
/************************************************************************/
  void Glyph_Header::reset(){
    num_contours = 0;
    x_min = y_min = x_max = y_max = 0;
  }

  void Glyph::reset(){
    header.reset();
    glyph_index = 0;
    num_instr = 0;
    pt_num = 0;
    root = true;
  }

#define MAX(a, b) ((a) > (b) ? (a) : (b))
  void Glyph::prepare(){
    end_contours = new USHORT[MAX(g_ttf->maxp.max_contours, g_ttf->maxp.max_composite_contours)];
    instrs = new BYTE[g_ttf->maxp.max_size_of_instructions];
    USHORT max_pts = MAX(g_ttf->maxp.max_points, g_ttf->maxp.max_composite_points);
    flags = new BYTE[max_pts];
    coordinates = new PointF[max_pts];
  }

  void Glyph::destroy(){
    DEL_A(end_contours);
    DEL_A(instrs);
    DEL_A(flags);
    DEL_A(coordinates);
  }

  bool Glyph_Header::load_glyph_header(Mem_Stream &msm){
    MREAD(msm, &num_contours);
    if(is_simply_glyph(num_contours) && num_contours > g_ttf->maxp.max_contours ||
      is_composite_glyph(num_contours) && num_contours > g_ttf->maxp.max_composite_contours){
      // ERROR: The contour number of this glyph is out-of-bound.
      return false;
    }
    MREAD(msm, &x_min);
    MREAD(msm, &y_min);
    MREAD(msm, &x_max);
    MREAD(msm, &y_max);
    return true;
  }

  void Glyph_Loader::load_simple_glyph(Mem_Stream &msm){
    MREAD_N(msm, end_contours, header.num_contours);
    MREAD(msm, &num_instr);
    if(num_instr > g_ttf->maxp.max_size_of_instructions){
      return;
    }
    //MREAD_N(msm, instructions, instruction_length);
    msm.seek(sizeof(BYTE) * num_instr); // FIXME: skip instructions
    pt_num = end_contours[header.num_contours - 1] + 1;
    read_flags(msm);
    read_coordinates(msm, coordinates, true);
    read_coordinates(msm, coordinates, false);
  }

  void Glyph_Loader::load_composite_glyph(Mem_Stream &msm){
    USHORT      flags;                  // component flag
    GLYPH_ID    glyph_index;            // glyph index of component
    SHORT       arg1, arg2;             // x/y-offset for component or point number; type depends on bits 0 and 1 in component flags
    SHORT       x, y;                   // The translate part of matrix transformation
    F2DOT14     xx, xy, yx, yy;         // The linear part of matrix transformation
    //                          | .------.   |           | .--------.   |
    // Linear Part      ->      | | 1  0 | 0 |   ->      | | xx  xy | 0 |
    //                  ->      | | 0  1 | 0 |   ->      | | yx  yy | 0 |
    //                          | .------.   |           | .--------.   |
    // Translate Part   ->      | | 0  0 | 1 |   ->      | | x   y  | 1 |
    //                          | .------.   |           | .--------.   |
    do{
      x = y = 0;
      xx = yy = 1;
      xy = yx = 0;
      MREAD(msm, &flags);
      MREAD(msm, &glyph_index);
      arg1 = arg2 = 0;
      if(flags & ARG_1_AND_2_ARE_WORDS){
        MREAD(msm, &arg1);
        MREAD(msm, &arg2);
      }else{
        MREAD(msm, (BYTE*)&arg1);
        MREAD(msm, (BYTE*)&arg2);
      }
      Matrix mtx;
      if(flags & ARGS_ARE_XY_VALUES){
        // Translate the points
        x = arg1;
        y = arg2;
      }
      if(flags & WE_HAVE_A_SCALE){
        MREAD(msm, &xx);
        yy = xx;
      }else if(flags & WE_HAVE_AN_X_AND_Y_SCALE){
        MREAD(msm, &xy);
        MREAD(msm, &yy);
      }else if(flags & WE_HAVE_A_TWO_BY_TWO){
        MREAD(msm, &xx);
        MREAD(msm, &xy);
        MREAD(msm, &yx);
        MREAD(msm, &yy);
      }
      mtx.SetElements(xx, xy, yx, yy, x, y);
      load_glyph(glyph_index, mtx);
    }while(flags & MORE_COMPONENTS);
    if(flags & WE_HAVE_INSTRUCTIONS){
      MREAD(msm, &num_instr);
      MREAD_N(msm, instrs, num_instr);
    }
  }

  void Glyph::glyph_to_path(GraphicsPath &path){
    PointF start_point, prev_point, cur_point;
    BYTE flag = 0;
    bool new_contour = true;
    int last = 0;               // The index of the last point of the jth contour.
    for(int i = 0, j = 0; i < pt_num; ++i){
    // i stands for the index of cur_point, while j stands for the index of current contour.
      flag = flags[i];
      cur_point = coordinates[i];

      if(new_contour){ // If this is the first point of jth contour.
        path.StartFigure();
        new_contour = false;
        // Calculate the prev_point.
        // <last_flag, flag>
        // <0, 0>: prev_point = mid(last_point, cur_point);
        // <0, 1>: prev_point = cur_point
        // <1, *>: prev_point = last_point
        last = end_contours[j];
        if(flags[last] & ON_CURVE){ // If the last point is on-curve, it'll be the prev_point of the first point.
          prev_point = coordinates[last];
        }else{ // If the last point is off-curve...
          if(flag & ON_CURVE){ // if the first point is on-curve, no need to draw anything.
            prev_point = cur_point;
          }else{ // Otherwise, the prev_point is the mid-point between the first and the last point (both off-curve).
            prev_point = PointF(
              (cur_point.X + coordinates[last].X) / 2.0f,
              (cur_point.Y + coordinates[last].Y) / 2.0f
              );
          }
        }
        start_point = prev_point; // start_point is not simply the first point.
      }
      if(cur_point.X < header.x_min){
        flag = flag;
      }
      // Draw something.
      // <flag, next_flag>
      // <0, 0>: add_quadratic_bezier(prev_point, cur_point, implicit_next_point)
      // <0, 1>: add_quadratic_bezier(prev_point, cur_point, next_point).
      // <1, *>: AddLine(prev_point, cur_point)
      if(flag & ON_CURVE){ // If this point is on-curve, just draw a straight line.
        path.AddLine(prev_point, cur_point);
        prev_point = cur_point;
      }else{ // Otherwise, draw a quadratic bezier.
        // Calculate the next point.
        PointF next_point;
        if(i == last){ // If this is the last point, the start_point is served as next_point.
          next_point = start_point;
        }else{ // Otherwise...
          BYTE next_flag = flags[i + 1];
          if(next_flag & ON_CURVE){ // If the next point is on-curve, it'll be the next_point.
            next_point = coordinates[i + 1];
          }else{ // Otherwise, next_point is the mid-point between cur_point and the next point.
            next_point = PointF(
              (cur_point.X + coordinates[i + 1].X) / 2.0f,
              (cur_point.Y + coordinates[i + 1].Y) / 2.0f
              );
          }
        }
        add_quadratic_bezier(path, prev_point, cur_point, next_point);
        prev_point = next_point;
      }
      if(i == last){ // If the last point is visited, close figure:)
        path.CloseFigure();
        new_contour = true;
        ++j;
      }
    }
  }

// Count the point number needed to render glyph points.
// all_pt_num counts all the points, including those implicit points.
// off_pt_num counts all the off-curve points.
  void Glyph::count_pt_num(int &all_pt_num, int &off_pt_num){
    all_pt_num = off_pt_num = 0;
    bool new_contour = true;
    BYTE flag = 0, prev_flag = 0;
    int last = 0;
    for(int i = 0, j = 0; i < pt_num; ++i, ++all_pt_num){
      flag = flags[i] & ON_CURVE;
      if(new_contour){ // If this is the first point of jth contour.
        new_contour = false;
        last = end_contours[j];
        prev_flag = flags[last] & ON_CURVE;
      }
      if(!flag){ // If the current point is OFF-curve, count it.
        ++off_pt_num;
        if(!prev_flag){ // If the previous point is also OFF-curve, count the implicit on-curve point.
          ++all_pt_num;
        }
      }
      if(i == last){ // If the last point is visited.
        new_contour = true;
        ++j;
      }
      prev_flag = flag;
    }
  }

// FIXME: why not combine 'output_pts' and 'glyph_to_path' together? 
  void Glyph::output_pts(PointF *all_pt, int *off_pt){
    PointF prev_point, cur_point;
    BYTE prev_flag = 0, flag = 0;
    bool new_contour = true;
    int last = 0;
    PointF *pt = all_pt;
    for(int i = 0, j = 0; i < pt_num; ++i){
    // i stands for the index of cur_point, while j stands for the index of current contour.
      flag = flags[i] & ON_CURVE;
      cur_point = coordinates[i];

      if(new_contour){ // If this is the first point of jth contour.
        new_contour = false;
        last = end_contours[j];
        prev_flag = flags[last] & ON_CURVE;
        prev_point = coordinates[last];
      }

      if(!prev_flag && !flag){
      // If the previous and current point are both OFF-curve, add the implicit on-curve point.
        *pt++ = PointF(
          (prev_point.X + cur_point.X) / 2.0f,
          (prev_point.Y + cur_point.Y) / 2.0f
          );
      }
      if(!flag){
        *off_pt++ = pt - all_pt;    // record the index of the off-curve point
      }
      *pt++ = prev_point = cur_point;
      prev_flag = flag;
      if(i == last){ // If the last point is visited.
        new_contour = true;
        ++j;
      }
    }
  }
}