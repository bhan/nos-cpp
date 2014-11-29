#include "ClassRep.hpp"
#include "FunctionRep.hpp"

#include <iostream>
#include <sstream> 

const static char* sNetObj_Class = "class NetObj";

ClassRep::ClassRep(std::string name) : name(name) {
}

ClassRep::~ClassRep() {
}

void ClassRep::addFunction(FunctionRep* funcRep) {
    functions.push_back(funcRep);
}

void ClassRep::addBaseClass(std::string baseClassName) {
    baseClasses.push_back(baseClassName);
}

const std::string ClassRep::getName() const {
    return name;
}

const std::vector<FunctionRep*> ClassRep::getFunctions() const {
    return functions;
}

bool ClassRep::isNetObj() const {
    for (auto &element : baseClasses) {
        if (element == sNetObj_Class) {
            return true;
        }
    }
    return false;
}

std::ostream& operator<<(std::ostream& os, const ClassRep& classRep) {
    os << "Class{" << classRep.getName() << ",isNetObj=" << classRep.isNetObj() << ",functions={";
    for (auto &element : classRep.functions) {
        os << *element << ',';
    }
    os << "},{baseClasses=";
    for (auto &element : classRep.baseClasses) {
        os << element << ',';
    }
    os << '}';
    return os;
}
