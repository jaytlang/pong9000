# Server side operations: a summary

*TL;DR, post your game code [here](https://web.mit.edu/jaytlang/postcode/)*

Our server architecture is fairly unique, in that each game on the server is associated with a specific hostname. Concurrently, each arduino has a hard-coded hostname, and software like the browser references this name when deciding which games to obtain. So, a lot of these calls feature `host` parameters - one can think of these as pointers to fellow peers on the net, and you can use these pointers to perform a variety of operations both in and out of the game sandbox. Examples are shown below.

## Retrieving and posting game data
Game data is stored on the server as tuples of length 5, representing a set of game-specific variables tweakable per implementation. You can `GET` or `POST` to mutate these variables, as follows:

```
GET(name, host, playernumber) => most recent game data, given game name
POST(variable[i] for i in range(5), host, name, playernumber)
```

Here, `name` represents the game name in question. This uses a different definition than the `game_name` identifier used to extract game code for the JIT, as shown below:

## Retrieving and posting game code

Getting/Posting code is done frequently within the JIT environment to set up game runtimes. The API for this is straightforward - note that `game_name` is used in code-related identification

```
GET(game_name, host) => game code
POST(game_name, host, game_code)
```
## Other operations

As used in the browser, and frequently for debugging the API, one can obtain a host-game mapping that accurately reflects the database state with a generic GET request:
```
GET() => game-host mapping for all hosts
```

Resetting a host to contain no code can be done with the following POST, though this isn't often used nor recommended:
```
POST(reset=*, host) => triggers a game reset, returns 'Done resetting!'
```

