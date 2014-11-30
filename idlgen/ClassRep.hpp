#ifndef _CLASS_REP_H
#define _CLASS_REP_H

#include <string>
#include <vector>

class FunctionRep;
class FieldRep;

class ClassRep {
public:
    ClassRep(std::string name);
    ~ClassRep();

    void addFunction(FunctionRep* funcRep);
    void addPublicField(FieldRep* fieldRep);
    void addBaseClass(std::string baseClassName);
    const std::vector<FunctionRep*> getFunctions() const;
    const std::vector<FieldRep*> getPublicFields() const;
    const std::string getName() const;
    bool isNetObj() const;

private:
    std::string name_;
    std::vector<FunctionRep*> functions_;
    std::vector<FieldRep*> public_fields_;
    std::vector<std::string> baseClasses_;

    friend std::ostream& operator<<(std::ostream& os, const ClassRep& classRep);
};


#endif /* _CLASS_REP_H */
