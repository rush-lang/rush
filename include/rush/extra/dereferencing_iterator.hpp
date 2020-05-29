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
#pragma once

#ifndef RUSH_EXTRA_DEREFERENCING_ITERATOR_HPP
#define RUSH_EXTRA_DEREFERENCING_ITERATOR_HPP

#include <iterator>
#include <type_traits>

#include <memory>

namespace rush {
   namespace detail {
      template <typename T>
      struct remove_pointer : std::remove_pointer<T> {};

      template <typename T>
      struct remove_pointer<std::weak_ptr<T>> : std::remove_pointer<T*> {};

      template <typename T>
      struct remove_pointer<std::unique_ptr<T>> : std::remove_pointer<T*> {};

      template <typename T>
      struct remove_pointer<std::shared_ptr<T>> : std::remove_pointer<T*> {};
   }

   template <typename IterT>
   class dereferencing_iterator
      : public std::iterator<
         typename std::iterator_traits<IterT>::iterator_category,
         typename detail::remove_pointer<typename std::iterator_traits<IterT>::value_type>::type> {
   private:
      using _base_type = std::iterator<
         typename std::iterator_traits<IterT>::iterator_category,
         typename detail::remove_pointer<typename std::iterator_traits<IterT>::value_type>::type>;

   public:
      using typename _base_type::value_type;
      using typename _base_type::pointer;
      using typename _base_type::reference;
      using typename _base_type::difference_type;
      using typename _base_type::iterator_category;

      explicit dereferencing_iterator(IterT it) : _it { it } {}

      constexpr reference operator *() const { return *(_it.operator*()); }
      constexpr pointer operator ->() const { return std::addressof(*(_it.operator->())); }

      dereferencing_iterator& operator ++() { ++_it; return *this; }
      dereferencing_iterator& operator --() { --_it; return *this; }

      dereferencing_iterator operator ++(int) { return { _it++ }; }
      dereferencing_iterator operator --(int) { return { _it-- }; }

      template <typename IterU>
      constexpr friend bool operator == (
         dereferencing_iterator<IterT> const& lhs,
         dereferencing_iterator<IterU> const& rhs)
         { return lhs._it == rhs._it; }

      template <typename IterU>
      constexpr friend bool operator != (
         dereferencing_iterator<IterT> const& lhs,
         dereferencing_iterator<IterU> const& rhs)
         { return lhs._it != rhs._it; }

      template <typename IterU>
      constexpr friend bool operator < (
         dereferencing_iterator<IterT> const& lhs,
         dereferencing_iterator<IterU> const& rhs)
         { return lhs._it < rhs._it; }

      template <typename IterU>
      constexpr friend bool operator > (
         dereferencing_iterator<IterT> const& lhs,
         dereferencing_iterator<IterU> const& rhs)
         { return lhs._it > rhs._it; }

      template <typename IterU>
      constexpr friend bool operator <= (
         dereferencing_iterator<IterT> const& lhs,
         dereferencing_iterator<IterU> const& rhs)
         { return lhs._it <= rhs._it; }

      template <typename IterU>
      constexpr friend bool operator >= (
         dereferencing_iterator<IterT> const& lhs,
         dereferencing_iterator<IterU> const& rhs)
         { return lhs._it >= rhs._it; }

   private:
      IterT _it;
   };

   template <typename IterT>
   dereferencing_iterator<IterT> make_deref_iterator(IterT iter)
   { return dereferencing_iterator<IterT> { iter }; }
}

#endif // RUSH_EXTRA_DEREFERENCING_ITERATOR_HPP
