/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 15:09:56 by laprieur          #+#    #+#             */
/*   Updated: 2023/12/28 11:39:46 by laprieur         ###   ########.fr       */
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

void	Server::registerUser(User& user, const std::string& userInfos) {
	std::istringstream iss(userInfos);
	std::string line;
	while (std::getline(iss, line)) {
		std::istringstream field(line);
		std::string command, value;
		if (field >> command >> value) {
			if (command == "PASS") {
				std::string password = value;
				if (password == _password && !user.getAuthentication()) {
					user.setAuthentication();
					serverLog(0, "User successfully authentified!");
				}
			} else if (command == "NICK") {
				std::string nickname = value;
				if ((user.getNickname()).empty()) {
					this->nick(user, nickname);
					serverLog(0, "New nickname setted!");
				}
			} else if (command == "USER") {
				std::string username = value;
				if ((user.getUsername()).empty()) {
					this->user(user, username);
					serverLog(0, "New username setted!");
				}
			}
		}
	}
	if (user.getAuthentication() && !(user.getNickname()).empty() && !(user.getUsername()).empty())
		user.setRegistration();
}

void	Server::launchCommand(User* user, const std::string& cmd, const std::string& args) {
	std::string		cmdNames[3] = {"pass", "nick", "user"};
	typedef void	(Server::*cmds)(User&, const std::string&);
	cmds			cmdFunc[3] = {&Server::pass, &Server::nick, &Server::user};

	for (int i = 0; i < 3; i++)
		if (cmdNames[i] == cmd)
			(this->*cmdFunc[i])(*user, args);
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
				User newUser("", "", REGULAR, userSocket, false, false, userAddress);
				client.addUser(newUser);
				// Add user socket to epoll
                if (addSocket(_event, userSocket, _epoll) == -1) {
					serverLog(1, "Failed to add user socket to epoll instance.");
                    close(userSocket);
                    continue;
                }
            } else { // Handle incoming data or other events: authenticate, set a nickname, a username, join a channel...
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
					} else { // Launch commands
						buffer[bytes] = '\0';
						std::cout << "buffer = " << buffer << std::endl;
						std::string buf(buffer);
						if (!(currentUser->getAuthentication()) && (currentUser->getNickname()).empty() && (currentUser->getUsername()).empty())
							registerUser(*currentUser, buf);
						std::istringstream iss(buf);
						std::string command, args;
						iss >> command;
						std::getline(iss >> std::ws, args);
						launchCommand(currentUser, command, args);
					}
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

void	Server::serverLog(int type, const std::string& log) {
	if (type == 0)
		std::cout << GREEN << log << NONE << std::endl;
	else if (type == 1)
		std::cerr << RED << log << NONE << std::endl;
}

void	Server::clientLog(int socket, const std::string& log) {
	std::cout << "je suis un client log" << std::endl;
	send(socket, log.c_str(), log.size(), 0);
}
