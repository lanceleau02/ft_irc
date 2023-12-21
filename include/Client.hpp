/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 15:56:15 by laprieur          #+#    #+#             */
/*   Updated: 2023/12/21 14:32:10 by laprieur         ###   ########.fr       */
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

		const User&	getUser(int idx) const;
		size_t		getNbUsers() const;
		void		addUser(const User& user);
};

#endif