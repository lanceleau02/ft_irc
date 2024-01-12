/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/12 13:51:57 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/12 13:51:57 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

/* ************************************************************************** */
/*                              CLASS FUNCTIONS                               */
/* ************************************************************************** */

Channel::Channel(const Client& op, const std::string& name) : _topic(), _key() {
	_userLimit = 100;
	_inviteOnly = false;
	_topicRestrictions = false;
	_channelKey = false;
	_isUserLimit = false;
	_name = name;
	addOperator(op);
	addUser(op);
}

Channel::~Channel() {}

/* ************************************************************************** */
/*                             GETTERS FUNCTIONS                              */
/* ************************************************************************** */

int		Channel::getUserLimit() const {
	return _userLimit;
}

int		Channel::getNbUsers() const {
	return _users.size();
}

bool	Channel::getInviteMode() const {
	return _inviteOnly;
}

bool	Channel::getTopicRestrictions() const {
	return _topicRestrictions;
}

bool	Channel::getKeyMode() const {
	return _channelKey;
}

const std::string&	Channel::getTopic() const {
	return _topic;
}

const std::string&	Channel::getKey() const {
	return _key;
}

const std::map<int, Client>& Channel::getMap(int type) const {
	return (type == OPERATORS) ? _operators : ((type == USERS) ? _users : _invitees);
}

int	Channel::getClient(std::string nickname) const {
	for (std::map<int, Client>::const_iterator it = _users.begin(); it != _users.end(); it++)
		if (it->second.getNickname() == nickname)
			return it->second.getSocket();
	return -1;
}

/* ************************************************************************** */
/*                             SETTERS FUNCTIONS                              */
/* ************************************************************************** */

void	Channel::setUserLimit(int mode, int userLimit) {
	_isUserLimit = (mode == UNSET_USER_LIMIT) ? false : ((mode == SET_USER_LIMIT) ? true : _isUserLimit);
	if (mode == CHANGE_USER_LIMIT)
		_userLimit = userLimit;
}

void	Channel::setInviteMode(int mode) {
	_inviteOnly = (mode == INVITE_MODE) ? true : ((mode == NON_INVITE_MODE) ? false : _inviteOnly);
}

void	Channel::setTopicRestrictions(int mode) {
	_topicRestrictions = (mode == OP_ONLY) ? false : ((mode == EVERYONE) ? true : _topicRestrictions);
}

void	Channel::setTopic(std::string topic) {
	_topic = topic;
}

void	Channel::setKey(int mode, std::string key) {
	_key = (mode == ADD_KEY) ? key : ((mode == REMOVE_KEY) ? "" : _key);
}

/* ************************************************************************** */
/*                              MEMBER FUNCTIONS                              */
/* ************************************************************************** */

bool	Channel::isOperator(int clientSocket) {
	return _operators.find(clientSocket) != _operators.end();
}

bool	Channel::isUser(int clientSocket) {
	return _users.find(clientSocket) != _users.end();
}

bool	Channel::isInvitee(int clientSocket) {
	return _invitees.find(clientSocket) != _invitees.end();
}

bool	Channel::findClient(std::string user) const {
	for (std::map<int, Client>::const_iterator it = _users.begin(); it != _users.end(); it++)
		if (it->second.getNickname() == user)
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

void	Channel::addOrRemove(int mode, int clientSocket) {
	if (mode == ADD_OPERATOR)
		addOperator(_users.at(clientSocket));
	else if (mode == REMOVE_OPERATOR)
		_operators.erase(clientSocket);
}

void	Channel::deleteUser(std::string nickname) {
	int socket = -1;
	for (std::map<int, Client>::iterator it = _users.begin(); it != _users.end(); it++)
		if (it->second.getNickname() == nickname)
			socket = it->second.getSocket();
	if (socket != -1)
		_users.erase(socket);
}

void	Channel::sendMessage(int mode, int clientSocket, const std::string& msg) {
	for (std::map<int, Client>::iterator it = _users.begin(); it != _users.end(); it++) {
		if (mode == SEND_TO_ALL || (mode == EXCLUDE_SENDER && it->second.getSocket() != clientSocket))
			Server::clientLog(it->second.getSocket(), msg);
	}
}
