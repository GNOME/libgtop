/* $Id$ */

/* Copyright (C) 1998-99 Martin Baulig
   This file is part of LibGTop 1.0.

   Contributed by Martin Baulig <martin@home-of-linux.org>, April 1998.

   LibGTop is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License,
   or (at your option) any later version.

   LibGTop is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with LibGTop; see the file COPYING. If not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#include <glibtop.h>
#include <glibtop/xmalloc.h>
#include <glibtop/parameter.h>

#define _write_data(ptr,size)	\
	if ((data_ptr == NULL) || (data_size < size)) \
		return -GLIBTOP_ERROR_SIZE_MISMATCH; \
	if (ptr == NULL) { strcpy (data_ptr, ""); return 1; } \
	memcpy (data_ptr, ptr, size);	\
	return size;

#define _check_data(size)	\
	if ((data_ptr == NULL) || (data_size != size)) { \
		glibtop_warn_r (server, "glibtop_set_parameter (%d): " \
				"Expected %lu bytes but got %lu.", \
				parameter, size, data_size); \
		return -GLIBTOP_ERROR_SIZE_MISMATCH; \
	}

#define _strlen(ptr)	(ptr ? strlen (ptr) : 0)

ssize_t
glibtop_get_parameter_l (glibtop *server, const unsigned parameter,
			 void *data_ptr, size_t data_size)
{
	switch (parameter) {
	case GLIBTOP_PARAM_METHOD:
		_write_data (&server->method,
			     sizeof (server->method));
	case GLIBTOP_PARAM_FEATURES:
		_write_data (&server->features,
			     sizeof (server->features));
	case GLIBTOP_PARAM_COMMAND:
		_write_data (server->server_command,
			     _strlen(server->server_command));
	case GLIBTOP_PARAM_HOST:
		_write_data (server->server_host,
			     _strlen(server->server_host));
	case GLIBTOP_PARAM_PORT:
		_write_data (&server->server_port,
			     sizeof (server->server_port));
	case GLIBTOP_PARAM_REMOTE_USER:
		_write_data (server->server_user,
			     _strlen(server->server_user));
	case GLIBTOP_PARAM_PATH_RSH:
		_write_data (server->server_rsh,
			     _strlen(server->server_rsh));
	case GLIBTOP_PARAM_ERROR_METHOD:
		_write_data (&server->error_method,
			     sizeof (server->error_method));
	case GLIBTOP_PARAM_REQUIRED:
		_write_data (&server->required,
			     sizeof (server->required));
	}

	return -GLIBTOP_ERROR_NO_SUCH_PARAMETER;
}

int
glibtop_get_parameter_size_l (glibtop *server, const unsigned parameter)
{
	switch (parameter) {
	case GLIBTOP_PARAM_METHOD:
	    return sizeof (server->method);
	case GLIBTOP_PARAM_FEATURES:
	    return sizeof (server->features);
	case GLIBTOP_PARAM_COMMAND:
	    return _strlen(server->server_command);
	case GLIBTOP_PARAM_HOST:
	    return _strlen(server->server_host);
	case GLIBTOP_PARAM_PORT:
	    return sizeof (server->server_port);
	case GLIBTOP_PARAM_REMOTE_USER:
	    return _strlen(server->server_user);
	case GLIBTOP_PARAM_PATH_RSH:
	    return _strlen(server->server_rsh);
	case GLIBTOP_PARAM_ERROR_METHOD:
	    return sizeof (server->error_method);
	case GLIBTOP_PARAM_REQUIRED:
	    return sizeof (server->required);
	}

	return -GLIBTOP_ERROR_NO_SUCH_PARAMETER;
}

int
glibtop_set_parameter_l (glibtop *server, const unsigned parameter,
			 const void *data_ptr, size_t data_size)
{
	switch (parameter) {
	case GLIBTOP_PARAM_METHOD:
		_check_data (sizeof (server->method));
		memcpy (&server->method, data_ptr, data_size);
		break;
	case GLIBTOP_PARAM_FEATURES:
		return -GLIBTOP_ERROR_READONLY_VALUE;
		break;
	case GLIBTOP_PARAM_COMMAND:
		if (server->server_command)
			glibtop_free_r (server, server->server_command);
		server->server_command = glibtop_strdup_r (server, data_ptr);
		break;
	case GLIBTOP_PARAM_HOST:
		if (server->server_host)
			glibtop_free_r (server, server->server_host);
		server->server_host = glibtop_strdup_r (server, data_ptr);
		break;
	case GLIBTOP_PARAM_PORT:
		_check_data (sizeof (server->server_port));
		memcpy (&server->server_port, data_ptr, data_size);
		break;
	case GLIBTOP_PARAM_REMOTE_USER:
		if (server->server_user)
			glibtop_free_r (server, server->server_user);
		server->server_user = glibtop_strdup_r (server, data_ptr);
		break;
	case GLIBTOP_PARAM_PATH_RSH:
		if (server->server_rsh)
			glibtop_free_r (server, server->server_rsh);
		server->server_rsh = glibtop_strdup_r (server, data_ptr);
		break;
	case GLIBTOP_PARAM_ERROR_METHOD:
		_check_data (sizeof (server->error_method));
		memcpy (&server->error_method, data_ptr, data_size);
		break;
	case GLIBTOP_PARAM_REQUIRED:
		_check_data (sizeof (server->required));
		memcpy (&server->required, data_ptr, data_size);
		break;
	default:
	  return -GLIBTOP_ERROR_NO_SUCH_PARAMETER;
	}

	return 0;
}
