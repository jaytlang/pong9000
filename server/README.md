# Server side operations: a summary

GET request for game data
    *name (what is the game name?)
    *host (who is the host?)
	*player (player 1 or player 2)
	*returns most recent game data (only if the player is not the one who posted last)

POST request for game data (USES BODY)
	*MUST input 5 variables, each labeled variable_1 to variable_5
	*host (who is the host?)
	*name (what is the game name?)
	*player (who is the player?)
	*returns the most recent data where the player didn't post (for the moment)

GET request for game code
	*game_name (NOT name! Used to differentiate between code and game)
	*host (who is the host?)
    *returns the most recent game code

POST request for game code (USES BODY)
	*game_name (what is the game's name?)
	*game_code (insert game code here)
	*host (who is the host)
	*returns the most recent post(for the moment)

GET request for game name and host
	*what (value doesn't matter in the slightest)
	*returns the most recent games each host is playing

POST request for reset
	*reset (value is whatever your heart desires)
	*host (who is the host you're resetting?)
    *returns "done" -- not usually invoked by an ESP32 so this is fine
	

