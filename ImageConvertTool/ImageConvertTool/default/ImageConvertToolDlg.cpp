
// ImageConvertToolDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "ImageConvertTool.h"
#include "ImageConvertToolDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <filesystem> 
#include <string.h>
#include <d3d11.h>

#include "DirectXTex.h"
#include "DDSTextureLoader.h"
#include "WICTextureLoader.h"

using namespace std;

using namespace filesystem;

using namespace DirectX;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CImageConvertToolDlg 대화 상자



CImageConvertToolDlg::CImageConvertToolDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IMAGECONVERTTOOL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImageConvertToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CImageConvertToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_PATH, &CImageConvertToolDlg::OnBnClickedButtonPath)
	ON_BN_CLICKED(IDC_BUTTON_CONVERT, &CImageConvertToolDlg::OnBnClickedButtonConvert)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CImageConvertToolDlg::OnBnClickedButtonClose)
END_MESSAGE_MAP()


// CImageConvertToolDlg 메시지 처리기

BOOL CImageConvertToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CImageConvertToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CImageConvertToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CImageConvertToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CImageConvertToolDlg::OnBnClickedButtonPath()
{
	// 초기 선택 폴더
	CString strInitPath = _T("C:\\");

	// 폴더 선택 다이얼로그
	CFolderPickerDialog Picker(strInitPath, OFN_FILEMUSTEXIST, NULL, 0);
	if (Picker.DoModal() == IDOK)
	{
		// 선택된 폴더 경로얻음
		CString strFolderPath = Picker.GetPathName();

		SetDlgItemText(IDC_PATH, strFolderPath);
		// 경로(strFolderPath)를 이용하여 이후작업 추가
	}
}

