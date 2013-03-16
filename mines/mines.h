#pragma once

#include "resource.h"

/*  ȫ�ֱ�������  */
extern HINSTANCE	hinst;
extern HWND			hwndMain;
extern HBITMAP		hBitmap;			// ����ͼƬ
extern HBITMAP		hBitmapFace;		// Ц��ͼƬ
extern HBITMAP		hBitmapNum;			// ����ͼƬ
extern INT			iSound;				// ������Ϸ����ѡ��
extern INT			iGameState;			// ������Ϸ״̬����
extern INT			iMines[2][18][32];	// ������Ϸ�ֵ��׾���
extern INT			iFaceState;			// ��������״̬
extern INT			iMineNum;			// ����ȫ������
extern INT			iMineFlag;			// �����������
extern INT			iTimeNum;			// ����ȫ��ʱ��
extern INT			iRow;				// ����ȫ������
extern INT			iCol;				// ����ȫ������
extern INT			iWidth;				// ����ȫ�ֿ��
extern INT			iHeight;			// ����ȫ�ָ߶�

/*  Ĭ�ϵ���Ϣ������  */
LRESULT CALLBACK MainWndProc( HWND, UINT, WPARAM, LPARAM );

/*  �˵�����Ϣ������  */
LRESULT CALLBACK MenuWndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

/*  ��ʱˢ�¿ͻ�������ģ��ĺ���  */
VOID CALLBACK OnTimerLight(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime);

/*  �޸���Ϸ�ѶȺ���  */
bool ModifyDifficulty( INT iX, INT iY, INT iNum );

/*  ��ʼ����Ϸ���ݺ���  */
bool GameStart( VOID );

/*  ���׺���  */
bool BuryMines( VOID );

/*  �����������ɺ���  */
bool GenerateNum( VOID );

/*  �����ݹ�չ������  */
bool Recursion( INT iy, INT ix );

/*  �ж��Ƿ�ʤ����������  */
bool JugdeWin( VOID );

/*  �Զ���Ի������Ϣ������  */
INT CALLBACK UserDef(  HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

/*  ���������Ϣ������  */
INT CALLBACK Help(  HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

/*  ���ڿ����Ϣ������  */
INT CALLBACK About( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam );


/*  ������Ϸ״̬����  */
#define GAMEUNSTART     0
#define GAMESTART		1
#define GAMEOVER	   -1

/*  ����Ц��״̬����  */
#define FACEORIG		0
#define FACEFAIL		1
#define FACEWIN			2
#define FACEORIGDOWN	3	// 3-5����������һ�����ݵ�ͼ�꣬��ʱûʵ��
#define FACEFAILDOWN	4
#define FACEWINDOWN		5

/*  ���嶨ʱ���ı��  */
#define IDT_LIGHT		1
#define IDT_TIME		2

/*  ������������ͼƬ���  */
#define NUMWIDTH		20

/*  ������������ͼƬ�߶�  */
#define NUMHRIGHT		40

/*  ����Ц������ͼƬ����  */
#define FACELONG		40

/*  �����������ͼƬ����  */
#define RCLONG			25

/*  ����ͻ������������ϱ�����ߵ�ƫ��  */
#define EXCU_W			25
#define EXCU_H			25

/*  ������Դ�ĺ궨�壬ͬʱӦ������ͼ˳��  */
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
