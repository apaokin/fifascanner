#include <fstream>
#include <vector>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <string>
#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm> 
#include <fstream>
//using namespace std;

class RowBeginEnd
{
	int x_beg, x_end, row;


public:
	bool checked = false;
	int get_row();
	
	int get_beg();
	
	int get_end();
	
	RowBeginEnd(int, int, int);
	
	friend std::ostream& operator<<(std::ostream& , const RowBeginEnd& );

};
//std::ostream& operator<<(std::ostream&, const RowBeginEnd&);



struct Numbers4
{
	int x1, y1, x2, y2;

	Numbers4(int, int, int, int);



	friend std::ostream& operator<<(std::ostream& , const Numbers4& );


};

//std::ostream& operator<<(std::ostream&, const Numbers4&);








class Screen
{
private:
	Pix * pix;
	std::ofstream fout;
	std::vector < Numbers4 > buf;
	std::vector < std::vector<RowBeginEnd> > table;
	static const l_uint32 white = 150;
	static const l_uint32 max_s = 25;// maximum distance between letters
	static const l_uint32 min_col = 700; // minimum column to start from
	static const l_uint32 min_row = 200; // minimum row to start from
	static const l_uint32 min_side = 10; // minimum length of  side  
	static const l_uint32 cols_max_number = max_s; //minimum distance between x 
	static const l_uint32 cols_part = 100;
	static const l_uint32 rows_max_number = 50;
	static const l_uint32 border = 10;
	static const l_uint32 min_dim = 10;
	
	public:
		Screen(char* file_name);

		void get_data();

		void remove_intersecs();

		void get_score();

		void black_box(int, int , int , int );
		
		void draw_box(int , int , int , int );
		
		void draw_box(const Numbers4 & );
		void proccess_one_elem(RowBeginEnd& , int , int , int , int );
};
