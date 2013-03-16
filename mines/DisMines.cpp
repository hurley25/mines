// DisMines.cpp : ����Ӧ�ó�����߼���������
//

#include "stdafx.h"
#include "mines.h"

/* ************************************************
*  OnTimerLight 
*  ����  ��ʱˢ�¿ͻ�������ģ��ĺ���
************************************************* */
VOID CALLBACK OnTimerLight( HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime )
{
	INT		cxClinent, cyClinent;
	HDC		hdc, hdcMen;
	INT		iX, iY, iMX, iMY, ix, iy;
	POINT	point;
	RECT	rect;

	GetWindowRect( hwndMain, &rect );
	GetCursorPos( &point );
	cxClinent = point.x - rect.left;
	cyClinent = point.y - rect.top - 48;
	hdc = GetDC( hwndMain );
	hdcMen = CreateCompatibleDC( hdc );
	SelectObject( hdcMen, hBitmap );
	if ( cxClinent >= EXCU_W && cxClinent <= iWidth -  (EXCU_W + 2) && 
		cyClinent >= EXCU_H * 2 && cyClinent <= iHeight - (EXCU_H * 3 + 2) &&
		iGameState != GAMEOVER  )
	{
		cxClinent = cxClinent - cxClinent % 25;
		cyClinent = cyClinent - cyClinent % 25;
		ix = ( cxClinent - EXCU_W ) / 25 + 1;
		iy = ( cyClinent - EXCU_H * 2 ) / 25 + 1;
		if ( ORIG == iMines[0][iy][ix] )
		{
			BitBlt( hdc, cxClinent, cyClinent, EXCU_W, EXCU_H,
				hdcMen, 0, EXCU_H * ORIGLIGHT, SRCCOPY );
			for ( iY = EXCU_H * 2, iMY = 1; iMY <= iRow; iY += EXCU_H, iMY++ )
			{
				for ( iX = EXCU_W, iMX = 1; iMX <= iCol; iX += EXCU_W, iMX++ )
				{
					if ( ( iMY != iy || iMX != ix ) && ORIG == iMines[0][iy][ix] )
					{
						BitBlt( hdc, iX, iY, EXCU_W, EXCU_H, 
									hdcMen, 0, EXCU_H * iMines[0][iMY][iMX], SRCCOPY );
					}
				}
			}
		}
		else
		{
			InvalidateRect( hwndMain, NULL, FALSE );
		}
	}
	else
	{
		InvalidateRect( hwndMain, NULL, FALSE );
	}
	DeleteDC( hdcMen );
	ReleaseDC( hwndMain, hdc );
}

/* ************************************************
*  ModifyDifficulty
*  ����  �޸���Ϸ�Ѷ�
************************************************* */
bool ModifyDifficulty( INT iX, INT iY, INT iNum )
{
	RECT rect;

	iRow = iX;
	iCol = iY;
	iMineNum = iNum;
	iWidth	= iCol * RCLONG + EXCU_W * 2;		// ���¼�����Ϸ���泤�Ϳ�
	iHeight	= iRow * RCLONG + EXCU_H * 5;
	GetWindowRect( hwndMain, &rect );
	if ( !MoveWindow( hwndMain, rect.left, rect.top, iWidth, iHeight, TRUE ) )
	{
		return FALSE;
	}

	return TRUE;
}

/* ************************************************
*  GameStart  
*  ����  ��ʼ����Ϸ���ݣ���ʼ����Ϸ
************************************************* */
bool GameStart( VOID )
{
	ZeroMemory( iMines, sizeof(iMines) );
	BuryMines();					// ����
	GenerateNum();					// ������
	iGameState = GAMEUNSTART;		// ��ȫ�ֱ�����ʼ��
	iFaceState = FACEORIG;
	iTimeNum = 0;
	iMineFlag = iMineNum;

	return TRUE;
}

/* ************************************************
*  BuryMines  
*  ����  ��������
************************************************* */
bool BuryMines( VOID )
{
	INT	iX, iY, iNum = 0;

	srand( (UINT)GetCurrentTime() );		// �õ�ǰϵͳʱ�������������
	while ( iNum < iMineNum )
	{
		iY = rand() % (iCol + 1);		// ������ɵ�����������
		iX = rand() % (iRow + 1);
		if ( iX != 0 && iY != 0 && iMines[1][iX][iY] != MINE )	// �����ظ�λ��
		{
			iMines[1][iX][iY] = MINE;
			iNum++;
		}
	}

	return TRUE;
}

