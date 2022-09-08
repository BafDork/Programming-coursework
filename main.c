#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum {
    MAX_LEN = 128,
    MAX_SES = 7
};

char separator = ';';
char file_name[] = "data.csv";

/* define student head structure */
struct LIST_STUD_HEAD {
    int Count;
    struct LIST_STUD_ELEM *First;
    struct LIST_STUD_ELEM *Last;
};
/* define structure type */
typedef struct LIST_STUD_HEAD LH_STUD;

/* define student element structure */
struct LIST_STUD_ELEM {
    int Id;
    char *Name;
    char *University;
    int Year_of_birth;
    int Year_of_admission;
    float Average_mark;
    float Price_of_studying;
    int Semester[MAX_SES];
    struct LIST_STUD_ELEM *Next;
};
/* define structured type */
typedef struct LIST_STUD_ELEM LE_STUD;

/* function for reading data from a file */
void read_file(LH_STUD *head);

/* function for writing data to a file */
void write_file(LH_STUD *head);

/* function for output of the manual for use */
void print_info();

/* function to search for a list item by id */
LE_STUD *find_elem_id(LH_STUD *head);

/* function for adding an item to a list */
void add_elem(LH_STUD *head);

/* function for changing an item in the list */
void edit_elem(LH_STUD *head);

/* function for deleting a list item */
void delete_elem(LH_STUD *head);

/* function to print the head of the list */
void print_header();

/* function to print data of a list item */
void print_elem(LE_STUD *elem);

/* function to print list data */
void print_list(LH_STUD *head);

/* function for searching by university value */
int search_university_value(LE_STUD *elem, char *str);

/* function for searching by the value of the year of birth */
int search_birth_year_value(LE_STUD *elem, char *str);

/* function for searching list items */
void search_elem(LH_STUD *head);

/* function for sorting by name value */
int sort_name_value(LE_STUD *elem1, LE_STUD *elem2);

/* function for sorting by the value of the average mark */
int sort_average_mark_value(LE_STUD *elem1, LE_STUD *elem2);

/* function for swapping values of list items */
void swap_elem(LE_STUD *elem1, LE_STUD *elem2);

/* function for sorting the list */
void sort_list(LH_STUD *head);

/* function to create head of the list */
LH_STUD *create_head();

/* function to create and add an element to a list */
void create_elem(LH_STUD *head, char **str);

/* function to clear memory from list */
void clear_list(LH_STUD *head);

/* function to split string to array by separator */
char **simple_split(char *str, int length, char sep);

/* function to clear array of string */
void clear_string_array(char **str, int n);

/* function for reading a string */
char *read_string(FILE *stream);

int main() {
    LH_STUD *head = NULL;
    int command, flag = 1;
    char *str;

    puts("Separator to use - ;");
    print_info();
    head = create_head();
    read_file(head);

    while (flag) {
        puts("Enter the command:");
        str = read_string(stdin);
        command = atoi(str);

        switch (command) {
            case 0:
                print_info();
                break;
            case 1:
                add_elem(head);
                break;
            case 2:
                edit_elem(head);
                break;
            case 3:
                delete_elem(head);
                break;
            case 4:
                print_list(head);
                break;
            case 5:
                search_elem(head);
                break;
            case 6:
                sort_list(head);
                break;
            case 7:
                write_file(head);
                clear_list(head);
                flag = 0;
                break;
            default:
                puts("Incorrect input");
        }
    }

    return 0;
}

/* ----------------------------------------- */
void read_file(LH_STUD *head) {
    char **split_str = NULL;
    char str[MAX_LEN];
    int count, filled, str_len, i;
    FILE *file;

    file = fopen(file_name, "r");
    if (file != NULL) {
        count = 0;
        while (fgets(str, MAX_LEN, file) != NULL) count++;
        rewind(file);

        for (i = 0, filled = 0; i < count; i++, filled++) {
            fgets(str, MAX_LEN, file);
            str_len = strlen(str);
            str[str_len - 1] = '\0';

            split_str = simple_split(str, str_len, separator);
            if (split_str != NULL) create_elem(head, split_str);
            else {
                i = count;
                puts("Error out of memory! (read_file : simple_split)");
            }
        }
        fclose(file);
    } else {
        puts("Open file Error! (read_file : fopen)");
    }
}

