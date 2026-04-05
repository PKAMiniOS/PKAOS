# Báo Cáo Tìm Hiểu Cấu Trúc Hệ Điều Hành PKAOS
**Thực hiện:** Phạm Quang Hà (Nhóm trưởng)

## Tổng Quan Về Hệ Điều Hành PKAOS

PKAOS là một hệ điều hành thực hành quy mô nhỏ (mini-OS) được xây dựng từ con số 0 (from scratch). Hệ điều hành được thiết kế dạng 32-bit (chuẩn x86), hoạt động độc lập hoàn toàn ở môi trường máy trần (bare-metal) thông qua bộ khởi động GRUB. Hệ thống chạy ở chế độ văn bản (VGA Text Mode), thao tác trực tiếp qua Command-line Shell với các tiện ích tích hợp, đặc biệt nổi bật là module **Giải phương trình bậc 2**.

## Tổng Quan Cấu Trúc Thư Mục & Nguyên Lý Thiết Kế

Một hệ điều hành là một dự án phần mềm khổng lồ. Để dễ quản lý, nâng cấp và phân chia công việc, cấu trúc source code thường phải áp dụng nguyên tắc **phân rã module (Modularity)** và **chia tách mối quan tâm (Separation of Concerns)**. 

Dưới đây là cấu trúc cây thư mục của hệ điều hành PKAOS:

```text
PKAOS/
├── Makefile       <-- File kịch bản của công cụ make, tự động hóa toàn bộ việc biên dịch hệ thống.
├── linker.ld      <-- Kịch bản của trình liên kết (Linker), quy hoạch bản đồ bộ nhớ lúc OS khởi động.
├── include/       <-- Thư mục chứa các định nghĩa chung dùng cho toàn dự án.
│   └── pkaos.h    <-- File Header kết nối các khối phần mềm và thiết bị (hardware-software interface).
└── src/           <-- Thư mục chứa mã nguồn chính, chia làm nhiều tầng:
    ├── apps/      <-- Tầng ứng dụng người dùng (Application). Chứa: shell.c (giao diện command line).
    ├── boot/      <-- Tầng mồi khởi động (Boot). Chứa assembler boot.s để tương tác với GRUB.
    ├── cpu/       <-- Tầng xử lý sâu bên trong vi xử lý kiến trúc x86 (GDT/IDT/ISR ngắt).
    ├── drivers/   <-- Tầng điều khiển giao tiếp thiết bị phần cứng (bàn phím, màn hình VGA).
    ├── kernel/    <-- "Trái tim" OS (Kernel Space). Chứa kernel.c làm vị trí xuất phát để điều phối hệ thống.
    └── lib/       <-- Tầng thư viện tiêu chuẩn dùng chung. Chứa xử lý toán học (fpu.c), xử lý chuỗi (string.c).
```

### Tại Sao Lại Thiết Kế Và Phân Trí Các File Này Như Vậy?

Sự phân hóa thành các block này đem lại lợi thế cực lớn trong quá trình xây dựng hệ điều hành:
1. **Dễ đọc, Dễ chia việc & Dễ cô lập lỗi**: Phần giao tiếp phần cứng rối rắm nằm toàn bộ ở `drivers/` và `cpu/`. Nhờ đó, ứng dụng của người dùng như file `shell.c` (đặt vuông vức tại cục `apps/`) chỉ việc dùng lại thủ tục nhập xuất mà không nhất thiết phải hiểu mã Scan Code bàn phím hoạt động thế nào.
2. **Ngăn chặn xung đột (Conflict) và lặp code**: Cấu trúc khai báo nằm tập trung ở trung tâm `include/pkaos.h`. Điều đó cho phép cả hệ thống dùng chung các khai báo mà không phải copy-paste nhiều nơi.
3. **Quản lý quy trình biên dịch chuyên nghiệp**: File `Makefile` nằm ở thư mục ngoài gốc (Root), tách biệt tư duy "Build" (sản xuất) khỏi tư duy "Code" (phát triển). `Makefile` sẽ bao quát toàn bộ source code `src/` và làm việc phối hợp với luật cấp phát không gian `linker.ld` để tạo ra kết quả boot.

