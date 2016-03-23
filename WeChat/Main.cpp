// TestOwnerUI.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Main.h"
#include "MainFrame.h"

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);

	//CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
	//CPaintManagerUI::SetResourceZip(_T("src.dat"));

	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) return 0;

	CMainFrame duiFrame;
	duiFrame.Create(NULL, _T("WeChat"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE /*| WS_EX_TOPMOST*/);
	duiFrame.SetIcon(IDI_WE_CHAT);
	duiFrame.CenterWindow();
	duiFrame.ShowModal();

	::CoUninitialize();
	return 0;
}