/* ----------------------------------------- */
void write_file(LH_STUD *head) {
    LE_STUD *elem = NULL;
    int i;
    FILE *file;

    file = fopen(file_name, "w");
    if (file != NULL) {
        elem = head->First;
        while (elem) {
            fprintf(file, "%s;", elem->Name);
            fprintf(file, "%s;", elem->University);
            fprintf(file, "%d;", elem->Year_of_birth);
            fprintf(file, "%d;", elem->Year_of_admission);
            fprintf(file, "%.2f;", elem->Average_mark);
            fprintf(file, "%.2f;", elem->Price_of_studying);

            for (i = 0; i < MAX_SES; i++) {
                if (elem->Semester[i]) {
                    fprintf(file, "%d ", elem->Semester[i]);
                }
            }
            fprintf(file, "\n");
            elem = elem->Next;
        }
        fclose(file);
    } else {
        puts("Open file Error! (write_file : fopen)");
    }
}

/* ----------------------------------------- */
void print_info() {
    puts("0: Information\n"
         "1: Adding cards\n"
         "2: Editing cards\n"
         "3: Deleting cards\n"
         "4: Print card file\n"
         "5: Search for cards\n"
         "6: Sorting the card file\n"
         "7: Exit");
}

/* ----------------------------------------- */
LE_STUD *find_elem_id(LH_STUD *head) {
    LE_STUD *elem = NULL;
    int id, find = 0;
    char *str;

    str = read_string(stdin);
    id = atoi(str);

    elem = head->First;
    while (elem && !find) {
        if (elem->Id == id) {
            find = 1;
        } else {
            elem = elem->Next;
        }
    }
    return elem;
}

/* ----------------------------------------- */
void add_elem(LH_STUD *head) {
    char **split_str = NULL;
    char *str;
    int str_len;

    puts("Enter additional elements. Press \"\\q\" for terminate the input:");
    while (strncmp(str, "\\q", 2) != 0) {
        printf("%s", "Please enter the following element: ");
        str = read_string(stdin);
        str_len = strlen(str);

        if (strncmp(str, "\\q", 2) != 0) {
            split_str = simple_split(str, str_len, separator);
            if (split_str != NULL) {
                create_elem(head, split_str);
            } else {
                puts("Error out of memory! (add_elem : simple_split)");
            }
        }
    }
    puts("The addition was completed successfully");

}

/* ----------------------------------------- */
void edit_elem(LH_STUD *head) {
    LE_STUD *elem = NULL;
    char **split_str = NULL;
    int str_len, count, i;
    char *str;

    puts("Enter the id of the card to edit");
    elem = find_elem_id(head);

    if (elem) {
        puts("Enter the new card details");
        str = read_string(stdin);
        str_len = strlen(str);
        split_str = simple_split(str, str_len, separator);
        if (split_str != NULL) {
            elem->Name = split_str[0];
            elem->University = split_str[1];
            elem->Year_of_birth = atoi(split_str[2]);
            elem->Year_of_admission = atoi(split_str[3]);
            elem->Average_mark = atof(split_str[4]);
            elem->Price_of_studying = atof(split_str[5]);

            str_len = strlen(split_str[6]);
            for (i = 0; i < MAX_SES; i++) {
                elem->Semester[i] = 0;
            }
            for (i = 0, count = 0; i < str_len && count < MAX_SES; i++) {
                if (isdigit(split_str[6][i])) {
                    elem->Semester[count++] = split_str[6][i] - '0';
                }
            }
            puts("Editing completed successfully");
        } else {
            puts("Error out of memory! (edit_card : simple_split)");
        }
    } else {
        puts("There is no element with this id");
    }

}

/* ----------------------------------------- */
void delete_elem(LH_STUD *head) {
    LE_STUD *previous = NULL, *elem = NULL;
    int id, find = 0;
    char *str;

    puts("Enter the id of the card to delete");
    str = read_string(stdin);
    id = atoi(str);

    elem = head->First;
    while (elem && !find) {
        if (elem->Id == id) {
            find = 1;
        } else {
            previous = elem;
            elem = elem->Next;
        }
    }

    if (elem) {
        if (previous) {
            previous->Next = elem->Next;
            if (!elem->Next) {
                head->Last = previous;
            }
        } else {
            head->First = elem->Next;
        }
        free(elem->Name);
        free(elem->University);
        free(elem);
        puts("Deletion completed successfully");
    } else {
        puts("There is no element with this id");
    }

}

