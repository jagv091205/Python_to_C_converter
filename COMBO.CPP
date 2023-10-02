#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

// Token types
typedef enum {
    // Arithmetic operators
    TOKEN_INTEGER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_LPAREN,
    TOKEN_RPAREN,

    // Comparison operators
    TOKEN_EQUAL,
    TOKEN_NOT_EQUAL,
    TOKEN_LESS,
    TOKEN_LESS_EQUAL,
    TOKEN_GREATER,
    TOKEN_GREATER_EQUAL,

    // Boolean operators
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_NOT,

    // Assignment operator
    TOKEN_ASSIGN,

    // Keywords
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_WHILE,

    // Identifiers and variables
    TOKEN_IDENTIFIER,

    // Colon and semicolon
    TOKEN_COLON,
    TOKEN_SEMICOLON,

    TOKEN_EOF,
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    char* value;
} Token;

// Function to initialize a token
Token* createToken(TokenType type, char* value) {
    Token* token = (Token*)malloc(sizeof(Token));
    if (token == NULL) {
        printf("Error: Memory allocation failed.\n");
        exit(1);
    }
    token->type = type;
    token->value = strdup(value);
    if (token->value == NULL) {
        printf("Error: Memory allocation failed.\n");
        exit(1);
    }
    return token;
}

