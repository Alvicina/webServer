/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandlerPOST.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 11:42:40 by alvicina          #+#    #+#             */
/*   Updated: 2024/07/05 12:52:20 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHANDLERPOST_HPP
#define REQUESTHANDLERPOST_HPP

#include "RequestHandler.hpp"

class RequestHandlerPost : public RequestHandler
{
	private:

	public:
		RequestHandlerPost(Request & request);
		RequestHandlerPost(RequestHandlerPost & copy);
		RequestHandlerPost& operator=(RequestHandlerPost & other);
		virtual ~RequestHandlerPost();
		virtual Response* handleRequest(void);
};

#endif