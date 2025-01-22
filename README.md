<div align="center">

![](https://raw.githubusercontent.com/ayogun/42-project-badges/refs/heads/main/badges/ft_irce.png)

# **ft_irc**

**Internet Relay Chat**

</div>

## Features

- Implement an IRC server compliant with the RFC 2812 protocol.
- Support multiple simultaneous client connections using sockets.
- Handle basic IRC commands such as `NICK`, `USER`, `JOIN`, `PART`, `PRIVMSG`, and `QUIT`.
- Manage user authentication and connection registration.
- Enable private messaging and public channel communication.
- Support operator privileges for managing channels and users.
- Ensure server stability and proper error handling under load.

## Installation

1. Clone the repository:

```bash
git clone https://github.com/lanceleau02/ft_irc.git
```

2. Navigate to the project directory:

```bash
cd ft_irc
```

3. Compile the program:

```bash
make
```

## Usage

The server can be run as follows:

```bash
./ircserv <port> <password>
```

Then, you need to connect to the server thanks to a client (e.g., HexChat) or thanks to the terminal by using Netcat:

```bash
nc localhost <port>
```

Here's the list of the available commands:

- `BOT`: Use the bot.
- `INVITE`: Invite a client to a channel.
- `JOIN`: Join a channel.
- `KICK`: Eject a client from the channel.
- `MODE`: Change the channel’s mode:
    - `i`: Set/remove Invite-only channel.
    - `t`: Set/remove the restrictions of the TOPIC command to channel operators.
    - `k`: Set/remove the channel key (password).
    - `o`: Give/take channel operator privilege.
    - `l`: Set/remove the user limit to channel.
- `NICK`: Set the client's nickname.
- `PASS`: Enter the server's password.
- `PRIVMSG`: Send a private message to a client.
- `TOPIC`: Change or view the channel topic.
- `USER`: Set the client's username.

## License

This project is licensed under the **42 School** License.

- **Educational Use Only**: This project is intended for educational purposes at the 42 School as part of the curriculum.
- **Non-commercial Use**: The code may not be used for commercial purposes or redistributed outside of the 42 School context.
- **No Warranty**: The project is provided "as-is", without any warranty of any kind.

For more details, see the [LICENSE](https://github.com/lanceleau02/ft_irc/blob/main/LICENSE) file.

## Resources

- [IRC (Wikipedia)](https://en.wikipedia.org/wiki/IRC)
- [RFC 1459 - Internet Relay Chat Protocol (Datatracker)](https://datatracker.ietf.org/doc/html/rfc1459)
