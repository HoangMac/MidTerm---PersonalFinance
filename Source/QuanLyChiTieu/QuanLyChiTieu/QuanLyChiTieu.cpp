// QuanLyChiTieu.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "QuanLyChiTieu.h"

CTList Database(6);
int ChiTieu::TongTien = 0;
vector<int> ChiTieu::TongTienMoiLoai{0, 0, 0, 0, 0, 0};


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    DialogProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    DialogChartProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_QUANLYCHITIEU, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_QUANLYCHITIEU));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_QUANLYCHITIEU));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_QUANLYCHITIEU);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hWnd, WM_SIZE, OnSize);
		HANDLE_MSG(hWnd, WM_NOTIFY, OnNotify);
		HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

wstring Int2Wstring(int value)
{
	wostringstream	wss;
	wss << value;
	return wss.str();
}

int Wstring2Int(wstring ws)
{
	int value = 0;
	for (int i = 0; i < ws.length(); i++)
	{
		value *= 10;
		value += ws[i] - '0';
	}
	return value;
}

void ThemChiTieu(CTList &Database, ChiTieu *chitieu)
{
	switch (chitieu->GetLoaiChiTieu())
	{
	case AN_UONG:
		Database[AN_UONG].push_back(chitieu);
		break;
	case DI_CHUYEN:
		Database[DI_CHUYEN].push_back(chitieu);
		break;
	case NHA_CUA:
		Database[NHA_CUA].push_back(chitieu);
		break;
	case XE_CO:
		Database[XE_CO].push_back(chitieu);
		break;
	case NHU_YEU_PHAM:
		Database[NHU_YEU_PHAM].push_back(chitieu);
		break;
	case DICH_VU:
		Database[DICH_VU].push_back(chitieu);
		break;
	}
}

void ReadFile(CTList &Database)
{
	ChiTieu *chitieu;
	int loaichitieu, sotien, i;
	wstring loai, noidung, st;

	wifstream fin("ChiTieu.txt", ios::binary);
	fin.imbue(locale(fin.getloc(), new codecvt_utf16<wchar_t, 0x10ffff, consume_header>));
	if (!fin.is_open())
	{
		MessageBox(0, _T("Lỗi! Không thể load dữ liệu!"), 0, 0);
		return;
	}
	
	wstring buffer;
	while (getline(fin, buffer))
	{
		loai = buffer[0];
		loaichitieu = Wstring2Int(loai);
		noidung = st = L"";
		i = 2;
		while (buffer[i] != L',')
		{
			noidung += buffer[i];
			i++;
		}
		i++;
		while (buffer[i] != L'.')
		{
			st += buffer[i];
			i++;
		}
		sotien = Wstring2Int(st);
		chitieu = new ChiTieu;
		chitieu->SetChiTieu(loaichitieu, sotien, noidung);
		ThemChiTieu(Database, chitieu);
	}
	fin.close();
}

void SaveFile(CTList Database)
{
	//Dạng: <loại chi tiêu>,<nội dung>,<số tiền>.\n
	wofstream fout("ChiTieu.txt", ios::binary);
	fout.imbue(locale(fout.getloc(), new codecvt_utf16<wchar_t, 0x10ffff, consume_header>));
	for (int i = 0; i < 6; i++)
	{
		int len = Database[i].size();
		for (int j = 0; j < len; j++)
		{
			wstring loaichitieu = Int2Wstring(i);
			wstring noidung = Database[i][j]->GetNoiDung();
			wstring sotien = Int2Wstring(Database[i][j]->GetSoTien());
			wstring wstr;
			wstr += loaichitieu + L',' + noidung + L',' + sotien + L".\n";

			fout << wstr;
		}
	}
	fout.close();
	for (int i = 0; i < 6; i++)
	{
		int len = Database[i].size();
		for (int j = 0; j < len; j++)
			delete Database[i][j];
		Database[i].empty();
	}
}

