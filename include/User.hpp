/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsebille <hsebille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 12:51:42 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/03 11:19:41 by hsebille         ###   ########.fr       */
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

		const std::string&	getNickname() const;
		const std::string&	getUsername() const;
		bool				getAuthentication() const;
		bool				getRegistration() const;
		bool				isOperator(Channel channel) const;
		int					getSocket() const;
		int					getType() const;
		const sockaddr_in&	getAddress() const;
		void				setType(int type);
		void				setNickname(const std::string& nickname);
		void				setUsername(const std::string& username);
		void				setAuthentication();
		void				setRegistration();
		void				setSocket(int socket);
		void				setAddress(sockaddr_in address);

};

#endif