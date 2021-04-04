#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <iostream>
#include <urlmon.h>
#include <string>
#include <thread>
#include <random>
#include <fstream>
#include <chrono>

#pragma comment(lib, "urlmon.lib")

std::string szPicturePattern = "<meta name=\"twitter:image:src\" content=\"";
std::string szBadUrl = "//st.prntscr.com/2018/10/13/2048/img/0_173a7b_211be8ff.png";
std::string szSiteUrl = "https://prnt.sc/";
std::string szSymbols = "qwertyuiopasdfghjklzxcvbnm1234567890"; //QWETYUIOPASDFGHJKLZXCVBNM";
constexpr short iStrSize = 6;


wchar_t* ChToLPCWSTR(const char* charArray)
{
	wchar_t* wString = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
	return wString;
}

void MainThread()
{
	std::cout << "Поток #" << GetCurrentThreadId() << " начинает работу." << std::endl;
	while (true)
	{
		std::string szSubdomain = "";
		int randsec = time(NULL);

		for (auto i = 0; i < iStrSize; i++)
			szSubdomain += szSymbols[(rand() * (GetCurrentThreadId() % 10) + randsec % 1000 + rand()) / 73 % szSymbols.size()];

		char szFullUrl[256];
		sprintf(szFullUrl, "%s%s", szSiteUrl.c_str(), szSubdomain.c_str());// std::cout << szFullUrl << std::endl;

		char szFileName[256];
		sprintf(szFileName, "%s%s", szSubdomain.c_str(), ".txt");

		HRESULT hr = URLDownloadToFile(NULL, ChToLPCWSTR(szFullUrl), ChToLPCWSTR(szFileName), 0, NULL);
		if (FAILED(hr))
			std::cout << "Поток #" << GetCurrentThreadId() << " не может скачать файл." << std::endl;

		std::ifstream HtmlFile(szFileName);
		if (!HtmlFile.is_open())
		{
			std::cout << "Поток #" << GetCurrentThreadId() << " не может открыть скачанный html-файл." << std::endl;
			DeleteFile(ChToLPCWSTR(szFileName));
		}
		else
		{
			size_t iFileLength = HtmlFile.seekg(NULL, std::ios::end).tellg();
			char* Buffer = (char*)malloc(iFileLength + 1);
			HtmlFile.seekg(NULL, std::ios::beg).read(Buffer, iFileLength);
			Buffer[iFileLength] = NULL;
			std::string szHtml = Buffer;
			free(Buffer);

			int iFind = szHtml.find(szPicturePattern);
			if (iFind != std::string::npos)
			{
				std::string szPicture;
				int iPictureStart = iFind + szPicturePattern.size();
				for (int i = 0; szHtml.at(iPictureStart + i) != '\"'; i++)
					szPicture += szHtml.at(iPictureStart + i);

				if (strcmp(szPicture.c_str(), szBadUrl.c_str()))
				{
					std::string szPngName = szSubdomain + ".png";
					URLDownloadToFile(NULL, ChToLPCWSTR(szPicture.c_str()), ChToLPCWSTR(szPngName.c_str()), 0, NULL);
					std::cout << "Поток #" << GetCurrentThreadId() << " успешно нашёл скриншот №" << szSubdomain << "." << std::endl;
				}
				else std::cout << "Поток #" << GetCurrentThreadId() << " нашёл удалённый скриншот." << std::endl;
			}
			else std::cout << "Поток #" << GetCurrentThreadId() << " не нашёл картинки(скриншот ещё не был сделан)." << std::endl;
		}

		HtmlFile.close();
		DeleteFile(ChToLPCWSTR(szFileName));
		Sleep(500);
	}
}

int main()
{
	setlocale(LC_ALL, "");
	srand(time(NULL));

	std::thread Thread1(MainThread);
	std::thread Thread2(MainThread);
	std::thread Thread3(MainThread);
	std::thread Thread4(MainThread);
	std::thread Thread5(MainThread);
	std::thread Thread6(MainThread);
	std::thread Thread7(MainThread);
	std::thread Thread8(MainThread);
	std::thread Thread9(MainThread);
	std::thread Thread10(MainThread);
	
	Thread1.join();
	Thread2.join();
	Thread3.join();
	Thread4.join();
	Thread5.join();
	Thread6.join();
	Thread7.join();
	Thread8.join();
	Thread9.join();
	Thread10.join();
	
	return NULL;
}