// mines.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "mines.h"

/*  ȫ�ֱ�������  */
HINSTANCE	hinst;											// ��ʵ������洢��ȫ�ֱ�����
HWND		hwndMain;										// �����ھ��
HBITMAP		hBitmap;										// ����ͼƬ
HBITMAP		hBitmapFace;									// Ц��ͼƬ
HBITMAP		hBitmapNum;										// ����ͼƬ
INT			iMines[2][18][32];								// ����ȫ�ֵ��׾���
INT			iSound      =  FALSE;							// ������Ϸ����ѡ��
INT			iGameState  =  GAMEUNSTART;						// ������Ϸ״̬
INT			iFaceState  =  FACEORIG;						// �������״̬
INT			iMineNum	=   10;								// ����ȫ������
INT			iTimeNum	=    0;								// ����ȫ��ʱ��
INT			iMineFlag   =   10;								// �����������
INT			iRow		=    9;								// ����ȫ������
INT			iCol		=    9;								// ����ȫ������
INT			iWidth		=   iCol * RCLONG + EXCU_W * 2;		// ����ȫ�ֿ��
INT			iHeight		=   iRow * RCLONG + EXCU_H * 5;		// ����ȫ�ָ߶�

/* ************************************************
*   WinMain
*   ����  ������
************************************************* */
INT APIENTRY WinMain(	 HINSTANCE	hInstance,
						 HINSTANCE	hPrevInstance, 
						 LPSTR		lpCmdLine, 
						 INT		iCmdShow   )
{
	WNDCLASSEX	wcx;					// ������
	HWND		hwnd;					// ���ھ��
	MSG			msg;					// ��Ϣ
	BOOL		fGotMessage;			// �Ƿ�ɹ���ȡ��Ϣ

	hinst = hInstance;					// Ӧ�ó���ʵ�����������Ϊȫ�ֱ���

	// ��䴰��������
	wcx.cbSize			=   sizeof(wcx);											// �ṹ��Ĵ�С
	wcx.style			=   CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;					// ��С�ı�ʱ�ػ����
	wcx.lpfnWndProc		=   MainWndProc;											// ������Ϣ������
	wcx.cbClsExtra		=   0;														// ��ʹ�����ڴ�
	wcx.cbWndExtra		=   0;														// ��ʹ�ô����ڴ�
	wcx.hInstance		=   hInstance;												// ������Ӧ�ó���ʵ�����
	wcx.hIcon			=   LoadIcon( hInstance, MAKEINTRESOURCE(IDI_ICONMAIN));	// ͼ��
	wcx.hCursor			=   LoadCursor( NULL, IDC_ARROW );							// ��꣺Ĭ��
	wcx.hbrBackground	=   (HBRUSH)GetStockObject((LTGRAY_BRUSH));					// ��������ɫ
	wcx.lpszMenuName	=   MAKEINTRESOURCE(IDR_MENUMAIN);							// �˵�
	wcx.lpszClassName	=   TEXT("MainWClass");										// ��������
	wcx.hIconSm			=   (HICON)LoadImage(	hInstance,							// Сͼ��
												MAKEINTRESOURCE(IDI_ICONMAIN),
												IMAGE_ICON,
												GetSystemMetrics(SM_CXSMICON),
												GetSystemMetrics(SM_CYSMICON),
												LR_DEFAULTCOLOR );
	// ע�ᴰ����
	if ( !RegisterClassEx(&wcx) )
	{
		MessageBox( NULL, TEXT("�������֧���� Windows NT �����У�"), TEXT("����"), MB_ICONERROR );
		return 1;
	}
	// ��������
	hwnd = CreateWindow(    TEXT("MainWClass"),								// ������
							TEXT("ɨ�� ���԰�"),								// ���ڱ���
							( WS_OVERLAPPED | WS_CAPTION | \
							  WS_SYSMENU    | WS_MINIMIZEBOX ),				// ������ʽ
							300,											// ˮƽλ��X
							100,											// ˮƽλ��Y
							iWidth,											// ���
							iHeight,										// �߶�
							(HWND) NULL,									// �����ڣ���
							(HMENU) NULL,									// �˵���ʹ�ô�����Ĳ˵�
							hInstance,										// Ӧ�ó���ʵ�����
							(LPVOID) NULL	);								// ���ڴ���ʱ���ݣ���

	hwndMain = hwnd;                  // ���������ھ����ȫ�ֱ���������Ӻ���ʹ��
	ShowWindow( hwnd, iCmdShow );     // ��ʾ����
	UpdateWindow( hwnd );			  // ����WM_PAINT��Ϣ
	
	// ����Ϣѭ��
	while ( (fGotMessage = GetMessage( &msg, (HWND) NULL, 0, 0 )) != 0 && fGotMessage != -1 )
	{
		TranslateMessage( &msg );    // ���Լ��̵���Ϣת��
		DispatchMessage( &msg );
	}

	return msg.wParam;
}