/* ----------------------------------------- */
void print_header() {
    printf("|%-2s|%-17s|%10s|%13s|%17s|%12s|%17s|%-15s|\n",
           "Id", "Name", "University", "Year_of_birth", "Year_of_admission", "Average_mark",
           "Price_of_studying", "Semester");
    printf("+--+-----------------+----------+-------------+-----------------+");
    printf("------------+-----------------+---------------+\n");
}

/* ----------------------------------------- */
void print_elem(LE_STUD *elem) {
    int i;

    printf("|%-2d|%-17s|%-10s|%-13d|%-17d|%-12.2f|%-17.2f| ",
           elem->Id, elem->Name, elem->University, elem->Year_of_birth,
           elem->Year_of_admission, elem->Average_mark, elem->Price_of_studying);
    for (i = 0; i < MAX_SES; i++) {
        if (elem->Semester[i] == 0) {
            printf("  ");
        } else {
            printf("%d ", elem->Semester[i]);
        }
    }
    printf("|\n");
}

/* ----------------------------------------- */
void print_list(LH_STUD *head) {
    LE_STUD *elem = NULL;

    print_header();
    elem = head->First;
    while (elem) {
        print_elem(elem);
        elem = elem->Next;
    }
}

/* ----------------------------------------- */
int search_university_value(LE_STUD *elem, char *str) {
    if (strncmp(str, elem->University, 2) == 0) {
        return 1;
    } else {
        return 0;
    }
}

/* ----------------------------------------- */
int search_birth_year_value(LE_STUD *elem, char *str) {
    if (elem->Year_of_birth == atoi(str)) {
        return 1;
    } else {
        return 0;
    }
}

/* ----------------------------------------- */
void search_elem(LH_STUD *head) {
    LE_STUD *elem = NULL;
    char *str;
    int (*function)(LE_STUD *, char *);
    int command, flag = 1;

    puts("Select the search parameter:\n"
         "1: University\n"
         "2: Year of birth");
    str = read_string(stdin);
    command = atoi(str);

    switch (command) {
        case 1:
            puts("Enter the name of the university to search for cards");
            function = search_university_value;
            break;
        case 2:
            puts("Enter the year of birth to search for cards");
            function = search_birth_year_value;
            break;
        default:
            flag = 0;
            puts("Incorrect input");
    }

    if (flag) {
        str = read_string(stdin);

        elem = head->First;
        while (elem) {
            if (function(elem, str)) {
                if (flag) {
                    print_header();
                    flag = 0;
                }
                print_elem(elem);

            }
            elem = elem->Next;
        }
        if (flag) {
            puts("There is no element with this parameter");
        }
    }
}

/* ----------------------------------------- */
int sort_name_value(LE_STUD *elem1, LE_STUD *elem2) {
    if (strcmp(elem1->Name, elem2->Name) < 0) {
        return 1;
    } else {
        return 0;
    }
}

/* ----------------------------------------- */
int sort_average_mark_value(LE_STUD *elem1, LE_STUD *elem2) {
    if (elem2->Average_mark < elem1->Average_mark) {
        return 1;
    } else {
        return 0;
    }
}

/* ----------------------------------------- */
void swap_elem(LE_STUD *elem1, LE_STUD *elem2) {
    int i;

    elem1->Id = elem2->Id;
    elem1->Name = elem2->Name;
    elem1->University = elem2->University;
    elem1->Year_of_birth = elem2->Year_of_birth;
    elem1->Year_of_admission = elem2->Year_of_admission;
    elem1->Average_mark = elem2->Average_mark;
    elem1->Price_of_studying = elem2->Price_of_studying;
    for (i = 0; i < MAX_SES; i++) {
        elem1->Semester[i] = elem2->Semester[i];
    }
}

