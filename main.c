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
float calculateWaitTime(int specialtyIndex);
float calculateSurcharge(int urgency, float baseFee);
float calculateWardCost(int daysAdmitted, int wardIndex);
float calculateDiscount(int age, float grossTotal);

int patientCount = 0;
void printSpecialtyTable(void);
void printWardTable(void);
void printSpecialtyTable(void);
void printWardTable(void);
void showMenu(void);
void registerPatient(void);
void displayAllPatients(void);
void printBill(int i);
void displaySortedPatients(void);
void generateReport(void);
void saveBedsToFile(void);
void loadBedsFromFile(void);
void appendPatientRecord(int i);

int main(void) {
    printf("Smart Hospital Patient & Resource Allocation System\n");
    printf("System initializing...\n\n");
    loadBedsFromFile();

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
            case 3:
                displaySortedPatients();
                break;
            case 4:
                generateReport();
                break;
            case 0:
                saveBedsToFile();
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
    printf("3. Display Patients by Priority\n");
    printf("4. Generate Performance Report\n");
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

    int age;
    do {
        printf("Enter Patient Age: ");
        scanf("%d", &age);
        if (age < 0 || age > 120) {
            printf("Invalid age. Please enter a realistic age.\n");
        }
    } while (age < 0 || age > 120);
    patientAge[i] = age;

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

    int specIndex = patientSpecialtyID[i] - 1;
    float baseFee = specialtyFees[specIndex];

    patientWaitTime[i] = calculateWaitTime(specIndex);

    float surcharge = calculateSurcharge(patientUrgency[i], baseFee);

    float wardCost = 0.0;
    if (patientIsAdmitted[i] == 1) {
        int wardIndex = patientWardID[i] - 1;
        wardCost = calculateWardCost(patientDaysAdmitted[i], wardIndex);
    }

    float grossTotal = baseFee + surcharge + wardCost;
    float discount = calculateDiscount(patientAge[i], grossTotal);
    float finalAmount = grossTotal - discount;

    patientFinalBill[i] = finalAmount;

    patientCount++;
    printBill(i);
    appendPatientRecord(i);
}

void displayAllPatients(void) {
    printf("Total patients registered: %d\n", patientCount);
}

float calculateWaitTime(int specialtyIndex) {
    float waitTime = specialtyQueueCount[specialtyIndex] * specialtyConsultTime[specialtyIndex];
    specialtyQueueCount[specialtyIndex]++;
    return waitTime;
}

float calculateSurcharge(int urgency, float baseFee) {
    if (urgency == 2) {
        return baseFee * 0.20;
    } else if (urgency == 3) {
        return baseFee * 0.50;
    }
    return 0.0;
}

float calculateWardCost(int daysAdmitted, int wardIndex) {
    if (daysAdmitted <= 0) {
        return 0.0;
    }
    return daysAdmitted * wardDailyRate[wardIndex];
}

float calculateDiscount(int age, float grossTotal) {
    if (age < 5 || age > 65) {
        return grossTotal * 0.15;
    }
    return 0.0;
}

