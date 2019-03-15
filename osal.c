/*
* osal:
* Operating system abstraction layer
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "osal.h"

#include <stdio.h>

static imx_osal_file *cast_to_osal_file(FILE *stream)
{
	return (imx_osal_file*)stream;
}

static FILE* cast_from_osal_file(imx_osal_file *stream)
{
	return (FILE*)stream;
}

static imx_osal_file *osal_file_open(const char *filename, const char *mode)
{
	return cast_to_osal_file(fopen(filename, mode));
}

static int osal_file_close(imx_osal_file *stream)
{
	return fclose(cast_from_osal_file(stream));
}

static char * osal_file_gets(char *str, int num, imx_osal_file *stream)
{
	return fgets(str, num, cast_from_osal_file(stream));
}

static int osal_file_seek(imx_osal_file *stream, long int offset, int origin)
{
	return fseek(cast_from_osal_file(stream), offset, origin);
}

static size_t osal_file_read(void *ptr, size_t size, size_t count, imx_osal_file *stream)
{
	return fread(ptr, size, count, cast_from_osal_file(stream));
}

static void osal_file_rewind(imx_osal_file *stream)
{
	rewind(cast_from_osal_file(stream));
}

long int osal_file_tell(imx_osal_file *stream)
{
	return ftell(cast_from_osal_file(stream));
}

struct imx_osal_ops default_osal_ops = {
	osal_file_open,
	osal_file_close,
	osal_file_gets,
	osal_file_seek,
	osal_file_read,
	osal_file_rewind,
	osal_file_tell
};

static imx_osal_ops* osal_ops = &default_osal_ops;

imx_osal_ops* imx_get_osal()
{
	return osal_ops;
}

void imx_set_osal(imx_osal_ops* ops)
{
	osal_ops = ops;
}
