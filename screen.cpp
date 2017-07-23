
#include <fifascanner/screen.h>

using namespace std;


	Screen::Screen(char* file_name)
	{
		fout.open("result.txt",ios::app);
		pix = NULL;
		pix = pixCopy(pix,  pixRead(file_name));
		
	}
	void  Screen::get_data()
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
	}

	void Screen::remove_intersecs()
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
	
	void Screen::get_score()
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
				if (s1 == 11)
				{
					cout << "Too many candidates for " << endl;
					return;
				}
				 
			}
		}
		fout.close();
	}
	void Screen::black_box(int x1,int y1, int x2,int y2)
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
	void Screen::draw_box(int x1, int y1, int x2, int y2)
	{
		BOX* cropWindow = boxCreate(x1 - border, y1 - border, 2*border + x2-x1, 2*border + y2-y1);
		PIX* croppedImage = pixClipRectangle(pix, cropWindow, NULL);
		tesseract::TessBaseAPI ocr;
		ocr.Init(NULL, "eng");
		ocr.SetImage(croppedImage);
		string result = ocr.GetUTF8Text();
		fout << result ;
	}

	void Screen::draw_box(const Numbers4 & s)
	{
		draw_box(s.x1, s.y1, s.x2, s.y2);
	}
	void Screen::proccess_one_elem(RowBeginEnd& in, int min_x, int max_x,int min_y,int i )
	{
		if (i == table.size() - 1)
		{
			return;
		}
		int minx = 0, maxx = 0;
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
		
				if (min_x - max_x >= min_dim &&  in.get_row() - min_y >= min_dim)
				{
					buf.emplace_back(Numbers4(min_x, min_y, max_x, in.get_row() ) );
					
				}
	}
	
