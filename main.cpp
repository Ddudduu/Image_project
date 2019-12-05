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
    Picture p = Picture();
    int choice1, choice2, choice3;
    char directory[256];
    std::string theDirectory;

    cout << "경로를 입력해주세요" << endl;
    getline(cin, theDirectory);
    cout << endl;

    cout << "[이미지관리]" << endl
         << "1.정보정렬" << endl
         << "2.디렉토리 분류" << endl
         << "원하는 번호를 입력해주세요: ";
    cin >> choice1;

    strcpy(directory, theDirectory.c_str());
    std::vector<std::string> filelist = list_dir(directory);

    if (choice1 == 1) {
        cout << endl
             << "[정보 정렬]" << endl
             << "1.크기 순" << endl
             << "2.날짜 순" << endl
             << "원하는 번호를 입력해주세요: ";
        cin >> choice2;

        cout << endl << endl;
        if (choice2 == 1) {
            for (int j = 0; j < filelist.size(); j++) {

                if (filelist[j] == "." || filelist[j] == "..")
                    continue;

                std::string dirfilename = theDirectory + "/" + filelist[j];
                char route[256];
                p.getRoute();
                strcpy(route, dirfilename.c_str());

                p.set_size(route);
                p.setName(filelist[j]);
                v.push_back(p);
            }
            cout << "===== 크기 순 정렬 =====" << endl;
            p.sort_size(v);
        }

        else if (choice2 == 2) {

            cout << theDirectory;
            for (int j = 0; j < filelist.size(); j++) {

                if (filelist[j] == "." || filelist[j] == "..")
                    continue;

                std::string dirfilename = theDirectory + "/" + filelist[j];
                char route[256];
                strcpy(route, dirfilename.c_str());

                p.set_time(route);
                p.setName(filelist[j]);
                v.push_back(p);
            }
            cout << "===== 날짜 순 정렬 =====" << endl;
            p.sorted_YMD(v);
        }
    } else if (choice1 == 2) {
        cout << endl
             << "[디렉토리 분류]" << endl
             << "1.해상도 별" << endl
             << "2.날짜 별" << endl
             << "원하는 번호를 입력해주세요: ";
        cin >> choice3;
        cout << endl << endl;
        if (choice3 == 1) {
            for (int j = 0; j < filelist.size(); j++) {

                if (filelist[j] == "." || filelist[j] == "..")
                    continue;

                std::string dirfilename = theDirectory + "/" + filelist[j];

                char route[256];
                strcpy(route, dirfilename.c_str());

                int x = 0;
                int y = 0;
                string the_type = "";
                bool didRun = false;

                didRun = p.GetImageSize(route, &x, &y, &the_type);

                Picture pic(p.getName(), p.getType(), x, y);
                pic.setRoute(route);
                pic.setName(filelist[j]);
                v.push_back(pic);
            }
            p.folder_resolution(theDirectory, v);
            cout << "===== 해상도 별 분류 완료 =====" << endl;
        } else if (choice3 == 2) {
            for (int j = 0; j < filelist.size(); j++) {

                if (filelist[j] == "." || filelist[j] == "..")
                    continue;

                std::string dirfilename = theDirectory + "/" + filelist[j];
                char route[256];
                strcpy(route, dirfilename.c_str());

                p.set_time(route);
                p.setName(filelist[j]);
                v.push_back(p);
            }
            p.folder_YMD(theDirectory, v);
            cout << "===== 날짜 별 분류 완료 =====" << endl;
        }
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
        if (entry->d_type == 4)
            continue;
        filenames.push_back(entry->d_name);
        log++;
    }
    closedir(dir);
    return filenames;
}
