/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 13:10:53 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/05 15:31:00 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

/* ************************************************************************** */
/*                              CLASS FUNCTIONS                               */
/* ************************************************************************** */

Channel::Channel(const Client& op, const std::string& name) : _topic(), _password() {
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

const std::map<int, Client>&	Channel::getMap(int type) {
	if (type == OPERATORS)
		return _operators;
	if (type == USERS)
		return _users;
	return _invitees;
}

/* ************************************************************************** */
/*                              MEMBER FUNCTIONS                              */
/* ************************************************************************** */

bool	Channel::isOnChannel(int clientSocket) {
	if (_users.find(clientSocket) != _users.end())
		return true;
	return false;
}

void	Channel::addOperator(const Client& op) {
	_operators.insert(std::pair<int, Client>(op.getSocket(), op));
}

void	Channel::addUser(const Client& user) {
	_users.insert(std::pair<int, Client>(user.getSocket(), user));
}

void	Channel::addInvitee(const Client& invitee) {
	_invitees.insert(std::pair<int, Client>(invitee.getSocket(), invitee));
}

void	Channel::sendMessage(const std::string& msg) {
	for (std::map<int, Client>::iterator it = _users.begin(); it != _users.end(); it++)
		Server::clientLog(it->second.getSocket(), msg);
}
