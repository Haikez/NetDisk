#include "showProcessBar.h"
void show_progress_bar(double progress) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int bar_length = w.ws_col - 15; // 减去额外字符的长度（如百分比和旋转指示器）
                                    //
    char spinner[] = {'|', '/', '-', '\\'};
    int spinner_index = (int)progress % 4;  // 动态改变旋转指示器

    int fill_length = (int)(progress * bar_length / 100.0);
    int empty_length = bar_length - fill_length;

    printf("\r[");
    for (int i = 0; i < fill_length; ++i) {
        putchar('#');
    }
    for (int i = 0; i < empty_length; ++i) {
        putchar('-');
    }
    printf("] %.2f%% %c", progress, spinner[spinner_index]);
    fflush(stdout);  // 刷新输出缓冲区
}
