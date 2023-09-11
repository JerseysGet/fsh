#include "../header/headers.h"

void initialize() {
    initPrompt();
    initHistory();
}

void destruct() { 
    destructHistory(); 
    destructBackground();
}

#define INPUT_BUFFER_SIZE 2048
char longestCommand[DIRECTORY_BUFFER_SIZE] = "";
time_t longestTime = 0;

int main() {
    initialize();
    while (1) {
        printFinishedProcesses();
        prompt();
        char input[INPUT_BUFFER_SIZE];
        fgets(input, INPUT_BUFFER_SIZE, stdin);
        int len = strlen(input);
        input[len - 1] = '\0';
        char spacedInput[INPUT_BUFFER_SIZE];
        strcpy(spacedInput, input);
        addSpacesAround(spacedInput, "|", "", input);
        addSpacesAround(input, ">", ">>", spacedInput);
        addSpacesAround(spacedInput, "<", "", input);
        char rawInput[INPUT_BUFFER_SIZE];
        strcpy(rawInput, input);
#ifdef DEBUG
        printf("[MAIN_DEBUG] Input recieved: %s\n", input);
#endif
        Command buffer[MAX_COMMAND_PER_LINE];
        int commandCt = parseInput(buffer, MAX_COMMAND_PER_LINE, input);
        addEvent(buffer, commandCt, rawInput);
        strcpy(longestCommand, "");
        longestTime = 0;
        for (int i = 0; i < commandCt; i++) {
            time_t startTime = time(NULL);
            executeCommand(&buffer[i]);
            time_t endTime = time(NULL);
            time_t timeTaken = endTime - startTime;

            if (buffer[i].argc && timeTaken > longestTime) {
                longestTime = timeTaken;
                strcpy(longestCommand, buffer[i].argv[0]);
            }
        }
        for (int i = 0; i < commandCt; i++) freeCommand(&buffer[i]);
    }

    destruct();
}