void LoadIcons(HIMAGELIST *hLarge)
{
	HICON hIcon;
	*hLarge = ImageList_Create(32, 32, ILC_COLOR32 | ILC_MASK, 7, 10);
	/* Index = 0 */
	hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_DANHSACH));
	ImageList_AddIcon(*hLarge, hIcon);
	DestroyIcon(hIcon);
	/* Index = 1 */
	hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_TATCA));
	ImageList_AddIcon(*hLarge, hIcon);
	DestroyIcon(hIcon);
	/* Index = 2 */
	hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ANUONG));
	ImageList_AddIcon(*hLarge, hIcon);
	DestroyIcon(hIcon);
	/* Index = 3 */
	hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_DICHUYEN));
	ImageList_AddIcon(*hLarge, hIcon);
	DestroyIcon(hIcon);
	/* Index = 4 */
	hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_NHACUA));
	ImageList_AddIcon(*hLarge, hIcon);
	DestroyIcon(hIcon);
	/* Index = 5 */
	hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_XECO));
	ImageList_AddIcon(*hLarge, hIcon);
	DestroyIcon(hIcon);
	/* Index = 6 */
	hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_NHUYEUPHAM));
	ImageList_AddIcon(*hLarge, hIcon);
	DestroyIcon(hIcon);
	/* Index = 7 */
	hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_DICHVU));
	ImageList_AddIcon(*hLarge, hIcon);
	DestroyIcon(hIcon);
	/* Index = 8 */
	hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_BIEUDO));
	ImageList_AddIcon(*hLarge, hIcon);
	DestroyIcon(hIcon);
}

void TreeView_Init(HWND hwnd, INITCOMMONCONTROLSEX icc, HIMAGELIST *hLarge, RECT WinRect)
{
	hTreeView = CreateWindowEx(0, WC_TREEVIEW, _T("Tree View"),
		WS_CHILD | WS_VISIBLE | WS_BORDER | WS_SIZEBOX | WS_VSCROLL | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT,
		0, 0, WinRect.right / 3, WinRect.bottom - 40,
		hwnd, (HMENU)ID_TREEVIEW, hInst, NULL);

	TreeView_SetImageList(hTreeView, *hLarge, TVSIL_NORMAL);

	TV_INSERTSTRUCT tvins;

	tvins.hParent = NULL;
	tvins.hInsertAfter = TVI_ROOT;
	tvins.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvins.item.pszText = _T("DANH SÁCH");
	tvins.item.lParam = ID_TV_DANHSACH;
	tvins.item.iImage = 0;
	tvins.item.iSelectedImage = 0;
	hTreeItemDanhSach = TreeView_InsertItem(hTreeView, &tvins);

	tvins.hParent = hTreeItemDanhSach;
	tvins.hInsertAfter = TVI_LAST;
	tvins.item.pszText = _T("TẤT CẢ");
	tvins.item.lParam = ID_TV_TATCA;
	tvins.item.iImage = 1;
	tvins.item.iSelectedImage = 1;
	TreeView_InsertItem(hTreeView, &tvins);

	tvins.hParent = hTreeItemDanhSach;
	tvins.hInsertAfter = TVI_LAST;
	tvins.item.pszText = _T("ĂN UỐNG");
	tvins.item.lParam = ID_TV_ANUONG;
	tvins.item.iImage = 2;
	tvins.item.iSelectedImage = 2;
	hTreeItemAnUong = TreeView_InsertItem(hTreeView, &tvins);

	tvins.hParent = hTreeItemDanhSach;
	tvins.hInsertAfter = TVI_LAST;
	tvins.item.pszText = _T("DI CHUYỂN");
	tvins.item.lParam = ID_TV_DICHUYEN;
	tvins.item.iImage = 3;
	tvins.item.iSelectedImage = 3;
	hTreeItemDiChuyen = TreeView_InsertItem(hTreeView, &tvins);

	tvins.hParent = hTreeItemDanhSach;
	tvins.hInsertAfter = TVI_LAST;
	tvins.item.pszText = _T("NHÀ CỬA");
	tvins.item.lParam = ID_TV_NHACUA;
	tvins.item.iImage = 4;
	tvins.item.iSelectedImage = 4;
	hTreeItemNhaCua = TreeView_InsertItem(hTreeView, &tvins);

	tvins.hParent = hTreeItemDanhSach;
	tvins.hInsertAfter = TVI_LAST;
	tvins.item.pszText = _T("XE CỘ");
	tvins.item.lParam = ID_TV_XECO;
	tvins.item.iImage = 5;
	tvins.item.iSelectedImage = 5;
	hTreeItemXeCo = TreeView_InsertItem(hTreeView, &tvins);

	tvins.hParent = hTreeItemDanhSach;
	tvins.hInsertAfter = TVI_LAST;
	tvins.item.pszText = _T("NHU YẾU PHẨM");
	tvins.item.lParam = ID_TV_NHUYEUPHAM;
	tvins.item.iImage = 6;
	tvins.item.iSelectedImage = 6;
	hTreeItemNhuYeuPham = TreeView_InsertItem(hTreeView, &tvins);

	tvins.hParent = hTreeItemDanhSach;
	tvins.hInsertAfter = TVI_LAST;
	tvins.item.pszText = _T("DỊCH VỤ");
	tvins.item.lParam = ID_TV_DICHVU;
	tvins.item.iImage = 7;
	tvins.item.iSelectedImage = 7;
	hTreeItemDichVu = TreeView_InsertItem(hTreeView, &tvins);

	tvins.hParent = NULL;
	tvins.hInsertAfter = TVI_ROOT;
	tvins.item.pszText = _T("BIỂU ĐỒ");
	tvins.item.lParam = ID_TV_BIEUDO;
	tvins.item.iImage = 8;
	tvins.item.iSelectedImage = 8;
	TreeView_InsertItem(hTreeView, &tvins);
}

