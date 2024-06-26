//===----------------------------------------------------------------------===//
//                         DuckDB
//
// mysql_scanner.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb.hpp"
#include "mysql_utils.hpp"
#include "mysql_connection.hpp"

namespace duckdb {
class MySQLTableEntry;
class MySQLTransaction;

struct MySQLBindData : public FunctionData {
	explicit MySQLBindData(MySQLTableEntry &table) : table(table) {
	}

	MySQLTableEntry &table;
	vector<MySQLType> mysql_types;
	vector<string> names;
	vector<LogicalType> types;
	string limit;

public:
	unique_ptr<FunctionData> Copy() const override {
		throw NotImplementedException("MySQLBindData copy not supported");
	}
	bool Equals(const FunctionData &other_p) const override {
		return false;
	}
};

class MySQLScanFunction : public TableFunction {
public:
	MySQLScanFunction();
};

class MySQLQueryFunction : public TableFunction {
public:
	MySQLQueryFunction();
};

class MySQLClearCacheFunction : public TableFunction {
public:
	MySQLClearCacheFunction();

	static void ClearCacheOnSetting(ClientContext &context, SetScope scope, Value &parameter);
};

class MySQLExecuteFunction : public TableFunction {
public:
	MySQLExecuteFunction();
};

} // namespace duckdb
