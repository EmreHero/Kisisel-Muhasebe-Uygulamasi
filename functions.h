#include <Windows.h>
#include <fstream>
#include <codecvt>
#include <string>

#define URUN (HMENU) 1
#define OZET (HMENU) 2

#define URUN_URUN	(HMENU) 11
#define URUN_LIRA	(HMENU) 12
#define URUN_KURUS	(HMENU) 13
#define URUN_GUN	(HMENU) 14
#define URUN_AY		(HMENU) 15
#define URUN_YIL	(HMENU) 16
#define	URUN_TAMAM	(HMENU) 17
#define URUN_SIL	(HMENU) 18
#define URUN_EDIT	(HMENU) 19

#define OZET_LISTE  (HMENU) 21

wchar_t* GetContent(HWND hwnd) {
	wchar_t buffer[16];
	GetWindowText(hwnd, buffer, GetWindowTextLength(hwnd) + 1);
	return buffer;
}

namespace Log {
	void Load(HWND ozet, HWND aylik);
	void Delete(HWND aylik, std::wstring item);
	void Rename(HWND urun, HWND ozet);
}
void Log::Load(HWND ozet, HWND aylik) {

	SendMessage(ozet, LB_RESETCONTENT, NULL, 0);
	SendMessage(aylik, LB_RESETCONTENT, NULL, 0);
	tm ltm;
	time_t now = time(0);
	localtime_s(&ltm, &now);

	wchar_t gunkac[16] = {}, aykac[16] = {}, yilkac[16] = {};
	wsprintfW(gunkac, L"%d", ltm.tm_mday);
	wsprintfW(aykac, L"%d", ltm.tm_mon + 1);
	wsprintfW(yilkac, L"%d", ltm.tm_year + 1900);
	std::wstring wdate[3];
	wdate[0] = std::wstring(yilkac);
	wdate[1] = std::wstring(aykac);
	wdate[2] = std::wstring(gunkac);

	int dateNow[3] = {}, dateMin[3] = {}; // 0 year, 1 month, 2 day
	int day;
	for (int i = 0; i < 3; i++) dateNow[i] = _wtoi(wdate[i].c_str());
	if (dateNow[1] != 1) {
		dateMin[1] = dateNow[1] - 1;
		dateMin[0] = dateNow[0];
	}
	else {
		dateMin[1] = 12;	
		dateMin[0] = dateNow[0] - 1;
	}
	switch (dateMin[1]) {
		case 1: case 3: case 5: case 7: case 8: case 10: case 12:
			day = 1;
			break;
		case 4: case 6: case 9: case 11:
			day = 0;
			break;
		case 2:
			if (dateNow[0] % 4 == 0) day = -1;
			else day = -2;
			break;
	}
	dateMin[2] = dateNow[2] + day;

	std::locale loc(std::locale::classic(), new std::codecvt_utf8<wchar_t>);
	std::wifstream list;
	list.open("dokuz/list.txt");
	list.imbue(loc);
	std::wifstream log;
	log.imbue(loc);

	int content[5] = {};
	int total[] = {0, 0};
	wchar_t* outputPtr;

	std::wstring listItem, logItem;
	while (list >> listItem) {
		std::wstring logLocation = L"dokuz/logs/";
		logLocation.append(listItem);
		logLocation.append(L".txt");
		log.open(logLocation);
		int price[] = { 0, 0 };
		int count = 0;
		while (log >> logItem) {
			std::wstring wcontent[5] = {};
			int i = 0;
			for (wchar_t c : logItem) {
				if (c == L'.') i++;
				else wcontent[i] += c;
			}
			for (int j = 0; j < 5; j++) content[j] = _wtoi(wcontent[j].c_str());
			if (content[0] == dateNow[0] && content[1] == dateNow[1] && content[2] <= dateNow[2] 
				|| content[0] == dateMin[0] && content[1] == dateMin[1] && content[2] >= dateMin[2]) {
				price[0] += content[3];
				price[1] += content[4];
			}
			count++;
		}
		log.close();
		for (; price[1] >= 100; price[1] - 100) price[0]++;
		std::wstring output = listItem;
		output.append(L" - ");
		output.append(std::to_wstring(price[0]));
		output.append(L",");
		output.append(std::to_wstring(price[1]));
		output.append(L"TL, ");
		output.append(std::to_wstring(count));
		output.append(L" Adet");
		total[0] += price[0];
		total[1] += price[1];
		outputPtr = &output[0];
		if (price[0] + price[1] != 0)SendMessage(ozet, LB_ADDSTRING, NULL, (LPARAM)outputPtr);
	}
	list.close();
	std::wstring totalOutput = L"Bu ay ";
	for (; total[1] >= 100; total[1] - 100) total[0]++;
	totalOutput.append(std::to_wstring(total[0]));
	totalOutput.append(L",");
	totalOutput.append(std::to_wstring(total[1]));
	totalOutput.append(L" TL harcadýnýz.");
	outputPtr = &totalOutput[0];
	SendMessage(aylik, LB_ADDSTRING, NULL, (LPARAM)outputPtr);
	
}
void Log::Delete(HWND aylik, std::wstring item) {
	
	std::wstring location = L"dokuz/logs/";
	location.append(item.append(L".txt"));
	const wchar_t* ptr = location.c_str();
	_wremove(ptr);
	return;
}
void Log::Rename(HWND urun, HWND ozet) {

	// burada kaldým
	return;
}