/* ************************************************
*  GenerateNum
*  ����  ���������е�����Χ������
************************************************* */
bool GenerateNum( VOID )
{
	INT	iX, iY, iNum;

	for ( iY = 1; iY <= iCol; iY++ )
	{
		for ( iX = 1; iX <= iRow; iX++ )
		{
			iNum = 0;
			if ( ORIG == iMines[1][iX][iY] )	// ����ÿһ���հ��Ƿ�Ҫ�������֣�ò�ư취�ϱ�
			{
				if ( iMines[1][iX-1][iY] == MINE )iNum++;
				if ( iMines[1][iX+1][iY] == MINE )iNum++;
				if ( iMines[1][iX][iY-1] == MINE )iNum++;
				if ( iMines[1][iX][iY+1] == MINE )iNum++;
				if ( iMines[1][iX-1][iY-1] == MINE )iNum++;
				if ( iMines[1][iX+1][iY+1] == MINE )iNum++;
				if ( iMines[1][iX-1][iY+1] == MINE )iNum++;
				if ( iMines[1][iX+1][iY-1] == MINE )iNum++;
				iMines[1][iX][iY] = iNum;
			}
		}
	}
	for ( iY = 1; iY <= iCol; iY++ )
	{
		for ( iX = 1; iX <= iRow; iX++ )
		{
			if ( ORIG == iMines[1][iX][iY] )
			{
				iMines[1][iX][iY] = BLANK;			// û�����ֵ���������Ϊ�հ�
			}
		}
	}

	return TRUE;
}

/* ************************************************
*  JugdeWin
*  ����  �ж��Ƿ�ʤ����������
************************************************* */
bool JugdeWin( VOID )
{
	INT iNum = 0, iFlag = 0, iQues = 0, iY, iX;

	for ( iX = 1; iX <= iRow; iX++ )
	{
		for ( iY = 1; iY <= iCol; iY++ )
		{
			if ( ORIG == iMines[0][iX][iY] )
			{
				iNum++;
			}
			else if ( FLAG == iMines[0][iX][iY] )
			{
				iFlag++;
			}
			else if ( QUES == iMines[0][iX][iY] )
			{
				iQues++;
			}
		}
	}
	if ( iMineNum == iNum + iFlag + iQues )
	{
		return TRUE;
	} 
	else
	{
		return FALSE;
	}
}

/* ************************************************
*  Recursion
*  ����  �ݹ�չ�������ĺ��� 
*		 �ܳ���һ���ݹ麯������ʱû�뵽�취����
************************************************* */
bool Recursion( INT iy, INT ix )
{
	INT iNum;
	do 
	{
		iNum = 0;
		if ( iMines[1][iy-1][ix] == BLANK && iMines[0][iy-1][ix] != BLANK )
		{
			iMines[0][iy-1][ix] = iMines[1][iy-1][ix];
			iNum++;
			Recursion( iy-1, ix );
		}
		else if ( iMines[1][iy-1][ix] != MINE )
		{
			iMines[0][iy-1][ix] = iMines[1][iy-1][ix];
		}
		if ( iMines[1][iy+1][ix] == BLANK && iMines[0][iy+1][ix] != BLANK )
		{
			iMines[0][iy+1][ix] = iMines[1][iy+1][ix];
			iNum++;
			Recursion( iy+1, ix );
		}
		else if ( iMines[1][iy+1][ix] != MINE )
		{
			iMines[0][iy+1][ix] = iMines[1][iy+1][ix];
			iNum++;
		}
		if ( iMines[1][iy][ix-1] == BLANK && iMines[0][iy][ix-1] != BLANK )
		{
			iMines[0][iy][ix-1] = iMines[1][iy][ix-1];
			iNum++;
			Recursion( iy, ix-1 );
		}
		else if ( iMines[1][iy][ix-1] != MINE )
		{
			iMines[0][iy][ix-1] = iMines[1][iy][ix-1];
			iNum++;
		}
		if ( iMines[1][iy][ix+1] == BLANK && iMines[0][iy][ix+1] != BLANK )
		{
			iMines[0][iy][ix+1] = iMines[1][iy][ix+1];
			Recursion( iy, ix+1 );
			iNum++;
		}
		else if ( iMines[1][iy][ix+1] != MINE )
		{
			iMines[0][iy][ix+1] = iMines[1][iy][ix+1];
			iNum++;
		}
		if ( iMines[1][iy-1][ix-1] == BLANK && iMines[0][iy-1][ix-1] != BLANK )
		{
			iMines[0][iy-1][ix-1] = iMines[1][iy-1][ix-1];
			iNum++;
			Recursion( iy-1, ix-1 );
		}
		else if ( iMines[1][iy-1][ix-1] != MINE )
		{
			iMines[0][iy-1][ix-1] = iMines[1][iy-1][ix-1];
			iNum++;
		}
		if ( iMines[1][iy+1][ix+1] == BLANK && iMines[0][iy+1][ix+1] != BLANK )
		{
			iMines[0][iy+1][ix+1] = iMines[1][iy+1][ix+1];
			iNum++;
			Recursion( iy+1, ix+1 );
		}
		else if ( iMines[1][iy+1][ix+1] != MINE )
		{
			iMines[0][iy+1][ix+1] = iMines[1][iy+1][ix+1];
			iNum++;
		}
		if ( iMines[1][iy-1][ix+1] == BLANK && iMines[0][iy-1][ix+1] != BLANK )
		{
			iMines[0][iy-1][ix+1] = iMines[1][iy-1][ix+1];
			iNum++;
			Recursion( iy-1, ix+1 );
		}
		else if ( iMines[1][iy-1][ix+1] != MINE )
		{
			iMines[0][iy-1][ix+1] = iMines[1][iy-1][ix+1];
			iNum++;
		}
		if ( iMines[1][iy+1][ix-1] == BLANK && iMines[0][iy+1][ix-1] != BLANK )
		{
			iMines[0][iy+1][ix-1] = iMines[1][iy+1][ix-1];
			iNum++;
			Recursion( iy+1, ix-1 );
		}
		else if ( iMines[1][iy+1][ix-1] != MINE )
		{
			iMines[0][iy+1][ix-1] = iMines[1][iy+1][ix-1];
			iNum++;
		}
	} while ( 0 == iNum );

	return TRUE;
}

