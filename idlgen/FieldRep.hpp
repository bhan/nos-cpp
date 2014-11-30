#ifndef _FIELD_REP_HPP_
#define _FIELD_REP_HPP_

#include <string>

class FieldRep {
  public:
    explicit FieldRep(std::string name) : name_(name) {}
    ~FieldRep() {}

    const std::string getName() const;
    std::unique_ptr<std::string> getNameCamelCase() const;
    void setType(std::string type);
    const std::string getType() const;

  private:
    std::string name_;
    std::string type_;
};

#endif /* _FIELD_REP_HPP_ */