namespace Urun {
	void Load(HWND urun);
	void LoadDate(HWND gun, HWND ay, HWND yil);
	void Reload(HWND urun, wchar_t* wurunptr, bool exsists, HWND lira, HWND kurus, HWND gun, HWND ay, HWND yil, HWND ozet, HWND aylik);
	void Remove(HWND urun, HWND ozet, HWND aylik);
	void Tamam(HWND urun, HWND lira, HWND kurus, HWND gun, HWND ay, HWND yil, HWND ozet, HWND aylik);
}
void Urun::Load(HWND urun) {

	std::locale loc(std::locale::classic(), new std::codecvt_utf8<wchar_t>);
	std::wifstream list;
	list.open("dokuz/list.txt");
	list.imbue(loc);
	std::wstring witem;

	while (list >> witem) {
		const wchar_t* witemptr = witem.c_str();

		SendMessage(urun, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)witemptr);
	}
	list.close();
}
void Urun::LoadDate(HWND gun, HWND ay, HWND yil) {
	tm ltm;
	time_t now = time(0);
	localtime_s(&ltm, &now);
	wchar_t gunkac[16] = {}, aykac[16] = {}, yilkac[16] = {};
	wsprintfW(gunkac, L"%d", ltm.tm_mday);
	wsprintfW(aykac, L"%d", ltm.tm_mon + 1);
	wsprintfW(yilkac, L"%d", ltm.tm_year + 1900);

	SetWindowText(gun, gunkac);
	SetWindowText(ay, aykac);
	SetWindowText(yil, yilkac);
}
void Urun::Reload(HWND urun, wchar_t* wurunptr, bool exsists, HWND lira, HWND kurus, HWND gun, HWND ay, HWND yil, HWND ozet, HWND aylik) {

	// urun reload
	SetWindowText(urun, L"");
	if (!exsists) SendMessage(urun, (UINT)CB_ADDSTRING, 0, (LPARAM)wurunptr);

	// lira reload
	SetWindowText(lira, L"0");

	// kurus reload
	SetWindowText(kurus, L"00");

	// date reload
	Urun::LoadDate(gun, ay, yil);

	// ozet reload
	Log::Load(ozet, aylik);
}
void Urun::Remove(HWND urun, HWND ozet, HWND aylik) { 

	wchar_t wurun[16] = {};
	wchar_t* contentptr = GetContent(urun);
	for (int i = 0; i < 16; i++) {
		wurun[i] = contentptr[i];
	}
	std::wstring wurunstr(wurun), item;

	std::locale loc(std::locale::classic(), new std::codecvt_utf8<wchar_t>);
	std::wifstream listin;
	listin.open("dokuz/list.txt");
	listin.imbue(loc);
	std::wfstream listout;
	listout.open("dokuz/temp.txt", std::fstream::out);
	listout.imbue(loc);

	while (listin >> item) {
		if (item != wurunstr) {
			listout << item << '\n';
		}
	}
	listin.close();
	listout.close();

	remove("dokuz/list.txt");
	rename("dokuz/temp.txt", "dokuz/list.txt");

	Log::Delete(aylik, wurunstr);
	SendMessage(urun, CB_RESETCONTENT, 0, 0);
	Log::Load(ozet, aylik);
	Urun::Load(urun);
}
void Urun::Tamam(HWND urun, HWND lira, HWND kurus, HWND gun, HWND ay, HWND yil, HWND ozet, HWND aylik) {
	// check if everything is expected
	wchar_t content[5][16] = {};
	wchar_t* contentptr = {};
	for (int i = 0; i < 5; i++) {
		switch (i) {
			case 0:
				contentptr = GetContent(yil);
				break;
			case 1:
				contentptr = GetContent(ay);
				break;
			case 2:
				contentptr = GetContent(gun);
				break;
			case 3:
				contentptr = GetContent(lira);
				break;
			case 4:
				contentptr = GetContent(kurus);
				break;
		}
		for (int j = 0; j < 16; j++) {
			content[i][j] = contentptr[j];
		}
	}
	std::wstring contentstr[5];
	for (int i = 0; i < 5; i++) {
		contentstr[i] = std::wstring(content[i]);
		for (char c : contentstr[i]) {
			if (!std::isdigit(c)) {
				MessageBox(urun, L"Inputs need to be numeric.", L"ERROR", 0);
				return;
			}
		}
	}
	wchar_t wurun[16] = {};
	contentptr = GetContent(urun);
	for (int i = 0; i < 16; i++) {
		wurun[i] = contentptr[i];
	}
	std::wstring wurunstr(wurun), item;
	if (wurunstr == L"") return;
	for (wchar_t c : wurunstr) {
		if (!iswalpha(c) && !iswdigit(c) && c != L' ') {
			MessageBox(urun, L"Unexpected character in item name.", L"ERROR", 0);
			return;
		}
	}
	// check if urun is in list.txt
	std::locale loc(std::locale::classic(), new std::codecvt_utf8<wchar_t>);
	std::wifstream listin;
	listin.open("dokuz/list.txt");
	listin.imbue(loc);
	
	bool exsists = false;
	while (listin >> item) {
		if (item == wurunstr || item == L"") {
			exsists = true;
			break;
		}
	}
	listin.close();
	if (!exsists) {
		std::wofstream listout;
		listout.open("dokuz/list.txt", std::ios::app);
		listout.imbue(loc);

		std::wstring urunout = L"\n";
		urunout.append(wurunstr);
		listout << urunout;
		listout.close();
	}
	// save logs, saves as "year.month.day.lira.kurus"
	std::wofstream log;
	std::wstring logLocation = L"dokuz/logs/";
	logLocation.append(wurunstr.append(L".txt"));
	log.open(logLocation, std::fstream::out | std::fstream::app);
	for (int i = 0; i < 5; i++) {
		log << contentstr[i];
		if (i != 4) log << '.';
		else log << '\n';
	}
	log.close();

	// reload
	Urun::Reload(urun, wurun, exsists, lira, kurus, gun, ay, yil, ozet, aylik);
}

