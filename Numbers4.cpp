struct Numbers4
{
	int x1, y1, x2,y2;

	Numbers4(int , int , int , int );

	

	friend ostream& operator<<(ostream& , const Numbers4& );
	

}

ostream& operator<<(ostream& , const Numbers4& )
{
	os << dt.x1 << '|' << dt.y1 << '|' << dt.x2<<"|"<< dt.y2;
	return os;
}