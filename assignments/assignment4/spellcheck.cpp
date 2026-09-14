#include "spellcheck.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <numeric>
#include <ranges>
#include <set>
#include <vector>

template <typename Iterator, typename UnaryPred>
std::vector<Iterator> find_all(Iterator begin, Iterator end, UnaryPred pred);

// 将输入的源码字符串分词并构建 Corpus（即 std::set<Token>）
Corpus tokenize(std::string &source)
{
  // 1. 调用 find_all 找到 source 中所有空白字符的迭代器位置（包含 source.begin() 与 source.end() 边界）
  auto space_its = find_all(source.begin(), source.end(), [](char c)
                            { return std::isspace(static_cast<unsigned char>(c)); }); // 谓词：判断字符是否为空白字符

  Corpus corpus; // 定义结果容器 corpus（即 std::set<Token>）

  // 2. 利用二元 std::transform 错位遍历相邻边界对 [first, last)，构造 Token 并插入到 corpus 中
  std::transform(
      space_its.begin(), space_its.end() - 1, space_its.begin() + 1, // 第一序列：左边界 [0, n-1)；第二序列：右边界 [1, n)
      std::inserter(corpus, corpus.end()),                           // 输出迭代器：将生成的 Token 插入到 corpus 集合末尾
      [&source](auto first, auto last)                               // Lambda 表达式：根据当前切片区间构造 Token 对象
      {
        return Token(source, first, last); // 调用 Token 构造函数切片提取子串，内部会自动调用 clean() 清理标点并转小写
      });

  // 3. 按照作业规范使用 std::erase_if 擦除 clean() 后 content 为空的非法 Token
  std::erase_if(corpus, [](const Token &tok)
                { return tok.content.empty(); }); // 谓词：若 Token 内容为空则予以剔除

  return corpus; // 返回构建并清理完毕的 Corpus 集合
}

// 对输入的 Corpus 进行拼写检查，对比 Dictionary 找出拼写错误的 Token 及修改建议
std::set<Misspelling> spellcheck(const Corpus &source, const Dictionary &dictionary)
{
  // 1. 构建声明式 C++20 Ranges 管道流：
  auto misspellings_view =
      source | std::ranges::views::filter([&dictionary](const Token &tok)
                                          { return !dictionary.contains(tok.content); }) | // 步骤 1：筛选不在字典中出现的错误 Token
      std::ranges::views::transform([&dictionary](const Token &tok)
                                    {
                           // 内层管道：在字典中筛选编辑距离恰好为 1 的单词作为推荐词
                           auto valid_suggestions = 
                               dictionary 
                               | std::ranges::views::filter([&tok](const std::string &dict_word)
                                               { return levenshtein(tok.content, dict_word) == 1; }); // 条件：编辑距离等于 1

                           // 将内层视图中的有效建议词物化保存至 std::set<std::string>
                           std::set<std::string> suggestions(valid_suggestions.begin(), valid_suggestions.end());
                           
                           // 将当前错误 Token 与对应的建议词集合组合构造 Misspelling 对象并返回
                           return Misspelling{tok, suggestions}; }) |
      std::ranges::views::filter([](const Misspelling &m)
                                 { return !m.suggestions.empty(); }); // 步骤 2：在物化前筛选掉 suggestions 为空的 Misspelling 记录

  // 2. 将最终完全符合条件（建议词非空）的 Ranges 视图一口气物化构造为 std::set<Misspelling>
  return std::set<Misspelling>(misspellings_view.begin(), misspellings_view.end()); // 返回结果集合
}

/* Helper methods */

#include "utils.cpp" // 包含工具函数的实现文件（如 levenshtein 函数及相关辅助逻辑）