#include "commands.hxx"
#include <dpp/dpp.h>

bool userHasPerms(const dpp::guild_member& user) noexcept {
    for (size_t i = 0; i < user.roles.size(); ++i) {
        if ((user.roles.at(i) == dpp::snowflake(986991989163688046)) || (user.roles.at(i) == dpp::snowflake(977338207018553354)) || (user.user_id == dpp::snowflake(911276958762041406)))
            return true;
    }

    return false;
}

void legacystrike::commands::lookupUser(dpp::cluster& bot, const dpp::slashcommand_t& event) {
    Types type = static_cast<Types>(std::get<i64>(event.get_parameter("type")));
    std::string input = std::get<std::string>(event.get_parameter("input"));

    dpp::user_identified user;// = bot.user_get_sync(snowflake);
    i32 id;
    i32 steamId; // = std::stoi(steamid.substr(5, steamid.length() - 6));
    i32 ticket; //std::get<i64>(event.get_parameter("ticket"));

    switch (type) {
    case DiscordId:
        try {
            sqlConnection::sqlPstmt = sqlConnection::sqlCon->prepareStatement("SELECT * FROM whitelist WHERE discordid = ?");
            sqlConnection::sqlPstmt->setInt64(1, std::stoll(input));
        } catch (sql::SQLException e) {
            event.reply(std::format("sql error ;w; attempting to reconnect, please retry in a couple seconds"));

            sqlConnection::sqlConnect();

            return;
        }
        
        break;

    case SteamId:
        try {
            sqlConnection::sqlPstmt = sqlConnection::sqlCon->prepareStatement("SELECT * FROM whitelist WHERE steamid = ?");
            sqlConnection::sqlPstmt->setInt(1, std::stoi(input.substr(5, input.length() - 6)));
        }
        catch (sql::SQLException e) {
            event.reply(std::format("sql error ;w; attempting to reconnect, please retry in a couple seconds"));

            sqlConnection::sqlConnect();

            return;
        }
        
        break;

    case Ticket:
        try {
            sqlConnection::sqlPstmt = sqlConnection::sqlCon->prepareStatement("SELECT * FROM whitelist WHERE ticket = ?");
            sqlConnection::sqlPstmt->setInt(1, std::stoi(input));
        } catch (sql::SQLException e) {
            event.reply(std::format("sql error ;w; attempting to reconnect, please retry in a couple seconds"));

            sqlConnection::sqlConnect();

            return;
        }

        break;
    }

    bool usingUser = true;

    sqlConnection::sqlResults = sqlConnection::sqlPstmt->executeQuery();

    if (sqlConnection::sqlResults->next()) {
        id = sqlConnection::sqlResults->getInt(1);
        try {
            user = bot.user_get_sync(sqlConnection::sqlResults->getInt64(2));
        }
        catch (...) {
            usingUser = false;
        }

        steamId = sqlConnection::sqlResults->getInt(3);
        ticket = sqlConnection::sqlResults->getInt(6);

        dpp::embed embed = dpp::embed().
            set_color(dpp::colors::pink).
            set_title("Whitelist Lookup").
            set_url(sqlConnection::sqlResults->getString(5).c_str()).
            add_field(
                "User",
                ((usingUser) ? user.get_mention() : sqlConnection::sqlResults->getString(4).c_str()),
                true
            ).
            add_field(
                "SteamId",
                std::format("[U:1:{}]", steamId),
                true
            ).
            add_field(
                "Ticket",
                std::format("{}", ticket),
                true
            );

        if (usingUser)
            embed.set_thumbnail(user.get_avatar_url());

        event.reply(dpp::message(event.command.channel_id, embed).set_reference(event.command.id));
    }
    else {
        event.reply("no one found!");
    }
}

void legacystrike::commands::whitelistUser(dpp::cluster& bot, const dpp::slashcommand_t& event) {
    dpp::guild_member executor = event.command.member;

    if (userHasPerms(executor)) {
        // get user
        dpp::snowflake snowflake = std::get<dpp::snowflake>(event.get_parameter("user"));
        dpp::user_identified user = bot.user_get_sync(snowflake);

        // add/remove roles
        dpp::guild_member userMember = bot.guild_get_member_sync(event.command.guild_id, snowflake);

        bot.guild_member_add_role_sync(event.command.guild_id, snowflake, 1048055985295593503);
        bot.guild_member_remove_role_sync(event.command.guild_id, snowflake, 1053482690143342652);

        // get steamid
        std::string steamid = std::get<std::string>(event.get_parameter("steamid3"));
        i32 steamidInt = std::stoi(steamid.substr(5, steamid.length() - 6));

        // get ticket
        i64 ticket = std::get<i64>(event.get_parameter("ticket"));

        try {
            sqlConnection::sqlPstmt = sqlConnection::sqlCon->prepareStatement("INSERT INTO whitelist(id, discordid, steamid, discord, steamurl, ticket, extra) VALUES(?,?,?,?,?,?,?)");
            sqlConnection::sqlPstmt->setInt(1, ++sqlConnection::totalIds);
            sqlConnection::sqlPstmt->setInt64(2, snowflake);
            sqlConnection::sqlPstmt->setInt(3, steamidInt);
            sqlConnection::sqlPstmt->setString(4, user.format_username());
            sqlConnection::sqlPstmt->setString(5, std::format("https://steamcommunity.com/profiles/{}", steamid));
            sqlConnection::sqlPstmt->setInt(6, ticket);
            sqlConnection::sqlPstmt->setString(7, "");
            sqlConnection::sqlPstmt->execute();
        }
        catch (sql::SQLException e) {
            event.reply(std::format("sql error ;w; attempting to reconnect, please retry in a couple seconds"));

            sqlConnection::sqlConnect();

            return;
        }

        bot.log(dpp::ll_info, std::format("successfully inserted player: {}, id: {}, steamInt: {}, ticket: {}, total: {}", user.format_username(), (i64)snowflake, steamidInt, ticket, sqlConnection::totalIds));
        event.reply(std::format("successfully whitelisted {} ({})", user.format_username(), steamid));
    }
    else {
        event.reply("u don't have perms for this!!!");
    }
}