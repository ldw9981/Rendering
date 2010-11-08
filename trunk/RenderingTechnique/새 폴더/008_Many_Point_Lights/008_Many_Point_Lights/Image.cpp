// Image.cpp: implementation of the Image class.
//
//////////////////////////////////////////////////////////////////////

#include "Image.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Image::Image()
{
	planes=0; sizeX=0; sizeY=0; data=NULL; isBGR=0;
}

Image::~Image()
{
	if(data!=NULL)delete [] data;
}

Image::Image(const Image &in)
{
	planes=0; sizeX=0; sizeY=0; data=NULL; isBGR=0;
	Set( in);
}

int Image::Load(char *filename)
{
	char p[5];
	int ret = 0;

	char* pdest = strrchr( filename, '.' );		// find '.'
	if( pdest[0]!=NULL)pdest++;
	strncpy( p, pdest, 4);
	strlwr(p);				// Convert a string to lowercase

	if( !strcmp(p,"bmp"))		ret = LoadBMP( filename);
	else if( !strcmp(p,"jpg"))	ret = LoadJPG( filename);
	else if( !strcmp(p,"jpeg"))	ret = LoadJPG( filename);
	else if( !strcmp(p,"tga"))	ret = LoadTGA( filename);
	if(ret) return 1;
	MessageBox( hWnd, filename, "Nenájdený súbor", MB_OK );
	return 0;
}

void Image::Set(const Image &in)
{
	if(in.planes<0 || in.planes>4)return;
	if(in.sizeX<1 || in.sizeY<1) return;
	if(in.data==NULL)return;
	planes = in.planes;
	sizeX = in.sizeX; 
	sizeY = in.sizeY; 
	isBGR = in.isBGR;
	if(data!=NULL) delete [] data;
	data = new unsigned char[sizeX*sizeY*planes];
	if(data==NULL)return;
	memcpy( data, in.data, sizeX*sizeY*planes);
}

int Image::SaveBMP(char *filename)
{
	BITMAPFILEHEADER fileheader;
	BITMAPINFOHEADER infoheader;
	FILE *sub;
	if(data==NULL || planes !=3)return 0;
	
	int line_size = planes*sizeX;
	if(line_size%4)line_size += 4 - line_size%4;
	
	fileheader.bfType = 0x4D42; // Magic identifier   - "BM"	| identifikacia BMP suboru musi byt "BM"
	fileheader.bfSize = planes*sizeX*sizeY+sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);	// File size in bytes			| velkos suboru v byte
	fileheader.bfReserved1 = 0;
	fileheader.bfReserved2 = 0;
	fileheader.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);	// Offset to image data, bytes	| posun na zaciatok dat
	
	infoheader.biSize = sizeof(BITMAPINFOHEADER);	// Header size in bytes			| velkost hlavicky BITMAPINFOHEADER
	infoheader.biWidth = sizeX;	// Width of image			| sirka obrazka - sizeX
	infoheader.biHeight = sizeY;	// Height of image			| vyska obrazka - sizeY
	infoheader.biPlanes = 1;		// Number of colour planes	| pocet farebnych rovin musi bit 1
	infoheader.biBitCount = 24;		// Bits per pixel			| bitov na pixel moze bit 1,4,8,24
	infoheader.biCompression = 0;	// Compression type			| typ compresie , 0 - bez kompresie
	infoheader.biSizeImage = line_size*infoheader.biHeight ;	// Image size in bytes		| velkost obrazka v byte
	infoheader.biXPelsPerMeter = 0;	// Pixels per meter X		| pixelov na meter v smere x
	infoheader.biYPelsPerMeter = 0;	// Pixels per meter Y		| pixelov na meter v smere y
	infoheader.biClrUsed = 0;		// Number of colours		| pocet  farieb v palete, ak 0 vsetky su pouzivane
	infoheader.biClrImportant = 0;	// Important colours		| dolezite farby v palete, ak 0 vsetky su dolezite
	
	sub = fopen(filename,"wb");
	if(sub==NULL)return 0;
	fwrite( &fileheader, sizeof(BITMAPFILEHEADER), 1, sub);
	fwrite( &infoheader, sizeof(BITMAPINFOHEADER), 1, sub);
	
	unsigned char* p = new unsigned char[line_size*sizeY];
	for(int y=0; y<sizeY; y++)
	{
		for(int x=0; x<sizeX; x++)
		{
			if(isBGR)
			{
				p[y*line_size+x*3+0] = data[(y*sizeX+x)*3+0];
				p[y*line_size+x*3+1] = data[(y*sizeX+x)*3+1];
				p[y*line_size+x*3+2] = data[(y*sizeX+x)*3+2];
			}
			else
			{
				p[y*line_size+x*3+0] = data[(y*sizeX+x)*3+2];
				p[y*line_size+x*3+1] = data[(y*sizeX+x)*3+1];
				p[y*line_size+x*3+2] = data[(y*sizeX+x)*3+0];
			}
		}
	}
	fwrite( p, line_size*sizeY, 1, sub);
	delete[] p;
	fclose(sub);
	return 1;
}

