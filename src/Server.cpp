/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 15:09:56 by laprieur          #+#    #+#             */
/*   Updated: 2023/12/23 19:01:30 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(char** params) {
	std::string	port(params[1]);
	if (port.find_first_not_of("0123456789") != std::string::npos)
		throw std::invalid_argument("invalid port number.");
	std::string password(params[2]);
	if (password.empty())
		throw std::invalid_argument("invalid connection password.");
	_port = atoi(port.c_str());
	_password = password;
}

Server::~Server() {}

void	Server::setup() {
	// Create a server socket
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == -1)
		throw std::runtime_error("impossible to create the server socket.");
	// Set up the socket structure
	memset(&_serverAddress, 0, sizeof(_serverAddress));
	_serverAddress.sin_family = AF_INET;
	_serverAddress.sin_addr.s_addr = INADDR_ANY;
	_serverAddress.sin_port = htons(_port);
	// Bind the server socket to a port
	if (bind(_socket, reinterpret_cast<sockaddr*>(&_serverAddress), sizeof(_serverAddress)) == -1) {
		close(_socket);
		throw std::runtime_error("failed to bind socket.");
	}
	// Listen for incoming connections
	if (listen(_socket, 5) == -1) {
		close(_socket);
		throw std::runtime_error("failed to listen.");
    }
	// Create an epoll() instance
	_epoll = epoll_create1(0);
	if (_epoll == -1) {
		close(_socket);
		throw std::runtime_error("failed to create epoll instance.");
	}
	// Set up the epoll structure
	_event.events = EPOLLIN;
	_event.data.fd = _socket;
	// Add server socket to epoll
	if (epoll_ctl(_epoll, EPOLL_CTL_ADD, _socket, &_event) == -1) {
		close(_epoll);
		close(_socket);
		throw std::runtime_error("failed to add socket to epoll.");
	}
}

int		Server::acceptConnection(sockaddr_in& userAddress) {
	socklen_t userAddressLength = sizeof(userAddress);
	return accept(_socket, reinterpret_cast<sockaddr*>(&userAddress), &userAddressLength);
}

int		Server::addSocket(epoll_event& event, int socket, int epoll) {
	event.events = EPOLLIN;
	event.data.fd = socket;
	return epoll_ctl(epoll, EPOLL_CTL_ADD, socket, &event);
}

void	Server::launchCommand(char* buffer, int bytes, User* user) {
	buffer[bytes] = '\0';
	std::string buf(buffer);
	std::istringstream iss(buf);
	std::string command, arg;
	iss >> command;
	std::getline(iss >> std::ws, arg);
	if (buf.compare(0, 4, "PASS") == 0)
		pass(*user, arg);
	else if (buf.compare(0, 4, "NICK") == 0)
		nick(*user, arg);
	else if (buf.compare(0, 4, "USER") == 0)
		this->user(*user, arg);
	/* else if (buf == "join")
		join();
	else if (buf == "privmsg")
		privmsg(); */
}

void	Server::start() {
	Client client;
	// Main event loop
	while (true) {
		// Wait for events on the epoll instance
        int numEvents = epoll_wait(_epoll, _events, 10, -1);
        if (numEvents == -1)
			throw std::runtime_error("failed to wait for events.");
        for (int i = 0; i < numEvents; ++i) {
			// Handle new users
            if (_events[i].data.fd == _socket) {
                // Accept new user connection
				struct sockaddr_in	userAddress;
				int userSocket = acceptConnection(userAddress);
                if (userSocket == -1) {
                    serverLog(1, "Failed to accept user connection.");
                    continue;
				}
				serverLog(0, "New user connected!");
				// Create new user
				User newUser("", "", REGULAR, userSocket, false, userAddress);
				client.addUser(newUser);
				// Add user socket to epoll
                if (addSocket(_event, userSocket, _epoll) == -1) {
					serverLog(1, "Failed to add user socket to epoll instance.");
                    close(userSocket);
                    continue;
                }
            }
			// Handle incoming data or other events: authenticate, set a nickname, a username, join a channel...
			else {
				// Manage events
				int		userSocket = _events[i].data.fd;
				User*	currentUser = NULL;
				// Select current user using its socket
				for (size_t j = 0; j < client.getNbUsers(); ++j) {
					if (client.getUser(j).getSocket() == userSocket) {
						currentUser = const_cast<User*>(&client.getUser(j));
						break;
					}
				}
				if (currentUser) {
					char	buffer[1024];
					int		bytes = recv(userSocket, buffer, sizeof(buffer), 0);
					// Handle error or disconnection
					if (bytes <= 0) {
						if (bytes == 0)
							serverLog(1, "User disconnected!");
						else
							serverLog(1, "Error or disconnection from client.");
						close(userSocket);
						epoll_ctl(_epoll, EPOLL_CTL_DEL, userSocket, NULL);
						/* client._users.erase(std::remove_if(client._users.begin(), client._users.end(),
							[&](const Client& c) { return c._users._socket == userSocket; }), client._users.end()); */
					}
					// Launch commands
					else
						launchCommand(buffer, bytes, currentUser);
				}
			}
        }
    }
	close(_socket);
	close(_epoll);
}

/* ************************************************************************** */
/*                                    UTILS                                   */
/* ************************************************************************** */

void	Server::serverLog(int type, std::string log) {
	if (type == 0)
		std::cout << GREEN << log << NONE << std::endl;
	else if (type == 1)
		std::cerr << RED << log << NONE << std::endl;
}

void	Server::clientLog(const User& user, int socket, int logCode, std::string cmd) {
	std::stringstream	logMessage;
	char				test[256];
	
	if (logCode == RPL_WELCOME) {
		(void)cmd;
		logMessage << "Welcome to the Internet Relay Network " << user.getNickname() << "!" << user.getUsername() << "@" << gethostname(test, sizeof(test)) << std::endl;
	}
	if (logCode == ERR_PASSWDMISMATCH)
		logMessage << cmd << ": Password incorrect" << std::endl;
	if (logCode == ERR_NEEDMOREPARAMS)
		logMessage << cmd << ": Not enough parameters" << std::endl;
	if (logCode == ERR_ALREADYREGISTRED)
		logMessage << cmd << ": Unauthorized command (already registered)" << std::endl;
	if (logCode == ERR_NONICKNAMEGIVEN)
		logMessage << cmd << ": No nickname given" << std::endl;
	if (logCode == ERR_ERRONEUSNICKNAME)
		logMessage << cmd << ": Erroneous nickname" << std::endl;
	send(socket, (logMessage.str()).c_str(), (logMessage.str()).size(), 0);
}
