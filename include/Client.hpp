/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 15:56:15 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/03 14:08:25 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "ft_irc.hpp"

class User;

class Client {
	private:
		std::vector<User>	_users;

	public:
		Client();
		~Client();

		const User&			getUser(int idx) const;
		size_t				getNbUsers() const;
		const std::vector<User>&	getUsers() const;
		void		addUser(const User& user);
};

#endif