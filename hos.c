#include "hospital.h"

// Lookup Table Initializations
const float BASE_FEES[NUM_SPECIALTIES] = {1500.0, 2500.0, 4500.0, 5000.0};
const int CONSULT_TIMES[NUM_SPECIALTIES] = {15, 20, 30, 30};
const float WARD_RATES[NUM_WARDS] = {3000.0, 6000.0, 12000.0, 25000.0};
const int WARD_CAPACITIES[NUM_WARDS] = {20, 10, 10, 5};

const char* SPEC_NAMES[NUM_SPECIALTIES] = {"General Practice (OPD)", "Paediatrics", "Cardiology", "Neurology"};
const char* WARD_NAMES[NUM_WARDS] = {"General Ward", "Paediatric Ward", "Surgical Ward", "ICU"};

// Array Definitions
char p_names[MAX_PATIENTS][50];
int p_ages[MAX_PATIENTS], p_urgency[MAX_PATIENTS], p_spec[MAX_PATIENTS];
int p_ward[MAX_PATIENTS], p_days[MAX_PATIENTS];
float p_gross[MAX_PATIENTS], p_discount[MAX_PATIENTS], p_final[MAX_PATIENTS];

int patient_count = 0;
int spec_queue[NUM_SPECIALTIES] = {0};
int bedOccupancy[NUM_WARDS][20] = {0};

void initializeSystem(void) {
    loadBedsFromFile();
}

void displayMenu(void) {
    printf("\n");
    printf("=========================================================\n");
    printf("       SMART HOSPITAL & RESOURCE ALLOCATION SYSTEM       \n");
    printf("=========================================================\n");
    printf("  [1] Register New Patient\n");
    printf("  [2] View Bed Occupancy Matrix\n");
    printf("  [3] View Emergency Triage Queue\n");
    printf("  [4] Generate Performance Reports\n");
    printf("  [5] Save System Data & Exit\n");
    printf("---------------------------------------------------------\n");
    printf("  > Please select an option (1-5): ");
}

