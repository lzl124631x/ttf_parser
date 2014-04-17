#ifndef GLYPH_DATA_H
#define GLYPH_DATA_H
#include "Type.h"
#include "TTF_Table.h"
#include <objidl.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")
/******************************* glyf ***********************************/
namespace ttf_dll{
	class DLL_API Glyph_Data{ // FIXME: consider to change this class into a pure vitual class
	private:
		Glyph_Data(ifstream &fin, ULONG offset);
	public:
		SHORT	number_of_contours; // If the 'number_of_contours' is greater than or equal to zero, this is a single glyph; if negative, this is a composite glyph.
		FWORD	x_min;
		FWORD	y_min;
		FWORD	x_max;
		FWORD	y_max;
		Glyph_Data(Glyph_Data &gd)
			:number_of_contours(gd.number_of_contours),
			x_min(gd.x_min), y_min(gd.y_min),
			x_max(gd.x_max), y_max(gd.y_max){}
		static Glyph_Data* load_table(ifstream &fin, ULONG offset, USHORT max_contours);
    virtual ~Glyph_Data(){};
		inline bool Glyph_Data::is_simply_glyph(){
			return number_of_contours >= 0; // Otherwise this is a composite glyph.	
		}
		virtual void dump_coordinates(){}
		virtual void dump_svg_outline(FILE *fp){}
		virtual void glyph_to_path(GraphicsPath &path){}
	};

	class DLL_API Simple_Glyph_Description: public Glyph_Data{
	private:
		void read_flags(ifstream &fin);
		void read_coordinates(ifstream& fin, SHORT *ptr, bool read_x);
		void dump_flags();
		void dump_coordinates();
		void dump_svg_outline(FILE *fp);
		void glyph_to_path(GraphicsPath &path);
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
		~Simple_Glyph_Description(){
			delete[] end_pts_of_contours;
      delete[] instructions;
      delete[] flags;
      delete[] x_coordinates;
      delete[] y_coordinates;
		}
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