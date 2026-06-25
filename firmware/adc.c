#include <stdio.h>

#define ADC_FILE "/sys/bus/iio/devices/iio:device0/in_voltage1_raw"

int read_adc()
{
    FILE *fp;
    int raw = 0;

    fp = fopen(ADC_FILE, "r");

    if (!fp)
        return -1;

    fscanf(fp, "%d", &raw);

    fclose(fp);

    return raw;
}
