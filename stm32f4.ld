

/* Heap size will be set to this if allocing memmory*/
_HeapSize = 0x2000;

/* linker checks that stack is bigger than this */
_MinStackSize = 0x2000; 

MEMORY
{
   rom (rx)  : org = 0x08000000, len = 1024K
   ram (xrw) : org = 0x10000000, len = 64K
}

SECTIONS
{

   ENTRY(Reset_Handler)

   .isr_vector :
   {
      . = ALIGN(4);
      KEEP(*(.isr_vector)) 
      . = ALIGN(4);
   }>rom

   .text :
   {
      . = ALIGN(4);
      *(.text) 
      *(.text*) 
      *(.rodata)
      *(.rodata*) 
      *(.glue_7) 
      *(.glue_7t)

      KEEP (*(.init))
      KEEP (*(.fini))

      . = ALIGN(4);
      _etext = .; 
      _exit = .;
   }>rom

    /* initialised data */
   .preinit_array :
   {
      PROVIDE_HIDDEN (__preinit_array_start = .);
      KEEP (*(.preinit_array*))
      PROVIDE_HIDDEN (__preinit_array_end = .);
   }>rom

   .init_array :
   {
      PROVIDE_HIDDEN (__init_array_start = .);
      KEEP (*(SORT(.init_array.*)))
      KEEP (*(.init_array*))
      PROVIDE_HIDDEN (__init_array_end = .);
   }>rom

   .fini_array :
   {
      PROVIDE_HIDDEN (__fini_array_start = .);
      KEEP (*(.fini_array*))
      KEEP (*(SORT(.fini_array.*)))
      PROVIDE_HIDDEN (__fini_array_end = .);
   }>rom

   _sidata = .;
   .data : AT ( _sidata )
   {
      . = ALIGN(4);
      _sdata = .; 
      *(.data) 
      *(.data*) 

      . = ALIGN(4);
      _edata = .; 
   }>ram

   . = ALIGN(4);
   .bss :
   {
      _sbss = .; 
      __bss_start__ = _sbss;
      *(.bss)
      *(.bss*)
      *(COMMON)
      . = ALIGN(4);
      _ebss = .; 
      __bss_end__ = _ebss;
   }>ram

   .check_heap :
   { 
      . = ALIGN(4);
      _HEAP_START  = .;
      . = . + _HeapSize;
      . = ALIGN(4);
      _HEAP_MAX = .;
   }>ram

   .check_stack :
   {
      . = ALIGN(4);
      . = . + _MinStackSize;
      . = ALIGN(4);

      _estack = ORIGIN(ram) + LENGTH(ram); 
   }>ram
  
   /DISCARD/ :
   {
      libc.a(*)
      libm.a(*)
      libgcc.a(*)
   }

}
