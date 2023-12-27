/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 12:55:54 by laprieur          #+#    #+#             */
/*   Updated: 2023/12/27 15:44:11 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User(const std::string& nickname, const std::string& username, int type, int socket, bool isAuthenticated, bool isRegistered, const sockaddr_in& address) {
	_nickname = nickname;
	_username = username;
	_type = type;
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

bool			User::getAuthentication() const {
	return _isAuthenticated;
}

bool			User::getRegistration() const {
	return _isRegistered;
}

int			User::getSocket() const {
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
