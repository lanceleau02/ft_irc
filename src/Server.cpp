/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsebille <hsebille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 15:09:56 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/05 10:38:27 by hsebille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* ************************************************************************** */
/*                              CLASS FUNCTIONS                               */
/* ************************************************************************** */

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

Server::~Server() {
	close(_socket);
	close(_epoll);
}

/* ************************************************************************** */
/*                              MEMBER FUNCTIONS                              */
/* ************************************************************************** */

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

void	signalHandler(int sig) {
	(void)sig;
	throw std::logic_error("server shutdown");
}

void	Server::start() {
	std::signal(SIGINT, signalHandler);
	std::signal(SIGHUP, signalHandler);
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
				User newUser("", "", userSocket, userAddress);
				client.addUser(userSocket, newUser);
				// Add user socket to epoll
                if (addSocket(_event, userSocket, _epoll) == -1) {
					serverLog(1, "Failed to add user socket to epoll instance.");
                    close(userSocket);
                    continue;
                }
            } 
			else { // Handle incoming data or other events: authenticate, set a nickname, a username, join a channel...
				// Manage events
				int	userSocket = _events[i].data.fd;
				// Select current user using its socket
				User* currentUser = const_cast<User*>(&client.getUser(userSocket));
				if (currentUser) {
					char	buffer[1024];
					int		bytes = recv(userSocket, buffer, sizeof(buffer), 0);
					// Handle error or disconnection
					if (bytes <= 0) {
						if (bytes == 0)
							serverLog(1, "User disconnected!");
						else
							serverLog(1, "Error or disconnection from client.");
						epoll_ctl(_epoll, EPOLL_CTL_DEL, userSocket, &_event);
						close(userSocket); 
						client.eraseUser(currentUser->getSocket());
						close(currentUser->getSocket());
					}
					else {
						buffer[bytes] = '\0';
						std::cout << "buffer = " << buffer << std::endl;
						executor(buffer, *currentUser);
						currentUser->display();
					}
				}
			}
        }
    }
}

/* ************************************************************************** */
/*                              UTILS FUNCTIONS                               */
/* ************************************************************************** */

int	Server::acceptConnection(sockaddr_in& userAddress) {
	socklen_t userAddressLength = sizeof(userAddress);
	return accept(_socket, reinterpret_cast<sockaddr*>(&userAddress), &userAddressLength);
}

int	Server::addSocket(epoll_event& event, int socket, int epoll) {
	event.events = EPOLLIN;
	event.data.fd = socket;
	return epoll_ctl(epoll, EPOLL_CTL_ADD, socket, &event);
}

void	Server::executor(const char* buf, User& user) {
	std::string			buffer(buf);
	std::istringstream	iss(buffer);
	std::string			line;

	while (std::getline(iss, line)) {
		if (line.find("CAP LS 302") != std::string::npos)
			continue;
		std::istringstream line_stream(line);
		std::string command;
		std::string arg;

		line_stream >> command >> arg;
		launchCommand(&user, command, arg);
	}
}

void	Server::launchCommand(User* user, const std::string& cmd, const std::string& args) {
	std::string		cmdNames[4] = {"PASS", "NICK", "USER", "JOIN"};
	typedef void	(Server::*cmds)(User&, const std::string&);
	cmds			cmdFunc[4] = {&Server::pass, &Server::nick, &Server::user, &Server::join};

	for (int i = 0; i < 4; i++)
		if (cmdNames[i] == cmd)
			(this->*cmdFunc[i])(*user, args);
}

void	Server::serverLog(int type, const std::string& log) {
	if (type == 0)
		std::cout << GREEN << log << NONE << std::endl;
	else if (type == 1)
		std::cerr << RED << log << NONE << std::endl;
}

void	Server::clientLog(int socket, const std::string& log) {
	send(socket, log.c_str(), log.size(), 0);
}
