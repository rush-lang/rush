#pragma once

#ifndef RUSH_EXTRA_HASH_COMBINE_HPP
#define RUSH_EXTRA_HASH_COMBINE_HPP

#include <functional>

namespace rush {
   template <class T>
   inline void hash_combine(std::size_t& seed, T const& v)
   {
      std::hash<T> hasher;
      seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
   }
}

#endif // RUSH_EXTRA_HASH_COMBINE_HPP
