/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsebille <hsebille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 16:33:57 by laprieur          #+#    #+#             */
/*   Updated: 2023/12/21 16:34:42 by hsebille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "ft_irc.hpp"

enum Logs {
	RPL_WELCOME,
	ERR_NEEDMOREPARAMS,
	ERR_ALREADYREGISTRED
};

enum Commands {
	PASS
};

class Server {
	private:
		int					_port;
		int					_socket;
		int					_epoll;
		std::string			_password;
		struct sockaddr_in	_serverAddress;
		struct epoll_event	_event, _events[MAX_EVENTS];
	
	public:
		Server(char** params);
		~Server();
		
		void	setup();
		void	start();
		void	pass(User& user, const std::string& password);
		bool	nick(User& user, std::string nickname);
		void	user();
		void	join();
		void	privmsg();
		void	clientLog(const User& user, int socket, int logCode, std::string cmd);
		
};

#endif