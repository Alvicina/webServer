/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandlerDELETE.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvicina <alvicina@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/05 11:39:28 by alvicina          #+#    #+#             */
/*   Updated: 2024/07/05 12:58:29 by alvicina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHANDLERDELETE_HPP
#define REQUESTHANDLERDELETE_HPP

#include "RequestHandler.hpp"

class RequestHandlerDelete : public RequestHandler
{
	private:

	public:
		RequestHandlerDelete(Request & request);
		RequestHandlerDelete(RequestHandlerDelete & copy);
		RequestHandlerDelete& operator=(RequestHandlerDelete & other);
		virtual ~RequestHandlerDelete();
		virtual Response* handleRequest(void);
};


#endif