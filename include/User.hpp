/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 15:56:15 by laprieur          #+#    #+#             */
/*   Updated: 2023/12/19 15:11:22 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

#include <string>

enum userType {
	OPERATOR,
	REGULAR
};

class User {
	private:
		std::string	_nickName;
		std::string	_userName;
		int			_type;
	
		User(const User& source);
		User& operator=(const User& source);

	public:
		User();
		~User();
};

#endif