void registerPatient(void) {
    if (patient_count >= MAX_PATIENTS) {
        printf("\n  [!] System is currently at maximum patient capacity.\n");
        return;
    }
    int i = patient_count;

    printf("\n=========================================================\n");
    printf("                  PATIENT REGISTRATION                   \n");
    printf("=========================================================\n");

    printf("  > Full Name: ");
    scanf(" %[^\n]s", p_names[i]);

    printf("  > Age (Years): ");
    scanf("%d", &p_ages[i]);

    printf("\n  --- Triage Assessment ---\n");
    printf("  [1] Normal\n  [2] Urgent\n  [3] Critical\n");
    printf("  > Select Urgency Level (1-3): ");
    scanf("%d", &p_urgency[i]);

    printf("\n  --- Department Routing ---\n");
    printf("  [1] General Practice (OPD)\n  [2] Paediatrics\n");
    printf("  [3] Cardiology\n  [4] Neurology\n");
    printf("  > Select Specialty ID (1-4): ");
    scanf("%d", &p_spec[i]);

    printf("\n  --- Ward Admission ---\n");
    int isAdmitted;
    printf("  > Does the patient require ward admission? (1=Yes, 0=No): ");
    scanf("%d", &isAdmitted);

    int allocated_bed = -1;
    if (isAdmitted == 1) {
        printf("  [1] General Ward\n  [2] Paediatric Ward\n");
        printf("  [3] Surgical Ward\n  [4] ICU\n");
        printf("  > Select Ward ID (1-4): ");
        scanf("%d", &p_ward[i]);
        printf("  > Estimated Days Admitted: ");
        scanf("%d", &p_days[i]);

        int w_idx = p_ward[i] - 1;
        for (int b = 0; b < WARD_CAPACITIES[w_idx]; b++) {
            if (bedOccupancy[w_idx][b] == 0) {
                bedOccupancy[w_idx][b] = 1;
                allocated_bed = b + 1;
                break;
            }
        }
        if (allocated_bed == -1) {
            printf("\n  [!] Warning: No beds currently available in %s.\n", WARD_NAMES[w_idx]);
        } else {
            printf("\n  [+] Successfully allocated Bed #%02d in %s.\n", allocated_bed, WARD_NAMES[w_idx]);
        }
    } else {
        p_ward[i] = 0;
        p_days[i] = 0;
    }

    // Mathematical Modelling for Billing
    int s_idx = p_spec[i] - 1;
    float base_fee = BASE_FEES[s_idx];
    float surcharge = 0.0;
    int surcharge_pct = 0;

    if (p_urgency[i] == 2) { surcharge = base_fee * 0.20; surcharge_pct = 20; }
    else if (p_urgency[i] == 3) { surcharge = base_fee * 0.50; surcharge_pct = 50; }

    float ward_cost = (p_ward[i] > 0) ? (p_days[i] * WARD_RATES[p_ward[i] - 1]) : 0.0;
    p_gross[i] = base_fee + surcharge + ward_cost;

    int subsidy_eligible = (p_ages[i] < 5 || p_ages[i] > 65) ? 1 : 0;
    p_discount[i] = subsidy_eligible ? (p_gross[i] * 0.15) : 0.0;
    p_final[i] = p_gross[i] - p_discount[i];

    float wait_time = spec_queue[s_idx] * CONSULT_TIMES[s_idx];
    spec_queue[s_idx]++;

    // Formatting matches Requirement 5 of the PDF explicitly[cite: 1]
    printf("\nSMART HOSPITAL ADMISSION & BILL\n");
    printf("%-23s: PAT-%d\n", "Patient ID", 1001 + i);
    printf("%-23s: %s\n", "Patient Name", p_names[i]);

    if (subsidy_eligible) {
        printf("%-23s: %d Years (15%% Subsidy Eligible)\n", "Age", p_ages[i]);
    } else {
        printf("%-23s: %d Years\n", "Age", p_ages[i]);
    }

    printf("%-23s: %s\n", "Specialty", SPEC_NAMES[s_idx]);

    if (p_ward[i] > 0 && allocated_bed != -1) {
        printf("%-23s: %s (Bed #%02d)\n", "Assigned Ward", WARD_NAMES[p_ward[i]-1], allocated_bed);
    } else {
        printf("%-23s: N/A (Outpatient)\n", "Assigned Ward");
    }

    const char* urgency_str = (p_urgency[i] == 1) ? "Level 1 (Normal)" : (p_urgency[i] == 2) ? "Level 2 (Urgent)" : "Level 3 (Critical)";
    printf("%-23s: %s\n", "Urgency Level", urgency_str);

    printf("%-23s: LKR %.2f\n", "Base Consultation Fee", base_fee);

    if (surcharge > 0) {
        printf("%-23s: LKR %.2f (%d%%)\n", "Emergency Surcharge", surcharge, surcharge_pct);
    } else {
        printf("%-23s: LKR 0.00\n", "Emergency Surcharge");
    }

    char ward_cost_lbl[50];
    if (p_ward[i] > 0) {
        sprintf(ward_cost_lbl, "Ward Stay Cost (%d Days)", p_days[i]);
        printf("%-23s: LKR %.2f\n", ward_cost_lbl, ward_cost);
    } else {
        printf("%-23s: LKR 0.00\n", "Ward Stay Cost (0 Days)");
    }

    printf("%-23s: LKR %.2f\n", "Gross Total Bill", p_gross[i]);

    if (subsidy_eligible) {
        printf("%-23s: LKR -%.2f (15%%)\n", "Age Subsidy Discount", p_discount[i]);
    } else {
        printf("%-23s: LKR 0.00\n", "Age Subsidy Discount");
    }

    printf("%-23s: LKR %.2f\n", "Final Payable Amount", p_final[i]);

    if (wait_time == 0) {
        printf("%-23s: 0.00 mins (Immediate Attention)\n", "Estimated Waiting Time");
    } else {
        printf("%-23s: %.2f mins\n", "Estimated Waiting Time", wait_time);
    }

    appendPatientRecordToFile(i);
    patient_count++;
}

void displayBedMatrix(void) {
    printf("\n=========================================================\n");
    printf("               HOSPITAL BED OCCUPANCY MATRIX             \n");
    printf("=========================================================\n");
    for (int w = 0; w < NUM_WARDS; w++) {
        printf(" %-16s | ", WARD_NAMES[w]);
        for (int b = 0; b < WARD_CAPACITIES[w]; b++) {
            if (bedOccupancy[w][b] == 1)
                printf("[X] ");
            else
                printf("[O] ");
        }
        printf("\n");
    }
    printf("---------------------------------------------------------\n");
    printf(" Legend: [O] = Available   [X] = Occupied\n");
    printf("=========================================================\n");
}

