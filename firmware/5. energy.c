#include <math.h>
#include <time.h>

#define ADC_SCALE       0.000805664
#define ACS_OFFSET      2.339
#define ACS_SENSITIVITY 0.100
#define MAINS_VOLTAGE   230.0

double calculate_current(int raw)
{
    double voltage = raw * ADC_SCALE;

    double current =
        fabs((ACS_OFFSET - voltage)
             / ACS_SENSITIVITY);

    if(current < 0.02)
        current = 0.0;

    return current;
}

double calculate_power(double current)
{
    return current * MAINS_VOLTAGE;
}

double update_energy(double energy,
                     double power,
                     double elapsed)
{
    return energy +
           ((power * elapsed)
            / 3600000.0);
}
