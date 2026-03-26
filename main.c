#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "converter.h"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <sys/ioctl.h>
    #include <unistd.h>
#endif

void DrawFromBin(int width, int height) {
    FILE *f = fopen("image.bin", "rb");
    if (!f) {
        printf("Hata: %s acilamadi!\n", "image.bin");
        return;
    }

    // Bir satırı tutacak kadar bellek ayırıyoruz (3 byte * genişlik)
    unsigned char *row1 = (unsigned char*)malloc(width * 3);
    unsigned char *row2 = (unsigned char*)malloc(width * 3);

    printf("\033[H"); // İmleci başa al

    for (int y = 0; y < height; y += 2) {
        // İki satırı aynı anda okuyoruz (Üst ve Alt piksel için)
        fread(row1, 1, width * 3, f);
        fread(row2, 1, width * 3, f);

        for (int x = 0; x < width; x++) {
            // row1[x*3] = R, row1[x*3+1] = G, row1[x*3+2] = B
            printf("\033[38;2;%d;%d;%dm\033[48;2;%d;%d;%dm▀\033[0m",
                   row1[x*3], row1[x*3+1], row1[x*3+2],
                   row2[x*3], row2[x*3+1], row2[x*3+2]);
        }
        printf("\n");
    }

    free(row1);
    free(row2);
    fclose(f);
}

int main(int argc, char** argv) {
#ifdef _WIN32
    SetConsoleOutputCP(65001); // UTF-8 ve Özel Karakter Desteği
#endif

    char input_path[256];

    int term_width, term_height;
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        term_width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        // Her terminal satırı 2 dikey piksel (üst/alt) bastığı için yüksekliği 2 ile çarpıyoruz.
        // -1 pay bırakmak terminal kaymalarını engeller.
        term_height = (csbi.srWindow.Bottom - csbi.srWindow.Top) * 2;
    } else {
        term_width = 80; term_height = 40; // Fallback (Hata durumunda varsayılan)
    }
#else
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
        term_width = w.ws_col;
        // Half-block tekniği kullanacağın için yüksekliği 2 ile çarpıyoruz.
        // Terminalde 1 satır pay bırakmak (w.ws_row - 1) kaymaları önlemek için akılcıdır.
        term_height = (w.ws_row - 1) * 2;
    } else {
        term_width = 80; term_height = 48; // Fallback
    }
#endif

    printf("Give image path:");
    fgets(input_path, 256, stdin);

    // Satır sonu karakterini bul ve NULL (\0) ile değiştir
    input_path[strcspn(input_path, "\n")] = 0;


    // 3. Dönüştürme
    convert_to_bin(input_path, term_width, term_height);

#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    // Fonksiyon parametre sırasına dikkat: (dosya, genişlik, yükseklik)
    DrawFromBin(term_width, term_height);

    if (remove("image.bin") == 0) {
    } else {
        perror("[UYARI] Gecici dosya silinemedi");
    }
    return 0;
}
