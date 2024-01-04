/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 15:56:15 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/04 13:31:32 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "ft_irc.hpp"

class User;

class Client {
	private:
		std::map<int, User>	_users;

	public:
		Client();
		~Client();

		size_t						getNbUsers() const;
		const User&					getUser(int socket) const;
		const std::map<int, User>&	getUsers() const;
		
		void	addUser(int socket, const User& user);
};

#endif