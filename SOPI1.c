#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEM_SIZE 4096

unsigned char MEMORY[MEM_SIZE];
int MEM_POS[MEM_SIZE] = {0};

typedef struct node {
  unsigned int key;
  char *data;
  struct node *next;
  struct node *prev;
} Node;

typedef struct list {
  Node *start;
  Node *end;
} List;

void *aloca(size_t size) {
  int i = 0;

  while (1) {
    while (i < MEM_SIZE && (i + size) < MEM_SIZE) {

      if (MEM_POS[i] == 0) {
        int count = 0;
        for (int j = 0 + i; j < size + i; j++) {
          if (MEM_POS[j] != 0) {
            count = 1;
          }
        }
        if (count == 0) {
          MEM_POS[i] = size;
          return &MEMORY[i];
        } else
          i++;
      } else {
        i += MEM_POS[i];
      }
    }
    return NULL;
  }
  return NULL;
}

void libera(void *ptr) {
  for (int i = 0; i < MEM_SIZE; i++) {
    if (&MEMORY[i] == ptr) {
      MEM_POS[i] = 0;
      return;
    }
  }
}

char *dupString(const char *data) {
  size_t size = strlen(data) + 1;
  char *newData = (char *)aloca(size);

  if (newData) {
    strcpy(newData, data);
  }

  return newData;
}

Node *createNode(unsigned int key, char *data) {
  Node *newNode = (Node *)aloca(sizeof(struct node));
  newNode->key = key;
  newNode->data = dupString(data);
  newNode->prev = NULL;
  newNode->next = NULL;
  return newNode;
}

List *createList() {
  List *newList = (List *)aloca(sizeof(struct list));

  newList->start = NULL;
  newList->end = NULL;

  return newList;
}

void insert(List *list, unsigned int key, char *data) {
  Node *newNode = createNode(key, data);

  if (list->start == NULL) {
    list->start = newNode;
    list->end = newNode;

    return;
  }

  list->end->next = newNode;
  newNode->prev = list->end;
  list->end = newNode;
}

void delete (List *list) {
  if (list->start != NULL) {
    Node *formerEnd = list->end;
    list->end = list->end->prev;
    list->end->next = NULL;

    libera(formerEnd->data);
    libera(formerEnd);
  }
}

void listNodes(List *list) {
  for (Node *aux = list->start; aux != NULL; aux = aux->next) {
    printf("key: %u ", aux->key);
    printf("data: %s\n", aux->data);
  }
}

int main(void) {
  List *lista = createList();
  unsigned int key = 0;
  char string[128];

  int option = 0, running = 1;
  while (running) {
    printf("\n0 - Sair");
    printf("\n1 - Listar");
    printf("\n2 - Pop");
    printf("\n3 - Criar\n");

    printf("\nQual a opcao desejada? ");
    scanf("%d", &option);
    printf("\n");

    switch (option) {
    case 0:
      running = 0;
      break;
    case 1:
      listNodes(lista);
      break;
    case 2:
      delete (lista);
      break;
    case 3:

      printf("\nInsira a chave para o no: ");
      scanf("%u", &key);

      printf("\nInsira a string para o no: ");
      getchar();
      fgets(string, sizeof(string), stdin);
      string[strcspn(string, "\n")] = '\0';
      insert(lista, key, string);
      break;
    }
  }

  libera(lista);

  return 0;
}