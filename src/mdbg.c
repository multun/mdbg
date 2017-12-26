#include <stdlib.h>
#include <unistd.h>
#include <err.h>

#include "tracee.h"
#include "tracer.h"


int trace(int argc, char *argv[])
{
  int cpid = fork();
  if (cpid == -1)
    err(1, "fork failed");
  else if (cpid == 0)
    return tracee(argc - 1, argv + 1);
  else
    return tracer(cpid);
}
