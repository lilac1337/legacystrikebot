#pragma once

#include "legacystrike.hxx"

namespace legacystrike::sqlConnection {
	inline int totalIds = 0;

	constexpr std::string_view server = "tcp://5.161.229.34";
	constexpr std::string_view username = "may";
	constexpr std::string_view password = "epic_password_1337";

	inline sql::Driver* sqlDriver;
	inline sql::Connection* sqlCon;
	inline sql::Statement* sqlStmt;
	inline sql::PreparedStatement* sqlPstmt;
	inline sql::ResultSet* sqlResults;

	void sqlConnect();
}
