#include "stdafx.h"
#include "Glyph_Data.h"

namespace ttf_dll{
  static Glyph_Data *glyf;
  static Simple_Glyph_Description       simple_glyph;
  static Composite_Glyph_Description    composite_glyph;
/************************************************************************/
/*                             Glyph Data                               */
/************************************************************************/
  void Glyph_Data::load_table(Table_Directory_Entry *entry, ifstream &fin, USHORT max_p, USHORT max_c, USHORT max_instr){
    max_points = max_p;
    max_contours = max_c;
    max_size_of_instructions = max_instr;
    data = NULL;
    fin.seekg(entry->offset, ios::beg);
    length = entry->length;
    data = new BYTE[length];
    fin.read((char*)data, length);

    simple_glyph.init(max_points, max_contours, max_size_of_instructions);
    glyf = this;
  }

  void Glyph_Data::clear(){
    simple_glyph.destroy();
    glyf = NULL;
  }

  static bool is_simply_glyph(SHORT number_of_contours){
    return number_of_contours >= 0;
  }

  Glyph *Glyph_Data::load_glyph(USHORT offset){ // FIXME: no proper. use glyph_index
    // Read 'number_of_contours' to determine the format of the glyph.
    Mem_Stream msm(data, length);
    msm.seek(offset);
    SHORT number_of_contours = 0;
    MREAD(msm, &number_of_contours);
    if(number_of_contours > max_contours){ // FIXME: Mathematica6.ttf has some glyph with number_of_contours greater than max_contours.
      return NULL;
    }
    Glyph *glyph = NULL;
    if(is_simply_glyph(number_of_contours)){
      glyph = &simple_glyph;
    }else{
      glyph = &composite_glyph;
    }
    glyph->number_of_contours = number_of_contours;
    glyph->load_glyph_header(msm);
    glyph->load_glyph(msm);
    return glyph;
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
/*                              Glyph                                   */
/************************************************************************/
  Glyph::Glyph(){
    number_of_contours = 0;
    x_min = y_min = x_max = y_max = 0;
  }

  void Glyph::load_glyph_header(Mem_Stream &msm){
    // 'number_of_contours' is already read in Glyph_Data::load_glyph.
    MREAD(msm, &x_min);
    MREAD(msm, &y_min);
    MREAD(msm, &x_max);
    MREAD(msm, &y_max);
  }
/************************************************************************/
/*                     Simple Glyph Description                         */
/************************************************************************/
  // Bit Order of Simply Glyph Description Flag
  enum Simple_Glyph_Description_Flag{ // FIXME: This enum should be defined in the declaration of class.
    ON_CURVE            = BIT(0),
    X_SHORT_VECTOR      = BIT(1),
    Y_SHORT_VECTOR      = BIT(2),
    REPEAT              = BIT(3),
    THIS_X_IS_SAME      = BIT(4),
    THIS_Y_IS_SAME      = BIT(5)
  };

  Simple_Glyph_Description::Simple_Glyph_Description(){
    end_pts_of_contours = NULL;
    instruction_length = 0;
    instructions = NULL;
    flags = NULL;
    x_coordinates = NULL;
    y_coordinates = NULL;
    pt_num = 0;
  }

  // This function works only when a new font is loaded.
  void Simple_Glyph_Description::init(USHORT max_contours, USHORT max_points, USHORT max_size_of_instructions){
    end_pts_of_contours = new USHORT[max_contours];
    instructions = new BYTE[max_size_of_instructions];
    flags = new BYTE[max_points];
    x_coordinates = new SHORT[max_points];
    y_coordinates = new SHORT[max_points];
  }

  void Simple_Glyph_Description::destroy(){
    delete[] end_pts_of_contours;
    end_pts_of_contours = NULL;
    delete[] instructions;
    instructions = NULL;
    delete[] flags;
    flags = NULL;
    delete[] x_coordinates;
    x_coordinates = NULL;
    delete[] y_coordinates;
    y_coordinates = NULL;
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

  // Simple_Glyph_Description::Simple_Glyph_Description(ifstream &fin) : Glyph(fin){
  //   end_pts_of_contours = new USHORT[number_of_contours];
  //   FREAD_N(fin, end_pts_of_contours, number_of_contours);

  //   FREAD(fin, &instruction_length);
  //   instructions = new BYTE[instruction_length];
  //   FREAD_N(fin, instructions, instruction_length);

  //   pt_num = end_pts_of_contours[number_of_contours - 1] + 1;
  //   read_flags(fin);
  //   x_coordinates = new SHORT[pt_num];
  //   read_coordinates(fin, x_coordinates, true);
  //   y_coordinates = new SHORT[pt_num];
  //   read_coordinates(fin, y_coordinates, false);
  // }

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

  void add_quadratic_bezier(GraphicsPath &path, const PointF &q0, const PointF &q1, const PointF &q2){
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
    for(int i = 0, j = 0; i < pt_num; ++i){
    // i stands for the index of cur_point, while j stands for the index of current contour.
      flag = flags[i] & ON_CURVE;
      cur_point = PointF(x_coordinates[i], y_coordinates[i]);

      if(new_contour){ // If this is the first point of jth contour.
        path.StartFigure();
        new_contour = false;
        // Calculate the prev_point.
        // <last_flag, flag>
        // <0, 0>: prev_point = mid(last_point, cur_point);
        // <0, 1>: prev_point = cur_point
        // <1, *>: prev_point = last_point
        int last = end_pts_of_contours[j];
        if(flags[last] == ON_CURVE){ // If the last point is on-curve, it'll be the prev_point of the first point.
          prev_point = PointF(x_coordinates[last], y_coordinates[last]);
        }else{ // If the last point is off-curve...
          if(flag == ON_CURVE){ // if the first point is on-curve, no need to draw anything.
            prev_point = cur_point;
          }else{ // Otherwise, the prev_point is the mid-point between the first and the last point (both off-curve).
            prev_point = PointF(
              (cur_point.X + x_coordinates[last]) / 2.0f,
              (cur_point.Y + y_coordinates[last]) / 2.0f
              );
          }
        }
        start_point = prev_point; // start_point is not simply first point.
      }

      // Draw something.
      // <flag, next_flag>
      // <0, 0>: add_quadratic_bezier(prev_point, cur_point, implicit_next_point)
      // <0, 1>: add_quadratic_bezier(prev_point, cur_point, next_point).
      // <1, *>: AddLine(prev_point, cur_point)
      if(flag == ON_CURVE){ // If this point is on-curve, just draw a straight line.
        path.AddLine(prev_point, cur_point);
        prev_point = cur_point;
      }else{ // Otherwise, draw a quadratic bezier.
        // Calculate the next point.
        PointF next_point;
        if(i == end_pts_of_contours[j]){ // If this is the last point, the start_point is served as next_point.
          next_point = start_point;
        }else{ // Otherwise...
          BYTE next_flag = flags[i + 1];
          if(next_flag == ON_CURVE){ // If the next point is on-curve, it'll be the next_point.
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
      if(i == end_pts_of_contours[j]){ // If the last point is visited, close figure:)
        path.CloseFigure();
        new_contour = true;
        ++j;
      }
    }
  }

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
    argument1 = 0;
    argument2 = 0;
    x_scale = 0;
    scale01 = 0;
    scale10 = 0;
    y_scale = 0;
    number_of_instructions = 0;
    instructions = NULL;
  }

  Glyph *Composite_Glyph_Description::load_glyph(Mem_Stream &msm){
    return NULL;
  }
  //Composite_Glyph_Description::Composite_Glyph_Description(ifstream &fin) : Glyph(fin){
  //  do{
  //    FREAD(fin, &flags);
  //    FREAD(fin, &glyph_index);
  //    if(flags & ARG_1_AND_2_ARE_WORDS){
  //      FREAD(fin, &argument1);
  //      FREAD(fin, &argument2);
  //    }else{
  //      FREAD(fin, (BYTE*)&argument1);
  //      FREAD(fin, (BYTE*)&argument2);
  //    }
  //    if(flags & WE_HAVE_A_SCALE){
  //      FREAD(fin, &x_scale);
  //    }else if(flags & WE_HAVE_AN_X_AND_Y_SCALE){
  //      FREAD(fin, &x_scale);
  //      FREAD(fin, &y_scale);
  //    }else if(flags & WE_HAVE_A_TWO_BY_TWO){
  //      FREAD(fin, &x_scale);
  //      FREAD(fin, &scale01);
  //      FREAD(fin, &scale10);
  //      FREAD(fin, &y_scale);
  //    }
  //  }while(flags & MORE_COMPONENTS);
  //  if(flags & WE_HAVE_INSTRUCTIONS){
  //    FREAD(fin, &number_of_instructions);
  //    instructions = new BYTE[number_of_instructions];
  //    FREAD_N(fin, instructions, number_of_instructions);
  //  }else{
  //    instructions = NULL;
  //  }
  //}

  void Composite_Glyph_Description::dump_info(FILE *fp, size_t indent){
    INDENT(fp, indent); fprintf(fp, "<compositeGlyphDescription>\n");
    INDENT(fp, indent + 1); fprintf(fp, "To be continued...\n");
    INDENT(fp, indent); fprintf(fp, "</compositeGlyphDescription>\n");
  }
}