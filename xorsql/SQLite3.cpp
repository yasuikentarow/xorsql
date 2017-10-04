/***********************************************************************//**
	@file
***************************************************************************/
#include "xorsql/SQLite3.hpp"
#include "xorsql/Vfs.hpp"

namespace xorsql {
/***********************************************************************//**
	@brief コンストラクタ
***************************************************************************/
SQLite3::SQLite3(const char* path, int flag, std::shared_ptr<Vfs> vfs)
  : db_(nullptr), 
    vfs_(vfs)
{
  sqlite3_open_v2(path, &db_, flag, 
                  vfs_ ? vfs_->getName().c_str() : nullptr);
}
/***********************************************************************//**
	@brief デストラクタ
***************************************************************************/
SQLite3::~SQLite3() {
  if(isOpen()) {
    sqlite3_close(db_);
  }
}
/***********************************************************************//**
	@brief 
***************************************************************************/
bool SQLite3::isOpen() const {
  return db_;
}
/***********************************************************************//**
	@brief 
***************************************************************************/
SQLite3::Result SQLite3::execute(const char* query) {
  if(auto statement = prepare(query)) {
    return step(*statement);
  }
  return RESULT_ERROR;
}
/***********************************************************************//**
	@brief 
***************************************************************************/
std::shared_ptr<SQLite3::Statement> SQLite3::prepare(const char* query) {
  if(isOpen()) {
    sqlite3_stmt* stmt = nullptr;
    if(sqlite3_prepare(db_, query, -1, &stmt, nullptr) == SQLITE_OK) {
      return std::make_shared<Statement>(stmt);
    }
  }
  return nullptr;
}
/***********************************************************************//**
	@brief 
***************************************************************************/
SQLite3::Result SQLite3::step(Statement& statement) {
  switch(sqlite3_step(statement.getStmt())) {
  case SQLITE_DONE:
    return RESULT_DONE;
  case SQLITE_ROW:
    return RESULT_ROW;
  default:
    break;
  }
  return RESULT_ERROR;
}
/***********************************************************************//**
	@brief 
***************************************************************************/
SQLite3::Statement::Statement(sqlite3_stmt* stmt)
  : stmt_(stmt)
{
}
/***********************************************************************//**
	@brief デストラクタ
***************************************************************************/
SQLite3::Statement::~Statement() {
  sqlite3_finalize(stmt_);
}
/***********************************************************************//**
	@brief 
***************************************************************************/
bool SQLite3::Statement::bind(int index, int value) {
  return sqlite3_bind_int(stmt_, index, value) == SQLITE_OK;
}
/***********************************************************************//**
	@brief 
***************************************************************************/
bool SQLite3::Statement::bind(int index, double value) {
  return sqlite3_bind_double(stmt_, index, value) == SQLITE_OK;
}
/***********************************************************************//**
	@brief 
***************************************************************************/
bool SQLite3::Statement::bind(int index, const std::string& value) {
  return sqlite3_bind_text(stmt_, index, value.c_str(), int(value.size()), 
                           SQLITE_TRANSIENT) == SQLITE_OK;
}
/***********************************************************************//**
	@brief 
***************************************************************************/
int SQLite3::Statement::getInt(int index) const {
  return sqlite3_column_int(stmt_, index);
}
/***********************************************************************//**
	@brief 
***************************************************************************/
double SQLite3::Statement::getDouble(int index) const {
  return sqlite3_column_double(stmt_, index);
}
/***********************************************************************//**
	@brief 
***************************************************************************/
std::string SQLite3::Statement::getText(int index) const {
  return std::string(reinterpret_cast<const char*>
                     (sqlite3_column_text(stmt_, index)));
}
/***********************************************************************//**
	$Id$
***************************************************************************/
}
