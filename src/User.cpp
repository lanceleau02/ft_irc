/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 13:42:32 by laprieur          #+#    #+#             */
/*   Updated: 2023/12/20 16:30:59 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User() : _nickname(""), _username(""), _type(REGULAR), _isAuthentified(false) {
	(void)_type;
	(void)_isAuthentified;
}

User::~User() {}

const std::string&	User::getNickname() const {
	return _nickname;
}

const bool&	User::getAuthentification() const {
	return _isAuthentified;
}

void	User::setNickname(const std::string& nickname) {
	_nickname = nickname;
}

void	User::setAuthentification() {
	_isAuthentified = true;
}