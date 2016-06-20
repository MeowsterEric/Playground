#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lists.h"

/* Add a group with name group_name to the group_list referred to by
* group_list_ptr. The groups are ordered by the time that the group was
* added to the list with new groups added to the end of the list.
*
* Returns 0 on success and -1 if a group with this name already exists.
*
* (I.e, allocate and initialize a Group struct, and insert it
* into the group_list. Note that the head of the group list might change
* which is why the first argument is a double pointer.)
*/
int add_group(Group **group_list_ptr, const char *group_name) {
    // search the existing group list
    Group *found_group = find_group(*group_list_ptr, group_name);
    // if the group exists, then return -1.
    if (found_group != NULL) {
        return -1;
    }

    // if the group does not exist, then reserve the memory for the new Group
    // and the group name. also checking whether or not the malloc fails
    Group *new_group = malloc(sizeof(Group));
    if (new_group == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    new_group->name = malloc(strlen(group_name) + 1);
    if (new_group->name == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    // initialize this new group
    strcpy(new_group->name, group_name);
    new_group->users = NULL;
    new_group->xcts = NULL;
    new_group->next = NULL;

    // searching the group list, starting from the head of this group list
    // if the head is null, then assign the Group to head
    if (*group_list_ptr == NULL) {
        *group_list_ptr = new_group;
        return 0;
    }

    // if the group list is non-empty, then loop through this group list
    // to find the tail group
    Group *current = *group_list_ptr;
    Group *previous = current;
    while (current != NULL) {
        // update the prev and curr pointer
        previous = current;
        current = current->next;
    }
    // when current reaches the tail group, connect the new group to the end
    previous->next = new_group;

    return 0;
}

/* Print to standard output the names of all groups in group_list, one name
*  per line. Output is in the same order as group_list.
*/
void list_groups(Group *group_list) {
    // setup a pointer to loop through the linked group list
    Group *current_group = group_list;
    while (current_group != NULL) {
        // output the group name to stdout, update pointer to the next group
        printf("Group: %s\n", current_group->name);
        current_group = current_group->next;
    }
}

/* Search the list of groups for a group with matching group_name
* If group_name is not found, return NULL, otherwise return a pointer to the
* matching group list node.
*/
Group *find_group(Group *group_list, const char *group_name) {
    // set up a pointer to loop through the linked list
    Group *current = group_list;
    // if the current group pointer is not NULL and
    // the group name does not match, update pointer to the next group
    while (current != NULL && strcmp(current->name, group_name) != 0) {
        current = current->next;
    }
    // return the group pointer
    //    1. group not found or group list is empty, return NULL
    //    2. group name match, then return the pointer to the matching group
    return current;
}

/* Add a new user with the specified user name to the specified group.
* Return 0 on success and -1 if the group already has a user with that name.
* (allocate and initialize a User data structure and insert it into the
* appropriate group list)
*/
int add_user(Group *group, const char *user_name) {
    // search the user list to find the previous user
    User *prev_user = find_prev_user(group, user_name);

    // if the user list has only 1 user (prev_user is the 1st and only user)
    // and the name matches user_name, return -1
    if (prev_user != NULL && prev_user == group->users
            && strcmp(prev_user->name, user_name) == 0) {
        return -1;
    }
    // if the user list has multiple users, and the next user of this prev_user
    // has the name that matches user_name, then return -1
    else if (prev_user != NULL && prev_user->next != NULL
             && strcmp((prev_user->next)->name, user_name) == 0) {
        return -1;
    }

    // if none of them above, meaning user does not exist, then reserve memory
    // and check the memory is successfully reserved or not
    User *new_user = malloc(sizeof(User));
    if (new_user == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    new_user->name = malloc(strlen(user_name) + 1);
    if (new_user->name == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    // initialized this new user
    strcpy(new_user->name, user_name);
    new_user->balance = 0.0;
    new_user->next = NULL;

    // if the user list is empty, then this user is the 1st user in the user
    // linked list; or if the user list is not empty, then add to the list
    // Note that, the user list must be sorted ascending order
    if (group->users == NULL) {
        group->users = new_user;
    }
    // if the new user balance is less than the 1st user in the list, then
    // add the new user to the 1st place, update the head pointer
    else if (new_user->balance < (group->users)->balance) {
        new_user->next = group->users;
        group->users = new_user;
    }
    // if none of the cases above, then we insert the new user according to
    // its current balance, so that the user list is sorted in ascending order
    else {
        User *curr_user = group->users;
        prev_user = curr_user;
        // find the right position to insert the user
        while (curr_user != NULL && new_user->balance >= curr_user->balance) {
            prev_user = curr_user;
            curr_user = curr_user->next;
        }
        // insert the user
        new_user->next = curr_user;
        prev_user->next = new_user;
    }

    return 0;
}

/* Remove the user with matching user and group name and
* remove all her transactions from the transaction list.
* Return 0 on success, and -1 if no matching user exists.
* Remember to free memory no longer needed.
* (Wait on implementing the removal of the user's transactions until you
* get to Part III below, when you will implement transactions.)
*/
int remove_user(Group *group, const char *user_name) {
    User *prev_user = find_prev_user(group, user_name);
    // if the user list is empty, return -1
    if (prev_user == NULL) {
        return -1;
    }

    // if the user list has exactly 1 user (prev_user is the 1st & only user)
    // or to be removed user is the 1st user in the list, then we remove all
    // of its transactions and free up the transaction memory, update the user
    // pointer to the next one, and free up the user memory
    if (prev_user == group->users && strcmp(prev_user->name, user_name) == 0) {
        remove_xct(group, prev_user->name);
        group->users = prev_user->next;
        // free up user memory
        free(prev_user->name);
        free(prev_user);
    }
    // if not the case above, then remove the user and free up the memory
    else {
        User *removed_user = prev_user->next;
        prev_user->next = removed_user->next;
        // remove all transactions and free up memory
        remove_xct(group, removed_user->name);
        free(removed_user->name);
        free(removed_user);
    }
    return 0;
}

/* Print to standard output the names of all the users in group, one
* per line, and in the order that users are stored in the list, namely
* lowest payer first.
*/
void list_users(Group *group) {
    // get the head of the user list
    User *current = group->users;
    // loop through the user list, print the name and update the pointer
    while (current != NULL) {
        printf("User: %s, current balance: %.2f\n",
               current->name, current->balance);
        current = current->next;
    }
}

/* Print to standard output the balance of the specified user. Return 0
* on success, or -1 if the user with the given name is not in the group.
*/
int user_balance(Group *group, const char *user_name) {
    User *prev_user = find_prev_user(group, user_name);
    // if the user list is empty, return -1;
    if (prev_user == NULL) {
        return -1;
    }

    // if prev_user is the 1st user in the user list
    if (prev_user == group->users && strcmp(prev_user->name, user_name) == 0) {
        printf("User %s has balance: %.2f\n",
               prev_user->name, prev_user->balance);
    }
    // if there are multiple users exist, then print the next user's balance
    else {
        printf("User %s has balance: %.2f\n",
               (prev_user->next)->name, (prev_user->next)->balance);
    }
    return 0;
}

/* Print to standard output the name of the user who has paid the least
* If there are several users with equal least amounts, all names are output.
* Returns 0 on success, and -1 if the list of users is empty.
* (This should be easy, since your list is sorted by balance).
*/
int under_paid(Group *group) {
    User *current = group->users;
    // if the user list is empty, return -1
    if (current == NULL) {
        return -1;
    }
    // if not empty, then print the user with the least balance to stdout
    double least_balance;
    printf("User %s has least balance: %.2f\n",
           current->name, (least_balance = current->balance));
    // check if remaining users have the least balance, print all to stdout
    while (current->next != NULL
            && least_balance == (current->next)->balance) {
        current = current->next;
        printf("User %s has least balance: %.2f\n",
               current->name, least_balance);
    }
    return 0;
}

/* Return a pointer to the user prior to the one in group with user_name. If
* the matching user is the first in the list (i.e. there is no prior user in
* the list), return a pointer to the matching user itself. If no matching user
* exists, return NULL.
*
* The reason for returning the prior user is that returning the matching user
* itself does not allow us to change the user that occurs before the
* matching user, and some of the functions you will implement require that
* we be able to do this.
*/
User *find_prev_user(Group *group, const char *user_name) {
    // get the first user in the user list, use a previous pointer
    // to locate previous user.
    User *current = group->users;
    User *previous = current;

    // loop through the user list, if the user list is empty, return NULL;
    // if the user list is not empty, then loop through the list, compare the
    // user name and return:
    //    1. the previous user of this user_name user
    //    2. or the tail user if user does not exist in the user list
    while (current != NULL && strcmp(current->name, user_name) != 0) {
        previous = current;
        current = current->next;
    }
    return previous;
}

/* Add the transaction represented by user_name and amount to the appropriate
* transaction list, and update the balances of corresponding user and group.
* Note that updating a user's balance might require the user to be moved to a
* different position in the list to keep the list in sorted order. Returns 0 on
* success, and -1 if the specified user does not exist.
*/
int add_xct(Group *group, const char *user_name, double amount) {
    User *prev_user = find_prev_user(group, user_name);
    // if the user list is empty, return -1. or if the prev_user is the tail
    // user in the list and the name does not match, return -1 as well
    if (prev_user == NULL) {
        return -1;
    } else if (prev_user->next == NULL
               && strcmp(prev_user->name, user_name) != 0) {
        return -1;
    }

    // reserve memory for the new transaction, check if malloc failed
    Xct *new_xct = malloc(sizeof(Xct));
    if (new_xct == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    new_xct->name = malloc(strlen(user_name) + 1);
    if (new_xct->name == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }

    // initialize the new transaction, then add this new transaction to the
    // head of the transaction list
    strcpy(new_xct->name, user_name);
    new_xct->amount = amount;
    new_xct->next = group->xcts;
    group->xcts = new_xct;

    /*****************************************************************/
    /* get the curr_user from the prev_user, then update the balance */
    /*****************************************************************/
    // the curr_user whose name is user_name, since the case where user_name
    // not existed in the list has been eliminated, then we only need to
    // consider the case where user list contains exactly 1 user
    User *curr_user;
    (strcmp(prev_user->name, user_name) == 0) ?
    (curr_user = prev_user) : (curr_user = prev_user->next);
    curr_user->balance += amount;

    // move the user to the right place, the user is sorted in ascending order
    // and it is based on user's current balance
    // ensure that the curr_user is not the last user in the user list
    while (curr_user->next != NULL
            && curr_user->balance > (curr_user->next)->balance) {
        // consider the case where curr_user is the 1st user in the list
        // then we swap the 1st and 2nd user
        if (curr_user == group->users) {
            // update the curr_user pointer to the next one, then update
            // the head of the user linked list
            curr_user = curr_user->next;
            group->users = curr_user;
            // swap the users
            prev_user->next = curr_user->next;
            curr_user->next = prev_user;
        } else {
            User *temp = curr_user;
            // update the curr_user pointer to the next, then swap the users
            curr_user = curr_user->next;
            prev_user->next = curr_user;
            temp->next = curr_user->next;
            curr_user->next = temp;
        }
        // update the reference of prev_user and curr_user
        prev_user = curr_user;
        curr_user = curr_user->next;
    }
    return 0;
}

/* Print to standard output the num_xct most recent transactions for the
* specified group (or fewer transactions if there are less than num_xct
* transactions posted for this group). The output should have one line per
* transaction that prints the name and the amount of the transaction. If
* there are no transactions, this function will print nothing.
*/
void recent_xct(Group *group, long num_xct) {
    Xct *current = group->xcts;
    // if the transaction list is empty, return
    if (current == NULL) {
        return;
    }
    // if list is not empty, print the num_xct transactions to stdout
    long counter = 0;
    while (current != NULL && counter < num_xct) {
        printf("User %s with transaction amount: %.2f\n",
               current->name, current->amount);
        current = current->next;
        counter += 1;
    }
}

/* Remove all transactions that belong to the user_name from the group's
* transaction list. This helper function should be called by remove_user.
* If there are no transactions for this user, the function should do nothing.
* Remember to free memory no longer needed.
*/
void remove_xct(Group *group, const char *user_name) {
    Xct *current = group->xcts;
    // if the transaction list is empty, return
    if (current == NULL) {
        return;
    }

    Xct *removed_xct;
    // the case where transaction is the first transaction in the list
    while (current != NULL && strcmp(current->name, user_name) == 0) {
        removed_xct = current;
        current = current->next;
        group->xcts = current;
        // free up the memory
        free(removed_xct->name);
        free(removed_xct);
    }

    // check the remaining transactions in this list
    Xct *previous = current;
    while (current != NULL) {
        // if the name matches, update pointer & balance, then free up memory
        if (strcmp(current->name, user_name) == 0) {
            removed_xct = current;
            current = current->next;
            previous->next = current;
            // free up the memory
            free(removed_xct->name);
            free(removed_xct);
        }
        // if not, move to the next transaction
        else {
            previous = current;
            current = current->next;
        }
    }

}