void ListView_Init(HWND hwnd, INITCOMMONCONTROLSEX icc, HIMAGELIST *hLarge, RECT WinRect)
{
	hListView = CreateWindowEx(0, WC_LISTVIEW, _T("List View"),
		WS_CHILD | WS_VISIBLE | WS_BORDER | WS_SIZEBOX | WS_VSCROLL | LVS_REPORT,
		WinRect.right / 3, 0, WinRect.right * 2 / 3, WinRect.bottom - 40,
		hwnd, (HMENU)ID_TREEVIEW, hInst, NULL);

	LVCOLUMN lvCol1;
	lvCol1.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
	lvCol1.fmt = LVCFMT_LEFT;
	lvCol1.cx = WinRect.right / 6;
	lvCol1.pszText = _T("Loại chi tiêu");
	ListView_InsertColumn(hListView, 0, &lvCol1);

	LVCOLUMN lvCol2;
	lvCol2.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
	lvCol2.fmt = LVCFMT_LEFT;
	lvCol2.cx = WinRect.right / 3;
	lvCol2.pszText = _T("Nội dung");
	ListView_InsertColumn(hListView, 1, &lvCol2);

	LVCOLUMN lvCol3;
	lvCol3.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
	lvCol3.fmt = LVCFMT_LEFT;
	lvCol3.cx = WinRect.right / 6;
	lvCol3.pszText = _T("Số tiền");
	ListView_InsertColumn(hListView, 2, &lvCol3);

	ListView_SetImageList(hListView, *hLarge, LVSIL_NORMAL);
}

void ListView_LoadDanhSach()
{
	LV_ITEM lv;
	lv.mask = LVIF_TEXT | LVIF_IMAGE;;
	lv.iItem = 0;
	lv.iSubItem = 0;
	lv.pszText = DanhSachLoaiCT[AN_UONG];
	lv.iImage = 2;
	ListView_InsertItem(hListView, &lv);

	lv.iItem = 1;
	lv.iSubItem = 0;
	lv.pszText = DanhSachLoaiCT[DI_CHUYEN];
	lv.iImage = 3;
	ListView_InsertItem(hListView, &lv);

	lv.iItem = 2;
	lv.iSubItem = 0;
	lv.pszText = DanhSachLoaiCT[NHA_CUA];
	lv.iImage = 4;
	ListView_InsertItem(hListView, &lv);

	lv.iItem = 3;
	lv.iSubItem = 0;
	lv.pszText = DanhSachLoaiCT[XE_CO];
	lv.iImage = 5;
	ListView_InsertItem(hListView, &lv);

	lv.iItem = 4;
	lv.iSubItem = 0;
	lv.pszText = DanhSachLoaiCT[NHU_YEU_PHAM];
	lv.iImage = 6;
	ListView_InsertItem(hListView, &lv);

	lv.iItem = 5;
	lv.iSubItem = 0;
	lv.pszText = DanhSachLoaiCT[DICH_VU];
	lv.iImage = 7;
	ListView_InsertItem(hListView, &lv);
	SendMessage(hListView, LVM_SETVIEW, (WPARAM)LVS_ICON, 0);
}

