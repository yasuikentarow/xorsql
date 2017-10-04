/***********************************************************************//**
	@file
***************************************************************************/
#include "test/Test.hpp"
/***********************************************************************//**
	@brief 
***************************************************************************/
void Test::onRun() {
  testDb();
}
/***********************************************************************//**
	@brief 
***************************************************************************/
void Test::testDb() {
  static const char* DBFILE = "test.db";
  static const struct {
    int id;
    const char* name;
    double weight;
  } ITEMS[] = {
    { 1, "yui", 50.0 }, 
    { 2, "mio", 54.0 }, 
    { 3, "ritsu", 48.0 }, 
    { 4, "mugi", 53.0 }, 
    { 5, "azusa", 46.0 }
  };
  remove(DBFILE);
  auto crypter = std::make_shared<xorsql::XorCrypter>();
  crypter->setKey(12345, 1021);
  auto vfs = std::make_shared<xorsql::CrypterVfs>(crypter);
  xorsql::SQLite3 db(DBFILE, 
                     SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, vfs);
  ATEST(db.isOpen());
  auto result =
    db.execute("create table test(id integer , name text, weight real)");
  ATEST_MESSAGE(result == xorsql::SQLite3::RESULT_DONE, "create table");
  for(auto& item : ITEMS) {
    auto statement = db.prepare("insert into test(id, name, weight) "
                                "values(?, ?, ?)");
    statement->bind(1, item.id);
    statement->bind(2, item.name);
    statement->bind(3, item.weight);
    ATEST_MESSAGE(db.step(*statement) == xorsql::SQLite3::RESULT_DONE, 
                  std::string("insert ") + item.name);
  }
  for(auto& item : ITEMS) {
    auto statement = 
      db.prepare("select name, weight from test where id = ?");
    statement->bind(1, item.id);
    if(db.step(*statement) == xorsql::SQLite3::RESULT_ROW) {
      ATEST_MESSAGE(statement->getText(0) == item.name &&
                    statement->getDouble(1) == item.weight, item.name);
    }
  }
}
/***********************************************************************//**
	$Id$
***************************************************************************/
