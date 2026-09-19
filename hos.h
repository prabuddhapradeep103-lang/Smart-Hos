#ifndef HOSPITAL_H
#define HOSPITAL_H

#include <stdio.h>
#include <string.h>

// Macro Constants
#define MAX_PATIENTS 100
#define NUM_SPECIALTIES 4
#define NUM_WARDS 4

// Global Lookup Arrays
extern const float BASE_FEES[NUM_SPECIALTIES];
extern const int CONSULT_TIMES[NUM_SPECIALTIES];
extern const float WARD_RATES[NUM_WARDS];
extern const int WARD_CAPACITIES[NUM_WARDS];

// Parallel Arrays for Patient Data
extern char p_names[MAX_PATIENTS][50];
extern int p_ages[MAX_PATIENTS];
extern int p_urgency[MAX_PATIENTS];
extern int p_spec[MAX_PATIENTS];
extern int p_ward[MAX_PATIENTS];
extern int p_days[MAX_PATIENTS];
extern float p_gross[MAX_PATIENTS];
extern float p_discount[MAX_PATIENTS];
extern float p_final[MAX_PATIENTS];

// System State Tracking
extern int patient_count;
extern int spec_queue[NUM_SPECIALTIES];
extern int bedOccupancy[NUM_WARDS][20];