// Lexer function
Token* lex(char* input) {
    int i = 0;
    Token* tokens = (Token*)malloc(100 * sizeof(Token)); // Assume a maximum of 100 tokens
    if (tokens == NULL) {
        printf("Error: Memory allocation failed.\n");
        exit(1);
    }
    int tokenCount = 0;

    while (input[i] != '\0') {
        if (isdigit(input[i])) {
            // Tokenize integers
            int j = i;
            while (isdigit(input[j])) {
                j++;
            }

            char* value = (char*)malloc((j - i + 1) * sizeof(char));
            if (value == NULL) {
                printf("Error: Memory allocation failed.\n");
                exit(1);
            }
            strncpy(value, &input[i], j - i);
            value[j - i] = '\0';

            tokens[tokenCount++] = *createToken(TOKEN_INTEGER, value);
            i = j;
        } else if (input[i] == '+') {
            tokens[tokenCount++] = *createToken(TOKEN_PLUS, "+");
            i++;
        } else if (input[i] == '-') {
            tokens[tokenCount++] = *createToken(TOKEN_MINUS, "-");
            i++;
        } else if (input[i] == '*') {
            tokens[tokenCount++] = *createToken(TOKEN_MULTIPLY, "*");
            i++;
        } else if (input[i] == '/') {
            tokens[tokenCount++] = *createToken(TOKEN_DIVIDE, "/");
            i++;
        } else if (input[i] == '(') {
            tokens[tokenCount++] = *createToken(TOKEN_LPAREN, "(");
            i++;
        } else if (input[i] == ')') {
            tokens[tokenCount++] = *createToken(TOKEN_RPAREN, ")");
            i++;
        } else if (input[i] == ' ' || input[i] == '\t' || input[i] == '\n') {
            // Ignore whitespace
            i++;
        } else if (input[i] == '&') {
            if (input[i + 1] == '&') {
                tokens[tokenCount++] = *createToken(TOKEN_AND, "&&");
                i += 2;
            } else {
                printf("Error: Unexpected character '%c'\n", input[i]);
                free(tokens);
                exit(1);
            }
        } else if (input[i] == '|') {
            if (input[i + 1] == '|') {
                tokens[tokenCount++] = *createToken(TOKEN_OR, "||");
                i += 2;
            } else {
                printf("Error: Unexpected character '%c'\n", input[i]);
                free(tokens);
                exit(1);
            }
        } else if (input[i] == '!') {
            tokens[tokenCount++] = *createToken(TOKEN_NOT, "!");
            i++;
        } else if (input[i] == '=') {
            if (input[i + 1] == '=') {
                tokens[tokenCount++] = *createToken(TOKEN_EQUAL, "==");
                i += 2;
            } else {
                tokens[tokenCount++] = *createToken(TOKEN_ASSIGN, "=");
                i++;
            }
        } else if (input[i] == '!') {
            if (input[i + 1] == '=') {
                tokens[tokenCount++] = *createToken(TOKEN_NOT_EQUAL, "!=");
                i += 2;
            } else {
                printf("Error: Unexpected character '%c'\n", input[i]);
                free(tokens);
                exit(1);
            }
        } else if (input[i] == '<') {
            if (input[i + 1] == '=') {
                tokens[tokenCount++] = *createToken(TOKEN_LESS_EQUAL, "<=");
                i += 2;
            } else {
                tokens[tokenCount++] = *createToken(TOKEN_LESS, "<");
                i++;
            }
        } else if (input[i] == '>') {
            if (input[i + 1] == '=') {
                tokens[tokenCount++] = *createToken(TOKEN_GREATER_EQUAL, ">=");
                i += 2;
            } else {
                tokens[tokenCount++] = *createToken(TOKEN_GREATER, ">");
                i++;
            }
        } else if (input[i] == ';') {
            tokens[tokenCount++] = *createToken(TOKEN_SEMICOLON, ";");
            i++;
        } else if (input[i] == ':') {
            tokens[tokenCount++] = *createToken(TOKEN_COLON, ":");
            i++;
        } else if (isalpha(input[i])) {
            // Tokenize identifiers and keywords
            int j = i;
            while (isalnum(input[j]) || input[j] == '_') {
                j++;
            }

            char* value = (char*)malloc((j - i + 1) * sizeof(char));
            if (value == NULL) {
                printf("Error: Memory allocation failed.\n");
                exit(1);
            }
            strncpy(value, &input[i], j - i);
            value[j - i] = '\0';

            if (strcmp(value, "if") == 0) {
                tokens[tokenCount++] = *createToken(TOKEN_IF, "if");
            } else if (strcmp(value, "else") == 0) {
                tokens[tokenCount++] = *createToken(TOKEN_ELSE, "else");
            } else if (strcmp(value, "while") == 0) {
                tokens[tokenCount++] = *createToken(TOKEN_WHILE, "while");
            } else if (strcmp(value, "true") == 0) {
                tokens[tokenCount++] = *createToken(TOKEN_TRUE, "true");
            } else if (strcmp(value, "false") == 0) {
                tokens[tokenCount++] = *createToken(TOKEN_FALSE, "false");
            } else {
                tokens[tokenCount++] = *createToken(TOKEN_IDENTIFIER, value);
            }

            i = j;
        } else {
            // Handle unexpected characters
            printf("Error: Unexpected character '%c'\n", input[i]);
            free(tokens);
            exit(1);
        }
    }

    // Add an EOF token
    tokens[tokenCount++] = *createToken(TOKEN_EOF, "");

    return tokens;
}

// AST Node structure
typedef struct ASTNode {
    TokenType type;
    char* value;
    struct ASTNode* left;
    struct ASTNode* right;
} ASTNode;

// Function to initialize an AST node
ASTNode* createASTNode(TokenType type, char* value, ASTNode* left, ASTNode* right) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    if (node == NULL) {
        printf("Error: Memory allocation failed.\n");
        exit(1);
    }
    node->type = type;
    node->value = strdup(value);
    if (node->value == NULL) {
        printf("Error: Memory allocation failed.\n");
        exit(1);
    }
    node->left = left;
    node->right = right;
    return node;
}

// Function to print the AST (in-order traversal)
void printAST(ASTNode* node) {
    if (node == NULL) {
        return;
    }

    // Recursively print the left subtree
    printAST(node->left);

    // Print the current node
    printf("Type: %d, Value: %s\n", node->type, node->value);

    // Recursively print the right subtree
    printAST(node->right);
}

// Helper function to parse expressions
ASTNode* parseExpression(Token* tokens, int* currentToken);

