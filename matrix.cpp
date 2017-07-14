
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <string>
#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm> 
#include <fstream>
using namespace std;
class RowBeginEnd
{
	int x_beg, x_end,row;
	
	
public:
	bool checked = false;
	int get_row()
	{
		return row;
	}
	int get_beg()
	{
		return x_beg;
	}
	int get_end()
	{
		return x_end;
	}
	RowBeginEnd(int  x_beg2, int x_end2,int  row2)
	{
		x_beg = x_beg2;
		x_end = x_end2;
		row = row2;
	}
	friend ostream& operator<<(ostream& os, const RowBeginEnd& dt);

};
ostream& operator<<(ostream& os, const RowBeginEnd& dt)
{
	os << dt.row << '|' << dt.x_beg << '|' << dt.x_end;
	return os;
}

struct Numbers4
{
	int x1, y1, x2,y2;

	Numbers4(int a, int b, int c, int d) : x1(a), y1(b), x2(c), y2(d) {};

	

	friend ostream& operator<<(ostream& os, const Numbers4& dt);
	

};

ostream& operator<<(ostream& os, const Numbers4& dt)
{
	os << dt.x1 << '|' << dt.y1 << '|' << dt.x2<<"|"<< dt.y2;
	return os;
}





class Screen
{
private:
	Pix *pix;
	ofstream fout;
	vector< Numbers4 > buf;
	vector < vector<RowBeginEnd> > table;
	static const l_uint32 white = 150;
	static const l_uint32 max_s = 25;
	static const l_uint32 min_col = 700;
	static const l_uint32 min_row = 200;
	static const l_uint32 min_dim = 10;
	static const l_uint32 cols_max_number = max_s;
	static const l_uint32 cols_part = 100;
	static const l_uint32 rows_max_number = 50;
	static const l_uint32 border = 10;

	//static const l_uint32 min_row = 200;


public:
	Screen(char* file_name)
	{
	
		
		fout.open("result.txt",ios::app);
		pix = NULL;
		pix = pixCopy(pix,  pixRead(file_name));
		
	}
	void get_rows()
	{
		l_int32 r, g, b;
		l_uint32 *d = pix->data;
		
		vector<RowBeginEnd> row;
		
		
		l_uint32 s;
		//FLAGS
		bool begin_met;
		int not_white_count;
		l_uint32 last_end,last_begin;
	
		
		
		for (l_uint32 i = min_row; i < pix->h; i++)
		{
			row.clear();
			begin_met = false;
			s = 0;
			for (l_uint32 j = min_col; j < pix->w; j++)
			{
				
				
				extractRGBValues(d[i*(pix->wpl) + j], &r, &g, &b);

				if ( (r>white) && (g>white) && (b>white) )  
				{
					
					
					
					if (!begin_met)
					{
						begin_met = true;
						last_begin = j;

						
					}
					last_end = j;
					s = 0;
					//Если белый, то проверяем расстояние до предыдущего белого.
					
					
				}

				else
				{
					d[i*(pix->wpl) + j] = 0;
					s++;
					if (s > max_s && begin_met)
					{
						row.emplace_back(RowBeginEnd(last_begin, last_end, i));
						
						begin_met = false;
					}

				}


			}
			if (row.size() > 0)
			{
				table.emplace_back(row);
			}
			

			
		}
		
	
		for (int i = 0; i < table.size(); i++)
		{
		
			auto row = table[i];
			for (auto d : row)
			{
			
				

				
			}

		}

	
		
		for (int i=0; i < table.size()-1;i++ )
		{
			auto row = table[i];
			for (auto d : row)
			{
				
				if (!d.checked)
				{
					
					d.checked = true; 
				
					proccess_one_elem(d, d.get_beg(), d.get_end(), d.get_row(),i);
					
				
				}
			}
			
		}
		
		remove_intersecs();

		

		get_score();

		

		//pixDisplay(pix, 0, 0);

	
	}
	
	void remove_intersecs()
	{
		vector<vector< Numbers4 >::iterator> delete_pos;
		vector< Numbers4 >::iterator it;
		vector< Numbers4 >::iterator jt;
		for ( it = buf.begin(); it != buf.end(); ++it)
		{
			 for (jt = buf.begin(); jt != buf.end(); ++jt)
			{
				 auto s = *it;
				 auto d = *jt;
				bool b1 = ((s.x1 >= d.x1) && (s.x1 <= d.x2) && (s.y1 >= d.y1) && (s.y1 <= d.y2));
				bool b2 = ((s.x2 >= d.x1) && (s.x2 <= d.x2) && (s.y2 >= d.y1) && (s.y2 <= d.y2));
				if ( ( b1 || b2 ) &&  it!=jt)
				{
					delete_pos.emplace_back(it);
				}
			}		
		}
		for (vector< Numbers4 >::iterator & f : delete_pos)
		{
			buf.erase(f);
		}
	}
	
