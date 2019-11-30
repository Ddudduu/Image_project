#ifndef __PICTURE_H__
#define __PICTURE_H__

#include <string>
#define MAX_TYPE_LEN 10

class Picture {
  public:
    Picture();
    Picture(std::string name, std::string type, int x, int y);

    void setName(std::string name);
    void setType(std::string type);
    void setX(int x);
    void setY(int y);

    std::string getName(void);
    std::string getType(void);
    int getX(void);
    int getY(void);

  private:
    std::string name;
    std::string type;
    int x, y;
};

#endif
