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
#include <unistd.h>
#include <vector>

using namespace std;
bool GetImageSize(const char *fn, int *x, int *y, string *str);
std::vector<std::string> list_dir(const char *path);

int main() {

    vector<Picture> v(10);
    vector<Picture>::iterator it;
    int i;
    std::string theDirectory = "/home/yujin/바탕화면/사과";
    char directory[256];
    strcpy(directory, theDirectory.c_str());
    std::vector<std::string> filelist = list_dir(directory);

    for (int j = 0; j < 90; j++) {

        std::string dirfilename = theDirectory + "/" + filelist[j];
        char route[256];
        strcpy(route, dirfilename.c_str());
        int the_x = 0;
        int the_y = 0;
        bool didRun = false;
        std::string the_type = "";

        didRun = GetImageSize(route, &the_x, &the_y, &the_type);

        std::cout << dirfilename << endl;
        std::cout << the_x << "*" << the_y << endl;
        std::cout << the_type << endl;
        cout << endl;
        Picture p(dirfilename, the_type, the_x, the_y);
        v.push_back(p);
    }

    return 0;
}

std::vector<std::string> list_dir(const char *path) {
    struct dirent *entry;
    DIR *dir = opendir(path);
    int i = 0;
    int log = 0;
    std::vector<std::string> filenames;

    if (dir == NULL) {
        return filenames;
    }

    while ((entry = readdir(dir)) != NULL) {

        filenames.push_back(entry->d_name);
        log++;
    }
    closedir(dir);
    return filenames;
}

bool GetImageSize(const char *fn, int *x, int *y, string *str) {
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
