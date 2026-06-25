#include <stdio.h>

double energy = 0.0;

void save_energy()
{
    FILE *fp = fopen("/data/energy.dat", "w");

    if (fp)
    {
        fprintf(fp, "%.8f\n", energy);
        fclose(fp);
    }
}

void load_energy()
{
    FILE *fp = fopen("/data/energy.dat", "r");

    if (fp)
    {
        fscanf(fp, "%lf", &energy);
        fclose(fp);

        printf("Loaded energy = %.6f kWh\n", energy);
    }
    else
    {
        energy = 0.0;
    }
}
