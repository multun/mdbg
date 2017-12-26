#include <stdlib.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>


int tracee(int argc, char *argv[])
{
  if (ptrace(PTRACE_TRACEME) < 0)
  {
    warn("child TRACEME failed");
    return 1;
  }
  puts("starting the child process");
  kill(getpid(), SIGSTOP);
  execvp(argv[0], argv);
  warn("child exec failed");
  return 1;
}