void printBill(int i) {
    int specIndex = patientSpecialtyID[i] - 1;
    float baseFee = specialtyFees[specIndex];
    float surcharge = calculateSurcharge(patientUrgency[i], baseFee);

    float surchargePercent = 0;
    if (patientUrgency[i] == 2) surchargePercent = 20;
    else if (patientUrgency[i] == 3) surchargePercent = 50;

    float wardCost = 0.0;
    if (patientIsAdmitted[i] == 1) {
        int wardIndex = patientWardID[i] - 1;
        wardCost = calculateWardCost(patientDaysAdmitted[i], wardIndex);
    }

    float grossTotal = baseFee + surcharge + wardCost;
    float discount = calculateDiscount(patientAge[i], grossTotal);

    const char *urgencyLabel;
    if (patientUrgency[i] == 1) urgencyLabel = "Level 1 (Normal)";
    else if (patientUrgency[i] == 2) urgencyLabel = "Level 2 (Urgent)";
    else urgencyLabel = "Level 3 (Critical)";

    printf("\n====================================================\n");
    printf(" SMART HOSPITAL ADMISSION & BILL\n");
    printf("----------------------------------------------------------------------------------------\n");
    printf("Patient ID     : PAT-%04d\n", 1000 + i + 1);
    printf("Patient Name   : %s\n", patientNames[i]);

    if (patientAge[i] < 5 || patientAge[i] > 65) {
        printf("Age            : %d Years (15%% Subsidy Eligible)\n", patientAge[i]);
    } else {
        printf("Age            : %d Years\n", patientAge[i]);
    }

    printf("Specialty      : %s\n", specialtyNames[specIndex]);

    if (patientIsAdmitted[i] == 1 && patientBedNumber[i] != -1) {
        int wardIndex = patientWardID[i] - 1;
        printf("Assigned Ward  : %s (Bed #%02d)\n", wardNames[wardIndex], patientBedNumber[i] + 1);
    } else if (patientIsAdmitted[i] == 1) {
        printf("Assigned Ward  : Waiting list (no bed available)\n");
    } else {
        printf("Assigned Ward  : Outpatient (Not Admitted)\n");
    }

    printf("Urgency Level  : %s\n", urgencyLabel);
    printf("----------------------------------------------------------------------------------------\n");
    printf("Base Consultation Fee   : LKR %.2f\n", baseFee);
    printf("Emergency Surcharge     : LKR %.2f (%.0f%%)\n", surcharge, surchargePercent);

    if (patientIsAdmitted[i] == 1) {
        printf("Ward Stay Cost (%d Days) : LKR %.2f\n", patientDaysAdmitted[i], wardCost);
    } else {
        printf("Ward Stay Cost          : LKR 0.00\n");
    }

    printf("----------------------------------------------------------------------------------------\n");
    printf("Gross Total Bill        : LKR %.2f\n", grossTotal);
    printf("Age Subsidy Discount    : LKR -%.2f (%.0f%%)\n", discount, (discount > 0) ? 15.0 : 0.0);
    printf("----------------------------------------------------------------------------------------\n");
    printf("Final Payable Amount    : LKR %.2f\n", patientFinalBill[i]);

    if (patientWaitTime[i] == 0) {
        printf("Estimated Waiting Time  : 0.00 mins (Immediate Attention)\n");
    } else {
        printf("Estimated Waiting Time  : %.2f mins\n", patientWaitTime[i]);
    }
    printf("====================================================\n");
}

void displaySortedPatients(void) {
    if (patientCount == 0) {
        printf("No patients registered yet.\n");
        return;
    }
    int order[MAX_PATIENTS];
    for (int k = 0; k < patientCount; k++) {
        order[k] = k;
    }

    for (int a = 0; a < patientCount - 1; a++) {
        for (int b = 0; b < patientCount - 1 - a; b++) {
            int idx1 = order[b];
            int idx2 = order[b + 1];
            if (patientUrgency[idx2] > patientUrgency[idx1]) {
                int temp = order[b];
                order[b] = order[b + 1];
                order[b + 1] = temp;
            }
        }
    }

    printf("\n========== PATIENTS BY PRIORITY ==========\n");
    for (int k = 0; k < patientCount; k++) {
        int i = order[k];
        const char *urgencyLabel;
        if (patientUrgency[i] == 1) urgencyLabel = "Normal";
        else if (patientUrgency[i] == 2) urgencyLabel = "Urgent";
        else urgencyLabel = "Critical";

        printf("%d. PAT-%04d | %-20s | Urgency: %-8s | Specialty: %s\n",
               k + 1, 1000 + i + 1, patientNames[i], urgencyLabel,
               specialtyNames[patientSpecialtyID[i] - 1]);
    }
    printf("============================================\n");
}