unsigned char* Image::Get(int *size)
{
	BITMAPFILEHEADER fileheader;
	BITMAPINFOHEADER infoheader;

	int line_size = planes*sizeX;
	if(line_size%4)line_size += 4 - line_size%4;
	
	fileheader.bfType = 0x4D42; // Magic identifier   - "BM"	| identifikacia BMP suboru musi byt "BM"
	fileheader.bfSize = planes*sizeX*sizeY+sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);	// File size in bytes			| velkos suboru v byte
	fileheader.bfReserved1 = 0;
	fileheader.bfReserved2 = 0;
	fileheader.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);	// Offset to image data, bytes	| posun na zaciatok dat
	
	infoheader.biSize = sizeof(BITMAPINFOHEADER);	// Header size in bytes			| velkost hlavicky BITMAPINFOHEADER
	infoheader.biWidth = sizeX;	// Width of image			| sirka obrazka - sizeX
	infoheader.biHeight = sizeY;	// Height of image			| vyska obrazka - sizeY
	infoheader.biPlanes = 1;		// Number of colour planes	| pocet farebnych rovin musi bit 1
	infoheader.biBitCount = 24;		// Bits per pixel			| bitov na pixel moze bit 1,4,8,24
	infoheader.biCompression = 0;	// Compression type			| typ compresie , 0 - bez kompresie
	infoheader.biSizeImage = line_size*infoheader.biHeight ;	// Image size in bytes		| velkost obrazka v byte
	infoheader.biXPelsPerMeter = 0;	// Pixels per meter X		| pixelov na meter v smere x
	infoheader.biYPelsPerMeter = 0;	// Pixels per meter Y		| pixelov na meter v smere y
	infoheader.biClrUsed = 0;		// Number of colours		| pocet  farieb v palete, ak 0 vsetky su pouzivane
	infoheader.biClrImportant = 0;	// Important colours		| dolezite farby v palete, ak 0 vsetky su dolezite

	int posun = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
	unsigned char* pdata = new unsigned char[line_size*sizeY+posun];
	if(pdata==NULL){ *size=0; return NULL;}
	*size = line_size*sizeY+posun;
	memcpy( pdata, &fileheader, sizeof(BITMAPFILEHEADER));
	memcpy( &pdata[sizeof(BITMAPFILEHEADER)], &infoheader, sizeof(BITMAPINFOHEADER));

	unsigned char* p = &pdata[posun];
	for(int y=0; y<sizeY; y++)
	{
		for(int x=0; x<sizeX; x++)
		{
			if(isBGR)
			{
				p[y*line_size+x*3+0] = data[(y*sizeX+x)*3+0];
				p[y*line_size+x*3+1] = data[(y*sizeX+x)*3+1];
				p[y*line_size+x*3+2] = data[(y*sizeX+x)*3+2];
			}
			else
			{
				p[y*line_size+x*3+0] = data[(y*sizeX+x)*3+2];
				p[y*line_size+x*3+1] = data[(y*sizeX+x)*3+1];
				p[y*line_size+x*3+2] = data[(y*sizeX+x)*3+0];
			}
		}
	}
	return pdata;
}

int Image::LoadBMP(char *filename)
{
	FILE* f;
	BITMAPFILEHEADER fileheader;
	BITMAPINFOHEADER infoheader;
	unsigned char* pom_data;

	f = fopen(filename,"rb");
	if(f==NULL)return 0;

	if(!fread( &fileheader, sizeof(BITMAPFILEHEADER), 1, f))
	{	fclose(f); return 0; }

	if(!fread( &infoheader, sizeof(BITMAPINFOHEADER), 1, f))
	{	fclose(f); return 0; }

	// kontrola ci je to BMP subor, 'BM'  == 0x4D42
	if(fileheader.bfType!=0x4D42){	fclose(f); return 0; }

	// nacitavame iba obrazky bez kompresie
	if(infoheader.biCompression){	fclose(f); return 0; }
	
	switch(infoheader.biBitCount)
	{
	case 1: return LoadBMP1bit( f, fileheader, infoheader);
	case 4: return LoadBMP4bit( f, fileheader, infoheader);
	case 8: return LoadBMP8bit( f, fileheader, infoheader);
	case 24: break;
	case 32: break;
	default:
		fclose(f); return 0;
	}

	int line_size = infoheader.biWidth*infoheader.biBitCount/8;
	if(line_size%4)line_size += 4 - line_size%4;
	int data_size = line_size*infoheader.biHeight;
	//	data_size = fileheader.bfSize-sizeof(BITMAPFILEHEADER)-infoheader.biSize;
	
	pom_data = new unsigned char[data_size];
	if(pom_data==NULL){	fclose(f); return 0; }
	if(!fread( pom_data, data_size, 1, f))
	{	
		fclose(f);
		delete [] pom_data;
		return 0; 
	}
	fclose(f);
	
	sizeX=infoheader.biWidth;
	sizeY=infoheader.biHeight;
	planes=infoheader.biBitCount/8;
	isBGR=1;

	if( (sizeX*planes)%4==0 ) 
	{
		data = pom_data;
		return 1;
	}
	
	if(data!=NULL) delete [] data;
	data = new unsigned char[planes*sizeX*sizeY];
	if(data==NULL){	fclose(f); delete [] pom_data; return 0; }
	
	line_size = sizeX*planes;
	if(line_size%4)line_size += 4 - line_size%4;	

	for(int y=0; y<sizeY; y++)
	{
		for(int x=0; x<sizeX; x++)
		{
			data[(sizeX*y+x)*planes+2]=pom_data[line_size*y+x*planes+0];
			data[(sizeX*y+x)*planes+1]=pom_data[line_size*y+x*planes+1];
			data[(sizeX*y+x)*planes+0]=pom_data[line_size*y+x*planes+2];
			if(planes==4)data[(sizeX*y+x)*planes+3]=pom_data[line_size*y+x*planes+3];
		}
	}
	isBGR = 0;
	delete [] pom_data;
	return 1;
}

