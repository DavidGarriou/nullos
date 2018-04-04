
//#include <boot/grub/multiboot.h>

.set ALIGN,                     1<<0
.set MEMINFO,                   1<<1
.set MULTIBOOT_HEADER_MAGIC,    0x1badb002
.set MULTIBOOT_HEADER_FLAGS,    (ALIGN)
.set MULTIBOOT_HEADER_CHECKSUM, -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

.set MULTIBOOT2_HEADER_MAGIC,    0xE85250D6
.set MULTIBOOT2_ARCHITECTURE,    0

.set MULTIBOOT_HEADER_TAG_ADDRESS, 0
.set MULTIBOOT_HEADER_TAG_OPTIONAL, 0
.set MULTIBOOT_HEADER_TAG_END, 0
 /*
Temporaire :
Positionnement des etiquettes qui seront valuées dans le scriptde link.
*/
.set BIDON, 0xDEADBEEF
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
multiboot2_header_start:
.long MULTIBOOT2_HEADER_MAGIC	/* magic */
.long MULTIBOOT2_ARCHITECTURE	/* flags */
.long multiboot2_header_end - multiboot2_header_start/* checksum */
.long 0x100000000 - (0xe85250d6 + 0 + (multiboot2_header_end - multiboot2_header_start))/* header_addr */
multiboot2_header_end:

//multiboot_header:
//.long MULTIBOOT_HEADER_MAGIC		/* magic */
//.long MULTIBOOT_HEADER_FLAGS/* flags */
//.long MULTIBOOT_HEADER_CHECKSUM/* checksum */
//.long 0x3/* header_addr */
address_tag_start:
//	.short MULTIBOOT_HEADER_TAG_ADDRESS
//	.short MULTIBOOT_HEADER_TAG_OPTIONAL
//	.long address_tag_end - address_tag_start
	/*  header_addr */
//	.long   multiboot_header
	/*  load_addr */
//	.long   _start
//	.long   0
	/*  load_end_addr */
//	.long   _edata
//	.long   0
	/*  bss_end_addr */
//	.long   _end
//	.long   0
address_tag_end:
entry_address_tag_start:
//	.short 0xB
//	.short 0xC
//	.long 0xD
	/*  entry_addr */
//	.long 0xE
entry_address_tag_end:
	.short MULTIBOOT_HEADER_TAG_END
	.short 0
	.long 8

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

//	mov #0x2f4b2f4f, 0xb8000

	call kernel_main

	cli
1:
	hlt
	jmp 1b

.size _start,  . - _start

