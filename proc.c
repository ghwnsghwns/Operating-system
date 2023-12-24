// scheduler function with lottery policy
void
scheduler(void)
{
  struct proc *p;
  struct cpu *c = mycpu();
  c->proc = 0;

  for(;;){
    // Avoid deadlock by ensuring that devices can interrupt.
    intr_on();

    unsigned tcount = 0;                // count for the number of total tickets of processes

    for(p = proc; p < &proc[NPROC]; p++) {
      acquire(&p->lock);
      if(p->state == RUNNABLE) tcount += p->tickets;    // If p->state is RUNNABLE, increment tcount.
      release(&p->lock);
    }

    unsigned winner =  rand() % tcount;          // winner is limited by whole tickets

    if(tcount > 0) {                               // If RUNNABLE processes exist
      unsigned count = 0;                          // count for accumulating tickets.

      for(p=proc; p < &proc[NPROC]; p++) {
        acquire(&p->lock);

        if(p->state == RUNNABLE) {                      // For RUNNABLE state processes
          count += p->tickets;                          // Increment count by tickets number.

          if(count > winner) {                          // If accumulated is greater than winner, increment p->wcount.
            p->wcount ++;
            p->state = RUNNING;                         // Change the state of the process from RUNNABLE to RUNNING.
            c->proc = p;
            swtch(&c->context, &p->context);            // context switching
            c->proc = 0;
            release(&p->lock);
            break;                                      // Lottery done
          }

          else p->lcount ++;                           // If accumulated is less than winner, increment p->ncount.
       }

      release(&p->lock);
      }
    }
  }
}
