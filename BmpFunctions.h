#pragma once
#include <vector>
#include <fstream>
#include <string>

namespace bmpfun
{
	unsigned char* LoadBMPFromFile(const std::string& filename, int& bitmapWidth, int& bitmapHeight)
	{
		std::fstream inputFile(filename, std::ios::in | std::ios::binary);
		if (!inputFile.is_open())
		{
			return nullptr;
		}
		else
		{
			const int headerSize = 14;
			const int informationHeaderSize = 40;

			unsigned char bmpHeader[headerSize]{};
			inputFile.read(reinterpret_cast<char*>(bmpHeader), headerSize);

			unsigned char bmpInformationHeader[informationHeaderSize]{};
			inputFile.read(reinterpret_cast<char*>(bmpInformationHeader), informationHeaderSize);

			int fileSize = bmpHeader[2] + (bmpHeader[3] << 8) + (bmpHeader[4] << 16) + (bmpHeader[5] << 24);

			bitmapWidth = bmpInformationHeader[4] + (bmpInformationHeader[5] << 8)
				+ (bmpInformationHeader[6] << 16) + (bmpInformationHeader[7] << 24);

			bitmapHeight = bmpInformationHeader[8] + (bmpInformationHeader[9] << 8)
				+ (bmpInformationHeader[10] << 16) + (bmpInformationHeader[11] << 24);

			int padding = (4 - (3 * bitmapWidth) % 4) % 4;
			unsigned char* bmp = new unsigned char[3 * bitmapWidth * bitmapHeight];

			int j{ 0 };
			for (int y = 0; y < bitmapHeight; y++)
			{
				for (int x = 0; x < bitmapWidth; x++)
				{
					unsigned char colors[3]{};
					inputFile.read(reinterpret_cast<char*>(colors), 3);

					//R
					bmp[y * bitmapWidth * 3 + j] = colors[2];
					//G
					bmp[y * bitmapWidth * 3 + j + 1] = colors[1];
					//B
					bmp[y * bitmapWidth * 3 + j + 2] = colors[0];

					j += 3;
				}
				j = 0;
				inputFile.ignore(padding);
			}
			j = 0;
			for (int y = 0; y < bitmapHeight; y++)
			{
				for (int x = 0; x < bitmapWidth; x++)
				{
					auto red = bmp[y * bitmapWidth * 3 + j];
					auto green = bmp[y * bitmapWidth * 3 + j + 1];
					auto blue = bmp[y * bitmapWidth * 3 + j + 2];
					j += 3;
				}
				j = 0;
			}

			return bmp;
		}
	}

	void ExportBitmapByArray(const std::string& outputName, unsigned char* bmp, int bitmapWidth, int bitmapHeight)
	{
		std::ofstream output(outputName, std::ios::out | std::ios::binary);

		unsigned char bmpPad[3] = { 0,0,0 };
		const int headerSize = 14;
		const int informationHeaderSize = 40;
		int padding = (4 - (3 * bitmapWidth) % 4) % 4;
		int filesize = headerSize + informationHeaderSize + bitmapWidth * bitmapHeight * 3 + padding * bitmapHeight;

		unsigned char fileHeader[headerSize]{};
		fileHeader[0] = 'B';
		fileHeader[1] = 'M';
		fileHeader[2] = filesize;
		fileHeader[3] = filesize >> 8;
		fileHeader[4] = filesize >> 16;
		fileHeader[5] = filesize >> 24;
		fileHeader[6] = 0;
		fileHeader[7] = 0;
		fileHeader[8] = 0;
		fileHeader[9] = 0;
		fileHeader[10] = headerSize + informationHeaderSize;
		fileHeader[11] = 0;
		fileHeader[12] = 0;
		fileHeader[13] = 0;
		unsigned char infoHeader[informationHeaderSize]{};
		infoHeader[0] = informationHeaderSize;
		infoHeader[1] = 0;
		infoHeader[2] = 0;
		infoHeader[3] = 0;
		infoHeader[4] = bitmapWidth;
		infoHeader[5] = bitmapWidth >> 8;
		infoHeader[6] = bitmapWidth >> 16;
		infoHeader[7] = bitmapWidth >> 24;
		infoHeader[8] = bitmapHeight;
		infoHeader[9] = bitmapHeight >> 8;
		infoHeader[10] = bitmapHeight >> 16;
		infoHeader[11] = bitmapHeight >> 24;
		infoHeader[12] = 1;
		infoHeader[13] = 0;
		infoHeader[14] = 24;
		for (int i = 15; i < 40; i++)
		{
			infoHeader[i] = 0;
		}

		output.write(reinterpret_cast<char*>(fileHeader), headerSize);
		output.write(reinterpret_cast<char*>(infoHeader), informationHeaderSize);
		int j = 0;
		for (int y = 0; y < bitmapHeight; y++)
		{
			for (int x = 0; x < bitmapWidth; x++)
			{
				unsigned char r = bmp[y * bitmapWidth * 3 + j];
				unsigned char g = bmp[y * bitmapWidth * 3 + j + 1];
				unsigned char b = bmp[y * bitmapWidth * 3 + j + 2];;
				unsigned char color[] = { b, g, r };
				output.write(reinterpret_cast<char*>(color), 3);
				j += 3;
			}
			j = 0;
			output.write(reinterpret_cast<char*>(bmpPad), padding);
		}

		output.close();
	}
}