#include "legacystrike.hxx"

void legacystrike::botHandler::handleBot() {
    constexpr std::string_view BOT_TOKEN = "MTA4MTM5MzM3Nzg1MDk1Mzc2MA.Gj8q4I.Ok5B0xfB9D3WKQcoUWZCHtwXjKL5P5_JvID_Z4";

    dpp::cluster bot(BOT_TOKEN.data());

    bot.on_log(dpp::utility::cout_logger());

    bot.on_slashcommand([&bot](const dpp::slashcommand_t& event) {
        if (event.command.get_command_name() == "lookup")
            legacystrike::commands::lookupUser(bot, event);

        if (event.command.get_command_name() == "whitelist")
            legacystrike::commands::whitelistUser(bot, event);
        });

    bot.on_channel_create([&bot](const dpp::channel_create_t& event) {
        std::string* name = &event.created->name;

        if (name->contains("ticket") && std::stoi(name->substr(7, 4)) > 6799) {
            bot.message_create_sync(dpp::message(event.created->id, "Hello - **PLEASE** post ALL Steam accounts here please - banned ones included. \n**UNPRIVATE GAMES & HOURS** - a mod will be here eventually to help.\n\nIf you have a VAC / Gameban please state the game"));
        }
        });

    bot.on_ready([&bot](const dpp::ready_t& event) {
        if (dpp::run_once<struct register_bot_commands>()) {
            dpp::slashcommand whitelist("whitelist", "whitelists a user", bot.me.id);
            whitelist.add_option(dpp::command_option(dpp::co_user, "user", "a mention of the user", true));
            whitelist.add_option(dpp::command_option(dpp::co_string, "steamid3", "steamid3 of the user", true));
            whitelist.add_option(dpp::command_option(dpp::co_integer, "ticket", "ticket they were added in", true));

            dpp::slashcommand lookup("lookup", "looks up a user", bot.me.id);
            lookup.add_option(dpp::command_option(dpp::co_integer, "type", "the type used as input", true).
                add_choice(dpp::command_option_choice("discord id", 0ll)).
                add_choice(dpp::command_option_choice("steamid 3", 1ll)).
                add_choice(dpp::command_option_choice("ticket", 2ll))
            );
            lookup.add_option(dpp::command_option(dpp::co_string, "input", "input", true));

            bot.global_command_create(whitelist);
            bot.global_command_create(lookup);
        }
        });

    bot.start(dpp::st_wait);
}