/***********************************************************************//**
	@file
***************************************************************************/
#pragma once

#include "xorsql/Vfs.hpp"

namespace xorsql {
/***********************************************************************//**
	@brief 
***************************************************************************/
class XorVfs
  : public Vfs, 
    private sqlite3_io_methods
{
  typedef Vfs super;

 private:
  struct File
    : public sqlite3_file
  {
    XorVfs* vfs;
  };

 private:
  std::string key_;

 public:
  XorVfs(const std::string& key, 
         const std::string& name = "xor", 
         const char* parent = nullptr);
  ~XorVfs() override = default;

 protected:
  int onOpen(const char* name, 
             sqlite3_file* file, 
             int flags, 
             int* outFlags) override;

  int onRead(sqlite3_file* file, 
             void* buff, 
             int amt, 
             sqlite3_int64 offset);

  int onWrite(sqlite3_file* file, 
             const void* buff, 
             int amt, 
             sqlite3_int64 offset);

 private:
  static int OnRead(sqlite3_file* file, 
                    void* buff, 
                    int amt, 
                    sqlite3_int64 offset);

  static int OnWrite(sqlite3_file* file, 
                     const void* buff, 
                     int amt, 
                     sqlite3_int64 offset);
};
/***********************************************************************//**
	$Id$
***************************************************************************/
}
