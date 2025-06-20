#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mymat.h"

#define MAX_LINE_LENGTH 1024
#define MAX_TOKENS 32
#define MAX_VALUES 16

/* Matrix instances */
mat MAT_A, MAT_B, MAT_C, MAT_D, MAT_E, MAT_F;

/* Matrix lookup table */
typedef struct {
    char *name;
    mat *matrix;
} MatrixEntry;

MatrixEntry matrices[] = {
    {"MAT_A", &MAT_A},
    {"MAT_B", &MAT_B}, 
    {"MAT_C", &MAT_C},
    {"MAT_D", &MAT_D},
    {"MAT_E", &MAT_E},
    {"MAT_F", &MAT_F}
};

#define NUM_MATRICES (sizeof(matrices) / sizeof(matrices[0]))

/* Command types */
typedef enum {
    CMD_READ_MAT,
    CMD_PRINT_MAT,
    CMD_ADD_MAT,
    CMD_SUB_MAT,
    CMD_MUL_MAT,
    CMD_MUL_SCALAR,
    CMD_TRANS_MAT,
    CMD_STOP,
    CMD_INVALID
} CommandType;

/* Command structure */
typedef struct {
    char *name;
    CommandType type;
    int min_args;
    int max_args;
    int requires_scalar;  /* 1 if command requires scalar argument */
} Command;

Command commands[] = {
    {"read_mat", CMD_READ_MAT, 1, 17, 0},      /* matrix + up to 16 values */
    {"print_mat", CMD_PRINT_MAT, 1, 1, 0},     /* matrix only */
    {"add_mat", CMD_ADD_MAT, 3, 3, 0},         /* matrix + matrix + matrix */
    {"sub_mat", CMD_SUB_MAT, 3, 3, 0},         /* matrix + matrix + matrix */
    {"mul_mat", CMD_MUL_MAT, 3, 3, 0},         /* matrix + matrix + matrix */
    {"mul_scalar", CMD_MUL_SCALAR, 3, 3, 1},   /* matrix + scalar + matrix */
    {"trans_mat", CMD_TRANS_MAT, 2, 2, 0},     /* matrix + matrix */
    {"stop", CMD_STOP, 0, 0, 0}                /* no arguments */
};

#define NUM_COMMANDS (sizeof(commands) / sizeof(commands[0]))

/* Function prototypes */
void initialize_matrices(void);
mat* get_matrix_by_name(const char *name);
CommandType get_command_type(const char *name);
int is_valid_number(const char *str);
double parse_number(const char *str);
int tokenize_line(char *line, char tokens[][MAX_LINE_LENGTH]);
int validate_tokens(char tokens[][MAX_LINE_LENGTH], int token_count);
int execute_command(char tokens[][MAX_LINE_LENGTH], int token_count);
void trim_whitespace(char *str);
int has_illegal_comma(const char *line);
int has_missing_comma(const char *line);
int has_consecutive_commas(const char *line);
int has_extraneous_text(const char *line);

/* Initialize all matrices to zero */
void initialize_matrices(void) {
    int i, j;
    for (i = 0; i < NUM_MATRICES; i++) {
        for (j = 0; j < 4; j++) {
            int k;
            for (k = 0; k < 4; k++) {
                matrices[i].matrix->data[j][k] = 0.0;
            }
        }
    }
}

/* Get matrix pointer by name */
mat* get_matrix_by_name(const char *name) {
    int i;
    for (i = 0; i < NUM_MATRICES; i++) {
        if (strcmp(matrices[i].name, name) == 0) {
            return matrices[i].matrix;
        }
    }
    return NULL;
}

/* Get command type by name */
CommandType get_command_type(const char *name) {
    int i;
    for (i = 0; i < NUM_COMMANDS; i++) {
        if (strcmp(commands[i].name, name) == 0) {
            return commands[i].type;
        }
    }
    return CMD_INVALID;
}

