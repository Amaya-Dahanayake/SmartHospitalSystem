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

#define MAX_PATIENTS 100

char patientNames[MAX_PATIENTS][50];
int patientAge[MAX_PATIENTS];
int patientUrgency[MAX_PATIENTS];
int patientSpecialtyID[MAX_PATIENTS];
int patientIsAdmitted[MAX_PATIENTS];
int patientWardID[MAX_PATIENTS];
int patientBedNumber[MAX_PATIENTS];
int patientDaysAdmitted[MAX_PATIENTS];
float patientWaitTime[MAX_PATIENTS];
float patientFinalBill[MAX_PATIENTS];

int patientCount = 0;
void printSpecialtyTable(void);
void printWardTable(void);
void printSpecialtyTable(void);
void printWardTable(void);
void showMenu(void);
void registerPatient(void);
void displayAllPatients(void);

int main(void) {
    printf("Smart Hospital Patient & Resource Allocation System\n");
    printf("System initializing...\n\n");

    int choice;
    do {
        showMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                registerPatient();
                break;
            case 2:
                displayAllPatients();
                break;
            case 0:
                printf("Exiting system. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
        printf("\n");

    } while (choice != 0);

    return 0;
}

void showMenu(void) {
    printf("========== MAIN MENU ==========\n");
    printf("1. Register New Patient\n");
    printf("2. Display All Patients (test)\n");
    printf("0. Exit\n");
    printf("Enter your choice: ");
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

void registerPatient(void) {
    if (patientCount >= MAX_PATIENTS) {
        printf("Patient records are full. Cannot register more.\n");
        return;
    }

    int i = patientCount;

    printf("\n---- New Patient Registration ----\n");

    printf("Enter Patient Name: ");
    scanf(" %[^\n]", patientNames[i]);

    printf("Enter Patient Age: ");
    scanf("%d", &patientAge[i]);

    int urgency;
    do {
        printf("Enter Urgency Level (1 = Normal, 2 = Urgent, 3 = Critical): ");
        scanf("%d", &urgency);
        if (urgency < 1 || urgency > 3) {
            printf("Invalid urgency level. Please enter 1, 2, or 3.\n");
        }
    } while (urgency < 1 || urgency > 3);
    patientUrgency[i] = urgency;

    int specID;
    do {
        printf("Select Specialty:\n");
        for (int s = 0; s < NUM_SPECIALTIES; s++) {
            printf("  %d. %s\n", s + 1, specialtyNames[s]);
        }
        printf("Enter Specialty ID: ");
        scanf("%d", &specID);
        if (specID < 1 || specID > NUM_SPECIALTIES) {
            printf("Invalid specialty ID.\n");
        }
    } while (specID < 1 || specID > NUM_SPECIALTIES);
    patientSpecialtyID[i] = specID;

    int admitted;
    do {
        printf("Is patient admitted to a ward? (1 = Yes, 0 = No): ");
        scanf("%d", &admitted);
        if (admitted != 0 && admitted != 1) {
            printf("Invalid input. Enter 1 or 0.\n");
        }
    } while (admitted != 0 && admitted != 1);
    patientIsAdmitted[i] = admitted;

    if (admitted == 1) {
        int wardID;
        do {
            printf("Select Ward:\n");
            for (int w = 0; w < NUM_WARDS; w++) {
                printf("  %d. %s\n", w + 1, wardNames[w]);
            }
            printf("Enter Ward ID: ");
            scanf("%d", &wardID);
            if (wardID < 1 || wardID > NUM_WARDS) {
                printf("Invalid ward ID.\n");
            }
        } while (wardID < 1 || wardID > NUM_WARDS);
        patientWardID[i] = wardID;

        printf("Enter Days Admitted: ");
        scanf("%d", &patientDaysAdmitted[i]);

        int wardIndex = wardID - 1;
        int bedFound = -1;
        for (int b = 0; b < wardBedCapacity[wardIndex]; b++) {
            if (bedOccupancy[wardIndex][b] == 0) {
                bedOccupancy[wardIndex][b] = 1;
                bedFound = b;
                break;
            }
        }

        if (bedFound == -1) {
            printf("No beds available in %s! Patient placed on waiting list (no bed assigned).\n",
                   wardNames[wardIndex]);
            patientBedNumber[i] = -1;
        } else {
            patientBedNumber[i] = bedFound;
            printf("Assigned to %s, Bed #%02d\n", wardNames[wardIndex], bedFound + 1);
        }

    } else {
        patientWardID[i] = 0;
        patientDaysAdmitted[i] = 0;
        patientBedNumber[i] = -1;
    }

    patientCount++;
    printf("Patient registered successfully! (Patient ID: PAT-%04d)\n", 1000 + i + 1);
}

void displayAllPatients(void) {
    printf("Total patients registered: %d\n", patientCount);
}
