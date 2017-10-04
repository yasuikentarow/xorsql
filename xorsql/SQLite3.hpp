/***********************************************************************//**
	@file
***************************************************************************/
#pragma once

namespace xorsql {
/***********************************************************************//**
	@brief SQLite3
***************************************************************************/
class SQLite3 {
 public:
  class Statement;

  enum Result {
    RESULT_DONE, 
    RESULT_ROW, 
    RESULT_ERROR
  };

 private:
  sqlite3* db_;
  std::shared_ptr<Vfs> vfs_;

 public:
  SQLite3(const char* path, int flag, std::shared_ptr<Vfs> vfs = nullptr);
  ~SQLite3();

  bool isOpen() const;

  Result execute(const char* query);

  std::shared_ptr<Statement> prepare(const char* query);
  Result step(Statement& statement);
};
/***********************************************************************//**
	@brief 
***************************************************************************/
class SQLite3::Statement {
 private:
  sqlite3_stmt* stmt_;

 public:
  Statement(sqlite3_stmt* stmt);
  ~Statement();

  sqlite3_stmt* getStmt() const {
    return stmt_;
  }

  bool bind(int index, int value);
  bool bind(int index, double value);
  bool bind(int index, const std::string& value);

  int getInt(int index) const;
  double getDouble(int index) const;
  std::string getText(int index) const;
};
/***********************************************************************//**
	$Id$
***************************************************************************/
}
