#include "stdafx.h"
#include "Glyph_Data.h"

namespace ttf_dll{
	// Bit Order of Simply Glyph Description Flag
#define ON_CURVE			0x1
#define X_SHORT_VECTOR		(ON_CURVE << 1)
#define Y_SHORT_VECTOR		(X_SHORT_VECTOR << 1)
#define REPEAT				(Y_SHORT_VECTOR << 1)
#define THIS_X_IS_SAME		(REPEAT << 1)
#define THIS_Y_IS_SAME		(THIS_X_IS_SAME << 1)

	void Simple_Glyph_Description::dump_flags(){
		printf("Simple_Glyph_Description::dump_flags\n");
		for(int i = 0; i < this->pt_num; ++i){
			if(this->flags[i] < 0x10) printf("0");
			printf("%X ", this->flags[i]);
			if((i + 1) % 5 == 0) printf("\n");
		}
		printf("\n");
	}

	void Simple_Glyph_Description::dump_coordinates(){
		printf("Simple_Glyph_Description::dump_coordinates\n");
		printf("<contour>\n");
		for(int i = 0, j = 0; i < this->pt_num; ++i){
			printf("    <pt x=\"%d\" y=\"%d\" on=\"%d\"/>\n", this->x_coordinates[i], this->y_coordinates[i],
				(this->flags[i] & ON_CURVE));
			if(i == this->end_pts_of_contours[j]){
				printf("</contour>\n");
				++j;
				if(j != number_of_contours)
					printf("<contour>\n");
			}
		}
		printf("\n");
	}

