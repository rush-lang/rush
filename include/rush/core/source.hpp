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

#ifndef RUSH_CORE_SOURCE_HPP
#define RUSH_CORE_SOURCE_HPP

#include <cstdint>
#include <string_view>
#include <memory>
#include <iosfwd>
#include <system_error>
#include <experimental/filesystem>

namespace rush {
   //! \brief Represents source code read from a file or string data.
   class source {
      struct impl;
      source(source const&) = delete;
      void operator = (source const&) = delete;

   public:
      using iterator = std::string_view::iterator;
      using const_iterator = std::string_view::const_iterator;

      // explicit implementation of default
      // destructor to support the 'pimpl' idiom.
      ~source();
      source();
      source(source&&);
      source& operator = (source&&);

      static source const none;

      //! \brief Constructs a source object from a file.
      static source from_file(std::filesystem::path path, bool is_volatile = false);

      //! \brief Constructs a source object from a raw string.
      static source from_string(std::string_view input, std::string_view id = "");

      //! \brief Constructs a source object from an input stream.
      static source from_stream(std::istream& input, std::string_view id = "");

      std::error_code error() const;

      //! \brief Returns the identifier of the source; typically the file path.
      std::string_view id() const;

      //! \brief Returns a view over the source buffer; typically the contents of a file.
      std::string_view buffer() const;

      //! \brief Returns the length of the source in bytes; typically the size of the file.
      std::size_t size() const;

      //! \brief Returns the length of the source in bytes; typically the size of the file.
      std::size_t length() const;

      //! \brief Returns an iterator to the beginning source buffer.
      const_iterator begin() const;

      //! \brief Returns an iterator to one past the end of the source buffer.
      const_iterator end() const;

   private:
      std::unique_ptr<impl> _pimpl;
      source(std::unique_ptr<impl> pimpl);
   };
}

#endif // RUSH_CORE_SOURCE_HPP

