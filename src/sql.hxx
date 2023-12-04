#pragma once

#include "legacystrike.hxx"

namespace legacystrike::sqlConnection {
	inline int totalIds = 0;

	constexpr std::string_view server = "tcp://5.161.156.194";
	constexpr std::string_view username = "whitelist";
	constexpr std::string_view password = "Wn8BkckU%uyQhLTxs^u";

	inline sql::Driver* sqlDriver;
	inline sql::Connection* sqlCon;
	inline sql::Statement* sqlStmt;
	inline sql::PreparedStatement* sqlPstmt;
	inline sql::ResultSet* sqlResults;

	void sqlConnect();
}
