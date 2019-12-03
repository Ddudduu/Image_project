#include "Picture.hpp"
#include <string.h>
#include <string>

Picture::Picture() {
    this->name = "";
    this->type = "";
    this->x = -1;
    this->y = -1;
}

Picture::Picture(std::string name, std::string type, int x, int y) {
    this->name = name;
    this->type = type;
    this->x = x;
    this->y = y;
}

void Picture::setName(std::string name) { this->name = name; }

void Picture::setType(std::string type) { this->type = type; }

void Picture::setRoute(std::string route) { this->route = route; }

std::string Picture::getName(void) { return this->name; }

std::string Picture::getType(void) { return this->type; }

std::string Picture::getRoute(void) { return this->route; }
