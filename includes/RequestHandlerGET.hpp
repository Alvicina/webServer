/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandlerGET.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 11:42:18 by alvicina          #+#    #+#             */
/*   Updated: 2024/07/05 12:52:34 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHANDLERGET_HPP
#define REQUESTHANDLERGET_HPP

#include "RequestHandler.hpp"

class RequestHandlerGet : public RequestHandler
{
	private:

	public:
		RequestHandlerGet(Request & request);
		RequestHandlerGet(RequestHandlerGet & copy);
		RequestHandlerGet& operator=(RequestHandlerGet & other);
		virtual ~RequestHandlerGet();
		virtual Response* handleRequest(void);
};

#endif