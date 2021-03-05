
#ifndef __HEAP_4_H
#define __HEAP_4_H
#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "b3d_types.h"
#include "b3d_config.h"
#define configTOTAL_HEAP_SIZE                   ENGINE_HEAP_SIZE   //stack size
#define configAPPLICATION_ALLOCATED_HEAP        1                             //use customer defined array as heap
#define configUSE_MALLOC_FAILED_HOOK            0                             //using failure callback


#define vTaskSuspendAll()    NULL       
#define xTaskResumeAll()     NULL       

 /* Normal assert() semantics without relying on the provision of an assert.h
 header file. */
#define configASSERT( x ) if( ( x ) == 0 ) {  for( ;; ); }




 
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


//
#ifndef traceMALLOC
#define traceMALLOC( pvAddress, uiSize )
#endif
//
#ifndef traceFREE
#define traceFREE( pvAddress, uiSize )
#endif

#ifndef mtCOVERAGE_TEST_MARKER
#define mtCOVERAGE_TEST_MARKER()
#endif

extern const size_t xHeapStructSize;

/*------------------------------------------------------------------------------------------------- 
Function declaration 
-------------------------------------------------------------------------------------------------*/
void* pvPortMalloc(size_t xWantedSize, dataType_e dType, u16 priority);
void vPortFree(void *pv);
void prvHeapInit(void);
size_t xPortGetFreeHeapSize(void);
size_t xPortGetMinimumEverFreeHeapSize(void);


#ifdef __cplusplus
}
#endif
#endif


