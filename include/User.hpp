/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 12:51:42 by laprieur          #+#    #+#             */
/*   Updated: 2023/12/21 14:07:23 by laprieur         ###   ########.fr       */
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
		std::string			_nickname;
		std::string			_username;
		int					_type;
		int					_socket;
		bool				_isAuthentified;
		struct sockaddr_in	_address;

	public:
		User();
		~User();

	const std::string&	getNickname() const;
	const std::string&	getUsername() const;
	const bool&			getAuthentification() const;
	const int&			getSocket() const;
	const sockaddr_in&	getAddress() const;
	void				setNickname(const std::string& nickname);
	void				setUsername(const std::string& username);
	void				setAuthentification();
	void				setSocket(int socket);
	void				setAddress(sockaddr_in address);
};

#endif