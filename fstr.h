typedef struct {
  char * c_str;
  int    len;
  int    flags;
} fstr;

#define FSTR_TEMPORARY 1

#define FSTR_DECLARE_WRAPPER(d, s) fstr d = {s, strlen(s), 0}
#define FSTR_DECLARE_NULL(d) fstr d = {NULL, 0, 0}

extern int fstrcat(fstr *, const fstr *, const fstr *);
extern void fstrfree(fstr *);
