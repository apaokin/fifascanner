
#include <fifascanner/screen.h>

using namespace std;


int main(int argc, char* argv[])
{


     const l_uint32 white=150;
	tesseract::TessBaseAPI ocr;
	ocr.Init(NULL, "eng");
    PIX* pix1 = pixRead("fiftest1.jpg"),*pix2;
	Screen screen("fiftest1.jpg");
	screen.get_data();
	Screen screen2("fiftest2.jpg");
	screen2.get_data();
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
