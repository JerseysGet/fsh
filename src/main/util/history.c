#include "../../header/headers.h"

Queue eventQueue;
FILE* historyFile;
char historyFilePath[DIRECTORY_BUFFER_SIZE];
void initHistory() {  // TODO change to struct with array of 15
    eventQueue = newQueue();
    strcpy(historyFilePath, homeDirectory);
    strcat(historyFilePath, "/.osn_shell_history");
    historyFile = fopen(historyFilePath, "rb");
    if (historyFile == NULL) {
        printf("[WARNING]: Unable to read history file\n");
    } else {
        size_t queueSize;
        fread(&queueSize, sizeof(size_t), 1,
              historyFile);  // TODO do something with read error
        for (int i = 0; i < queueSize; i++) {
            size_t stringSize;
            fread(&stringSize, sizeof(size_t), 1, historyFile);
            char* inputString = (char*)malloc(sizeof(char) * (stringSize + 1));
            fread(inputString, sizeof(char), stringSize + 1, historyFile);
            push(eventQueue, inputString);
        }
        fclose(historyFile);
    }

    while (eventQueue->size > 15) {
        pop(eventQueue);
    }
}

void writeHistory() {
#ifdef DEBUG
    printf("[WRITE_DEBUG] Attempting to write to %s\n", historyFilePath);
#endif

    historyFile = fopen(historyFilePath, "wb");
    if (historyFile == NULL) {
        fprintf(stderr, "[ERROR]: Unable to write history file\n");
    } else {
        size_t queueSize = eventQueue->size;
        Node itr = eventQueue->front;
        fwrite(&queueSize, sizeof(size_t), 1, historyFile);
        for (int i = 0; i < queueSize; i++, itr = itr->next) {
            size_t stringSize = strlen(itr->val);
            fwrite(&stringSize, sizeof(size_t), 1, historyFile);
            char* inputString = itr->val;
            fwrite(inputString, sizeof(char), stringSize + 1, historyFile);
        }
        fclose(historyFile);
    }
}

void destructHistory() {
    writeHistory();
    freeQueue(eventQueue);
}

bool addEvent(Command* commands, int commandCt, char* inputString) {
    char* newString = (char*)malloc(sizeof(char) * (strlen(inputString) + 1));
    strcpy(newString, inputString);
    // TODO check if commands contains pastevents
    if (eventQueue->size == 0) {
        push(eventQueue, newString);
    } else if (strcmp(front(eventQueue), newString) == 0) {
        free(newString);
        return false;
    } else {
        push(eventQueue, newString);
    }
    
    while (eventQueue->size > 15) pop(eventQueue);
    writeHistory();
    return true;
}

char* getKthLastEvent(int k) {
    if (eventQueue->size < k) {
        fprintf(stderr, "[ERROR]: eventHistory size is only %ld\n",
                eventQueue->size);
        return NULL;
    }

    Node itr = eventQueue->front;
    for (int i = 1; i < k; i++) {
        itr = itr->next;
    }

    return itr->val;
}

size_t getHistorySize() { return eventQueue->size; }