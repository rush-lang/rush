/*************************************************************************
* Copyright 2018 Reegan Troy Layzell
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*************************************************************************/
#ifndef RUSH_AST_TYPES_TYPE_RESOLVER_HPP
#define RUSH_AST_TYPES_TYPE_RESOLVER_HPP

#include "rush/ast/node.hpp"
#include "rush/ast/types/builtin.hpp"
#include "rush/ast/types/type_ref.hpp"

namespace rush::ast {
   //! \brief Allows types to delay their resolution until
   //         a specified point in time. function_types use
   //         this to delay resolving the return type until
   //         it is requested by the return_type method.
   class type_resolver {
   public:
      //! \brief A callback type that derived types will use
      //         in order to be called on resolve.
      struct callback {
         virtual ~callback() = default;
         virtual ast::type_ref operator ()() = 0;
      };

      type_resolver(std::unique_ptr<callback> cb)
         : _callback { std::move(cb) }
         , _resolving { false } {}

      ast::type_ref resolve() {
         if (_resolving)
            return ast::types::recursive_ref;

         _resolving = true;
         auto result = (*_callback)();
         _resolving = false;
         return result;
      }

   private:
      std::unique_ptr<callback> _callback;
      bool _resolving;
   };
}

#endif // RUSH_AST_TYPES_TYPE_RESOLVER_HPP