void sortByPriority(void) {
    for (int i = 0; i < patient_count - 1; i++) {
        int max_idx = i;
        for (int j = i + 1; j < patient_count; j++) {
            if (p_urgency[j] > p_urgency[max_idx]) {
                max_idx = j;
            }
        }
        if (max_idx != i) {
            char temp_name[50];
            strcpy(temp_name, p_names[i]); strcpy(p_names[i], p_names[max_idx]); strcpy(p_names[max_idx], temp_name);

            int temp;
            temp = p_ages[i]; p_ages[i] = p_ages[max_idx]; p_ages[max_idx] = temp;
            temp = p_urgency[i]; p_urgency[i] = p_urgency[max_idx]; p_urgency[max_idx] = temp;
            temp = p_spec[i]; p_spec[i] = p_spec[max_idx]; p_spec[max_idx] = temp;
            temp = p_ward[i]; p_ward[i] = p_ward[max_idx]; p_ward[max_idx] = temp;
            temp = p_days[i]; p_days[i] = p_days[max_idx]; p_days[max_idx] = temp;

            float temp_f;
            temp_f = p_gross[i]; p_gross[i] = p_gross[max_idx]; p_gross[max_idx] = temp_f;
            temp_f = p_discount[i]; p_discount[i] = p_discount[max_idx]; p_discount[max_idx] = temp_f;
            temp_f = p_final[i]; p_final[i] = p_final[max_idx]; p_final[max_idx] = temp_f;
        }
    }
}

void displayPriorityList(void) {
    sortByPriority();
    printf("\n=========================================================\n");
    printf("                EMERGENCY TRIAGE QUEUE                   \n");
    printf("=========================================================\n");

    if (patient_count == 0) {
        printf("  No patients currently in the queue.\n");
    } else {
        printf("  #  | %-20s | %-10s | %-10s\n", "Patient Name", "Urgency", "Payable (LKR)");
        printf("---------------------------------------------------------\n");
        for (int i = 0; i < patient_count; i++) {
            const char* urg_lbl = (p_urgency[i] == 3) ? "CRITICAL" : (p_urgency[i] == 2) ? "URGENT" : "NORMAL";
            printf(" %02d. | %-20s | %-10s | %.2f\n", i + 1, p_names[i], urg_lbl, p_final[i]);
        }
    }
    printf("=========================================================\n");
}

void generateReports(void) {
    if (patient_count == 0) {
        printf("\n  [!] Insufficient data to generate reports.\n");
        return;
    }

    int lvl1 = 0, lvl2 = 0, lvl3 = 0;
    float total_revenue = 0, total_discount = 0;
    int max_idx = 0;

    for (int i = 0; i < patient_count; i++) {
        if (p_urgency[i] == 1) lvl1++;
        else if (p_urgency[i] == 2) lvl2++;
        else if (p_urgency[i] == 3) lvl3++;

        total_revenue += p_final[i];
        total_discount += p_discount[i];

        if (p_final[i] > p_final[max_idx]) max_idx = i;
    }

    printf("\n=========================================================\n");
    printf("            PERFORMANCE REPORTS & ANALYTICS              \n");
    printf("=========================================================\n");
    printf("  --- Triage Breakdown ---\n");
    printf("  Normal Cases (Level 1)   : %d\n", lvl1);
    printf("  Urgent Cases (Level 2)   : %d\n", lvl2);
    printf("  Critical Cases (Level 3) : %d\n", lvl3);
    printf("---------------------------------------------------------\n");
    printf("  --- Financial Summary ---\n");
    printf("  Total Revenue Collected  : LKR %.2f\n", total_revenue);
    printf("  Total Subsidies Granted  : LKR %.2f\n", total_discount);
    printf("---------------------------------------------------------\n");
    printf("  --- Highest Paying Patient ---\n");
    printf("  Name                     : %s\n", p_names[max_idx]);
    printf("  Total Bill               : LKR %.2f\n", p_final[max_idx]);
    printf("=========================================================\n");
}

void saveBedsToFile(void) {
    FILE *fp = fopen("beds_status.txt", "w");
    if (!fp) return;
    for (int w = 0; w < NUM_WARDS; w++) {
        for (int b = 0; b < WARD_CAPACITIES[w]; b++) {
            fprintf(fp, "%d ", bedOccupancy[w][b]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
    printf("\n  [+] Bed statuses successfully saved to 'beds_status.txt'.\n");
}

void loadBedsFromFile(void) {
    FILE *fp = fopen("beds_status.txt", "r");
    if (!fp) return;
    for (int w = 0; w < NUM_WARDS; w++) {
        for (int b = 0; b < WARD_CAPACITIES[w]; b++) {
            if (fscanf(fp, "%d", &bedOccupancy[w][b]) != 1) break;
        }
    }
    fclose(fp);
    printf("  [+] Previous bed statuses successfully loaded.\n");
}

void appendPatientRecordToFile(int i) {
    FILE *fp = fopen("patient_records.txt", "a");
    if (!fp) return;
    fprintf(fp, "PAT-%d | %-20s | Age: %02d | Urgency: %d | Bill: LKR %.2f\n",
            1001 + i, p_names[i], p_ages[i], p_urgency[i], p_final[i]);
    fclose(fp);
}
