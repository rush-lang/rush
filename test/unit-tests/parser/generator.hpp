#ifndef RUSH_PARSER_TEST_GENERATOR_HPP
#define RUSH_PARSER_TEST_GENERATOR_HPP

#pragma once

#include "rush/parser/parse.hpp"
#include "printer.hpp"

#include "catch2/catch_all.hpp"
#include "yaml-cpp/yaml.h"

#include <fstream>
#include <sstream>

struct parser_test {
public:
   std::string name;
   std::string input;
   std::string actual;
   std::string expected;
};

class parser_test_generator : public Catch::Generators::IGenerator<parser_test> {
private:
   YAML::const_iterator _docit;
   YAML::const_iterator _testit;
   YAML::Node _doc;

public:
   parser_test_generator(std::string path) {
      _doc = YAML::LoadFile(std::move(path));
      _docit = _doc.begin();
      _testit = (*_docit)["fixtures"].begin();
   }

   virtual bool next() override {
      return false;
      // if (_docit == _doc.end()) {
      //    return false;
      // }

      // if (_testit == _docit->end()) {
      //    ++_docit;
      //    _testit = _docit->begin();
      //    return true;
      // }

      // ++_testit;
      // return true;
   }

   virtual parser_test const& get() const override {
      auto const input = collect_parser_input();
      auto result = parser_test {};
      result.name = _docit->Tag();
      result.input = get_test_string("input");
      result.expected = get_test_string("expect");

      auto context = rush::ast::context {};
      auto syntax_analysis = rush::parse(input, context);
      auto oss = std::ostringstream {};
      syntax_analysis.accept(test_printer { oss });
      result.actual = oss.str();
   }

private:
   std::string get_test_string(std::string key) const {
      auto const node = (*_testit)[key];
      if (!node.IsDefined()) {
         throw std::runtime_error("expected test " + key);
      }

      return node.as<std::string>();
   }

   std::string collect_parser_input() const {
      auto input = std::string {};
      if ((*_docit)["before"].IsDefined()) {
         input += (*_docit)["before"].as<std::string>();
         input += '\n';
      }

      if ((*_testit)["input"].IsDefined()) {
         input += (*_testit)["input"].as<std::string>();
      }

      return input;
   }
};

// This helper function provides a nicer UX when instantiating the generator
// Notice that it returns an instance of GeneratorWrapper<int>, which
// is a value-wrapper around std::unique_ptr<IGenerator<int>>.
Catch::Generators::GeneratorWrapper<parser_test> fixture(std::string path) {
   return Catch::Generators::GeneratorWrapper<parser_test>(
      Catch::Detail::make_unique<parser_test_generator>(std::move(path)));
}

#endif // RUSH_PARSER_TEST_GENERATOR_HPP
