/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandlerGET.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 11:42:18 by alvicina          #+#    #+#             */
/*   Updated: 2024/07/15 12:24:16 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHANDLERGET_HPP
#define REQUESTHANDLERGET_HPP

#include "RequestHandler.hpp"

class Response; 

class RequestHandlerGet : public RequestHandler
{
	private:
		void ResponseContentRoutine(Response *response);
		void setNewLocation(Request & request);
		void checkAndSetReturn(Request & request, bool & reddir);
		std::string createPathToResource(void);
		void contentForFile(Response *response, std::string & pathToResource);
		void contentForDIR(Response * response, std::string & pathToResource);
		void htmlIndexBuilder(Response * response);
		void openReadCopyFile(Response *response, std::string & pathToResource);
		void exceptionRoutine(int statusCode, Response *response);
	public:
		RequestHandlerGet(Request & request);
		RequestHandlerGet(RequestHandlerGet & copy);
		RequestHandlerGet& operator=(RequestHandlerGet & other);
		virtual ~RequestHandlerGet();
		virtual Response* doHandleRequest(void);
};

#endif