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

#ifndef __IMX_OSAL_H__
#define __IMX_OSAL_H__

#include <stdlib.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/* std FILE* replacement */
struct imx_osal_file;
typedef struct imx_osal_file imx_osal_file;

/* OSAL operations */
struct imx_osal_ops
{
	/* std fopen replacement */
	imx_osal_file* (*file_open)(const char *filename, const char *mode);

	/* std fclose replacement */
	int (*file_close)(imx_osal_file *stream);

	/* std fgets replacement */
	char * (*file_gets)(char *str, int num, imx_osal_file *stream);

	/* std fseek replacement */
	int (*file_seek)(imx_osal_file *stream, long int offset, int origin);

	/* std fread replacement */
	size_t (*file_read)(void *ptr, size_t size, size_t count, imx_osal_file *stream);

	/* std rewind replacement */
	void (*file_rewind)(imx_osal_file *stream);

	/* std ftell replacement */
	long int (*file_tell)(imx_osal_file *stream);

	/* posix access R_OK replacement */
	int (*file_access_read)(const char *filename);

	/* printf/fprintf wrapper */
	void(*print)(int error, const char* format, va_list args);
};
typedef struct imx_osal_ops imx_osal_ops;

/* Returns currently registered OSAL operations. */
const imx_osal_ops* imx_get_osal();

/* Sets OSAL operations to be used by the libraries. */
void imx_set_osal(const imx_osal_ops* ops);

#ifdef __cplusplus
}
#endif

#endif /* __IMX_OSAL_H__ */
