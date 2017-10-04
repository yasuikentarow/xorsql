/***********************************************************************//**
	@file
***************************************************************************/
#include "xorsql/Crypter.hpp"
#include "xorsql/CrypterVfs.hpp"

namespace xorsql {
/***********************************************************************//**
	@brief コンストラクタ
***************************************************************************/
CrypterVfs::CrypterVfs(std::shared_ptr<Crypter> crypter, 
                       const char* parent)
  : super(parent), 
    crypter_(crypter)
{
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int CrypterVfs::onRead(File* file, void* buff, int amt, size_t offset) {
  auto result = super::onRead(file, buff, amt, offset);
  crypter_->encrypt(buff, amt, offset);
  return result;
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int CrypterVfs::onWrite(File* file, const void* buff, int amt, 
                        size_t offset) {
  uint8_t _buff[amt];
  memcpy(_buff, buff, amt);
  crypter_->decrypt(_buff, amt, offset);
  return super::onWrite(file, _buff, amt, offset);
}
/***********************************************************************//**
	$Id$
***************************************************************************/
}
