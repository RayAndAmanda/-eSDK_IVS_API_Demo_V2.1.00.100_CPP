
// IVS_Demo.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "IVS_Demo.h"
#include "IVS_DemoDlg.h"
#include "IVSSDKSheet.h"
#include "log.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIVS_DemoApp

BEGIN_MESSAGE_MAP(CIVS_DemoApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CIVS_DemoApp construction

CIVS_DemoApp::CIVS_DemoApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CIVS_DemoApp object

CIVS_DemoApp theApp;


// CIVS_DemoApp initialization

BOOL CIVS_DemoApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	//���ڴ� by ray
	/*AllocConsole();
	freopen("CONOUT$","w+t",stdout);
	freopen("CONIN$","r+t",stdin);*/
	CIVSSDKSheet sheet (_T("IVS_Demo"));


	///////test log
	// GqyInsertLog(GQY_LOG_INFO,"%s",_T("hello world!"));
	// GqyInsertLog(GQY_LOG_ERROR,"%s,%d %s",_T("hello world!"),12,sss);
	// GqyInsertLog(GQY_LOG_WARNING,"%s,%d %s",_T("hello world!"),12,sss);
	m_pMainWnd = &sheet;
	INT_PTR nResponse = sheet.DoModal();

	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

