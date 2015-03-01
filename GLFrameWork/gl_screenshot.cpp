#include "GL\include\GL\glut.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "gl_screenshot.h"
#include "main.h"


gl_screenshot::gl_screenshot()
{
}

gl_screenshot::~gl_screenshot()
{
}

// 画面の内容をビットマップファイルに出力
//   filename: 出力ファイル名
//   bpp: ピクセルあたりのビット数（24, 32）
bool gl_screenshot::screenshot(const char* filename,int bpp)
{
 const int width = SCREEN_WIDTH;   // 画像の大きさ
 const int height = SCREEN_HEIGHT;
 // ピクセルデータ全体のサイズ
 const int datasize = height*( ( ( ( width*bpp / 8 ) + 3 ) >> 2 ) << 2 );
 // ファイルサイズ
 const int filesize = 2 + sizeof(BITMAPFILEHEADER_) + sizeof(BITMAPINFOHEADER_) + datasize;

 // ビットマップのヘッダ
 BITMAPFILEHEADER_ bmfh = {filesize,0,0,54,};
 BITMAPINFOHEADER_ bmih = {40,width,height,1,bpp,0,0,0,0,0,0,};

 // データのフォーマット
 int format;
 if(bpp == 24) {
  format = GL_RGB;
 }
 else if(bpp == 32) {
  format = GL_RGBA;
 }
 else {
  std::cerr << "invalid parameter 'bpp'" << std::endl;
  return false;
 }

 // データをもらう
 std::vector<GLubyte> buf(datasize);
 //glPixelStorei(GL_PACK_ALIGNMENT, 1);
 //glReadBuffer(GL_FRONT);
 glReadPixels(0,0,width,height,format,GL_UNSIGNED_BYTE,&buf[0]);

 // RGB → BGR
 for(int i = 0; i<datasize; i += bpp / 8) {
  std::swap(buf[i],buf[i + 2]);
 }

 // 出力
 std::ofstream fs(filename,std::ios::out | std::ios::trunc | std::ios::binary);
 if(!fs) {
  std::cerr << "fstream::open() failed." << std::endl;
  return false;
 }

 fs.write("BM",2);
 fs.write(reinterpret_cast<const char*>( &bmfh ),sizeof(BITMAPFILEHEADER_));
 fs.write(reinterpret_cast<const char*>( &bmih ),sizeof(BITMAPINFOHEADER_));
 fs.write(reinterpret_cast<const char*>( &buf[0] ),datasize);
 fs.close();
 return true;
}