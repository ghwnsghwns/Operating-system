#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// A xv6-riscv syscall can take up to six arguments.
#define max_args 6

// Print a help message.
void print_help(int argc, char **argv) {
  fprintf(2, "%s <options: pid or S/R/X/Z>%s\n",
             argv[0], argc > 7 ? ": too many args" : "");
}
// Check validation of argv
int isValid(char *argv){
        int flag = 0;
        char *temp = argv;
        while(*temp!='\0'){
                if(!atoi(temp)) {flag = 0; break;}
                else flag = 1;
                temp++;
        }
        return flag;
}

int main(int argc, char **argv) {
  // Print a help message.
  if(argc > 7) { print_help(argc, argv); exit(1); }

  // Argument vector
  int args[max_args];
  memset(args, 0, max_args * sizeof(int));
  
  for (int i=1; i<argc; i++) {
        if(*argv[i] == 'S') *(args+i-1) = -1;
        else if(*argv[i] == 'R') *(args+i-1) = -2;
        else if(*argv[i] == 'X') *(args+i-1) = -3;
        else if(*argv[i] == 'Z') *(args+i-1) = -4;      // Convert char to int
        else {
                if(isValid(argv[i])) *(args+i-1) = atoi(argv[i]);
                else { print_help(argc, argv); exit(1); }       // Error handling for invalid input
        }
}

  // Call the pstate() syscall
  int ret = pstate(args[0], args[1], args[2], args[3], args[4], args[5]);
  if(ret) { fprintf(2, "pstate failed\n"); exit(1); }

  exit(0);
}
