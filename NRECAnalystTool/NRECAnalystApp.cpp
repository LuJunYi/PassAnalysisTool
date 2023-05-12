﻿
// NRECAnalystApp.cpp: 定义应用程序的类行为。
//

#include "pch.h"
#include "framework.h"
#include "NRECAnalystApp.h"
#include "NRECAnalystAppDlg.h"

#include "base.hxx"
#include "license.hxx"
#include "kernapi.hxx"
#include "fileinfo.hxx"
#include "spa_unlock_result.hxx"

// place the spatial_license.h file from the download site in include folder
#include "spatial_license.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void unlock_license()
{
	spa_unlock_result out = spa_unlock_products(SPATIAL_LICENSE);
	if (out.get_state() == SPA_UNLOCK_PASS_WARN)
	{ /* insert your warning notification code here */;
	}
	else if (out.get_state() != SPA_UNLOCK_PASS)
	{ /* insert your error handling code here */;
	}
}

int initialize_acis()
{
	outcome result;

	int level = 0;
	// Base initalization will among other things
	//  create thread-local storage and a thread ID.
	if (initialize_base())
	{
		++level;
		outcome result = api_start_modeller(0);
		if (result.ok())
		{
			++level;
			// Add common ACIS state settings here.
		}
	}
	FileInfo fileinfo;
	fileinfo.set_units(1.0);
	fileinfo.set_product_id("ACIS (c) SPATIAL");
	result = api_set_file_info(3, fileinfo);
	if (2 > level)
	{
		//qDebug() << "Could not initialize ACIS";
	}
	else
	{
		unlock_license();
	}
	return level;
}

void terminate_acis(int level)
{
	// terminate ACIS
	if (level > 1)
	{
		// This outcome object must destruct before terminate base is called,
		//  because thread-local storage is accessed.
		outcome result = api_stop_modeller();
		if (result.ok() != FALSE)
		{
			--level;
		}
	}

	if (level > 0)
	{
		if (terminate_base())
		{
			--level;
		}
	}
}

// CNRECAnalystAppApp

BEGIN_MESSAGE_MAP(CNRECAnalystAppApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CNRECAnalystAppApp 构造

CNRECAnalystAppApp::CNRECAnalystAppApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CNRECAnalystAppApp 对象

CNRECAnalystAppApp theApp;


// CNRECAnalystAppApp 初始化

BOOL CNRECAnalystAppApp::InitInstance()
{	
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Initialize ACIS on the main thread
	if (initialize_acis() != 2)
	{
		MessageBox(NULL, L"Could not initialize ACIS, exiting", L"Fatal Error!", NULL);
		exit(1);
	}

	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CNRECAnalystAppDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	terminate_acis(2);

	return FALSE;
}
