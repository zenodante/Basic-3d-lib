/*
 *	为FreeRTOS中的heap_4移植添加头文件，加入一些移植所需定义和配置选项
 */

#ifndef __HEAP_4_H
#define __HEAP_4_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>


#define configTOTAL_HEAP_SIZE                   ( ( size_t ) ( 437 * 1024 ) )  //堆空间大小配置
#define configAPPLICATION_ALLOCATED_HEAP        1                             //使用用户自定义空间作为堆分配内存
#define configUSE_MALLOC_FAILED_HOOK            0                             //使用malloc失败回调函数


#define vTaskSuspendAll()    NULL       //挂起所有任务
#define xTaskResumeAll()     NULL       //继续所有任务

 /* Normal assert() semantics without relying on the provision of an assert.h
 header file. */
#define configASSERT( x ) if( ( x ) == 0 ) {  for( ;; ); }




 //字节对齐，以这个的倍数分配内存
#define portBYTE_ALIGNMENT			8
#if portBYTE_ALIGNMENT == 8
#define portBYTE_ALIGNMENT_MASK ( 0x0007 )
#endif

#if portBYTE_ALIGNMENT == 4
#define portBYTE_ALIGNMENT_MASK	( 0x0003 )
#endif

#if portBYTE_ALIGNMENT == 2
#define portBYTE_ALIGNMENT_MASK	( 0x0001 )
#endif

#if portBYTE_ALIGNMENT == 1
#define portBYTE_ALIGNMENT_MASK	( 0x0000 )
#endif

#ifndef portBYTE_ALIGNMENT_MASK
#error "Invalid portBYTE_ALIGNMENT definition"
#endif


//malloc追踪
#ifndef traceMALLOC
#define traceMALLOC( pvAddress, uiSize )
#endif
//free追踪
#ifndef traceFREE
#define traceFREE( pvAddress, uiSize )
#endif

#ifndef mtCOVERAGE_TEST_MARKER
#define mtCOVERAGE_TEST_MARKER()
#endif



/* 函数声明 */
void *pvPortMalloc(size_t xWantedSize);
void vPortFree(void *pv);
size_t xPortGetFreeHeapSize(void);
size_t xPortGetMinimumEverFreeHeapSize(void);


#ifdef __cplusplus
}
#endif

#endif