void ListView_LoadLoaiChiTieu(CTList Database, int Loai, TCHAR *TenLoai)
{
	LV_ITEM lv;
	lv.mask = LVIF_TEXT;
	wstring tmp;
	TCHAR *text;
	int SoTien, j;
	for (int i = 0; i < Database[Loai].size(); i++)
	{
		lv.iItem = i;
		lv.iSubItem = 0;
		lv.pszText = TenLoai;
		ListView_InsertItem(hListView, &lv);

		lv.iSubItem = 1;
		tmp = Database[Loai][i]->GetNoiDung().c_str();
		text = new TCHAR[tmp.size()];
		j = 0;
		for (j; j < tmp.size(); j++)
			text[j] = tmp[j];
		text[j] = '\0';
		lv.pszText = text;
		ListView_SetItem(hListView, &lv);

		lv.iSubItem = 2;
		SoTien = Database[Loai][i]->GetSoTien();
		tmp = Int2Wstring(SoTien).c_str();
		text = new TCHAR[tmp.size()];
		j = 0;
		for (j; j < tmp.size(); j++)
			text[j] = tmp[j];
		text[j] = '\0';
		lv.pszText = text;
		ListView_SetItem(hListView, &lv);
	}
	SendMessage(hListView, LVM_SETVIEW, (WPARAM)LVS_REPORT, 0);
}

void ListView_LoadTatCa(CTList Database)
{
	for (int i = 5; i > -1; i--)
		ListView_LoadLoaiChiTieu(Database, i, DanhSachLoaiCT[i]);
}

void ListView_LoadAnUong(CTList Database)
{
	ListView_LoadLoaiChiTieu(Database, AN_UONG, DanhSachLoaiCT[AN_UONG]);
}

void ListView_LoadDiChuyen(CTList Database)
{
	ListView_LoadLoaiChiTieu(Database, DI_CHUYEN, DanhSachLoaiCT[DI_CHUYEN]);
}

void ListView_LoadNhaCua(CTList Database)
{
	ListView_LoadLoaiChiTieu(Database, NHA_CUA, DanhSachLoaiCT[NHA_CUA]);
}

void ListView_LoadXeCo(CTList Database)
{
	ListView_LoadLoaiChiTieu(Database, XE_CO, DanhSachLoaiCT[XE_CO]);
}

void ListView_LoadNhuYeuPham(CTList Database)
{
	ListView_LoadLoaiChiTieu(Database, NHU_YEU_PHAM, DanhSachLoaiCT[NHU_YEU_PHAM]);
}

void ListView_LoadDichVu(CTList Database)
{
	ListView_LoadLoaiChiTieu(Database, DICH_VU, DanhSachLoaiCT[DICH_VU]);
}

BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(icc);
	icc.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icc);

	// Lấy font hệ thống
	/*LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	HFONT TextFont = CreateFont(lf.lfHeight, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);*/

	RECT WinRect;
	GetClientRect(hwnd, &WinRect);
	
	HIMAGELIST *hLarge = new HIMAGELIST;
	LoadIcons(hLarge);

	TreeView_Init(hwnd, icc, hLarge, WinRect);

	ListView_Init(hwnd, icc, hLarge, WinRect);

	hButton_Them = CreateWindowEx(0, L"BUTTON", L"Thêm", 
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
		WinRect.right * 2 / 3 - 100, WinRect.bottom - 40, 100, 40, 
		hwnd, (HMENU)IDC_BUTTON_THEM, hInst, NULL);

	ReadFile(Database);
	Started = TRUE;
	return true;
}

void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	HTREEITEM NodeSelected;
	switch (id)
	{
	case IDM_ABOUT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, About);
		break;
	case IDM_EXIT:
		DestroyWindow(hwnd);
		break;
	case IDC_BUTTON_THEM:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, DialogProc);
		break;
	}
}

void OnSize(HWND hwnd, UINT state, int cx, int cy)
{
	RECT WinRect;
	GetClientRect(hwnd, &WinRect);

	MoveWindow(hTreeView, 0, 0, WinRect.right / 3, WinRect.bottom - 40, FALSE);
	MoveWindow(hListView, WinRect.right / 3, 0, WinRect.right * 2 / 3, WinRect.bottom - 40, FALSE);
	ListView_SetColumnWidth(hListView, 0, WinRect.right / 6);
	ListView_SetColumnWidth(hListView, 1, WinRect.right / 3);
	ListView_SetColumnWidth(hListView, 2, WinRect.right / 6);
	MoveWindow(hButton_Them, WinRect.right * 2 / 3 - 100, WinRect.bottom - 40, 100, 40, FALSE);
}

