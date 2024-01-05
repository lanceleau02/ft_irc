/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 16:33:57 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/05 13:53:19 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "ft_irc.hpp"

class Client;
class Channel;

class Server {
	private:
		int								_port;
		int								_socket;
		int								_epoll;
		std::string						_password;
		struct sockaddr_in				_serverAddress;
		struct epoll_event				_event, _events[MAX_EVENTS];
		std::map<int, Client>			_clients;
		std::map<std::string, Channel>	_channels;
	
	public:
		Server(char** params);
		~Server();

		void	setup();
		void	start();

		int			acceptConnection(sockaddr_in& clientAddress);
		int			addSocket(epoll_event& event, int socket, int epoll);
		void		executor(const char* buf, Client& client);
		void		launchCommand(Client& client, const std::string& cmd, const std::string& args);
		void		addClient(const Client& client);
		void		eraseClient(int socket);
		static void	serverLog(int type, const std::string& log);
		static void	clientLog(int socket, const std::string& log);

		void	pass(Client& client, const std::string& password);
		void	nick(Client& client, const std::string& nickname);
		void	user(Client& client, const std::string& username);
		void	join(Client& client, const std::string& channelName);
		void	privmsg();
};

#endif