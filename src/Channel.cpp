/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 13:10:53 by laprieur          #+#    #+#             */
/*   Updated: 2023/12/28 16:36:54 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(const User& op, const std::string& name) : _topic(), _password() {
	_userLimit = 0;
	_inviteMode = false;
	_topicMode = false;
	_passwordMode = false;
	_name = name;
	addOperator(op);
	addUser(op);
}

Channel::~Channel() {}

int		Channel::getUserLimit() {
	return _userLimit;
}

int		Channel::getNbUsers() {
	return _users.size();
}

void	Channel::addOperator(const User& op) {
	_operators.insert(std::pair<int, User>(op.getSocket(), op));
}

void	Channel::addUser(const User& user) {
	_users.insert(std::pair<int, User>(user.getSocket(), user));
}

void	Channel::addInvitee(const User& invitee) {
	_invitees.insert(std::pair<int, User>(invitee.getSocket(), invitee));
}