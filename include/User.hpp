/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 12:51:42 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/04 13:17:01 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

#include "ft_irc.hpp"

enum Type {
	OPERATOR,
	REGULAR
};

class User {
	private:
		int					_type;
		int					_socket;
		bool				_isAuthenticated;
		bool				_isRegistered;
		std::string			_nickname;
		std::string			_username;
		struct sockaddr_in	_address;

	public:
		User(const std::string& nickname, const std::string& username, int socket, bool isAuthenticated, bool isRegistered, const sockaddr_in& address);
		~User();

		int					getType() const;
		int					getSocket() const;
		bool				getAuthentication() const;
		bool				getRegistration() const;
		const std::string&	getNickname() const;
		const std::string&	getUsername() const;
		const sockaddr_in&	getAddress() const;
		
		void	setType(int type);
		void	setSocket(int socket);
		void	setAuthentication();
		void	setRegistration();
		void	setNickname(const std::string& nickname);
		void	setUsername(const std::string& username);
		void	setAddress(sockaddr_in address);
		
		bool	isOperator(Channel channel) const;
		void	display() const;
};

#endif