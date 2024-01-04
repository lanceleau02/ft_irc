/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 12:38:30 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/04 13:11:17 by laprieur         ###   ########.fr       */
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

class User;

class Channel {
	private:
		int					_userLimit;
		bool				_inviteOnly;
		bool				_topicRestrictions;
		bool				_channelKey;
		std::string			_name;
		std::string			_topic;
		std::string			_password;
		std::map<int, User>	_operators;
		std::map<int, User>	_users;
		std::map<int, User>	_invitees;

	public:
		Channel(const User& op, const std::string& name);
		~Channel();

		int							getUserLimit();
		int							getNbUsers();
		bool						getInviteMode();
		bool						getPasswordMode();
		const std::string&			getPassword();
		const std::map<int, User>&	getMap(int type);

		bool	isOnChannel(int userSocket);
		void	addOperator(const User& op);
		void	addUser(const User& user);
		void	addInvitee(const User& invitee);
};

#endif