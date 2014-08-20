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
   Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include <config.h>
#include <glibtop.h>
#include <glibtop/parameter.h>
#include <glibtop/error.h>

#define _write_data(ptr,size)	\
	if ((data_ptr == NULL) || (data_size < size)) return -size; \
	if (ptr == NULL) { char * const p = data_ptr; *p = '\0'; return 1; } \
	memcpy (data_ptr, ptr, size);	\
	return size;

#define _check_data(size)	\
	if ((data_ptr == NULL) || (data_size != size)) { \
		glibtop_warn_r (server, "glibtop_set_parameter (%u): " \
				"Expected %lu bytes but got %lu.", \
				(unsigned) parameter, (unsigned long) size, (unsigned long) data_size); \
		return; \
	}

#define _strlen(ptr)	(ptr ? strlen (ptr) : 0)

size_t
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
	case GLIBTOP_PARAM_ERROR_METHOD:
		_write_data (&server->error_method,
			     sizeof (server->error_method));
	case GLIBTOP_PARAM_REQUIRED:
		_write_data (&server->required,
			     sizeof (server->required));
	}

	return 0;
}

void
glibtop_set_parameter_l (glibtop *server, const unsigned parameter,
			 const void *data_ptr, size_t data_size)
{
	switch (parameter) {
	case GLIBTOP_PARAM_METHOD:
		_check_data (sizeof (server->method));
		memcpy (&server->method, data_ptr, data_size);
		break;
	case GLIBTOP_PARAM_FEATURES:
		/* You should not be allowed to set this field. */
		glibtop_warn_r (server, "glibtop_set_parameter (%u): " \
				"Cannot modify read-only value.",
				parameter);
		break;
	case GLIBTOP_PARAM_ERROR_METHOD:
		_check_data (sizeof (server->error_method));
		memcpy (&server->error_method, data_ptr, data_size);
		break;
	case GLIBTOP_PARAM_REQUIRED:
		_check_data (sizeof (server->required));
		memcpy (&server->required, data_ptr, data_size);
		break;
	}
}
