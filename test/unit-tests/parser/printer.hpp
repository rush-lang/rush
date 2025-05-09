#ifndef RUSH_PARSER_TEST_PRINTER_HPP
#define RUSH_PARSER_TEST_PRINTER_HPP

#pragma once

#include "rush/ast/printer.hpp"
#include "rush/ast/module.hpp"
#include "rush/ast/source.hpp"

#include <iostream>

class test_printer : public rush::ast::printer {
public:
   test_printer(std::basic_ostream<char>& out) : rush::ast::printer { out } {}
   virtual void visit_module(rush::ast::module_node const& mdl) override { traverse(mdl); }
   virtual void visit_source(rush::ast::source_node const& src) override { traverse(src); }
};

#endif // RUSH_PARSER_TEST_PRINTER_HPP