	/*
	If the last point is off-curve, the first point is regarded as the end point.
	If two off-curve point appear in a row, the midpoint of them is regarded as the implicit on-curve point.
	<prev_flag, flag>
	<0, 0>: 1. append implicit on-curve point; 2. append 'Q' and current point.
	<0, 1>: append current point
	<1, 0>: append 'Q' and current point
	<1, 1>: append 'L' and current point
	*/
	void Simple_Glyph_Description::dump_svg_outline(){
		FWORD width = x_max - x_min;
		FWORD height = y_max - y_min;
		double x_ratio = 300.0 / width;
		double y_ratio = 300.0 / height;

		printf("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
		printf("<svg xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n");
		printf("<g transform=\"scale(%.2lf, %.2lf) translate(%d, %d) matrix(1,0,0,-1,0,0)\">", x_ratio, y_ratio, -x_min, y_max);
		printf("<rect x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" fill=\"none\" stroke=\"blue\" stroke-width=\"10\"/>\n", x_min, y_min, width, height);
		printf("<path d=\"");
		BYTE prev_flag = 1, flag = 1;
		bool new_contour = true;
		for(int i = 0, j = 0; i < this->pt_num; ++i){
			flag = this->flags[i] & ON_CURVE;
			if(prev_flag == 0 && flag == 0){ // append implicit on-curve point
				printf("%d, %d ", (this->x_coordinates[i - 1] + this->x_coordinates[i]) >> 1,
					(this->y_coordinates[i - 1] + y_coordinates[i]) >> 1);	
			}
			if(new_contour){
				printf("M");
				new_contour = false;
			}else if(flag == 0){
				printf("Q");
			}else if(prev_flag == 1){
				printf("L");
			}
			printf("%d, %d ", this->x_coordinates[i], this->y_coordinates[i]);
			if(i == this->end_pts_of_contours[j]){
				if(flag == 0){
					if(j == 0){
						printf("%d, %d", this->x_coordinates[0], this->y_coordinates[0]);
					}else{
						printf("%d, %d", this->x_coordinates[this->end_pts_of_contours[j - 1] + 1],
							this->y_coordinates[this->end_pts_of_contours[j - 1] + 1]);
					}
				}
				printf(" Z ");
				new_contour = true;
				++j;
			}
			prev_flag = flag;
		}
		printf("\"/>\n");
		printf("</g>");
		printf("</svg>");
	}

	void Simple_Glyph_Description::read_flags(ifstream &fin){
		BYTE flag = 0;	
		this->flags = new BYTE[pt_num];
		for(int i = 0; i < this->pt_num;){
			ifstream_read_big_endian(fin, &flag, sizeof(BYTE));
			this->flags[i++] = flag;
			if(flag & REPEAT){
				BYTE repeat_num = 0;
				ifstream_read_big_endian(fin, &repeat_num, sizeof(BYTE));
				while(repeat_num-- > 0){
					this->flags[i++] = flag;
				}
			}
		}
	}

	void Simple_Glyph_Description::read_coordinates(ifstream& fin, SHORT *ptr, bool read_x){
		SHORT last = 0;
		BYTE flag = 0;
		BYTE SHORT_VECTOR = X_SHORT_VECTOR << (read_x? 0: 1);
		BYTE IS_SAME = THIS_X_IS_SAME << (read_x? 0: 1);
		for(int i = 0; i < this->pt_num; ++i, ++ptr){
			flag = this->flags[i];
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

	Simple_Glyph_Description::Simple_Glyph_Description(Glyph_Data &gd, ifstream &fin):Glyph_Data(gd){
		this->end_pts_of_contours = new USHORT[this->number_of_contours];
		ifstream_read_big_endian(fin, this->end_pts_of_contours,
			sizeof(USHORT), this->number_of_contours);

		ifstream_read_big_endian(fin, &this->instruction_length, sizeof(USHORT));
		this->instructions = new BYTE[this->instruction_length];
		ifstream_read_big_endian(fin, this->instructions,
			sizeof(BYTE), this->instruction_length);

		this->pt_num = this->end_pts_of_contours[this->number_of_contours - 1] + 1;
		this->read_flags(fin);
		this->x_coordinates = new SHORT[pt_num];
		this->read_coordinates(fin, this->x_coordinates, true);
		this->y_coordinates = new SHORT[pt_num];
		this->read_coordinates(fin, this->y_coordinates, false);
		//this->dump_flags(); // TEST
		//this->dump_coordinates(); // TEST
	}

	// Bit Order of Composite Glyph Description Flag
#define ARG_1_AND_2_ARE_WORDS		0x1
#define ARGS_ARE_XY_VALUES			(ARG_1_AND_2_ARE_WORDS << 1)
#define ROUND_XY_TO_GRID			(ARGS_ARE_XY_VALUES << 1)
#define WE_HAVE_A_SCALE				(ROUND_XY_TO_GRID << 1)
#define MORE_COMPONENTS				(WE_HAVE_A_SCALE << 2)
#define WE_HAVE_AN_X_AND_Y_SCALE	(MORE_COMPONENTS << 1)
#define WE_HAVE_A_TWO_BY_TWO		(WE_HAVE_AN_X_AND_Y_SCALE << 1)
#define WE_HAVE_INSTRUCTIONS		(WE_HAVE_A_TWO_BY_TWO << 1)
#define USE_MY_METRICS				(WE_HAVE_INSTRUCTIONS << 1)
	Composite_Glyph_Description::Composite_Glyph_Description(Glyph_Data &gd, ifstream &fin):Glyph_Data(gd){
		ifstream_read_big_endian(fin, &this->flags, sizeof(USHORT));
		ifstream_read_big_endian(fin, &this->glyph_index, sizeof(USHORT));
		if(this->flags & ARG_1_AND_2_ARE_WORDS){
			ifstream_read_big_endian(fin, &this->argument1, sizeof(SHORT));
			ifstream_read_big_endian(fin, &this->argument1, sizeof(SHORT));
		}else{
			ifstream_read_big_endian(fin, &this->argument1, sizeof(BYTE));
			ifstream_read_big_endian(fin, &this->argument1, sizeof(BYTE));
		}
	}

	Glyph_Data::Glyph_Data(ifstream &fin, ULONG offset){
		fin.seekg(offset, ios::beg);
		ifstream_read_big_endian(fin, &this->number_of_contours, sizeof(SHORT));
		ifstream_read_big_endian(fin, &this->x_min, sizeof(FWORD));
		ifstream_read_big_endian(fin, &this->y_min, sizeof(FWORD));
		ifstream_read_big_endian(fin, &this->x_max, sizeof(FWORD));
		ifstream_read_big_endian(fin, &this->y_max, sizeof(FWORD));
	}

	Glyph_Data* Glyph_Data::load_table(ifstream &fin, ULONG offset){
		Glyph_Data glyph_data(fin, offset);
		if(glyph_data.is_simply_glyph()){
			return new Simple_Glyph_Description(glyph_data, fin);
		}
		if(glyph_data.is_composite_glyph()){
			return new Composite_Glyph_Description(glyph_data, fin);
		}
		// FIXME: 0, no glyph data; -2, -3, ... furture use. Maybe they should not be treated as errors.  
		printf("Error in Glyph_Data::load_table: number_of_contours = %d, offset=%ul\n",
			glyph_data.number_of_contours, offset);
		return NULL;
	}
}