#include "pch.h"
#include "Filter.h"
#include "FilterC.h"

static void FindMinRGB(int& minR, int& minG, int& minB, unsigned char* bmp, int y_pos, int x_pos, int width)
{
	//positioning at top left
	y_pos++;
	x_pos--;
	int R{}, G{}, B{};
	for (int y = y_pos; y >= y_pos - 2; y--)
	{
		for (int x = x_pos; x < x_pos + 3; x++)
		{
			//calculate address in array
			int res = y * 3 * width + (x * 3);
			R = bmp[res];
			G = bmp[res + 1];
			B = bmp[res + 2];
			//compare to minimal values
			if (minR > R)
			{
				minR = R;
			}
			if (minG > G)
			{
				minG = G;
			}
			if (minB > B)
			{
				minB = B;
			}
		}
	}
}

void FilterBMP(int y_pos, int width, unsigned char* bmp, unsigned char* resbmp)
{
	int minR{ 255 }, minG{ 255 }, minB{ 255 };
	int R{}, G{}, B{};
	int x_pos{ 1 };
	for (int col = x_pos; col < width - 1; col++)
	{
		FindMinRGB(minR, minG, minB, bmp, y_pos, col, width);
		//change center pixel
		int res = y_pos * 3 * width + (col*3);
		resbmp[res] = minR;
		resbmp[res + 1] = minG;
		resbmp[res + 2] = minB;
		minR = minG = minB = 255;
	}
}