/* Check if string is a valid number */
int is_valid_number(const char *str) {
    char *endptr;
    if (str == NULL || *str == '\0') {
        return 0;
    }
    
    /* Skip leading whitespace */
    while (isspace(*str)) str++;
    
    /* Check if empty after trimming */
    if (*str == '\0') {
        return 0;
    }
    
    strtod(str, &endptr);
    
    /* Skip trailing whitespace */
    while (isspace(*endptr)) endptr++;
    
    return (*endptr == '\0');
}

/* Parse string to number */
double parse_number(const char *str) {
    return strtod(str, NULL);
}

/* Trim leading and trailing whitespace */
void trim_whitespace(char *str) {
    char *start = str;
    char *end;
    
    /* Trim leading space */
    while (isspace(*start)) start++;
    
    /* All spaces? */
    if (*start == '\0') {
        *str = '\0';
        return;
    }
    
    /* Trim trailing space */
    end = start + strlen(start) - 1;
    while (end > start && isspace(*end)) end--;
    
    /* Write new null terminator */
    end[1] = '\0';
    
    /* Move trimmed string to beginning */
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }
}

/* Check for illegal comma at end */
int has_illegal_comma(const char *line) {
    int len = strlen(line);
    int i;
    
    /* Check from end backwards, skipping whitespace */
    for (i = len - 1; i >= 0; i--) {
        if (isspace(line[i])) {
            continue;
        }
        if (line[i] == ',') {
            return 1;
        }
        break;
    }
    return 0;
}

/* Check for missing comma between arguments */
int has_missing_comma(const char *line) {
    /* Simplified approach: look for pattern "word space word" without comma between */
    char *ptr = (char*)line;
    int in_word = 0;
    int word_count = 0;
    int last_was_space = 0;
    
    /* Skip command name first */
    while (*ptr && !isspace(*ptr)) ptr++;
    while (*ptr && isspace(*ptr)) ptr++;
    
    /* Now check arguments for missing commas */
    while (*ptr) {
        if (isspace(*ptr)) {
            if (in_word) {
                in_word = 0;
                word_count++;
                last_was_space = 1;
            } else if (!last_was_space) {
                last_was_space = 1;
            }
        } else if (*ptr == ',') {
            if (in_word) {
                word_count++;
                in_word = 0;
            }
            last_was_space = 0;
            /* Skip spaces after comma */
            ptr++;
            while (*ptr && isspace(*ptr)) ptr++;
            continue;
        } else {
            /* Regular character */
            if (!in_word) {
                if (last_was_space && word_count > 0) {
                    /* We found a word after space without comma - this is error */
                    return 1;
                }
                in_word = 1;
            }
            last_was_space = 0;
        }
        ptr++;
    }
    
    return 0;
}

/* Check for consecutive commas */
int has_consecutive_commas(const char *line) {
    int i;
    int comma_found = 0;
    
    for (i = 0; line[i] != '\0'; i++) {
        if (line[i] == ',') {
            if (comma_found) {
                return 1;  /* Found consecutive commas */
            }
            comma_found = 1;
        } else if (!isspace(line[i])) {
            comma_found = 0;
        }
    }
    return 0;
}

/* Tokenize input line, handling commas and whitespace */
int tokenize_line(char *line, char tokens[][MAX_LINE_LENGTH]) {
    int token_count = 0;
    char *ptr = line;
    char current_token[MAX_LINE_LENGTH];
    int token_pos = 0;
    
    /* Skip leading whitespace */
    while (isspace(*ptr)) ptr++;
    
    while (*ptr != '\0' && token_count < MAX_TOKENS) {
        if (*ptr == ',') {
            /* End current token if we have one */
            if (token_pos > 0) {
                current_token[token_pos] = '\0';
                trim_whitespace(current_token);
                if (strlen(current_token) > 0) {
                    strcpy(tokens[token_count], current_token);
                    token_count++;
                }
                token_pos = 0;
            }
            ptr++;
            /* Skip whitespace after comma */
            while (isspace(*ptr)) ptr++;
        } else if (isspace(*ptr)) {
            /* End token on whitespace if we're at the first token (command) */
            if (token_count == 0 && token_pos > 0) {
                current_token[token_pos] = '\0';
                trim_whitespace(current_token);
                strcpy(tokens[token_count], current_token);
                token_count++;
                token_pos = 0;
                /* Skip whitespace */
                while (isspace(*ptr)) ptr++;
            } else if (token_pos > 0) {
                /* Add space to token */
                current_token[token_pos++] = *ptr;
                ptr++;
            } else {
                /* Skip leading whitespace within token */
                ptr++;
            }
        } else {
            /* Regular character */
            current_token[token_pos++] = *ptr;
            ptr++;
        }
    }
    
    /* Handle last token */
    if (token_pos > 0) {
        current_token[token_pos] = '\0';
        trim_whitespace(current_token);
        if (strlen(current_token) > 0) {
            strcpy(tokens[token_count], current_token);
            token_count++;
        }
    }
    
    return token_count;
}

