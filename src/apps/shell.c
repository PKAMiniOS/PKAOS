#include "pkaos.h"

#define SHELL_BUFFER_SIZE 128

static char shell_buffer[SHELL_BUFFER_SIZE];
static int shell_len = 0;

typedef enum {
	SHELL_MODE_COMMAND = 0,
	SHELL_MODE_PT2_A,
	SHELL_MODE_PT2_B,
	SHELL_MODE_PT2_C
} shell_mode_t;

static shell_mode_t shell_mode = SHELL_MODE_COMMAND;
static float pt2_a = 0.0f;
static float pt2_b = 0.0f;
static float pt2_c = 0.0f;

static float my_absf(float x) {
	if (x < 0.0f) return -x;
	return x;
}

static int is_zero(float x) {
	return my_absf(x) < 0.0001f;
}

static int parse_float_strict(const char *s, float *out) {
	int i = 0;
	int digit_count = 0;

	while (s[i] == ' ' || s[i] == '\t') i++;

	if (s[i] == '+' || s[i] == '-') i++;

	while (s[i] >= '0' && s[i] <= '9') {
		digit_count++;
		i++;
	}

	if (s[i] == '.') {
		i++;
		while (s[i] >= '0' && s[i] <= '9') {
			digit_count++;
			i++;
		}
	}

	while (s[i] == ' ' || s[i] == '\t') i++;

	if (digit_count == 0 || s[i] != '\0') {
		return 0;
	}

	*out = my_atof(s);
	return 1;
}

static void print_float_line(const char *label, float value) {
	char buf[32];
	print(label);
	ftoa(value, buf, 4);
	print(buf);
	print("\n");
}

static void solve_pt2(float a, float b, float c) {
	float delta;

	if (is_zero(a)) {
		if (is_zero(b)) {
			if (is_zero(c)) {
				print("Vo so nghiem.\n");
			} else {
				print("Vo nghiem.\n");
			}
			return;
		}

		print("Phuong trinh bac 1.\n");
		print_float_line("x = ", -c / b);
		return;
	}

	delta = b * b - 4.0f * a * c;
	print_float_line("Delta = ", delta);

	if (delta < 0.0f) {
		print("Vo nghiem thuc.\n");
		return;
	}

	if (is_zero(delta)) {
		print_float_line("Nghiem kep x = ", -b / (2.0f * a));
		return;
	}

	print_float_line("x1 = ", (-b + my_sqrt(delta)) / (2.0f * a));
	print_float_line("x2 = ", (-b - my_sqrt(delta)) / (2.0f * a));
}

static void shell_prompt() {
	if (shell_mode == SHELL_MODE_COMMAND) {
		print("pkaos> ");
	} else if (shell_mode == SHELL_MODE_PT2_A) {
		print("Nhap a:> ");
	} else if (shell_mode == SHELL_MODE_PT2_B) {
		print("Nhap b:> ");
	} else {
		print("Nhap c:> ");
	}
}

static void shell_execute(const char *line) {
	float value;

	if (shell_mode == SHELL_MODE_PT2_A) {
		if (!parse_float_strict(line, &value)) {
			print("Gia tri a khong hop le. Nhap lai.\n");
			shell_prompt();
			return;
		}

		pt2_a = value;
		shell_mode = SHELL_MODE_PT2_B;
		shell_prompt();
		return;
	}

	if (shell_mode == SHELL_MODE_PT2_B) {
		if (!parse_float_strict(line, &value)) {
			print("Gia tri b khong hop le. Nhap lai.\n");
			shell_prompt();
			return;
		}

		pt2_b = value;
		shell_mode = SHELL_MODE_PT2_C;
		shell_prompt();
		return;
	}

	if (shell_mode == SHELL_MODE_PT2_C) {
		if (!parse_float_strict(line, &value)) {
			print("Gia tri c khong hop le. Nhap lai.\n");
			shell_prompt();
			return;
		}

		pt2_c = value;
		solve_pt2(pt2_a, pt2_b, pt2_c);
		shell_mode = SHELL_MODE_COMMAND;
		return;
	}

	if (line[0] == '\0') {
		return;
	}

	if (strcmp((const uint8_t *)line, (const uint8_t *)"help") == 0) {
		print("Lenh co ban:\n");
		print("  help  : Hien huong dan\n");
		print("  clear : Xoa man hinh\n");
		print("  credit: In danh sach thanh vien\n");
		print("  pt2   : Giai phuong trinh bac 2\n");
		return;
	}

	if (strcmp((const uint8_t *)line, (const uint8_t *)"clear") == 0) {
		clear_screen();
		return;
	}

	if (strcmp((const uint8_t *)line, (const uint8_t *)"pt2") == 0) {
		shell_mode = SHELL_MODE_PT2_A;
		print("Nhap he so cho ax^2 + bx + c = 0\n");
		shell_prompt();
		return;
	}

	if (strcmp((const uint8_t *)line, (const uint8_t *)"credit") == 0) {
		print("Thanh vien nhom 16:\n");
		print("  - Pham Quang Ha (nhom truong)\n");
		print("  - Dao Ba Tuan Ngoc\n");
		print("  - Ngo Tam Ngoc\n");
        print("  - Phan Thi Ngan Quynh\n");
        print("  - Nguyen Huu Quang Long\n");
		return;
	}

	print("Lenh khong hop le. Go 'help' de xem lenh.\n");
}

void shell_init() {
	shell_len = 0;
	shell_buffer[0] = '\0';
	shell_mode = SHELL_MODE_COMMAND;
	print("Go 'help' de xem lenh.\n");
	shell_prompt();
}

void shell_handle_char(char ch) {
	if (shell_len >= SHELL_BUFFER_SIZE - 1) {
		return;
	}

	shell_buffer[shell_len++] = ch;
	shell_buffer[shell_len] = '\0';
	putchar(ch);
}

void shell_handle_backspace() {
	if (shell_len <= 0) {
		return;
	}

	shell_len--;
	shell_buffer[shell_len] = '\0';
	backspace();
}

void shell_handle_enter() {
	putchar('\n');
	shell_execute(shell_buffer);
	shell_len = 0;
	shell_buffer[0] = '\0';
	if (shell_mode == SHELL_MODE_COMMAND) {
		shell_prompt();
	}
}
