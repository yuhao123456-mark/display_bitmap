// display_bitmap.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "display_bitmap_resources.h"
#include "display_bitmap_specifications.h"
#include "gx_api.h"
#include "windows.h"
#include <stdio.h>
GX_WINDOW_ROOT *root;

extern UINT win32_graphics_driver_setup_565rgb(GX_DISPLAY *display);

#define TYPE_OPERAND 1
#define MAX_TEXT_LENGTH 3

typedef struct
{
    GX_CHAR *text;
    INT x_offset;
    INT y_offset;
    USHORT widget_id;
    USHORT event_type;
    LONG type;
} KEY_INFO_ENTRY;

//KEY_INFO_ENTRY key_info[] = {"hello world", 0, 0, IDC_BUTTON, GX_SIGNAL(IDC_BUTTON, GX_EVENT_CLICKED), TYPE_OPERAND};
//
//VOID custom_pixelmap_button_draw(GX_PIXELMAP_BUTTON *widget)
//{
//    GX_STRING text;
//    text.gx_string_ptr = "hellow world";
//    text.gx_string_length = string_length_get(key_info->text, MAX_TEXT_LENGTH);
//    //gx_pixelmap_button_draw(widget);
//    //gx_widget_text_draw_ext(widget, DISPLAY_1_COLOR_TABLE_SIZE, DISPLAY_1_FONT_TABLE_SIZE, &text, key_info->x_offset, key_info->y_offset);
//
//}

VOID custom_pixelmap_canvas_draw(GX_WINDOW *widget)
{
}

VOID custom_pixelmap_button_draw(GX_ICON_BUTTON *widget)
{
    gx_icon_button_draw(widget);
}

UINT canvas_event_handler(GX_WINDOW *widget, GX_EVENT *event_ptr)
{

    GX_VALUE width = 0;
    GX_VALUE height = 0;
    WINDOW_CONTROL_BLOCK current_button;

    gx_window_event_process(widget, event_ptr);

    GX_WIDGET *window;
    window = (GX_WIDGET *)widget;
    if (event_ptr->gx_event_type == GX_EVENT_PEN_DRAG)
    {
        if (widget->gx_window_move_mode)
        {

            int xShift = (GX_VALUE)(event_ptr->gx_event_payload.gx_event_pointdata.gx_point_x -
                                    widget->gx_window_move_start.gx_point_x);
            int yShift = (GX_VALUE)(event_ptr->gx_event_payload.gx_event_pointdata.gx_point_y -
                                    widget->gx_window_move_start.gx_point_y);

            widget->gx_window_move_start = event_ptr->gx_event_payload.gx_event_pointdata;
            _gx_widget_shift(window, xShift, yShift, GX_TRUE);
        }
    }
    //MessageBox(0, 1, 0, 0);

    return 1;
}

UINT string_length_get(GX_CONST GX_CHAR *input_string, UINT max_string_length)
{
    UINT length = 0;

    if (input_string)
    {
        /* Traverse the string.  */
        for (length = 0; input_string[length]; length++)
        {
            /* Check if the string length is bigger than the max string length.  */
            if (length >= max_string_length)
            {
                break;
            }
        }
    }

    return length;
}

VOID tx_application_define(void *first_unused_memory)
{
    /* Initialize GUIX.  */
    gx_system_initialize();

    gx_studio_display_configure(DISPLAY_1, win32_graphics_driver_setup_565rgb,
                                LANGUAGE_ENGLISH, DISPLAY_1_THEME_1, &root);

    /* Create calculator screen */
    gx_studio_named_widget_create("window", (GX_WIDGET *)root, GX_NULL);

    /* Show the root window to make it and patients screen visible.  */
    gx_widget_show(root);

    /* start GUIX thread */
    gx_system_start();
    _gxe_widget_status_add(root, GX_STATUS_MOVABLE);
}

int main(int argc, char **argv)
{
    AllocConsole();
    FILE *new_stream = 0;
    freopen_s(&new_stream, "CONOUT$", "w", stdout);
    printf("retarget_console");
    tx_kernel_enter();
    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧:
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
