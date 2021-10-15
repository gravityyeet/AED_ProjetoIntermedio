/*
    Roap library
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "roap.h"

void checkAllocationError(const void* ptr, const char* errorMsg) {
    if ( ! ptr ) {
        printf("\n%s\n", errorMsg);
        exit(EXIT_FAILURE);
    }
}

LabList *criar_No_Lab (FILE *fp) {

    LabList *newnode;

    newnode = (LabList *) malloc(sizeof(LabList));
    if(newnode == NULL) {
        exit(EXIT_FAILURE);
    }

    newnode->lab = inputLab(fp);
    newnode->next = NULL;

    return newnode;
}

LabList *insert_in_list (LabList *head, LabList *innode) {

    LabList *auxA, *auxN;

    if (head == NULL) {
        head = innode;
    } else {
        auxA = head;
        auxN = head->next;
        while (auxN != NULL) {
            auxA = auxN;
            auxN = auxN->next;
        }
        auxA->next = innode;
    }
    return head;
}



Labirinto* inputLab(FILE* filePtr) {
    char buffer[BUFFERSIZE];
    int lines, cols, celLine, celCol;
    int parede1 = 0, parede2 = 0, parede_v = 0;

    int conversions = fscanf(filePtr, "%d %d %d %d",
        &lines, &cols, &celLine, &celCol);

    if( conversions != 4 ) {    // EOF
        printf("\n EOF with conversions=%d \n", conversions);
        return NULL;
    }

    Labirinto* m = calloc(1, sizeof(Labirinto) );
    checkAllocationError(m, "Erro ao alocar labirinto! m");

    m->L= lines;
    m->C = cols;
    m->cel_L = celLine;
    m->cel_C = celCol;

    printf("conversions:%d size: %d %d treasure: %d %d \n",
        conversions, m->L, m->C, m->cel_L, m->cel_C);

    conversions = fscanf(filePtr, "%s", buffer);

    printf("conversions:%d buffer=[%s] \n", conversions, buffer);

    if (buffer[0]=='A') {
        m->modo = buffer[1]-'0';
        if (m->modo == 6) {
            conversions = fscanf(filePtr, "%d %d", &m->cel_2_L, &m->cel_2_C);
            printf("conversions:%d same room? %d %d \n",
                                conversions, m->cel_2_L, m->cel_2_C);
        }
        conversions = fscanf(filePtr, "%d", &m->P);
    } else {
        m->modo = 0;
        buffer[BUFFERSIZE-1] = '\0';
        conversions = sscanf(buffer, "%d", &m->P);
    }

    printf("conversions: %d modo=%d P=%d \n",
                       conversions, m->modo, m->P);

    /* Alloc and read info from file to tabuleiro */
    alloc_tabuleiro(m);
    for (int t = 0; t < m->P; t++) {
        fscanf(filePtr, "%d %d %d", &parede1, &parede2, &parede_v);
        m->tabuleiro[parede1 - 1][parede2 - 1] = parede_v;
    }

    fscanf(filePtr, " ");
    return m;
}

/* Alloc e free do tabuleiro 2D sugerido */
void alloc_tabuleiro(Labirinto *lab) {
    lab->tabuleiro = (int **) malloc(sizeof(int *) * lab->L);
    if (lab->tabuleiro == NULL) {
        exit(EXIT_FAILURE);
    } 
    for (int alloc_c = 0; alloc_c < lab->L; alloc_c++) {
        lab->tabuleiro[alloc_c] = (int *) malloc(sizeof(int) * lab->C);
        if (lab->tabuleiro[alloc_c] == NULL) {
            exit(EXIT_FAILURE);
        }
    }
}

void free_tabuleiro(Labirinto *lab) {
    int linhas = lab->L;

    for (int t = 0; t < lab->L; t++) {
        free(lab->tabuleiro[t]);
    }
    free(lab->tabuleiro);
}

void free_lista(LabList *head) {
    LabList *aux;

    aux = head;

    while (head != NULL) {
        aux = head;
        head = head->next;

        free_tabuleiro(aux->lab);
        free(aux->lab);
        //free(lista->lab->tabuleiro);
        free(aux);  
    }
}



