/* Validate parsed tokens according to command requirements */
int validate_tokens(char tokens[][MAX_LINE_LENGTH], int token_count) {
    CommandType cmd_type;
    Command *cmd = NULL;
    int i;
    
    if (token_count == 0) {
        return 1; /* Empty line is valid */
    }
    
    /* Check if command exists */
    cmd_type = get_command_type(tokens[0]);
    if (cmd_type == CMD_INVALID) {
        printf("Undefined command name\n");
        return 0;
    }
    
    /* Find command structure */
    for (i = 0; i < NUM_COMMANDS; i++) {
        if (commands[i].type == cmd_type) {
            cmd = &commands[i];
            break;
        }
    }
    
    /* Check argument count */
    if (token_count - 1 < cmd->min_args) {
        printf("Missing argument\n");
        return 0;
    }
    
    if (token_count - 1 > cmd->max_args) {
        printf("Extraneous text after end of command\n"); 
        return 0;
    }
    
    /* Validate arguments based on command type */
    switch (cmd_type) {
        case CMD_READ_MAT:
            /* First arg must be matrix */
            if (get_matrix_by_name(tokens[1]) == NULL) {
                printf("Undefined matrix name\n");
                return 0;
            }
            /* Remaining args must be numbers */
            for (i = 2; i < token_count; i++) {
                if (!is_valid_number(tokens[i])) {
                    printf("Argument is not a real number\n");
                    return 0;
                }
            }
            break;
            
        case CMD_PRINT_MAT:
            /* Single matrix argument */
            if (get_matrix_by_name(tokens[1]) == NULL) {
                printf("Undefined matrix name\n");
                return 0;
            }
            break;
            
        case CMD_ADD_MAT:
        case CMD_SUB_MAT:
        case CMD_MUL_MAT:
            /* Three matrix arguments */
            for (i = 1; i < 4; i++) {
                if (get_matrix_by_name(tokens[i]) == NULL) {
                    printf("Undefined matrix name\n");
                    return 0;
                }
            }
            break;
            
        case CMD_MUL_SCALAR:
            /* Matrix, scalar, matrix */
            if (get_matrix_by_name(tokens[1]) == NULL) {
                printf("Undefined matrix name\n");
                return 0;
            }
            if (!is_valid_number(tokens[2])) {
                printf("Argument is not a scalar\n");
                return 0;
            }
            if (get_matrix_by_name(tokens[3]) == NULL) {
                printf("Undefined matrix name\n");
                return 0;
            }
            break;
            
        case CMD_TRANS_MAT:
            /* Two matrix arguments */
            for (i = 1; i < 3; i++) {
                if (get_matrix_by_name(tokens[i]) == NULL) {
                    printf("Undefined matrix name\n");
                    return 0;
                }
            }
            break;
            
        case CMD_STOP:
            /* No arguments needed */
            break;
            
        default:
            printf("Undefined command name\n");
            return 0;
    }
    
    return 1;
}

