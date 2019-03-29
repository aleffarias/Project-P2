/*
 * extract.c
 *
 *  Created on: 29 de mar de 2019
 *      Author: alef
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "_binary_tree.h"

int TRASH_SIZE_EXTRACTED = 0;
// int TREE_SIZE_EXTRACTED = 0;

typedef struct _binary_tree {
  unsigned char item;
  struct _binary_tree *left, *right;
} binary_tree;

int power(int a, int b) {
  int base = a;
  if (!b) {
    return 1;
  }
  for (int i = 1; i < b; i++) {
    a *= base;
  }
  return a;
}

int is_bit_i_set(unsigned char c, int i) {
  unsigned char mask = 1 << i;
  return mask & c;
}

int calc_tree_size(binary_tree *bt, FILE *compressed) {
  unsigned char byte = fgetc(compressed);
  int tree_size;

  for (int i = 15; i >= 13; i--) {
    if (is_bit_i_set(byte, i % 8)) {
      TRASH_SIZE_EXTRACTED += power(2, i % 13);
    }
  }

  for (int i = 12; i >= 0; i--) {
    if (is_bit_i_set(byte, i % 8)) {
      tree_size += power(2, i);
    }
    if (i == 8) {
      byte = fgetc(compressed);
    }
  }
  return tree_size;
}

void write_file(binary_tree *bt, FILE *compressed, FILE *extracted,
                long int file_size) {
  int byte, pos_byte = -1;
  unsigned char c;
  binary_tree *aux_bt = bt;

  while (1) {
    if (pos_byte < 0) {
      byte = fgetc(compressed);
      c = byte;
      pos_byte = 7;
    }

    if (aux_bt->left == NULL && aux_bt->right == NULL) {
      fputc(aux_bt->item, extracted);
      aux_bt = bt;
      if ((ftell(compressed) == file_size) &&
          (pos_byte == TRASH_SIZE_EXTRACTED - 1)) {
        return;
      }
    } else {
      (is_bit_i_set(c, pos_byte)) ? (aux_bt = aux_bt->right)
                                  : (aux_bt = aux_bt->left);
      pos_byte--;
    }
  }
}

void decompress() {
  binary_tree *bt = create_empty_binary_tree();
  FILE *compressed = fopen("compressed_file.huff", "r");

  if (compressed == NULL) {
    puts("File error\n");
    return;
  }

  fseek(compressed, 0, SEEK_END);
  long int file_size = ftell(compressed);
  rewind(compressed);

  FILE *extracted = fopen("jubileu.pdf", "w+");

  int tree_size = calc_tree_size(bt, compressed);

  bt = create_tree(bt, compressed, '0', tree_size);

  write_file(bt, compressed, extracted, file_size);

  fclose(compressed);
  fclose(extracted);

  // print_pre_order(bt);
  puts("\n");
}