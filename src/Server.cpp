/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 14:32:37 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/16 11:57:01 by laprieur         ###   ########.fr       */
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
/*                             GETTERS FUNCTIONS                              */
/* ************************************************************************** */

const std::map<int, Client>&	Server::getClients() const {
	return _clients;
}

/* ************************************************************************** */
/*                              MEMBER FUNCTIONS                              */
/* ************************************************************************** */

void	Server::setup() {
	createSocket();
	setupSocket();
	bindSocket();
	listenConnections();
	createEpoll();
	addSocketToEpoll();
}

void	Server::start() {
	std::signal(SIGINT, signalHandler);
	std::signal(SIGHUP, signalHandler);
	// Main event loop
	while (true) {
		// Wait for events on the epoll instance
        int nbEvents = waitEvents();
        for (int i = 0; i < nbEvents; ++i) {
			// Handle new clients
            if (_events[i].data.fd == _socket) {
                // Accept new client connection
				struct sockaddr_in	clientAddress;
				int	clientSocket = acceptConnection(clientAddress);
                if (clientSocket == -1) {
                    serverLog(FAILURE, "Failed to accept client connection.");
                    continue;
				}
				serverLog(SUCCESS, "Client connected!");
				// Create new client
				Client	newClient(clientSocket, clientAddress);
				addClient(newClient);
				// Add client socket to epoll
                if (addSocket(_event, clientSocket, _epoll) == -1) {
					serverLog(FAILURE, "Failed to add client socket to epoll instance.");
                    close(clientSocket);
                    continue;
                }
            } else { // Handle incoming data or other events: authenticate, set a nickname, a username, join a channel...
				// Manage events
				int		clientSocket = _events[i].data.fd;
				char	buffer[1024];
				int		bytes;
				
				bytes = recv(clientSocket, buffer, sizeof(buffer), 0);
				buffer[bytes] = '\0';
				_clients.at(clientSocket).addToBuffer(buffer);
				if (bytes <= 0) {
					removeUserAndChannels(clientSocket);
					serverLog(FAILURE, "Client disconnected!");
					epoll_ctl(_epoll, EPOLL_CTL_DEL, clientSocket, &_event);
					eraseClient(clientSocket);
					close(clientSocket);
				} else if (_clients.at(clientSocket).getBuffer().find('\n') != std::string::npos) {
					std::cout << _clients.at(clientSocket).getBuffer();
					executor(_clients.at(clientSocket).getBuffer(), _clients.at(clientSocket));
					_clients.at(clientSocket).cleanBuffer();
				}
			}
        }
    }
}

/* ************************************************************************** */
/*                              UTILS FUNCTIONS                               */
/* ************************************************************************** */

void	Server::removeUserAndChannels(int clientSocket) {
	std::vector<std::string>	channelsToRemove;
	
	for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); it++) {
		if (findClient(it->second.getMap(USERS), _clients.at(clientSocket).getNickname())) {
			if (it->second.isOperator(clientSocket))
				it->second.addOrRemove(REMOVE_OPERATOR, clientSocket);
			it->second.sendMessage(SEND_TO_ALL, clientSocket, RPL_PART(_clients.at(clientSocket).getNickname(), it->first));
			it->second.deleteUser(_clients.at(clientSocket).getNickname());
		}
		if (it->second.getMap(USERS).empty())
			channelsToRemove.push_back(it->first);
	}
	for (std::vector<std::string>::iterator it = channelsToRemove.begin(); it != channelsToRemove.end(); ++it)
		_channels.erase(*it);
}

void	Server::createSocket() {
	_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket == -1)
		throw std::runtime_error("impossible to create the server socket.");
}

void	Server::setupSocket() {
	int	opt = 1;
	setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	memset(&_serverAddress, 0, sizeof(_serverAddress));
	_serverAddress.sin_family = AF_INET;
	_serverAddress.sin_addr.s_addr = INADDR_ANY;
	_serverAddress.sin_port = htons(_port);
}

void	Server::bindSocket() {
	if (bind(_socket, reinterpret_cast<sockaddr*>(&_serverAddress), sizeof(_serverAddress)) == -1) {
		close(_socket);
		throw std::runtime_error("failed to bind socket.");
	}
}

void	Server::listenConnections() {
	if (listen(_socket, 5) == -1) {
		close(_socket);
		throw std::runtime_error("failed to listen.");
    }
}

void	Server::createEpoll() {
	_epoll = epoll_create1(0);
	if (_epoll == -1) {
		close(_socket);
		throw std::runtime_error("failed to create epoll instance.");
	}
	_event.events = EPOLLIN;
	_event.data.fd = _socket;
}

void	Server::addSocketToEpoll() {
	if (epoll_ctl(_epoll, EPOLL_CTL_ADD, _socket, &_event) == -1) {
		close(_epoll);
		close(_socket);
		throw std::runtime_error("failed to add socket to epoll.");
	}
}

int	Server::waitEvents() {
	int nbEvents = epoll_wait(_epoll, _events, 10, -1);
    if (nbEvents == -1)
		throw std::runtime_error("failed to wait for events.");
	return nbEvents;
}

int	Server::acceptConnection(sockaddr_in& clientAddress) {
	socklen_t clientAddressLength = sizeof(clientAddress);
	return accept(_socket, reinterpret_cast<sockaddr*>(&clientAddress), &clientAddressLength);
}

int	Server::addSocket(epoll_event& event, int socket, int epoll) {
	event.events = EPOLLIN;
	event.data.fd = socket;
	return epoll_ctl(epoll, EPOLL_CTL_ADD, socket, &event);
}

void	Server::executor(std::string buffer, Client& client) {
	std::istringstream	iss(buffer);
	std::string			line;
	std::string			command;
	std::string			args;

	while (std::getline(iss, line)) {
		if (line.find("CAP LS 302") != std::string::npos)
			continue;
		std::istringstream cmd(line);
		cmd >> command;
		std::getline(cmd, args);
		launchCommand(client, command, args);
		if (client.getAuthentication() && !client.getNickname().empty() && !client.getUsername().empty() && !client.getRegistration()) {
			client.setRegistration();
			clientLog(client.getSocket(), RPL_WELCOME(client.getNickname()));
			serverLog(SUCCESS, client.getNickname() + " just arrived!");
		}
	}
}

void	Server::launchCommand(Client& client, const std::string& cmd, const std::string& args) {
	std::string		cmdNames[10] = {"PASS", "NICK", "USER", "JOIN", "PRIVMSG", "TOPIC", "KICK", "MODE", "INVITE", "BOT"};
	typedef void	(Server::*cmds)(Client&, const std::string&);
	cmds			cmdFunc[10] = {&Server::pass, &Server::nick, &Server::user, &Server::join, &Server::privmsg, &Server::topic, &Server::kick, &Server::mode, &Server::invite, &Server::bot};

	for (int i = 0; i < 10; i++)
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
	if (type == SUCCESS)
		std::cout << GREEN << log << NONE << std::endl << std::endl;
	else if (type == FAILURE)
		std::cerr << RED << log << NONE << std::endl << std::endl;
}

void	Server::clientLog(int socket, const std::string& log) {
	send(socket, log.c_str(), log.size(), 0);
}
