// mines.cpp : 定义应用程序的一些消息处理函数。
//

#include "stdafx.h"
#include "mines.h"

/* ************************************************
*  MenuWndProc
*  功能  菜单的消息处理函数
************************************************* */
LRESULT CALLBACK MenuWndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	INT				wmId, wmEvent, choice;
	BITMAP			bitmap, bitmapface;
	static HMENU	hMenu;

	wmId    = LOWORD(wParam);
	wmEvent = HIWORD(wParam);

	switch ( wmId )
	{
	case IDM_START:		// 开局
		// 如果游戏开始了而且没有结束则询问是否放弃当前游戏
		if ( iGameState == GAMESTART && iGameState != GAMEOVER )
		{
			choice = MessageBox( hwnd, TEXT("您要放弃本局并开始新的游戏? "), 
									TEXT("询问"), MB_OKCANCEL | MB_ICONQUESTION );
			if ( IDOK == choice )
			{
				GameStart();			// 调用游戏开始函数
				InvalidateRect( hwndMain, NULL, FALSE );  // 发送WM_PAINT消息，重绘窗口
			}
		}
		else
		{
			GameStart();
			InvalidateRect( hwndMain, NULL, FALSE );
		}
		break;
	case IDM_PRIMARY:		// 初级
		ModifyDifficulty( 9, 9, 10 );		// 调用游戏难度修改函数
		GameStart();
		InvalidateRect( hwndMain, NULL, FALSE );
		break;
	case IDM_MIDDLE:		// 中级
		ModifyDifficulty( 16, 16, 40 );
		GameStart();
		InvalidateRect( hwndMain, NULL, FALSE );
		break;
	case IDM_ADVAN:			// 高级
		ModifyDifficulty( 16, 30, 99 );
		GameStart();
		InvalidateRect( hwndMain, NULL, FALSE );
		break;
	case IDM_USERDEF:		// 自定义
		DialogBox( hinst, MAKEINTRESOURCE(IDD_USERDEF), hwnd, UserDef );	// 交由自定义对话框处理
		break;
	case IDM_SOUND:
		hMenu = GetMenu( hwnd ) ;
		// 声音开关
		if ( TRUE == iSound )
		{
			iSound = FALSE;
			CheckMenuItem( hMenu, IDM_SOUND, MF_UNCHECKED ) ;
		} 
		else
		{
			iSound = TRUE;
			CheckMenuItem( hMenu, IDM_SOUND, MF_CHECKED ) ;
		}
		break;
	case ID_ORIG:			// 切换到原始皮肤
		hBitmap  = LoadBitmap( hinst, MAKEINTRESOURCE (IDB_MINES) );
		GetObject( hBitmap, sizeof(BITMAP), &bitmap );
		hBitmapFace  = LoadBitmap( hinst, MAKEINTRESOURCE (IDB_FACE) );
		GetObject( hBitmapFace, sizeof(BITMAP), &bitmapface );
		InvalidateRect( hwndMain, NULL, FALSE );
		break;
	case ID_RED:			// 切换到红色皮肤
		hBitmap  = LoadBitmap( hinst, MAKEINTRESOURCE (IDB_MRED) );
		GetObject( hBitmap, sizeof(BITMAP), &bitmap );
		hBitmapFace  = LoadBitmap( hinst, MAKEINTRESOURCE (IDB_FRED) );
		GetObject( hBitmapFace, sizeof(BITMAP), &bitmapface );
		InvalidateRect( hwndMain, NULL, FALSE );
		break;
	case ID_YELLOW:			// 切换到黄色皮肤
		hBitmap  = LoadBitmap(hinst, MAKEINTRESOURCE (IDB_MYELLOW) );
		GetObject( hBitmap, sizeof(BITMAP), &bitmap );
		hBitmapFace  = LoadBitmap( hinst, MAKEINTRESOURCE (IDB_FYELLOW) );
		GetObject( hBitmapFace, sizeof(BITMAP), &bitmapface );
		InvalidateRect( hwndMain, NULL, FALSE );
		break;
	case ID_GREEN:			// 切换到绿色皮肤
		hBitmap  = LoadBitmap( hinst, MAKEINTRESOURCE (IDB_MGREEN) );
		GetObject( hBitmap, sizeof(BITMAP), &bitmap );
		hBitmapFace  = LoadBitmap( hinst, MAKEINTRESOURCE (IDB_FGREEN) );
		GetObject( hBitmapFace, sizeof(BITMAP), &bitmapface );
		InvalidateRect( hwndMain, NULL, FALSE );
		break;
	case ID_PURPLE:			// 切换到紫色皮肤
		hBitmap  = LoadBitmap( hinst, MAKEINTRESOURCE (IDB_MPURPLE) );
		GetObject( hBitmap, sizeof(BITMAP), &bitmap );
		hBitmapFace  = LoadBitmap( hinst, MAKEINTRESOURCE (IDB_FPURPLE) );
		GetObject( hBitmapFace, sizeof(BITMAP), &bitmapface );
		InvalidateRect( hwndMain, NULL, FALSE );
		break;
	case IDM_HELP:			// 帮助
		DialogBox( hinst, MAKEINTRESOURCE(IDD_HELP), hwnd, Help );
		break;
	case IDM_ABOUT:			// 关于
		DialogBox( hinst, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, About );
		break;
	case IDM_EXIT:			// 退出
		DestroyWindow( hwnd );
		break;
	default:
		return DefWindowProc( hwnd, uMsg, wParam, lParam );
	}

	return DefWindowProc( hwnd, uMsg, wParam, lParam );
}
