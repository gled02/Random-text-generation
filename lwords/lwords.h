//  Partie interface du module lwords

#ifndef LWORDS__H
#define LWORDS__H

#include <stdio.h>
#include <stdbool.h>

//  struct lwords, lwords : structure capable de gérer une liste dynamique
//    simplement chainée dont les éléments sont des objets quelconques. La
//    création de la structure est confiée à la fonction lwords_empty.
//  La structure ne stocke pas les objets mais des pointeurs vers ces objets.
//    Les pointeurs sont du type générique const void *.
//  L'utilisateur doit garantir que l'adresse de chaque objet fournie aux
//    diverses fonctions est valide durant toute la durée de la présence de
//    l'objet dans la structure. Si des opérations d'allocation dynamique
//    sont effectuées, elles le sont pour la gestion propre de la structure,
//    et en aucun cas pour réaliser des copies ou des destructions d'objets
typedef struct lwords lwords;

//  Les fonctions qui suivent ont un comportement indéterminé si leur paramètre
//    de type lwords * n'est pas l'adresse d'un objet préalablement renvoyé par
//    lwords_empty et non révoqué depuis par lwords_dispose.
//    Cette règle ne souffre que d'une seule exception : lwords_dispose
//    tolère que la déréférence de son argument ait pour valeur NULL

//  lwords_empty : crée un objet de type lwords. La liste est initialement vide.
//    Renvoie NULL en cas de dépassement de capacité, un pointeur vers l'objet
//    en cas de succès.
extern lwords *lwords_empty(void);

//  lwords_is_empty : renvoie true ou false selon que la liste associée à s est
//    vide ou non
extern bool lwords_is_empty(const lwords *s);

//  lwords_length : renvoie la longueur de la liste associé à s
extern size_t lwords_length(const lwords *s);

//  lwords_add_tail, lwords_add_head: Ajout en queue (tête) de la liste associé
//    à s l'élément d'adresse xptr. Renvoie NULL si xptr vaut NULL ou en cas de
//    dépassement de capacité.Renvoie sinon l'adresse de l'élément xptr
extern const void *lwords_add_tail(lwords *s, const void *xptr);
extern const void *lwords_add_head(lwords *s, const void *xptr);

//  lwords_reverse : crée une liste qui contient les mêmes éléments que celui
//    associé à s mais à l'ordre inverse (insertion en tête). Renvoie NULL en
//    cas de dépassement de capacité. Renvoie un pointeur vers l'objet qui gère
//    la liste créée sinon
extern lwords *lwords_reverse(lwords *src);

//  lwords_key : crée une liste de longueur order avec tous les éléments qui
//    vallent la valeur pointée par xptr. Renvoie NULL en cas de dépassement de
//    capacité ou si xptr vaut NULL. Renvoie un pointeur vers l'objet qui gère
//    la liste créée sinon
extern lwords *lwords_key(const void *xptr, size_t order);

//  lwords_replace : crée une liste qui contient les mêmes éléments que celui
//    associé s sans le premier et ajoute en queue l'élément d'adresse xptr.
//    Renvoie NULL en cas de dépassement de capacité, si la liste associé à s
//    est vide ou si xptr vaut NULL. Renvoie un pointeur vers l'objet qui gère
//    la liste créée sinon
extern lwords *lwords_replace(const lwords *s, const void *xptr);

//  lwords_index_value : cherche dans la liste pointée par s l'élément d'indice
//    k. Renvoie NULL si k est strictement supérieur à la longueur de s ou si s
//    est vide. Renvoie l'adresse de l'élément trouvée sinon
extern const void *lwords_index_value(const lwords *s, size_t k);

//  lwords_comparaison : compare les éléments des deux listes associées à s1 et
//    s2 selon l'ordre lexicographique Renvoie une valeur strictement
//    inférieure, égale ou strictement supérieure à zéro selon que le premier
//    objet est strictement inférieur, égal ou strictement supérieur au second
extern int lwords_comparison(lwords *s1, lwords *s2);

//  lwords_hashfun : l'une des fonctions de pré-hachage conseillées par
//    Kernighan et Pike traduite pour les listes
extern size_t lwords_hashfun(const lwords *s);

//  lwords_fput : écrit sur le flot contrôlé par l'objet pointé par stream les
//    éléments de la liste associée à s. Les éléments sont écrits séparés par
//    une tabulation. Le délimiteur gauche est vide. Renvoie zéro en cas de
//    succès, une valeur non nulle en cas d'échec
extern int lwords_fput(const lwords *s, FILE *stream);

//  lwords_dispose : si *sptr ne vaut pas NULL, libère les ressources allouées à
//    *sptr ainsi qu'à la liste associée puis affecte la valeur NULL à *sptr
extern void lwords_dispose(lwords **sptr);

#endif
