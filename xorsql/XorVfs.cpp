/***********************************************************************//**
	@file
***************************************************************************/
#include "xorsql/XorVfs.hpp"

namespace xorsql {
/***********************************************************************//**
	@brief コンストラクタ
***************************************************************************/
XorVfs::XorVfs(const std::string& key, 
               const std::string& name, 
               const char* parent)
  : super(name, parent), 
    key_(key)
{
  sqlite3_vfs::szOsFile += sizeof(XorVfs*);
  sqlite3_io_methods::iVersion = 0;
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int XorVfs::onOpen(const char* name, 
                   sqlite3_file* file, 
                   int flags, 
                   int* outFlags) {
  auto result = super::onOpen(name, file, flags, outFlags);
  if(sqlite3_io_methods::iVersion == 0) {
    *static_cast<sqlite3_io_methods*>(this) = *file->pMethods;
  }
  static_cast<File*>(file)->vfs = this;
  return result;
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int XorVfs::OnRead(sqlite3_file* file, 
                   void* buff, 
                   int amt, 
                   sqlite3_int64 offset) {
  return static_cast<File*>(file)->vfs->onRead(file, buff, amt, offset);
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int XorVfs::OnWrite(sqlite3_file* file, 
                    const void* buff, 
                    int amt, 
                    sqlite3_int64 offset) {
  return static_cast<File*>(file)->vfs->onWrite(file, buff, amt, offset);
}
/***********************************************************************//**
	$Id$
***************************************************************************/
}
