/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsebille <hsebille@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 12:38:30 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/11 11:18:48 by hsebille         ###   ########.fr       */
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

class Client;

class Channel {
	private:
		int						_userLimit;
		bool					_inviteOnly;
		bool					_topicRestrictions;
		bool					_channelKey;
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
		bool							getInviteMode();
		bool							getTopicRestrictions();
		bool							getKeyMode();
		const std::string&				getTopic();
		const std::string&				getKey();
		const std::map<int, Client>&	getMap(int type);
		
		void	setTopic(std::string topic);

		bool	isOnChannel(int clientSocket);
		bool	isInvited(int clientSocket);
		void	addOperator(const Client& op);
		void	addUser(const Client& user);
		void	addInvitee(const Client& invitee);
		void	sendMessage(int mode, int clientSocket, const std::string& msg);
};

#endif