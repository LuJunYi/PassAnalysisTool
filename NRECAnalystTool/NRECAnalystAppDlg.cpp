
// NRECAnalystAppDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "NRECAnalystApp.h"
#include "NRECAnalystAppDlg.h"
#include "afxdialogex.h"

#include "StringUtils.h"
#include "NRECToolPathHander.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CNRECAnalystAppDlg 对话框



CNRECAnalystAppDlg::CNRECAnalystAppDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_NRECANALYSTAPP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNRECAnalystAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CNRECAnalystAppDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_Select, &CNRECAnalystAppDlg::OnBnClickedButtonSelect)
	ON_BN_CLICKED(IDOK, &CNRECAnalystAppDlg::OnBnClickedOk)
//	ON_EN_CHANGE(IDC_EDIT_RADUS, &CNRECAnalystAppDlg::OnEnChangeEditRadus)
END_MESSAGE_MAP()


// CNRECAnalystAppDlg 消息处理程序

BOOL CNRECAnalystAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CNRECAnalystAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CNRECAnalystAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CNRECAnalystAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CNRECAnalystAppDlg::OnBnClickedButtonSelect()
{
	// TODO: 在此添加控件通知处理程序代码
	  // 设置过滤器   
	TCHAR szFilter[] = _T("所有文件(*.*)|*.*||");
	// 构造打开文件对话框   
	CFileDialog fileDlg(TRUE, _T("txt"), NULL, 0, szFilter, this);
	CString strFilePath;

	// 显示打开文件对话框   
	if (IDOK == fileDlg.DoModal())
	{
		// 如果点击了文件对话框上的“打开”按钮，则将选择的文件路径显示到编辑框里   
		strFilePath = fileDlg.GetPathName();
		SetDlgItemText(IDC_EDIT_nrec_path, strFilePath);
	}
}


void CNRECAnalystAppDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
	CString strPath;
	GetDlgItemText(IDC_EDIT_nrec_path, strPath);
	CString strLength;
	GetDlgItemText(IDC_EDIT_length, strLength);
	CString strRadius;
	GetDlgItemText(IDC_EDIT_TOOL_RADIUS, strRadius);

	if (strLength.IsEmpty() || strRadius.IsEmpty() || strPath.IsEmpty())
	{
		MessageBox(L"输入不能为空！", L"提示", MB_OK);
		return;
	}

	std::string strLen = StringUtils::wstring2string(strLength.GetBuffer());
	double dlength = atof(strLen.c_str());
	if (dlength < 0.1) {
		dlength = 1.0;
	}

	std::string strRad = StringUtils::wstring2string(strRadius.GetBuffer());
	double dRadius = atof(strRad.c_str());

	NRECToolPathHander handle;
	if (!handle.Excuting(strPath.GetBuffer(), dlength, dRadius)) {
		MessageBox(L"计算失败", _T("错误"), MB_OK);
	}
	else {
		MessageBox(L"计算成功",L"提示", MB_OK);
	}
}

