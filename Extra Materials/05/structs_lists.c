#include <stdlib.h>
#include <stdio.h>

struct student {
    int s_id;
    int year; ß
    struct student *next;
};

void PrintStudent(struct student *s) {
    if (s != NULL)
        printf("Student ID: %d, Student's year: %d\n", s->s_id, s->year);
}

/* Task 1: Complete CreateStudent
 * CreateStudent dynamically allocates memory to store a student (using malloc)
 * and initializes its members. It returns a pointer to the new student
 */
struct student* CreateStudent(int s_id, int year) {
    struct student *p;

    // step one: allocate space to hold a new struct student that
    //           p can point to
    p = malloc(sizeof(struct student));

    // step two: initialize the s_id and year members of p
    //           using the arguments provided to the function
    p->s_id = s_id;
    p->year = year;

    // step three: initialize the next pointer of p to NULL
    p->next = NULL;

    // step four: Instead of returning NULL, return your new student
    return p;
}

/* Task 2: Complete BuildThree
 * BuildThree creates three students using the student IDs and years
 * provided as arguments and links them together as shown in the picture
 * in the handout
 */
struct student* BuildThree(int sid_1, int sid_2, int sid_3, int year_1, int year_2, int year_3) {
    struct student *first = CreateStudent(sid_1, year_1);
    struct student *second = CreateStudent(sid_2, year_2);
    struct student *third = CreateStudent(sid_3, year_3);

    // Step 1: Connect the first student to the second
    first->next = second;

    // Step 2: Connect the second student to the third
    second->next = third;

    // Step 3: Instead of returning NULL return a pointer to
    //         the first student in the list
    return first;
}


/*  Task 3: Complete Push
 *  Push creates a new student and makes it the front of the list
 *  it returns a pointer to the new student which is now the head of the list
 */
struct student* Push(int s_id, int year, struct student *head) {
    // Step 1: Create a new student using your CreateStudent function
    struct student *new_student = CreateStudent(s_id, year);

    // Step 2: Add it to the front of the list
    new_student->next = head;
    head = new_student;

    // Step 3: Replace the return statement below to return a pointer
    //         to your new student which is now the head of the list
    return head;
}


/* PrintList takes a pointer to the first student in a linked list of students;
 * it walks through the list of students by following the pointers in the structs
 * and prints out each student in the list
 */
void PrintList(struct student *head) {
    struct student* current = head;

    if (head == NULL) {
        printf("The list is empty\n");
        return;
    }

    while (current != NULL) {
        printf("Student ID: %d, Student's year: %d\n", current->s_id, current->year);
        current = current->next;
    }
}

/* Task 4:  Complete Append
 * Append creates a new student and adds it at the end of the list
 * that means the pointer of the student who is currently the last
 * in the list needs to point to the new student
 */
void Append(int s_id, int year, struct student *head) {
    // Step 1: Create a new student using your CreateStudent function
    struct student *new_student = CreateStudent(s_id, year);

    // Step 2: Iterate through the list and when you reach the last node,
    //         append your newly created node to the last node.
    //         Look at PrintList for ideas for how to iterate through the list
    //         You may assume that the list is not empty,
    //         i.e. it has at least one node in it
    struct student *current = head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = new_student;
}

void Remove(int s_id, struct student *head) {

    // step 1: check if the linked list is empty
    if (head == NULL) {
        printf("Linked list is empty.\n");
        return;
    }

    // step 2: iterate through the list to find the student node
    struct student *current = head;
    struct student *previous = current;

    while (current != NULL && current->s_id != s_id) {
        previous = current;
        current = current->next;
    }
    // find the node, if the id does not exist in the linked list, return
    if (current == NULL) {
        printf("Student (id: %d) does not exist.\n", s_id);
        return;
    }
    // then we perform deletion
    struct student *delete_student = current;
    previous->next = current->next;
    free(delete_student);
}

int main(void) {
    struct student *my_first_student;
    struct student *my_student_list;

    // Create one student and print it
    printf(">>>> Create one student. <<<<\n");
    my_first_student = CreateStudent(1, 3);
    PrintStudent(my_first_student);
    printf("\n");

    // Create three students and link them together in a list, print the list
    printf(">>>> Create three students. <<<<\n");
    my_student_list = BuildThree(1, 2, 3, 1, 2, 3);
    PrintList(my_student_list);
    printf("\n");

    // Push creates a new student and adds it at the beginning of the list we created with BuildThree
    printf(">>>> Add to the head. <<<<\n");
    my_student_list = Push(4, 4, my_student_list);
    PrintList(my_student_list);
    printf("\n");

    // Append creates a new student and adds it at the end of the list we created
    printf(">>>> Add to the tail. <<<<\n");
    Append(5, 5, my_student_list);
    PrintList(my_student_list);
    printf("\n");

    // Remove student with id 4;
    printf(">>>> Remove existing student. <<<<\n");
    Remove(4, my_student_list);
    PrintList(my_student_list);
    printf("\n");
    Remove(2, my_student_list);
    PrintList(my_student_list);
    printf("\n");

    // Remove non-existing student
    printf(">>>> Remove non-existing student. <<<<\n");
    Remove(999, my_student_list);
    PrintList(my_student_list);
    printf("\n");

    return 0;
}
