#include "stdafx.h"
#include "Glyph_Data.h"
#include "True_Type_Font.h"

namespace ttf_dll{
  static Glyph_Data *glyf;  // FIXME: Do I really need this pointer?
  static Glyph glyph;
/************************************************************************/
/*                             Glyph Data                               */
/************************************************************************/
  void Glyph_Data::load_table(Table_Directory_Entry *entry, ifstream &fin){
    data = NULL;
    fin.seekg(entry->offset, ios::beg);
    length = entry->length;
    data = new BYTE[length];
    fin.read((char*)data, length);
    glyf = this;
  }

  Glyph *Glyph_Data::load_glyph(USHORT glyph_index){
    // Read 'number_of_contours' to determine the format of the glyph.
    ULONG offset = g_ttf->glyph_index_to_offset(glyph_index);
    Mem_Stream msm(data, length);
    msm.seek(offset);
    Glyph_Loader loader(glyph);
    glyph.load_glyph_header(msm);
    return &glyph;
  }

  void Glyph_Data::dump_info(FILE *fp, size_t indent){
    INDENT(fp, indent); fprintf(fp, "<glyf>\n");
    //for(int i = 0; i < maxp.num_glyphs; ++i){
      //Glyph *glyph = create_glyph()
      //glyph_data_array[i]->dump_info(fp, indent + 1);
    //}
    INDENT(fp, indent); fprintf(fp, "</glyf>\n");
  }
/************************************************************************/
/*                           Glyph Loader                               */
/************************************************************************/
  Glyph_Loader::Glyph_Loader(Glyph &glyph){
    end_contours = glyph.end_contours;
    flags = glyph.flags;
    x_coordinates = glyph.x_coordinates;
    y_coordinates = glyph.y_coordinates;
  }
/************************************************************************/
/*                              Glyph                                   */
/************************************************************************/
  Glyph::Glyph(){
    num_contours = 0;
    x_min = y_min = x_max = y_max = 0;
    end_contours = NULL;
    flags = NULL;
    x_coordinates = y_coordinates = NULL;
  }

  Glyph *Glyph::load(Mem_Stream &msm, Glyph_Loader &loader){
    if(!load_glyph_header(msm)) return NULL;
    if(is_simply_glyph()){
      return glyph.load_simple_glyph(msm, loader);
    }else if(is_composite_glyph()){
      return glyph.load_composite_glyph(msm, loader);
    }
    return NULL;
  }

  bool Glyph::load_glyph_header(Mem_Stream &msm){
    MREAD(msm, &num_contours);
    if(is_simply_glyph() && num_contours > g_ttf->maxp.max_contours ||
      is_composite_glyph() && num_contours > g_ttf->maxp.max_composite_contours){
      // ERROR: The contour number of this glyph is out-of-bound.
      return NULL;
    }
    MREAD(msm, &x_min);
    MREAD(msm, &y_min);
    MREAD(msm, &x_max);
    MREAD(msm, &y_max);
  }

  Glyph *Glyph::load_simple_glyph(Mem_Stream &msm, Glyph_Loader &loader){
    MREAD_N(msm, end_contours, num_contours);
    MREAD(msm, &instr_len);
    if(instr_len > g_ttf->maxp.max_size_of_instructions){
      return NULL;
    }
    //MREAD_N(msm, instructions, instruction_length);
    msm.seek(sizeof(BYTE) * instr_len); // FIXME: skip instructions
    pt_num = end_contours[num_contours - 1] + 1;
    read_flags(msm);
    read_coordinates(msm, x_coordinates, true);
    read_coordinates(msm, y_coordinates, false);
    return NULL;
  }

