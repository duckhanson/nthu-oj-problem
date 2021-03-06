#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef enum token {
  ID, OP_AND, OP_OR
}Token;
typedef struct node {
  int val, ID;
  Token tokentype;
  struct node *left, *right, *parent;
} Node;

Node* head;
Node** var;  // store pointers points to Tree Nodes. EX, var[0] = Node ID = 0.

Node* ConstructSyntaxTree() {
  char input = getchar();
  Node* root = (Node*)calloc(1, sizeof(Node));
  root->parent = NULL;
  root->val = 0;
  switch (input) {
    case '\n':
      return NULL;
    case '|':
    case '&':
      if (input == '|') root->tokentype = OP_OR;
      else root->tokentype = OP_AND;
      root->ID = 0;
      root->left = ConstructSyntaxTree();
      root->left->parent = root;
      root->right = ConstructSyntaxTree();
      root->right->parent = root;
      break;
    default:
      root->tokentype = ID;
      scanf("%d", &root->ID);  // remember ID would not be one digit.
      root->left = root->right = NULL;
      var[root->ID] = root;
      getchar();  // for ']'
      break;
  }
  return root;
}

void check_up(Node* cur) {
  if (cur == NULL) return;
  int ans;
  if (cur->tokentype == OP_OR)
    ans = cur->left->val | cur->right->val;
  else if (cur->tokentype == OP_AND)
    ans = cur->left->val & cur->right->val;

  if (ans ^ cur->val) { // xor
    cur->val = ans;
    check_up(cur->parent);
  }
}

void free_tree(Node* root) {
  if(root != NULL) {
    free_tree(root->left);
    free_tree(root->right);
    free(root);
  }
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("input.txt", "r", stdin);
#endif
  int id;
  int T, N, M;
  scanf(" %d", &T);
  while (T--) {
    scanf(" %d %d\n", &N, &M);
    var = (Node**)malloc(sizeof(Node*) * (N + 1));
    head = ConstructSyntaxTree();
    while (M--) {
      scanf(" %d", &id);
      var[id]->val = var[id]->val ^ 1; // 0->1, 1->0
      check_up(var[id]->parent);
      printf("%d\n", head->val);
    }
    free(var);
    free_tree(head);
  }

  return 0;
}