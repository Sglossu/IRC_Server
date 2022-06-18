
Internet Relay Chat or IRC is a text-based communication protocol on the Internet. It offers real-time messaging that can be either public or private. Users can exchange direct messages and join group channels.

IRC clients connect to IRC servers in order to join channels. IRC servers are connected together to form a network.

__<h2>Server start</h2>__ (think up password for connection)

    ./ircserv 6667 your_password

__<h2>Connect</h2>__ through any client (Adium, LimeChat and etc) or using simple command with nc:

    nc localhost 6667

A bot is attached to the server that requires the weather using [api.openweathermap.org]("https://api.openweathermap.org/)

Launch bot:
```
python3 bots/first_bot.py 
```
Example command for bot:
```
PRIVMSG ded :weather Kazan
// :ded!Maksim@127.0.0.1 NOTICE a :Сейчас в kazan облачно с прояснениями, 22.01 градусов. Сила ветра 4.74.
```
---
__<h2>Registration</h2>__

Mandatory conditions for user registration:

* PASS-message
* NICK-message
* USER-message

Example for the first commands through _nc_:

    USER Ivan
    NICK kotik777
    PASS 1234

__<h2>Send message</h2>__

To write someone a message, use command PRIVMSG. If you want to send more than 1 word to client, use : (colon) before the message. For example:

    PRIVMSG max :hello, what's up?

All commands you can find in [IRC documentation](https://www.lissyara.su/doc/rfc/rfc1459/)