	void get_score()
	{
		
		
		vector< Numbers4 >::iterator it;
		vector< Numbers4 >::iterator jt;
		bool b;
		int s1=0;
		for (it = buf.begin(); it != buf.end(); ++it)
		{
			b = false;
			auto& s = *it;
			for (jt = it; jt != buf.end(); ++jt)
			{
				Numbers4 & n = *jt;
				if ( n.y1 - s.y2 < rows_max_number && n.y2 != s.y2 && abs(n.x1 - s.x1) < cols_part )
				{

					b = true;
					break;
				}


			}
			
		
			if (b)
			{
				
				s1++;


				draw_box(*jt);		
				fout << "Result:    ";
				draw_box(s);
				fout << endl;
				buf.erase(jt);
				if (s1 == 11) return;
			}
			
		}
		fout.close();

	}
	

	

	void black_box(int x1,int y1, int x2,int y2)
	{
		
		
		
		auto d = pix->data;
	

		for (l_uint32 i = y1; i < y2; i++)
		{
			for (l_uint32 j = x1; j < x2; j++)
			{
				d[i*(pix->wpl) + j] = 0;
			}
		}

	}
	void draw_box(int x1, int y1, int x2, int y2)
	{

		BOX* cropWindow = boxCreate(x1 - border, y1 - border, 2*border + x2-x1, 2*border + y2-y1);
		PIX* croppedImage = pixClipRectangle(pix, cropWindow, NULL);
		tesseract::TessBaseAPI ocr;
		ocr.Init(NULL, "eng");
		ocr.SetImage(croppedImage);
		string result = ocr.GetUTF8Text();

		fout << result ;

		//pixDisplay(croppedImage, 0, 0);

	}

	void draw_box(const Numbers4 & s)
	{

		draw_box(s.x1, s.y1, s.x2, s.y2);



	}

	
	


	void proccess_one_elem(RowBeginEnd& in, int min_x, int max_x,int min_y,int i )
	{
		//cout << in<< " "<<i<<endl;
		if (i == table.size() - 1)
		{

			return;


		}
		
		int minx = 0, maxx = 0;
		//cout <<  table[i+1][0].get_row()<<" ";
		//cout <<in.get_row() + 1<<" "<<  ( in.get_row() + 1 == table[i + 1][0].get_row() ) <<endl;
		
		
		if ( table[i+1][0].get_row() == in.get_row() + 1 )
		{

			
			
			for (RowBeginEnd& d : table[i + 1])
			{
				
				if (abs(in.get_beg() - d.get_beg()) < cols_max_number)	
				{
					
					d.checked = true;
					minx = (in.get_beg() < min_x) ? in.get_beg() : min_x;
					maxx = (in.get_end() > max_x) ? in.get_end() : max_x;
					proccess_one_elem(d, minx, maxx, min_y,i+1);
					return;
				}
			}
			

		}
		
		{
			
			{
				
				if (min_x - max_x >= min_dim &&  in.get_row() - min_y >= min_dim)
				{
					buf.emplace_back(Numbers4(min_x, min_y, max_x, in.get_row() ) );
					
				}
				



			}
			
	
		}

		
		

	}
	
};

class CutImage
{

};

int main(int argc, char* argv[])
{


     const l_uint32 white=150;
	tesseract::TessBaseAPI ocr;
	ocr.Init(NULL, "eng");
    PIX* pix1 = pixRead("fiftest1.jpg"),*pix2;
	Screen screen("fiftest1.jpg");
	screen.get_rows();
	Screen screen2("fiftest2.jpg");
	screen2.get_rows();
    l_int32 r,g,b,a;
	
    unsigned *con;

	

    pix2=NULL;
    pix2=pixCopy(pix2,pix1);
	l_uint32 *d = pix2->data;
	
    for (l_uint32 i=0;i<pix2->h;i++)
    for (l_uint32 j=0;j<pix2->w;j++)
    {
         extractRGBValues ( d[i*(pix2->wpl)+j],&r,&g,&b );
		
         if (!((r>white) && (g>white) && (b>white))) d[i*(pix2->wpl)+j]=0;
    }

	//0 - black 


	
     

    /*con=new unsigned [pix2->h*pix2->w];

    long long unsigned curlab=1;

      for (int i = 1; i < pix2->h; ++i)
      for (int j = 1; j<pix2->w; ++j)
      {
		  con[i*(pix2->wpl) + j] = 0;
      }
	  */

	return 0;
   
   pixDisplay(pix2,0,0);
    printf("%d %d %d %d\n",pix1->d,pix1->w,pix1->h,pix1->wpl);


	

	return 0;
}
