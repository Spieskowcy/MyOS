#ifndef _PROC_H_
#define _PROC_H_

//#include "proc/task.h"

/* Default process timeslice (milliseconds) */
#define SCHED_TIMESLICE     100

extern struct task *current;
extern struct task ktask;

extern int need_resched;


void scheduler(void);

void scheduler_init(void);

/**
 * Process pending (non masked) signals.
 */
int do_signal(void);

/*
 * Arch dependent return preparation from a signal handler.
 */
void sigret_prepare(struct isr_frame *ifr,
                    const struct sigaction *act, int sig);


/*
 * Start init user-mode process.
 */
void init_start(void);

/**
 * Idle procedure.
 * This is executed by the first kernel process (pid=0) after that the
 * init process (pid=1) has been started.
 */
void idle(void);


void proc_dump(void);


#endif /* BEEOS_PROC_H_ */

