#ifndef DuiTrayIcon_h__
#define DuiTrayIcon_h__

#pragma once
#include <ShellAPI.h>
#define ICON_TWINKING 1009
namespace DuiLib
{
	//class IDuiTimer;
	class CDuiTrayIcon
	{
	public:
		CDuiTrayIcon(void);
		~CDuiTrayIcon(void);

	public:
		void CreateTrayIcon( HWND _RecvHwnd,UINT _IconIDResource,LPCTSTR _ToolTipText = NULL,UINT _Message = NULL/*(UIEVENT_TRAYICON)*/);
		void DeleteTrayIcon();
		bool SetTooltipText(LPCTSTR _ToolTipText);
		bool SetTooltipText(UINT _IDResource);
		CDuiString GetTooltipText() const;

		bool SetIcon(HICON _Hicon);
		bool SetIcon(LPCTSTR _IconFile);
		bool SetIcon(UINT _IDResource);
		HICON GetIcon() const;
		void SetHideIcon();
		void SetShowIcon();
		void RemoveIcon();
		bool StartTwinkling(); //∆Ù∂Ø…¡À∏
		void StopTwinkling(); //Õ£÷π…¡À∏

		bool Enabled(){return m_bEnabled;};
		bool IsVisible(){return !m_bVisible;};
		bool IsTwinkling(){return m_bTwinkling;};

		//void OnTimer(IDuiTimer* pTimer);
	private:
		bool m_bEnabled;
		bool m_bVisible;
		bool m_bTwinkling;
		HWND m_hWnd;
		UINT m_uMessage;
		HICON m_hIcon;
		//IDuiTimer* pIDuiTimer;
		NOTIFYICONDATA	m_trayData;
	};
}
#endif // DuiTrayIcon_h__

