/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 12:38:30 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/05 13:29:12 by laprieur         ###   ########.fr       */
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

class Client;

class Channel {
	private:
		int						_userLimit;
		bool					_inviteOnly;
		bool					_topicRestrictions;
		bool					_channelKey;
		std::string				_name;
		std::string				_topic;
		std::string				_password;
		std::map<int, Client>	_operators;
		std::map<int, Client>	_users;
		std::map<int, Client>	_invitees;

	public:
		Channel(const Client& op, const std::string& name);
		~Channel();

		int								getUserLimit();
		int								getNbUsers();
		bool							getInviteMode();
		bool							getPasswordMode();
		const std::string&				getPassword();
		const std::map<int, Client>&	getMap(int type);

		bool	isOnChannel(int clientSocket);
		void	addOperator(const Client& op);
		void	addUser(const Client& user);
		void	addInvitee(const Client& invitee);
};

#endif