/*
* imx_usb_lib:
* IMX USB execution part available as a library.
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

#ifndef __IMX_USB_LIB_H__
#define __IMX_USB_LIB_H__

#ifdef __cplusplus
extern "C" {
#endif

struct sdp_work;

struct usb_app_args {
	int verify;
	struct sdp_work *cmd_head;
	char const *base_path;
	char const *conf_path;
	char const *sim_vidpid;
	int bus;
	int address;
};
typedef struct usb_app_args usb_app_args;

int usb_app_execute(usb_app_args* args);

#ifdef __cplusplus
}
#endif

#endif /* __IMX_USB_LIB_H__ */
