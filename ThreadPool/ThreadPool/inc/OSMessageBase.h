#ifndef OSMESSAGEBASE_H_
#define OSMESSAGEBASE_H_
#include "OSDefs.h"
/*
 * FA Messages
 */

#define FM_NULL                         0x0000
#define FM_ACT           				0x0001      // 异步完成消息
#define FM_GETSERVICE    				0x0002      // 获取系统服务
#define FM_CREATE                       0x0003
#define FM_DESTROY                      0x0004
#define FM_MOVE                         0x0005
#define FM_SIZE                         0x0006
#define FM_ACTIVATE                     0x0007
//
#define FM_SETFOCUS                     0x0007
#define FM_KILLFOCUS                    0x0008
//
#define FM_ENABLE                       0x000A
#define FM_SETTEXT                      0x000C
#define FM_GETTEXT                      0x000D
//
#define FM_PAINT                        0x000E
#define FM_CLOSE                        0x000F
#define FM_QUIT                         0x0010
#define FM_ERASEBKGND                   0x0011
#define FM_SETTINGCHANGE                0x0012
#define FM_ACTIVATEAPP                  0x0013
#define FM_FONTCHANGE                   0x0014
#define FM_TIMECHANGE                   0x0015
#define FM_SETCURSOR                    0x0016
#define FM_MOUSEACTIVATE                0x0017
#define FM_CHILDACTIVATE                0x0018
//
#define FM_KEYUP						0x0019
#define FM_KEYDOWN						0x001A
//
#define FM_TIMEOUT						0x001B


// 用户自定义消息起始码
#define FM_USER			 				0X00FF

//
// class defined
//

class OSMessageBase;

class MSG
{
public:
	MSG( void );

	OSMessageBase* m_pSource;   // 发出该消息的对象
	DWORD		   m_nCmd;      // 命令
	DWORD	       m_wParam;    // 参数1
	DWORD	       m_lParam;    // 参数2
	BOOL		   m_bAsyn;     // 异步标记
	void*		   m_pACT;      // 异步完成标识

	//static void*  operator new( size_t nSize );
	//static void   operator delete( void *p, size_t nSize );
};

class OSMessageBase
{
public:
	OSMessageBase( int nQueueBuffSize = 256 );
	virtual ~OSMessageBase( void );

	BOOL isOK() const { return m_bOk; }

	int PostMessage( OSMessageBase *pTarget, MSG *msg );
	int PostMessage( OSMessageBase *pTarget,
					 DWORD nCmd,
					 DWORD wParam,
					 DWORD lParam,
					 void* act = NULL );

	int SendMessage( OSMessageBase *pTarget, MSG *msg );
	int SendMessage( OSMessageBase *pTarget,
					 DWORD nCmd,
					 DWORD wParam,
					 DWORD lParam );

protected:
	virtual int	OnHandleMessage( MSG *msg );
	virtual int ReceiveMessage( MSG *msg );

	BOOL QueueInitialize( void );
	int  Post( MSG *msg );
	int  Pend( MSG *&msg, DWORD dwWaitTime = 0 );

private:
	BOOL	 m_bOk;
	//OSQueue	 m_Queue;
};

#endif // __MSGBASE_H__
