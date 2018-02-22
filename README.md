# mySlack

## SERVER

The server can receive multiple connections from different clients.

A config file can be written by the server administrator in order to choose the server port, the maximum number of simultaneously connected clients and the list of default channels.
By default the connection port is 12345, maximum clients number is 4 and default channels is one channel named General.

When a new client connects, the server notifies every user (on every channel), same behaviour when someone disconnects.

The server broadcasts any message sent by a user to all connected clients on this user's channel.

- Handles multiple connections
- Accessible from anywhere
- Maximum connected client number
- Server configuration file : port, maximum clients number, default channels list
- Connection / Disconnection notification to all users
- Channel message broadcasting

## CLIENT

When launching the client, the user must pass multiple parameters in the CLI : host address, port, nickname, and optionally a channel.

The client will then be connected to the server and will be assigned the chosen nickname. If a channel was defined in the CLI arguments, then the client would automatically join this channel. Otherwise, the client will join the default channel.

When the server sends a message to the client, the message is displayed. Generally it is a channel message but sometimes a server message can be sent (in this case the message is marked as a server message).

- CLI parameters : host address, port, nickname, (optional) channel
- Checks if the nickname is already used
- Connects the user to the requested channel (if it exists)
- Connects the user to the server default channel if no channel is specified in the CLI

## CHANNELS

When a user is connected to a channel, it receives every message sent to this channel (by other users) and can also send messages to the other users connected to the channel.

It is possible to list all server channels thanks to the command `/list_channels`. To join a channel, the user must use `/join <channel>`. If the channel exists the user will join it otherwise the user can create it using the following command : `/create <channel>` which will create the channel and move the user inside of it. To leave a channel the user must use the `/leave` command, it will move him to the server default channel.

## COMMANDS

There are multiple commands implemented on our chat system, server and client side.

Client commands :
- `/send_message <message>` sends a message to the channel (is the same than not using any command)
- `/list_commands` displays the list of available commands on the server
- `/commands_list` is an alias of `/list_commands`
- `/help <command>` gives informations about a command, or explains how to get the list of available commands
- `/direct_message <nickname> <message>` sends a direct message to the specified user (only this user will be able to see the direct message)
- `/list_channels` lists server channels
- `/join <channel>` joins an existing channel
- `/create <channel>` creates a new channel
- `/leave` moves the user to the default server channel
- `/ping` answers the message `Pong` to the sender or ping a user
- `/wizz <nickname>` Wizz (annoy) a user
- `/nickname <nickname>` changes the nickname of the user
- `/important <message>` sends an important message to all the connected users (no matter what their channel is)
- `/color <color>` changes the user messages' color 
- `/bg_color <color>` changes the user messages' background color
- `/list_colors` lists all available colors for the messages and messages' background
- `/reset_color` resets the user messages' color
- `/reset_bg_color` resets the user messages' background color
- `/logout` disconnects the user

Server commands :
- `/send_message <message>` sends a message to all connected users
- `/kick <nickname> [reason]` disconnects a user from the server
- `/create_channel <channel>` creates a channel
- `/remove_channel <channel>` removes a channel (and move all its users to default channel)
- `/mute <nickname>` mutes a user (he can't send messages anymore)
- `/unmute <nickname>` unmutes a user
- `/list_commands` lists all available commands
- `/stop` stops the server

If a wrong command is sent, the server answers a help message to the user.

## Docker

### Building the container
```bash
$ make build
```
Or
```bash
$ docker build -t myslack .
```

### Creating the network
```bash
$ make create_network
```
Or
```bash
$ docker network create -d bridge myslack
```

### Running the server
```bash
$ make start_server_container
```
Or
```bash
$ docker run --rm -it -v "$PWD:/usr/src/app" -w /usr/src/app --name server --net myslack myslack
```

Then
```bash
cd server && make re && ./server
```


### Running a client
```bash
$ make start_client_container [NUMBER=<number>]
```
Or
```bash
$ docker run --rm -it -v "$PWD:/usr/src/app" -w /usr/src/app --name client_1 --net myslack myslack
```

Then
```bash
cd client && make re
```
Finally
```bash
./client server <port> <nickname>
```