#include "Picture.hpp"
#include <ctime>
#include <iostream>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

using namespace std;

void Picture::print(vector<Picture> v) {
    for (int i = 0; i < v.size(); i++) {

        cout << v[i].getName() << " : " << v[i].get_year() << "/"
             << v[i].get_month() << "/" << v[i].get_date() << endl;
        cout << endl;
    }
}

void Picture::sorted_YMD(vector<Picture> v) // recent order
{
    Picture tmp;

    for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < v.size() - i - 1; j++) {
            if (v[j].get_year() < v[j + 1].get_year()) {
                tmp = v[j];
                v[j] = v[j + 1];
                v[j + 1] = tmp;
            } else if (v[j].get_year() == v[j + 1].get_year()) {
                if (v[j].get_month() < v[j + 1].get_month()) {
                    tmp = v[j];
                    v[j] = v[j + 1];
                    v[j + 1] = tmp;
                } else if (v[j].get_month() == v[j + 1].get_month()) {
                    if (v[j].get_date() < v[j + 1].get_date()) {
                        tmp = v[j];
                        v[j] = v[j + 1];
                        v[j + 1] = tmp;
                    }
                }
            }
        }
    }
    print(v);
}

void Picture::folder_YMD(string path, vector<Picture> v) {
    vector<string> names;

    for (int i = 0; i < v.size(); i++) {
        char dst[100];
        char src[100];
        string fname = to_string(v[i].get_year()) + "_" +
                       to_string(v[i].get_month()) + "_" +
                       to_string(v[i].get_date());
        strcpy(dst, (path + "/" + fname).c_str());
        strcpy(src, (path + "/" + v[i].getName()).c_str());
        int pid = fork();
        if (pid == 0) {
            bool m = true;
            for (int j = 0; j < names.size(); j++) {
                if (names[j] == fname) {
                    m = false;
                } else
                    continue;
            }
            if (m) // same name not exist
            {
                mkdir(dst, 0755);
                execl("/bin/cp", "cp", src, dst, NULL);
                names.push_back(fname);
            } else {
                execl("/bin/cp", "cp", src, dst, NULL);
            }
        } else if (pid == -1) {
            perror("fork error");
            exit(-1);
        } else {
            wait(&pid);
        }
    }
}

void Picture::set_time(const char *pathname) {
    struct stat buf;
    struct tm time;

    int r = stat(pathname, &buf);
    if (r == -1) {
        exit(-1);
    }

    gmtime_r(&buf.st_mtime, &time);
    this->b.year = time.tm_year + 1900;
    this->b.month = time.tm_mon + 1;
    this->b.date = time.tm_mday;
}
int Picture::get_year() { return this->b.year; }
int Picture::get_month() { return this->b.month; }
int Picture::get_date() { return this->b.date; }
