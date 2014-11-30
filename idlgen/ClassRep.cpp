#include "ClassRep.hpp"
#include "FunctionRep.hpp"

#include <iostream>
#include <sstream> 

const static char* sNetObj_Class = "class NetObj";

ClassRep::ClassRep(std::string name) : name_(name) {
}

ClassRep::~ClassRep() {
}

void ClassRep::addFunction(FunctionRep* funcRep) {
    functions_.push_back(funcRep);
}

void ClassRep::addPublicField(FieldRep* fieldRep) {
    public_fields_.push_back(fieldRep);
}

void ClassRep::addBaseClass(std::string baseClassName) {
    baseClasses_.push_back(baseClassName);
}

const std::string ClassRep::getName() const {
    return name_;
}

const std::vector<FunctionRep*> ClassRep::getFunctions() const {
    return functions_;
}

const std::vector<FieldRep*> ClassRep::getPublicFields() const {
    return public_fields_;
}

bool ClassRep::isNetObj() const {
    for (auto &element : baseClasses_) {
        if (element == sNetObj_Class) {
            return true;
        }
    }
    return false;
}

std::ostream& operator<<(std::ostream& os, const ClassRep& classRep) {
    os << "Class{" << classRep.getName() << ",isNetObj=" << classRep.isNetObj() << ",functions={";
    for (auto &element : classRep.functions_) {
        os << *element << ',';
    }
    os << "},{baseClasses=";
    for (auto &element : classRep.baseClasses_) {
        os << element << ',';
    }
    os << '}';
    return os;
}
