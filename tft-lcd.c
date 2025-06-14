#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/kd.h>
#include <stdbool.h>
#include <termios.h>
#include <time.h>

#define FBDEV_FILE "/dev/fb0"

unsigned long *pfbmap;
int fbfd;
struct fb_var_screeninfo fbinfo;
struct fb_fix_screeninfo fbfix;
int fbWidth, fbHeight;
int line_length;

void fb_close() {
    if (pfbmap)
        munmap(pfbmap, fbWidth * fbHeight * 4);
    if (fbfd >= 0)
        close(fbfd);
}

void fb_clear() {
    for (int y = 0; y < fbHeight; y++) {
        unsigned long *ptr = pfbmap + y * fbWidth;
        for (int x = 0; x < fbWidth; x++) {
            *ptr++ = 0x000000;
        }
    }
}

void draw_game_scene(int car_lane, int carY_offset) {
    fb_clear();

    // ?꾨줈 諛곌꼍
    for (int y = 0; y < fbHeight; y++) {
        for (int x = 0; x < fbWidth; x++) {
            pfbmap[y * fbWidth + x] = 0x404040;
        }
    }

    int lane1 = fbHeight / 3;
    int lane2 = fbHeight * 2 / 3;
    for (int x = 0; x < fbWidth; x += 40) {
        for (int dx = 0; dx < 20 && x + dx < fbWidth; dx++) {
            if (lane1 >= 0 && lane1 < fbHeight)
                pfbmap[lane1 * fbWidth + (x + dx)] = 0xFFFFFF;
            if (lane2 >= 0 && lane2 < fbHeight)
                pfbmap[lane2 * fbWidth + (x + dx)] = 0xFFFFFF;
        }
    }

    int carW = fbHeight / 5;
    int carH = (carW * 2) * 0.8; // ?먮룞李?湲몄씠瑜?議곌툑 以꾩엫
    int carX = fbWidth / 2 - carH / 2;
    int carY;
 
    switch (car_lane) {
        case 0:
            carY = fbHeight / 6 - carW / 2;
            break;
        case 1:
            carY = fbHeight / 2 - carW / 2;
            break;
        case 2:
            carY = fbHeight * 5 / 6 - carW / 2;
            break;
        default:
            carY = fbHeight / 2 - carW / 2;
    }

    carY += carY_offset;
    if (carY < 0) carY = 0;
    if (carY + carW > fbHeight) carY = fbHeight - carW;

    for (int y = carY; y < carY + carW; y++) {
        for (int x = carX; x < carX + carH; x++) {
            if (x >= 0 && x < fbWidth && y >= 0 && y < fbHeight)
                pfbmap[y * fbWidth + x] = 0x000000;
        }
    }
}

int load_bmp(const char *filename, unsigned char **rgbdata, int *width, int *height) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) return -1;

    fseek(fp, 18, SEEK_SET);
    fread(width, sizeof(int), 1, fp);
    fread(height, sizeof(int), 1, fp);
    fseek(fp, 54, SEEK_SET);

    int row_padded = (*width * 3 + 3) & (~3);
    *rgbdata = (unsigned char *)malloc(row_padded * (*height));
    fread(*rgbdata, sizeof(unsigned char), row_padded * (*height), fp);
    fclose(fp);
    return 0;
}

void draw_bmp_image(const char *filename) {
    unsigned char *bmpdata = NULL;
    int bmpW = 0, bmpH = 0;
    if (load_bmp(filename, &bmpdata, &bmpW, &bmpH) != 0) return;

    fb_clear();
    int row_padded = (bmpW * 3 + 3) & (~3);
    for (int y = 0; y < bmpH && y < fbHeight; y++) {
        for (int x = 0; x < bmpW && x < fbWidth; x++) {
            int idx = (bmpH - 1 - y) * row_padded + x * 3;
            unsigned char b = bmpdata[idx];
            unsigned char g = bmpdata[idx + 1];
            unsigned char r = bmpdata[idx + 2];
            pfbmap[y * fbWidth + x] = (r << 16) | (g << 8) | b;
        }
    }
    free(bmpdata);
}

void draw_menu_screen() {
    draw_bmp_image("menu.bmp");
}

void set_nonblocking() {
    struct termios ttystate;
    tcgetattr(STDIN_FILENO, &ttystate);
    ttystate.c_lflag &= ~ICANON;
    ttystate.c_lflag &= ~ECHO;
    ttystate.c_cc[VMIN] = 0;
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
}

void game_loop() {
    set_nonblocking();
    int car_lane = 1;
    int carY_offset = 0;
    while (1) {
        draw_game_scene(car_lane, carY_offset);
        usleep(30000);

        char input = getchar();
        if (input == 'w') carY_offset -= 10;
        else if (input == 's') carY_offset += 10;
        else if (input == 'q') break;
    }
}

int main(int argc, char *argv[]) {
    int conFD = open("/dev/tty0", O_RDWR);
    ioctl(conFD, KDSETMODE, KD_GRAPHICS);
    close(conFD);

    fbfd = open(FBDEV_FILE, O_RDWR);
    if (fbfd < 0) {
        perror("open");
        return -1;
    }

    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &fbinfo) || ioctl(fbfd, FBIOGET_FSCREENINFO, &fbfix)) {
        perror("ioctl");
        fb_close();
        return -1;
    }

    if (fbinfo.bits_per_pixel != 32) {
        fb_close();
        return -1;
    }

    fbWidth = fbinfo.xres;
    fbHeight = fbinfo.yres;
    line_length = fbfix.line_length;

    pfbmap = (unsigned long *)mmap(0, fbWidth * fbHeight * 4, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if ((unsigned long)pfbmap == (unsigned long)-1) {
        perror("mmap");
        fb_close();
        return -1;
    }

    draw_bmp_image("last.bmp");
    sleep(2);

    while (1) {
        draw_menu_screen();
        char input = getchar();
        getchar();

        switch (input) {
            case '1':
                draw_bmp_image("game_start.bmp");
                while (1) {
                    char subinput = getchar();
                    getchar();
                    if (subinput == 's') {
                        game_loop();
                        break;
                    }
                }
                break;
            case '2':
                draw_bmp_image("setting.bmp");
                sleep(2);
                break;
            case '3':
                draw_bmp_image("leaderboard.bmp");
                sleep(2);
                break;
            case '4':
                goto quit;
            default:
                break;
        }
    }

quit:
    fb_clear();
    fb_close();
    return 0;
}