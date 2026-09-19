#include "hospital.h"

int main(void) {

    printf("\n=========================================================\n");
    printf("              BOOTING SMART HOSPITAL SYSTEM              \n");
    printf("=========================================================\n\n");

    initializeSystem();

    int choice;

    do {
        displayMenu();
        if (scanf("%d", &choice) != 1) {

            while (getchar() != '\n');
            choice = 0;
        }

        switch (choice) {
            case 1:
                registerPatient();
                break;
            case 2:
                displayBedMatrix();
                break;
            case 3:
                displayPriorityList();
                break;
            case 4:
                generateReports();
                break;
            case 5:
                saveBedsToFile();
                printf("\n  [!] Shutting down system. Goodbye!\n");
                printf("=========================================================\n\n");
                break;
            default:
                printf("\n  [!] Invalid selection. Please enter a number between 1 and 5.\n");
        }
    } while (choice != 5);

    return 0;
}
