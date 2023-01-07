# Introduction au parallélisme

Cet exercice est une inroduction au cours de parallélisme et par extension au calcul à haute performance.

Le but de cette séance pratique est de se rendre compte qu'il est possible d'accélérer votre code sur votre propre machine/laptop.
En effet, la majorité des approches qui seront étudiées dans ce cours ne sont plus limitées aux grands centres de calcul scientifique.
Ce qui n'était pas le cas avant 2005.
La justification de cette date sera donnée lors la partie théorique du cours. 

L'idée est simple: pourquoi se limiter à un seul CPU (ou core) quand on en a plusieurs à notre disposition pour résoudre un problème?
C'est un peu l'approche de force brute: plus on a de muscles, plus on peut taper fort.

## Threads et mémoire partagée

Il n'est pas nécessaire de vous introduire le modèle à mémoire partagée (shared memory), ni les technologies ou librairie pour cette séance, étant donné que tout a déjà été étudié lors du cours de concurrence.

En effet, lors de ce cours vous avez étudié les threads Posix de manière à procéder à une exécution **concurrente** des tâches: un thread léger par tâche, qui sont créés par le thread principal.
Le modèle à mémoire partagée implique simplement que tous les threads partagent le même espace mémoire. Grossièrement, c'est-à-dire que tous peuvent lire et écrire dans des variables communes, dites partagées.

Dans le cadre de cet exercice, nous allons invoquer plusieurs threads qui vont collaborer à la résolution d'une tâche unique.
L'idée sera souvent la même: on découpe un grand problème en plusieurs sous-problèmes, si possible en sous-problèmes indépendants.
On résoud ces sous-problèmes par le biais d'autres processus (qu'ils soient légers ou lourds).
Si nécessaire, on combine les solutions des sous-problèmes pour obtenir la solution du grand problème.

L'intéret de cette approche est d'exploiter les architectures courantes, dites multi-core ou multi-CPU (ce dernier terme étant, à mon avis, plus correcte que le premier).
Les threads **concurrents** seront potentiellement exécutés sur les différents CPUs de la puce.
Ce qui implique que certains sous-problèmes devraient résolus en même temps, c'est-à-dire en **parallèle**.
Mais comme tout ce qui touche à la concurrence, on en est jamais vraiment sûr.
Ce dernier point sera résolu avec l'utilisation du cluster de l'UniGE qui incorpore des noeuds de calculs de l'HEPIA.
Mais pour le moment, nous nous limitons à nos propres machines.

## Les thread Posix

Voici  bref rappel des thread Posix
Mais très très bref.
Veuillez vous référer à votre cours de concurrence pour plus de détail ou au `man` de votre machine.

On rappelle qu'un thread Posix se crée avec:

```c
pthread_create(&tid, NULL, f, (void*) &f_args);
```

où `tid` est l'id du thread de type `pthread_t`,
`f` la tâche à exécuter par le thread et `f_args` l'argument de `f`.

On rappelle que:
- `f` retourne un `void*` mais ce pointeur peut toujours être casté dans le type désiré.
Notez que l'espace mémoire est partagé, donc si nécessaire, les threads peuvent aussi accéder à une variable commune (càd. partagée ou _shared_).
- Si la fonciton `f` ne prend aucun argument, on passe `NULL`.
- S'il faut passer plusieurs argument à `f`, on utilise habituellement une `struct` pour `f_args`.

Une fois que les threads ont accompli leur tâche, il faut s'assurer de libérer les ressources prise par les threads correctement. 
Ceci se fait avec la fonction `pthread_exit`:
```c
void* thread_func(void* arg) {
  // Thread code goes here
  pthread_exit(NULL);
}
```

À noter que si on souhaite retourner un pointeur non `NULL` on peut faire, par exemple:
```c
void* thread_func(void* arg) {
  int* result = (int*) malloc(sizeof(int));
  *result = 0;
  // Thread code goes here
  pthread_exit((void*) &result);
}
```
où le retour est dans `pthread_exit`.

Pour terminer on joint les threads dans le thread principal.
Typiquement, dans le `main`:

```c
pthread_t tid;
pthread_create(&tid, NULL, thread_func, NULL);
void* res;
pthread_join(tid, &res);
int* result = (int*) res;
```

Attention, il faut noter que la fonction `pthread_exit` ne libère pas automatiquement toutes les ressources allouée dans le thread. 
En effet, si des ressources ont été allouées dynamiquement, il faut les libérer manuellement avant d'appeler `pthread_exit`.
Par exemple:

```c
void* thread_func(void* arg) {
  int* result = malloc(sizeof(int));
  // Thread code goes here
  free(result);
  pthread_exit((void*)result);
}
```

