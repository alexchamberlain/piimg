#include "command.h"
#include <string.h>

int run_argv(struct cmd_struct *cmds, int cmds_n, int argc, char ** argv) {
  const char *cmd = argv[0];
  int i;

  ++argv;
  --argc;
  
  for(i = 0; i < cmds_n; ++i) {
    struct cmd_struct *p = cmds+i;
    if(strcmp(p->cmd, cmd))
      continue;

    return p->fn(argc, argv);
  }

  return 1;
}