// Helper function to parse factors
ASTNode* parseFactor(Token* tokens, int* currentToken) {
    Token* token = &tokens[*currentToken];
    (*currentToken)++;

    if (token->type == TOKEN_INTEGER) {
        return createASTNode(TOKEN_INTEGER, token->value, NULL, NULL);
    } else if (token->type == TOKEN_LPAREN) {
        ASTNode* expression = parseExpression(tokens, currentToken);
        if (tokens[*currentToken].type == TOKEN_RPAREN) {
            (*currentToken)++;
            return expression;
        } else {
            printf("Error: Expected ')' after '('.\n");
            free(tokens);
            exit(1);
        }
    } else if (token->type == TOKEN_TRUE || token->type == TOKEN_FALSE) {
        return createASTNode(token->type, token->value, NULL, NULL);
    } else if (token->type == TOKEN_IDENTIFIER) {
        return createASTNode(TOKEN_IDENTIFIER, token->value, NULL, NULL);
    } else {
        printf("Error: Unexpected token '%s'\n", token->value);
        free(tokens);
        exit(1);
    }

    // Add a default return statement or handle the error case differently if needed
    return NULL; // For example, return NULL in case of an error
}

// Helper function to parse multiplicative expressions
ASTNode* parseMultiplicativeExpression(Token* tokens, int* currentToken) {
    ASTNode* left = parseFactor(tokens, currentToken);
    while (tokens[*currentToken].type == TOKEN_MULTIPLY || tokens[*currentToken].type == TOKEN_DIVIDE) {
        Token* op = &tokens[*currentToken];
        (*currentToken)++;
        ASTNode* right = parseFactor(tokens, currentToken);
        left = createASTNode(op->type, op->value, left, right);
    }
    return left;
}

// Helper function to parse additive expressions
ASTNode* parseAdditiveExpression(Token* tokens, int* currentToken) {
    ASTNode* left = parseMultiplicativeExpression(tokens, currentToken);
    while (tokens[*currentToken].type == TOKEN_PLUS || tokens[*currentToken].type == TOKEN_MINUS) {
        Token* op = &tokens[*currentToken];
        (*currentToken)++;
        ASTNode* right = parseMultiplicativeExpression(tokens, currentToken);
        left = createASTNode(op->type, op->value, left, right);
    }
    return left;
}

// Helper function to parse relational expressions
ASTNode* parseRelationalExpression(Token* tokens, int* currentToken) {
    ASTNode* left = parseAdditiveExpression(tokens, currentToken);
    while (tokens[*currentToken].type == TOKEN_LESS || tokens[*currentToken].type == TOKEN_LESS_EQUAL ||
           tokens[*currentToken].type == TOKEN_GREATER || tokens[*currentToken].type == TOKEN_GREATER_EQUAL) {
        Token* op = &tokens[*currentToken];
        (*currentToken)++;
        ASTNode* right = parseAdditiveExpression(tokens, currentToken);
        left = createASTNode(op->type, op->value, left, right);
    }
    return left;
}

// Helper function to parse equality expressions
ASTNode* parseEqualityExpression(Token* tokens, int* currentToken) {
    ASTNode* left = parseRelationalExpression(tokens, currentToken);
    while (tokens[*currentToken].type == TOKEN_EQUAL || tokens[*currentToken].type == TOKEN_NOT_EQUAL) {
        Token* op = &tokens[*currentToken];
        (*currentToken)++;
        ASTNode* right = parseRelationalExpression(tokens, currentToken);
        left = createASTNode(op->type, op->value, left, right);
    }
    return left;
}

// Helper function to parse logical AND expressions
ASTNode* parseLogicalAndExpression(Token* tokens, int* currentToken) {
    ASTNode* left = parseEqualityExpression(tokens, currentToken);
    while (tokens[*currentToken].type == TOKEN_AND) {
        Token* op = &tokens[*currentToken];
        (*currentToken)++;
        ASTNode* right = parseEqualityExpression(tokens, currentToken);
        left = createASTNode(op->type, op->value, left, right);
    }
    return left;
}

