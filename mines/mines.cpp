// mines.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "mines.h"

/*  全局变量定义  */
HINSTANCE	hinst;											// 将实例句柄存储在全局变量中
HWND		hwndMain;										// 主窗口句柄
HBITMAP		hBitmap;										// 雷区图片
HBITMAP		hBitmapFace;									// 笑脸图片
HBITMAP		hBitmapNum;										// 数字图片
INT			iMines[2][18][32];								// 定义全局地雷矩阵
INT			iSound      =  FALSE;							// 定义游戏声音选项
INT			iGameState  =  GAMEUNSTART;						// 定义游戏状态
INT			iFaceState  =  FACEORIG;						// 定义表情状态
INT			iMineNum	=   10;								// 定义全局雷数
INT			iTimeNum	=    0;								// 定义全局时间
INT			iMineFlag   =   10;								// 定义雷数标记
INT			iRow		=    9;								// 定义全局行数
INT			iCol		=    9;								// 定义全局列数
INT			iWidth		=   iCol * RCLONG + EXCU_W * 2;		// 定义全局宽度
INT			iHeight		=   iRow * RCLONG + EXCU_H * 5;		// 定义全局高度

/* ************************************************
*   WinMain
*   功能  主函数
************************************************* */
INT APIENTRY WinMain(	 HINSTANCE	hInstance,
						 HINSTANCE	hPrevInstance, 
						 LPSTR		lpCmdLine, 
						 INT		iCmdShow   )
{
	WNDCLASSEX	wcx;					// 窗口类
	HWND		hwnd;					// 窗口句柄
	MSG			msg;					// 消息
	BOOL		fGotMessage;			// 是否成功获取消息

	hinst = hInstance;					// 应用程序实例句柄，保存为全局变量

	// 填充窗口类数据
	wcx.cbSize			=   sizeof(wcx);											// 结构体的大小
	wcx.style			=   CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;					// 大小改变时重绘界面
	wcx.lpfnWndProc		=   MainWndProc;											// 窗口消息处理函数
	wcx.cbClsExtra		=   0;														// 不使用类内存
	wcx.cbWndExtra		=   0;														// 不使用窗口内存
	wcx.hInstance		=   hInstance;												// 所处的应用程序实例句柄
	wcx.hIcon			=   LoadIcon( hInstance, MAKEINTRESOURCE(IDI_ICONMAIN));	// 图标
	wcx.hCursor			=   LoadCursor( NULL, IDC_ARROW );							// 光标：默认
	wcx.hbrBackground	=   (HBRUSH)GetStockObject((LTGRAY_BRUSH));					// 背景：灰色
	wcx.lpszMenuName	=   MAKEINTRESOURCE(IDR_MENUMAIN);							// 菜单
	wcx.lpszClassName	=   TEXT("MainWClass");										// 窗口类名
	wcx.hIconSm			=   (HICON)LoadImage(	hInstance,							// 小图标
												MAKEINTRESOURCE(IDI_ICONMAIN),
												IMAGE_ICON,
												GetSystemMetrics(SM_CXSMICON),
												GetSystemMetrics(SM_CYSMICON),
												LR_DEFAULTCOLOR );
	// 注册窗口类
	if ( !RegisterClassEx(&wcx) )
	{
		MessageBox( NULL, TEXT("本程序仅支持在 Windows NT 下运行！"), TEXT("警告"), MB_ICONERROR );
		return 1;
	}
	// 创建窗口
	hwnd = CreateWindow(    TEXT("MainWClass"),								// 窗口名
							TEXT("扫雷 测试版"),								// 窗口标题
							( WS_OVERLAPPED | WS_CAPTION | \
							  WS_SYSMENU    | WS_MINIMIZEBOX ),				// 窗口样式
							300,											// 水平位置X
							100,											// 水平位置Y
							iWidth,											// 宽度
							iHeight,										// 高度
							(HWND) NULL,									// 父窗口：无
							(HMENU) NULL,									// 菜单：使用窗口类的菜单
							hInstance,										// 应用程序实例句柄
							(LPVOID) NULL	);								// 窗口创建时数据：无

	hwndMain = hwnd;                  // 保存主窗口句柄到全局变量里，便于子函数使用
	ShowWindow( hwnd, iCmdShow );     // 显示窗口
	UpdateWindow( hwnd );			  // 发送WM_PAINT消息
	
	// 主消息循环
	while ( (fGotMessage = GetMessage( &msg, (HWND) NULL, 0, 0 )) != 0 && fGotMessage != -1 )
	{
		TranslateMessage( &msg );    // 来自键盘的消息转化
		DispatchMessage( &msg );
	}

	return msg.wParam;
}

