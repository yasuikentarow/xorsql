/***********************************************************************//**
	@file
***************************************************************************/
#pragma once

#include "xorsql/Crypter.hpp"

namespace xorsql {
/***********************************************************************//**
	@brief 
***************************************************************************/
class XorCrypter
  : public Crypter
{
  typedef Crypter super;

 private:
  std::vector<uint8_t> key_;

 public:
  XorCrypter() = default;
  ~XorCrypter() override = default;

  void setKey(const std::vector<uint8_t>& key) {
    key_ = key;
  }

  void setKey(int seed, size_t size);

 protected:
  void encrypt(void* buff, size_t size, size_t offset) override;

  void decrypt(void* buff, size_t size, size_t offset) override {
    encrypt(buff, size, offset);
  }
};
/***********************************************************************//**
	$Id$
***************************************************************************/
}
