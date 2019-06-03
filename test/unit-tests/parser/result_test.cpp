#include "catch2/catch.hpp"
#include "rush/ast/node.hpp"
#include "rush/parser/result.hpp"
#include "rush/parser/parse.hpp"
#include "rush/parser/dump.hpp"

namespace ast = rush::ast;

class test_node : public ast::node {
public:
   using ast::node::accept;
   virtual void accept(ast::visitor&) const override {}
   virtual void attach(ast::node&, ast::context&) override {}
   virtual void detach(ast::node&, ast::context&) override {}
};

TEST_CASE( "rush::parse_result", "[unit][parser]" ) {

   GIVEN( "An empty instance of parse_result" ) {
      rush::parse_result<ast::node> r1;

      THEN( "failed() should be true" ) { REQUIRE( r1.failed() ); }
      THEN( "success() should be false" ) { REQUIRE_FALSE( r1.success() ); }
      THEN( "node() should be null" ) { REQUIRE( r1.node() == nullptr ); }
      THEN( "errors() should be empty" ) { REQUIRE( r1.errors().empty() ); }

      WHEN( "'r1' is assigned an AST node" ) {
         r1 = std::make_unique<test_node>();

         THEN( "failed() should be false" ) { REQUIRE_FALSE( r1.failed() ); }
         THEN( "success() should be true" ) { REQUIRE( r1.success() ); }
         THEN( "node() should not be null" ) { REQUIRE( r1.node() != nullptr ); }
         THEN( "errors() should be empty" ) { REQUIRE( r1.errors().empty() ); }

         AND_WHEN( "'r1' is move-constructed to another parse_result 'r2'" ) {
            auto r2 = std::move(r1);

            THEN( "failed() should be false" ) { REQUIRE_FALSE( r2.failed() ); }
            THEN( "success() should be true" ) { REQUIRE( r2.success() ); }
            THEN( "node() should not be null" ) { REQUIRE( r2.node() != nullptr ); }
            THEN( "errors() should be empty" ) { REQUIRE( r2.errors().empty() ); }
         }

         AND_WHEN( "'r1' is move-assigned to another parse_result 'r2'" ) {
            decltype(r1) r2 = rush::syntax_error { "message", rush::location::undefined };
            r2 = std::move(r1);

            THEN( "failed() should be false" ) { REQUIRE_FALSE( r2.failed() ); }
            THEN( "success() should be true" ) { REQUIRE( r2.success() ); }
            THEN( "node() should not be null" ) { REQUIRE( r2.node() != nullptr ); }
            THEN( "errors() should be not empty" ) { REQUIRE_FALSE( r2.errors().empty() ); }
         }
      }

      WHEN( "'r1' is assigned a syntax_error" ) {
         r1 = rush::syntax_error { "message", rush::location::undefined };

         THEN( "failed() should be true" ) { REQUIRE( r1.failed() ); }
         THEN( "success() should be false" ) { REQUIRE_FALSE( r1.success() ); }
         THEN( "node() should be null" ) { REQUIRE( r1.node() == nullptr ); }
         THEN( "errors() should be not empty" ) { REQUIRE_FALSE( r1.errors().empty() ); }

         AND_WHEN( "'r1' is move-constructed to another parse_result 'r2'" ) {
            auto r2 = std::move(r1);

            THEN( "failed() should be true" ) { REQUIRE( r2.failed() ); }
            THEN( "success() should be false" ) { REQUIRE_FALSE( r2.success() ); }
            THEN( "node() should be null" ) { REQUIRE( r2.node() == nullptr ); }
            THEN( "errors() should be not empty" ) { REQUIRE_FALSE( r2.errors().empty() ); }
         }

         AND_WHEN( "'r1' is move-assigned to another parse_result 'r2'" ) {
            decltype(r1) r2 = std::make_unique<test_node>();
            r2 = std::move(r1);

            THEN( "failed() should be true" ) { REQUIRE( r2.failed() ); }
            THEN( "success() should be false" ) { REQUIRE_FALSE( r2.success() ); }
            THEN( "node() should be null" ) { REQUIRE( r2.node() == nullptr ); }
            THEN( "errors() should be not empty" ) { REQUIRE_FALSE( r2.errors().empty() ); }
         }
      }
   }
}
