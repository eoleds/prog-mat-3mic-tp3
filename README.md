
# Programmation Orientée Matériel TP

L'objectif du TP est d'optimiser l'implémentation d'une base de données.

L'interface (API) que doit avoir la base de données est spécifiée dans le fichier `api.h` et le fichier `base.c` contient l'implémentation actuelle que vous êtes chargés d'optimiser.

Afin de mesurer les performances de votre implémentation, il vous est fourni un programme de test `main.c`.
Le projet est fourni avec un `Makefile`, celui-ci vous permet de compiler le programme de test (et votre implémentation) à l'aide la commande `make`.


## Familiarisation avec le code

Ouvrez le fichier `api.h` et analysez la structure de données `struct Person`. Celle-ci représente un élément de votre base de données.
Regardez également les différentes fonctions dans le fichier. Ces fonctions correspondent à des requêtes faîtes sur une base de données que vous devrez implémenter ou améliorer.

- Quelle est la taille (en octets) d'une structure Person ?

Ouvrez le fichier `base.c` qui contient l'implémentation actuelle de la base de donnée. 

 - Quelle est la structure en mémoire de la base de données ? Représentez la schématiquement.
 - Quelle est la mémoire occupée par la base de données pour N éléments ?
 - Quel est le nombre maximum d'élément que vous pouvez garder en RAM sur votre machine ?

Ouvrez le fichier `main.c`. Celui-ci contient du code pour générer des données et faire des tests de performance sur la base de données. Vous pouvez ignorer la plupart du code dans ce fichier et vous concentrer uniquement sur le contenu de la fonction `main`.

Une fois le projet compilé (avec `make`) vous pouvez lancer le test de performance avec la commande

    ./base N

où `N` est en entier entre 6 et 27. Le nombre d'éléments dans la base de données sera de 2^N.


## Passage à l'échelle

Les benchmarks contiennent deux tests qui cherche à accéder à grand nombre d'éléments de la base de données : `sequential_access` et `random_access`.
Regardez dans le fichier `main.c` à quoi correspondent ces tests.

On souhaite étudier le passage à l'échelle de ces benchmarks en fonction du nombre d'éléments dans la base de données.

 - Tracez les courbes du temps par opération pour ces deux benchmarks pour N allant de 6 à 27.
 - Comment expliquez vous l'allure de ces courbes ?

Trouvez la taille des caches sur votre machine (commande `lscpu`).

 - Pour les accès aléatoires, tracez les temps de calcul par opération en fonction de la quantité de données en mémoire. Que constatez vous ?
 - Expliquez les résultats à partir de la proportion de données pouvant entrer dans chaque niveau de cache.

 - Comment expliquer vous les résultats différents pour l'accès séquentiel ?




### Alignement mémoire
 
 - Combien de lignes de cache sont nécessaire pour contenir une personne ? Au minimum/maximum ?

On rappelle que sur les processeurs Intel modernes une ligne de cache fait 64 octets et que, par conséquent toutes les lignes de cache commencent à une adresse multiple de 64.

- Vérifiez l'adresse des 2 premièrs éléments de la base de données. Tiennent-t'ils bien une ligne de cache ? Vous pouvez lire et modifier la valeur d'une adresse en la convertissant en entier :

```c
Person *p = ...;
unsigned long address = (unsigned long) p;
p = (Person *) address;
```

- Modifiez la fonction `db_init()` pour s'assurer que tous les éléments tiennent sur une seule ligne de cache.
- Mesurez l'impact en performance sur les accès séquentiels et aléatoires.

## Optimisation de requêtes

### Counting

- Calculez le nombre d'opérations et la mémoire parcourue par `db_count_male` (pour un N donné)
- Quel est le facteur limitant ? 

### Distances

On cherche à analyser les différentes opérations dans la boucle de `db_closest`

Pour vous aider à identifier les différentes instructions et trouver leur coût, vous pourrez regarder l'assembleur généré pendant la compilation (`base.o.asm`) et sur la table des instructions : [https://www.agner.org/optimize/instruction_tables.pdf](https://www.agner.org/optimize/instruction_tables.pdf) (attention à bien sélectionner votre processeur).

- Quelles sont les dépendances entres les opérations ?
- Quelle est l'instruction la plus coûteuse ?
- Comment peut-on limiter le coût des instructions utilisées ?


## Struct of Arrays

Partant du constat que l'on arrive pas à compter les individus masculins suffisamment rapidement, un collègue a pris sur lui de restructurer la base de données.
Il a créé une nouvelle structure de données interne pour la base de données et a mis son implémentation dans `arrays.c`.
Malheureusement il est parti avant d'avoir pu implémenter la moindre de requête.

Le `Makefile` contient aussi les spécifications pour compiler ce programme et produit l'exécutable `./arrays`.
Pour en activer la compilation, ajoutez la cible `arrays` dans le champ `TARGETS`.

- Analysez sa structure de données et mesurez les performances sur les accès séquentiels et aléatoires.
- Qu'est ce qui explique les performances de cette structure ?
- À votre avis, pourquoi a-t-il fait ce choix là ?


### Implémentation des requêtes

Implémentez les requêtes manquantes, d'abord dans une première version qui correspond à l'implémentation initialement trouvée dans `base.c`.

Quand vous faites tourner les programmes `./arrays` et `./base`, la dernière ligne affiche le résultat des requêtes. Vous pouvez l'utiliser pour comparer que votre implémentation donne bien le même résultat.

Cherchez maintenant à optimiser au maximum chacune des fonctions présentes. Vous chercherez en particulier à limiter au maximum les différents aléas du pipeline (structurels, de données et de contrôle).


## SIMD

Avant toute chose, ouvrez le Makefile et ajoutez l'option `-march=native`. Cette option spécifie au compilateur de spécialiser le code généré pour votre machine en particulier.

En commençant par la request `db_max_age()`, utiliser les instruction SIMD pour paralléliser vos requêtes.

Vous pourrez pour cela vous appuyer sur les pages suivantes : 


 - [http://0x80.pl/notesen/2018-10-03-simd-index-of-min.html](http://0x80.pl/notesen/2018-10-03-simd-index-of-min.html)
 - [https://www.cs.virginia.edu/~cr4bd/3330/F2018/simdref.html](https://www.cs.virginia.edu/~cr4bd/3330/F2018/simdref.html)


Un peu d'aide :

 - l'utilisation d'instruction SIMD requiert typiquement que la mémoire accédee soit alignée sur la taille du bus de données (qui peut être 128, 256 ou 512 bits selons les processeurs). Vous pouvez utiliser la fonction `memalign` à la place de `malloc` pour allouer de la mémoire alignée. pour plus d'infos :

```
man memalign
```


 - Vous aurez besoins des *intrinsics* pour les instructions SIMD sur les processeurs Intel se trouvent dans les headers suivants. (d'autre headers peuvent être nécessaires pour des instructions flottantes ou d'autres jeux d'isntruction).

```c
#include <smmintrin.h>
#include <immintrin.h>
```