int Image::LoadBMP1bit(FILE *f, BITMAPFILEHEADER &fileheader, BITMAPINFOHEADER &infoheader)
{
	RGBQUAD pal[2];
	unsigned int countpal;

	if(infoheader.biBitCount!=1)return 0;
	if(infoheader.biClrUsed==0)countpal=2;
	else countpal=infoheader.biClrUsed;
	if(fread( pal, sizeof(RGBQUAD), countpal, f)!=countpal)
	{
		fclose(f);
		return 0;
	}

	unsigned char* pom_data;
	int line_size = infoheader.biWidth/8;
	if(infoheader.biWidth%8)line_size++;
	if(line_size%4)line_size += 4 - line_size%4;
	pom_data = new unsigned char[line_size*infoheader.biHeight];
	if(pom_data==NULL){	fclose(f); return 0; }
	if(!fread( pom_data, line_size*infoheader.biHeight, 1, f))
	{	
		fclose(f);
		delete [] pom_data;
		return 0; 
	}
	fclose(f);

	sizeX=infoheader.biWidth;
	sizeY=infoheader.biHeight;
	planes=3;
	isBGR=0;

	if(data!=NULL)delete [] data;
	data = new unsigned char[planes*sizeX*sizeY];
	if(data==NULL){ delete [] pom_data; return 0; }

	for(int y=0; y<sizeY; y++)
	{
		for(int x=0; x<sizeX; x++)
		{
			unsigned char index=pom_data[y*line_size+x/8];
			index >>= (7-x%8);
			index &= 0x01;
			data[(y*sizeX+x)*planes+0] = pal[index].rgbRed;
			data[(y*sizeX+x)*planes+1] = pal[index].rgbGreen;
			data[(y*sizeX+x)*planes+2] = pal[index].rgbBlue;
		}
	}
	if(pom_data!=NULL)delete []pom_data;
	return 1;
}

int Image::LoadBMP4bit(FILE *f, BITMAPFILEHEADER &fileheader, BITMAPINFOHEADER &infoheader)
{
	RGBQUAD pal[16];
	unsigned int countpal;

	if(infoheader.biBitCount!=4)return 0;
	if(infoheader.biClrUsed==0)countpal=4;
	else countpal=infoheader.biClrUsed;
	if(fread( pal, sizeof(RGBQUAD), countpal, f)!=countpal)
	{
		fclose(f);
		return 0;
	}

	unsigned char* pom_data;
	int line_size = infoheader.biWidth/2+infoheader.biWidth%2;
	if(line_size%4)line_size += 4 - line_size%4;
	pom_data = new unsigned char[line_size*infoheader.biHeight];
	if(pom_data==NULL){	fclose(f); return 0; }
	if(!fread( pom_data, line_size*infoheader.biHeight, 1, f))
	{	
		fclose(f);
		delete [] pom_data;
		return 0; 
	}
	fclose(f);

	sizeX=infoheader.biWidth;
	sizeY=infoheader.biHeight;
	planes=3;
	isBGR=0;

	if(data!=NULL)delete [] data;
	data = new unsigned char[planes*sizeX*sizeY];
	if(data==NULL){ delete [] pom_data; return 0; }

	for(int y=0; y<sizeY; y++)
	{
		for(int x=0; x<sizeX; x++)
		{
			int index;
			if(x%2) index = pom_data[y*line_size+x/2]&0x0F;
			else index = (pom_data[y*line_size+x/2]&0x0F0)>>4;
			data[(y*sizeX+x)*planes+0] = pal[index].rgbRed;
			data[(y*sizeX+x)*planes+1] = pal[index].rgbGreen;
			data[(y*sizeX+x)*planes+2] = pal[index].rgbBlue;
		}
	}
	if(pom_data!=NULL)delete []pom_data;
	return 1;
}

