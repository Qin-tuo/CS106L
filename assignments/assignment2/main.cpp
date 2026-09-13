/*
 * CS106L Assignment 2: Marriage Pact
 * Created by Haven Whitney with modifications by Fabio Ibanez & Jacob Roberts-Baca.
 *
 * Welcome to Assignment 2 of CS106L! Please complete each STUDENT TODO
 * in this file. You do not need to modify any other files.
 *
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <queue>
#include <set>
#include <string>
#include <unordered_set>

std::string kYourName ="Jack Chen"; // Don't forget to change this!

/**
 * Takes in a file name and returns a set containing all of the applicant names as a set.
 *
 * @param filename  The name of the file to read.
 *                  Each line of the file will be a single applicant's name.
 * @returns         A set of all applicant names read from the file.
 *
 * @remark Feel free to change the return type of this function (and the function
 * below it) to use a `std::unordered_set` instead. If you do so, make sure
 * to also change the corresponding functions in `utils.h`.
 */
std::set<std::string> get_applicants(std::string filename) {
  // STUDENT TODO: Implement this function.
  std::ifstream file(filename);
  std::set<std::string> applicants;
  if (file.is_open()) {
    std::string line;
    while (std::getline(file, line)) {
      applicants.insert(line);
    }
  }
  else {
    std::cerr << "Error opening file: " << filename << std::endl;
  }
  return applicants;
}

std::string get_initials(const std::string& name) {
  if (name.find(' ') == std::string::npos) {
    return "";
  }
  std::istringstream iss(name);
  std::string firstname ;
  std::string lastname;
  iss >> firstname >> lastname;
  if (firstname.empty() || lastname.empty()) {
    return "";
  }
  std::string initials;
  initials += firstname[0];
  initials += lastname[0];
  return initials;
}

/**
 * Takes in a set of student names by reference and returns a queue of names
 * that match the given student name.
 *
 * @param name      The returned queue of names should have the same initials as this name.
 * @param students  The set of student names.
 * @return          A queue containing pointers to each matching name.
 */
std::queue<const std::string*> find_matches(std::string name, std::set<std::string>& students) {
  // STUDENT TODO: Implement this function.

  std::queue<const std::string*> matches;
  std::string initials = get_initials(name);
  for (const auto& student : students) {
    std::string student_initials = get_initials(student);
    if (initials == student_initials) {
      matches.push(&student); 
    }
  }
  return matches;
}

/**
 * Takes in a queue of pointers to possible matches and determines the one true match!
 *
 * You can implement this function however you'd like, but try to do something a bit
 * more complicated than a simple `pop()`.
 *
 * @param matches The queue of possible matches.
 * @return        Your magical one true love.
 *                Will return "NO MATCHES FOUND." if `matches` is empty.
 */
std::string get_match(std::queue<const std::string*>& matches) {
  // STUDENT TODO: Implement this function.
  if (matches.empty()) {
    std::cout << "NO MATCHES FOUND." << std::endl;
    return "NO MATCHES FOUND.";
  }
  const std::string* best_match = matches.front();
  matches.pop();
  while (!matches.empty()) {
    const std::string* current_match = matches.front();
    matches.pop();
    if (current_match->length() < best_match->length()) {
      best_match = current_match; 
    }
  }
  return *best_match;
}

/* #### Please don't remove this line! #### */
#include "autograder/utils.hpp"
