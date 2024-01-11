/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsebille <hsebille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 12:38:30 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/11 17:10:28 by hsebille         ###   ########.fr       */
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
		bool					_channelKey;
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

		int								getUserLimit();
		int								getNbUsers();
		int								getClient(std::string nickname);
		bool							getInviteMode();
		bool							getTopicRestrictions();
		bool							getKeyMode();
		const std::string&				getTopic();
		const std::string&				getKey();
		const std::map<int, Client>&	getMap(int type);
		
		void	setTopic(std::string topic);
		void	setInviteMode(int mode);
		void	setTopicRestrictions(int mode);
		void	setKey(int mode, std::string key);
		void	setUserLimit(int mode, int limit);

		bool	isOnChannel(int clientSocket);
		bool	findClient(std::string user) const;
		bool	isInvited(int clientSocket);
		void	addOperator(const Client& op);
		void	addOrRemove(int mode, int clientSocket);
		void	addUser(const Client& user);
		void	deleteUser(std::string nickname);
		void	addInvitee(const Client& invitee);
		void	sendMessage(int mode, int clientSocket, const std::string& msg);
};

#endif