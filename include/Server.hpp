/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:38:26 by hsebille          #+#    #+#             */
/*   Updated: 2024/01/15 11:22:58 by laprieur         ###   ########.fr       */
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

		const std::map<int, Client>&	getClients() const;
		
		void	setup();
		void	start();

		int			waitEvents();
		int			acceptConnection(sockaddr_in& clientAddress);
		int			addSocket(epoll_event& event, int socket, int epoll);
		void		createSocket();
		void		setupSocket();
		void		bindSocket();
		void		listenConnections();
		void		createEpoll();
		void		addSocketToEpoll();
		void		executor(std::string buffer, Client& client);
		void		launchCommand(Client& client, const std::string& cmd, const std::string& args);
		void		addClient(const Client& client);
		void		eraseClient(int socket);
		static void	serverLog(int type, const std::string& log);
		static void	clientLog(int socket, const std::string& log);

		void	pass(Client& client, const std::string& password);
		void	kick(Client& client, const std::string& args);
		void	nick(Client& client, const std::string& nickname);
		void	user(Client& client, const std::string& username);
		void	join(Client& client, const std::string& channelName);
		void	privmsg(Client& client, const std::string& args);
		void	topic(Client& client, const std::string& args);
		void	mode(Client& client, const std::string& args);
		void	invite(Client& client, const std::string& args);
};

#endif