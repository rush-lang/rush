#include "rush/core/source.hpp"

#include "llvm/Support/MemoryBuffer.h"

#include <variant>

namespace rush {
   struct source::impl {
   public:
      llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> buffer;
      impl(llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> buf)
         : buffer { std::move(buf) } {}
   };

   source source::file(std::string_view path, bool is_volatile) {
      return { std::make_unique<source::impl>(llvm::MemoryBuffer::getFile(
         llvm::StringRef { path.begin(), path.size() },
         -1L, true, is_volatile)) };
   }

   source source::string(std::string_view input, std::string_view id) {
      return { std::make_unique<source::impl>(llvm::MemoryBuffer::getMemBuffer(
         llvm::StringRef { input.begin(), input.size() },
         llvm::StringRef { id.begin(), id.size() })) };
   }

   source::~source() {}
   source::source(std::unique_ptr<source::impl> pimpl)
      : _pimpl { std::move(pimpl) } {}

   std::string_view source::id() const {
      auto ident = (*_pimpl->buffer)->getBufferIdentifier();
      return { ident.begin(), ident.size() };
   }

   std::string_view source::buffer() const {
      auto buf = (*_pimpl->buffer)->getBuffer();
      return { buf.begin(), buf.size() };
   }

   std::string_view::const_iterator source::begin() const {
      return (*_pimpl->buffer)->getBuffer().begin();
   }

   std::string_view::const_iterator source::end() const {
      return (*_pimpl->buffer)->getBuffer().end();
   }
}