/* ************************************************
*  UserDef
*  ����  �Զ�����Ϸ�Ի�����Ϣ������
************************************************* */
INT CALLBACK UserDef(  HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	TCHAR	cHeight[256], cWidth[256], cMine[256];
	INT		iH, iW, iM;

	ZeroMemory( cHeight, sizeof(cHeight) );
	ZeroMemory( cWidth, sizeof(cWidth) );
	ZeroMemory( cMine, sizeof(cMine) );

	UNREFERENCED_PARAMETER( lParam );
	switch ( uMsg )
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if ( LOWORD(wParam) == IDOK )
		{
			GetDlgItemText( hwnd, IDC_HEIGHT, cHeight, sizeof(cHeight)/sizeof(TCHAR));
			GetDlgItemText( hwnd, IDC_WIDTH, cWidth, sizeof(cWidth)/sizeof(TCHAR));
			GetDlgItemText( hwnd, IDC_NUM, cMine, sizeof(cMine)/sizeof(TCHAR));
			iH = _wtoi( cHeight );		// �ַ�ת��Ϊ����
			iW = _wtoi( cWidth );
			iM = _wtoi( cMine );
			// �����Զ�������Ҫ��һ������ķ�Χ��
			if (	iH >= 9  && iH <= 16 &&
					iW >= 9  && iW <= 30 &&
					iM >= 10 && iM <= 99 &&
					iH * iW >= iM -10  )
			{
				ModifyDifficulty( iH, iW, iM );
				GameStart();
				InvalidateRect( hwndMain, NULL, FALSE );
				EndDialog( hwnd, LOWORD(wParam) );
			} 
			else
			{
				MessageBox( hwnd, TEXT("���������ֵ��Χ�������������룡(�������ó������*�߶�-10)"), 
					TEXT("����"), MB_OK | MB_ICONERROR  );
			}
			return TRUE;
		}
		else if ( LOWORD(wParam) == IDCANCEL )
		{
			EndDialog( hwnd, LOWORD(wParam) );
		}
		break;
	}
	return FALSE;
}

/* ************************************************
*  Help
*  ����  ���������Ϣ������
************************************************* */
INT CALLBACK Help(  HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	UNREFERENCED_PARAMETER( lParam );
	switch ( uMsg )
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if ( LOWORD(wParam) == IDOK )
		{
			EndDialog( hwnd, LOWORD(wParam) );
			return TRUE;
		}
		break;
	}
	return FALSE;
}

/* ************************************************
*  About
*  ����  ���ڿ����Ϣ������
************************************************* */
INT CALLBACK About(  HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	UNREFERENCED_PARAMETER( lParam );
	switch ( uMsg )
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if ( LOWORD(wParam) == IDOK )
		{
			EndDialog( hwnd, LOWORD(wParam) );
			return TRUE;
		}
		break;
	}
	return FALSE;
}
