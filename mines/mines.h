#pragma once

#include "resource.h"

/*  全局变量声明  */
extern HINSTANCE	hinst;
extern HWND			hwndMain;
extern HBITMAP		hBitmap;			// 雷区图片
extern HBITMAP		hBitmapFace;		// 笑脸图片
extern HBITMAP		hBitmapNum;			// 数字图片
extern INT			iSound;				// 声明游戏声音选项
extern INT			iGameState;			// 声明游戏状态参数
extern INT			iMines[2][18][32];	// 声明游戏局地雷矩阵
extern INT			iFaceState;			// 声明表情状态
extern INT			iMineNum;			// 声明全局雷数
extern INT			iMineFlag;			// 定义雷数标记
extern INT			iTimeNum;			// 定义全局时间
extern INT			iRow;				// 声明全局行数
extern INT			iCol;				// 声明全局列数
extern INT			iWidth;				// 声明全局宽度
extern INT			iHeight;			// 声明全局高度

/*  默认的消息处理函数  */
LRESULT CALLBACK MainWndProc( HWND, UINT, WPARAM, LPARAM );

/*  菜单的消息处理函数  */
LRESULT CALLBACK MenuWndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

/*  定时刷新客户区高亮模块的函数  */
VOID CALLBACK OnTimerLight(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime);

/*  修改游戏难度函数  */
bool ModifyDifficulty( INT iX, INT iY, INT iNum );

/*  初始化游戏数据函数  */
bool GameStart( VOID );

/*  布雷函数  */
bool BuryMines( VOID );

/*  雷区数字生成函数  */
bool GenerateNum( VOID );

/*  雷区递归展开函数  */
bool Recursion( INT iy, INT ix );

/*  判断是否胜利函数函数  */
bool JugdeWin( VOID );

/*  自定义对话框的消息处理函数  */
INT CALLBACK UserDef(  HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

/*  帮助框的消息处理函数  */
INT CALLBACK Help(  HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

/*  关于框的消息处理函数  */
INT CALLBACK About( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam );


/*  定义游戏状态参数  */
#define GAMEUNSTART     0
#define GAMESTART		1
#define GAMEOVER	   -1

/*  定义笑脸状态参数  */
#define FACEORIG		0
#define FACEFAIL		1
#define FACEWIN			2
#define FACEORIGDOWN	3	// 3-5本意是做出一个凹陷的图标，暂时没实现
#define FACEFAILDOWN	4
#define FACEWINDOWN		5

/*  定义定时器的编号  */
#define IDT_LIGHT		1
#define IDT_TIME		2

/*  定义数字区域图片宽度  */
#define NUMWIDTH		20

/*  定义数字区域图片高度  */
#define NUMHRIGHT		40

/*  定义笑脸区域图片长度  */
#define FACELONG		40

/*  定义地雷区域图片长度  */
#define RCLONG			25

/*  定义客户区距离雷区上边与左边的偏移  */
#define EXCU_W			25
#define EXCU_H			25

/*  雷区资源的宏定义，同时应用于贴图顺序  */
#define  ORIG			 0
#define  NUM1			 1
#define  NUM2			 2
#define  NUM3			 3
#define  NUM4			 4
#define  NUM5			 5
#define  NUM6			 6
#define  NUM7			 7
#define  NUM8			 8
#define  MINE			 9
#define  MINE_RED		10
#define  MINE_ERR		11
#define  FLAG			12
#define  QUES			13
#define  BLANK			14
#define  ORIGLIGHT		15
