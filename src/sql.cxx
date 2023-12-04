#include "legacystrike.hxx"

void legacystrike::sqlConnection::sqlConnect() {
    try {
        sqlDriver = get_driver_instance();
        sqlCon = sqlDriver->connect(server.data(), username.data(), password.data());
    } catch (sql::SQLException e) {
        std::cout << "Could not connect to server. Error message: " << e.what() << std::endl;
        system("pause");
        exit(1);
    }

    sqlCon->setSchema("whitelist");

    sqlPstmt = sqlCon->prepareStatement("SELECT COUNT(id) FROM whitelist");
    sqlResults = sqlPstmt->executeQuery();
    sqlResults->next();

    totalIds = sqlResults->getInt(1);

    std::cout << "connected to sql :3" << std::endl;
}