int Image::LoadBMP8bit(FILE *f, BITMAPFILEHEADER &fileheader, BITMAPINFOHEADER &infoheader)
{
	RGBQUAD pal[256];
	unsigned int countpal;

	if(infoheader.biBitCount!=8)return 0;
	if(infoheader.biClrUsed==0)countpal=256;
	else countpal=infoheader.biClrUsed;
	if(fread( pal, sizeof(RGBQUAD), countpal, f)!=countpal)
	{
		fclose(f);
		return 0;
	}

	unsigned char* pom_data;
	int line_size = infoheader.biWidth;
	if(line_size%4)line_size += 4 - line_size%4;
	pom_data = new unsigned char[line_size*infoheader.biHeight];
	if(pom_data==NULL){	fclose(f); return 0; }
	if(!fread( pom_data, line_size*infoheader.biHeight, 1, f))
	{	
		fclose(f);
		delete [] pom_data;
		return 0; 
	}
	fclose(f);

	sizeX=infoheader.biWidth;
	sizeY=infoheader.biHeight;
	planes=3;
	isBGR=0;

	if(data!=NULL)delete [] data;
	data = new unsigned char[planes*sizeX*sizeY];
	if(data==NULL){ delete [] pom_data; return 0; }

	for(int y=0; y<sizeY; y++)
	{
		for(int x=0; x<sizeX; x++)
		{
			data[(y*sizeX+x)*planes+0] = pal[pom_data[y*line_size+x]].rgbRed;
			data[(y*sizeX+x)*planes+1] = pal[pom_data[y*line_size+x]].rgbGreen;
			data[(y*sizeX+x)*planes+2] = pal[pom_data[y*line_size+x]].rgbBlue;
		}
	}
	if(pom_data!=NULL)delete []pom_data;
	return 1;
}

int Image::LoadJPG(char *filename)
{
/*	if(data!=NULL)delete [] data;
	data = NULL;
	if(! ::LoadJPG(filename, &planes, &sizeX, &sizeY, &data))return 0;
	isBGR = 0;
	Flip();
	return 1;*/
	return 0;
}

// These defines are used to tell us about the type of TARGA file it is
#define TGA_RGB		 2		// This tells us it's a normal RGB (really BGR) file
#define TGA_A		 3		// This tells us it's a ALPHA file
#define TGA_RLE		10		// This tells us that the targa is Run-Length Encoded (RLE)

