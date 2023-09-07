/* 定义了BMP文件的
 * 位图文件头和
 * 位图信息头的结构
 */

#ifndef BMPCLASS_H
#define BMPCLASS_H

#include <math.h>
#include <QFile>
#include <QVector>
#include <QString>
#include <QMessageBox>

typedef quint8 BYTE; //8位无符号数，1个字节
typedef quint16 WORD; //16位无符号数，2个字节
typedef quint32 DWORD; //32位无符号数，4个字节
typedef qint32 LONG; //32位整数，4个字节

typedef struct tagBITMAPFILEHEADER {
        WORD    bfType;
        DWORD   bfSize;
        WORD    bfReserved1;
        WORD    bfReserved2;
        DWORD   bfOffBits;
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER{
        DWORD      biSize;
        LONG       biWidth;
        LONG       biHeight;
        WORD       biPlanes;
        WORD       biBitCount;
        DWORD      biCompression;
        DWORD      biSizeImage;
        LONG       biXPelsPerMeter;
        LONG       biYPelsPerMeter;
        DWORD      biClrUsed;
        DWORD      biClrImportant;
} BITMAPINFOHEADER;

typedef struct tagRGBQUAD
{
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed;
    BYTE rgbReserved;
} RGBQUAD;

#endif // BMPCLASS_H
