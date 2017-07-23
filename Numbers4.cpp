#include <fifascanner/screen.h>

using namespace std;

	

	Numbers4::Numbers4(int a, int b, int c, int d) : x1(a), y1(b), x2(c), y2(d) {};

	





ostream& operator<<(ostream& os , const Numbers4& dt )
{
	os << dt.x1 << '|' << dt.y1 << '|' << dt.x2<<"|"<< dt.y2;
	return os;
}