int Image::LoadTGA(char *filename)
{
	// code from www.gametutorials.com
	WORD width = 0, height = 0;			// The dimensions of the image
	byte length = 0;					// The length in bytes to the pixels
	byte imageType = 0;					// The image type (RLE, RGB, Alpha...)
	byte bits = 0;						// The bits per pixel for the image (16, 24, 32)
	FILE *pFile = NULL;					// The file pointer
	int channels = 0;					// The channels of the image (3 = RGA : 4 = RGBA)
	int stride = 0;						// The stride (channels * width)
		
	// Open a file pointer to the targa file and check if it was found and opened 
	if((pFile = fopen(filename, "rb")) == NULL) return 0;
	if(data!=NULL) delete [] data;
	data = NULL;
	isBGR=0;
		
	fread(&length, sizeof(byte), 1, pFile);
	fseek(pFile,1,SEEK_CUR); 
	fread(&imageType, sizeof(byte), 1, pFile);
	fseek(pFile, 9, SEEK_CUR); 

	// Read the width, height and bits per pixel (16, 24 or 32)
	fread(&width,  sizeof(WORD), 1, pFile);
	fread(&height, sizeof(WORD), 1, pFile);
	fread(&bits,   sizeof(byte), 1, pFile);
	
	// Now we move the file pointer to the pixel data
	fseek(pFile, length + 1, SEEK_CUR); 

	// Check if the image is RLE compressed or not
	if(imageType != TGA_RLE)
	{
		// Check if the image is a 24 or 32-bit image
		if(bits == 24 || bits == 32)
		{
			// Calculate the channels (3 or 4) - (use bits >> 3 for more speed).
			// Next, we calculate the stride and allocate enough memory for the pixels.
			channels = bits / 8;
			stride = channels * width;
			data = new unsigned char[stride * height];

			// Load in all the pixel data line by line
			for(int y = 0; y < height; y++)
			{
				// Store a pointer to the current line of pixels
				unsigned char *pLine = &(data[stride * y]);

				// Read in the current line of pixels
				fread(pLine, stride, 1, pFile);
			}
			isBGR=1;
		}
		// Check if the image is a 16 bit image (RGB stored in 1 unsigned short)
		else if(bits == 16)
		{
			unsigned short pixels = 0;
			int r=0, g=0, b=0;

			// Since we convert 16-bit images to 24 bit, we hardcode the channels to 3.
			// We then calculate the stride and allocate memory for the pixels.
			channels = 3;
			stride = channels * width;
			data = new unsigned char[stride * height];

			// Load in all the pixel data pixel by pixel
			for(int j = 0; j < width*height; j++)
			{
				// Read in the current pixel
				fread(&pixels, sizeof(unsigned short), 1, pFile);
				
				// To convert a 16-bit pixel into an R, G, B, we need to
				// do some masking and such to isolate each color value.
				// 0x1f = 11111 in binary, so since 5 bits are reserved in
				// each unsigned short for the R, G and B, we bit shift and mask
				// to find each value.  We then bit shift up by 3 to get the full color.
				b = (pixels & 0x1f) << 3;
				g = ((pixels >> 5) & 0x1f) << 3;
				r = ((pixels >> 10) & 0x1f) << 3;
				
				// This essentially assigns the color to our array and swaps the
				// B and R values at the same time.
				data[j * 3 + 0] = r;
				data[j * 3 + 1] = g;
				data[j * 3 + 2] = b;
			}
		}	
		else
			return 0;
	}
	// Else, it must be Run-Length Encoded (RLE)
	else
	{
		int j=0;
		byte rleID = 0;
		int colorsRead = 0;
		channels = bits / 8;
		stride = channels * width;

		data = new unsigned char[stride * height];
		byte *pColors = new byte [channels];

		while(j < width*height)
		{
			fread(&rleID, sizeof(byte), 1, pFile);
			
			if(rleID < 128)
			{
				rleID++;
				while(rleID)
				{
					fread(pColors, sizeof(byte) * channels, 1, pFile);
					data[colorsRead + 0] = pColors[2];
					data[colorsRead + 1] = pColors[1];
					data[colorsRead + 2] = pColors[0];

					if(bits == 32)data[colorsRead + 3] = pColors[3];
					j++;
					rleID--;
					colorsRead += channels;
				}
			}
			else
			{
				rleID -= 127;
				fread(pColors, sizeof(byte) * channels, 1, pFile);

				while(rleID)
				{
					data[colorsRead + 0] = pColors[2];
					data[colorsRead + 1] = pColors[1];
					data[colorsRead + 2] = pColors[0];

					if(bits == 32)data[colorsRead + 3] = pColors[3];
					j++;
					rleID--;
					colorsRead += channels;
				}
				
			}
				
		}
	}
	fclose(pFile);

	planes = channels;
	sizeX    = width;
	sizeY    = height;
	Flip();
	return 1;
}

void Image::BGRtoRGB()
{
	if(!isBGR || planes<3 || data==NULL)return;
	for(int y=0; y<sizeY; y++)
	{
		for(int x=0; x<sizeX; x++)
		{
			unsigned char pom = data[(sizeX*y+x)*planes+2];
			data[(sizeX*y+x)*planes+2]=data[(sizeX*y+x)*planes+0];
			data[(sizeX*y+x)*planes+0]=pom;
		}
	}
	isBGR=0;
}

void Image::shift()				// jas * 0.5
{
	if(data==NULL)return;
	for(int y=0; y<sizeY; y++)
	{
		for(int x=0; x<sizeX; x++)
		{
			data[(y*sizeX+x)*planes+0] = data[(y*sizeX+x)*planes+0]>>1;
			data[(y*sizeX+x)*planes+1] = data[(y*sizeX+x)*planes+1]>>1;
			data[(y*sizeX+x)*planes+2] = data[(y*sizeX+x)*planes+2]>>1;
		}
	}
}

void Image::invShift()			// invertovana a potom jas*0.5
{
	if(data==NULL)return;
	for(int y=0; y<sizeY; y++)
	{
		for(int x=0; x<sizeX; x++)
		{
			data[(y*sizeX+x)*planes+0] = (255-data[(y*sizeX+x)*planes+0])>>1;
			data[(y*sizeX+x)*planes+1] = (255-data[(y*sizeX+x)*planes+1])>>1;
			data[(y*sizeX+x)*planes+2] = (255-data[(y*sizeX+x)*planes+2])>>1;
		}
	}
}

