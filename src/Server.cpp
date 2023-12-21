/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 15:09:56 by laprieur          #+#    #+#             */
/*   Updated: 2023/12/21 15:34:23 by laprieur         ###   ########.fr       */
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

void	Server::clientLog(const User& user, int socket, int logCode, std::string cmd) {
	std::stringstream	logMessage;
	char				test[256];
	
	if (logCode == RPL_WELCOME) {
		(void)cmd;
		logMessage << "Welcome to the Internet Relay Network " << user.getNickname() << "!" << user.getUsername() << "@" << gethostname(test, sizeof(test));
	}
	if (logCode == ERR_NEEDMOREPARAMS)
		logMessage << cmd << ": Not enough parameters";
	if (logCode == ERR_ALREADYREGISTRED)
		logMessage << cmd << ": Unauthorized command (already registered)";
	send(socket, (logMessage.str()).c_str(), (logMessage.str()).size(), 0);
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
            if (_events[i].data.fd == _socket) {
                // Accept new client connection
				struct sockaddr_in	userAddress;
                socklen_t userAddressLength = sizeof(userAddress);
                int userSocket = accept(_socket, reinterpret_cast<sockaddr*>(&userAddress), &userAddressLength);
                if (userSocket == -1) {
                    std::cerr << RED << "Error: failed to accept client connection." << NONE << std::endl;
                    continue;
				}
				std::cout << "New client connected!" << std::endl;

				// Create a new User instance
				User	newUser;
				newUser.setSocket(userSocket);
				newUser.setAddress(userAddress);
				client.addUser(newUser);

				// Add client socket to epoll
                _event.events = EPOLLIN;
                _event.data.fd = userSocket;
                if (epoll_ctl(_epoll, EPOLL_CTL_ADD, userSocket, &_event) == -1) {
                    std::cerr << RED << "Error: failed to add client socket to epoll instance." << NONE << std::endl;
                    close(userSocket);
                    continue;
                }
				//clientLog(users, userSocket, RPL_WELCOME);
            }
			else {
				// Handle incoming data or other events: authenticate, set a nickname, a username, join a channel...
				int		userSocket = _events[i].data.fd;

				User* currentUser = NULL;

				for (size_t j = 0; j < client.getNbUsers(); ++j) {
					if (client.getUser(j).getSocket() == userSocket) {
						currentUser = const_cast<User*>(&client.getUser(j));
						break;
					}
				}

				if (currentUser) {
					char	buffer[1024];
					int		bytes = recv(userSocket, buffer, sizeof(buffer), 0);
					if (bytes <= 0) {
						if (bytes == 0)
							std::cout << "Client disconnected!" << std::endl;
						else
							std::cout << "Error or disconnection from client." << std::endl;
						close(userSocket);
						epoll_ctl(_epoll, EPOLL_CTL_DEL, userSocket, NULL);
						/* client._users.erase(std::remove_if(client._users.begin(), client._users.end(),
							[&](const Client& c) { return c._users._socket == userSocket; }), client._users.end()); */
					}
					else {
						buffer[bytes] = '\0';
						std::string buf(buffer);
						std::istringstream iss(buf);
						std::string command, arg;
						iss >> command;
						std::getline(iss >> std::ws, arg);
						if (buf.compare(0, 4, "PASS") == 0)
							pass(*currentUser, arg);
						else if (buf.compare(0, 4, "NICK") == 0) {
							nick(*currentUser, arg);
						}
/* 						else if (buf == "user")
							user();
						else if (buf == "join")
							join();
						else if (buf == "privmsg")
							privmsg(); */
					}
				}
			}
        }
    }
	close(_socket);
	close(_epoll);
}
