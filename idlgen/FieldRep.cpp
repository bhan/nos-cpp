#include <memory>
#include <string>
#include <sstream>
#include "FieldRep.hpp"

const std::string FieldRep::getName() const {
    return name_;
}

std::unique_ptr<std::string> FieldRep::getNameCamelCase() const {
    std::stringstream ss;

    bool uppercaseNext = true;
    for (const char& c : name_) {
        if (c == '_') {
            uppercaseNext = true;
        } else if (uppercaseNext) {
            ss << static_cast<char>(::toupper(c));
            uppercaseNext = false;
        } else {
            ss << c;
        }
    }

    std::unique_ptr<std::string> output(new std::string(ss.str()));
    return move(output);
}

void FieldRep::setType(std::string type) {
    type_ = type;
}

const std::string FieldRep::getType() const {
    return type_;
}