int Image::resize(int newx, int newy)
{
	unsigned char *old_data = data;
	int old_sizeX = sizeX;
	int old_sizeY = sizeY;
	float *d;

	sizeX = newx;
	sizeY = newy;
	data = new unsigned char[sizeX*sizeY*planes];
	if(data==NULL)
	{
		data = old_data;
		sizeX = old_sizeX;
		sizeY = old_sizeY;
		return 0;
	}

	d = new float[old_sizeX*old_sizeY*planes];
	if(d==NULL)
	{
		delete [] data;
		data = old_data;
		sizeX = old_sizeX;
		sizeY = old_sizeY;
		return 0;
	}

	for(int j=0; j<old_sizeX*old_sizeY*planes; j++)d[j] = (float)old_data[j];

	float xf,yf;
	int ix,iy;		// index do vstupneho obrazka
	for(int y=0; y<newy; y++)
	{
	//	yf = y/(float)(newy-1);
		yf = y/(float)(newy);
		yf *= (float) old_sizeY;
		iy = (int) floor(yf);		// vrati mensiu celu cast 2.8 -> 2.0
		yf = (float)fmod(yf,1.0);			// zvysok po deleni 1
		for(int x=0; x<newx; x++)
		{
	//		xf = x/(float)(newx-1);
			xf = x/(float)(newx);
			xf *= (float) old_sizeX;
			ix = (int) floor(xf);		// vrati mensiu celu cast 2.8 -> 2.0
			xf = (float)fmod(xf,1.0);			// zvysok po deleni 1
		
			for(int c=0; c<planes; c++)
			{
				float a,b;
				if(ix<old_sizeX-1 && iy<old_sizeY-1)
				{
					a = (1.f-xf)*d[(iy*old_sizeX+ix)*planes+c]+xf*d[(iy*old_sizeX+ix+1)*planes+c];
					b = (1.f-xf)*d[((iy+1)*old_sizeX+ix)*planes+c]+xf*d[((iy+1)*old_sizeX+ix+1)*planes+c];
					a = (1.f-yf)*a+yf*b;
				}
				else
				{
					if(ix<old_sizeX-1)		// iy >= old_sizeY-1
					{
						iy = old_sizeY-1;
						a = (1.f-xf)*d[(iy*old_sizeX+ix)*planes+c]+xf*d[(iy*old_sizeX+ix+1)*planes+c];
					}
					else if(iy<old_sizeY-1)	// ix >= old_sizeX-1 && iy<old_sizeY-1
					{
						ix = old_sizeX-1;
						a = (1.f-yf)*d[(iy*old_sizeX+ix)*planes+c]+yf*d[((iy+1)*old_sizeX+ix)*planes+c];
					}
					else
					{
						ix = old_sizeX-1;
						iy = old_sizeY-1;
						a = d[(iy*old_sizeX+ix)*planes+c];
					}
				}
				data[(y*sizeX+x)*planes+c] = (unsigned char) a;
			}
		}
	}

	delete [] d;
	delete [] old_data;
	return 1;
}

int Image::resizeToHalfX()
{
	unsigned char *old_data = data;
	int old_sizeX = sizeX;
	int old_sizeY = sizeY;

	if(sizeX<=1)return 0;
	
	sizeX >>=1;		// /2
	data = new unsigned char[planes*sizeX*sizeY];
	if(data==NULL)
	{
		data = old_data;
		sizeX = old_sizeX;
		sizeY = old_sizeY;
		return 0;
	}

	for(int y=0; y<sizeY; y++)
	{
		for(int x=0; x<sizeX; x++)
		{
			data[(y*sizeX+x)*planes+0] = ((unsigned int)old_data[(y*old_sizeX+2*x)*planes+0]+(unsigned int)old_data[(y*old_sizeX+2*x+1)*planes+0])>>1;
			if(planes>1)data[(y*sizeX+x)*planes+1] = ((unsigned int)old_data[(y*old_sizeX+2*x)*planes+1]+(unsigned int)old_data[(y*old_sizeX+2*x+1)*planes+1])>>1;
			if(planes>2)data[(y*sizeX+x)*planes+2] = ((unsigned int)old_data[(y*old_sizeX+2*x)*planes+2]+(unsigned int)old_data[(y*old_sizeX+2*x+1)*planes+2])>>1;
			if(planes>3)data[(y*sizeX+x)*planes+3] = ((unsigned int)old_data[(y*old_sizeX+2*x)*planes+3]+(unsigned int)old_data[(y*old_sizeX+2*x+1)*planes+3])>>1;
		}
	}
	delete [] old_data;
	return 1;
}

