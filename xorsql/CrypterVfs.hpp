/***********************************************************************//**
	@file
***************************************************************************/
#pragma once

#include "xorsql/Vfs.hpp"

namespace xorsql {
/***********************************************************************//**
	@brief 暗号化VFS
***************************************************************************/
class CrypterVfs
  : public Vfs
{
  typedef Vfs super;

 private:
  std::shared_ptr<Crypter> crypter_;

 public:
  CrypterVfs(std::shared_ptr<Crypter> crypter, 
             const char* parent = nullptr);
  ~CrypterVfs() override = default;

 protected:
  int onRead(File* file, void* buff, int amt, size_t offset) override;
  int onWrite(File* file, const void* buff, int amt, size_t offset) override;
};
/***********************************************************************//**
	$Id$
***************************************************************************/
}
