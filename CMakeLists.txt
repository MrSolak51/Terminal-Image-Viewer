cmake_minimum_required(VERSION 4.1)
project(Terminal_Image_Viewer)


# Executable dosyasını oluştur
add_executable(Terminal_Image_Viewer main.c
        converter.c
        converter.h)

# KRİTİK EKLENTİ: Matematik kütüphanesini bağla (-lm bayrağı)
target_link_libraries(Terminal_Image_Viewer m)