namespace Create {

	HWND Window(LPCWSTR className, LPCWSTR windowName, int x, int y, int width, int height, HWND hwnd, HMENU hmenu);

	HWND ChildWindow(LPCWSTR windowName, int x, int y, int width, int height, HWND hwnd, HMENU hmenu, HWND parent);
	HWND ChildCombo(LPCWSTR windowName, int x, int y, int width, int height, HWND hwnd, HMENU hmenu, HWND parent);
	HWND ChildEdit(LPCWSTR windowName, int x, int y, int width, int height, HWND hwnd, HMENU hmenu, HWND parent);
	HWND ChildButton(LPCWSTR windowName, int x, int y, int width, int height, HWND hwnd, HMENU hmenu, HWND parent);
	HWND ChildList(LPCWSTR windowName, int x, int y, int width, int height, HWND hwnd, HMENU hmenu, HWND parent);
}
HWND Create::Window(LPCWSTR className, LPCWSTR windowName, int x, int y, int width, int height, HWND hwnd, HMENU hmenu) {
	return CreateWindow(className, windowName, WS_BORDER | WS_VISIBLE | WS_CHILD, x, y, width, height, hwnd, hmenu, 0, 0);
}
HWND Create::ChildWindow(LPCWSTR windowName, int x, int y, int width, int height, HWND hwnd, HMENU hmenu, HWND parent) {
	RECT rectHwnd, rectParent;
	GetWindowRect(parent, &rectParent);
	GetWindowRect(hwnd, &rectHwnd);
	return CreateWindow(L"STATIC", windowName, WS_VISIBLE | WS_CHILD, x + rectParent.left - rectHwnd.left, y + rectParent.top - rectHwnd.top, width, height, hwnd, hmenu, 0, 0);
}
HWND Create::ChildCombo(LPCWSTR windowName, int x, int y, int width, int height, HWND hwnd, HMENU hmenu, HWND parent) {
	RECT rectHwnd, rectParent;
	GetWindowRect(parent, &rectParent);
	GetWindowRect(hwnd, &rectHwnd);
	return CreateWindowEx(0, L"COMBOBOX", windowName, WS_VSCROLL | CBS_AUTOHSCROLL | CBS_SORT | CBS_SIMPLE | CBS_HASSTRINGS | WS_VISIBLE | WS_CHILD, x + rectParent.left - rectHwnd.left, y + rectParent.top - rectHwnd.top, width, height, hwnd, hmenu, 0, 0);
}
HWND Create::ChildEdit(LPCWSTR windowName, int x, int y, int width, int height, HWND hwnd, HMENU hmenu, HWND parent) {
	RECT rectHwnd, rectParent;
	GetWindowRect(parent, &rectParent);
	GetWindowRect(hwnd, &rectHwnd);
	return CreateWindow(L"EDIT", windowName, ES_NUMBER | ES_RIGHT | WS_VISIBLE | WS_CHILD, x + rectParent.left - rectHwnd.left, y + rectParent.top - rectHwnd.top, width, height, hwnd, hmenu, 0, 0);
}
HWND Create::ChildButton(LPCWSTR windowName, int x, int y, int width, int height, HWND hwnd, HMENU hmenu, HWND parent) {
	RECT rectHwnd, rectParent;
	GetWindowRect(parent, &rectParent);
	GetWindowRect(hwnd, &rectHwnd);
	return CreateWindow(L"BUTTON", windowName, WS_VISIBLE | WS_CHILD, x + rectParent.left - rectHwnd.left, y + rectParent.top - rectHwnd.top, width, height, hwnd, hmenu, 0, 0);
}
HWND Create::ChildList(LPCWSTR windowName, int x, int y, int width, int height, HWND hwnd, HMENU hmenu, HWND parent) {
	RECT rectHwnd, rectParent;
	GetWindowRect(parent, &rectParent);
	GetWindowRect(hwnd, &rectHwnd);
	return CreateWindow(L"LISTBOX", windowName, LBS_HASSTRINGS | WS_VSCROLL | WS_HSCROLL | WS_BORDER | WS_VISIBLE | WS_CHILD, x + rectParent.left - rectHwnd.left, y + rectParent.top - rectHwnd.top, width, height, hwnd, hmenu, 0, 0);
}