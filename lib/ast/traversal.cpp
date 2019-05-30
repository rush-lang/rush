#include "rush/ast/types.hpp"
#include "rush/ast/statements.hpp"
#include "rush/ast/expressions.hpp"
#include "rush/ast/declarations.hpp"
#include "rush/ast/traversal.hpp"
#include "rush/ast/visitor.hpp"


using namespace rush;

namespace rush::ast {
   class traversal_roundtrip : public traversal {
      friend class traversal;
   protected:
      virtual void accept(ast::node const& ast) override { ast.accept(*_v); }
   private:
      ast::visitor* _v;
      traversal_roundtrip(ast::visitor* v) : _v { v } {}
   };

   void traversal::traverse(ast::node const& ast) {
      auto v = traversal_roundtrip { this };
      ast.accept(v);
   }
}