--- 

## Chi Tiết Chức Năng 4 Thành Phần Cốt Lõi

Dưới đây là phân tích chuyên sâu về cách hoạt động và luồng xử lý của file/thành phần bạn đảm nhận: `Makefile`, `linker.ld`, `pkaos.h`, và `shell.c`.

---

## 1. File `Makefile`
**Đóng vai trò như "Giám đốc thi công", tự động hóa hoàn toàn việc dịch và đóng gói OS.**

- **Công việc chính:** Thay vì phải gõ hàng chục lệnh biên dịch thủ công với GCC, NASM, và LD, `Makefile` sẽ tự động tìm kiếm toàn bộ các file `.c` và `.s` trong thư mục `src/`, dịch chúng thành file đối tượng (`.o`).
- **Cách hoạt động:**
  1. Sử dụng trình biên dịch chéo (`gcc -m32 -ffreestanding`) để đảm bảo code C không phụ thuộc vào hệ điều hành đang dùng (như Windows / Linux) mà chỉ dịch thành ngôn ngữ máy trần (bare-metal) chuẩn x32.
  2. Gộp các file `.o` thành một file thực thi duy nhất là `pkaos.bin` (thông qua Linker).
  3. Sử dụng công cụ `grub-mkrescue` để bọc `pkaos.bin` vào trong một file ảnh CD-ROM là `pkaos.iso` có khả năng boot.
- **Workflow:** Gõ `make run`, `Makefile` sẽ build ra ISO và tự động kích hoạt máy ảo `qemu-system-i386` để chạy thử OS. Gõ `make clean` để dọn dẹp file rác.

---

## 2. File `linker.ld` (Linker Script)
**Đóng vai trò "Quy hoạch bộ nhớ" cho hệ điều hành khi máy tính vừa khởi động.**

- **Công việc chính:** Máy tính khi bật lên không biết phải nạp code của hệ điều hành vào đâu. File Linker Script có nhiệm vụ chỉ định quy tắc sắp đặt các vùng nhớ cốt lõi.
- **Cách hoạt động:**
  - Quy định định dạng file biên dịch là `elf32-i386` để GRUB Bootloader có thể đọc được dữ liệu.
  - Phân lô bộ nhớ vật lý: Chỉ đạo nạp kernel bắt đầu từ vị trí **1MB** (`. = 1M;`) trong RAM.
  - Phân vùng mã thành từng khối 4KB (chuẩn của phân trang RAM):
    - `.text`: Vùng chứa toàn bộ các lệnh logic (mã thực thi).
    - `.rodata`: Chứa dữ liệu chỉ đọc (VD: Các chuỗi string cố định như `"pkaos> "`).
    - `.data`: Chứa các biến toàn cục đã khởi tạo (VD: `int x = 1;`).
    - `.bss`: Chứa các biến động / cục bộ / mảng trống chưa khởi tạo (nó sẽ được làm sạch khi khởi động).

---

## 3. File `pkaos.h` (Thư viện Header Cốt Lõi)
**Đóng vai trò "Từ điểm chung" quy chuẩn cấu trúc toàn hệ thống.**

- **Công việc chính:** Đây là mạch máu liên kết giữa phần cứng (CPU, RAM, Text Mode của VGA) và phần mềm (Các hàm điều khiển màn hình, Shell, Toán học).
- **Cách hoạt động:**
  - Định nghĩa chuẩn các kiểu biến (VD: `uint8_t`, `uint32_t`) vì ở môi trường kernel trần không có `<stdint.h>`.
  - Khai báo các cổng giao tiếp phần cứng thấp `inb`, `outb` (dùng để tương tác với bàn phím và con trỏ màn hình).
  - Quy định cấu trúc bộ nhớ mô tả cốt lõi của CPU (GDT - Global Descriptor Table và IDT - Interrupt Descriptor Table) để cho phép OS nhảy vào cấu trúc 32-bit và xử lý các ngắt (Interrupt).
  - Có các function signature (tên hàm chuẩn) của thư viện thao tác toán (`my_sqrt`), thư viện xử lý chuỗi và thư viện vẽ text (`clear_screen`, `print`). 

