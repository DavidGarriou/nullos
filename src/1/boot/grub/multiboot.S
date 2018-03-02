
//#include <boot/grub/multiboot.h>

.set ALIGN,                     1<<0
.set MEMINFO,                   1<<1
.set MULTIBOOT_HEADER_MAGIC,    0x1badb002
.set MULTIBOOT_HEADER_FLAGS,    (ALIGN)
.set MULTIBOOT_HEADER_CHECKSUM, -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

/*
Temporaire :
Positionnement des etiquettes qui seront valuées dans le scriptde link.
*/
.set BIDON, 0
.set MULTIBOOT_HEADER_LOAD_ADDR,     BIDON
.set MULTIBOOT_HEADER_LOAD_END_ADDR, BIDON
.set MULTIBOOT_HEADER_BSS_END_ADDR,  BIDON
.set MULTIBOOT_HEADER_ENTRY_ADDR,    BIDON
.set MULTIBOOT_HEADER_MODE_TYPE,     BIDON
.set MULTIBOOT_HEADER_WIDTH,         BIDON
.set MULTIBOOT_HEADER_HEIGHT,        BIDON
.set MULTIBOOT_HEADER_DEPTH,         BIDON

.section .multiboot
.align 4
multiboot_header:
.long MULTIBOOT_HEADER_MAGIC
.long MULTIBOOT_HEADER_FLAGS
.long MULTIBOOT_HEADER_CHECKSUM
//.long 11
/.long multiboot_header
.long MULTIBOOT_HEADER_LOAD_ADDR
.long MULTIBOOT_HEADER_LOAD_END_ADDR
.long MULTIBOOT_HEADER_BSS_END_ADDR
.long MULTIBOOT_HEADER_ENTRY_ADDR
.long MULTIBOOT_HEADER_MODE_TYPE
.long MULTIBOOT_HEADER_WIDTH
.long MULTIBOOT_HEADER_HEIGHT
.long MULTIBOOT_HEADER_DEPTH

.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

.section .text
.global _start
.type _start, @function
_start:
	mov $stack_top, %esp

	pushl $0
	popf

	/* Push the magic and the address on the stack, so that they
	will be the parameters of the cmain function */
	pushl %ebx
	pushl %eax

	call kernel_main

	cli
1:
	hlt
	jmp 1b

.size _start,  . - _start

