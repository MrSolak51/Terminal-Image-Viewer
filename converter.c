//
// Created by mrsolak on 26.03.2026.
//

#include <stdio.h>  // printf, FILE, fopen için en üstte olmalı
#include <stdlib.h> // malloc, free, atoi için

#define STB_IMAGE_IMPLEMENTATION
#include "converter.h"

int convert_to_bin(const char* input_path, int target_w, int target_h) {
    int width, height, channels;

    // stb_image fonksiyonu
    unsigned char *img_data = stbi_load(input_path, &width, &height, &channels, 3);

    if (!img_data) {
        printf("Hata: Resim yuklenemedi! Yol: %s\n", input_path);
        printf("stb_image hatasi: %s\n", stbi_failure_reason());
        return 1;
    }

    FILE *f = fopen("image.bin", "wb");
    if (!f) {
        printf("Hata: Dosya yazma amaciyla acilamadi!\n");
        stbi_image_free(img_data);
        return 1;
    }

    // Algoritma: Nearest Neighbor Resampling
    for (int y = 0; y < target_h; y++) {
        for (int x = 0; x < target_w; x++) {
            int src_x = x * width / target_w;
            int src_y = y * height / target_h;
            int pos = (src_y * width + src_x) * 3;
            fwrite(&img_data[pos], 1, 3, f);
        }
    }

    fclose(f);
    stbi_image_free(img_data);
    printf("Donusturme basarili: %s (%dx%d)\n", "image.bin", target_w, target_h);
    return 0;
}