int Image::resizeToHalfY()
{
	unsigned char *old_data = data;
	int old_sizeX = sizeX;
	int old_sizeY = sizeY;;

	if(sizeY<=1)return 0;
	
	sizeY >>=1;		// /2
	data = new unsigned char[planes*sizeX*sizeY];
	if(data==NULL)
	{
		data = old_data;
		sizeX = old_sizeX;
		sizeY = old_sizeY;
		return 0;
	}

	for(int y=0; y<sizeY; y++)
	{
		for(int x=0; x<sizeX; x++)
		{
			data[(y*sizeX+x)*planes+0] = ((unsigned int)old_data[(2*y*old_sizeX+x)*planes+0]+(unsigned int)old_data[((2*y+1)*old_sizeX+x)*planes+0])>>1;
			if(planes>1)data[(y*sizeX+x)*planes+1] = ((unsigned int)old_data[(2*y*old_sizeX+x)*planes+1]+(unsigned int)old_data[((2*y+1)*old_sizeX+x)*planes+1])>>1;
			if(planes>2)data[(y*sizeX+x)*planes+2] = ((unsigned int)old_data[(2*y*old_sizeX+x)*planes+2]+(unsigned int)old_data[((2*y+1)*old_sizeX+x)*planes+2])>>1;
			if(planes>3)data[(y*sizeX+x)*planes+3] = ((unsigned int)old_data[(2*y*old_sizeX+x)*planes+3]+(unsigned int)old_data[((2*y+1)*old_sizeX+x)*planes+3])>>1;
		}
	}
	delete [] old_data;
	return 1;
}

int Image::resizeToHalfXY()
{
	unsigned char *old_data = data;
	int old_sizeX = sizeX;
	int old_sizeY = sizeY;

	if(sizeX<=1 && sizeY<=1)return 0;
	
	sizeX >>=1;		// /2
	sizeY >>=1;		// /2
	data = new unsigned char[planes*sizeX*sizeY];
	if(data==NULL)
	{
		data = old_data;
		sizeX = old_sizeX;
		sizeY = old_sizeY;
		return 0;
	}

	for(int y=0; y<sizeY; y++)
	{
		for(int x=0; x<sizeX; x++)
		{
			data[(y*sizeX+x)*planes+0] = (
							(unsigned int)old_data[(2*y*old_sizeX+2*x)*planes+0]+
							(unsigned int)old_data[((2*y+1)*old_sizeX+2*x)*planes+0]+
							(unsigned int)old_data[(2*y*old_sizeX+2*x+1)*planes+0]+
							(unsigned int)old_data[((2*y+1)*old_sizeX+2*x+1)*planes+0])>>2;
			if(planes>1)data[(y*sizeX+x)*planes+1] = (
							(unsigned int)old_data[(2*y*old_sizeX+2*x)*planes+1]+
							(unsigned int)old_data[((2*y+1)*old_sizeX+2*x)*planes+1]+
							(unsigned int)old_data[(2*y*old_sizeX+2*x+1)*planes+1]+
							(unsigned int)old_data[((2*y+1)*old_sizeX+2*x+1)*planes+1])>>2;
			if(planes>2)data[(y*sizeX+x)*planes+2] = (
							(unsigned int)old_data[(2*y*old_sizeX+2*x)*planes+2]+
							(unsigned int)old_data[((2*y+1)*old_sizeX+2*x)*planes+2]+
							(unsigned int)old_data[(2*y*old_sizeX+2*x+1)*planes+2]+
							(unsigned int)old_data[((2*y+1)*old_sizeX+2*x+1)*planes+2])>>2;
			if(planes>3)data[(y*sizeX+x)*planes+3] = (
							(unsigned int)old_data[(2*y*old_sizeX+2*x)*planes+3]+
							(unsigned int)old_data[((2*y+1)*old_sizeX+2*x)*planes+3]+
							(unsigned int)old_data[(2*y*old_sizeX+2*x+1)*planes+3]+
							(unsigned int)old_data[((2*y+1)*old_sizeX+2*x+1)*planes+3])>>2;
		}
	}
	delete [] old_data;
	return 1;
}

void Image::Flip()				// up - down
{
	unsigned char *old = data;
	if(data==NULL)return;
	data = new unsigned char[planes*sizeX*sizeY];
	if(data==NULL)
	{
		data = old;
		return;
	}

	for(int y=0; y<sizeY; y++)
	{
		memcpy( &data[planes*sizeX*y], &old[planes*sizeX*(sizeY-y-1)], planes*sizeX );
	}
	delete [] old;
}

void Image::FlipMirror()		// up - down, right - left
{
	unsigned char *old = data;
	if(planes<3 || data==NULL)return;
	data = new unsigned char[planes*sizeX*sizeY];
	if(data==NULL)
	{
		data = old;
		return;
	}

	for(int y=0; y<sizeY; y++)
	{
		for(int x=0; x<sizeX; x++)
		{
			data[(y*sizeX+x)*planes+0]=old[( (sizeY-y-1)*sizeX+sizeX-x-1)*planes+0];
			data[(y*sizeX+x)*planes+1]=old[( (sizeY-y-1)*sizeX+sizeX-x-1)*planes+1];
			data[(y*sizeX+x)*planes+2]=old[( (sizeY-y-1)*sizeX+sizeX-x-1)*planes+2];
		}
	}
	delete [] old;
}

