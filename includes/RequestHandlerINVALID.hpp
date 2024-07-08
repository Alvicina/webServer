/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandlerINVALID.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 13:27:23 by alvicina          #+#    #+#             */
/*   Updated: 2024/07/08 12:05:09 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHANDLERINVALID_HPP
#define REQUESTHANDLERINVALID_HPP

#include "RequestHandler.hpp"

class RequestHandlerInvalid : public RequestHandler
{
	private:
		void ResponseContentFileRoutine(Response & response);
		void ResponseHeaderRoutine(Response & response);
		void ResponseContentType(Response & response);

	public:
		RequestHandlerInvalid(Request & request, int errorCode);
		RequestHandlerInvalid(Request & request);
		RequestHandlerInvalid(RequestHandlerInvalid & copy);
		RequestHandlerInvalid& operator=(RequestHandlerInvalid & other);
		virtual ~RequestHandlerInvalid();
		virtual Response* handleRequest(void);
};

#endif