/* ----------------------------------------- */
void sort_list(LH_STUD *head) {
    LE_STUD *temp = NULL, *elem1 = NULL, *elem2 = NULL;
    char *str;
    int (*function)(LE_STUD *, LE_STUD *);
    int command, flag = 1;

    puts("Select the search parameter:\n"
         "1: Name\n"
         "2: Average Mark");
    str = read_string(stdin);
    command = atoi(str);

    switch (command) {
        case 1:
            function = sort_name_value;
            break;
        case 2:
            function = sort_average_mark_value;
            break;
        default:
            flag = 0;
            puts("Incorrect input");
    }

    if (flag) {
        elem1 = head->First;
        elem2 = head->First;
        temp = (LE_STUD *) malloc(sizeof(LE_STUD));
        if (temp != NULL) {
            while (elem1) {
                while (elem2) {
                    if (elem1 != elem2 && function(elem1, elem2)) {
                        swap_elem(temp, elem1);
                        swap_elem(elem1, elem2);
                        swap_elem(elem2, temp);
                    }
                    elem2 = elem2->Next;
                }
                elem2 = head->First;
                elem1 = elem1->Next;
            }
            puts("Sorting was completed successfully");
        } else {
            puts("Error out of memory! (sort_list : malloc)");
        }
    }
}

/* ----------------------------------------- */
LH_STUD *create_head() {
    LH_STUD *head = NULL;

    head = (LH_STUD *) malloc(sizeof(LH_STUD));
    if (head) {
        head->Count = 0;
        head->First = NULL;
        head->Last = NULL;
    } else {
        puts("Error out of memory! (create_head : malloc)");
    }
    return head;
}

/* ----------------------------------------- */
void create_elem(LH_STUD *head, char **str) {
    LE_STUD *elem = NULL;
    int str_len, count, i;

    elem = (LE_STUD *) malloc(sizeof(LE_STUD));
    if (elem != NULL) {
        head->Count += 1;
        if (head->First) {
            head->Last->Next = elem;
        } else {
            head->First = elem;
        }
        head->Last = elem;

        elem->Id = head->Count;
        elem->Name = str[0];
        elem->University = str[1];
        elem->Year_of_birth = atoi(str[2]);
        elem->Year_of_admission = atoi(str[3]);
        elem->Average_mark = atof(str[4]);
        elem->Price_of_studying = atof(str[5]);
        elem->Next = NULL;

        str_len = strlen(str[6]);
        for (i = 0; i < MAX_SES; i++) {
            elem->Semester[i] = 0;
        }
        for (i = 0, count = 0; i < str_len && count < MAX_SES; i++) {
            if (isdigit(str[6][i])) elem->Semester[count++] = str[6][i] - '0';;
        }
    } else {
        puts("Error out of memory! (create_elem : malloc)");
    }
}

/* ----------------------------------------- */
void clear_list(LH_STUD *head) {
    LE_STUD *elem = NULL, *following = NULL;

    elem = head->First;
    while (elem) {
        following = elem->Next;
        free(elem->Name);
        free(elem->University);
        free(elem);
        elem = following;
    }
    free(head);
}

/* ----------------------------------------- */
char **simple_split(char *str, int length, char sep) {
    char **str_array = NULL;
    int i, j, k, m;
    int key, count;
    for (j = 0, m = 0; j < length; j++) {
        if (str[j] == sep) m++;
    }

    key = 0;
    str_array = (char **) malloc((m + 1) * sizeof(char *));
    if (str_array != NULL) {
        for (i = 0, count = 0; i <= m; i++, count++) {
            str_array[i] = (char *) malloc(length * sizeof(char));
            if (str_array[i] != NULL) key = 1;
            else {
                key = 0;
                i = m;
            }
        }
        if (key) {
            k = 0;
            m = 0;
            for (j = 0; j < length; j++) {
                if (str[j] != sep) str_array[m][j - k] = str[j];
                else {
                    str_array[m][j - k] = '\0';
                    k = j + 1;
                    m++;
                }
            }
        } else {
            clear_string_array(str_array, count);
        }
    }
    return str_array;
}

/* ----------------------------------------- */
void clear_string_array(char **str, int n) {
    int i;

    for (i = 0; i < n; i++) {
        free(str[i]);
        str[i] = NULL;
    }
    free(str);
    str = NULL;
}

/* ----------------------------------------- */
char *read_string(FILE *stream) {
    char *str = NULL;
    int str_len;

    str = (char *) malloc(MAX_LEN * sizeof(char));
    if (str != NULL) {
        fgets(str, MAX_LEN, stream);
        str_len = strlen(str);
        str[str_len - 1] = '\0';
    } else {
        puts("Error out of memory! (read_string : malloc)");
    }
    return str;
}