/* ************************************************
*  MainWndProc  
*  功能  窗口消息处理函数
************************************************* */
LRESULT CALLBACK MainWndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( uMsg )
	{
		static INT			cxClinent, cyClinent, cxSource, cySource;	// 只在WM_CREATE时初始化，故定义为静态
		static HINSTANCE	hInstance;									// 只在WM_CREATE时初始化，故定义为静态
		static HDC			hdc, hdcMen;
		BITMAP				bitmap, bitmapface, bitmapnum;
		PAINTSTRUCT			ps;
		INT					iX, iY,iMX, iMY, ix, iy;
		INT					iMineFlag1, iMineFlag10, iTime1, iTime10, iTime100;	 // 定义标记雷数的个位和十位

	case WM_CREATE:
		hInstance = ((LPCREATESTRUCT) lParam)->hInstance;
		hBitmap  = LoadBitmap( hInstance, MAKEINTRESOURCE (IDB_MINES) );	// 加载扫雷的图片
		GetObject( hBitmap, sizeof(BITMAP), &bitmap );
		cxSource = bitmap.bmWidth;						// 其实就是RCLONG(25)
		cySource = (bitmap.bmHeight) / 16;				// 计算出每个方块高度，其实就是25

		hBitmapFace  = LoadBitmap( hInstance, MAKEINTRESOURCE (IDB_FACE) );	// 加载笑脸的图片
		GetObject( hBitmapFace, sizeof(BITMAP), &bitmapface );

		hBitmapNum  = LoadBitmap( hInstance, MAKEINTRESOURCE (IDB_NUM) );	// 加载数字的图片
		GetObject( hBitmapNum, sizeof(BITMAP), &bitmapnum );

		GameStart();									// 调用游戏开始函数
		SetTimer( hwnd, IDT_LIGHT, 30, OnTimerLight );  // 每隔30毫秒调用鼠标所在区块高亮函数

		return 0;
	case WM_PAINT:				// 窗口重绘消息
		hdc = BeginPaint( hwnd, &ps );
		hdcMen = CreateCompatibleDC( hdc );
		SelectObject( hdcMen, hBitmap );
		// 贴雷区图
		for ( iY = EXCU_H * 2, iMY = 1; iMY <= iRow; iY += cySource, iMY++ )
		{
			for ( iX = EXCU_W, iMX = 1; iMX <= iCol; iX += cxSource, iMX++ )
			{
				BitBlt( hdc, iX, iY, cxSource, cySource, hdcMen, 0, cySource * iMines[0][iMY][iMX], SRCCOPY );
			}
		}
		SelectObject( hdcMen, hBitmapFace );
		// 贴笑脸图
		BitBlt( hdc, iWidth / 2 - 25, 5, FACELONG, FACELONG, hdcMen, 0, FACELONG * iFaceState, SRCCOPY );
		SelectObject( hdcMen, hBitmapNum );
		// 贴雷数图
		if ( iMineFlag >= 0 )			// 由剩余没标记雷数确定第一个图片是空数字还是负号
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
		// 贴时间图
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
			MessageBox( hwnd, TEXT("您超过了游戏最大限时 999 秒，游戏结束！"), 
						TEXT("游戏结束"), MB_OK | MB_ICONINFORMATION );
		} 
		
		return 0;
	case WM_COMMAND:			// 来自菜单的消息转发给菜单消息处理函数
		MenuWndProc( hwnd, uMsg, wParam, lParam );
		return 0;
	case WM_LBUTTONUP:			// 鼠标左键抬起消息
		hdc = GetDC( hwnd );
		hdcMen = CreateCompatibleDC( hdc );
		SelectObject( hdcMen, hBitmap );

		cxClinent = LOWORD( lParam );		// 转换鼠标x坐标
		cyClinent = HIWORD( lParam );		// 转换鼠标y坐标
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
			iy = ( cyClinent - EXCU_H * 2 ) / EXCU_W + 1;		// 将鼠标坐标转换为雷区数组的x,y坐标
			if ( iGameState == GAMEUNSTART )
			{
				iTimeNum = 0;
				SetTimer( hwnd, IDT_TIME, 1000, NULL );
			}
			// 如果第一次点到的是地雷，那么重启游戏并模拟用户鼠标点击直到第一个不是地雷（很纠结要不要这个功能）
			while ( MINE == iMines[1][iy][ix] && iGameState == GAMEUNSTART )
			{
				GameStart();
				InvalidateRect( hwndMain, NULL, FALSE );
				mouse_event( MOUSEEVENTF_LEFTUP, LOWORD( lParam ), HIWORD( lParam ), 0, 0 );
			}
			if ( MINE == iMines[1][iy][ix] )		// 如果踩到了地雷
			{
				KillTimer( hwnd, IDT_TIME );		// 结束游戏计时器
				iGameState = GAMEOVER;				// 全局游戏状态设为游戏结束
				iFaceState = FACEFAIL;				// 全局表情标记设为失败表情
				if ( TRUE == iSound )				// 如果全局声音变量表示声音开启则播放爆炸声
				{
					PlaySound( MAKEINTRESOURCE(IDR_EXPLODE), NULL, SND_RESOURCE | SND_ASYNC );
				}
				for ( iY = EXCU_H * 2, iMY = 1; iMY <= iRow; iY += cySource, iMY++ )
				{
					for ( iX = EXCU_W, iMX = 1; iMX <= iCol; iX += cxSource, iMX++ )
					{
						// 显示出所有的地雷
						if ( MINE == iMines[1][iMY][iMX] )	
						{
							BitBlt( hdc, iX, iY, cxSource, cySource, 
								    hdcMen, 0, cySource * iMines[1][iMY][iMX], SRCCOPY );
							iMines[0][iMY][iMX] = MINE;
						} 
						// 贴出标记错了的地雷为带叉号的地雷
						if ( MINE != iMines[1][iMY][iMX] && FLAG == iMines[0][iMY][iMX] )
						{
							BitBlt( hdc, iX, iY, cxSource, cySource, 
									hdcMen, 0, cySource * MINE_ERR, SRCCOPY );
							iMines[0][iMY][iMX] = MINE_ERR;
						}
					}
				}
				iMines[0][iy][ix] = MINE_RED;					// 踩到的地雷标记为红色并贴图
				BitBlt( hdc, cxClinent, cyClinent, cxSource, cySource, 
						hdcMen, 0, cySource * MINE_RED, SRCCOPY );
			}
			if ( ORIG == iMines[0][iy][ix] )					// 如果踩到了正常数字
			{
				iGameState = GAMESTART;							// 游戏状态标志改为游戏已经开始
				if ( BLANK == iMines[1][iy][ix] )				// 如果点到的位置是一个空白区
				{
					Recursion( iy, ix );						// 调用雷区展开递归函数
					iMines[0][iy][ix] = BLANK;
					InvalidateRect( hwndMain, NULL, FALSE );	// 展开后发送窗口无效消息要求重绘窗口
				} 
				else											// 如果点到数字区则显示相应数字
				{
					BitBlt( hdc, cxClinent, cyClinent, cxSource, cySource, 
						hdcMen, 0, cySource * iMines[1][iy][ix], SRCCOPY );
					iMines[0][iy][ix] = iMines[1][iy][ix];
				}
			} 
			if ( JugdeWin() )									// 判断游戏是否胜利
			{
				KillTimer( hwnd, IDT_TIME );					// 结束游戏计时器
				iGameState = GAMEOVER;							// 全局游戏状态设为游戏结束
				iFaceState = FACEWIN;							// 全局表情标记设为游戏胜利
				if ( TRUE == iSound )
				{
					PlaySound( MAKEINTRESOURCE(IDR_WIN), NULL, SND_RESOURCE | SND_ASYNC );
				}
				// 所有的地雷全部用旗帜表示出来
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
	case WM_RBUTTONUP:		// 鼠标右键抬起消息
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
			if ( ORIG == iMines[0][iy][ix] )						// 如果当前为空白，则转换为旗帜
			{
				BitBlt( hdc, cxClinent, cyClinent, cxSource, cySource, hdcMen, 0, cySource * FLAG, SRCCOPY );
				iMines[0][iy][ix] = FLAG;
				iMineFlag--;
			} // 下面的else很重要，第一次没写，出错了居然想了好一会才明白  -_-||
			else if ( FLAG == iMines[0][iy][ix] )					// 如果当前为旗帜，则转换为问号
			{
				BitBlt( hdc, cxClinent, cyClinent, cxSource, cySource, hdcMen, 0, cySource * QUES, SRCCOPY );
				iMines[0][iy][ix] = QUES;
				iMineFlag++;
			}
			else if ( QUES == iMines[0][iy][ix] )					// 如果当前为问号，则转换为空白
			{
				BitBlt( hdc, cxClinent, cyClinent, cxSource, cySource, hdcMen, 0, cySource * ORIG, SRCCOPY );
				iMines[0][iy][ix] = ORIG;
			}
		}
		DeleteDC( hdcMen );
		ReleaseDC( hwnd, hdc );

		return 0;
	case WM_DESTROY:		// 窗口销毁消息
		KillTimer( hwnd, IDT_TIME );		// 结束游戏时间计时器
		KillTimer( hwnd, IDT_LIGHT );		// 结束客户区区块高亮刷新计时器
		DeleteObject( hBitmap );
		PostQuitMessage(0);

		return 0;
	default :
		return DefWindowProc( hwnd, uMsg, wParam, lParam );
	}

	return DefWindowProc( hwnd, uMsg, wParam, lParam );
}
