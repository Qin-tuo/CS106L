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

Corpus tokenize(std::string &source)
{
  // 1. 获取包含 source.begin()、所有空白位置、source.end() 的完整边界迭代器序列
  auto space_its = find_all(source.begin(), source.end(), [](char c)
                            { return std::isspace(static_cast<unsigned char>(c)); });

  Corpus corpus;

  // 2. 直接使用 std::transform 将构造的 Token 插入到 corpus 中
  std::transform(
      space_its.begin(), space_its.end() - 1, space_its.begin() + 1,
      std::inserter(corpus, corpus.end()),
      [&source](auto first, auto last)
      {
        return Token(source, first, last); // 自动调用 clean() 处理标点和转小写
      });

  // 3. 按照作业规范显式使用 std::erase_if 剔除 content 为空的 Token
  std::erase_if(corpus, [](const Token &tok)
                { return tok.content.empty(); });

  return corpus;
}

std::set<Misspelling> spellcheck(const Corpus &source, const Dictionary &dictionary)
{
  // 1. 声明式 Ranges 管道：
  //    - 过滤不在字典中的 Token
  //    - 映射为 Misspelling 对象（内层筛选编辑距离恰好为 1 的建议词）
  //    - 过滤掉建议词为空 (suggestions.empty()) 的 Misspelling
  auto misspellings_view =
      source | std::ranges::views::filter([&dictionary](const Token &tok)
                                          { return !dictionary.contains(tok.content); }) |
      std::ranges::views::transform([&dictionary](const Token &tok)
                                    {
                           auto valid_suggestions = 
                               dictionary 
                               | std::ranges::views::filter([&tok](const std::string &dict_word)
                                               { return levenshtein(tok.content, dict_word) == 1; });

                           std::set<std::string> suggestions(valid_suggestions.begin(), valid_suggestions.end());
                           return Misspelling{tok, suggestions}; }) |
      std::ranges::views::filter([](const Misspelling &m)
                                 { return !m.suggestions.empty(); });

  // 2. 将最终完全符合条件（非空建议词）的流水线结果一口气物化为 std::set<Misspelling>
  return std::set<Misspelling>(misspellings_view.begin(), misspellings_view.end());
}

/* Helper methods */

#include "utils.cpp"