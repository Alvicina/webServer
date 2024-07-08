/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandlerGET.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 11:42:18 by alvicina          #+#    #+#             */
/*   Updated: 2024/07/08 18:40:49 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHANDLERGET_HPP
#define REQUESTHANDLERGET_HPP

#include "RequestHandler.hpp"

class Response; 

class RequestHandlerGet : public RequestHandler
{
	private:

	public:
		RequestHandlerGet(Request & request);
		RequestHandlerGet(RequestHandlerGet & copy);
		RequestHandlerGet& operator=(RequestHandlerGet & other);
		virtual ~RequestHandlerGet();
		virtual Response* doHandleRequest(void);
};

#endif