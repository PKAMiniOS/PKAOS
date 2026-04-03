#include "pkaos.h"
#define VIDEO_ADDRESS 0xB8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0F

static int cursor_offset = 0;
/* Hàm clear_screen() dùng để:
        - Xóa toàn bộ nội dung trên màn hình VGA text mode
        - Ghi khoảng trắng lên toàn bộ vùng nhớ video
        - Đặt màu chữ là WHITE_ON_BLACK(trắng nền đen)
*/
void clear_screen()
{
    uint8_t *screen = (uint8_t *)VIDEO_ADDRESS;
    for (int i = 0; i < MAX_COLS * MAX_ROWS * 2; i += 2)
    {
        screen[i] = ' ';
        screen[i + 1] = WHITE_ON_BLACK;
    }
    cursor_offset = 0;
}
/* Hàm print dùng để:
            - In chuỗi ký tự ra màn hình tại vị trí con trỏ hiện tại
            - Tự xử lý xuống dòng '\n'
            - Cập nhật vị trí con trỏ sau mỗi ký tự
*/
void print(const char *str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        putchar(str[i]);
        i++;
    }
}

void putchar(char c)
{
    uint8_t *screen = (uint8_t *)VIDEO_ADDRESS;
    if (c == '\n')
    {
        int current_row = (cursor_offset / 2) / MAX_COLS;
        cursor_offset = (current_row + 1) * MAX_COLS * 2;
        return;
    }

    screen[cursor_offset] = c;
    screen[cursor_offset + 1] = WHITE_ON_BLACK;
    cursor_offset += 2;

    if (cursor_offset >= MAX_COLS * MAX_ROWS * 2)
    {
        cursor_offset = 0;
    }
}

void backspace()
{
    if (cursor_offset >= 2)
    {
        cursor_offset -= 2;
        uint8_t *screen = (uint8_t *)VIDEO_ADDRESS;
        screen[cursor_offset] = ' ';
        screen[cursor_offset + 1] = WHITE_ON_BLACK;
    }
}
// Note1: - *screen : dùng con trỏ ở đây do đang làm việc trực
//                  tiếp với vùng nhớ phần cứng
//        - Ta ép kiểu về uint8_t* để thao tác từng byte
//        - screen -> trỏ tới bộ nhớ màn hình

// Note2: - AX_COLS * MAX_ROWS * 2: nhân 2 do mỗi ô = 2 byte
//        - cursor_offset = 0; : reset con trỏ do ta đã xóa màn hình -> con trỏ sẽ phải quay lại từ đầu

// Note3: - while (str[i] != '\0') : vòng lặp chạy cho đến khi gặp ký tự kết thúc chuỗi

/*Note4 : - cursor_offset:
 *            Vị trí hiện tại của con trỏ trong vùng nhớ video.
 *            Mỗi ký tự chiếm 2 byte:
 *                  - 1 byte ký tự
 *                  - 1 byte màu
 */

/* screen:
 *      Con trỏ trỏ đến vùng nhớ video.
 *
 * current_row:
 *      Dòng hiện tại của con trỏ, dùng khi xử lý xuống dòng.
 */

/*NoteDefine: - VIDEO_ADDRESS: 0xB8000 là địa chỉ bộ nhớ của VGA text mode, khi ghi dữ liệu vào vùng địa chỉ này -> chữ sẽ hiện lên màn hình
              - MAX_ROWS, MAX_COLS: do VGA text mode được quy ước là 25x80
              - WHITE_ON_BLACK : Đây là mã màu,0x0F nghĩa là chữ trắng trên nền đen
              - static int cursor_offset = 0: vị trí mặc định của con trỏ là 0(tức hàng 0 cột 0), mỗi lần ghi dữ liệu sẽ cộng thêm 2 byte
*/