---

## 4. File `shell.c` (Giao Diện Trực Quan Của Hệ Điều Hành)
**Đóng vai trò "Lễ tân", là chương trình tương tác trực tiếp với người sử dụng hệ điều hành.**

- **Công việc chính:** Ghi nhận tín hiệu đánh phím từ người dùng, lưu vào bộ nhớ đệm (buffer), cho phép thao tác sửa-xóa và đọc lệnh kết quả thi hành, bao gồm ứng dụng "Giải phương trình bậc 2" (`pt2`).
- **Cách hoạt động & Phân tích tính năng pt2:**
  - Shell hoạt động theo mô hình State-Machine (Máy trạng thái) qua biến enum biến thiên là `shell_mode` (gồm chế độ gõ COMMAND mặc định, nhập A, nhập B và nhập C).
  - Khi gõ `pt2`, biến trạng thái sẽ nhảy sang nhập a. Lúc này người dùng gõ chuỗi (`1.5`, `-3`) và hàm `parse_float_strict` sẽ bóc dỡ text char thành biến số thực Float (dùng Logic Ascii tự chế vì OS không có `<stdlib.h>`).
  - Lặp lại với hệ số B và C.
  - Khi lấy đủ C, hàm `solve_pt2()` sẽ tính toán `Delta` và căn bậc hai (`my_sqrt()`) để in kết quả ra thông qua hàm điều khiển màn hình `ftoa()`.
  
---

## TỔNG KẾT WORFKOW HỆ THỐNG
1. **[Build]** Lệnh `make` kích hoạt `Makefile`, dùng `linker.ld` tạo file `pkaos.iso`.
2. **[Khởi tạo]** Virtual Machine nạp kernel tại `1MB`, quy chuẩn kiến trúc ngắt và bảng mô tả dựa trên `pkaos.h`.
3. **[Hiển thị & Nghe ngóng]** Chức năng `shell_init()` được gọi để vẽ giao diện Command prompt.
4. **[Phản ứng ngắt]** Khi gõ phím, phần cứng (IRQ1) truyền ký tự gọi tới `shell_handle_char()`.
5. **[Thực thi]** Khi gõ phím Enter (`shell_handle_enter()`), câu lệnh hoàn chỉnh trong buffer sẽ được đem đi so khớp (`strcmp`). Nếu gõ `pt2`, bộ Shell sẽ chuyển chế độ thành ứng dụng toán học thu hẹp, hoàn tất bài toán căn bậc hai và lại quay về nhận lệnh bình thường.

---

## Bảng Phân Công Nhiệm Vụ

Dưới đây là chi tiết phân công công việc của các thành viên trong nhóm để hoàn thiện cấu trúc PKAOS:

- **Phạm Quang Hà (Nhóm trưởng):** Quản lý cấu trúc build, quy hoạch bộ nhớ và giao diện shell tổng (`Makefile`, `linker.ld`, `pkaos.h`, `shell.c`). Chịu trách nhiệm nắm bắt luồng xử lý toàn hệ thống và viết báo cáo chung.
- **Đào Bá Tuấn Ngọc:** Chịu trách nhiệm thiết lập tầng mồi khởi động (`boot.s`) tương tác với GRUB và xử lý logic bắt ký tự nhập từ bàn phím.
- **Nguyễn Hữu Quang Long:** Viết và đảm nhận toàn bộ thành phần liên quan đến Ngắt (Interrupts), thiết lập IDT và cơ chế định tuyến lỗi CPU.
- **Ngọ Tâm Ngọc:** Chịu trách nhiệm lập trình Driver hiển thị màn hình (VGA Text Mode), cùng với các thư viện tính toán dấu phẩy động (`fpu.c`) và xử lý chuỗi (`string.c`).
- **Phan Thị Ngân Quỳnh:** Định nghĩa các cổng I/O phần cứng (`ports.c`), thiết kế tính năng cuộn màn hình khi văn bản tràn hình, quản trị bộ nhớ hệ thống (`memory.c`), và chịu trách nhiệm **chạy nghiệm thu (test) file .iso hệ điều hành thực tế trên máy ảo VirtualBox**.
