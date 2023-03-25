#pragma once

#include "legacystrike.hxx"

namespace legacystrike::commands {
	void lookupUser(dpp::cluster& bot, const dpp::slashcommand_t& event);
	void whitelistUser(dpp::cluster& bot, const dpp::slashcommand_t& event);
}