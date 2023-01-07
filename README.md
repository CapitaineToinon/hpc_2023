# Introduction au parallélisme

Cet exercice est une inroduction au cours de parallélisme et par extension au calcul à haute performance.

Le but de cette séance pratique est de se rendre compte qu'il est possible d'accélérer votre code sur votre propre machine/laptop.
En effet, la majorité des approches qui seront étudiées dans ce cours ne sont plus limitées aux grands centres de calcul scientifique.
Ce qui n'était pas le cas avant 2005.
La justification de cette date sera donnée lors la partie théorique du cours. 

## Threads et mémoire partagée

Il n'est pas nécessaire de vous introduire le modèle à mémoire partagée (shared memory), ni les technologies ou librairie pour cette séance, étant donné que tout a déjà été étudié lors du cours de concurrence.

En effet, lors de ce cours vous avez étudié les threads Posix de manière à procéder à une exécution **concurrente** des tâches: un thread léger par tâche, qui sont créés par le thread principal.
Le modèle à mémoire partagée implique simplement que tous les threads partagent le même espace mémoire. Grossièrement, c'est-à-dire que tous peuvent lire et écrire dans des variables communes, dites partagées.

Dans le cadre de cet exercice, nous allons invoquer plusieurs threads qui vont collaborer à la résolution d'une tâche unique.
L'idée sera souvent la même: on découpe un grand problème en plusieurs sous-problèmes, si possible en sous-problèmes indépendants.
On résoud ces sous-problèmes par le biais d'un autre processus (léger ou lourd).
Si nécessaire, on combine les solutions des sous-problèmes pour obtenir la solution du grand problème.

L'intéret de cette approche est d'exploiter les architectures courantes, dites multi-core ou multi-CPU (ce dernier terme étant, à mon avis, plus correcte que le premier).
Les threads **concurrents** seront potentiellement exécutés sur les différents CPU de la puce.
Ce qui implique que certains sous-problèmes seront résolus en même temps, c'est-à-dire en **parallèle**.
Mais comme tout ce qui touche à la concurrence, on en est jamais vraiment sûr.

Tout ceci sera pratiqué dans l'exercice ci-dessous.

## Somme en parallèle

Le but de cet exercice est très simple: il s'agit de calculer la somme des éléments d'un vecteur de doubles.

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

- votre programme prend à la ligne de commande deux entiers: (1) la taille du vecteur à sommer, (2) le nombres de threads.

On rappelle qu'un thread Posix se crée avec:
```c
pthread_create(&tid, NULL, f, (void*) &f_args);
```
où `tid` est l'id du thread de type `pthread_t`,
`f` la tâche à exécuter par le thread et `f_args` l'argument de `f`.
On rappelle que `f` retourne un `void*`.

Une fois les threads terminé, on joint ces dernier avec: 
```c
pthread_join(tid, NULL);
```
où `tid` est comme prédécemment l'id du thread de type `pthread_t`.

On rappelle également que les définitions sont dans `pthread.h` et qu'il faut passer la librairie `pthread` pour l'éditeur de lien (avec `gcc` l'option est `-l`).
Si nécessaire, installez cette librairie sur votre machine.

Il s'agit d'un bref rappel. 
Veuillez vous référer à votre cours de concurrence pour plus de détail ou au `man` de votre machine.

## Mesures de performance

Une fois votre implémentation terminée, mesurez le gain obtenu par le parallélisme.
Pour ceci procédez à des mesures des temps d'exécution de vos implémentations séquentielles et parallèles.

Pour ce type de mesures il faut noter les points suivants:
- la version séquentielle (non-parallèle) est censée être la meilleure implémentation séquentielle possible.
En effet, il serait pas très malin de penser que le gain de parallélisme est bon si la version séquentielle de référence n'est pas la plus efficace. 

- En pratique, dans ce cours, on a pas toujours accès à cette implémentation. Elle peut même s'avérée compliquer à implémenter.
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

## Questions

Comprenez-vous bien la différence entre exécution concurrente et exécution parallèle ? 
Est-ce que l'une implique l'autre ?

À votre avis, dans le problème de la somme, est-ce qu'il y a un intéret à augmenter le nombre de threads ? 
Si oui, jusqu'à quel point ? 
Y-a-t-il une limite ?

On vous a demandé précédemment de combiner les sommes partielles dans le thread principale.
À votre avis, est-ce que ceci vous semble optimal ?
Pouvez-vous proposer une stratégie plus intéressante en terme de performance ?

# Solutions

Les solutions seront soit ajoutées ici, ou expliquées en cours.
