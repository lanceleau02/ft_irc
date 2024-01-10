/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 11:54:49 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/10 12:01:11 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* ************************************************************************** */
/* Error Replies:                                                             */
/* 403	ERR_NOSUCHCHANNEL		"<channel name> :No such channel"             */
/* 441	ERR_USERNOTINCHANNEL	"<nick> <chan> :They aren't on that channel"  */
/* 442	ERR_NOTONCHANNEL		"<channel> :You're not on that channel"       */
/* 461	ERR_NEEDMOREPARAMS		"<command> :Not enough parameters"            */
/* 482	ERR_CHANOPRIVSNEEDED	"<channel> :You're not channel operator"      */
/* ************************************************************************** */