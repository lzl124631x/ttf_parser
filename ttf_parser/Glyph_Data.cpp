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
    Glyph_Loader loader(glyph);
    Mem_Stream msm(data, length);
    loader.load_glyph(msm, glyph_index);
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
  void Glyph_Loader::load_glyph(Mem_Stream &msm, USHORT glyph_index){
	  // Read 'number_of_contours' to determine the format of the glyph.
	  ULONG offset = g_ttf->glyph_index_to_offset(glyph_index);
    //msm.reset();// Need reset the cursor
	  msm.seek(offset);
    Glyph_Header header;
	  header.load_glyph_header(msm);
    if(glyph.header.loaded == false){ // If this is the first time to load this glyph
      glyph.header = header;
    }
    if(is_simply_glyph(header.num_contours)){
      load_simple_glyph(msm);
    }else if(is_composite_glyph(header.num_contours)){
      load_composite_glyph(msm);
    }
  }
/************************************************************************/
/*                              Glyph                                   */
/************************************************************************/
  void Glyph_Header::init(){
	  num_contours = 0;
	  x_min = y_min = x_max = y_max = 0;
    loaded = false;
  }

  void Glyph::init(){
    header.init();
    end_contours = NULL;
    flags = NULL;
    x_coordinates = y_coordinates = NULL;
  }

  bool Glyph_Header::load_glyph_header(Mem_Stream &msm){
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

  void Glyph_Loader::load_simple_glyph(Mem_Stream &msm){
    MREAD_N(msm, glyph.end_contours, glyph.header.num_contours);
    MREAD(msm, &glyph.instr_len);
    if(glyph.instr_len > g_ttf->maxp.max_size_of_instructions){
      return;
    }
    //MREAD_N(msm, instructions, instruction_length);
    msm.seek(sizeof(BYTE) * glyph.instr_len); // FIXME: skip instructions
    pt_num = end_contours[glyph.header.num_contours - 1] + 1;
    read_flags(msm);
    read_coordinates(msm, x_coordinates, true);
    read_coordinates(msm, y_coordinates, false);
    return;
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
}