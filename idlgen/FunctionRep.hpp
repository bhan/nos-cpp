#ifndef _FUNCTION_REP_H
#define _FUNCTION_REP_H

#include <string>
#include <vector>

class FunctionRep {
public:
    FunctionRep(std::string name, bool constructor);
    ~FunctionRep();

    void addArgmument(std::string type, std::string name);
    void setReturnType(std::string retType);

    bool isConstructor() const;
    const std::string getReturnType() const;
    const std::string getName() const;
    const std::string getArgNames() const;
    const std::string getArgNamesWithTypes() const;

private:
    std::string retType;
    std::string name;
    bool constructor;
    std::vector<std::string> argTypes;
    std::vector<std::string> argNames;
};

std::ostream& operator<<(std::ostream& os, const FunctionRep& funcRep);

#endif /* _FUNCTION_REP_H */
