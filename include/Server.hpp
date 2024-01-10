/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsebille <hsebille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:38:26 by hsebille          #+#    #+#             */
/*   Updated: 2024/01/10 11:38:29 by hsebille         ###   ########.fr       */
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

		int			waitEvents();
		int			acceptConnection(sockaddr_in& clientAddress);
		int			addSocket(epoll_event& event, int socket, int epoll);
		bool		findClientByNick(std::string nickname) const;
		void		createSocket();
		void		setupSocket();
		void		bindSocket();
		void		listenConnections();
		void		createEpoll();
		void		addSocketToEpoll();
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
		void	privmsg(Client& client, const std::string& args);
};

#endif