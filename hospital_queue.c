
void show_queues_brief() {
    printf("\n--- Live Queues (next 5 per department) ---\n");
    for (int d = 0; d < DEPT_COUNT; ++d) {
        printf("\n[%s] (avg %d min/patient)\n", DEPT_NAMES[d], avgTimePerPatient[d]);
        int limit = queues[d].size < 5 ? queues[d].size : 5;
        if (limit == 0) { printf("  (no patients waiting)\n"); continue; }
        for (int i = 0; i < limit; ++i) {
            Patient *p = &queues[d].items[i];
            printf("  %s | %-16s | pri:%d | wait ~ %d min\n",
                   p->token, p->name, p->priority, estimate_wait_minutes(d, i));
        }
    }
}

void staff_call_next() {
    printf("\nSelect department to call next: 1) OPD 2) Cardio 3) Emergency 4) General\nChoice: ");
    int choice;
    if (scanf("%d", &choice) != 1) { getchar(); return; }
    getchar();
    if (choice < 1 || choice > 4) { printf("Invalid.\n"); return; }
    Dept d = (Dept)(choice - 1);
    if (queues[d].size == 0) { printf("No patients in %s queue.\n", DEPT_NAMES[d]); return; }

    Patient next = queues[d].items[0];
    for (int i = 1; i < queues[d].size; ++i) queues[d].items[i-1] = queues[d].items[i];
    queues[d].size--;

    char ts[32];
    struct tm *tm = localtime(&next.arrival);
    strftime(ts, sizeof(ts), "%Y-%m-%d %H:%M:%S", tm);
    printf("\nCalling next patient: %s | %s | Age %d | Arrived: %s\nSymptoms: %s\n",
           next.token, next.name, next.age, ts, next.symptoms);
    printf("Priority: %s\n", next.priority == 2 ? "EMERGENCY" : (next.priority == 1 ? "URGENT" : "NORMAL"));
    printf("Action: (1) Mark Finished   (2) Escalate to Emergency   (3) Reassign dept   (Any other = put back)\nChoice: ");
    int act;
    if (scanf("%d", &act) != 1) { getchar(); act = 0; }
    getchar();
    if (act == 1) {
        printf("Marked finished for %s.\n", next.token);
    } else if (act == 2) {
        next.priority = 2;
        next.dept = EMERGENCY_DEPT;
        gen_token(next.dept, next.token, queues[next.dept].nextTokenNo++);
        enqueue_patient(next);
        printf("Escalated and requeued as %s in Emergency.\n", next.token);
    } else if (act == 3) {
        printf("Choose new dept: 1) OPD 2) Cardio 3) Emergency 4) General\nChoice: ");
        int nd;
        if (scanf("%d", &nd) != 1) nd = 1;
        getchar();
        if (nd < 1 || nd > 4) nd = 1;
        next.dept = (Dept)(nd - 1);
        gen_token(next.dept, next.token, queues[next.dept].nextTokenNo++);
        enqueue_patient(next);
        printf("Reassigned as %s in %s.\n", next.token, DEPT_NAMES[next.dept]);
    } else {
        for (int i = queues[d].size; i > 0; --i) queues[d].items[i] = queues[d].items[i-1];
        queues[d].items[0] = next; queues[d].size++;
        printf("Patient put back to front of %s queue.\n", DEPT_NAMES[d]);
    }
}

void staff_mark_done() {
    char tok[8];
    printf("Enter token to mark done (e.g., O001): ");
    if (scanf("%7s", tok) != 1) { getchar(); return; }
    getchar();
    Dept d; int idx;
    if (find_token(tok, &d, &idx) == 0) {
        for (int i = idx+1; i < queues[d].size; ++i) queues[d].items[i-1] = queues[d].items[i];
        queues[d].size--;
        printf("Token %s removed (marked done).\n", tok);
    } else {
        printf("Token not found.\n");
    }
}

void staff_reprioritize() {
    char tok[8];
    printf("Enter token to reprioritize: ");
    if (scanf("%7s", tok) != 1) { getchar(); return; }
    getchar();
    Dept d; int idx;
    if (find_token(tok, &d, &idx) != 0) { printf("Not found.\n"); return; }
    Patient p = queues[d].items[idx];
    for (int i = idx+1; i < queues[d].size; ++i) queues[d].items[i-1] = queues[d].items[i];
    queues[d].size--;
    printf("Current priority %d. Set new priority: 0 Normal, 1 Urgent, 2 Emergency: ", p.priority);
    int np;
    if (scanf("%d", &np) != 1) np = p.priority;
    getchar();
    if (np < 0) np = 0; if (np > 2) np = 2;
    p.priority = np;
    if (p.priority == 2) {
        p.dept = EMERGENCY_DEPT;
        gen_token(p.dept, p.token, queues[p.dept].nextTokenNo++);
    }
    enqueue_patient(p);
    printf("Reprioritized and requeued as %s (dept %s, pri %d).\n", p.token, DEPT_NAMES[p.dept], p.priority);
}

void staff_view_full_queue() {
    printf("\nSelect department to view: 1) OPD 2) Cardio 3) Emergency 4) General\nChoice: ");
    int choice;
    if (scanf("%d", &choice) != 1) { getchar(); return; }
    getchar();
    if (choice < 1 || choice > 4) { printf("Invalid.\n"); return; }
    Dept d = (Dept)(choice - 1);
    printf("\n--- %s Queue (total %d) ---\n", DEPT_NAMES[d], queues[d].size);
    for (int i = 0; i < queues[d].size; ++i) {
        Patient *p = &queues[d].items[i];
        char ts[32];
        struct tm *tm = localtime(&p->arrival);
        strftime(ts, sizeof(ts), "%Y-%m-%d %H:%M:%S", tm);
        printf("%2d. %s | %s | Age:%d | pri:%d | wait ~ %d min | arr:%s\n",
               i+1, p->token, p->name, p->age, p->priority, estimate_wait_minutes(d, i), ts);
    }
}

/* main menu and program entry */
void main_menu() {
    while (1) {
        printf("\n====== Intelligent Patient Queue (Console) ======\n");
        printf("1) Register Patient (Quick)\n");
        printf("2) View Queues (brief)\n");
        printf("3) Staff Dashboard\n");
        printf("4) Seed demo patients\n");
        printf("5) Exit\n");
        printf("Choose: ");
        int ch;
        if (scanf("%d", &ch) != 1) { getchar(); continue; }
        getchar();
        if (ch == 1) register_patient();
        else if (ch == 2) show_queues_brief();
        else if (ch == 3) {
            while (1) {
                printf("\n--- Staff Dashboard ---\n");
                printf("1) View full queue\n");
                printf("2) Call Next\n");
                printf("3) Mark patient done (by token)\n");
                printf("4) Re-prioritize patient (by token)\n");
                printf("5) Back to main\n");
                printf("Choose: ");
                int s;
                if (scanf("%d", &s) != 1) { getchar(); break; }
                getchar();
                if (s == 1) staff_view_full_queue();
                else if (s == 2) staff_call_next();
                else if (s == 3) staff_mark_done();
                else if (s == 4) staff_reprioritize();
                else if (s == 5) break;
                else printf("Invalid.\n");
            }
        } else if (ch == 4) seed_demo_patients();
        else if (ch == 5) { printf("Goodbye.\n"); break; }
        else printf("Invalid.\n");
    }
}

int main() {
    init_queues();
    printf("Intelligent Patient Queue Prototype (Console)\n");
    main_menu();
    return 0;
}
/* End Team C section */
