#include <stdio.h>

#define NUM_SPECIALTIES 4
#define NUM_WARDS 4
#define MAX_BEDS 20

const char *specialtyNames[NUM_SPECIALTIES] = {
    "General Practice (OPD)", "Paediatrics", "Cardiology", "Neurology"
};
const float specialtyFees[NUM_SPECIALTIES] = {
    1500.00, 2500.00, 4500.00, 5000.00
};
const int specialtyConsultTime[NUM_SPECIALTIES] = {
    15, 20, 30, 30
};
const int specialtyDailyCap[NUM_SPECIALTIES] = {
    30, 20, 12, 10
};

const char *wardNames[NUM_WARDS] = {
    "General Ward", "Paediatric Ward", "Surgical Ward", "ICU (Intensive Care Unit)"
};
const float wardDailyRate[NUM_WARDS] = {
    3000.00, 6000.00, 12000.00, 25000.00
};
const int wardBedCapacity[NUM_WARDS] = {
    20, 10, 10, 5
};

int bedOccupancy[NUM_WARDS][MAX_BEDS] = {0};

int specialtyQueueCount[NUM_SPECIALTIES] = {0};

void printSpecialtyTable(void);
void printWardTable(void);

int main(void) {
    printf("Smart Hospital Patient & Resource Allocation System\n");
    printf("System initializing...\n\n");

    printSpecialtyTable();
    printWardTable();

    return 0;
}

void printSpecialtyTable(void) {
    printf("---- Doctor Specialties ----\n");
    for (int i = 0; i < NUM_SPECIALTIES; i++) {
        printf("ID %d | %-25s | Fee: LKR %.2f | %d mins | Cap: %d/day\n",
               i + 1, specialtyNames[i], specialtyFees[i],
               specialtyConsultTime[i], specialtyDailyCap[i]);
    }
    printf("\n");
}

void printWardTable(void) {
    printf("---- Hospital Wards ----\n");
    for (int i = 0; i < NUM_WARDS; i++) {
        printf("ID %d | %-25s | Rate: LKR %.2f/day | Capacity: %d beds\n",
               i + 1, wardNames[i], wardDailyRate[i], wardBedCapacity[i]);
    }
    printf("\n");
}
