/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 4 -*- */

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
glibtop_get_parameter_l (glibtop_client *client, const unsigned parameter,
			 void *data_ptr, size_t data_size)
{
    switch (parameter) {
    case GLIBTOP_PARAM_ERROR_METHOD:
	_write_data (&server->_param.error_method,
		     sizeof (server->_param.error_method));
    case GLIBTOP_PARAM_FEATURES:
	_write_data (&server->_param.features,
		     sizeof (server->_param.features));
    case GLIBTOP_PARAM_NCPU:
	_write_data (&server->_param.ncpu,
		     sizeof (server->_param.ncpu));
    case GLIBTOP_PARAM_OS_VERSION_CODE:
	_write_data (&server->_param.os_version_code,
		     sizeof (server->_param.os_version_code));
    case GLIBTOP_PARAM_REQUIRED:
	_write_data (&server->_param.required,
		     sizeof (server->_param.required));
    }

    return -GLIBTOP_ERROR_NO_SUCH_PARAMETER;
}

int
glibtop_get_parameter_size_l (glibtop_client *client, const unsigned parameter)
{
    switch (parameter) {
    case GLIBTOP_PARAM_ERROR_METHOD:
	return sizeof (server->_param.error_method);
    case GLIBTOP_PARAM_FEATURES:
	return sizeof (server->_param.features);
    case GLIBTOP_PARAM_NCPU:
	return sizeof (server->_param.ncpu);
    case GLIBTOP_PARAM_OS_VERSION_CODE:
	return sizeof (server->_param.os_version_code);
    case GLIBTOP_PARAM_REQUIRED:
	return sizeof (server->_param.required);
    }

    return -GLIBTOP_ERROR_NO_SUCH_PARAMETER;
}

int
glibtop_set_parameter_l (glibtop_client *client, const unsigned parameter,
			 const void *data_ptr, size_t data_size)
{
    switch (parameter) {
    case GLIBTOP_PARAM_ERROR_METHOD:
	_check_data (sizeof (server->_param.error_method));
	memcpy (&server->_param.error_method, data_ptr, data_size);
	break;
    case GLIBTOP_PARAM_FEATURES:
    case GLIBTOP_PARAM_NCPU:
    case GLIBTOP_PARAM_OS_VERSION_CODE:
	return -GLIBTOP_ERROR_READONLY_VALUE;
	break;
    case GLIBTOP_PARAM_REQUIRED:
	_check_data (sizeof (server->_param.required));
	memcpy (&server->_param.required, data_ptr, data_size);
	break;
    default:
	return -GLIBTOP_ERROR_NO_SUCH_PARAMETER;
    }

    return 0;
}
