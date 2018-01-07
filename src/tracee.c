#include <stdlib.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>

#include "tracee.h"

int tracee(__attribute__((unused)) int argc, char *argv[])
{
  if (ptrace(PTRACE_TRACEME) < 0)
      err(1, "child TRACEME failed");

  execvp(argv[0], argv);
  err(1, "child exec failed");
}