LRESULT OnNotify(HWND hwnd, int idFrom, NMHDR *pnm)
{
	
	int nCurSelIndex;
	LPNMTOOLBAR lpnmToolBar = (LPNMTOOLBAR)pnm;

	if (Started)
	{
		LPNMTREEVIEW lpnmTree = (LPNMTREEVIEW)pnm;

		switch (pnm->code)
		{
		case TVN_SELCHANGED:
		{
			switch (lpnmTree->itemNew.lParam)
			{
			case ID_TV_DANHSACH:
				TreeView_Expand(hTreeView, hTreeItemDanhSach, TVE_EXPAND);
				ListView_DeleteAllItems(hListView);
				ListView_LoadDanhSach();
				break;
			case ID_TV_TATCA:
				ListView_DeleteAllItems(hListView);
				ListView_LoadTatCa(Database);
				break;
			case ID_TV_ANUONG:
				ListView_DeleteAllItems(hListView);
				ListView_LoadAnUong(Database);
				break;
			case ID_TV_DICHUYEN:
				ListView_DeleteAllItems(hListView);
				ListView_LoadDiChuyen(Database);
				break;
			case ID_TV_NHACUA:
				ListView_DeleteAllItems(hListView);
				ListView_LoadNhaCua(Database);
				break;
			case ID_TV_XECO:
				ListView_DeleteAllItems(hListView);
				ListView_LoadXeCo(Database);
				break;
			case ID_TV_NHUYEUPHAM:
				ListView_DeleteAllItems(hListView);
				ListView_LoadNhuYeuPham(Database);
				break;
			case ID_TV_DICHVU:
				ListView_DeleteAllItems(hListView);
				ListView_LoadDichVu(Database);
				break;
			case ID_TV_BIEUDO:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hwnd, DialogChartProc);
				break;
			}
		}
		break;
		case NM_DBLCLK:
			if (pnm->hwndFrom == hListView)
			{
				nCurSelIndex = ListView_GetNextItem(hListView, -1, LVNI_FOCUSED);
				if (nCurSelIndex != -1)
				{
					switch (nCurSelIndex)
					{
					case 0:
						TreeView_SelectItem(hTreeView, hTreeItemAnUong);
						break;
					case 1:
						TreeView_SelectItem(hTreeView, hTreeItemDiChuyen);
						break;
					case 2:
						TreeView_SelectItem(hTreeView, hTreeItemNhaCua);
						break;
					case 3:
						TreeView_SelectItem(hTreeView, hTreeItemXeCo);
						break;
					case 4:
						TreeView_SelectItem(hTreeView, hTreeItemNhuYeuPham);
						break;
					case 5:
						TreeView_SelectItem(hTreeView, hTreeItemDichVu);
						break;
					}
				}
			}
			break;
		}
	}

	return 0;
}

void OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc;
	hdc = BeginPaint(hWnd, &ps);

	RECT WinRect;
	GetClientRect(hWnd, &WinRect);

	Graphics *graph = new Graphics(hdc);
	int LineWidth = 3;
	SolidBrush Sbrush(Color::DarkRed);

	FontFamily  fontFamily(L"Segoe UI");
	Font        font(&fontFamily, 20, FontStyleBold, UnitPixel);

	graph->DrawString(L"MẠC MINH HOÀNG - 1512176", -1, &font, PointF(0, WinRect.bottom - 40), &Sbrush);
	
	delete graph;
	EndPaint(hWnd, &ps);
}

void OnDestroy(HWND hwnd)
{
	GdiplusShutdown(gdiplusToken);
	SaveFile(Database);
	PostQuitMessage(0);
}

