#ifndef _MEMORY_PAGING_H
#define _MEMORY_PAGING_H

#define ADDR_MASK  0xFFFFFFFFFF000
#define PRESENT    0x1
#define READ_WRITE 0x2

/*
typedef struct {
  u8     xd :  1;
  u16       : 10;
  u8        :  5;
  u64  addr : 40;
  u8        :  4;
  u8     ps :  1;
  u8      d :  1;
  u8      a :  1;
  u8    pcd :  1;
  u8    pwt :  1;
  u8    u_s :  1;
  u8    r_w :  1;
  u8      p :  1;
} table_entry_s;
*/

#endif

