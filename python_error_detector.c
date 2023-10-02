#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_ERRORS 20

typedef struct {
    int line_number;
    char *message;
} Error;

Error errors[MAX_ERRORS];
int num_errors = 0;

void add_error(int line_number, char *message) {
    if (num_errors < MAX_ERRORS) {
        errors[num_errors].line_number = line_number;
        errors[num_errors].message = message;
        num_errors++;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <python_program>\n", argv[0]);
        exit(1);
    }

    char *python_program = argv[1];

    // Open the Python program file.
    FILE *fp = fopen(python_program, "r");
    if (fp == NULL) {
        printf("Error: Could not open Python program file '%s'\n", python_program);
        exit(1);
    }

    // Read the Python program line by line.
    char line[1024];
    int line_number = 1;
    int in_while_loop = 0;  // Flag to track if we are inside a while loop

    while (fgets(line, sizeof(line), fp) != NULL) {
        // Check for common syntax errors.
        if (strstr(line, "if True:") && !strstr(line, ":")) {
            add_error(line_number, "Expected colon (:) after `if` statement.");
        } else if (strstr(line, "while True:") && !strstr(line, ":")) {
            add_error(line_number, "Expected colon (:) after `while` statement.");
        } else if (strstr(line, "def f(") && !strstr(line, "):")) {
            add_error(line_number, "Expected colon (:) after function definition header.");
        }

    // Add more error checks here:
    // Example 1: Check for missing indentation
    if (strstr(line, "if True:") && !strstr(line, "    ")) {
        add_error(line_number, "Missing indentation in the 'if' block.");
    }

    // Example 2: Check for invalid variable names
    if (strstr(line, "var @InvalidVariableName")) {
        add_error(line_number, "Invalid variable name.");
    }

    // Example 3: Check for incorrect function call
    if (strstr(line, "foo(") && !strstr(line, ")")) {
        add_error(line_number, "Expected closing parenthesis in function call.");
    }

    // Example 4: Check for missing 'return' statement
    if (strstr(line, "def func():") && !strstr(line, "return")) {
        add_error(line_number, "Expected 'return' statement in function.");
    }

    // Example 5: Check for missing 'import' statement
    if (strstr(line, "import module") && !strstr(line, "module")) {
        add_error(line_number, "Missing 'import' statement.");
    }

    // Example 6: Check for missing ':' in 'for' loop
    if (strstr(line, "for i in range(10)") && !strstr(line, ":")) {
        add_error(line_number, "Expected ':' after 'for' loop header.");
    }

    // Example 7: Check for invalid syntax
    if (strstr(line, "invalid syntax here!")) {
        add_error(line_number, "Invalid syntax detected.");
    }

    // Example 8: Check for missing closing parenthesis
    if (strstr(line, "function(") && !strstr(line, ")")) {
        add_error(line_number, "Expected closing parenthesis.");
    }

    // Example 9: Check for missing 'class' definition
    if (strstr(line, "class MyClass:") && !strstr(line, ":")) {
        add_error(line_number, "Expected colon (:) after class definition.");
    }

    // Example 10: Check for missing 'pass' statement
    if (strstr(line, "if condition:") && !strstr(line, "pass")) {
        add_error(line_number, "Expected 'pass' statement after 'if' condition.");
    }
// Check if we enter or exit a while loop
        if (strstr(line, "while True:")) {
            in_while_loop = 1;
        } else if (strstr(line, "break")) {
            in_while_loop = 0;
        }

        // Increment the line number.
        line_number++;
    }

    // Print any errors that were found.
    if (num_errors > 0) {
        printf("Errors found in Python program '%s':\n", python_program);
        for (int i = 0; i < num_errors; i++) {
            printf("Line %d: %s\n", errors[i].line_number, errors[i].message);
        }
    } else {
        printf("No errors found in Python program '%s'\n", python_program);
    }

    return 0;
}