INT_PTR CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	HWND hComboBox;
	HWND hEdit;
	HWND hStatic;
	ChiTieu *chitieu;
	wchar_t *buffer;
	
	int sotien, size, nCursel;
		
	switch (message)
	{
	case WM_INITDIALOG:
		hComboBox = CreateWindowEx(0, L"ComboBox", L"",
			WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, 
			130, 50, 120, 100, hDlg, (HMENU)IDC_CB_LOAICT, hInst, NULL);
		for (int i = 0; i < DanhSachLoaiCT.size(); i++)
			ComboBox_AddString(hComboBox, DanhSachLoaiCT[i]);
		hStatic = GetDlgItem(hDlg, IDC_STATIC_LCT);
		Static_SetText(hStatic, L"Loại Chi Tiêu");

		hStatic = GetDlgItem(hDlg, IDC_STATIC_ND);
		Static_SetText(hStatic, L"Nội Dung");

		hStatic = GetDlgItem(hDlg, IDC_STATIC_ST);
		Static_SetText(hStatic, L"Số Tiền");

		hEdit = GetDlgItem(hDlg, IDC_EDIT4);
		SetWindowLong(hEdit, GWL_STYLE, WS_CHILD | WS_VISIBLE | ES_NUMBER);

		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			{
				hComboBox = GetDlgItem(hDlg, IDC_CB_LOAICT);
				nCursel = ComboBox_GetCurSel(hComboBox);
				if (nCursel == -1)
				{
					MessageBox(hDlg, L"Bạn phải chọn loại chi tiêu!", 0, MB_ICONERROR);
					break;
				}
					
				hEdit = GetDlgItem(hDlg, IDC_EDIT3);
				size = GetWindowTextLength(hEdit);
				buffer = new wchar_t[size + 1];
				GetWindowText(hEdit, buffer, size + 1);
				wstring noidung(buffer);
				if (noidung == L"")
				{
					MessageBox(hDlg, L"Bạn không được để trống!", 0, MB_ICONERROR);
					break;
				}

				hEdit = GetDlgItem(hDlg, IDC_EDIT4);
				size = GetWindowTextLength(hEdit);
				buffer = new wchar_t[size + 1];
				GetWindowText(hEdit, buffer, size + 1);
				wstring st(buffer);
				sotien = _wtoi(buffer);
				if (st == L"")
				{
					MessageBox(hDlg, L"Bạn không được để trống!", 0, MB_ICONERROR);
					break;
				}
				if (sotien < 0)
				{
					MessageBox(hDlg, L"Số tiền không thể là số âm!", 0, MB_ICONERROR);
					break;
				}

				chitieu = new ChiTieu;
				chitieu->SetChiTieu(nCursel, sotien, noidung);
				ThemChiTieu(Database, chitieu);



				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
			}
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK DialogChartProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	HWND hwnd = GetParent(hDlg);
	RECT WinRect;
	GetClientRect(hwnd, &WinRect);

	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc;
			hdc = BeginPaint(hDlg, &ps);

			vector<Color> ListColor;
			ListColor.push_back(Color::Blue);
			ListColor.push_back(Color::Red);
			ListColor.push_back(Color::Purple);
			ListColor.push_back(Color::Green);
			ListColor.push_back(Color::Yellow);
			ListColor.push_back(Color::Aquamarine);
			RECT WinRect;
			GetClientRect(hDlg, &WinRect);
			REAL average, sweepAngle, startAngle = -90.f;

			Graphics *graph = new Graphics(hdc);
			SolidBrush Sbrush(Color::Black);

			FontFamily  fontFamily(L"Segoe UI");
			Font        font(&fontFamily, 20, FontStyleBold, UnitPixel);

			RectF PieRect(100, 150, 300, 300);
			RectF ArcRect(400, 50, 100, 100);
			PointF textpoint(520, 70);
				
			for (int i = 0; i < 6; i++)
			{
				average = (REAL)ChiTieu::TongTienMoiLoai[i] / (REAL)ChiTieu::TongTien;
				sweepAngle = average * 360;
				Sbrush.SetColor(ListColor[i]);
				graph->FillPie(&Sbrush, PieRect, startAngle, sweepAngle);
				startAngle += sweepAngle;
				graph->FillPie(&Sbrush, ArcRect, -40.f, 40.f);
				Sbrush.SetColor(Color::Black);
				graph->DrawString(DanhSachLoaiCT[i], -1, &font, textpoint, &Sbrush);
				ArcRect.Y += 100;
				textpoint.Y += 100;
			}
			
			delete graph;
			EndPaint(hDlg, &ps);
		}
	}
	return (INT_PTR)FALSE;
}
