#include <iostream>
#include "bmp_reader.h"

using namespace std;

#define OUTPUT_TITLE(title) \
	cout << "[" << #title << "]" << endl;

#define OUTPUT_ITEM_CONTENT(item, name) \
	cout << "\t" << #name << ": " << item->name << endl;

void OutputBitMapFileHeader(PBITMAPFILEHEADER pFileHeader) {
	OUTPUT_TITLE("Bitmap File Header");
	
	if (NULL == pFileHeader) {
		return;
	}

	OUTPUT_ITEM_CONTENT(pFileHeader, bfType);
	OUTPUT_ITEM_CONTENT(pFileHeader, bfSize);
	OUTPUT_ITEM_CONTENT(pFileHeader, bfReserved1);
	OUTPUT_ITEM_CONTENT(pFileHeader, bfReserved2);
	OUTPUT_ITEM_CONTENT(pFileHeader, bfOffBits);
}

void OutputBitMapInfoHeader(PBITMAPINFOHEADER pInfoHeader) {
	OUTPUT_TITLE("Bitmap Info Header");
	
	if (NULL == pInfoHeader) {
		return;
	}

	OUTPUT_ITEM_CONTENT(pInfoHeader, biSize);
	OUTPUT_ITEM_CONTENT(pInfoHeader, biWidth);
	OUTPUT_ITEM_CONTENT(pInfoHeader, biHeight);
	OUTPUT_ITEM_CONTENT(pInfoHeader, biPlanes);
	OUTPUT_ITEM_CONTENT(pInfoHeader, biBitCount);
	OUTPUT_ITEM_CONTENT(pInfoHeader, biCompression);
	OUTPUT_ITEM_CONTENT(pInfoHeader, biSizeImage);
	OUTPUT_ITEM_CONTENT(pInfoHeader, biXPelsPerMeter);
	OUTPUT_ITEM_CONTENT(pInfoHeader, biYPelsPerMeter);
	OUTPUT_ITEM_CONTENT(pInfoHeader, biClrUsed);
	OUTPUT_ITEM_CONTENT(pInfoHeader, biClrImportant);
}

void OutputColorAt(const CBmpReader &reader, int row, int col) {
	cout << "BGR @ [" << row << ", " << col << "]: "  
		<< reader.GetBlueAt(row, col) << ", "
		<< reader.GetGreenAt(row, col) << ", "
		<< reader.GetRedAt(row, col) << endl;
}

int main(int argc, char * argv[]) {
	CBmpReader reader;
	reader.Load("./data.bmp");
	
	int width = reader.GetWidth();
	int height = reader.GetHeight();
	cout << "width: " << width << " height: " << height << endl;

	PBITMAPFILEHEADER pFileHeader = reader.GetFileHeader();
	OutputBitMapFileHeader(pFileHeader);

	PBITMAPINFOHEADER pInfoHeader = reader.GetInfoHeader();
	OutputBitMapInfoHeader(pInfoHeader);
	
	OutputColorAt(reader, 0, 0);
	OutputColorAt(reader, 0, 1);
	OutputColorAt(reader, 0, 2);
	OutputColorAt(reader, 1, 0);
	OutputColorAt(reader, 1, 1);
	OutputColorAt(reader, 1, 2);
	OutputColorAt(reader, 0, width - 1);
	OutputColorAt(reader, height - 1, 0);
	OutputColorAt(reader, height - 1, width - 1);

	return 0;
}