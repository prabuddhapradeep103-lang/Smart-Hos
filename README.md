# Smart Hospital & Resource Allocation System

## Overview
This is a modular, menu-driven Smart Hospital Patient & Resource Allocation System developed in the C programming language. It simulates daily hospital operations, including patient intake, bed allocation, emergency triage sorting, and medical billing estimations.

This project was developed as the individual assignment for the CSC 1012 Introduction to Computer Programming course at the University of Sri Jayewardenepura, Faculty of Applied Sciences.

## System Features
* **Patient Intake & Triage:** Registers patient demographics and categorizes urgency levels (Normal, Urgent, Critical)[cite: 1].
* **Resource Allocation:** Automatically assigns available beds across four specialized wards using a 2D bed occupancy matrix[cite: 1].
* **Dynamic Billing Model:** Calculates final payable amounts based on base consultation fees, emergency surcharges (20% or 50%), total ward stay costs, and age-based subsidy discounts (15% for children under 5 and seniors over 65)[cite: 1].
* **Priority Sorting:** Uses a selection sort algorithm to organize the triage queue primarily by Urgency Level (Level 3 first) and secondarily by registration order[cite: 1].
* **Performance Analytics:** Generates summary reports on patient breakdown, total revenue, total subsidies, and the highest-paying patient[cite: 1].
* **Persistent Data Storage:** Utilizes C file handling to save and load the hospital bed occupancy matrix (`beds_status.txt`) and append permanent patient billing logs (`patient_records.txt`)[cite: 1].

## Technical Architecture
The system is built using a modular architecture separating definitions, implementation, and execution:
* `hospital.h`: Contains macro constants, parallel array extern declarations, lookup table signatures, and function prototypes.
* `hospital.c`: Contains the core algorithmic implementation, mathematical modelling, file I/O operations, and parallel array manipulation.
* `main.c`: Contains the primary interactive console loop and system initialization.

### Data Structures
The system utilizes **Parallel 1D Arrays** to maintain patient records concurrently based on index mapping[cite: 1]. A **2D Integer Array** (`int bedOccupancy[4][20];`) is used to map and track real-time bed availability across different hospital wards[cite: 1].

## Installation & Execution
1. Clone this repository to your local machine:
   ```bash
   git clone [https://github.com/prabuddhapradeep103-lang/Smart-Hospital-System.git](https://github.com/YOUR_USERNAME/Smart-Hospital-System.git)
