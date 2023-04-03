#pragma once

#include "legacystrike.hxx"

namespace legacystrike::commands {
	void checkUser(dpp::cluster& bot, const dpp::slashcommand_t& event);
	void lookupUser(dpp::cluster& bot, const dpp::slashcommand_t& event);
	void whitelistUser(dpp::cluster& bot, const dpp::slashcommand_t& event);
}