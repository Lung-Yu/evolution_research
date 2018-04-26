#ifndef _BMP_READER_H_
#define _BMP_READER_H_

#include "bmp_header_def.h"

class CBmpReader {
public:
	CBmpReader();
	~CBmpReader();
	
public:
	int Load(const char * bmp_file);
	void Unload();

	unsigned int GetWidth() const;
	unsigned int GetHeight() const;
	
	PBITMAPFILEHEADER GetFileHeader() const;
	PBITMAPINFOHEADER GetInfoHeader() const;
	
	char * GetRawData() const;
	
	unsigned int GetRedAt(int row, int col) const;
	unsigned int GetGreenAt(int row, int col) const;
	unsigned int GetBlueAt(int row, int col) const;
	
private:
	PBITMAPFILEHEADER m_pFileHeader;
	PBITMAPINFOHEADER m_pInfoHeader;
	char * m_buffer;
	char * m_pRawData;
	unsigned int m_nWidth;
	unsigned int m_nHeight;
	unsigned int m_nLineBytes;
};

#endif