unsigned char Image::getSavePoint( int x, int y, int plane)
{
	x = (x+sizeX)%sizeX;						// repeat
//	y = (y+sizeY)%sizeY;						// repeat
//	if(x>=sizeX)x=sizeX-1; else if(x<0)x=0;		// clamp
	if(y>=sizeY)y=sizeY-1; else if(y<0)y=0;		// clamp
	return data[(sizeX*y+x)*planes+plane];
}

float Image::evaluateMask(  int x, int y, int plane, \
							float m11, float m12, float m13, \
							float m21, float m22, float m23, \
							float m31, float m32, float m33 )
{
	float r;

	r  = m11*getSavePoint( x-1, y-1, plane);
	r += m12*getSavePoint( x  , y-1, plane);
	r += m13*getSavePoint( x+1, y-1, plane);
	r += m21*getSavePoint( x-1, y  , plane);
	r += m22*getSavePoint( x  , y  , plane);
	r += m23*getSavePoint( x+1, y  , plane);
	r += m31*getSavePoint( x-1, y+1, plane);
	r += m32*getSavePoint( x  , y+1, plane);
	r += m33*getSavePoint( x+1, y+1, plane);
	return r/255.f;
}

void Image::Filter( int maska, int plane, float strong)
{
	if(data==NULL)return;
	unsigned char *new_data = new unsigned char[sizeX*sizeY*3];
	if(new_data==NULL)return;
	if(plane>=planes)plane = planes-1;

	float dx,dy;		// dx derivacia vysky (farby) v smere x, dy derivacia v smere y

	for(int x=0; x<sizeX; x++)
	{
		for(int y=0; y<sizeY; y++)
		{
			switch(maska)
			{
			case 0:
				// Sobel-Prewitt maska y - vyhladava vertikalne ciary = derivaciu x
				dx  = getSavePoint( x-1, y-1, plane);					//  1  0 -1
				dx += 2.f*(float)getSavePoint(  x-1, y, plane);			//  2  0 -2
				dx += getSavePoint(  x-1, y+1, plane);					//  1  0 -1
				dx -= getSavePoint(  x+1, y-1, plane);
				dx -= 2.f*(float)getSavePoint(  x+1, y, plane);
				dx -= getSavePoint(  x+1, y+1, plane);
				dx /= 255.f;
				// Sobel-Prewitt maska x - vyhladava horizontalne ciary = derivacia y
				dy  = getSavePoint(  x-1, y-1, plane);					//  1  2  1
				dy += 2.f*(float)getSavePoint(  x  , y-1, plane);		//  0  0  0
				dy += getSavePoint(  x+1, y-1, plane);					// -1 -2 -1
				dy -= getSavePoint(  x-1, y+1, plane);
				dy -= 2.f*(float)getSavePoint(  x  , y+1, plane);
				dy -= getSavePoint(  x+1, y+1, plane);
				dy /= 255.f;
				break;
			case 1:
				dx = evaluateMask( x, y, plane, 1,  0, -1,  1,  0, -1,  1,  0, -1 );	// Prewitt maska y
				dy = evaluateMask( x, y, plane, 1,  1,  1,  0,  0,  0, -1, -1, -1 );	// Prewitt maska x
				break;
			case 2:
				dx = evaluateMask( x, y, plane, 1,  1, -1,  1, -2, -1,  1,  1, -1 );	// Prewitt-Robinson maska y
				dy = evaluateMask( x, y, plane, 1,  1,  1,  1, -2,  1, -1, -1, -1 );	// Prewitt-Robinson maska x
				break;
			case 3:
				dx = evaluateMask( x, y, plane, 3,  3, -5,  3,  0, -5,  3,  3, -5 );	// Kirsch maska y
				dy = evaluateMask( x, y, plane, 3,  3,  3,  3,  0,  3, -5, -5, -5 );	// Kirsch maska x
				break;
			default:
				dx = evaluateMask( x, y, plane, 1,  0, -1,  2,  0, -2,  1,  0, -1 );	// Sobel-Prewitt maska y
				dy = evaluateMask( x, y, plane, 1,  2,  1,  0,  0,  0, -1, -2, -1 );	// Sobel-Prewitt maska x
			}

			float nX,nY,nZ;
			nX = strong*dx;
			nY = strong*dy;
			nZ = 1.f;
			float normalise = 1.0f/((float) sqrt(nX*nX + nY*nY + nZ*nZ));
			nX *= normalise;
			nY *= normalise;
			nZ *= normalise;
			nX = nX*0.5f+0.5f;
			nY = nY*0.5f+0.5f;
			nZ = nZ*0.5f+0.5f;

			new_data[(sizeX*y+x)*3+0] = (unsigned char)(255.f*nX);
			new_data[(sizeX*y+x)*3+1] = (unsigned char)(255.f*nY);
			new_data[(sizeX*y+x)*3+2] = (unsigned char)(255.f*nZ);
		}
	}
	delete [] data;
	data = new_data;
	planes = 3;
}

