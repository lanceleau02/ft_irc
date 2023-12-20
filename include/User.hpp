/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 15:56:15 by laprieur          #+#    #+#             */
/*   Updated: 2023/12/20 15:53:08 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

#include "ft_irc.hpp"

#include <string>

enum userType {
	OPERATOR,
	REGULAR
};

class User {
	private:
		std::string	_nickname;
		std::string	_username;
		int			_type;
		bool		_isAuthentified;

	public:
		User();
		~User();

		const std::string&	getNickname() const;
		const bool&			getAuthentification() const;
		void				setNickname(const std::string& nickname);
		void				setAuthentification();
};

#endif