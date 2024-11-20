// MathLibrary.h - Contains declarations of math functions
#pragma once

#ifdef FILTERC_EXPORTS
#define FILTERC_API __declspec(dllexport)
#else
#define FILTERC_API __declspec(dllimport)
#endif

extern "C" FILTERC_API void FilterBMP(int y_pos, int width, unsigned char* bmp, unsigned char* resbmp);