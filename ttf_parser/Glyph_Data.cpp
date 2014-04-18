#include "stdafx.h"
#include "Glyph_Data.h"

namespace ttf_dll{
/******************************* Glyph_Data ***********************************/
  Glyph_Data::Glyph_Data(ifstream &fin, ULONG offset){
    fin.seekg(offset, ios::beg);
    ifstream_read_big_endian(fin, &number_of_contours, sizeof(SHORT));
    ifstream_read_big_endian(fin, &x_min, sizeof(FWORD));
    ifstream_read_big_endian(fin, &y_min, sizeof(FWORD));
    ifstream_read_big_endian(fin, &x_max, sizeof(FWORD));
    ifstream_read_big_endian(fin, &y_max, sizeof(FWORD));
  }

  Glyph_Data* Glyph_Data::create_glyph_data(ifstream &fin, ULONG offset, USHORT max_contours){
    SHORT number_of_contours = 0;
    fin.seekg(offset, ios::beg);
    ifstream_read_big_endian(fin, &number_of_contours, sizeof(number_of_contours));
    if(number_of_contours > max_contours){ // FIXME: Mathematica6.ttf has some glyph with number_of_contours greater than max_contours.
      return NULL;
    }
    if(is_simply_glyph(number_of_contours)){
      return new Simple_Glyph_Description(fin, offset);
    }else{
      return new Composite_Glyph_Description(fin, offset);
    }
  }
/******************************* Simple_Glyph_Description ***********************************/
  // Bit Order of Simply Glyph Description Flag
  enum Simple_Glyph_Description_Flag{ // FIXME: This enum should be defined in the declaration of class.
    ON_CURVE            = 0x1,
    X_SHORT_VECTOR      = (ON_CURVE << 1),
    Y_SHORT_VECTOR      = (X_SHORT_VECTOR << 1),
    REPEAT              = (Y_SHORT_VECTOR << 1),
    THIS_X_IS_SAME      = (REPEAT << 1),
    THIS_Y_IS_SAME      = (THIS_X_IS_SAME << 1)
  };

  Simple_Glyph_Description::Simple_Glyph_Description(ifstream &fin, ULONG offset)
  : Glyph_Data(fin, offset){
    end_pts_of_contours = new USHORT[number_of_contours];
    ifstream_read_big_endian(fin, end_pts_of_contours,
      sizeof(USHORT), number_of_contours);

    ifstream_read_big_endian(fin, &instruction_length, sizeof(USHORT));
    instructions = new BYTE[instruction_length];
    ifstream_read_big_endian(fin, instructions,
      sizeof(BYTE), instruction_length);

    pt_num = end_pts_of_contours[number_of_contours - 1] + 1;
    read_flags(fin);
    x_coordinates = new SHORT[pt_num];
    read_coordinates(fin, x_coordinates, true);
    y_coordinates = new SHORT[pt_num];
    read_coordinates(fin, y_coordinates, false);
    //dump_flags(); // TEST
    //dump_coordinates(); // TEST
  }

  void Simple_Glyph_Description::read_flags(ifstream &fin){
    BYTE flag = 0;  
    flags = new BYTE[pt_num];
    for(int i = 0; i < pt_num;){
      ifstream_read_big_endian(fin, &flag, sizeof(BYTE));
      flags[i++] = flag;
      if(flag & REPEAT){
        BYTE repeat_num = 0;
        ifstream_read_big_endian(fin, &repeat_num, sizeof(BYTE));
        while(repeat_num-- > 0){
          flags[i++] = flag;
        }
      }
    }
  }

  void Simple_Glyph_Description::read_coordinates(ifstream& fin, SHORT *ptr, bool read_x){
    SHORT last = 0;
    BYTE flag = 0;
    BYTE SHORT_VECTOR = X_SHORT_VECTOR << (read_x? 0: 1);
    BYTE IS_SAME = THIS_X_IS_SAME << (read_x? 0: 1);
    for(int i = 0; i < pt_num; ++i, ++ptr){
      flag = flags[i];
      *ptr = 0;
      if(flag & SHORT_VECTOR){
        ifstream_read_big_endian(fin, ptr, sizeof(BYTE));
        if(~flag & IS_SAME){
          *ptr = -*ptr;
        }
      }else{
        if(~flag & IS_SAME){
          ifstream_read_big_endian(fin, ptr, sizeof(SHORT));
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
      printf("    <pt x=\"%d\" y=\"%d\" on=\"%d\"/>\n", x_coordinates[i], y_coordinates[i],
        (flags[i] & ON_CURVE));
      if(i == end_pts_of_contours[j]){
        printf("</contour>\n");
        ++j;
        if(j != number_of_contours)
          printf("<contour>\n");
      }
    }
    printf("\n");
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
/******************************* Composite_Glyph_Description ***********************************/
  // Bit Order of Composite Glyph Description Flag
  enum Composite_Glyph_Description_Flag{
    ARG_1_AND_2_ARE_WORDS         = 0x1,
    ARGS_ARE_XY_VALUES            = (ARG_1_AND_2_ARE_WORDS << 1),
    ROUND_XY_TO_GRID              = (ARGS_ARE_XY_VALUES << 1),
    WE_HAVE_A_SCALE               = (ROUND_XY_TO_GRID << 1),
    // Bit 4 is reserved and set to 0.
    MORE_COMPONENTS               = (WE_HAVE_A_SCALE << 2),
    WE_HAVE_AN_X_AND_Y_SCALE      = (MORE_COMPONENTS << 1),
    WE_HAVE_A_TWO_BY_TWO          = (WE_HAVE_AN_X_AND_Y_SCALE << 1),
    WE_HAVE_INSTRUCTIONS          = (WE_HAVE_A_TWO_BY_TWO << 1),
    USE_MY_METRICS                = (WE_HAVE_INSTRUCTIONS << 1)
  };

  Composite_Glyph_Description::Composite_Glyph_Description(ifstream &fin, ULONG offset)
  : Glyph_Data(fin, offset){
    ifstream_read_big_endian(fin, &flags, sizeof(USHORT));
    ifstream_read_big_endian(fin, &glyph_index, sizeof(USHORT));
    if(flags & ARG_1_AND_2_ARE_WORDS){
      ifstream_read_big_endian(fin, &argument1, sizeof(SHORT));
      ifstream_read_big_endian(fin, &argument1, sizeof(SHORT));
    }else{
      ifstream_read_big_endian(fin, &argument1, sizeof(BYTE));
      ifstream_read_big_endian(fin, &argument1, sizeof(BYTE));
    }
  }
}