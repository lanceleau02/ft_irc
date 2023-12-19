/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 16:33:57 by laprieur          #+#    #+#             */
/*   Updated: 2023/12/19 16:32:37 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "ft_irc.hpp"

class Server {
	private:
		int					_port;
		int					_socket;
		int					_epoll;
		std::string			_password;
		struct sockaddr_in	_serverAddress;
        struct sockaddr_in	_clientAddress;
		struct epoll_event	_event, _events[MAX_EVENTS];

		Server();
		Server(const Server& source);
		Server& operator=(const Server& source);
	
	public:
		Server(char** params);
		~Server();
		
		void	setup();
		void	start();
};

#endif