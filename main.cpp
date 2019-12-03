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

    vector<Picture> v;
    vector<Picture>::iterator it;
    int i;
    std::string theDirectory = "/home/yujin/바탕화면/사과";
    char directory[256];
    strcpy(directory, theDirectory.c_str());
    std::vector<std::string> filelist = list_dir(directory);

    for (int j = 0; j < filelist.size(); j++) {

        std::string dirfilename = theDirectory + "/" + filelist[j];
        char route[256];
        strcpy(route, dirfilename.c_str());

        Picture p = Picture();
        v.push_back(p);
    }

    return 0;
}

vector<string> list_dir(const char *path) {
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
