#include <iostream>
#include <string>
#include <memory>

using namespace std;

enum class animal_type: uint8_t {
  CAT = 0,
  DOG = 1
};

std::ostream& operator<<(std::ostream& os, const animal_type& v) {
  switch(v) {
    case animal_type::CAT:
      os << "CAT";
      break;
    case animal_type::DOG:
      os << "DOG";
      break;
    default:
      os << "UNKNOWN";
      break;
  }
  return os;
}

class animal {
public:
  typedef std::shared_ptr<animal> ptr;
  virtual ~animal() {}
  virtual void action() = 0;

  const animal_type type_;
protected:
  animal(animal_type type) : type_(type) {}
};

class cat: public animal {
public:
  typedef std::shared_ptr<cat> ptr;
  cat() : animal(animal_type::CAT), name_("tom") {}
  cat(const std::string& name) : animal(animal_type::CAT), name_(name) {}
  virtual ~cat() {}
  void action() { std::cout << name_ << "(" << type_ << ")" << "\t: mew!" << std::endl; }
  const std::string name_;
};

class dog: public animal {
public:
  typedef std::shared_ptr<dog> ptr;
  dog() : animal(animal_type::DOG), name_("spike") {}
  dog(const std::string& name) : animal(animal_type::DOG), name_(name) {}
  virtual ~dog() {}
  void action() { std::cout << name_ << "(" << type_ << ")" << "\t: bark!" << std::endl; }
  const std::string name_;
};

class animal_feeder {
public:
  animal_feeder() {}
  virtual ~animal_feeder() {}
  virtual void feed(animal::ptr& ptr) = 0;
};

class schrodinger_feeder: public animal_feeder {
public:
  schrodinger_feeder() {}
  virtual ~schrodinger_feeder() {}
  virtual void feed(animal::ptr& ptr) {
    if(ptr) {
      animal::ptr dog_ptr = std::make_shared<dog>("schrodinger's cat");
      animal::ptr cat_ptr = std::make_shared<cat>("schrodinger's dog");
      switch (ptr->type_) {
        case animal_type::CAT:
          ptr.swap(dog_ptr);
          break;
        case animal_type::DOG:
          ptr.swap(cat_ptr);
          break;
        default:
          break;
      }
    }
  }
};

int main(int argc, char* argv[]) {
  animal::ptr null_animal;
  animal::ptr pussy_cat = std::make_shared<cat>("Thomas");
  animal::ptr friendly_dog = std::make_shared<dog>("Fido");
  cat::ptr pcat;
  dog::ptr pdog;
  try {
    pcat = std::dynamic_pointer_cast<cat>(pussy_cat);
    pdog = std::dynamic_pointer_cast<dog>(pussy_cat);
    if(pcat) {
      std::cout << "std::dynamic_pointer_cast<cat>(pussy_cat) success." << std::endl;
      pcat->action();
    } else {
      std::cout << "std::dynamic_pointer_cast<cat>(pussy_cat) failed." << std::endl;
    }
    if(pdog) {
      std::cout << "std::dynamic_pointer_cast<dog>(pussy_cat) success." << std::endl;
      pdog->action();
    } else {
      std::cout << "std::dynamic_pointer_cast<dog>(pussy_cat) failed." << std::endl;
    }
  } catch (...) {
    std::cout << "std::dynamic_pointer_cast() error." << std::endl;
  }
  std::cout << "[PLAY 1, Opening, synopsis..]" << std::endl;
  pussy_cat->action();
  friendly_dog->action();

  std::cout << "[PLAY 2, Swap soul, synopsis..]" << std::endl;
  pussy_cat.swap(friendly_dog);
  null_animal = friendly_dog;
  pussy_cat->action();
  friendly_dog->action();
  null_animal->action();

  std::cout << "[PLAY 3, Feed animals, synopsis..]" << std::endl;
  schrodinger_feeder feeder;
  feeder.feed(pussy_cat);
  pussy_cat->action();
  feeder.feed(friendly_dog);
  friendly_dog->action();
  feeder.feed(null_animal);
  null_animal->action();

  std::cout << "sizeof(bool) = " << sizeof(bool) << std::endl;
  std::cout << "sizeof(animal_type) = " << sizeof(animal_type) << std::endl;
	return 0;
}
