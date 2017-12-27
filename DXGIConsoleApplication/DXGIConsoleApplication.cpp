// DXGIConsoleApplication.cpp : Defines the entry point for the console application.
//

#include "DuplicationManager.h"
#include <time.h>

clock_t start = 0, stop = 0, duration = 0;
int count = 0;
FILE *log_file;
char file_name[MAX_PATH];

void save_as_bitmap(unsigned char *bitmap_data, int rowPitch, int height, char *filename)
{
	// A file is created, this is where we will save the screen capture.

	FILE *f;

	BITMAPFILEHEADER   bmfHeader;
	BITMAPINFOHEADER   bi;

	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = rowPitch/4;
	//Make the size negative if the image is upside down.
	bi.biHeight = -height;
	//There is only one plane in RGB color space where as 3 planes in YUV.
	bi.biPlanes = 1;
	//In windows RGB, 8 bit - depth for each of R, G, B and alpha.
	bi.biBitCount = 32;
	//We are not compressing the image.
	bi.biCompression = BI_RGB;
	// The size, in bytes, of the image. This may be set to zero for BI_RGB bitmaps.
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	// rowPitch = the size of the row in bytes.
	DWORD dwSizeofImage = rowPitch * height;

	// Add the size of the headers to the size of the bitmap to get the total file size
	DWORD dwSizeofDIB = dwSizeofImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	//Offset to where the actual bitmap bits start.
	bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

	//Size of the file
	bmfHeader.bfSize = dwSizeofDIB;

	//bfType must always be BM for Bitmaps
	bmfHeader.bfType = 0x4D42; //BM   

							   // TODO: Handle getting current directory
	fopen_s(&f, filename, "wb");

	DWORD dwBytesWritten = 0;
	dwBytesWritten += fwrite(&bmfHeader, sizeof(BITMAPFILEHEADER), 1, f);
	dwBytesWritten += fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, f);
	dwBytesWritten += fwrite(bitmap_data, 1, dwSizeofImage, f);

	fclose(f);
}

int main()
{
	fopen_s(&log_file, "logY.txt", "w");

	DUPLICATIONMANAGER DuplMgr;
	DUPL_RETURN Ret;

	UINT Output = 0;
	
	// Make duplication manager
	Ret = DuplMgr.InitDupl(log_file, Output);
	if (Ret != DUPL_RETURN_SUCCESS)
	{
		fprintf_s(log_file,"Duplication Manager couldn't be initialized.");
		return 0;
	}

	BYTE* pBuf = new BYTE[10000000];
	
	// Main duplication loop
	for (int i = 0; i < 100; i++)
	{
		// Get new frame from desktop duplication
		Ret = DuplMgr.GetFrame(pBuf);
		if (Ret != DUPL_RETURN_SUCCESS)
		{
			fprintf_s(log_file, "Could not get the frame.");
		}
		sprintf_s(file_name, "%d.bmp", i);
		save_as_bitmap(pBuf, DuplMgr.GetImagePitch(), DuplMgr.GetImageHeight(), file_name);
	}
	delete pBuf;

	fclose(log_file);
    return 0;
}
