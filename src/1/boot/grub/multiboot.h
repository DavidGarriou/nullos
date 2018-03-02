
#define MULTIBOOT_HEADER_MAGIC		0x1badb002
/* The magic number passed by a Multiboot-compliant boot loader.  */
#define MULTIBOOT_BOOTLOADER_MAGIC	0x2BADB002

/*
 * 3.3 Boot information format
 * The format of the Multiboot information structure (as defined so far) follows:
 *
 *             +-------------------+
 *     0       | flags             |    (required)
 *             +-------------------+
 *     4       | mem_lower         |    (present if flags[0] is set)
 *     8       | mem_upper         |    (present if flags[0] is set)
 *             +-------------------+
 *     12      | boot_device       |    (present if flags[1] is set)
 *             +-------------------+
 *     16      | cmdline           |    (present if flags[2] is set)
 *             +-------------------+
 *     20      | mods_count        |    (present if flags[3] is set)
 *     24      | mods_addr         |    (present if flags[3] is set)
 *             +-------------------+
 *     28 - 40 | syms              |    (present if flags[4] or
 *             |                   |                flags[5] is set)
 *             +-------------------+
 *     44      | mmap_length       |    (present if flags[6] is set)
 *     48      | mmap_addr         |    (present if flags[6] is set)
 *             +-------------------+
 *     52      | drives_length     |    (present if flags[7] is set)
 *     56      | drives_addr       |    (present if flags[7] is set)
 *             +-------------------+
 *     60      | config_table      |    (present if flags[8] is set)
 *             +-------------------+
 *     64      | boot_loader_name  |    (present if flags[9] is set)
 *             +-------------------+
 *     68      | apm_table         |    (present if flags[10] is set)
 *             +-------------------+
 *     72      | vbe_control_info  |    (present if flags[11] is set)
 *     76      | vbe_mode_info     |
 *     80      | vbe_mode          |
 *     82      | vbe_interface_seg |
 *     84      | vbe_interface_off |
 *     86      | vbe_interface_len |
 *             +-------------------+
 *     88      | framebuffer_addr  |    (present if flags[12] is set)
 *     96      | framebuffer_pitch |
 *     100     | framebuffer_width |
 *     104     | framebuffer_height|
 *     108     | framebuffer_bpp   |
 *     109     | framebuffer_type  |
 *     110-115 | color_info        |
 *             +-------------------+
 *
 *     flags:0  all boot modules loaded along with the operating system must be aligned on page (4KB) boundaries
 *     flags:1  ‘mem_*’ fields of the Multiboot information structure must be included
 *              ‘mmap_*’ fields must be included if the boot can pass a memory map
 *     flags:2   information about the video mode table must be available to the kernel.
 *     flags:16 fields at offsets 12-28 in the Multiboot header are valid
 */

typedef struct multiboot_information {
	unsigned long flags;
	unsigned long mem_lower;
	unsigned long mem_upper;
	unsigned long boot_device;
	unsigned long cmdline;
	unsigned long mods_count;
	unsigned long mods_addr;
	unsigned long syms[3];
	unsigned long mmap_length;
	unsigned long mmap_addr;
	unsigned long drive_length;
	unsigned long drives_addr;
	unsigned long config_table;
	unsigned long boot_loader_name;
	unsigned long apm_table;
	unsigned long vbe_control_info;
	unsigned long vbe_mode_info;
	unsigned long vbe_mode;
	unsigned long vbe_interface_seg;
	unsigned long vbe_interface_off;
	unsigned long vbe_interface_len;
	unsigned long framebuffer_addr;
	unsigned long framebuffer_pitch;
	unsigned long framebuffer_width;
	unsigned long framebuffer_height;
	unsigned char framebuffer_bpp;
	unsigned char framebuffer_type;
	unsigned char color_info[6];
} t_multiboot_information;


