/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 13:42:32 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/16 11:58:30 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/* ************************************************************************** */
/*                              CLASS FUNCTIONS                               */
/* ************************************************************************** */

Client::Client(int socket, const sockaddr_in& address) : _nickname(), _username(), _buffer() {
	_socket = socket;
	_isAuthenticated = false;
	_isRegistered = false;
	_address = address;
}

Client::~Client() {}

/* ************************************************************************** */
/*                             GETTERS FUNCTIONS                              */
/* ************************************************************************** */

int	Client::getSocket() const {
	return _socket;
}

bool	Client::getAuthentication() const {
	return _isAuthenticated;
}

bool	Client::getRegistration() const {
	return _isRegistered;
}

const std::string&	Client::getNickname() const {
	return _nickname;
}

const std::string&	Client::getUsername() const {
	return _username;
}

const std::string&	Client::getBuffer() const {
	return _buffer;
}

const sockaddr_in&	Client::getAddress() const {
	return _address;
}

/* ************************************************************************** */
/*                             SETTERS FUNCTIONS                              */
/* ************************************************************************** */

void	Client::setSocket(int socket) {
	_socket = socket;
}

void	Client::setAuthentication(bool type) {
	_isAuthenticated = type;
}

void	Client::setRegistration() {
	_isRegistered = true;
}

void	Client::setNickname(const std::string& nickname) {
	_nickname = nickname;
}

void	Client::setUsername(const std::string& username) {
	_username = username;
}

void	Client::setAddress(sockaddr_in address) {
	_address = address;
}

/* ************************************************************************** */
/*                              MEMBER FUNCTIONS                              */
/* ************************************************************************** */

void	Client::addToBuffer(std::string buffer) {
	_buffer += buffer;
}

void	Client::cleanBuffer() {
	_buffer = "";
}
