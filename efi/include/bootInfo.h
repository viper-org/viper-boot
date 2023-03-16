#ifndef VB_INFO_H
#define VB_INFO_H

typedef struct
{
    unsigned int* Base;
    unsigned long Size;

    unsigned int Horiz;
    unsigned int Vert;

    unsigned int Pitch;
} VBFramebuffer;

typedef struct
{
    VBFramebuffer Framebuffer;
} VBInfo;

#endif