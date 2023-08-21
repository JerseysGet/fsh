#include "../header/headers.h"

void initialize() { 
    initPrompt();
    initHistory(); 
}

void destruct() {
    destructHistory();
}

#define MAX_COMMAND_PER_LINE 128
#define INPUT_BUFFER_SIZE 4096

int main() {
    initialize();
    while (1) {
        prompt();
        char input[INPUT_BUFFER_SIZE];
        fgets(input, INPUT_BUFFER_SIZE, stdin);
        int len = strlen(input);
        input[len - 1] = '\0';
        char rawInput[INPUT_BUFFER_SIZE];
        strcpy(rawInput, input);
#ifdef DEBUG
        printf("[MAIN_DEBUG] Input recieved: %s\n", input);
#endif

        Command buffer[MAX_COMMAND_PER_LINE];
        int commandCt = parseInput(buffer, MAX_COMMAND_PER_LINE, input);
        addEvent(buffer, commandCt, rawInput);
        for (int i = 0; i < commandCt; i++) executeCommand(&buffer[i]);
        for (int i = 0; i < commandCt; i++) freeCommand(&buffer[i]);
    }

    destruct();
}
