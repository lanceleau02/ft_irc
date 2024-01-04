/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 13:42:32 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/04 13:31:09 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/* ************************************************************************** */
/*                              CLASS FUNCTIONS                               */
/* ************************************************************************** */

Client::Client() {}

Client::~Client() {}

/* ************************************************************************** */
/*                             GETTERS FUNCTIONS                              */
/* ************************************************************************** */

size_t	Client::getNbUsers() const {
	return _users.size();
}

const User&	Client::getUser(int socket) const {
	return ((_users.find(socket))->second);
}

const std::map<int, User>&	Client::getUsers() const {
	return _users;
}

/* ************************************************************************** */
/*                              MEMBER FUNCTIONS                              */
/* ************************************************************************** */

void	Client::addUser(int socket, const User& user) {
	_users.insert(std::pair<int, User>(socket, user));
}
