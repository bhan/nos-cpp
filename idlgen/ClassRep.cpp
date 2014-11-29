#include "ClassRep.hpp"
#include "FunctionRep.hpp"

#include <iostream>
#include <sstream> 

ClassRep::ClassRep(std::string name) : name(name) {
}

ClassRep::~ClassRep() {
}

void ClassRep::addFunction(FunctionRep* funcRep) {
    functions.push_back(funcRep);
}

const std::string ClassRep::getName() const {
    return name;
}

std::ostream& operator<<(std::ostream& os, const ClassRep& classRep) {
    os << "Class{" << classRep.getName() << ",functions={";
    for (auto &element : classRep.functions) {
        os << *element << ',';
    }
    os << '}';
    return os;
}
