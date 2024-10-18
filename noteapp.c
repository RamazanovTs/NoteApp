#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Process Variable
int process = 1;

void commands(char *command, char **text, char *filepath);
void saveFile(char **text, char *filepath);
void changeLine(char **text);
int printText(const char *path, char **text);
char *fileName(const char *path);
char **readFile(char *filepath);
int getTextSize(char **text);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filepath>\n", argv[0]);
        return 1;
    }

    char *filepath = argv[1];
    char command[100];
    char **text = readFile(filepath);

    while (process) {
        system("cls");
        if (text == NULL) {
            fprintf(stderr, "Failed to read file.\n");
            break;
        }
        printText(filepath, text);

        printf("[Command] ");
        scanf("%99s", command);
        commands(command, text, filepath);
    }

    // Free allocated memory for text
    for (int i = 0; text[i] != NULL; i++) {
        free(text[i]);
    }
    free(text);

    return 0;
}

int printText(const char *path, char **text) {
    char *filename = fileName(path);
    printf("\n_______________[%s]______________\n", filename);
    free(filename);

    for (int i = 0; text[i] != NULL; i++) {
        printf("%d| %s", i + 1, text[i]);
    }

    printf("\n__________________________________\n");
    return 1;
}

char *fileName(const char *path) {
    int last_folder = -1;
    int length = strlen(path);

    for (int i = 0; i < length; i++) {
        if (path[i] == '\\' || path[i] == '/') {
            last_folder = i;
        }
    }
    const char *filename_start = (last_folder == -1) ? path : path + last_folder + 1;
    char *filename = strdup(filename_start);
    return filename;
}

void commands(char *command, char **text, char *filepath) {
    if (strcmp(command, "exit") == 0) {
        printf("Exiting...\n");
        process = 0;
    } else if (strcmp(command, "replace") == 0) {
        changeLine(text);
    } else if (strcmp(command, "save") == 0) {
        saveFile(text, filepath);
    }
}

int fileLineNumber(char *filepath) {
    int ch, linenum = 0;
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            linenum++;
        }
    }
    fclose(file);
    return linenum + 1;
}

char **readFile(char *filepath) {
    int lines = fileLineNumber(filepath);
    if (lines < 1) return NULL;

    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    char **textDoc = malloc(lines * sizeof(char *));
    if (textDoc == NULL) {
        perror("Error allocating memory");
        fclose(file);
        return NULL;
    }

    size_t index = 0;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), file) != NULL && index < lines) {
        textDoc[index] = malloc(strlen(buffer) + 1);
        if (textDoc[index] == NULL) {
            perror("Error allocating memory for line");
            for (size_t i = 0; i < index; i++) {
                free(textDoc[i]);
            }
            free(textDoc);
            fclose(file);
            return NULL;
        }
        strcpy(textDoc[index], buffer);
        index++;
    }

    textDoc[index] = NULL;
    fclose(file);
    return textDoc;
}

void changeLine(char **text) {
    int line;
    char buffer[256];
    printf("[Line to change] ");
    scanf("%d", &line);
    getchar();

    if (line < 1 || line > getTextSize(text)) {
        printf("Invalid line number!\n");
        return;
    }

    int index = line - 1;

    printf("[Enter text] ");
    fgets(buffer, sizeof(buffer), stdin);

    free(text[index]);
    text[index] = malloc(strlen(buffer) + 1);
    if (text[index] == NULL) {
        perror("Error allocating memory for new line");
        return;
    }
    strcpy(text[index], buffer);
}

void saveFile(char **text, char *filepath) {
    FILE *file = fopen(filepath, "w");
    if (file == NULL) {
        printf("Unable to open file\n");
        return;
    }

    for (int index = 0; text[index] != NULL; index++) {
        fprintf(file, "%s", text[index]);
    }
    fclose(file);
}

int getTextSize(char **text) {
    int len = 0;
    while (text[len] != NULL) {
        len++;
    }
    return len;
}
