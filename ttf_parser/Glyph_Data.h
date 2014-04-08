#ifndef GLYPH_DATA_H
#define GLYPH_DATA_H
#include "Type.h"
#include "TTF_Table.h"
/******************************* glyf ***********************************/
namespace ttf_dll{
	class DLL_API Glyph_Data{
	private:
		Glyph_Data(ifstream &fin, ULONG offset);
	public:
		SHORT	number_of_contours;
		FWORD	x_min;
		FWORD	y_min;
		FWORD	x_max;
		FWORD	y_max;
		Glyph_Data(Glyph_Data &gd)
			:number_of_contours(gd.number_of_contours),
			x_min(gd.x_min), y_min(gd.y_min),
			x_max(gd.x_max), y_max(gd.y_max){}
		static Glyph_Data* load_table(ifstream &fin, ULONG offset);
		inline bool Glyph_Data::is_simply_glyph(){
			return this->number_of_contours > 0;	
		}
		inline bool Glyph_Data::is_composite_glyph(){
			return this->number_of_contours == -1;
		}
		virtual void dump_coordinates(){}
		virtual void dump_svg_outline(){}
	};

	class DLL_API Simple_Glyph_Description: public Glyph_Data{
	private:
		void read_flags(ifstream &fin);
		void read_coordinates(ifstream& fin, SHORT *ptr, bool read_x);
		void dump_flags();
		void dump_coordinates();
		void dump_svg_outline();
	public:
		USHORT	*end_pts_of_contours;
		USHORT	instruction_length;
		BYTE	*instructions; /* [instruction_length] */
		BYTE	*flags;/* [number_of_points] */
		SHORT	*x_coordinates;/* [number_of_points] */
		SHORT	*y_coordinates;/* [number_of_points] */
		USHORT	pt_num;/* the number of points in this glyph */
		// Points are indexed from 0. end_pts_of_contours stores the index of each contour's end point.
		// The last contour's end point has the largest index which equals pt_num - 1.
		Simple_Glyph_Description(Glyph_Data &gd, ifstream &fin);
	};

	class Composite_Glyph_Description: public Glyph_Data{
	public:
		USHORT	flags;
		USHORT	glyph_index;
		SHORT	argument1;
		SHORT	argument2;
		Composite_Glyph_Description(Glyph_Data &gd, ifstream &fin);
	};
}
#endif