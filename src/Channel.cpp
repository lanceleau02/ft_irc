/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsebille <hsebille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 13:10:53 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/05 11:38:42 by hsebille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

/* ************************************************************************** */
/*                              CLASS FUNCTIONS                               */
/* ************************************************************************** */

Channel::Channel(const User& op, const std::string& name) : _topic(), _password() {
	_userLimit = 100;
	_inviteOnly = false;
	_topicRestrictions = false;
	_channelKey = false;
	_name = name;
	addOperator(op);
	addUser(op);
}

Channel::~Channel() {}

/* ************************************************************************** */
/*                             GETTERS FUNCTIONS                              */
/* ************************************************************************** */

int		Channel::getUserLimit() {
	return _userLimit;
}

int		Channel::getNbUsers() {
	return _users.size();
}

bool	Channel::getInviteMode() {
	return _inviteOnly;
}

bool	Channel::getPasswordMode() {
	return _channelKey;
}

const std::string&	Channel::getPassword() {
	return _password;
}

const std::map<int, User>&	Channel::getMap(int type) {
	if (type == OPERATORS)
		return _operators;
	if (type == USERS)
		return _users;
	return _invitees;
}

/* ************************************************************************** */
/*                              MEMBER FUNCTIONS                              */
/* ************************************************************************** */

bool	Channel::isOnChannel(int userSocket) {
	if (_users.find(userSocket) != _users.end())
		return true;
	return false;
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