#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "pstat.h"

// return 0 on success, -1 otherwise
int
sys_getpinfo(struct pstat* proc_stats)
{
	argptr(0, (void*)&proc_stats, sizeof(*proc_stats));
	if(proc_stats == 0) return -1; // failure
	for(int i = 0; i < NPROC; i++) {
		// call function in proc.c
		struct proc* current = read_ptable(i);
		// populate pstat
		if(current->state == UNUSED)
			proc_stats->inuse[i] = 0;
		else proc_stats->inuse[i] = 1;
		proc_stats->tickets[i] = current->tickets;
		proc_stats->pid[i] = current->pid;
		proc_stats->ticks[i] = current->ticks; // not sure incrementing ticks is correct
	}
	return 0; // success
}

// returns 0 on success, -1 otherwise
int
sys_settickets(int number)
{
    // not sure if current_process will ever be null, but checking just to be certain
    argint(0, &number);
	struct proc *current_process = myproc();
	//cprintf("Trying to give %d %d tickets\n", current_process->pid, number);
    if ((number < 1) | (current_process == 0)) return -1; // failure

    current_process->tickets = number;
    return 0; // success
}

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0) // gets process id? which one?
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
