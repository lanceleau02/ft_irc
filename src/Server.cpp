/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 15:47:27 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/08 16:44:00 by laprieur         ###   ########.fr       */
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
	int opt = 1;
    setsockopt(this->_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
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
	// Main event loop
	while (true) {
		// Wait for events on the epoll instance
        int numEvents = epoll_wait(_epoll, _events, 10, -1);
        if (numEvents == -1)
			throw std::runtime_error("failed to wait for events.");
        for (int i = 0; i < numEvents; ++i) {
			// Handle new clients
            if (_events[i].data.fd == _socket) {
                // Accept new client connection
				struct sockaddr_in	clientAddress;
				int clientSocket = acceptConnection(clientAddress);
                if (clientSocket == -1) {
                    serverLog(1, "Failed to accept client connection.");
                    continue;
				}
				serverLog(0, "New client connected!");
				// Create new client
				Client newClient(clientSocket, clientAddress);
				addClient(newClient);
				// Add client socket to epoll
                if (addSocket(_event, clientSocket, _epoll) == -1) {
					serverLog(1, "Failed to add client socket to epoll instance.");
                    close(clientSocket);
                    continue;
                }
            } 
			else { // Handle incoming data or other events: authenticate, set a nickname, a username, join a channel...
				// Manage events
				int	clientSocket = _events[i].data.fd;
				// Select current client using its socket
				char	buffer[1024];
				int		bytes = recv(clientSocket, buffer, sizeof(buffer), 0);
				// Handle error or disconnection
				if (bytes <= 0) {
					if (bytes == 0)
						serverLog(1, "User disconnected!");
					else
						serverLog(1, "Error or disconnection from client.");
					epoll_ctl(_epoll, EPOLL_CTL_DEL, clientSocket, &_event);
					close(clientSocket); 
					eraseClient(clientSocket);
					close(clientSocket);
				}
				else {
					buffer[bytes] = '\0';
					std::cout << "buffer = " << buffer << std::endl;
					executor(buffer, _clients.at(clientSocket));
					_clients.at(clientSocket).display();
				}
			}
        }
    }
}

/* ************************************************************************** */
/*                              UTILS FUNCTIONS                               */
/* ************************************************************************** */

int	Server::acceptConnection(sockaddr_in& clientAddress) {
	socklen_t clientAddressLength = sizeof(clientAddress);
	return accept(_socket, reinterpret_cast<sockaddr*>(&clientAddress), &clientAddressLength);
}

int	Server::addSocket(epoll_event& event, int socket, int epoll) {
	event.events = EPOLLIN;
	event.data.fd = socket;
	return epoll_ctl(epoll, EPOLL_CTL_ADD, socket, &event);
}

bool	Server::findClientByNick(std::string nickname) {
	for (std::map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
		if (it->second.getNickname() == nickname)
			return true;
	return false;
}

void	Server::executor(const char* buf, Client& client) {
	std::string			buffer(buf);
	std::istringstream	iss(buffer);
	std::string			line;

	while (std::getline(iss, line)) {
		if (line.find("CAP LS 302") != std::string::npos)
			continue;
		std::string command = line.substr(0, line.find(" "));
		std::string args = line.substr(line.find(" ") + 1, line.size());
		launchCommand(client, command, args);
		if (client.getAuthentication() && !client.getNickname().empty() && !client.getUsername().empty())
			client.setRegistration();
	}
}

void	Server::launchCommand(Client& client, const std::string& cmd, const std::string& args) {
	std::string		cmdNames[5] = {"PASS", "NICK", "USER", "JOIN", "MSG"};
	typedef void	(Server::*cmds)(Client&, const std::string&);
	cmds			cmdFunc[5] = {&Server::pass, &Server::nick, &Server::user, &Server::join, &Server::privmsg};

	for (int i = 0; i < 5; i++)
		if (cmdNames[i] == cmd)
			(this->*cmdFunc[i])(client, args);
}

void	Server::addClient(const Client& client) {
	_clients.insert(std::pair<int, Client>(client.getSocket(), client));
}

void	Server::eraseClient(int socket) {
	_clients.erase(socket);
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
