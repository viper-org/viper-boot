#ifndef REQUEST_FRAMEBUFFER_H
#define REQUEST_FRAMEBUFFER_H
#include <efi/efi.h>
#include <viper.h>

extern struct ViperFramebufferResponse fb;

struct ViperFramebufferResponse GetFramebuffer();

#endif