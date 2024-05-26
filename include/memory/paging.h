#ifndef _MEMORY_PAGING_H
#define _MEMORY_PAGING_H

#define NEXT_ENTRY(Ptr) ((table_entry_s *) (((char *) Ptr) + sizeof(table_entry_s)))

typedef struct {
  u8     xd :  1;
  u16 _avl0 : 10;
  u8   _res :  5;
  u64  addr : 40;
  u8  _avl1 :  4;
  u8     ps :  1;
  u8      d :  1;
  u8      a :  1;
  u8    pcd :  1;
  u8    pwt :  1;
  u8    u_s :  1;
  u8    r_w :  1;
  u8      p :  1;
} table_entry_s;

typedef table_entry_s *table_t;

#endif

