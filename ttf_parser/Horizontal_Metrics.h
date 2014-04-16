#ifndef HORIZONTAL_METRICS_H
#define HORIZONTAL_METRICS_H

#include "Type.h"
#include "TTF_Table.h"
/******************************* hmtx ***********************************/
namespace ttf_dll{
	class True_Type_Font; // FIXME: How about remove this declaration?
	typedef struct _Long_Hor_Metric{
		UFWORD	advance_width;
		FWORD	lsb;
	}Long_Hor_Metric;

	class DLL_API Horizontal_Metrics{
	public:
		Long_Hor_Metric	*hmetrics;/* [number_of_hmetrics(from hhea)] */
		FWORD			*left_side_bearing;/* [num_glyphs(from maxp) - number_of_hmetrics] */
		void load_table(Table_Directory_Entry *entry, ifstream &fin, True_Type_Font *ttf);
		~Horizontal_Metrics(){
			delete[] hmetrics, left_side_bearing;
		}
	};
}
#endif