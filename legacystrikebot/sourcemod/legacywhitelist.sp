#include <sourcemod>
#include <sdktools>
#include <clientprefs>

#pragma semicolon 1
#pragma newdecls required

#define PLUGIN_VERSION "1337"

Database db = null;

public Plugin myinfo = 
{
	name = "legacy whitelist", 
	author = "may", 
	description = "i love kaworu", 
	version = PLUGIN_VERSION, 
	url = "https://github.com/lilac1337"
};

public void OnPluginStart()
{
	char error[255];
	db = SQL_Connect("whitelist", true, error, sizeof(error));
	if (!db) LogError(error);
	SQL_Query(db, "CREATE TABLE IF NOT EXISTS whitelist (id INT, discordid BIGINT, steamid INT, discord VARCHAR(37), steamurl VARCHAR(256), ticket INT, extra VARCHAR(256))");
}

public void OnClientAuthorized(int client, const char[] auth)
{
	char query[512];
	int steamid = GetSteamAccountID(client, true);
	FormatEx(query, sizeof(query),
	         "SELECT * FROM whitelist WHERE steamid = %d",
	         steamid);
	db.Query(SQL_CheckClient, query, GetClientSerial(client));
}

void SQL_CheckClient(Database callbackDb, DBResultSet results, const char[] error, int serial)
{
	if (results == null)
	{
		LogError("SQL_CheckClient error: '%s'", error);
		return;
	}

	int client = GetClientFromSerial(serial);
	if (client < 1) return;

	if (!results.FetchRow())
	{
		KickClient(client, "you are not whitelisted! please join the discord and apply!");
	}
}