On rappelle également que les définitions sont dans `pthread.h` et qu'il faut passer la librairie `pthread` pour l'éditeur de lien (avec `gcc` l'option est `-l`).
Donc ajouer: `-lpthread`.
Si nécessaire, installez cette librairie sur votre machine.

Pensez également à ajouter `-O3`.
Si vous ne connaissez pas cette option, veuillez vous référer au `man` de gcc.

Tout ceci sera pratiqué dans les exercices ci-dessous.

## Calcul de pi en parallèle

Le but de cet exercice est d'approximer la valeure de pi.

Pour ceci, implémentez la méthode de Monte Carlo.
La méthode consiste à générer un grand nombre de points aléatoires dans le carré unitaire et de compter le nombre de points qui tombent dans le cercle unitaire inclus dans le carré. 
La proportion de nombre de points qui tombent dans le cercle permettent d'approximer pi/4.
C'est une méthode que vous avez probablement vue plusieurs fois au cours de vos études, ici même à HEPIA.

Pour ceci:
- écrivez une fonction qui génère des points aléatoires dans le carré unitaire et retourne le nombre qui tombent dans le cercle.
Cette fonction sera la tâche de vos threads, elle prendra en paramètre le nombre de points à générer par le thread.
- Calculez le rapport pour approximer pi en sommant les contributions de vos threads.
Si vous le souhaitez, ceci peut se faire dans le thread principal.

Vous être libre d'implémenter votre programme comme bon vous semble, mais suivez les points suivants:
- comparez l'approximation avec la valeure de pi calculée avec la constante `M_PI` dans la librairie `math.h`.
Ceci pour vous assurer que votre implémentation soit correcte. 
- Votre programme prend à la ligne de commande deux entiers: (1) le nombre de point totaux à générer, (2) le nombres de threads.

## Somme en parallèle

Le but de cet exercice est de calculer la somme des éléments d'un vecteur de doubles.

Commencez par une implémentation naïve.
Une basée sur une simple boucle ``for`` devrait faire l'affaire:
```c
s = 0;
for (int k = 0; k < n; k++) {
  s += v[k];
}
```

Implémentez une version accélérée basée sur les thread Posix.
L'idée est simple:
- chaque thread calcul la somme d'un sous-vecteur,
- les sommes partielles sont ensuites réduites en une somme globale.

Vous pouvez faire la réduction dans le thread principale.

Vous être libre d'implémenter votre programme comme bon vous semble, mais suivez les points suivants:
- implémentez une fonction qui initialise un vecteur de taille `n` qui contient les entiers alland de 1 à `n`.
Notez que le type des valeurs doit être `double`.
C'est sur ce vecteur que la somme sera calculée.
L'avantage est que la somme est connue, il s'agit de `n*(n+1)/2`.
Utilisez la pour vérifier que votre programme est correcte.

- votre programme prend à la ligne de commande deux entiers: (1) la taille du vecteur à sommer, (2) le nombres de threads.

## Mesures de performance

Une fois vos implémentation terminées, mesurez le gain obtenu par le parallélisme.
Pour ceci procédez à des mesures des temps d'exécution de vos implémentations séquentielles et parallèles.

Pour ce type de mesures il faut noter les points suivants:
- la version séquentielle (non-parallèle) est censée être la meilleure implémentation séquentielle possible.
En effet, il serait pas très malin de penser que le gain de parallélisme est bon si la version séquentielle de référence n'est pas la plus efficace. 

- En pratique, dans ce cours, on a pas toujours accès à cette implémentation. Elle peut même s'avérer compliquée à implémenter.
Un bonne exemple d'implémentation séquentielle compliquée peut être la multiplication de matrices carrées.
Dans ce cas et uniquement pour ce cours, on se rabattra sur une implémentation séquentielle **naïve**.

- Il ne faut pas utiliser pas votre implémentation parallèle avec un seul thread.
La raison est qu'on ne veut pas mesurer la surcharge de travail (overhead) dû à la cération et destruction des threads.

- Pour les mesures il faut résoudre un problème **suffisament grand**.
Bien que cette notion de "suffisament grand" soit vague, l'idée est simple: l'exécution la plus rapide doit être de l'ordre de la minute,
sinon les mesures peuvent être non significative. P.ex. on ne mesure que l'initialisation, le chargement du programme en mémoire, etc.

- Il ne faut pas qu'un autre programme tourne lors de ces mesures.
Après avoir suivi le cours de concurrence, ceci devrait être une évidence.
Évidement, ceci ne concerne pas les processus de maintenance du système d'exploitation.

Faites donc varier le nombre de thread ainsi que la taille du vecteur et mesurez les temps d'exécution.
Idéalement, pour faire de telles mesures, on utilise un script qui automatise l'exécution.

Notez que vous pouvez mesurer votre temps d'exécution directement dans votre code.
L'extrait suivant devrait vous aider:

```c
struct timespec start, finish;
clock_gettime(CLOCK_MONOTONIC, &start);

// code à mesurer

clock_gettime(CLOCK_MONOTONIC, &finish);
double seconds_elapsed = finish.tv_sec-start.tv_sec;
seconds_elapsed += (finish.tv_nsec-start.tv_nsec)/1.0e9;
```

Vous devez inclure `time.h` pour que ceci fonctionne et compiler avec `-std=gnu11`.

## Questions

Comprenez-vous bien la différence entre exécution concurrente et exécution parallèle ? 
Est-ce que l'une implique l'autre ?

À votre avis, dans les problèmes implémetés lors de cette séance, est-ce qu'il y a un intéret à augmenter le nombre de threads ? 
Si oui, jusqu'à quel point ? 
Y-a-t-il une limite ?

On vous a demandé précédemment de combiner les sommes partielles dans le thread principale.
À votre avis, est-ce que ceci vous semble optimal ?
Pouvez-vous proposer une stratégie plus intéressante en terme de performance ?

On a proposé précédemment et pour ce cours, de se rabattre sur une implémentation séquentielle naïve pour évaluer les performances de l'implémentation parallèle.
À votre avis, pouvons nous-utiliser l'implémentation parallèle en utilisant un seul thread ?
Notez que concrètement, il y aura deux threads: le principale (toujours) et le thread léger. 

# Solutions

Les solutions seront soit ajoutées ici, ou expliquées en cours.
