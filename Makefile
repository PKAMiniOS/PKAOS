# Khởi tạo biến cho các công cụ biên dịch: C_compiler, Assembler, Linker
CC = gcc -m32 -ffreestanding -fno-pie -nostdlib -O2 -Wall -Wextra -Iinclude
AS = nasm -f elf32
LD = ld -m elf_i386

# Quét toàn bộ file .c và .s trong thư mục src và các thư mục con
C_SOURCES = $(shell find src -name '*.c')
ASM_SOURCES = $(shell find src -name '*.s')

# Chuyển đổi tên file .c và .s thành file .o
OBJS = $(C_SOURCES:.c=.o) $(ASM_SOURCES:.s=.o)

# Lệnh khi chạy make sẽ tạo file pkaos.iso
all: pkaos.iso

# Sử dụng C_compiler để biên dịch các file .c thành file .o, và assembler để biên dịch các file .s thành file .o
%.o: %.c
	$(CC) -c $< -o $@

%.o: %.s
	$(AS) $< -o $@

# Sử dụng linker để liên kết các file .o thành file thực thi .bin
pkaos.bin: $(OBJS) linker.ld
	$(LD) -T linker.ld -o $@ $(OBJS)

# Đóng gói thành .iso với GRUB
pkaos.iso: pkaos.bin
	mkdir -p isodir/boot/grub
	cp pkaos.bin isodir/boot/pkaos.bin
	echo 'set timeout=0' > isodir/boot/grub/grub.cfg
	echo 'set default=0' >> isodir/boot/grub/grub.cfg
	echo 'menuentry "PKAOS" {' >> isodir/boot/grub/grub.cfg
	echo '  multiboot /boot/pkaos.bin' >> isodir/boot/grub/grub.cfg
	echo '  boot' >> isodir/boot/grub/grub.cfg
	echo '}' >> isodir/boot/grub/grub.cfg
	grub-mkrescue -o pkaos.iso isodir

# Thêm lệnh run để chạy hệ điều hành trong QEMU
run: pkaos.bin
	qemu-system-i386 -m 256 -kernel pkaos.bin
# Thêm lệnh clean để xóa các file tạm và file vừa được biên dịch
clean:
	rm -rf $(OBJS) pkaos.bin pkaos.iso isodir

#Note1: Makefile đọc từ dưới lên trên, thay vì chạy từ trên xuống dưới như bình thường

#Note2: chúng ta sử dụng make file để có thể tự động hóa việc biên dịch và đóng gói hệ điều hành
# 		thay vì việc ngồi gõ từng lệnh một cách thủ công.

#Note3: GRUB có tác dụng nạp hệ điều hành vào bộ nhớ RAM (bắt đầu từ 1MB), bật chế độ 32-bit, ...
# 		và chuyển quyền điều khiển cho nó, giúp chúng ta có thể chạy hđh.

#Note4: set timeout=0: khởi động ngay lập tức
# 		set default=0: chọn mục đầu tiên trong menu làm mặc định
# 		menuentry "PKAOS": tạo một mục trong menu GRUB với tên "PKAOS"
# 		multiboot /boot/pkaos.bin: chỉ định file kernel để GRUB nạp vào bộ nhớ
# 		boot: lệnh để GRUB bắt đầu khởi động hệ điều hành
