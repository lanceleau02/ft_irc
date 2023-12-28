/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 16:33:57 by laprieur          #+#    #+#             */
/*   Updated: 2023/12/28 15:56:44 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "ft_irc.hpp"

class Channel;

class Server {
	private:
		int								_port;
		int								_socket;
		int								_epoll;
		std::string						_password;
		struct sockaddr_in				_serverAddress;
		struct epoll_event				_event, _events[MAX_EVENTS];
		std::map<std::string, Channel>	_channels;
	
	public:
		Server(char** params);
		~Server();
		
		/* INITIALIZATION */
		void	setup();
		void	start();
		int		acceptConnection(sockaddr_in& userAddress);
		int		addSocket(epoll_event& event, int socket, int epoll);
		void	registerUser(User& user, const std::string& userInfos);
		void	launchCommand(User* user, const std::string& cmd, const std::string& args);

		/* COMMANDS */
		void	pass(User& user, const std::string& password);
		void	nick(User& user, const std::string& nickname);
		void	user(User& user, const std::string& username);
		void	join(User& user, const std::string& channelName);
		void	privmsg();

		/* UTILS */
		static void	serverLog(int type, const std::string& log);
		static void	clientLog(int socket, const std::string& log);
};

#endif