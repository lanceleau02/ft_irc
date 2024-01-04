/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 13:42:32 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/04 10:59:07 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() {}

Client::~Client() {}

const User&	Client::getUser(int socket) const {
	return ((_users.find(socket))->second);
}

size_t	Client::getNbUsers() const {
	return _users.size();
}

const std::map<int, User>&	Client::getUsers() const {
	return _users;
}

void	Client::addUser(int socket, const User& user) {
	_users.insert(std::pair<int, User>(socket, user));
}
