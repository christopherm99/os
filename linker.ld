OUTPUT_FORMAT(elf64-x86-64)
OUTPUT_ARCH(i386:x86-64)

ENTRY(_start)

PHDRS {
  requests PT_LOAD    FLAGS(6); /* RW- */
  text     PT_LOAD    FLAGS(5); /* R-E */
  rodata   PT_LOAD    FLAGS(4); /* R-- */
  data     PT_LOAD    FLAGS(6); /* RW- */
  dynamic  PT_DYNAMIC FLAGS(6); /* DYNAMIC */
}

SECTIONS {
  . = 0xffffffff80000000; /* Limine spec requires that we use the top 2GiB */

  /* Limine requests */
  .requests : {
    KEEP(*(.requests_start_marker))
    KEEP(*(.requests))
    KEEP(*(.requests_end_marker))
  } :requests

  /* Place .text in next page */
  . += CONSTANT(MAXPAGESIZE);

  .text : {
    *(.text .text.*)
  } :text

  .rodata BLOCK(4K) : ALIGN(4K) {
    *(.rodata .rodata.*)
  } :rodata

  . += CONSTANT(MAXPAGESIZE);

  .data : {
    *(.data .data.*)
  } :data

  .dynamic : {
    *(.dynamic)
  } :data :dynamic

  /* NOTE: .bss MUST be last in :data */
  .bss : {
    *(COMMON)
    *(.bss .bss.*)
  } :data

  /* Discard .note.* and .eh_frame as they may cause issues on some hosts */
  /DISCARD/ : {
    *(.eh_frame)
    *(.note .note.*)
  }
}

