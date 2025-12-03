#include <stdio.h>
int calculateFuel(int fuel, int consumption, int recharge, int solarBonus, int planet, int totalPlanets) {
    if (fuel <= 0) {
        printf("Planet %d: Fuel Remaining = 0 (Fuel exhausted!)\n", planet);
        return 0;
    }
    if (planet > totalPlanets) {
        printf("Mission Complete! All planets visited.\n");
        return fuel;
    }
    fuel -= consumption;
    
    fuel += recharge;

    if (planet % 4 == 0) {
        fuel += solarBonus;
        printf("Solar Recharge Activated at Planet %d!\n", planet);
    }
    if (fuel < 0) fuel = 0;
    
    printf("Planet %d: Fuel Remaining = %d\n", planet, fuel);
    return calculateFuel(fuel, consumption, recharge, solarBonus, planet + 1, totalPlanets);
}
int main() {
    int fuel, consumption, recharge, solarBonus, totalPlanets;
    printf("Enter initial fuel: ");
    scanf("%d", &fuel);

    printf("Enter fuel consumption per planet: ");
    scanf("%d", &consumption);
    
    printf("Enter fuel recharge in gravitational zones: ");
    scanf("%d", &recharge);
    
    printf("Enter solar recharge bonus after every 4th planet: ");
    scanf("%d", &solarBonus);

    printf("Enter total number of planets to visit: ");
    scanf("%d", &totalPlanets);
    
    printf("\n--- Spacecraft Fuel Tracking Report ---\n");
    
    int finalFuel = calculateFuel(fuel, consumption, recharge, solarBonus, 1, totalPlanets);
    if (finalFuel > 0)
        printf("\nJourney Successful! Final Fuel Remaining: %d\n", finalFuel);
    else
        printf("\nMission Failed! Fuel was not sufficient.\n");
    return 0;
}
