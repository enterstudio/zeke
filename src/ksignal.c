/**
 *******************************************************************************
 * @file    ksignal.c
 * @author  Olli Vanhoja
 *
 * @brief   Source file for thread Signal Management in kernel.
 *
 *******************************************************************************
 */

#include "sched.h"
#include "timers.h"
#include "ksignal.h"

/**
 * Set signal and wakeup the thread.
 */
int32_t ksignal_threadSignalSet(osThreadId thread_id, int32_t signal)
{
    int32_t prev_signals;
    threadInfo_t * thread = sched_get_pThreadInfo(thread_id);

    if ((thread->flags & SCHED_IN_USE_FLAG) == 0)
        return 0x80000000; /* Error code specified in CMSIS-RTOS */

    prev_signals = thread->signals;

    thread->signals |= signal; /* OR with all signals */

    /* Update event struct */
    thread->event.value.signals = signal; /* Only this signal */
    thread->event.status = osEventSignal;

    if (((thread->flags & SCHED_NO_SIG_FLAG) == 0)
        && ((thread->sig_wait_mask & signal) != 0)) {
        ksignal_threadSignalWaitMaskClear(thread);

        /* Set the signaled thread back into execution */
        sched_thread_set_exec(thread_id);
    }

    return prev_signals;
}

/**
 * Clear signal wait mask of a given thread
 */
void ksignal_threadSignalWaitMaskClear(threadInfo_t * thread)
{
    /* Release wait timeout timer */
    if (thread->wait_tim >= 0) {
        timers_release(thread->wait_tim);
    }

    /* Clear signal wait mask */
    current_thread->sig_wait_mask = 0;
}

/**
 * Clear selected signals
 * @param thread_id Thread id
 * @param signal    Signals to be cleared.
 */
int32_t ksignal_threadSignalClear(osThreadId thread_id, int32_t signal)
{
    int32_t prev_signals;
    threadInfo_t * thread = sched_get_pThreadInfo(thread_id);

    if ((thread->flags & SCHED_IN_USE_FLAG) == 0)
        return 0x80000000; /* Error code specified in CMSIS-RTOS */

    prev_signals = thread->signals;
    thread->signals &= ~(signal & 0x7fffffff);

    return prev_signals;
}

/**
 * Get signals of the currently running thread
 * @return Signals
 */
int32_t ksignal_threadSignalGetCurrent(void)
{
    return current_thread->signals;
}

/**
 * Get signals of a thread
 * @param thread_id Thread id.
 * @return          Signals of the selected thread.
 */
int32_t ksignal_threadSignalGet(osThreadId thread_id)
{
    threadInfo_t * thread = sched_get_pThreadInfo(thread_id);
    return thread->signals;
}

/**
 * Wait for a signal(s)
 * @param signals   Signals that can woke-up the suspended thread.
 * @millisec        Timeout if selected signal is not invoken.
 * @return          Event that triggered resume back to running state.
 */
osEvent * ksignal_threadSignalWait(int32_t signals, uint32_t millisec)
{
    int tim;

    current_thread->event.status = osEventTimeout;

    if (millisec != (uint32_t)osWaitForever) {
        if ((tim = timers_add(current_thread->id, osTimerOnce, millisec)) < 0) {
            /* Timer error will be most likely but not necessarily returned
             * as is. There is however a slight chance of event triggering
             * before control returns back to this thread. It is completely OK
             * to clear this error if that happens. */
            current_thread->event.status = osErrorResource;
        }
        current_thread->wait_tim = tim;
    }

    if (current_thread->event.status != osErrorResource) {
        current_thread->sig_wait_mask = signals;
        sched_thread_sleep_current();
    }

    /* Event status is now timeout but will be changed if any event occurs
     * as event is returned as a pointer. */
    return (osEvent *)(&(current_thread->event));
}
