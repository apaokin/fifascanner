#include <fifascanner/screen.h>
using namespace std;
	
	

	int RowBeginEnd::get_row()
	{
		return row;
	}
	int  RowBeginEnd::get_beg()
	{
		return x_beg;
	}
	int   RowBeginEnd::get_end()
	{
		return x_end;
	}
	RowBeginEnd::RowBeginEnd(int  x_beg2, int x_end2,int  row2)
	{
		x_beg = x_beg2;
		x_end = x_end2;
		row = row2;
	}
	//ffRowBeginEnd::friend ostream& operator<<(ostream& os, const RowBeginEnd& dt);


ostream& operator<<(ostream& os, const RowBeginEnd& dt)
{
	os << dt.row << '|' << dt.x_beg << '|' << dt.x_end;
	return os;
}