/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 15:09:56 by laprieur          #+#    #+#             */
/*   Updated: 2023/12/19 16:36:01 by laprieur         ###   ########.fr       */
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

void	Server::start() {
	// Main event loop
	while (true) {
		// Wait for events on the epoll instance
        int numEvents = epoll_wait(_epoll, _events, 10, -1);
        if (numEvents == -1)
			throw std::runtime_error("failed to wait for events.");
        for (int i = 0; i < numEvents; ++i) {
            if (_events[i].data.fd == _socket) {
                // Accept new client connection
                socklen_t clientAddressLength = sizeof(_clientAddress);
                int clientSocket = accept(_socket, (struct sockaddr*)&_clientAddress, &clientAddressLength);
                if (clientSocket == -1) {
                    std::cerr << RED << "Error: failed to accept client connection." << NONE << std::endl;
                    continue;
				}
                // Add client socket to epoll
                _event.events = EPOLLIN;
                _event.data.fd = clientSocket;
                if (epoll_ctl(_epoll, EPOLL_CTL_ADD, clientSocket, &_event) == -1) {
                    std::cerr << RED << "Error: failed to add client socket to epoll instance." << NONE << std::endl;
                    close(clientSocket);
                    continue;
                }
				std::cout << "New client connected!" << std::endl;
            } else {
                // Handle client data
                int clientSocket = _events[i].data.fd;
				(void)clientSocket;
				// Read and process data from clientSocket
            }
        }
    }
	close(_socket);
	close(_epoll);
}
