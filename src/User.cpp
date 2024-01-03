/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 12:55:54 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/03 16:40:35 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User(const std::string& nickname, const std::string& username, int socket, bool isAuthenticated, bool isRegistered, const sockaddr_in& address) {
	_nickname = nickname;
	_username = username;
	_type = REGULAR;
	_socket = socket;
	_isAuthenticated = isAuthenticated;
	_isRegistered = isRegistered;
	_address = address;
}

User::~User() {}

const std::string&	User::getNickname() const {
	return _nickname;
}

const std::string&	User::getUsername() const {
	return _username;
}

bool				User::getAuthentication() const {
	return _isAuthenticated;
}

bool				User::getRegistration() const {
	return _isRegistered;
}

bool				User::isOperator(Channel channel) const {
	std::map<int, User>::const_iterator i = (channel.getMap(OPERATORS)).find(_socket);
	if (i != channel.getMap(OPERATORS).end())
		return (true);
	return (false);
}

int					User::getSocket() const {
	return _socket;
}

int					User::getType() const {
	return _type;
}

const sockaddr_in&	User::getAddress() const {
	return _address;
}

void	User::setType(int type) {
	_type = type;
}

void	User::setNickname(const std::string& nickname) {
	_nickname = nickname;
}

void	User::setUsername(const std::string& username) {
	_username = username;
}

void	User::setAuthentication() {
	_isAuthenticated = true;
}

void	User::setRegistration() {
	_isRegistered = true;
}

void	User::setSocket(int socket) {
	_socket = socket;
}

void	User::setAddress(sockaddr_in address) {
	_address = address;
}

void User::display() const {
	std::cout << "Type : " << _type << std::endl;
	std::cout << "socket : " << _socket << std::endl;
	std::cout << "isAuthenticated : " << _isAuthenticated << std::endl;
	std::cout << "isRegistered : " << _isRegistered << std::endl;
	std::cout << "nickname : " << _nickname << std::endl;
	std::cout << "username : " << _username << std::endl;
}
