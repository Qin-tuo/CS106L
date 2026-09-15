/*
 * CS106L Assignment 5: TreeBook
 * Created by Fabio Ibanez with modifications by Jacob Roberts-Baca.
 */

#include <iostream>
#include <string>

class User
{
public:
  User(const std::string &name);
  void add_friend(const std::string &name);
  std::string get_name() const;
  size_t size() const;
  void set_friend(size_t index, const std::string &name);

  /**
   * STUDENT TODO:
   * Your custom operators and special member functions will go here!
   */
  // 1. 析构函数
  ~User();

  // 2. 拷贝构造函数
  User(const User &user);

  // 3. 拷贝赋值运算符
  User &operator=(const User &user);

  // 4. 禁用移动构造函数
  User(User &&user) = delete;

  // 5. 禁用移动赋值运算符
  User &operator=(User &&user) = delete;

  // Part 3: 自定义运算符重载 (成员函数)
  User &operator+=(User &rhs);
  bool operator<(const User &rhs) const;

  // Part 1: 流插入运算符 (友元函数)
  friend std::ostream &operator<<(std::ostream &os, const User &user);

private:
  std::string _name;
  std::string *_friends;
  size_t _size;
  size_t _capacity;
};