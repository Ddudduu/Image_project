#include "Picture.hpp"
#include <iostream>
#include <stdlib.h>
#include <sys/stat.h>
using namespace std;

void Picture::set_size(const char *pathname) {
    struct stat buf;
    int ret = stat(pathname, &buf);

    if (ret < 0) {
        cout << "ERROR!" << endl;
        exit(-1);
    }

    this->size = buf.st_size;
}

int Picture::get_size(void) { return this->size; };

void Picture::sort_size(vector<Picture> v) {

    for (int k = 0; k < v.size(); k++) {
        for (int j = 0; j < v.size() - k - 1; j++) {
            Picture sorted_size;
            if (v[j].get_size() < v[j + 1].get_size()) {
                sorted_size = v[j];
                v[j] = v[j + 1];
                v[j + 1] = sorted_size;
            }
        }
    }
    print_size(v);
}

void Picture::print_size(vector<Picture> v) {

    for (int i = 0; i < v.size(); i++) {
        cout << v[i].get_size() << endl;
    }
}