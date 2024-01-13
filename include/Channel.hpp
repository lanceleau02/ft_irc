/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsebille <hsebille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 12:38:30 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/13 09:44:02 by hsebille         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "ft_irc.hpp"

enum MapType {
	OPERATORS,
	USERS,
	INVITEES
};

enum MessageType {
	SEND_TO_ALL,
	EXCLUDE_SENDER
};

enum Mode {
	NON_INVITE_MODE,
	INVITE_MODE,
	OP_ONLY,
	EVERYONE,
	ADD_KEY,
	REMOVE_KEY,
	ADD_OPERATOR,
	REMOVE_OPERATOR,
	UNSET_USER_LIMIT,
	SET_USER_LIMIT,
	CHANGE_USER_LIMIT
};

class Client;

class Channel {
	private:
		int						_userLimit;
		bool					_inviteOnly;
		bool					_topicRestrictions;
		bool					_isUserLimit;
		std::string				_name;
		std::string				_topic;
		std::string				_key;
		std::map<int, Client>	_operators;
		std::map<int, Client>	_users;
		std::map<int, Client>	_invitees;

	public:
		Channel(const Client& op, const std::string& name);
		~Channel();

		int								getUserLimit() const;
		int								getNbUsers() const;
		int								getClient(std::string nickname) const;
		bool							getInviteMode() const;
		bool							getTopicRestrictions() const;
		bool							getIsUserLimit() const;
		const std::string&				getTopic() const;
		const std::string&				getKey() const;
		const std::map<int, Client>&	getMap(int type) const;
		
		void	setUserLimit(int mode, int limit);
		void	setInviteMode(int mode);
		void	setTopicRestrictions(int mode);
		void	setTopic(std::string topic);
		void	setKey(int mode, std::string key);

		bool	isOperator(int clientSocket);
		bool	isUser(int clientSocket);
		bool	isInvitee(int clientSocket);
		bool	findClient(std::string user) const;
		void	addOperator(const Client& op);
		void	addUser(const Client& user);
		void	addInvitee(const Client& invitee);
		void	addOrRemove(int mode, int clientSocket);
		void	deleteUser(std::string nickname);
		void	sendMessage(int mode, int clientSocket, const std::string& msg);
};

#endif