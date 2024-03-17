#include "functions.h"

HWND urun,	// urun listesi urunler ust
	lira,	// lira girilen yer urunler orta
	kurus,	// kurus girilen yer urunler orta
	gun,	// gun girilen yer urunler alt
	ay,		// ay girilen yer urunler alt
	yil,	// yil girilen yer urunler alt
	ozet,	// ozet listesi ozetler ust
	aylik;	// aylik harcama ozetler alt

void SetWindows(HWND hwnd) {
	
	HWND urunler = Create::Window(L"STATIC", L"Ürünler", 10, 20, 225, 270, hwnd, URUN);
	HWND ozetler = Create::Window(L"STATIC", L"Özet", 245, 20, 237, 270, hwnd, OZET);
	// urunler
	urun = Create::ChildCombo(L"dokuz", 10, 10, 100, 100, hwnd, URUN_URUN, urunler);
	Create::ChildButton(L"Ürün Sil", 120, 10, 80, 20, hwnd, URUN_SIL, urunler);
	Create::ChildButton(L"Düzenle", 120, 32, 80, 20, hwnd, URUN_EDIT, urunler);

	lira = Create::ChildEdit(L"0", 10, 120, 80, 16, hwnd, URUN_LIRA, urunler);
	Create::ChildWindow(L",", 99, 120, 5, 16, hwnd, 0, urunler);
	kurus = Create::ChildEdit(L"00", 110, 120, 16, 16, hwnd, URUN_KURUS, urunler);
	Create::ChildWindow(L"TL", 134, 120, 20, 16, hwnd, 0, urunler);

	gun = Create::ChildEdit(L"00", 10, 170, 16, 16, hwnd, URUN_GUN, urunler);
	Create::ChildWindow(L".", 30, 170, 5, 16, hwnd, 0, urunler);
	ay = Create::ChildEdit(L"00", 35, 170, 16, 16, hwnd, URUN_AY, urunler);
	Create::ChildWindow(L".", 55, 170, 5, 16, hwnd, 0, urunler);
	yil = Create::ChildEdit(L"0000", 60, 170, 35, 16, hwnd, URUN_YIL, urunler);
	Urun::LoadDate(gun, ay, yil);

	Create::ChildButton(L"Tamam", 120, 168, 80, 20, hwnd, URUN_TAMAM, urunler);
	Urun::Load(urun);
	// ozet
	ozet = Create::ChildList(L"Liste", 10, 10, 200, 200, hwnd, OZET_LISTE, ozetler);
	aylik = Create::ChildList(0, 10, 210, 200, 20, hwnd, 0, ozetler);
	Log::Load(ozet, aylik);
}