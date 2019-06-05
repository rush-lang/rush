#pragma once

#ifndef RUSH_CORE_SOURCE_HPP
#define RUSH_CORE_SOURCE_HPP

#include <cstdint>
#include <string_view>
#include <memory>

namespace rush {
   //! \brief Represents source code read from a file or string data.
   class source {
      struct impl;
      source(source const&) = delete;
      void operator =(source const&) = delete;

   public:
      // explicit implementation of default
      // destructor to support the 'pimpl' idiom.
      ~source();

      //! \brief Constructs a source object from a file.
      static source file(std::string_view path, bool is_volatile = false);

      //! \brief Constructs a source object from a raw string.
      static source string(std::string_view input, std::string_view id = "");

      //! \brief Returns the identifier of the source; typically the file path.
      std::string_view id() const;

      //! \brief Returns a view over the source buffer; typically the contents of a file.
      std::string_view buffer() const;

      //! \brief Returns an iterator to the beginning source buffer.
      std::string_view::const_iterator begin() const;

      //! \brief Returns an iterator to one past the end of the source buffer.
      std::string_view::const_iterator end() const;

   private:
      std::unique_ptr<impl> _pimpl;
      source(std::unique_ptr<impl> pimpl);
   };
}

#endif // RUSH_CORE_SOURCE_HPP

