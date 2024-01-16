/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 15:56:15 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/16 11:58:33 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "ft_irc.hpp"

class Client {
	private:
		int					_socket;
		bool				_isAuthenticated;
		bool				_isRegistered;
		std::string			_nickname;
		std::string			_username;
		std::string			_buffer;
		struct sockaddr_in	_address;

	public:
		Client(int socket, const sockaddr_in& address);
		~Client();

		int					getSocket() const;
		bool				getAuthentication() const;
		bool				getRegistration() const;
		const std::string&	getNickname() const;
		const std::string&	getUsername() const;
		const std::string&	getBuffer() const;
		const sockaddr_in&	getAddress() const;

		void	setSocket(int socket);
		void	setAuthentication(bool type);
		void	setRegistration();
		void	setNickname(const std::string& nickname);
		void	setUsername(const std::string& username);
		void	setAddress(sockaddr_in address);

		void	addToBuffer(std::string buffer);
		void	cleanBuffer();
};

#endif