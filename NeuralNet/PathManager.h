#pragma once
#include <windows.h>
#include <iostream>
#include <vector>


class PathManager
{
public:

/*
	* Удаляет часть названия пути с конца до первого попавшегося знака '\\'.
	*
	* path - путь до файла
	*
	* count - количество удаления до знака '\\'.
	*		  Пример: если path = "Man//hello//world", то при count = 2, путь будет следующим
	*					   path = "Man"
	*
	* sizePath - размер строчки
	*/
static void cutLastName(char* path, int count, int sizePath = -1)
{
	if (sizePath == -1) sizePath = strlen(path);
	char* endSlash = path;
	int x = sizePath - 1;

	for (int i = 0; i < count; i++)
	{
		for (x; (endSlash[x] != '\\') && (x >= 0); x--);
		if (x < sizePath)
			path[x] = '\0';
	}
}

static void cutLastName(wchar_t* path, int count, int sizePath = -1)
{
	if (sizePath == -1) sizePath = lstrlenW(path);
	wchar_t* endSlash = path;
	int x = sizePath - 1;

	for (int i = 0; i < count; i++)
	{
		for (x; (endSlash[x] != '\\') && (x >= 0); x--);
		if (x < sizePath)
			path[x] = '\0';
	}
}

/*
* Возвращает указатель на последнюю часть пути (до count знаков '\' c конца)
*
* сount - количество удаления до знака '\\'.
*	Пример: если path = "Man//hello//world", то при count = 2, путь будет следующим path = "hello//world"
*
* sizePath - размер строчки
*/

static char* getLastName(char* path, int count, int sizePath = -1)
{
	if (sizePath == -1) sizePath = strlen(path);
	int countSlash = 0;

	int x = sizePath - 1;
	for (int i = 0; i < count; i++)
		for (x; x >= 0 && path[x] != '\\'; x--);

	return path + x + 1;
}

static wchar_t* getLastName(wchar_t* path, int count, int sizePath = -1)
{
	if (sizePath == -1) sizePath = lstrlenW(path);
	int countSlash = 0;

	int x = sizePath - 1;
	for (int i = 0; i < count; i++)
		for (x; x >= 0 && path[x] != '\\'; x--);

	return path + x + 1;
}

/*
* Удаляет часть названия пути с конца до первого попавшегося знака '\\',
* а затем вставляет в конец строку ins
*
* path - путь до файла
*
* count - количество удаления до знака '\\'.
*		  Пример: если path = "Man//hello//world", то при count = 2, путь будет следующим
*					   path = "Man"
*
* ins - строка, которая после сокращения пути path добавляется к строке path в конец
*
* sizePath - размер строчки
*/
static void addNameInstance(char* path, int count, const char* ins, int sizePath = -1)
{
	cutLastName(path, count, sizePath);
	strcat(path, ins);
}

static void addNameInstance(wchar_t* path, int count, const wchar_t* ins, int sizePath = -1)
{
	cutLastName(path, count, sizePath);
	lstrcatW(path, ins);
}

/*
*
* Копирует путь path в outPath, а затем удаляет часть названия пути outPath с конца до первого попавшегося знака '\\',
* а затем вставляет в конец строку ins
*
* path - путь до файла
*
* count - количество удаления до знака '\\'.
*		  Пример: если path = "Man//hello//world", то при count = 2, путь будет следующим
*					   path = "Man"
*
* ins - строка, которая после сокращения пути path добавляется к строке path в конец
*
* sizePath - размер строчки
*/
static char* addNameInstance(char* path, char* outPath, int count, const char* ins, int sizePath = -1)
{
	strcpy(outPath, path);
	cutLastName(outPath, count, sizePath);
	strcat(outPath, ins);
	return outPath;
}

static wchar_t* addNameInstance(wchar_t* path, wchar_t* outPath, int count, const wchar_t* ins, int sizePath = -1)
{
	lstrcpyW(outPath, path);
	cutLastName(outPath, count, sizePath);
	lstrcatW(outPath, ins);
	return outPath;
}

static void getFolderWithExe(char* path)
{
	DWORD sizePath = GetModuleFileNameA(NULL, path, strlen(path));
	cutLastName(path, 1, sizePath);
}

static void getFolderWithExe(wchar_t* path)
{
	DWORD sizePath = GetModuleFileNameW(NULL, path, lstrlenW(path));
	cutLastName(path, 1, sizePath);
}

static char** getFolderNames(char* path, int* countNames)
{
	int size = strlen(path);

	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind;
	hFind = FindFirstFileA(path, &FindFileData);

	if (hFind == INVALID_HANDLE_VALUE)
		throw;

	FindNextFileA(hFind, &FindFileData);
	FindNextFileA(hFind, &FindFileData);

	std::vector<char*> namesVec = std::vector<char*>();

	do
	{
		int sizeName = strlen(FindFileData.cFileName);
		char* str = new char[size + sizeName];
		memcpy(str, path, size - 1);
		memcpy(str + size - 1, FindFileData.cFileName, sizeName);
		str[size + sizeName - 1] = '\0';

		namesVec.push_back(str);
	} while (FindNextFileA(hFind, &FindFileData));

	char** names = new char* [namesVec.size()];
	for (int x = 0; x < namesVec.size(); x++)
		names[x] = namesVec[x];

	*countNames = namesVec.size();
	return names;
}

static wchar_t** getFolderNames(wchar_t* path, int* countNames)
{
	int size = lstrlenW(path);

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	hFind = FindFirstFileW(path, &FindFileData);

	if (hFind == INVALID_HANDLE_VALUE)
		throw;

	FindNextFileW(hFind, &FindFileData);
	FindNextFileW(hFind, &FindFileData);

	std::vector<wchar_t*> namesVec = std::vector<wchar_t*>();

	do
	{
		int sizeName = lstrlenW(FindFileData.cFileName);
		wchar_t* str = new wchar_t[size + sizeName];
		memcpy(str, path, size - 1);
		memcpy(str + size - 1, FindFileData.cFileName, sizeName);
		str[size + sizeName - 1] = L'\0';

		namesVec.push_back(str);
	} while (FindNextFileW(hFind, &FindFileData));

	wchar_t** names = new wchar_t* [namesVec.size()];
	for (int x = 0; x < namesVec.size(); x++)
		names[x] = namesVec[x];

	*countNames = namesVec.size();
	return names;
}

static void IntToString(int num, char* string)
{
	int size = num == 0 ? 1 : log10f(num) + 1;
	for (int x = size - 1; x >= 0; x--)
	{
		string[x] = '0' + num % 10;
		num /= 10;
	}
	string[size] = '\0';
}

static void IntToString(int num, wchar_t* string)
{
	int size = num == 0 ? 1 : log10f(num) + 1;
	for (int x = size - 1; x >= 0; x--)
	{
		string[x] = L'0' + num % 10;
		num /= 10;
	}
	string[size] = L'\0';
}
};