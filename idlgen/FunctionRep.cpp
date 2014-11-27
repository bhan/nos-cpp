#include "FunctionRep.hpp"

#include <iostream>
#include <sstream> 

FunctionRep::FunctionRep(std::string name, bool constructor)
    : name(name), constructor(constructor) {
}

FunctionRep::~FunctionRep() {
}

void FunctionRep::setReturnType(std::string newRetType) {
    retType = newRetType;
}

void FunctionRep::addArgmument(std::string argType, std::string argName) {
    argTypes.push_back(argType);
    argNames.push_back(argName);
}

bool FunctionRep::isConstructor() const {
    return constructor;
}

const std::string FunctionRep::getReturnType() const {
    return retType;
}

const std::string FunctionRep::getName() const {
    return name;
}

const std::string FunctionRep::getArgNames() const {
    std::ostringstream os;
    for (size_t i = 0; i < argNames.size(); ++i) {
        if (i != 0) {
            os << ", ";
        }
        os << argNames[i];
    }
    return os.str();
}

const std::string FunctionRep::getArgNamesWithTypes() const {
    std::ostringstream os;
    for (size_t i = 0; i < argNames.size(); ++i) {
        if (i != 0) {
            os << ", ";
        }
        os << argTypes[i] << " " << argNames[i];
    }
    return os.str();
}

std::ostream& operator<<(std::ostream& os, const FunctionRep& funcRep) {
    if (!funcRep.isConstructor()) {
        os << funcRep.getReturnType() << " ";
    }
    os << funcRep.getName() << "(" << funcRep.getArgNamesWithTypes() << ")";
    return os;
}