  void Simple_Glyph_Description::read_flags(Mem_Stream &msm){
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
/************************************************************************/
/*                     Simple Glyph Description                         */
/************************************************************************/
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

  Simple_Glyph_Description::Simple_Glyph_Description(){
    end_pts_of_contours = NULL;
    instruction_length = 0;
    instructions = NULL;
    flags = NULL;
    x_coordinates = y_coordinates = NULL;
    pt_num = 0;
  }

  // This function works only when a new font is loaded.
  void Simple_Glyph_Description::init(){
    USHORT max_points = g_ttf->maxp.max_points;
    USHORT max_contours = g_ttf->maxp.max_contours;
    USHORT max_size_of_instructions = g_ttf->maxp.max_size_of_instructions;
    end_pts_of_contours = new USHORT[max_contours];
    instructions = new BYTE[max_size_of_instructions];
    flags = new BYTE[max_points];
    x_coordinates = new SHORT[max_points];
    y_coordinates = new SHORT[max_points];
  }

  void Simple_Glyph_Description::destroy(){
    DEL_A(end_pts_of_contours);
    DEL_A(instructions);
    DEL_A(flags);
    DEL_A(x_coordinates);
    DEL_A(y_coordinates);
  }

  Glyph *Simple_Glyph_Description::load_glyph(Mem_Stream &msm){
    MREAD_N(msm, end_pts_of_contours, number_of_contours);
    MREAD(msm, &instruction_length);
    if(instruction_length > glyf->max_size_of_instructions){
      return NULL;
    }
    MREAD_N(msm, instructions, instruction_length);
    pt_num = end_pts_of_contours[number_of_contours - 1] + 1;
    read_flags(msm);
    read_coordinates(msm, x_coordinates, true);
    read_coordinates(msm, y_coordinates, false);
    return this;
  }

  void Simple_Glyph_Description::read_coordinates(Mem_Stream &msm, SHORT *ptr, bool read_x){
    SHORT last = 0;
    BYTE flag = 0;
    BYTE SHORT_VECTOR = X_SHORT_VECTOR << (read_x ? 0: 1);
    BYTE IS_SAME = THIS_X_IS_SAME << (read_x ? 0: 1);
    for(int i = 0; i < pt_num; ++i, ++ptr){
      flag = flags[i];
      *ptr = 0;
      if(flag & SHORT_VECTOR){
        MREAD(msm, (BYTE*)ptr);         // ATTENTION: DO NOT omit (BYTE*)! 
        if(~flag & IS_SAME){
          *ptr = -*ptr;
        }
      }else{
        if(~flag & IS_SAME){
          MREAD(msm, (SHORT*)ptr);
          // ATTENTION: Though (SHORT*) is dispensable here, remember the coordinates are of either BYTE or SHORT!
        }
      }
      *ptr += last;
      last = *ptr;
    }
  }

  void Simple_Glyph_Description::dump_flags(){
    printf("Simple_Glyph_Description::dump_flags\n");
    for(int i = 0; i < pt_num; ++i){
      if(flags[i] < 0x10) printf("0");
      printf("%X ", flags[i]);
      if((i + 1) % 5 == 0) printf("\n");
    }
    printf("\n");
  }

  void Simple_Glyph_Description::dump_coordinates(){
    printf("Simple_Glyph_Description::dump_coordinates\n");
    printf("<contour>\n");
    for(int i = 0, j = 0; i < pt_num; ++i){
      printf("    <pt x=\"%d\" y=\"%d\" on=\"%d\"/>\n",
        x_coordinates[i], y_coordinates[i], (flags[i] & ON_CURVE));
      if(i == end_pts_of_contours[j]){
        printf("</contour>\n");
        ++j;
        if(j != number_of_contours)
          printf("<contour>\n");
      }
    }
    printf("\n");
  }

  void Simple_Glyph_Description::dump_info(FILE *fp, size_t indent){
    INDENT(fp, indent);
    fprintf(fp, "<simpleGlyphDescription numberOfContours=\"%d\" xMin=\"%d\" yMin=\"%d\" xMax=\"%d\" yMax=\"%d\">\n",
      number_of_contours, x_min, y_min, x_max, y_max); // FIME: These information should be dumped by the base class.
    ++indent;
    INDENT(fp, indent); fprintf(fp, "<endPtsOfContours>\n");
    dump_array<USHORT>(fp, indent + 1, end_pts_of_contours, number_of_contours, "%-u");
    INDENT(fp, indent); fprintf(fp, "</endPtsOfContours>\n");
    INDENT(fp, indent); fprintf(fp, "<instructionLength value=\"%u\"/>\n", instruction_length);
    INDENT(fp, indent); fprintf(fp, "<instructions>\n");
    dump_array<BYTE>(fp, indent + 1, instructions, instruction_length, "%+02x");
    INDENT(fp, indent); fprintf(fp, "</instructions>\n");
    INDENT(fp, indent); fprintf(fp, "<flags>\n");
    dump_array<BYTE>(fp, indent + 1, flags, pt_num, "%+02x");
    INDENT(fp, indent); fprintf(fp, "</flags>\n");
    INDENT(fp, indent); fprintf(fp, "<xCoordinates>\n");
    dump_array<SHORT>(fp, indent + 1, x_coordinates, pt_num, "%+5d");
    INDENT(fp, indent); fprintf(fp, "</xCoordinates>\n");
    INDENT(fp, indent); fprintf(fp, "<yCoordinates>\n");
    dump_array<SHORT>(fp, indent + 1, y_coordinates, pt_num, "%+5d");
    INDENT(fp, indent); fprintf(fp, "</yCoordinates>\n");
    --indent;
    INDENT(fp, indent); fprintf(fp, "</simpleGlyphDescription>\n");
  }

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

  void Simple_Glyph_Description::glyph_to_path(GraphicsPath &path){
    PointF start_point, prev_point, cur_point;
    BYTE flag = 0;
    bool new_contour = true;
    int last = 0;               // The index of the last point of the jth contour.
    for(int i = 0, j = 0; i < pt_num; ++i){
    // i stands for the index of cur_point, while j stands for the index of current contour.
      flag = flags[i];
      cur_point = PointF(x_coordinates[i], y_coordinates[i]);

      if(new_contour){ // If this is the first point of jth contour.
        path.StartFigure();
        new_contour = false;
        // Calculate the prev_point.
        // <last_flag, flag>
        // <0, 0>: prev_point = mid(last_point, cur_point);
        // <0, 1>: prev_point = cur_point
        // <1, *>: prev_point = last_point
        last = end_pts_of_contours[j];
        if(flags[last] & ON_CURVE){ // If the last point is on-curve, it'll be the prev_point of the first point.
          prev_point = PointF(x_coordinates[last], y_coordinates[last]);
        }else{ // If the last point is off-curve...
          if(flag & ON_CURVE){ // if the first point is on-curve, no need to draw anything.
            prev_point = cur_point;
          }else{ // Otherwise, the prev_point is the mid-point between the first and the last point (both off-curve).
            prev_point = PointF(
              (cur_point.X + x_coordinates[last]) / 2.0f,
              (cur_point.Y + y_coordinates[last]) / 2.0f
              );
          }
        }
        start_point = prev_point; // start_point is not simply the first point.
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
            next_point = PointF(x_coordinates[i + 1], y_coordinates[i + 1]);
          }else{ // Otherwise, next_point is the mid-point between cur_point and the next point.
            next_point = PointF(
              (cur_point.X + x_coordinates[i + 1]) / 2.0f,
              (cur_point.Y + y_coordinates[i + 1]) / 2.0f
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

  // FIXME: The following code and comment is out-of-date. UPDATE!
  // If the last point is off-curve, the first point is regarded as the end point.
  // If two off-curve point appear in a row, the midpoint of them is regarded as the implicit on-curve point.
  // <prev_flag, flag>
  // <0, 0>: 1. append implicit on-curve point; 2. append 'Q' and current point.
  // <0, 1>: append current point
  // <1, 0>: append 'Q' and current point
  // <1, 1>: append 'L' and current point
  void Simple_Glyph_Description::dump_svg_outline(FILE *fp){
    FWORD width = x_max - x_min;
    FWORD height = y_max - y_min;
    double x_ratio = 300.0 / width;
    double y_ratio = 300.0 / height;

    fprintf(fp, "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
    fprintf(fp, "<svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n");
    fprintf(fp, "<g transform=\"scale(%.2lf, %.2lf) translate(%d, %d) matrix(1,0,0,-1,0,0)\">", x_ratio, y_ratio, -x_min, y_max);
    fprintf(fp, "<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" fill=\"none\" stroke=\"blue\" stroke-width=\"10\"/>\n", x_min, y_min, width, height);
    fprintf(fp, "<path d=\"");
    BYTE prev_flag = 1, flag = 1;
    bool new_contour = true;
    // FIXME: assume the first point of each contour must be on-curve.
    for(int i = 0, j = 0; i < pt_num; ++i){
      flag = flags[i] & ON_CURVE;
      if(prev_flag == 0 && flag == 0){ // append implicit on-curve point
        fprintf(fp, "%d, %d ", (x_coordinates[i - 1] + x_coordinates[i]) >> 1,
          (y_coordinates[i - 1] + y_coordinates[i]) >> 1);  
      }
      if(new_contour){
        fprintf(fp, "M");
        new_contour = false;
      }else if(flag == 0){
        fprintf(fp, "Q");
      }else if(prev_flag == 1){
        fprintf(fp, "L");
      }
      fprintf(fp, "%d, %d ", x_coordinates[i], y_coordinates[i]);
      if(i == end_pts_of_contours[j]){// This point is the last point of this contour.
        if(flag == 0){// If this is the last point of contour and is off-curve, use the first point of contour as end point.
          if(j == 0){
            fprintf(fp, "%d, %d", x_coordinates[0], y_coordinates[0]);
          }else{
            fprintf(fp, "%d, %d", x_coordinates[end_pts_of_contours[j - 1] + 1],
              y_coordinates[end_pts_of_contours[j - 1] + 1]);
          }
        }
        fprintf(fp, " Z ");
        new_contour = true;
        ++j;
      }
      prev_flag = flag;
    }
    fprintf(fp, "\"/>\n");
    fprintf(fp, "</g>");
    fprintf(fp, "</svg>");
  }

// Count the point number needed to render glyph points.
// all_pt_num counts all the points, including those implicit points.
// off_pt_num counts all the off-curve points.
  void Simple_Glyph_Description::count_pt_num(int &all_pt_num, int &off_pt_num){
    all_pt_num = off_pt_num = 0;
    bool new_contour = true;
    BYTE flag = 0, prev_flag = 0;
    int last = 0;
    for(int i = 0, j = 0; i < pt_num; ++i, ++all_pt_num){
      flag = flags[i] & ON_CURVE;
      if(new_contour){ // If this is the first point of jth contour.
        new_contour = false;
        last = end_pts_of_contours[j];
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
  void Simple_Glyph_Description::output_pts(PointF *all_pt, int *off_pt){
    PointF prev_point, cur_point;
    BYTE prev_flag = 0, flag = 0;
    bool new_contour = true;
    int last = 0;
    PointF *pt = all_pt;
    for(int i = 0, j = 0; i < pt_num; ++i){
    // i stands for the index of cur_point, while j stands for the index of current contour.
      flag = flags[i] & ON_CURVE;
      cur_point = PointF(x_coordinates[i], y_coordinates[i]);

      if(new_contour){ // If this is the first point of jth contour.
        new_contour = false;
        last = end_pts_of_contours[j];
        prev_flag = flags[last] & ON_CURVE;
        prev_point = PointF(x_coordinates[last], y_coordinates[last]);
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
/************************************************************************/
/*                     Composite Glyph Description                      */
/************************************************************************/
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

  Composite_Glyph_Description::Composite_Glyph_Description(){
    flags = 0;
    glyph_index = 0;
    argument1 = argument2 = 0;
    x_scale = scale01 = scale10 = y_scale = 0;
    number_of_instructions = 0;
    instructions = NULL;
  }

  void Composite_Glyph_Description::init(){
    USHORT max_composite_points = g_ttf->maxp.max_composite_points;
    USHORT max_composite_contours = g_ttf->maxp.max_composite_contours;
    end_pts_of_contours = new USHORT[max_composite_contours];
    flags = new BYTE[max_composite_points];
    x_coordinates = new SHORT[max_composite_points];
    y_coordinates = new SHORT[max_composite_points];
  }

  Glyph *Composite_Glyph_Description::load_glyph(Mem_Stream &msm){
    offset = msm.tell();
    // FIXME: Composite glyph rendering is still in test mode.
    // Currently I decide not to read information from composite glyph while loading.
    return this;
  }

  void Composite_Glyph_Description::glyph_to_path(GraphicsPath &path){
    Mem_Stream msm(glyf->data, glyf->length);
    msm.seek(offset);
    do{
      MREAD(msm, &flags);
      MREAD(msm, &glyph_index);
      argument1 = argument2 = 0;
      // Must reset these two arguments. Otherwise, if they are bytes in this run, you might get the wrong answer.
      if(flags & ARG_1_AND_2_ARE_WORDS){
        MREAD(msm, &argument1);
        MREAD(msm, &argument2);
      }else{
        MREAD(msm, (BYTE*)&argument1);
        MREAD(msm, (BYTE*)&argument2);
      }
      Glyph *glyph = glyf->load_glyph(glyph_index);
      GraphicsPath tmp_path;
      glyph->glyph_to_path(tmp_path);
      if(flags & ARGS_ARE_XY_VALUES){
        Matrix mtx;
        mtx.Translate(argument1, argument2);
        tmp_path.Transform(&mtx);
      }
      path.AddPath(&tmp_path, false);
      if(flags & WE_HAVE_A_SCALE){
        MREAD(msm, &x_scale);
      }else if(flags & WE_HAVE_AN_X_AND_Y_SCALE){
        MREAD(msm, &x_scale);
        MREAD(msm, &y_scale);
      }else if(flags & WE_HAVE_A_TWO_BY_TWO){
        MREAD(msm, &x_scale);
        MREAD(msm, &scale01);
        MREAD(msm, &scale10);
        MREAD(msm, &y_scale);
      }
    }while(flags & MORE_COMPONENTS);
    if(flags & WE_HAVE_INSTRUCTIONS){
      MREAD(msm, &number_of_instructions);
      // FIXME: test mode, instruction not support.
      // instructions = new BYTE[number_of_instructions];
      // FREAD_N(fin, instructions, number_of_instructions);
    }else{
      instructions = NULL;
    }
  }

  void Composite_Glyph_Description::dump_info(FILE *fp, size_t indent){
    INDENT(fp, indent); fprintf(fp, "<compositeGlyphDescription>\n");
    INDENT(fp, indent + 1); fprintf(fp, "To be continued...\n");
    INDENT(fp, indent); fprintf(fp, "</compositeGlyphDescription>\n");
  }
}