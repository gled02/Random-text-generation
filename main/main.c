/*
 *  Affiche sur la sortie standard :
 *  - un text aléatoire des différents produit par des mots lus sur l'entrée
 *  - le bilan de santé de la table de hachage utilisée.
 *  Limitation : les mots sont compris au format "%ns" où n a pour valeur
 *    STRING_LENGTH_MAX
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hashtable.h"
#include "holdall.h"
#include "lwords.h"

#define STR(s)  #s
#define XSTR(s) STR(s)

#define STRING_LENGTH_MAX 31

#define FUN_SUCCESS 0
#define FUN_FAILURE 1

static int key__succ_display(lwords *key, lwords *succ);

static int rfree__list(void *ptr);

static int rfree__word(void *ptr);

enum {
  PROGN_IDX,
  ORDER_IDX,
  LIMIT_IDX,
  HELP_IDX,
  TABL_IDX
};

int main(int argc, char *argv[]) {
  srand(0);
  int n = 1;
  int m = 10;
  
  //--- Partie options ---------------------------------------------------------

  if (argc == 2 && strlen(argv[HELP_IDX - 2]) > 2
      && strncmp(argv[HELP_IDX - 2], "--help",
      strlen(argv[HELP_IDX - 2])) == 0) {
    fprintf(stdout, "Usage: %s\n", argv[PROGN_IDX]);
    fprintf(stdout, "Random text generator game.\n\t");
    fprintf(stdout, "Guide\n");
    fprintf(stdout, "\tEnter the program name followed by these options\n\t");
    fprintf(stdout, "\t-nNUMBER -> fix the order of generation to NUMBER\n\t");
    fprintf(stdout,
        "\t-lNUMBER -> fix the limit of words to produce to NUMBER\n\t");
    fprintf(stdout, "You can also enter\n\t");
    fprintf(stdout, "\tProgram name followed by -nNUMBER, followed by :\n\t");
    fprintf(stdout, "\t--dispaly-table- > display the table\n");
    fprintf(stdout, "\tChoose --help if you need help\n");
    return EXIT_SUCCESS;
  } else if (argc == 3) {
    if (strncmp(argv[ORDER_IDX], "-n", 2) == 0) {
      if (atoi((char *) argv[ORDER_IDX] + 2 * sizeof(char)) != 0) {
        n = atoi((char *) argv[ORDER_IDX] + 2 * sizeof(char));
      } else {
        fprintf(stderr, "*** Invalid order of generation.\n");
        return EXIT_FAILURE;
      }
    }
    if (strncmp(argv[LIMIT_IDX], "-l", 2) == 0) {
      if (atoi((char *) argv[LIMIT_IDX] + 2 * sizeof(char)) != 0) {
        m = atoi((char *) argv[LIMIT_IDX] + 2 * sizeof(char));
      } else {
        fprintf(stderr, "*** Invalid number of words.\n");
        return EXIT_FAILURE;
      }
    }
  } else {
    fprintf(stderr, "*** Bad number of parameters. Try --help.\n");
    return EXIT_FAILURE;
  }
  
  //--- Implantation d'algorithme ----------------------------------------------

  hashtable *ht = hashtable_empty((int (*)(const void *,
          const void *))lwords_comparison,
          (size_t (*)(const void *))lwords_hashfun);
  holdall *hakey = holdall_empty();
  holdall *hasucc = holdall_empty();
  holdall *haword = holdall_empty();
  lwords *key = lwords_key(" ", (size_t) n);
  if (ht == NULL || hakey == NULL || hasucc == NULL || haword == NULL
      || key == NULL) {
    goto error_capacity;
  }
  char string[STRING_LENGTH_MAX + 1];
  char *v;
  lwords *succ;
  do {
    succ = (lwords *) hashtable_value(ht, key);
    if (succ == NULL) {
      succ = lwords_empty();
      if (succ == NULL) {
        goto error_capacity;
      }
      if (holdall_put(hakey, key) != 0) {
        lwords_dispose(&succ);
        goto error_capacity;
      }
      if (holdall_put(hasucc, succ) != 0) {
        lwords_dispose(&succ);
        goto error_capacity;
      }
      if (hashtable_add(ht, key, succ) == NULL) {
        lwords_dispose(&succ);
        goto error_capacity;
      }
    } else {
      if (holdall_put(hasucc, key) != 0) {
        goto error_capacity;
      }
    }
    if (scanf("%" XSTR(STRING_LENGTH_MAX) "s", string) != 1) {
      v = " ";
    } else {
      if (strlen(string) >= STRING_LENGTH_MAX) {
        fprintf(stderr, "*** Warning: String '%s...' possibly sliced.\n",
            string);
      }
      v = malloc(strlen(string) + 1);
      if (v == NULL) {
        goto error_capacity;
      }
      strcpy(v, string);
      if (holdall_put(haword, v) != 0) {
        free(v);
        goto error_capacity;
      }
    }
    if (lwords_add_tail(succ, v) == NULL) {
      goto error_capacity;
    }
    key = lwords_replace(key, v);
    if (key == NULL) {
      goto error_capacity;
    }
  } while (strcmp(v, " ") != 0);
  
  //--- Affichage du tableau si l'opsion --display-table est choisi ------------

  if (strcmp(argv[TABL_IDX - 2], "--display-table") == 0) {
    lwords *key_r = lwords_reverse((lwords *) hakey);
    if (key_r == NULL) {
      goto error_capacity;
    }
    if (holdall_apply_context((holdall *) key_r,
        (int (*)(void *, void *))key__succ_display,
        (void *(*)(void *, void *))hashtable_value, ht) != 0) {
      lwords_dispose(&key_r);
      goto error_write;
    }
    lwords_dispose(&key_r);
    goto success;
  }
  
  //--- Génération aléatoire ---------------------------------------------------

  lwords *key_n = lwords_key(" ", (size_t) n);
  if (key_n == NULL) {
    goto error_capacity;
  }
  succ = (lwords *) hashtable_value(ht, key_n);
  v = (char *) lwords_index_value(succ, (size_t) rand() % lwords_length(succ));
  if (v == NULL) {
    lwords_dispose(&key_n);
    goto error_capacity;
  }
  int count = 0;
  while (strcmp(v, " ") != 0 && count < m) {
    fprintf(stdout, "%s ", v);
    key_n = lwords_replace(key_n, v);
    if (key_n == NULL) {
      lwords_dispose(&key_n);
      goto error_capacity;
    }
    succ = (lwords *) hashtable_value(ht, key_n);
    v = (char *) lwords_index_value(succ,
            (size_t) rand() % lwords_length(succ));
    if (v == NULL) {
      lwords_dispose(&key_n);
      goto error_capacity;
    }
    ++count;
  }
  lwords_dispose(&key_n);
  fprintf(stdout, "\n");
#ifdef HASHTABLE_CHECKUP
  hashtable_display_checkup(ht, stderr);
#endif
  int r = EXIT_SUCCESS;
  goto dispose;
error_capacity:
  fprintf(stderr, "*** Error: Not enough memory.\n");
  r = EXIT_FAILURE;
  goto dispose;
error_write:
  fprintf(stderr, "*** Error: A write error occurs.\n");
  r = EXIT_FAILURE;
  goto dispose;
success:
  r = EXIT_SUCCESS;
  goto dispose;
dispose:
  hashtable_dispose(&ht);
  if (hakey != NULL) {
    holdall_apply(hakey, rfree__list);
  }
  holdall_dispose(&hakey);
  if (hasucc != NULL) {
    holdall_apply(hasucc, rfree__list);
  }
  holdall_dispose(&hasucc);
  if (haword != NULL) {
    holdall_apply(haword, rfree__word);
  }
  holdall_dispose(&haword);
  lwords_dispose(&key);
  return r;
}

//--- Fonctions locals ---------------------------------------------------------

int key__succ_display(lwords *key, lwords *succ) {
  if (lwords_fput(key, stdout) != 0) {
    return FUN_FAILURE;
  }
  if (lwords_fput(succ, stdout) != 0) {
    return FUN_FAILURE;
  }
  fprintf(stdout, "\n");
  return FUN_SUCCESS;
}

int rfree__list(void *ptr) {
  lwords_dispose((lwords **) &ptr);
  return FUN_SUCCESS;
}

int rfree__word(void *ptr) {
  free(ptr);
  return FUN_SUCCESS;
}
