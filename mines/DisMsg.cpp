// mines.cpp : ����Ӧ�ó����һЩ��Ϣ��������
//

#include "stdafx.h"
#include "mines.h"

/* ************************************************
*  MenuWndProc
*  ����  �˵�����Ϣ������
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
	case IDM_START:		// ����
		// �����Ϸ��ʼ�˶���û�н�����ѯ���Ƿ������ǰ��Ϸ
		if ( iGameState == GAMESTART && iGameState != GAMEOVER )
		{
			choice = MessageBox( hwnd, TEXT("��Ҫ�������ֲ���ʼ�µ���Ϸ? "), 
									TEXT("ѯ��"), MB_OKCANCEL | MB_ICONQUESTION );
			if ( IDOK == choice )
			{
				GameStart();			// ������Ϸ��ʼ����
				InvalidateRect( hwndMain, NULL, FALSE );  // ����WM_PAINT��Ϣ���ػ洰��
			}
		}
		else
		{
			GameStart();
			InvalidateRect( hwndMain, NULL, FALSE );
		}
		break;
	case IDM_PRIMARY:		// ����
		ModifyDifficulty( 9, 9, 10 );		// ������Ϸ�Ѷ��޸ĺ���
		GameStart();
		InvalidateRect( hwndMain, NULL, FALSE );
		break;
	case IDM_MIDDLE:		// �м�
		ModifyDifficulty( 16, 16, 40 );
		GameStart();
		InvalidateRect( hwndMain, NULL, FALSE );
		break;
	case IDM_ADVAN:			// �߼�
		ModifyDifficulty( 16, 30, 99 );
		GameStart();
		InvalidateRect( hwndMain, NULL, FALSE );
		break;
	case IDM_USERDEF:		// �Զ���
		DialogBox( hinst, MAKEINTRESOURCE(IDD_USERDEF), hwnd, UserDef );	// �����Զ���Ի�����
		break;
	case IDM_SOUND:
		hMenu = GetMenu( hwnd ) ;
		// ��������
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
	case ID_ORIG:			// �л���ԭʼƤ��
		hBitmap  = LoadBitmap( hinst, MAKEINTRESOURCE (IDB_MINES) );
		GetObject( hBitmap, sizeof(BITMAP), &bitmap );
		hBitmapFace  = LoadBitmap( hinst, MAKEINTRESOURCE (IDB_FACE) );
		GetObject( hBitmapFace, sizeof(BITMAP), &bitmapface );
		InvalidateRect( hwndMain, NULL, FALSE );
		break;
	case ID_RED:			// �л�����ɫƤ��
		hBitmap  = LoadBitmap( hinst, MAKEINTRESOURCE (IDB_MRED) );
		GetObject( hBitmap, sizeof(BITMAP), &bitmap );
		hBitmapFace  = LoadBitmap( hinst, MAKEINTRESOURCE (IDB_FRED) );
		GetObject( hBitmapFace, sizeof(BITMAP), &bitmapface );
		InvalidateRect( hwndMain, NULL, FALSE );
		break;
	case ID_YELLOW:			// �л�����ɫƤ��
		hBitmap  = LoadBitmap(hinst, MAKEINTRESOURCE (IDB_MYELLOW) );
		GetObject( hBitmap, sizeof(BITMAP), &bitmap );
		hBitmapFace  = LoadBitmap( hinst, MAKEINTRESOURCE (IDB_FYELLOW) );
		GetObject( hBitmapFace, sizeof(BITMAP), &bitmapface );
		InvalidateRect( hwndMain, NULL, FALSE );
		break;
	case ID_GREEN:			// �л�����ɫƤ��
		hBitmap  = LoadBitmap( hinst, MAKEINTRESOURCE (IDB_MGREEN) );
		GetObject( hBitmap, sizeof(BITMAP), &bitmap );
		hBitmapFace  = LoadBitmap( hinst, MAKEINTRESOURCE (IDB_FGREEN) );
		GetObject( hBitmapFace, sizeof(BITMAP), &bitmapface );
		InvalidateRect( hwndMain, NULL, FALSE );
		break;
	case ID_PURPLE:			// �л�����ɫƤ��
		hBitmap  = LoadBitmap( hinst, MAKEINTRESOURCE (IDB_MPURPLE) );
		GetObject( hBitmap, sizeof(BITMAP), &bitmap );
		hBitmapFace  = LoadBitmap( hinst, MAKEINTRESOURCE (IDB_FPURPLE) );
		GetObject( hBitmapFace, sizeof(BITMAP), &bitmapface );
		InvalidateRect( hwndMain, NULL, FALSE );
		break;
	case IDM_HELP:			// ����
		DialogBox( hinst, MAKEINTRESOURCE(IDD_HELP), hwnd, Help );
		break;
	case IDM_ABOUT:			// ����
		DialogBox( hinst, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, About );
		break;
	case IDM_EXIT:			// �˳�
		DestroyWindow( hwnd );
		break;
	default:
		return DefWindowProc( hwnd, uMsg, wParam, lParam );
	}

	return DefWindowProc( hwnd, uMsg, wParam, lParam );
}
