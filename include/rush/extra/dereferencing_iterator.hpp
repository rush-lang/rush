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

   // Iterator adapter designed to adapt containers with values of pointer types
   // such that when the iterator is dereferenced, a reference instead of a pointer
   // is obtained.
   template <typename IterT>
   class dereferencing_iterator {
   public:
      using iterator_category = typename std::iterator_traits<IterT>::iterator_category;
      using difference_type = typename std::iterator_traits<IterT>::difference_type;
      using value_type = typename detail::remove_pointer<typename std::iterator_traits<IterT>::value_type>::type;
      using pointer = value_type*;
      using reference = value_type&;
      using size_type = std::size_t;

      explicit dereferencing_iterator(IterT it) : _it { it } {}

      constexpr reference operator *() const { return *(_it.operator*()); }
      constexpr pointer operator ->() const { return std::addressof(*(_it.operator->())); }

      dereferencing_iterator& operator ++() { ++_it; return *this; }
      dereferencing_iterator& operator --() { --_it; return *this; }

      dereferencing_iterator operator ++(int) { return { _it++ }; }
      dereferencing_iterator operator --(int) { return { _it-- }; }

      dereferencing_iterator& operator +=(size_type offset) { _it += offset; return *this; }
      dereferencing_iterator& operator -=(size_type offset) { _it -= offset; return *this; }

      dereferencing_iterator& operator += (dereferencing_iterator const& other) { _it += other._it; return *this; }
      dereferencing_iterator& operator -= (dereferencing_iterator const& other) { _it -= other._it; return *this; }

      friend dereferencing_iterator operator + (
         dereferencing_iterator const& deref_it, size_type offset)
         { return dereferencing_iterator { deref_it._it } += offset; }

      friend dereferencing_iterator operator + (
         size_type offset, dereferencing_iterator const& deref_it)
         { return dereferencing_iterator { deref_it._it } += offset; }

      friend dereferencing_iterator operator - (
         dereferencing_iterator const& deref_it, size_type offset)
         { return dereferencing_iterator { deref_it._it } -= offset; }

      friend size_type operator - (
         dereferencing_iterator const& lhs, dereferencing_iterator const& rhs)
         { return lhs._it - rhs._it; }

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
