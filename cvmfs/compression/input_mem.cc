/**
 * This file is part of the CernVM File System.
 */

#include <cstdlib>
#include "input_mem.h"

namespace zlib {

InputMem::InputMem(const unsigned char* src, const size_t src_size,
                   size_t max_chunk_size, bool is_owner) :
                                  InputAbstract(is_owner, max_chunk_size),
                                  src_(src), src_size_(src_size) {
  if (IsValid()) {
    has_chunk_left_ = true;
    idx_ = -1;
  }
}

InputMem::~InputMem() {
  if (IsValid()) {
    if (is_owner_) {
      // we need to ignore "const" here
      free(const_cast<unsigned char*>(src_));
    }
  }
}

bool InputMem::NextChunk() {
  if ((idx_ != -1ul && idx_ + max_chunk_size_ >= src_size_)
      || !has_chunk_left_) {
    return false;
  }

  chunk_size_ = max_chunk_size_;
  if (idx_ == -1ul) {
    idx_ = 0;
  } else {
    idx_ += max_chunk_size_;
  }

  if (src_size_ - idx_ <= max_chunk_size_) {
    has_chunk_left_ = false;
    chunk_size_ = src_size_ - idx_;
  }

  // just moving pointer as "moving window". no need to create a copy
  // we need to ignore "const" to be able to use InputAbstract protected vars
  chunk_ = const_cast<unsigned char*>(src_) + idx_;

  return true;
}

bool InputMem::IsValid() {
  return src_ != NULL;
}

}  // namespace zlib