/* Execute validated command */
int execute_command(char tokens[][MAX_LINE_LENGTH], int token_count) {
    CommandType cmd_type;
    mat *m1, *m2, *m3;
    double values[MAX_VALUES];
    double scalar;
    int i, value_count;
    
    if (token_count == 0) {
        return 1; /* Empty line */
    }
    
    cmd_type = get_command_type(tokens[0]);
    
    switch (cmd_type) {
        case CMD_READ_MAT:
            m1 = get_matrix_by_name(tokens[1]);
            value_count = token_count - 2;
            
            /* Parse values */
            for (i = 0; i < value_count && i < MAX_VALUES; i++) {
                values[i] = parse_number(tokens[i + 2]);
            }
            
            read_mat(m1, values, value_count);
            break;
            
        case CMD_PRINT_MAT:
            m1 = get_matrix_by_name(tokens[1]);
            print_mat(m1);
            break;
            
        case CMD_ADD_MAT:
            m1 = get_matrix_by_name(tokens[1]);
            m2 = get_matrix_by_name(tokens[2]);
            m3 = get_matrix_by_name(tokens[3]);
            add_mat(m1, m2, m3);
            break;
            
        case CMD_SUB_MAT:
            m1 = get_matrix_by_name(tokens[1]);
            m2 = get_matrix_by_name(tokens[2]);
            m3 = get_matrix_by_name(tokens[3]);
            sub_mat(m1, m2, m3);
            break;
            
        case CMD_MUL_MAT:
            m1 = get_matrix_by_name(tokens[1]);
            m2 = get_matrix_by_name(tokens[2]);
            m3 = get_matrix_by_name(tokens[3]);
            mul_mat(m1, m2, m3);
            break;
            
        case CMD_MUL_SCALAR:
            m1 = get_matrix_by_name(tokens[1]);
            scalar = parse_number(tokens[2]);
            m3 = get_matrix_by_name(tokens[3]);
            mul_scalar(m1, scalar, m3);
            break;
            
        case CMD_TRANS_MAT:
            m1 = get_matrix_by_name(tokens[1]);
            m2 = get_matrix_by_name(tokens[2]);
            trans_mat(m1, m2);
            break;
            
        case CMD_STOP:
            return 0; /* Signal to stop */
            
        default:
            printf("Undefined command name\n");
            return 1;
    }
    
    return 1;
}

/* Main program loop */
int main(void) {
    char line[MAX_LINE_LENGTH];
    char original_line[MAX_LINE_LENGTH];
    char tokens[MAX_TOKENS][MAX_LINE_LENGTH];
    int token_count;
    int continue_program = 1;
    
    /* Initialize matrices */
    initialize_matrices();
    
    printf("Matrix Calculator\n");
    printf("Available commands: read_mat, print_mat, add_mat, sub_mat, mul_mat, mul_scalar, trans_mat, stop\n");
    printf("Available matrices: MAT_A, MAT_B, MAT_C, MAT_D, MAT_E, MAT_F\n\n");
    
    while (continue_program) {
        printf("Enter command: ");
        
        if (fgets(line, sizeof(line), stdin) == NULL) {
            /* EOF encountered */
            break;
        }
        
        /* Remove newline */
        line[strcspn(line, "\n")] = '\0';
        
        /* Save original line for error checking */
        strcpy(original_line, line);
        
        /* Skip empty lines */
        trim_whitespace(line);
        if (strlen(line) == 0) {
            continue;
        }
        
        /* Check for specific error patterns before tokenization */
        if (has_illegal_comma(original_line)) {
            printf("Illegal comma\n");
            continue;
        }
        
        if (has_consecutive_commas(original_line)) {
            printf("Multiple consecutive commas\n");
            continue;
        }
        
        /* Temporarily disable missing comma check - too aggressive
        if (has_missing_comma(original_line)) {
            printf("Missing comma\n");
            continue;
        }
        */
        
        /* Tokenize the line */
        token_count = tokenize_line(line, tokens);
        
        /* Validate tokens */
        if (!validate_tokens(tokens, token_count)) {
            continue;
        }
        
        /* Execute command */
        continue_program = execute_command(tokens, token_count);
    }
    
    printf("Program terminated.\n");
    return 0;
}