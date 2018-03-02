/* Surely you will remove the processor conditionals and this comment
 *    appropriately depending on whether or not you use C++. */
#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif
#include <stddef.h>
#include <stdint.h>
//#include <stdarg.h>
/* Include definitions of the multiboot standard */
#include <boot/grub/multiboot.h>
#include <drivers/klibc.h>

/* Check if the compiler thinks we are targeting the wrong operating
 * system. */
//#if defined(__linux__)
//#error "You are not using a cross-compiler, you will most certainly run into trouble"
//#endif
 
/* This tutorial will only work for the 32-bit ix86 targets. */
//#if !defined(__i386__)
//#error "This tutorial needs to be compiled with a ix86-elf compiler"
//#endif
 
/* Hardware text mode color constants. */
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};
 
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
{
	return fg | bg << 4;
}
 
static inline uint16_t vga_entry(unsigned char uc, uint8_t color) 
{
	return (uint16_t) uc | (uint16_t) color << 8;
}
 
/*size_t strlen(const char* str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}
 */
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
 
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;
 
void terminal_initialize(void) 
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}
 
void terminal_setcolor(uint8_t color) 
{
	terminal_color = color;
}
 
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}
 
void terminal_putchar(char c) 
{
	if (c == '\n') {
		terminal_column = 0;
		terminal_row++;
	} else {
		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
		if (++terminal_column == VGA_WIDTH) {
			terminal_column = 0;
			if (++terminal_row == VGA_HEIGHT)
				terminal_row = 0;
		}
	}
}
 
void terminal_write(const char* data, size_t size) 
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}
 
void terminal_writestring(const char* data) 
{
	terminal_write(data, strlen(data));
}

unsigned char *strrev(unsigned char *str)
{
	unsigned char *p1, *p2;

	if (! str || ! *str)
		return str;
	for (p1 = str, p2 = str + strlen((const char *)str) - 1; p2 > p1; ++p1, --p2)
	{
		*p1 ^= *p2;
		*p2 ^= *p1;
		*p1 ^= *p2;
	}
	return str;
}

/* reverse:  reverse string s in place */
void reverse(char s[])
{
	int i, j;
	char c;

	for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
		c = s[i];
		s[i] = s[j];
		s[j] = c;
	}
}


int itoa(int num, unsigned char* str, int len, int base)
{
	int sum = num;
	int i = 0;
	int digit;
	char c;

	if (len == 0)
		return -1;

	do {
		digit = sum % base;
		if (digit < 0xA) {
			c = '0' + digit;
		} else {
			c = 'A' + digit - 0xA;
		}
		str[i++] = c;
		sum /= base;
	} while (sum && (i < (len - 1)));

	if (i == (len - 1) && sum)
		return -1;

	str[i] = '\0';
	str = strrev(str);

	return 0;
}

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
void kernel_main(unsigned long bootloader_header_magic, unsigned long bootloader_info_addr) 
{
	char buffer[256];
	int lengthOfIntString = 16;
	/* Informations fro Grub */
	t_multiboot_information *info_from_bootloader;
	info_from_bootloader= (t_multiboot_information *) bootloader_info_addr;

	/* Initialize terminal interface */
	terminal_initialize();
 
	/* Newline support is left as an exercise. */
	terminal_writestring("Hello, kernel World!\n");

//	terminal_writestring(formatstring(0,"bootloader_header_magic : %x\n",bootloader_header_magic));
	formatstring(buffer,0,"bootloader_header_magic : \0");
	terminal_writestring(buffer);
/*
	buffer[5]='\0';
	terminal_writestring(buffer);
		terminal_writestring("\n");
	buffer[2]='\0';
	terminal_writestring(buffer);
		terminal_writestring("\n");
	buffer[0]='\0';
	terminal_writestring(buffer);
		terminal_writestring("\n");
*/
	if (bootloader_header_magic == MULTIBOOT_BOOTLOADER_MAGIC) {
		/* Loaded with Grub */
		terminal_writestring("Informations from Grub\n");
		terminal_writestring("======================\n");
		//terminal_writestring("RAM lower: %x\n", (unsigned)info_from_bootloader->mem_lower);
		terminal_writestring("Flags: ");
		(void)itoa((unsigned)info_from_bootloader->flags,(unsigned char*)buffer,4,16);
		terminal_writestring(buffer);
		terminal_writestring("\n");
		terminal_writestring("RAM lower: ");
		(void)itoa(info_from_bootloader->mem_lower,(unsigned char*)buffer,4,16);
		terminal_writestring(buffer);
		terminal_writestring("\n");
		//terminal_writestring("RAM upper: %x\n", (unsigned)info_from_bootloader->mem_upper);
//		terminal_writestring("RAM upper: %x\n", (unsigned)info_from_bootloader->mem_upper);
		terminal_writestring("RAM upper: ");
		(void)itoa(info_from_bootloader->mem_upper,(unsigned char*)buffer,4,16);
		terminal_writestring(buffer);
		terminal_writestring("\n");
		terminal_writestring("Boot device: ");
		(void)itoa(info_from_bootloader->boot_device,(unsigned char*)buffer,lengthOfIntString,16);
		terminal_writestring(buffer);
		terminal_writestring("\n");
/*				sos_x86_videomem_printf(1, 0,
			    SOS_X86_VIDEO_FG_YELLOW | SOS_X86_VIDEO_BG_BLUE,
			    "Welcome From GRUB to %s%c RAM is %dMB (upper mem = 0x%x kB)",
			    "SOS", ',',
			    (unsigned)(mbi->mem_upper >> 10) + 1,
			    (unsigned)mbi->mem_upper);
*/
	} else {
		terminal_writestring("Boot header unknown:");
		(void)itoa(bootloader_header_magic, (unsigned char*)buffer,lengthOfIntString,16);
		terminal_writestring(buffer);
		terminal_writestring("\n");
		terminal_writestring("MULTIBOOT_HEADER_MAGIC:");
		(void)itoa(MULTIBOOT_HEADER_MAGIC, (unsigned char*)buffer,lengthOfIntString,16);
		terminal_writestring(buffer);
		terminal_writestring("\n");
	}
}
