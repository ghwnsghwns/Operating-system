// New system call added in sysproc.c 
uint64
sys_pstate(void)
{

  struct proc *p;

  int get_args[MAX_ARGS];
  memset(get_args, 0, MAX_ARGS * sizeof(int));

  for(int i=0; i<MAX_ARGS; i++) argint(i, get_args + i); // Get arguments in process trapframe


  // Print the result on console
  printf("PID\tPPID\tState\tRuntime\t  Name\n");

  for(p = proc; p < &proc[NPROC]; p++){         // Loop all proc table
          acquire(&p -> lock);                  // Acquire &p->lock
          int ppid = 0;


          if(*get_args == 0 && p->pid) {                        // get_args is empty
                if(p->parent != 0) ppid = p->parent->pid;

                printf("%d\t%d\t", p->pid, ppid);
                if(p->state == SLEEPING) printf("S\t");
                if(p->state == RUNNABLE) printf("R\t");
                if(p->state == RUNNING) printf("X\t");
                if(p->state == ZOMBIE) printf("Z\t");

                acquire(&tickslock);
                int runtime = ticks - (p->set_t);
                int minute = runtime / 600;
                int sec = (runtime - (600*minute)) / 10;
                int sec_point = (runtime - (600*minute)) % 10;

                printf("%d:%d.%d\t  %s\n", minute, sec, sec_point, p->name);
                release(&tickslock);
          }
          else{                                                 // get_args is not empty
                int stateflag = 0;
                int pidflag = 0;                // flag variables for check matching

                if(p->parent != 0) ppid = p->parent->pid;

                for(int i = 0; i < MAX_ARGS; i++){
                        if(!*(get_args + i) || pidflag || stateflag) break;
                        // If get_args empty or flags valid, break the loop
                        if(p->state == SLEEPING && *(get_args + i) == -1) stateflag = 1;
                        else if(p->state == RUNNABLE && *(get_args + i) == -2) stateflag = 1;
                        else if(p->state == RUNNING && *(get_args + i) == -3) stateflag = 1;
                        else if(p->state == ZOMBIE && *(get_args + i) == -4) stateflag = 1;
                        else if(p->pid == *(get_args + i)) pidflag = 1;

                        if(pidflag || stateflag){               // Print only when pidflag or stateflag are valid
                                printf("%d\t%d\t", p->pid, ppid);
                                if(p->state == SLEEPING) printf("S\t");
                                if(p->state == RUNNABLE) printf("R\t");
                                if(p->state == RUNNING) printf("X\t");
                                if(p->state == ZOMBIE) printf("Z\t");

                                acquire(&tickslock);
                                int runtime = ticks - (p->set_t);
                                int minute = runtime / 600;
                                int sec = (runtime - (600*minute)) / 10;
                                int sec_point = (runtime - (600*minute)) % 10;

                                printf("%d:%d.%d\t  %s\n", minute, sec, sec_point, p->name);
                                release(&tickslock);
                        }
                }
          }
          release(&p -> lock);
  }
  return 0;
}