void generateReport(void) {
    if (patientCount == 0) {
        printf("No patients registered yet. Nothing to report.\n");
        return;
    }

    int normalCount = 0, urgentCount = 0, criticalCount = 0;
    for (int i = 0; i < patientCount; i++) {
        if (patientUrgency[i] == 1) normalCount++;
        else if (patientUrgency[i] == 2) urgentCount++;
        else if (patientUrgency[i] == 3) criticalCount++;
    }

    float totalRevenue = 0.0;
    float totalDiscounts = 0.0;
    for (int i = 0; i < patientCount; i++) {
        int specIndex = patientSpecialtyID[i] - 1;
        float baseFee = specialtyFees[specIndex];
        float surcharge = calculateSurcharge(patientUrgency[i], baseFee);
        float wardCost = 0.0;
        if (patientIsAdmitted[i] == 1) {
            int wardIndex = patientWardID[i] - 1;
            wardCost = calculateWardCost(patientDaysAdmitted[i], wardIndex);
        }
        float grossTotal = baseFee + surcharge + wardCost;
        float discount = calculateDiscount(patientAge[i], grossTotal);

        totalRevenue += patientFinalBill[i];
        totalDiscounts += discount;
    }

    float occupancyPercent[NUM_WARDS];
    for (int w = 0; w < NUM_WARDS; w++) {
        int occupiedBeds = 0;
        for (int b = 0; b < wardBedCapacity[w]; b++) {
            if (bedOccupancy[w][b] == 1) occupiedBeds++;
        }
        occupancyPercent[w] = ((float)occupiedBeds / wardBedCapacity[w]) * 100.0;
    }

    int topPayerIndex = 0;
    for (int i = 1; i < patientCount; i++) {
        if (patientFinalBill[i] > patientFinalBill[topPayerIndex]) {
            topPayerIndex = i;
        }
    }

    printf("\n============ HOSPITAL PERFORMANCE REPORT ============\n");
    printf("-- Patients by Urgency Level --\n");
    printf("  Normal (Level 1)   : %d\n", normalCount);
    printf("  Urgent (Level 2)   : %d\n", urgentCount);
    printf("  Critical (Level 3) : %d\n", criticalCount);
    printf("  Total Registered   : %d\n", patientCount);

    printf("\n-- Financials --\n");
    printf("  Total Revenue Earned    : LKR %.2f\n", totalRevenue);
    printf("  Total Discounts Granted : LKR %.2f\n", totalDiscounts);

    printf("\n-- Bed Occupancy per Ward --\n");
    for (int w = 0; w < NUM_WARDS; w++) {
        printf("  %-25s : %.1f%%\n", wardNames[w], occupancyPercent[w]);
    }

    printf("\n-- Highest-Paying Patient --\n");
    printf("  Name       : %s\n", patientNames[topPayerIndex]);
    printf("  Total Bill : LKR %.2f\n", patientFinalBill[topPayerIndex]);
    printf("=======================================================\n");
}

void saveBedsToFile(void) {
    FILE *file = fopen("beds_status.txt", "w");
    if (file == NULL) {
        printf("Error: could not save bed status.\n");
        return;
    }

    for (int w = 0; w < NUM_WARDS; w++) {
        for (int b = 0; b < wardBedCapacity[w]; b++) {
            fprintf(file, "%d ", bedOccupancy[w][b]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Bed status saved.\n");
}

void loadBedsFromFile(void) {
    FILE *file = fopen("beds_status.txt", "r");
    if (file == NULL) {
        return;
    }

    for (int w = 0; w < NUM_WARDS; w++) {
        for (int b = 0; b < wardBedCapacity[w]; b++) {
            fscanf(file, "%d", &bedOccupancy[w][b]);
        }
    }

    fclose(file);
    printf("Previous bed status loaded.\n");
}

void appendPatientRecord(int i) {
    FILE *file = fopen("patient_records.txt", "a");
    if (file == NULL) {
        printf("Error: could not save patient record.\n");
        return;
    }

    fprintf(file, "PAT-%04d | %s | Age: %d | Urgency: %d | Specialty: %s | Final Bill: LKR %.2f\n",
            1000 + i + 1, patientNames[i], patientAge[i], patientUrgency[i],
            specialtyNames[patientSpecialtyID[i] - 1], patientFinalBill[i]);

    fclose(file);
}
