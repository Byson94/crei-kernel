#pragma once

#define VGA_COLS 80
#define VGA_ROWS 25
#define VGA_SCREEN_SIZE (VGA_COLS * VGA_ROWS)

extern char* current_cmd;
void run_shell();
void scroll_screen();
