/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestFactory.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 18:51:51 by alvicina          #+#    #+#             */
/*   Updated: 2024/07/05 13:41:22 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTFACTORY_HPP
#define REQUESTFACTORY_HPP

#include "webserv.hpp"
#include "RequestHandler.hpp"
#include "RequestHandlerDELETE.hpp"
#include "RequestHandlerGET.hpp"
#include "RequestHandlerPOST.hpp"
#include "RequestHandlerINVALID.hpp"
#include "Request.hpp"

class RequestFactory
{
	public:
		RequestFactory();
		~RequestFactory();
		RequestFactory(RequestFactory & copy);
		RequestFactory& operator=(RequestFactory & other);
		static RequestHandler* makeRequestHandler(Request & request);	
};

#endif