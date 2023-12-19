/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 15:09:56 by laprieur          #+#    #+#             */
/*   Updated: 2023/12/19 15:41:30 by laprieur         ###   ########.fr       */
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

void	Server::startServer() {
	// Create a server socket
	int	serverSock = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSock == -1) {
		std::cerr << RED << "Impossible to create the server socket." << NONE << std::endl;
		return 1;
	}
	// Set up the socket structure
	struct sockaddr_in	serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(atoi(argv[1]));
	// Bind the server socket to a port
	if (bind(serverSock, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1) {
		std::cerr << "Failed to bind socket." << std::endl;
		close(serverSock);
		return 1;
	}
	// Listen for incoming connections
	if (listen(serverSock, 5) == -1) {
		std::cerr << "Failed to listen." << std::endl;
		close(serverSock);
		return 1;
    }
	// Create an epoll() instance
	int epollFd = epoll_create1(0);
	if (epollFd == -1) {
		std::cerr << "Failed to create epoll instance." << std::endl;
		close(serverSock);
		return 1;
	}
	// Set up the epoll structure
	struct epoll_event	event, events[10];
	event.events = EPOLLIN;
	event.data.fd = serverSock;
	// Add server socket to epoll
	if (epoll_ctl(epollFd, EPOLL_CTL_ADD, serverSock, &event) == -1) {
		std::cerr << "Failed to add socket to epoll\n";
		close(epollFd);
		close(serverSock);
		return 1;
	}
	// Main event loop
	while (true) {
        int numEvents = epoll_wait(epollFd, events, 10, -1);
        if (numEvents == -1) {
            std::cerr << "Failed to wait for events." << std::endl;
            break;
        }
        for (int i = 0; i < numEvents; ++i) {
            if (events[i].data.fd == serverSock) {
                // Accept new client connection
                struct sockaddr_in clientAddress;
                socklen_t clientAddressLength = sizeof(clientAddress);
                int clientFd = accept(serverSock, (struct sockaddr*)&clientAddress, &clientAddressLength);
                if (clientFd == -1) {
                    std::cerr << "Failed to accept client connection." << std::endl;
                    continue;
				}
                // Add client socket to epoll
                event.events = EPOLLIN;
                event.data.fd = clientFd;
                if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientFd, &event) == -1) {
                    std::cerr << "Failed to add client socket to epoll instance." << std::endl;
                    close(clientFd);
                    continue;
                }
				std::cout << "New client connected!" << std::endl;
            } else {
                // Handle client data
                int clientFd = events[i].data.fd;
				(void)clientFd;
				// Read and process data from clientSocket
            }
        }
    }
	close(serverSock);
	close(epollFd);
}