void CImageConvertToolDlg::OnBnClickedButtonConvert()
{
	CString strFolderPath;
	GetDlgItemText(IDC_PATH, strFolderPath);
	path FolderPath = string(CT2CA(strFolderPath));

	int SuccessCount = 0;
	int FailCount = 0;
	int MaxCount = 0;

	if (true == AllocConsole()) 
	{
		FILE* nfp[3];
		freopen_s(nfp + 0, "CONOUT$", "rb", stdin);
		freopen_s(nfp + 1, "CONOUT$", "wb", stdout);
		freopen_s(nfp + 2, "CONOUT$", "wb", stderr);
	}

	for (auto& entry : recursive_directory_iterator(FolderPath))
	{
		char path[MAX_PATH] = "";
		strcpy_s(path, entry.path().string().c_str());
		char drive[MAX_PATH] = "";
		char dir[MAX_PATH] = "";
		char fileName[MAX_PATH] = "";
		char ext[MAX_PATH] = "";
		char ImagePath[MAX_PATH] = "";

		TCHAR tempPath[MAX_PATH] = L"";
		TCHAR savePath[MAX_PATH] = L"";

		HRESULT hr = 0;
		ScratchImage scrImage;
		ScratchImage mipChain;
		ScratchImage compChain;
		ScratchImage decompChain;

		_splitpath_s(path, drive, MAX_PATH, dir, MAX_PATH, fileName, MAX_PATH, ext, MAX_PATH);

		if (0 == strcmp(ext, ".tga") || 0 == strcmp(ext, ".TGA"))
		{
			

			strcat_s(ImagePath, drive);
			strcat_s(ImagePath, dir);
			strcat_s(ImagePath, fileName);
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, ImagePath, strlen(ImagePath), savePath, 256);
			strcat_s(ImagePath, ext);
			printf(ImagePath);
			printf("\n");

			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, ImagePath, strlen(ImagePath), tempPath, 256);

			hr = DirectX::LoadFromTGAFile(tempPath, TGA_FLAGS_NONE, nullptr, scrImage);

			if (scrImage.GetMetadata().format == DXGI_FORMAT_R8G8B8A8_UNORM)
			{
				++MaxCount;

				StrCat(savePath, L".dds");

				hr == GenerateMipMaps(scrImage.GetImages(), scrImage.GetImageCount(), scrImage.GetMetadata(), TEX_FILTER_DEFAULT, 0, mipChain);
				if (hr == S_OK)
				{
					printf("Success GenerateMipMaps Image");
					printf("\n");
				}
				else
				{
					printf("Fail GenerateMipMaps Image");
					printf("\n");
				}


				hr = Compress(mipChain.GetImages(), mipChain.GetImageCount(),
					mipChain.GetMetadata(), DXGI_FORMAT_BC3_UNORM,
					TEX_COMPRESS_DEFAULT, TEX_THRESHOLD_DEFAULT,
					compChain);
				if (hr == S_OK)
				{
					printf("Success Compress Image");
					printf("\n");
				}
				else
				{
					printf("Fail Compress Image");
					printf("\n");
				}

				hr = SaveToDDSFile(compChain.GetImages(), compChain.GetImageCount(), compChain.GetMetadata(), DDS_FLAGS_NONE, savePath);

				if (hr == S_OK)
				{
					printf("Success Save Image");
					printf("\n");
					++SuccessCount;
				}
				else
				{
					printf("Fail Save Image");
					printf("\n");
					++FailCount;
				}
			}

			
		}
		else if (0 == strcmp(ext, ".png") || 0 == strcmp(ext, ".PNG"))
		{
			

			strcat_s(ImagePath, drive);
			strcat_s(ImagePath, dir);
			strcat_s(ImagePath, fileName);
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, ImagePath, strlen(ImagePath), savePath, 256);
			strcat_s(ImagePath, ext);
			printf(ImagePath);
			printf("\n");

			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, ImagePath, strlen(ImagePath), tempPath, 256);

			hr = DirectX::LoadFromWICFile(tempPath, WIC_FLAGS_NONE, nullptr, scrImage);

			if (scrImage.GetMetadata().format == DXGI_FORMAT_B8G8R8A8_UNORM)
			{
				++MaxCount;

				StrCat(savePath, L".dds");

				hr == GenerateMipMaps(scrImage.GetImages(), scrImage.GetImageCount(), scrImage.GetMetadata(), TEX_FILTER_DEFAULT, 0, mipChain);
				if (hr == S_OK)
				{
					printf("Success GenerateMipMaps Image");
					printf("\n");
				}
				else
				{
					printf("Fail GenerateMipMaps Image");
					printf("\n");
				}


				hr = Compress(mipChain.GetImages(), mipChain.GetImageCount(),
					mipChain.GetMetadata(), DXGI_FORMAT_BC3_UNORM,
					TEX_COMPRESS_DEFAULT, TEX_THRESHOLD_DEFAULT,
					compChain);
				if (hr == S_OK)
				{
					printf("Success Compress Image");
					printf("\n");
				}
				else
				{
					printf("Fail Compress Image");
					printf("\n");
				}

				hr = SaveToDDSFile(compChain.GetImages(), compChain.GetImageCount(), compChain.GetMetadata(), DDS_FLAGS_NONE, savePath);

				if (hr == S_OK)
				{
					printf("Success Save Image");
					printf("\n");
					++SuccessCount;
				}
				else
				{
					printf("Fail Save Image");
					printf("\n");
					++FailCount;
				}
			}
		}
		else if (0 == strcmp(ext, ".dds") || 0 == strcmp(ext, ".DDS"))
		{
			

			strcat_s(ImagePath, drive);
			strcat_s(ImagePath, dir);
			strcat_s(ImagePath, fileName);
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, ImagePath, strlen(ImagePath), savePath, 256);
			strcat_s(ImagePath, ext);
			printf(ImagePath);
			printf("\n");

			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, ImagePath, strlen(ImagePath), tempPath, 256);

			hr = DirectX::LoadFromDDSFile(tempPath, DDS_FLAGS_NONE, nullptr, scrImage);

			if (scrImage.GetMetadata().format == DXGI_FORMAT_BC5_UNORM)
			{
				++MaxCount;

				StrCat(savePath, L".dds");

				
				
				hr = Decompress(scrImage.GetImages(), scrImage.GetImageCount(), scrImage.GetMetadata(), DXGI_FORMAT_UNKNOWN, decompChain);

				hr == GenerateMipMaps(decompChain.GetImages(), decompChain.GetImageCount(), decompChain.GetMetadata(), TEX_FILTER_DEFAULT, 0, mipChain);
				if (hr == S_OK)
				{
					printf("Success GenerateMipMaps Image");
					printf("\n");
				}
				else
				{
					printf("Fail GenerateMipMaps Image");
					printf("\n");
				}

				hr = Compress(mipChain.GetImages(), mipChain.GetImageCount(),
					mipChain.GetMetadata(), DXGI_FORMAT_BC1_UNORM,
					TEX_COMPRESS_DEFAULT, TEX_THRESHOLD_DEFAULT,
					compChain);
				if (hr == S_OK)
				{
					printf("Success Compress Image");
					printf("\n");
				}
				else
				{
					printf("Fail Compress Image");
					printf("\n");
				}

				hr = SaveToDDSFile(compChain.GetImages(), compChain.GetImageCount(), compChain.GetMetadata(), DDS_FLAGS_NONE, savePath);

				if (hr == S_OK)
				{
					printf("Success Save Image");
					printf("\n");
					++SuccessCount;
				}
				else
				{
					printf("Fail Save Image");
					printf("\n");
					++FailCount;
				}
			}
		}


	}

	CString strTemp = L"Success Count ";

	CString strSuccessCount; 
	strSuccessCount.Format(_T("%d"), SuccessCount);

	CString strMaxCount;
	strMaxCount.Format(_T("%d"), MaxCount);

	strTemp = strTemp + L" ( " + strSuccessCount + L" / " + strMaxCount + L" ) ";
	AfxMessageBox((TCHAR*)(LPCTSTR)strTemp, MB_OK | MB_ICONQUESTION);
}


void CImageConvertToolDlg::OnBnClickedButtonClose()
{
	if (!FreeConsole()) { AfxMessageBox(_T("Failed to free the console!"), MB_ICONEXCLAMATION); }
	PostQuitMessage(0);
}
