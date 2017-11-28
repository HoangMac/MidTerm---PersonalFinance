#pragma once

#include "resource.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <locale>
#include <codecvt>
#include <string>
#include <vector>
#include <windowsx.h>
#include <commctrl.h>
#include <objidl.h>
#include <gdiplus.h>
#include <atlconv.h>

#pragma comment (lib,"Gdiplus.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "ComCtl32.lib")

using namespace Gdiplus;
using namespace std;

#define MAX_LOADSTRING 100

class ChiTieu
{
private:
	int LoaiChiTieu;
	int SoTien;
	wstring NoiDung;
public:
	static int TongTien;
	static vector<int> TongTienMoiLoai;

	int GetLoaiChiTieu() { return LoaiChiTieu; }
	int GetSoTien() { return SoTien; }
	wstring GetNoiDung() { return NoiDung; }

	void SetChiTieu(int loaict, long sotien, wstring noidung)
	{
		LoaiChiTieu = loaict;
		SoTien = sotien;
		NoiDung = noidung;
		TongTien += SoTien;
		TongTienMoiLoai[loaict] += sotien;
	}
};

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

HWND hButton_Them;
HWND hTreeView;
HWND hListView;
GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR           gdiplusToken;

#define AN_UONG 0
#define DI_CHUYEN 1
#define NHA_CUA 2
#define XE_CO 3
#define NHU_YEU_PHAM 4
#define DICH_VU 5

BOOL Started = FALSE, Chart = FALSE;
vector<TCHAR*> DanhSachLoaiCT = { _T("Ăn Uống"), _T("Di Chuyển"), _T("Nhà Cửa"), _T("Xe Cộ"), _T("Nhu Yếu Phẩm"), _T("Dịch Vụ") };
typedef vector< vector<ChiTieu*> > CTList;
extern CTList Database;
HTREEITEM hTreeItemDanhSach;
HTREEITEM hTreeItemAnUong;
HTREEITEM hTreeItemDiChuyen;
HTREEITEM hTreeItemNhaCua;
HTREEITEM hTreeItemXeCo;
HTREEITEM hTreeItemNhuYeuPham;
HTREEITEM hTreeItemDichVu;

wstring Int2Wstring(int value);
int Wstring2Int(wstring ws);
void ThemChiTieu(CTList &Database, ChiTieu *chitieu);
void ReadFile(CTList &Database);
void SaveFile(CTList Database);
void TreeView_Init(HWND hwnd, INITCOMMONCONTROLSEX icc, HIMAGELIST *hLarge, RECT WinRect);
void ListView_Init(HWND hwnd, INITCOMMONCONTROLSEX icc, HIMAGELIST *hLarge, RECT WinRect);
void LoadIcons(HIMAGELIST *hLarge);
void ListView_LoadDanhSach();
void ListView_LoadLoaiChiTieu(CTList Database, int Loai, TCHAR *TenLoai);
void ListView_LoadTatCa(CTList Database);
void ListView_LoadAnUong(CTList Database);
void ListView_LoadDiChuyen(CTList Database);
void ListView_LoadNhaCua(CTList Database);
void ListView_LoadXeCo(CTList Database);
void ListView_LoadNhuYeuPham(CTList Database);
void ListView_LoadDichVu(CTList Database);

BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void OnSize(HWND hwnd, UINT state, int cx, int cy);
LRESULT OnNotify(HWND hwnd, int idFrom, NMHDR *pnm);
void OnPaint(HWND hwnd);
void OnDestroy(HWND hwnd);



