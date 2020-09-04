#include "gx_api.h"

VOID pixelmap_compressed_write(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap)
{
    WAWO_LOG("pixelmap_compressed_write {%d, %d}", xpos, ypos);
    INT yval;
    INT xval;
    GX_CONST USHORT *get;
    USHORT *put;
    USHORT *putrow;
    USHORT count;
    USHORT pixel;

    GX_RECTANGLE *clip = context->gx_draw_context_clip;

    get = (GX_CONST USHORT *)pixelmap->gx_pixelmap_data;
    /* compressed with no alpha is a two-byte count and two-byte pixel value */

    /* first, skip to the starting row */
    for (yval = ypos; yval < clip->gx_rectangle_top; yval++)
    {
        xval = 0;
        while (xval < pixelmap->gx_pixelmap_width)
        {
            count = *get++;

            if (count & 0x8000)
            {
                count = (USHORT)((count & 0x7fff) + 1u);
                get++; /* skip repeated pixel value */
            }
            else
            {
                count++;
                get += count; /* skip raw pixel values */
            }
            xval += count;
        }
    }

    /* now we are on the first visible row, copy pixels until we get
        to the enf of the last visible row
      */
    putrow = (USHORT *)context->gx_draw_context_memory;
    putrow += yval * context->gx_draw_context_pitch;
    putrow += xpos;

    while (yval <= clip->gx_rectangle_bottom)
    {
        put = putrow;
        xval = xpos;

        while (xval < xpos + pixelmap->gx_pixelmap_width)
        {
            count = *get++;

            if (count & 0x8000)
            {
                /* repeated value */
                count = (USHORT)((count & 0x7fff) + 1u);
                pixel = *get++;
                while (count--)
                {
                    if (xval >= clip->gx_rectangle_left && xval <= clip->gx_rectangle_right)
                    {
                        *put = pixel;
                    }
                    put++;
                    xval++;
                }
            }
            else
            {
                /* string of non-repeated values */
                count++;

                while (count--)
                {
                    if (xval >= clip->gx_rectangle_left && xval <= clip->gx_rectangle_right)
                    {
                        *put = *get;
                    }
                    put++;
                    get++;
                    xval++;
                }
            }
        }
        putrow += context->gx_draw_context_pitch;
        yval++;
    }
}
