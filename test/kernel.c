#include <viper.h>

static volatile struct ViperFramebufferRequest fbReq = {
    .id = VIPER_FRAMEBUFFER
};

int kmain()
{
    struct ViperFramebufferResponse* fb = fbReq.response;
    unsigned char* screen = (unsigned char*)fb->base;
    unsigned int where = 50 * 4 + 50 * fb->pitch;
    screen[where] = 0xFF;
    screen[where + 1] = 0xFF;
    screen[where + 2] = 0xFF;
    for(;;);
}