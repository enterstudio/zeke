/**
 *******************************************************************************
 * @file    sched.h
 * @author  Olli Vanhoja
 * @brief   Kernel scheduler header file for sched.c.
 *******************************************************************************
 */

/** @addtogroup Scheduler
  * @{
  */

#pragma once
#ifndef SCHED_H
#define SCHED_H

#if configDEVSUBSYS == 1
#include "dev.h"
#endif

#include "kernel.h"

#define SCHED_IN_USE_FLAG   0x00000001u /*!< IN USE FLAG */
#define SCHED_EXEC_FLAG     0x00000002u /*!< EXEC = 1 / SLEEP = 0 */
#define SCHED_NO_SIG_FLAG   0x00000004u /*!< Thread cannot be woken up by a signal. */

/* When these flags are both set for a it's ok to make a context switch to it. */
#define SCHED_CSW_OK_FLAGS  (SCHED_EXEC_FLAG | SCHED_IN_USE_FLAG)

#define SCHED_DEV_WAIT_BIT 0x40000000 /*!< Dev wait signal bit. */

extern volatile uint32_t sched_enabled;

/** Thread info struct
 *
 * Process Control Block structure.
 */
typedef struct {
    void * sp;                  /*!< Stack pointer */
    uint32_t flags;             /*!< Status flags */
    int32_t signals;            /*!< Signal flags
                                 * @note signal bit 30 is reserved for dev. */
    int32_t sig_wait_mask;      /*!< Signal wait mask */
#if configDEVSUBSYS == 1
    unsigned int dev_wait;       /*!< Waiting for (major) dev. Currently only
                                  * major number (whole driver) level is used.
                                  * This means that optimal way to use device
                                  * drivers is to lock the whole driver for a
                                  * short period of time. */
#endif
    int wait_tim;               /*!< Reference to a timeout timer */
    osEvent event;              /*!< Event struct */
    osPriority def_priority;    /*!< Thread priority */
    osPriority priority;        /*!< Thread dynamic priority */
    int ts_counter;             /*!< Time slice counter */
    osThreadId id;              /*!< Thread id in task table */
    /**
     * Thread inheritance; Parent and child thread pointers.
     *
     *  inh : Parent and child thread relations
     *  ---------------------------------------
     * + first_child is a parent thread attribute containing address to a first
     *   child of the parent thread
     * + parent is a child thread attribute containing address to a parent
     *   thread of the child thread
     * + next_child is a child thread attribute containing address of a next
     *   child node of the common parent thread
     */
    struct threadInheritance_t {
        void * parent;              /*!< Parent thread */
        void * first_child;         /*!< Link to the first child thread */
        void * next_child;          /*!< Next child of the common parent */
    } inh;
} threadInfo_t;

/* Public function prototypes ------------------------------------------------*/
void sched_init(void);
void sched_start(void);
void sched_handler(void);

void sched_thread_set_exec(int thread_id);
void sched_get_loads(uint32_t loads[3]);
void sched_threadDevSignal(int32_t signal, osDev_t dev);

/* Functions used by syscalls */
osThreadId sched_ThreadCreate(osThreadDef_t * thread_def, void * argument);
osThreadId sched_thread_getId(void);
osStatus sched_thread_terminate(osThreadId thread_id);
osStatus sched_thread_setPriority(osThreadId thread_id, osPriority priority);
osPriority sched_thread_getPriority(osThreadId thread_id);
osStatus sched_threadDelay(uint32_t millisec);
osEvent * sched_threadWait(uint32_t millisec);
int32_t sched_threadSignalSet(osThreadId thread_id, int32_t signal);
void sched_threadSignalWaitMaskClear(void);
int32_t sched_threadSignalClear(osThreadId thread_id, int32_t signal);
int32_t sched_threadSignalGetCurrent(void);
int32_t sched_threadSignalGet(osThreadId thread_id);
#if configDEVSUBSYS == 1
osEvent * sched_threadDevWait(osDev_t dev, uint32_t millisec);
#endif
osEvent * sched_threadSignalWait(int32_t signals, uint32_t millisec);

#endif /* SCHED_H */

/**
  * @}
  */
