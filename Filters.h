#pragma once
#include "FilterC.h"
#include <Windows.h>
#include <chrono>

namespace filter
{
	using namespace std::chrono;

	void OneThreadC(int height, int width, unsigned char* bmp, milliseconds& timeResult, std::string& output)
	{
		const unsigned int size = width * height * 3;
		unsigned char* res = new unsigned char[size];
		std::memcpy(res, bmp, size);

		const auto tBegin = high_resolution_clock::now();
		for (int i = 1; i < height - 1; i++)
		{
			FilterBMP(i, width, bmp, res);
		}
		const auto tEnd = high_resolution_clock::now();

		timeResult = duration_cast<milliseconds>(tEnd - tBegin);
		output = "Results\\CppResult.bmp";
		bmpfun::ExportBitmapByArray(output, res, width, height);
		delete[] res;
	}

	void ThreadedC(int threadsNr, int height, int width, unsigned char* bmp, milliseconds& timeResult, std::string& output)
	{
		const unsigned int size = width * height * 3;
		unsigned char* res = new unsigned char[size];
		std::memcpy(res, bmp, size);
		std::vector<std::thread> threads;

		const auto tBegin = high_resolution_clock::now();
		const int operationsPerThread = height / threadsNr;
		for (int i = 0; i < threadsNr; i++)
		{
			int start = (i * operationsPerThread) + 1;
			int end = (i == threadsNr - 1) ? height - 1 : (start + operationsPerThread);
			threads.push_back(std::thread([start, end, &width, &bmp, &res]()
				{
					for (int i = start; i < end; i++)
					{
						FilterBMP(i, width, bmp, res);
					}
				}));
		}
		for (auto& t : threads)
		{
			t.join();
		}
		const auto tEnd = high_resolution_clock::now();

		timeResult = duration_cast<milliseconds>(tEnd - tBegin);
		output = "Results\\CppThreadedResult.bmp";
		bmpfun::ExportBitmapByArray(output, res, width, height);
		delete[] res;
	}

	void OneThreadNoVectorAssembly(int bitmapHeight, int bitmapWidth, unsigned char* bmp, unsigned char* assemblyOneThreadNoVectorRes)
	{
		HINSTANCE dllHandle = LoadLibrary(L"JAProjekt.dll");
		typedef int(_fastcall* MinimalFilter)(int y_pos, int bitmapWidth, const unsigned char* bmp, unsigned char* resbmp);
		if (dllHandle != 0)
		{
			const MinimalFilter minimalFilterAssemblyNoVector = (MinimalFilter)GetProcAddress(dllHandle, "MinimalFilterNoVector");
			for (int i = 1; i < bitmapHeight - 1; i++)
			{
				minimalFilterAssemblyNoVector(i, bitmapWidth, bmp, assemblyOneThreadNoVectorRes);
			}
		}
	}

	void OneThreadAssembly(int height, int width, unsigned char* bmp, milliseconds& timeResult, std::string& output)
	{
		HINSTANCE dllHandle = LoadLibrary(L"JAProjekt.dll");
		typedef int(_fastcall* MinimalFilter)(int y_pos, int width, const unsigned char* bmp, unsigned char* resbmp);
		if (dllHandle != NULL)
		{
			const MinimalFilter minimalFilterAssembly = (MinimalFilter)GetProcAddress(dllHandle, "MinimalFilter");
			const unsigned int size = width * height * 3;
			unsigned char* res = new unsigned char[size];
			std::memcpy(res, bmp, size);

			const auto tBegin = high_resolution_clock::now();
			for (int i = 1; i < height - 1; i++)
			{
				minimalFilterAssembly(i, width, bmp, res);
			}
			const auto tEnd = high_resolution_clock::now();

			timeResult = duration_cast<milliseconds>(tEnd - tBegin);
			output = "Results\\AsmResult.bmp";
			bmpfun::ExportBitmapByArray(output, res, width, height);
			delete[] res;
		}
	}

	void ThreadedAssembly(int threadsNr, int height, int width, unsigned char* bmp, milliseconds& timeResult, std::string& output)
	{
		HINSTANCE dllHandle = LoadLibrary(L"JAProjekt.dll");
		typedef int(_fastcall* MinimalFilter)(int y_pos, int width, const unsigned char* bmp, unsigned char* resbmp);
		if (dllHandle != NULL)
		{
			const MinimalFilter minimalFilterAssembly = (MinimalFilter)GetProcAddress(dllHandle, "MinimalFilter");
			const unsigned int size = width * height * 3;
			unsigned char* res = new unsigned char[size];
			std::memcpy(res, bmp, size);
			std::vector<std::thread> threads;

			const auto tBegin = high_resolution_clock::now();
			const int operationsPerThread = height / threadsNr;
			for (int i = 0; i < threadsNr; i++)
			{
				int start = (i * operationsPerThread) + 1;
				int end = (i == threadsNr - 1) ? height - 1 : start + operationsPerThread;

				threads.push_back(std::thread(([start, end, &bmp, &res, &width, minimalFilterAssembly]()
					{
						for (int j = start; j < end; j++)
						{
							minimalFilterAssembly(j, width, bmp, res);
						}
					})));
			}
			for (auto& t : threads)
			{
				t.join();
			}
			const auto tEnd = high_resolution_clock::now();

			timeResult = duration_cast<milliseconds>(tEnd - tBegin);
			output = "Results\\AsmThreadedResult.bmp";
			bmpfun::ExportBitmapByArray(output, res, width, height);
			delete[] res;
		}

	}
}