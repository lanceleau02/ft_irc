/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: laprieur <laprieur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/10 12:51:16 by laprieur          #+#    #+#             */
/*   Updated: 2024/01/10 13:04:02 by laprieur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* ************************************************************************** */
/* Command Replies:                                                           */
/* 301	RPL_AWAY				"<nick> :<away message>"                      */
/* 341	RPL_INVITING			"<channel> <nick>"                            */
/* ************************************************************************** */

/* ************************************************************************** */
/* Error Replies:                                                             */
/* 401	ERR_NOSUCHNICK			"<nickname> :No such nick/channel"            */
/* 442	ERR_NOTONCHANNEL		"<channel> :You're not on that channel"       */
/* 443	ERR_USERONCHANNEL	"<user> <channel> :is already on channel"         */
/* 461	ERR_NEEDMOREPARAMS		"<command> :Not enough parameters"            */
/* 482	ERR_CHANOPRIVSNEEDED	"<channel> :You're not channel operator"      */
/* ************************************************************************** */