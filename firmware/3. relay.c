#include <stdio.h>

int relay_status = 0;

void relay_on()
{
    FILE *fp = fopen("/dev/ttyS2", "w");

    if (!fp)
    {
        printf("Cannot open /dev/ttyS2\n");
        return;
    }

    unsigned char cmd[] =
    {
        0xFF, 0x05, 0x00, 0x00,
        0xFF, 0x00, 0x99, 0xE4
    };

    fwrite(cmd, 1, sizeof(cmd), fp);

    fclose(fp);

    relay_status = 1;

    printf("Relay ON\n");
}

void relay_off()
{
    FILE *fp = fopen("/dev/ttyS2", "w");

    if (!fp)
    {
        printf("Cannot open /dev/ttyS2\n");
        return;
    }

    unsigned char cmd[] =
    {
        0xFF, 0x05, 0x00, 0x00,
        0x00, 0x00, 0xD8, 0x14
    };

    fwrite(cmd, 1, sizeof(cmd), fp);

    fclose(fp);

    relay_status = 0;

    printf("Relay OFF\n");
}
