#ifndef _CLASS_REP_H
#define _CLASS_REP_H

#include <string>
#include <vector>

class FunctionRep;

class ClassRep {
public:
    ClassRep(std::string name);
    ~ClassRep();

    void addFunction(FunctionRep* funcRep);
    void addBaseClass(std::string baseClassName);
    const std::string getName() const;

private:
    std::string name;
    std::vector<FunctionRep*> functions;
    std::vector<std::string> baseClasses;

    friend std::ostream& operator<<(std::ostream& os, const ClassRep& classRep);
};


#endif /* _CLASS_REP_H */
