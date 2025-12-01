// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>
// #include <time.h>

// #define MAXQ 200
// #define NAME_LEN 50
// #define SYM_LEN 80
// #define DEPT_COUNT 4

// typedef enum { OPD = 0, CARDIO, EMERGENCY_DEPT, GENERAL } Dept;
// const char *DEPT_NAMES[] = {"OPD", "Cardiology", "Emergency", "General"};
// const char DEPT_LETTER[] = {'O', 'C', 'E', 'G'};

// typedef struct {
//     char token[8];
//     char name[NAME_LEN];
//     int age;
//     Dept dept;
//     char symptoms[SYM_LEN];
//     int priority;
//     time_t arrival;
// } Patient;

// typedef struct {
//     Patient items[MAXQ];
//     int size;
//     int nextTokenNo;
// } Queue;

// /* shared globals */
// Queue queues[DEPT_COUNT];
// int avgTimePerPatient[DEPT_COUNT] = {6, 12, 8, 7};

// /* initialize queues */
// void init_queues() {
//     for (int i = 0; i < DEPT_COUNT; ++i) {
//         queues[i].size = 0;
//         queues[i].nextTokenNo = 1;
//     }
// }

// /* generate token into out (size >= 8) */
// void gen_token(Dept d, char *out, int next) {
//     snprintf(out, 8, "%c%03d", DEPT_LETTER[d], next);
// }

// /* enqueue by priority (2 high, 1 urgent, 0 normal).
//    Inserts patient into the queue at a position maintaining descending priority order. */
// int enqueue_patient(Patient p) {
//     Queue *q = &queues[p.dept];
//     if (q->size >= MAXQ) return -1;
//     int idx = q->size;
//     for (int i = 0; i < q->size; ++i) {
//         if (q->items[i].priority < p.priority) { idx = i; break; }
//     }
//     for (int j = q->size; j > idx; --j) q->items[j] = q->items[j-1];
//     q->items[idx] = p;
//     q->size++;
//     return 0;
// }

// /* find token across all departments */
// int find_token(const char *token, Dept *out_dept, int *out_index) {
//     for (int d = 0; d < DEPT_COUNT; ++d) {
//         for (int i = 0; i < queues[d].size; ++i) {
//             if (strcmp(queues[d].items[i].token, token) == 0) {
//                 if (out_dept) *out_dept = (Dept)d;
//                 if (out_index) *out_index = i;
//                 return 0;
//             }
//         }
//     }
//     return -1;
// }

// /* estimate wait in minutes given department and zero-based position index */
// int estimate_wait_minutes(Dept d, int positionIndex) {
//     if (positionIndex < 0) return 0;
//     return positionIndex * avgTimePerPatient[d];
// }