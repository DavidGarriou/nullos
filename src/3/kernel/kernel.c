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
 
/* Check if the compiler thinks we are targeting the wrong operating
 * system. */
//#if defined(__linux__)
//#error "You are not using a cross-compiler, you will most certainly run into trouble"
//#endif
 
/* This tutorial will only work for the 32-bit ix86 targets. */
//#if !defined(__i386__)
//#error "This tutorial needs to be compiled with a ix86-elf compiler"
//#endif
 
/*  Macros. */

/*  Some screen stuff. */
/*  The number of columns. */
#define COLUMNS                 80
/*  The number of lines. */
#define LINES                   24
/*  The attribute of an character. */
#define ATTRIBUTE               7
/*  The video memory address. */
#define VIDEO                   0xB8000

/*  Variables. */
/*  Save the X position. */
static int xpos;
/*  Save the Y position. */
static int ypos;
/*  Point to the video memory. */
static volatile unsigned char *video;
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
 
size_t strlen(const char* str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}
 
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


/*int itoa(int num, unsigned char* str, int len, int base)
{
	int sum = num;
	int i = 0;
	int digit;
	char c;

	  terminal_putchar('<');
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
	  terminal_putchar(c);
	} while (sum && (i < (len - 1)));

	if (i == (len - 1) && sum)
		return -1;

	str[i] = '\0';
	str = strrev(str);
	  terminal_putchar('>');

	return 0;
}
*/
/*  Clear the screen and initialize VIDEO, XPOS and YPOS. */
static void
cls (void)
{
  int i;

  video = (unsigned char *) VIDEO;

  for (i = 0; i < COLUMNS * LINES * 2; i++)
    *(video + i) = 0;

  xpos = 0;
  ypos = 0;
}

/*  Convert the integer D to a string and save the string in BUF. If
   BASE is equal to 'd', interpret that D is decimal, and if BASE is
   equal to 'x', interpret that D is hexadecimal. */
static void
itoa (char *buf, int base, int d)
{
  char *p = buf;
  char *p1, *p2;
  unsigned long ud = d;
  int divisor = 10;

  /*  If %d is specified and D is minus, put `-' in the head. */
  if (base == 'd' && d < 0)
    {
      *p++ = '-';
      buf++;
      ud = -d;
    }
  else if (base == 'x')
    divisor = 16;

  /*  Divide UD by DIVISOR until UD == 0. */
  do
    {
      int remainder = ud % divisor;

      *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
    }
  while (ud /= divisor);

  /*  Terminate BUF. */
  *p = 0;

  /*  Reverse BUF. */
  p1 = buf;
  p2 = p - 1;
  while (p1 < p2)
    {
      char tmp = *p1;
      *p1 = *p2;
      *p2 = tmp;
      p1++;
      p2--;
    }
}

/*  Put the character C on the screen. */
static void
putchar (int c)
{
  if (c == '\n' || c == '\r')
    {
    newline:
      xpos = 0;
      ypos++;
      if (ypos >= LINES)
        ypos = 0;
      return;
    }

  *(video + (xpos + ypos * COLUMNS) * 2) = c & 0xFF;
  *(video + (xpos + ypos * COLUMNS) * 2 + 1) = ATTRIBUTE;

  xpos++;
  if (xpos >= COLUMNS)
    goto newline;
}

/*  Format a string and print it on the screen, just like the libc
   function printf. */
void
printf (const char *format, ...)
{
  char **arg = (char **) &format;
  int c;
  char buf[20];

  arg++;

  while ((c = *format++) != 0)
    {
      if (c != '%')
        putchar (c);
      else
        {
          char *p, *p2;
          int pad0 = 0, pad = 0;

          c = *format++;
          if (c == '0')
            {
              pad0 = 1;
              c = *format++;
            }

          if (c >= '0' && c <= '9')
            {
              pad = c - '0';
              c = *format++;
            }

          switch (c)
            {
            case 'd':
            case 'u':
            case 'x':
              itoa (buf, c, *((int *) arg++));
              p = buf;
              goto string;
              break;

            case 's':
              p = *arg++;
              if (! p)
                p = "(null)";

            string:
              for (p2 = p; *p2; p2++);
              for (; p2 < p + pad; p2++)
                putchar (pad0 ? '0' : ' ');
              while (*p)
                putchar (*p++);
              break;

            default:
              putchar (*((int *) arg++));
              break;
            }
        }
    }
}

#if defined(__cplusplus)
extern "C" /* Use C linkage for kernel_main. */
#endif
void kernel_main(unsigned long bootloader_header_magic, unsigned long bootloader_info_addr) 
{
	struct multiboot_tag *tag;
	unsigned long mbi_size;

	/* Informations fro Grub */
	t_multiboot_information *info_from_bootloader;
	info_from_bootloader= (t_multiboot_information *) bootloader_info_addr;


	/* Initialize terminal interface */
	cls(); 
	/* Newline support is left as an exercise. */
	printf("Hello, kernel World!\n");

//	printf("bootloader_info_addr = ");
//	printf("bootloader_info_addr = %x", bootloader_info_addr);
//	(void)itoa((unsigned)bootloader_info_addr,(unsigned char*)buffer,lengthOfIntString,16);
//	printf(buffer);
//	printf("\n");

	if (bootloader_header_magic == MULTIBOOT_BOOTLOADER_MAGIC) {
		/* Loaded with Grub */
		printf("Informations from Grub\n");
		printf("======================\n");
		if ((bootloader_info_addr & 7) != 0) {
			printf("mbi is not aligned : %x\n", bootloader_info_addr);
			return;
		}
		mbi_size = *(unsigned long *) bootloader_info_addr;
		printf("mbi size : %x\n", mbi_size);
		/* Initialisation du tag avec le premier  tag de la
		 * structure */
		tag = (struct multiboot_tag *) (bootloader_info_addr + 8);
		printf("TAGS=================\n");
		printf("Tag type %x size %x\n", tag->type, tag->size);
		tag = (struct multiboot_tag *) ((multiboot_uint8_t *) tag + (tag->size));
		printf("Tag \n");
		printf("Tag \n");
	} else {
		printf("Boot header unknown: %x\n", bootloader_header_magic);
		printf("MULTIBOOT_BOOTLOADER_MAGIC: %x\n", MULTIBOOT_BOOTLOADER_MAGIC);
		printf("MULTIBOOT_BOOTLOADER_MAGIC: %x\n", MULTIBOOT_BOOTLOADER_MAGIC);
	}
}
