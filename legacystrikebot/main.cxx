#include <iostream>
#include <format>
#include <dpp/dpp.h>
#include "types.hxx"

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

std::string BOT_TOKEN = "MTA4MTM5MzM3Nzg1MDk1Mzc2MA.Gj8q4I.Ok5B0xfB9D3WKQcoUWZCHtwXjKL5P5_JvID_Z4";

const std::string server = "tcp://5.161.156.194";
const std::string username = "csgoserver";
const std::string password = "2numJ8TyXfL5pLZ";

sql::Driver* sqlDriver;
sql::Connection* sqlCon;
sql::Statement* sqlStmt;
sql::PreparedStatement* sqlPstmt;
sql::ResultSet* sqlResults;

i32 totalIds = 0;

void sqlConnect() {
    try
    {
        sqlDriver = get_driver_instance();
        sqlCon = sqlDriver->connect(server, username, password);
    }
    catch (sql::SQLException e)
    {
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

bool userHasPerms(dpp::guild_member& user) {
    for (size_t i = 0; i < user.roles.size(); ++i) {
        if ((user.roles.at(i) == (dpp::snowflake)986991989163688046) || (user.roles.at(i) == (dpp::snowflake)977338207018553354) || (user.user_id == (dpp::snowflake)911276958762041406)) 
            return true;
    }

    return false;
}

int main() {
    std::cout << "made it here!" << std::endl;

    sqlConnect();

    dpp::cluster bot(BOT_TOKEN);

    bot.on_log(dpp::utility::cout_logger());

    bot.on_slashcommand([&bot](const dpp::slashcommand_t& event) {
        if (event.command.get_command_name() == "whitelist") {
            dpp::guild_member executor = event.command.member;

            if (userHasPerms(executor)) {
                // get user
                dpp::snowflake snowflake = std::get<dpp::snowflake>(event.get_parameter("user"));
                dpp::user_identified user = bot.user_get_sync(snowflake);

                // get steamid
                std::string steamid = std::get<std::string>(event.get_parameter("steamid3"));
                i32 steamidInt = std::stoi(steamid.substr(5, steamid.length() - 6));

                // get ticket
                i64 ticket = std::get<i64>(event.get_parameter("ticket"));

                sqlPstmt = sqlCon->prepareStatement("INSERT INTO whitelist(id, discordid, steamid, discord, steamurl, ticket, extra) VALUES(?,?,?,?,?,?,?)");
                sqlPstmt->setInt(1, ++totalIds);
                sqlPstmt->setInt64(2, snowflake);
                sqlPstmt->setInt(3, steamidInt);
                sqlPstmt->setString(4, user.format_username());
                sqlPstmt->setString(5, std::format("https://steamcommunity.com/profiles/{}", steamid));
                sqlPstmt->setInt(6, ticket);
                sqlPstmt->setString(7, "");
                sqlPstmt->execute();

                bot.log(dpp::ll_info, std::format("successfully inserted player: {}, id: {}, steamInt: {}, ticket: {}, total: {}", user.format_username(), (i64)snowflake, steamidInt, ticket, totalIds));
                event.reply(std::format("successfully whitelisted {} ({})", user.format_username(), steamid));
            }
            else {
                event.reply("u don't have perms for this!!!");
            }
        }
        });

    bot.on_ready([&bot](const dpp::ready_t& event) {
        if (dpp::run_once<struct register_bot_commands>()) {
            dpp::slashcommand newcommand("whitelist", "whitelists a user", bot.me.id);
            newcommand.add_option(dpp::command_option(dpp::co_user, "user", "a mention of the user", true));
            newcommand.add_option(dpp::command_option(dpp::co_string, "steamid3", "steamid3 of the user", true));
            newcommand.add_option(dpp::command_option(dpp::co_integer, "ticket", "ticket they were added in", true));

            bot.global_command_create(newcommand);
        }
        });

    bot.start(dpp::st_wait); 
}