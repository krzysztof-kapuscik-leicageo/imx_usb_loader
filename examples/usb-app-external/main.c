#include <string.h>
#include <stdio.h>

#include "imx_usb_loader/osal.h"
#include "imx_usb_loader/imx_usb_lib.h"

#ifndef WIN32
#define PATH_SEPARATOR '/'
#else
#define PATH_SEPARATOR '\\'
#endif

//--------------------------------------------------------

#define DBGMSG(x)   x

//--------------------------------------------------------

static const char* FILE_DATA_IMX_USB_CONF =
"#vid:pid, config_file\n"
"0x066f:0x3780, mx23_usb_work.conf\n"
"0x15a2:0x004f, mx28_usb_work.conf\n"
"0x15a2:0x0052, mx50_usb_work.conf\n"
"0x15a2:0x0054, mx6_usb_work.conf\n"
"0x15a2:0x0061, mx6_usb_work.conf\n"
"0x15a2:0x0063, mx6_usb_work.conf\n"
"0x15a2:0x0071, mx6_usb_work.conf\n"
"0x15a2:0x007d, mx6_usb_work.conf\n"
"0x15a2:0x0080, mx6ull_usb_work.conf\n"
"0x1fc9:0x0128, mx6_usb_work.conf\n"
"0x15a2:0x0076, mx7_usb_work.conf\n"
"0x1fc9:0x0126, mx7ulp_usb_work.conf\n"
"0x15a2:0x0041, mx51_usb_work.conf\n"
"0x15a2:0x004e, mx53_usb_work.conf\n"
"0x15a2:0x006a, vybrid_usb_work.conf\n"
"0x066f:0x37ff, linux_gadget.conf\n"
"0x1b67:0x4fff, mx6_usb_sdp_spl.conf\n"
"0x0525:0xb4a4, mx6_usb_sdp_spl.conf\n"
"0x1fc9:0x012b, mx8mq_usb_work.conf\n"
"0x1fc9:0x0134, mx8mm_usb_work.conf\n"
"0x3016:0x1000, mx8mm_usb_sdp_spl.conf\n"
;

static const char* FILE_DATA_IMX_MX6_USB_WORK_CONF =
"mx6_qsb\n"
"#hid/bulk,[old_header,]max packet size, dcd_addr, {ram start, ram size}(repeat valid ram areas)\n"
"hid,1024,0x910000,0x10000000,1G,0x00900000,0x40000\n"
;

//--------------------------------------------------------

static const char* get_file_data(const char* filename, int* length)
{
    const char* data = NULL;

    *length = 0;

    if (filename[0] == PATH_SEPARATOR)
    {
        ++filename;

        if (strcmp(filename, "imx_usb.conf") == 0)
        {
            data = FILE_DATA_IMX_USB_CONF;
            *length = strlen(data);
        }
        else if (strcmp(filename, "mx6_usb_work.conf") == 0)
        {
            data = FILE_DATA_IMX_MX6_USB_WORK_CONF;
            *length = strlen(data);
        }
    }

    DBGMSG(printf("get_file_data(%s,%p) -> %p\n", filename, length, data);)

    return data;
}

//--------------------------------------------------------

struct file_impl
{
    const char* data;
    int length;
    int current_pos;
};
typedef struct file_impl file_impl;

static imx_osal_file* osal_cast_to_osal_file(file_impl* file)
{
    return (imx_osal_file*)file;
}

static file_impl* osal_cast_to_file_impl(imx_osal_file* file)
{
    return (file_impl*)file;
}

static imx_osal_file* osal_file_open(const char *filename, const char *mode)
{
    DBGMSG(printf("osal_file_open(%s,%s)\n", filename, mode);)

    imx_osal_file* file = NULL;
    const char* data = NULL;
    int length = 0;

    if (strcmp(mode, "rb") == 0)
    {
        data = get_file_data(filename, &length);
    }

    if (data != NULL)
    {
        file_impl* file_impl = calloc(1, sizeof(*file_impl));
        file_impl->data = data;
        file_impl->length = length;
        file_impl->current_pos = 0;

        file = osal_cast_to_osal_file(file_impl);
    }

    DBGMSG(printf("osal_file_open(%s,%s) -> %p\n", filename, mode, file);)

    return file;
}

static int osal_file_close(imx_osal_file *stream)
{
    DBGMSG(printf("osal_file_close(%p)\n", stream);)

    file_impl* file = osal_cast_to_file_impl(stream);

    memset(file, 0, sizeof(file));
    free(file);

    return 0;
}

static char* osal_file_gets(char *str, int num, imx_osal_file *stream)
{
    file_impl* file = osal_cast_to_file_impl(stream);

    char* dst = str;

    if (num < 0)
    {
        return NULL;
    }

    if (file->current_pos == file->length)
    {
        return NULL;
    }

    while ((file->current_pos < file->length) && (num > 1))
    {
        char c = file->data[file->current_pos++];
        *dst++ = c;
        --num;
        if (c == '\n')
        {
            break;
        }
    }

    if (num > 0)
    {
        *dst = 0;
    }

//    DBGMSG(printf("osal_file_gets(%p,%d,%p) -> %s\n", str, num, stream, str);)

    return str;
}

static int osal_file_seek(imx_osal_file *stream, long int offset, int origin)
{
    file_impl* file = osal_cast_to_file_impl(stream);
    int result = -1;

    switch (origin)
    {
        case SEEK_SET:
        {
            if ((offset >= 0) && (offset <= file->length))
            {
                file->current_pos = offset;
                result = 0;
            }
        }
        break;

        case SEEK_CUR:
        {
            long int pos = file->current_pos + offset;
            if ((pos >= 0) && (pos <= file->length))
            {
                file->current_pos = pos;
                result = 0;
            }
        }
        break;

        case SEEK_END:
        {
            long int pos = file->length + offset;
            if ((pos >= 0) && (pos <= file->length))
            {
                file->current_pos = pos;
                result = 0;
            }
        }
        break;

        default:
            break;
    }

    return result;
}

static size_t osal_file_read(void *ptr, size_t size, size_t count, imx_osal_file *stream)
{
    file_impl* file = osal_cast_to_file_impl(stream);

    int fixed_count = (int)(size * count);

    if (fixed_count > file->length - file->current_pos)
    {
        fixed_count = file->length - file->current_pos;
    }
    fixed_count /= size;

    memcpy(ptr, &file->data[file->current_pos], fixed_count * size);

    return fixed_count;
}

static void osal_file_rewind(imx_osal_file *stream)
{
    file_impl* file = osal_cast_to_file_impl(stream);
    file->current_pos = 0;
}

static long int osal_file_tell(imx_osal_file *stream)
{
    file_impl* file = osal_cast_to_file_impl(stream);

    return file->current_pos;
}

static int osal_file_access_read(const char *filename)
{
    int length;

    if (get_file_data(filename, &length) != NULL)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

static imx_osal_ops osal_ops_impl =
{
    osal_file_open,
    osal_file_close,
    osal_file_gets,
    osal_file_seek,
    osal_file_read,
    osal_file_rewind,
    osal_file_tell,
    osal_file_access_read
};


//--------------------------------------------------------

int main(int argc, char* argv)
{
    imx_set_osal(&osal_ops_impl);

    usb_app_args app_args = {
        1, /* verify */
        NULL, /* cmd_head */
        "", /* base_path */
        "", /* conf_path */
        "0x15a2:0x0061", /* sim_vidpid */
        0, /* bus - simulation: not used */
        0, /* address - simulation: not used */
    };

    return usb_app_execute(&app_args);
}
