/***********************************************************************//**
	@file
***************************************************************************/
#include "xorsql/Vfs.hpp"

namespace xorsql {
/***********************************************************************//**
	@brief コンストラクタ
***************************************************************************/
Vfs::Vfs(const char* parent)
  : parent_(sqlite3_vfs_find(parent))
{
  assert(parent_);
  {
    std::stringstream stream;
    stream << this;
    name_ = stream.str();
  }
  *static_cast<sqlite3_vfs*>(this) = *parent_;
  sqlite3_vfs::pNext = nullptr;
  sqlite3_vfs::zName = name_.c_str();
  sqlite3_vfs::szOsFile = sizeof(File);
  sqlite3_vfs::xOpen = &Vfs::OnOpen;

  memset(static_cast<sqlite3_io_methods*>(this), 0, 
         sizeof(sqlite3_io_methods));
  sqlite3_io_methods::xClose = &Vfs::OnClose;
  sqlite3_io_methods::xRead = &Vfs::OnRead;
  sqlite3_io_methods::xWrite = &Vfs::OnWrite;
  sqlite3_io_methods::xTruncate = &Vfs::OnTruncate;
  sqlite3_io_methods::xSync = &Vfs::OnSync;
  sqlite3_io_methods::xFileSize = &Vfs::OnFileSize;
  sqlite3_io_methods::xLock = &Vfs::OnLock;
  sqlite3_io_methods::xUnlock = &Vfs::OnUnlock;
  sqlite3_io_methods::xCheckReservedLock = &Vfs::OnCheckReservedLock;
  sqlite3_io_methods::xFileControl = &Vfs::OnFileControl;
  sqlite3_io_methods::xSectorSize = &Vfs::OnSectorSize;
  sqlite3_io_methods::xDeviceCharacteristics = &Vfs::OnDeviceCharacteristics;
  sqlite3_io_methods::xShmMap = &Vfs::OnShmMap;
  sqlite3_io_methods::xShmLock = &Vfs::OnShmLock;
  sqlite3_io_methods::xShmBarrier = &Vfs::OnShmBarrier;
  sqlite3_io_methods::xShmUnmap = &Vfs::OnShmUnmap;
  sqlite3_io_methods::xFetch = &Vfs::OnFetch;
  sqlite3_io_methods::xUnfetch = &Vfs::OnUnfetch;

  sqlite3_vfs_register(static_cast<sqlite3_vfs*>(this), 0);
}
/***********************************************************************//**
	@brief デストラクタ
***************************************************************************/
Vfs::~Vfs() {
  sqlite3_vfs_unregister(static_cast<sqlite3_vfs*>(this));
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int Vfs::onOpen(const char* name, File* file, int flags, int* outFlags) {
  file->parent = 
    static_cast<sqlite3_file*>(sqlite3_malloc(parent_->szOsFile));
  return parent_->xOpen(static_cast<sqlite3_vfs*>(this), name, 
                        file->parent, flags, outFlags);
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int Vfs::onClose(File* file) {
  return file->parent->pMethods->xClose(file->parent);
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int Vfs::onRead(File* file, void* buff, int amt, size_t offset) {
  return file->parent->pMethods->xRead(file->parent, buff, amt, offset);
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int Vfs::onWrite(File* file, const void* buff, int amt, size_t offset) {
  return file->parent->pMethods->xWrite(file->parent, buff, amt, offset);
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int Vfs::onTruncate(File* file, size_t size) {
  return file->parent->pMethods->xTruncate(file->parent, size);
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int Vfs::onSync(File* file, int flags) {
  return file->parent->pMethods->xSync(file->parent, flags);
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int Vfs::onFileSize(File* file, size_t* size) {
  sqlite3_int64 _size;
  auto result = file->parent->pMethods->xFileSize(file->parent, &_size);
  *size = _size;
  return result;
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int Vfs::onLock(File* file, int type) {
  return file->parent->pMethods->xLock(file->parent, type);
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int Vfs::onUnlock(File* file, int type) {
  return file->parent->pMethods->xUnlock(file->parent, type);
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int Vfs::onCheckReservedLock(File* file, int* resOut) {
  return file->parent->pMethods->xCheckReservedLock(file->parent, resOut);
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int Vfs::onFileControl(File* file, int op, void* arg) {
  return file->parent->pMethods->xFileControl(file->parent, op, arg);
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int Vfs::onSectorSize(File* file) {
  return file->parent->pMethods->xSectorSize(file->parent);
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int Vfs::onDeviceCharacteristics(File* file) {
  return file->parent->pMethods->xDeviceCharacteristics(file->parent);
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int Vfs::onShmMap(File* file, int page, int pageSize, 
                  int extend, volatile void** pt) {
  return file->parent->pMethods->xShmMap(file->parent, page, pageSize, 
                                         extend, pt);
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int Vfs::onShmLock(File* file, int offset, int n, int flags) {
  return file->parent->pMethods->xShmLock(file->parent, offset, n, flags);
}
/***********************************************************************//**
	@brief 
***************************************************************************/
void Vfs::onShmBarrier(File* file) {
  file->parent->pMethods->xShmBarrier(file->parent);
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int Vfs::onShmUnmap(File* file, int deleteFlag) {
  return file->parent->pMethods->xShmUnmap(file->parent, deleteFlag);
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int Vfs::onFetch(File* file, size_t offset, int amt, void** pt) {
  return file->parent->pMethods->xFetch(file->parent, offset, amt, pt);
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int Vfs::onUnfetch(File* file, size_t offset, void* pt) {
  return file->parent->pMethods->xUnfetch(file->parent, offset, pt);
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int Vfs::OnOpen(sqlite3_vfs* vfs, const char* name, sqlite3_file* file, 
                int flags, int* outFlags) {
  auto _file = static_cast<File*>(file);
  _file->vfs = static_cast<Vfs*>(vfs);
  _file->pMethods = static_cast<sqlite3_io_methods*>(_file->vfs);
  return static_cast<Vfs*>(vfs)->onOpen(name, _file, flags, outFlags);
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int Vfs::OnClose(sqlite3_file* file) {
  auto _file = static_cast<File*>(file);
  return _file->vfs->onClose(_file);
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int Vfs::OnRead(sqlite3_file* file, void* buff, int amt, 
                sqlite3_int64 offset) {
  auto _file = static_cast<File*>(file);
  return _file->vfs->onRead(_file, buff, amt, offset);
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int Vfs::OnWrite(sqlite3_file* file, const void* buff, int amt, 
                 sqlite3_int64 offset) {
  auto _file = static_cast<File*>(file);
  return _file->vfs->onWrite(_file, buff, amt, offset);
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int Vfs::OnTruncate(sqlite3_file* file, sqlite3_int64 size) {
  auto _file = static_cast<File*>(file);
  return _file->vfs->onTruncate(_file, size);
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int Vfs::OnSync(sqlite3_file* file, int flags) {
  auto _file = static_cast<File*>(file);
  return _file->vfs->onSync(_file, flags);
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int Vfs::OnFileSize(sqlite3_file* file, sqlite3_int64* size) {
  auto _file = static_cast<File*>(file);
  size_t _size;
  auto result = _file->vfs->onFileSize(_file, &_size);
  *size = _size;
  return result;
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int Vfs::OnLock(sqlite3_file* file, int type) {
  auto _file = static_cast<File*>(file);
  return _file->vfs->onLock(_file, type);
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int Vfs::OnUnlock(sqlite3_file* file, int type) {
  auto _file = static_cast<File*>(file);
  return _file->vfs->onUnlock(_file, type);
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int Vfs::OnCheckReservedLock(sqlite3_file* file, int* resOut) {
  auto _file = static_cast<File*>(file);
  return _file->vfs->onCheckReservedLock(_file, resOut);
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int Vfs::OnFileControl(sqlite3_file* file, int op, void* arg) {
  auto _file = static_cast<File*>(file);
  return _file->vfs->onFileControl(_file, op, arg);
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int Vfs::OnSectorSize(sqlite3_file* file) {
  auto _file = static_cast<File*>(file);
  return _file->vfs->onSectorSize(_file);
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int Vfs::OnDeviceCharacteristics(sqlite3_file* file) {
  auto _file = static_cast<File*>(file);
  return _file->vfs->onDeviceCharacteristics(_file);
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int Vfs::OnShmMap(sqlite3_file* file, int page, int pageSize, 
                  int extend, volatile void** pt) {
  auto _file = static_cast<File*>(file);
  return _file->vfs->onShmMap(_file, page, pageSize, extend, pt);
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int Vfs::OnShmLock(sqlite3_file* file, int offset, int n, int flags) {
  auto _file = static_cast<File*>(file);
  return _file->vfs->onShmLock(_file, offset, n, flags);
}
/***********************************************************************//**
	@brief 
***************************************************************************/
void Vfs::OnShmBarrier(sqlite3_file* file) {
  auto _file = static_cast<File*>(file);
  _file->vfs->onShmBarrier(_file);
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int Vfs::OnShmUnmap(sqlite3_file* file, int deleteFlag) {
  auto _file = static_cast<File*>(file);
  return _file->vfs->onShmUnmap(_file, deleteFlag);
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int Vfs::OnFetch(sqlite3_file* file, sqlite3_int64 offset, int amt, 
                 void** pt) {
  auto _file = static_cast<File*>(file);
  return _file->vfs->onFetch(_file, offset, amt, pt);
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int Vfs::OnUnfetch(sqlite3_file* file, sqlite3_int64 offset, void* pt) {
  auto _file = static_cast<File*>(file);
  return _file->vfs->onUnfetch(_file, offset, pt);
}
/***********************************************************************//**
	$Id$
***************************************************************************/
}
