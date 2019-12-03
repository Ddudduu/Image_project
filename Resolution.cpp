#include "Picture.hpp"
#include <dirent.h>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

void Picture::setX(int x) { this->x = x; }

void Picture::setY(int y) { this->y = y; }

int Picture::getX(void) { return this->x; }

int Picture::getY(void) { return this->y; }

void Picture::folder_resolution(string path, vector<Picture> v) {
    char dst1[100];
    char dst2[100];
    char dst3[100];
    strcpy(dst1, (path + "/" + "정방형 사진").c_str());
    strcpy(dst2, (path + "/" + "가로방향 사진").c_str());
    strcpy(dst3, (path + "/" + "세로방향 사진").c_str());
    mkdir(dst1, 0755);
    mkdir(dst2, 0755);
    mkdir(dst3, 0755);

    for (int i = 0; i < v.size(); i++) {

        char src[100];
        strcpy(src, (v[i].getName()).c_str());
        int pid = fork();
        if (pid == 0) {
            if (v[i].getX() == v[i].getY()) { //정방형
                execl("/bin/cp", "cp", src, dst1, NULL);
            } else if (v[i].getX() > v[i].getY()) { //가로
                execl("/bin/cp", "cp", src, dst2, NULL);
            }

            else if (v[i].getX() < v[i].getY()) { //세로
                execl("/bin/cp", "cp", src, dst3, NULL);
            }
        } else if (pid == -1) {
            perror("fork error");
            exit(-1);
        } else {
            wait(&pid);
            continue;
        }
    }
}

bool Picture::GetImageSize(const char *fn, int *x, int *y, string *str) {
    FILE *f = fopen(fn, "rb");
    if (f == 0)
        return false;
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (len < 24) {
        fclose(f);
        return false;
    }
    unsigned char buf[24];
    fread(buf, 1, 24, f);

    if (buf[0] == 0xFF && buf[1] == 0xD8 && buf[2] == 0xFF && buf[3] == 0xE0 &&
        buf[6] == 'J' && buf[7] == 'F' && buf[8] == 'I' && buf[9] == 'F') {
        long pos = 2;
        while (buf[2] == 0xFF) {
            if (buf[3] == 0xC0 || buf[3] == 0xC1 || buf[3] == 0xC2 ||
                buf[3] == 0xC3 || buf[3] == 0xC9 || buf[3] == 0xCA ||
                buf[3] == 0xCB)
                break;
            pos += 2 + (buf[4] << 8) + buf[5];
            if (pos + 12 > len)
                break;
            fseek(f, pos, SEEK_SET);
            fread(buf + 2, 1, 12, f);
        }
    }

    fclose(f);

    if (buf[0] == 0xFF && buf[1] == 0xD8 && buf[2] == 0xFF) {
        *y = (buf[7] << 8) + buf[8];
        *x = (buf[9] << 8) + buf[10];
        *str = "JPEG";
        return true;
    }

    if (buf[0] == 'G' && buf[1] == 'I' && buf[2] == 'F') {
        *x = buf[6] + (buf[7] << 8);
        *y = buf[8] + (buf[9] << 8);
        *str = "GIF";
        return true;
    }

    if (buf[0] == 0x89 && buf[1] == 'P' && buf[2] == 'N' && buf[3] == 'G' &&
        buf[4] == 0x0D && buf[5] == 0x0A && buf[6] == 0x1A && buf[7] == 0x0A &&
        buf[12] == 'I' && buf[13] == 'H' && buf[14] == 'D' && buf[15] == 'R') {
        *x =
            (buf[16] << 24) + (buf[17] << 16) + (buf[18] << 8) + (buf[19] << 0);
        *y =
            (buf[20] << 24) + (buf[21] << 16) + (buf[22] << 8) + (buf[23] << 0);
        *str = "PNG";
        return true;
    }

    return false;
}
