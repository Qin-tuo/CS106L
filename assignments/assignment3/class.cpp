#include <string>
#include <iostream>
#include "class.h"


StanfordID::StanfordID(std::string name, std::string email, int id) {
    this->name = name;
    this->email = email;
    this->id = id;
}

StanfordID::StanfordID() : name(""), email(""), id(0) {} // Default constructor

std::string StanfordID::getData() const {
    return "Name: " + name + ", Email: " + email + ", ID: " + std::to_string(id);
}

std::string StanfordID::getName() const {
    getData();
    return name;
}
std::string StanfordID::getID() const {
    if (id < 0) {
        return "Invalid ID";
    }   
    else {
        return std::to_string(id);
    }
}
std::string StanfordID::getEmail() const {
    return email;
}

void StanfordID::setEmail(std::string newEmail) {
    email = newEmail;
}

void StanfordID::setName(std::string newName) {
    name = newName;
}

void StanfordID::setID(int newID) {
    if (newID < 0) {
        std::cout << "Invalid ID" << std::endl;
    } else {
        id = newID;
    }
}