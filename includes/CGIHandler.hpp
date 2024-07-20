/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIHandler.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 10:11:57 by alvicina          #+#    #+#             */
/*   Updated: 2024/07/19 13:51:03 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIHANDLER_HPP
#define CGIHANDLER_HPP

#include "Request.hpp"
#include "Response.hpp"
#include "Server.hpp"

class CgiHandler
{
	private:
		Request 	*_request;
		std::string _content;
		char		**_env;
		char		**_args;
		std::map<std::string, std::string>  _mapEnv;
		
		std::string createPathToResource(void);
		void exceptionRoutine(int statusCode, Response *response);
		void contentForFile(Response *response, std::string & pathToResource);
		void contentForDIR(Response *response, std::string & pathToResource);
		void cgiExecute(Response *response, std::string & pathToResource);
		void initEnvironmentForCgi(std::string & pathToResource);
		void parseEnvironmentForCgi(Response *response);
		void initArgsForCgi(std::string & pathToResource, Response *response);
		void forkAndExecve(std::string & pathToResource, Response *response);
		std::string methodToString(int number);
		std::string getScriptName(void);
		
	public:
		CgiHandler(Request & request);
		~CgiHandler();
		CgiHandler(CgiHandler & copy);
		CgiHandler& operator=(CgiHandler & other);
		void setRequest(Request & request);
		Request* getRequest(void);
		void handleCgiRequest(Response * response);
		void setContent(std::string & content);
		std::string& getContent(void);
};
#endif