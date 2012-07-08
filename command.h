#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

struct cmd_struct {
  const char *cmd;
  int (*fn)(int, char **);
};

int run_argv(struct cmd_struct *cmds, int cmds_n, int argc, char ** argv);
