typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;
typedef uint pde_t;
struct uproc {
  uint sz;
  int state;
  int pid;
  int parent_pid;
  int killed;
  char name[16];
};
