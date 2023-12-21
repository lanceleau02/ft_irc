/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 12:55:54 by laprieur          #+#    #+#             */
/*   Updated: 2023/12/21 13:47:16 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User() : _nickname(""), _username(""), _type(REGULAR), _isAuthentified(false) {
	(void)_type;
	(void)_isAuthentified;
}

User::~User() {}

const std::string&	User::getNickname() const {
	return _nickname;
}

const std::string&	User::getUsername() const {
	return _username;
}

const bool&			User::getAuthentification() const {
	return _isAuthentified;
}

const int&			User::getSocket() const {
	return _socket;
}

const sockaddr_in&	User::getAddress() const {
	return _address;
}

void	User::setNickname(const std::string& nickname) {
	_nickname = nickname;
}

void	User::setUsername(const std::string& username) {
	_username = username;
}

void	User::setAuthentification() {
	_isAuthentified = true;
}

void	User::setSocket(int socket) {
	_socket = socket;
}

void	User::setAddress(sockaddr_in address) {
	_address = address;
}
