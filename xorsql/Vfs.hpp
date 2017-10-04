/***********************************************************************//**
	@file
***************************************************************************/
#pragma once

namespace xorsql {
/***********************************************************************//**
	@brief 
***************************************************************************/
class Vfs
  : protected sqlite3_vfs, 
    private sqlite3_io_methods
{
 protected:
  struct File
    : public sqlite3_file
  {
    Vfs* vfs;
    sqlite3_file* parent;
  };

 private:
  std::string name_;
  sqlite3_vfs* parent_;

 public:
  Vfs(const char* parent = nullptr);
  virtual ~Vfs();

  const std::string& getName() const {
    return name_;
  }

 protected:
  virtual int onOpen(const char* name, File* file, int flags, int* outFlags);
  virtual int onClose(File* file);
  virtual int onRead(File* file, void* buff, int amt, size_t offset);
  virtual int onWrite(File* file, const void* buff, int amt, size_t offset);
  virtual int onTruncate(File* file, size_t size);
  virtual int onSync(File* file, int flags);
  virtual int onFileSize(File* file, size_t* size);
  virtual int onLock(File* file, int type);
  virtual int onUnlock(File* file, int type);
  virtual int onCheckReservedLock(File* file, int* resOut);
  virtual int onFileControl(File* file, int op, void* arg);
  virtual int onSectorSize(File* file);
  virtual int onDeviceCharacteristics(File* file);

  virtual int onShmMap(File* file, int page, int pageSize, 
                       int extend, volatile void** pt);
  virtual int onShmLock(File* file, int offset, int n, int flags);
  virtual void onShmBarrier(File* file);
  virtual int onShmUnmap(File* file, int deleteFlag);

  virtual int onFetch(File* file, size_t offset, int amt, void** pt);
  virtual int onUnfetch(File* file, size_t offset, void* pt);

 private:
  static int OnOpen(sqlite3_vfs* vfs, const char* name, sqlite3_file* file, 
                    int flags, int* outFlags);
  static int OnClose(sqlite3_file* file);
  static int OnRead(sqlite3_file* file, void* buff, int amt, 
                    sqlite3_int64 offset);
  static int OnWrite(sqlite3_file* file, const void* buff, int amt, 
                     sqlite3_int64 offset);
  static int OnTruncate(sqlite3_file* file, sqlite3_int64 size);
  static int OnSync(sqlite3_file* file, int flags);
  static int OnFileSize(sqlite3_file* file, sqlite3_int64* size);
  static int OnLock(sqlite3_file* file, int type);
  static int OnUnlock(sqlite3_file* file, int type);
  static int OnCheckReservedLock(sqlite3_file* file, int* resOut);
  static int OnFileControl(sqlite3_file* file, int op, void* arg);
  static int OnSectorSize(sqlite3_file* file);
  static int OnDeviceCharacteristics(sqlite3_file* file);

  static int OnShmMap(sqlite3_file* file, int page, int pageSize, 
                      int extend, volatile void** pt);
  static int OnShmLock(sqlite3_file* file, int offset, int n, int flags);
  static void OnShmBarrier(sqlite3_file* file);
  static int OnShmUnmap(sqlite3_file* file, int deleteFlag);

  static int OnFetch(sqlite3_file* file, sqlite3_int64 offset, int amt, 
                     void** pt);
  static int OnUnfetch(sqlite3_file* file, sqlite3_int64 offset, void* pt);
};
/***********************************************************************//**
	$Id$
***************************************************************************/
}