/* ************************************************
*  MainWndProc  
*  ����  ������Ϣ������
************************************************* */
LRESULT CALLBACK MainWndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( uMsg )
	{
		static INT			cxClinent, cyClinent, cxSource, cySource;	// ֻ��WM_CREATEʱ��ʼ�����ʶ���Ϊ��̬
		static HINSTANCE	hInstance;									// ֻ��WM_CREATEʱ��ʼ�����ʶ���Ϊ��̬
		static HDC			hdc, hdcMen;
		BITMAP				bitmap, bitmapface, bitmapnum;
		PAINTSTRUCT			ps;
		INT					iX, iY,iMX, iMY, ix, iy;
		INT					iMineFlag1, iMineFlag10, iTime1, iTime10, iTime100;	 // �����������ĸ�λ��ʮλ

	case WM_CREATE:
		hInstance = ((LPCREATESTRUCT) lParam)->hInstance;
		hBitmap  = LoadBitmap( hInstance, MAKEINTRESOURCE (IDB_MINES) );	// ����ɨ�׵�ͼƬ
		GetObject( hBitmap, sizeof(BITMAP), &bitmap );
		cxSource = bitmap.bmWidth;						// ��ʵ����RCLONG(25)
		cySource = (bitmap.bmHeight) / 16;				// �����ÿ������߶ȣ���ʵ����25

		hBitmapFace  = LoadBitmap( hInstance, MAKEINTRESOURCE (IDB_FACE) );	// ����Ц����ͼƬ
		GetObject( hBitmapFace, sizeof(BITMAP), &bitmapface );

		hBitmapNum  = LoadBitmap( hInstance, MAKEINTRESOURCE (IDB_NUM) );	// �������ֵ�ͼƬ
		GetObject( hBitmapNum, sizeof(BITMAP), &bitmapnum );

		GameStart();									// ������Ϸ��ʼ����
		SetTimer( hwnd, IDT_LIGHT, 30, OnTimerLight );  // ÿ��30�������������������������

		return 0;
	case WM_PAINT:				// �����ػ���Ϣ
		hdc = BeginPaint( hwnd, &ps );
		hdcMen = CreateCompatibleDC( hdc );
		SelectObject( hdcMen, hBitmap );
		// ������ͼ
		for ( iY = EXCU_H * 2, iMY = 1; iMY <= iRow; iY += cySource, iMY++ )
		{
			for ( iX = EXCU_W, iMX = 1; iMX <= iCol; iX += cxSource, iMX++ )
			{
				BitBlt( hdc, iX, iY, cxSource, cySource, hdcMen, 0, cySource * iMines[0][iMY][iMX], SRCCOPY );
			}
		}
		SelectObject( hdcMen, hBitmapFace );
		// ��Ц��ͼ
		BitBlt( hdc, iWidth / 2 - 25, 5, FACELONG, FACELONG, hdcMen, 0, FACELONG * iFaceState, SRCCOPY );
		SelectObject( hdcMen, hBitmapNum );
		// ������ͼ
		if ( iMineFlag >= 0 )			// ��ʣ��û�������ȷ����һ��ͼƬ�ǿ����ֻ��Ǹ���
		{
			iMineFlag10 = (INT)(iMineFlag / 10);
			iMineFlag1  = iMineFlag % 10;
			BitBlt( hdc, 30, 5, NUMWIDTH, NUMHRIGHT, hdcMen, 0, NUMHRIGHT * 0, SRCCOPY );
		} 
		else
		{
			iMineFlag10 = (INT)(-iMineFlag / 10);
			iMineFlag1  = -iMineFlag % 10;
			BitBlt( hdc, 30, 5, NUMWIDTH, NUMHRIGHT, hdcMen, 0, NUMHRIGHT * 10, SRCCOPY );
		}
		BitBlt( hdc, 50, 5, NUMWIDTH, NUMHRIGHT, hdcMen, 0, NUMHRIGHT * iMineFlag10, SRCCOPY );
		BitBlt( hdc, 70, 5, NUMWIDTH, NUMHRIGHT, hdcMen, 0, NUMHRIGHT * iMineFlag1, SRCCOPY );
		// ��ʱ��ͼ
		iTime100 = iTimeNum / 100;
		iTime10  = iTimeNum / 10 % 10;
		iTime1   = iTimeNum % 10;
		BitBlt( hdc, iWidth - 90, 5, NUMWIDTH, NUMHRIGHT, hdcMen, 0, NUMHRIGHT * iTime100, SRCCOPY );
		BitBlt( hdc, iWidth - 70, 5, NUMWIDTH, NUMHRIGHT, hdcMen, 0, NUMHRIGHT * iTime10, SRCCOPY );
		BitBlt( hdc, iWidth - 50, 5, NUMWIDTH, NUMHRIGHT, hdcMen, 0, NUMHRIGHT * iTime1, SRCCOPY );
		DeleteDC( hdcMen );
		EndPaint( hwnd, &ps );

		return 0;
	case WM_TIMER:
		if ( iTimeNum < 999 )
		{
			iTimeNum++;
		}
		else
		{
			KillTimer( hwnd, IDT_TIME );
			iGameState = GAMEOVER;
			iFaceState = FACEFAIL;
			for ( iY = EXCU_H * 2, iMY = 1; iMY <= iRow; iY += cySource, iMY++ )
			{
				for ( iX = EXCU_W, iMX = 1; iMX <= iCol; iX += cxSource, iMX++ )
				{
					if ( MINE == iMines[1][iMY][iMX] )
					{
						BitBlt( hdc, iX, iY, cxSource, cySource, 
								hdcMen, 0, cySource * iMines[1][iMY][iMX], SRCCOPY );
						iMines[0][iMY][iMX] = MINE;
					}
					if ( MINE != iMines[1][iMY][iMX] && FLAG == iMines[0][iMY][iMX] )
					{
						BitBlt( hdc, iX, iY, cxSource, cySource, hdcMen, 0, cySource * MINE_ERR, SRCCOPY );
								iMines[0][iMY][iMX] = MINE_ERR;
					}
				}
			}
			MessageBox( hwnd, TEXT("����������Ϸ�����ʱ 999 �룬��Ϸ������"), 
						TEXT("��Ϸ����"), MB_OK | MB_ICONINFORMATION );
		} 
		
		return 0;
	case WM_COMMAND:			// ���Բ˵�����Ϣת�����˵���Ϣ������
		MenuWndProc( hwnd, uMsg, wParam, lParam );
		return 0;
	case WM_LBUTTONUP:			// ������̧����Ϣ
		hdc = GetDC( hwnd );
		hdcMen = CreateCompatibleDC( hdc );
		SelectObject( hdcMen, hBitmap );

		cxClinent = LOWORD( lParam );		// ת�����x����
		cyClinent = HIWORD( lParam );		// ת�����y����
		if ( cxClinent >= iWidth / 2 - 27 && cxClinent <= iWidth / 2 + 17 && 
			 cyClinent >= 6 && cyClinent <= FACELONG + 4  )
		{
			if ( iGameState == GAMEOVER )
			{
				GameStart();
				InvalidateRect( hwndMain, NULL, FALSE );
			}
		}
		if ( cxClinent >= EXCU_W	 && cxClinent <= iWidth - (EXCU_W + 2)	&& 
			 cyClinent >= EXCU_H * 2 && cyClinent <= iHeight - (EXCU_H * 3 + 2) &&
			 iGameState != GAMEOVER  )
		{
			cxClinent = cxClinent - cxClinent % EXCU_W;
			cyClinent = cyClinent - cyClinent % EXCU_W;
			ix = ( cxClinent - EXCU_W ) / EXCU_W + 1;
			iy = ( cyClinent - EXCU_H * 2 ) / EXCU_W + 1;		// ���������ת��Ϊ���������x,y����
			if ( iGameState == GAMEUNSTART )
			{
				iTimeNum = 0;
				SetTimer( hwnd, IDT_TIME, 1000, NULL );
			}
			// �����һ�ε㵽���ǵ��ף���ô������Ϸ��ģ���û������ֱ����һ�����ǵ��ף��ܾ���Ҫ��Ҫ������ܣ�
			while ( MINE == iMines[1][iy][ix] && iGameState == GAMEUNSTART )
			{
				GameStart();
				InvalidateRect( hwndMain, NULL, FALSE );
				mouse_event( MOUSEEVENTF_LEFTUP, LOWORD( lParam ), HIWORD( lParam ), 0, 0 );
			}
			if ( MINE == iMines[1][iy][ix] )		// ����ȵ��˵���
			{
				KillTimer( hwnd, IDT_TIME );		// ������Ϸ��ʱ��
				iGameState = GAMEOVER;				// ȫ����Ϸ״̬��Ϊ��Ϸ����
				iFaceState = FACEFAIL;				// ȫ�ֱ�������Ϊʧ�ܱ���
				if ( TRUE == iSound )				// ���ȫ������������ʾ���������򲥷ű�ը��
				{
					PlaySound( MAKEINTRESOURCE(IDR_EXPLODE), NULL, SND_RESOURCE | SND_ASYNC );
				}
				for ( iY = EXCU_H * 2, iMY = 1; iMY <= iRow; iY += cySource, iMY++ )
				{
					for ( iX = EXCU_W, iMX = 1; iMX <= iCol; iX += cxSource, iMX++ )
					{
						// ��ʾ�����еĵ���
						if ( MINE == iMines[1][iMY][iMX] )	
						{
							BitBlt( hdc, iX, iY, cxSource, cySource, 
								    hdcMen, 0, cySource * iMines[1][iMY][iMX], SRCCOPY );
							iMines[0][iMY][iMX] = MINE;
						} 
						// ������Ǵ��˵ĵ���Ϊ����ŵĵ���
						if ( MINE != iMines[1][iMY][iMX] && FLAG == iMines[0][iMY][iMX] )
						{
							BitBlt( hdc, iX, iY, cxSource, cySource, 
									hdcMen, 0, cySource * MINE_ERR, SRCCOPY );
							iMines[0][iMY][iMX] = MINE_ERR;
						}
					}
				}
				iMines[0][iy][ix] = MINE_RED;					// �ȵ��ĵ��ױ��Ϊ��ɫ����ͼ
				BitBlt( hdc, cxClinent, cyClinent, cxSource, cySource, 
						hdcMen, 0, cySource * MINE_RED, SRCCOPY );
			}
			if ( ORIG == iMines[0][iy][ix] )					// ����ȵ�����������
			{
				iGameState = GAMESTART;							// ��Ϸ״̬��־��Ϊ��Ϸ�Ѿ���ʼ
				if ( BLANK == iMines[1][iy][ix] )				// ����㵽��λ����һ���հ���
				{
					Recursion( iy, ix );						// ��������չ���ݹ麯��
					iMines[0][iy][ix] = BLANK;
					InvalidateRect( hwndMain, NULL, FALSE );	// չ�����ʹ�����Ч��ϢҪ���ػ洰��
				} 
				else											// ����㵽����������ʾ��Ӧ����
				{
					BitBlt( hdc, cxClinent, cyClinent, cxSource, cySource, 
						hdcMen, 0, cySource * iMines[1][iy][ix], SRCCOPY );
					iMines[0][iy][ix] = iMines[1][iy][ix];
				}
			} 
			if ( JugdeWin() )									// �ж���Ϸ�Ƿ�ʤ��
			{
				KillTimer( hwnd, IDT_TIME );					// ������Ϸ��ʱ��
				iGameState = GAMEOVER;							// ȫ����Ϸ״̬��Ϊ��Ϸ����
				iFaceState = FACEWIN;							// ȫ�ֱ�������Ϊ��Ϸʤ��
				if ( TRUE == iSound )
				{
					PlaySound( MAKEINTRESOURCE(IDR_WIN), NULL, SND_RESOURCE | SND_ASYNC );
				}
				// ���еĵ���ȫ�������ı�ʾ����
				for ( iY = EXCU_H * 2, iMY = 1; iMY <= iRow; iY += cySource, iMY++ )
				{
					for ( iX = EXCU_W, iMX = 1; iMX <= iCol; iX += cxSource, iMX++ )
					{
						if ( MINE == iMines[1][iMY][iMX] )
						{
							BitBlt( hdc, iX, iY, cxSource, cySource, 
								    hdcMen, 0, cySource * FLAG, SRCCOPY );
									iMines[0][iMY][iMX] = FLAG;
						}
						if ( BLANK == iMines[1][iMY][iMX] )
						{
							BitBlt( hdc, iX, iY, cxSource, cySource, 
									hdcMen, 0, cySource * BLANK, SRCCOPY );
									iMines[0][iMY][iMX] = BLANK;
						}
					}
				}
			}
		}
		DeleteDC( hdcMen );
		ReleaseDC( hwnd, hdc );
		return 0;
	case WM_RBUTTONUP:		// ����Ҽ�̧����Ϣ
		hdc = GetDC( hwnd );
		hdcMen = CreateCompatibleDC( hdc );
		SelectObject( hdcMen, hBitmap );
		cxClinent = LOWORD( lParam );
		cyClinent = HIWORD( lParam );
		if ( cxClinent >= EXCU_W && cxClinent <= iWidth - (EXCU_W + 2) && 
			 cyClinent >= EXCU_H * 2 && cyClinent <= iHeight - (EXCU_H * 3 + 2) &&
			 iGameState != GAMEOVER )
		{
			cxClinent = cxClinent - cxClinent % EXCU_W;
			cyClinent = cyClinent - cyClinent % EXCU_W;
			ix = ( cxClinent - EXCU_W ) / EXCU_W + 1;
			iy = ( cyClinent - EXCU_H * 2 ) / EXCU_W + 1;
			if ( ORIG == iMines[0][iy][ix] )						// �����ǰΪ�հף���ת��Ϊ����
			{
				BitBlt( hdc, cxClinent, cyClinent, cxSource, cySource, hdcMen, 0, cySource * FLAG, SRCCOPY );
				iMines[0][iy][ix] = FLAG;
				iMineFlag--;
			} // �����else����Ҫ����һ��ûд�������˾�Ȼ���˺�һ�������  -_-||
			else if ( FLAG == iMines[0][iy][ix] )					// �����ǰΪ���ģ���ת��Ϊ�ʺ�
			{
				BitBlt( hdc, cxClinent, cyClinent, cxSource, cySource, hdcMen, 0, cySource * QUES, SRCCOPY );
				iMines[0][iy][ix] = QUES;
				iMineFlag++;
			}
			else if ( QUES == iMines[0][iy][ix] )					// �����ǰΪ�ʺţ���ת��Ϊ�հ�
			{
				BitBlt( hdc, cxClinent, cyClinent, cxSource, cySource, hdcMen, 0, cySource * ORIG, SRCCOPY );
				iMines[0][iy][ix] = ORIG;
			}
		}
		DeleteDC( hdcMen );
		ReleaseDC( hwnd, hdc );

		return 0;
	case WM_DESTROY:		// ����������Ϣ
		KillTimer( hwnd, IDT_TIME );		// ������Ϸʱ���ʱ��
		KillTimer( hwnd, IDT_LIGHT );		// �����ͻ����������ˢ�¼�ʱ��
		DeleteObject( hBitmap );
		PostQuitMessage(0);

		return 0;
	default :
		return DefWindowProc( hwnd, uMsg, wParam, lParam );
	}

	return DefWindowProc( hwnd, uMsg, wParam, lParam );
}
