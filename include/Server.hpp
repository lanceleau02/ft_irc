/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 16:33:57 by laprieur          #+#    #+#             */
/*   Updated: 2023/12/27 16:38:37 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "ft_irc.hpp"

enum Logs {
	/* WELCOME */
	RPL_WELCOME,
	/* PASSWORD */
	ERR_PASSWDMISMATCH,
	ERR_NEEDMOREPARAMS,
	ERR_ALREADYREGISTRED,
	/* NICK */
	ERR_NONICKNAMEGIVEN,
	ERR_ERRONEUSNICKNAME,
	ERR_NICKNAMEINUSE,
	ERR_UNAVAILRESOURCE,
	ERR_USERNOTINCHANNEL,
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
		void	join();
		void	privmsg();

		/* UTILS */
		static void	serverLog(int type, std::string log);
		static void	clientLog(int socket, const char* log);
};

#endif