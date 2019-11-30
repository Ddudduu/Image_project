//#include "Read_Image.hpp"
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <list>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

class Image {
  public:
    Image();
    Image(const char *name, double height, double width);
    bool GetImageSize(const char *name, double *width, double *height);

  private:
    const char *name;
    double height;
    double width;
};

int main(void) {
    list<Image> imgList;
    Image *image;

    for (int i = 1; i < 5; i++) {
        char filename[256];
        sprintf(filename, "%d.jpg", i);

        double height = 0;
        double width = 0;
        bool didRun = false;

        didRun = image->GetImageSize((const char *)filename, &height, &width);
        Image img(filename, height, width);

        cout << "name: " << filename << endl;
        cout << "width:" << width << endl;
        cout << "height: " << height << endl;

        imgList.push_back(img);
        cout << ">>Succesfully added to list!" << endl;
        // strcpy((char *)name, "02.jpg");
    }

    string filepath = "./ImageList.txt";

    int fd = open(filepath.c_str(), O_CREAT | O_APPEND | O_WRONLY, 0644);
    if (fd == -1) {
        perror("open() error!");
        return 1;
    }

    list<Image>::iterator iter;

    for (iter = imgList.begin(); iter != imgList.end(); ++iter) {
        if (write(fd, &(*iter), sizeof(Image)) == -1) {
            perror("write() error!");
            return 2;
        }
    }
    close(fd);

    cout << ">>" << imgList.size()
         << " imgage's info was succesfully saved to the " << filepath << endl;

    return 0;
}

Image::Image() {}

Image::Image(const char *name, double height, double width) {
    this->name = "0";
    this->height = height;
    this->width = width;
}

bool Image::GetImageSize(const char *name, double *width, double *height) {
    FILE *f = fopen(name, "rb");
    if (f == 0)
        return false;
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (len < 24) {
        fclose(f);
        return false;
    }
    cout << name << endl;

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
        *height = (buf[7] << 8) + buf[8];
        *width = (buf[9] << 8) + buf[10];

        return true;
    }

    if (buf[0] == 'G' && buf[1] == 'I' && buf[2] == 'F') {
        *width = buf[6] + (buf[7] << 8);
        *height = buf[8] + (buf[9] << 8);
        return true;
    }

    if (buf[0] == 0x89 && buf[1] == 'P' && buf[2] == 'N' && buf[3] == 'G' &&
        buf[4] == 0x0D && buf[5] == 0x0A && buf[6] == 0x1A && buf[7] == 0x0A &&
        buf[12] == 'I' && buf[13] == 'H' && buf[14] == 'D' && buf[15] == 'R') {
        *width =
            (buf[16] << 24) + (buf[17] << 16) + (buf[18] << 8) + (buf[19] << 0);
        *height =
            (buf[20] << 24) + (buf[21] << 16) + (buf[22] << 8) + (buf[23] << 0);
        return true;
    }

    return false;
}
