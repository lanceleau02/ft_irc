/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 12:38:30 by laprieur          #+#    #+#             */
/*   Updated: 2023/12/29 13:07:43 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "ft_irc.hpp"

enum AccessMode {
	FREE_ACCESS,
	INVITE_ONLY
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

		int					getUserLimit();
		int					getNbUsers();
		bool				getInviteMode();
		bool				getPasswordMode();
		const std::string&	getPassword();

		void	addOperator(const User& op);
		void	addUser(const User& user);
		void	addInvitee(const User& invitee);
};

#endif