// Helper function to parse logical OR expressions
ASTNode* parseLogicalOrExpression(Token* tokens, int* currentToken) {
    ASTNode* left = parseLogicalAndExpression(tokens, currentToken);
    while (tokens[*currentToken].type == TOKEN_OR) {
        Token* op = &tokens[*currentToken];
        (*currentToken)++;
        ASTNode* right = parseLogicalAndExpression(tokens, currentToken);
        left = createASTNode(op->type, op->value, left, right);
    }
    return left;
}

// Function to parse expressions
ASTNode* parseExpression(Token* tokens, int* currentToken) {
    return parseLogicalOrExpression(tokens, currentToken);
}

// Function to evaluate expressions
bool evaluateExpression(ASTNode* node) {
    if (node->type == TOKEN_INTEGER) {
        return atoi(node->value) != 0;
    } else if (node->type == TOKEN_TRUE) {
        return true;
    } else if (node->type == TOKEN_FALSE) {
        return false;
    } else if (node->type == TOKEN_PLUS) {
        return evaluateExpression(node->left) + evaluateExpression(node->right);
    } else if (node->type == TOKEN_MINUS) {
        return evaluateExpression(node->left) - evaluateExpression(node->right);
    } else if (node->type == TOKEN_MULTIPLY) {
        return evaluateExpression(node->left) * evaluateExpression(node->right);
    } else if (node->type == TOKEN_DIVIDE) {
        return evaluateExpression(node->left) / evaluateExpression(node->right);
    } else if (node->type == TOKEN_AND) {
        return evaluateExpression(node->left) && evaluateExpression(node->right);
    } else if (node->type == TOKEN_OR) {
        return evaluateExpression(node->left) || evaluateExpression(node->right);
    } else if (node->type == TOKEN_NOT) {
        return !evaluateExpression(node->left);
    } else if (node->type == TOKEN_EQUAL) {
        return evaluateExpression(node->left) == evaluateExpression(node->right);
    } else if (node->type == TOKEN_NOT_EQUAL) {
        return evaluateExpression(node->left) != evaluateExpression(node->right);
    } else if (node->type == TOKEN_LESS) {
        return evaluateExpression(node->left) < evaluateExpression(node->right);
    } else if (node->type == TOKEN_LESS_EQUAL) {
        return evaluateExpression(node->left) <= evaluateExpression(node->right);
    } else if (node->type == TOKEN_GREATER) {
        return evaluateExpression(node->left) > evaluateExpression(node->right);
    } else if (node->type == TOKEN_GREATER_EQUAL) {
        return evaluateExpression(node->left) >= evaluateExpression(node->right);
    } else {
        printf("Error: Invalid node type for evaluation.\n");
        exit(1);
    }
}

// Function to free memory used by the AST
void freeAST(ASTNode* node) {
    if (node == NULL) {
        return;
    }
    freeAST(node->left);
    freeAST(node->right);
    free(node->value);
    free(node);
}

int main() {
    char input[1000]; // Increased buffer size for multiline input

    printf("Enter an expression (press Ctrl+D to end input):\n");

    // Read multiline input until Ctrl+D (EOF) is encountered
    while (fgets(input, sizeof(input), stdin) != NULL) {
        Token* tokens = lex(input);

        int currentToken = 0;
        ASTNode* root = parseExpression(tokens, &currentToken);

        // Print the AST (for demonstration purposes)
        printf("Abstract Syntax Tree:\n");
        printAST(root);

        // Evaluate the expression
        bool result = evaluateExpression(root);
        printf("Result: %s\n", result ? "true" : "false");

        // Free memory
        for (int i = 0; tokens[i].type != TOKEN_EOF; i++) {
            free(tokens[i].value);
        }
        free(tokens);

        // Free the memory used by the AST
        freeAST(root);
    }

    return 0;
}
