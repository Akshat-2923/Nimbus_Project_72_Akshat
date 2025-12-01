// static void time_str(time_t t, char *buf, size_t n) {
//     struct tm *tm = localtime(&t);
//     strftime(buf, n, "%Y-%m-%d %H:%M:%S", tm);
// }

// void register_patient() {
//     Patient p;
//     char namebuf[NAME_LEN];
//     char symbuf[SYM_LEN];
//     int age;
//     int dept_choice;
//     char emergency_choice;

//     printf("\n--- Patient Registration ---\n");
//     printf("Full name: ");
//     if (!fgets(namebuf, sizeof(namebuf), stdin)) return;
//     if (namebuf[strlen(namebuf)-1] == '\n') namebuf[strlen(namebuf)-1] = '\0';
//     strncpy(p.name, namebuf, NAME_LEN-1);

//     printf("Age: ");
//     if (scanf("%d", &age) != 1) { getchar(); return; }
//     p.age = age;
//     getchar();

//     printf("Select department: 1) OPD  2) Cardiology  3) Emergency  4) General\nChoice: ");
//     if (scanf("%d", &dept_choice) != 1) dept_choice = 1;
//     getchar();
//     if (dept_choice < 1 || dept_choice > 4) dept_choice = 1;
//     p.dept = (Dept)(dept_choice - 1);

//     printf("Brief symptoms (short): ");
//     if (!fgets(symbuf, sizeof(symbuf), stdin)) symbuf[0] = '\0';
//     if (symbuf[strlen(symbuf)-1] == '\n') symbuf[strlen(symbuf)-1] = '\0';
//     strncpy(p.symptoms, symbuf, SYM_LEN-1);

//     printf("Is this an emergency? (y/N): ");
//     if (scanf("%c", &emergency_choice) != 1) emergency_choice = 'n';
//     getchar();

//     p.priority = (emergency_choice == 'y' || emergency_choice == 'Y') ? 2 : 0;
//     p.arrival = time(NULL);

//     Queue *q = &queues[p.dept];
//     gen_token(p.dept, p.token, q->nextTokenNo++);
//     if (p.dept == EMERGENCY_DEPT && p.priority == 0) p.priority = 1;

//     if (enqueue_patient(p) == 0) {
//         printf("\nRegistered — Token #%s\n", p.token);
//         printf("Department: %s | Priority: %s\n",
//                DEPT_NAMES[p.dept],
//                p.priority == 2 ? "EMERGENCY" : (p.priority == 1 ? "URGENT" : "NORMAL"));
//         printf("View status: Use 'View Queues' and search token %s\n", p.token);
//     } else {
//         printf("Sorry, queue full. Try again later.\n");
//     }
// }

// void seed_demo_patients() {
//     Patient p;
//     p.age = 45;
//     strcpy(p.name, "Amit Kumar");
//     strcpy(p.symptoms, "Chest pain");
//     p.dept = CARDIO;
//     p.priority = 1;
//     p.arrival = time(NULL) - 300;
//     gen_token(p.dept, p.token, queues[p.dept].nextTokenNo++);
//     enqueue_patient(p);

//     p.age = 29;
//     strcpy(p.name, "Rekha Singh");
//     strcpy(p.symptoms, "Fever");
//     p.dept = OPD;
//     p.priority = 0;
//     p.arrival = time(NULL) - 200;
//     gen_token(p.dept, p.token, queues[p.dept].nextTokenNo++);
//     enqueue_patient(p);

//     p.age = 70;
//     strcpy(p.name, "Mr. Rao");
//     strcpy(p.symptoms, "Breathing difficulty");
//     p.dept = EMERGENCY_DEPT;
//     p.priority = 2;
//     p.arrival = time(NULL) - 100;
//     gen_token(p.dept, p.token, queues[p.dept].nextTokenNo++);
//     enqueue_patient(p);

//     p.age = 22;
//     strcpy(p.name, "Sana Bhat");
//     strcpy(p.symptoms, "Dizziness");
//     p.dept = OPD;
//     p.priority = 0;
//     p.arrival = time(NULL) - 80;
//     gen_token(p.dept, p.token, queues[p.dept].nextTokenNo++);
//     enqueue_patient(p);
// }