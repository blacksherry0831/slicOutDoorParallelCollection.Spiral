/* RT-Thread config file */
#ifndef __RTTHREAD_CFG_H__
#define __RTTHREAD_CFG_H__

#define RT_USING_NEWLIB
//#define RT_USING_PTHREADS


/* RT_NAME_MAX*/
#define RT_NAME_MAX	    8

/* RT_ALIGN_SIZE*/
#define RT_ALIGN_SIZE	8

/* PRIORITY_MAX */
//#define RT_THREAD_PRIORITY_MAX	32

/* Tick per Second */
//#define RT_TICK_PER_SECOND	    1000

/* SECTION: RT_DEBUG */
/* Thread Debug */
#define RT_DEBUG
#define RT_THREAD_DEBUG

#define RT_USING_OVERFLOW_CHECK

/* Using Hook */
#define RT_USING_HOOK

/* Using Software Timer */
/* #define RT_USING_TIMER_SOFT */
#define RT_USING_SEMAPHORE

/* SECTION: IPC */
/* Using Semaphore*/


/* Using Mutex */


/* Using Event */


/* Using MailBox */


/* Using Message Queue */


/* SECTION: Memory Management */
/* Using Memory Pool Management*/
//#define RT_USING_MEMPOOL

/* Using Dynamic Heap Management */
#define RT_USING_HEAP

/* Using Small MM */
//#define RT_USING_SMALL_MEM
#define RT_USING_SLAB
/* SECTION: Device System */
/* Using Device System */
//#define RT_USING_DEVICE

/* SECTION: Console options */

/* the buffer size of console*/


/* SECTION: finsh, a C-Express shell */


/* SECTION: device filesystem */


/* SECTION: lwip, a lighwight TCP/IP protocol stack */
//#define RT_USING_LWIP
/* LwIP uses RT-Thread Memory Management */


/* SECTION: RT-Thread/GUI */
/* #define RT_USING_RTGUI */


/* image support */
/* #define RTGUI_IMAGE_XPM */
/* #define RTGUI_IMAGE_BMP */

#endif
