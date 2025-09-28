1.1 cf. xo1

1.2. add "slowers" inside threads. Possible to provoke "enlacement" with sleep() and usleep()

2. On a N threads "travailleurs" (un groupe).
On a 1 thread spécial : thread_barriere.
Tous les threads font un bout de travail simulé par des attentes aléatoires (avec sleep() ou usleep()).
Chaque thread du groupe atteint un point de rendez-vous (barrière).
Quand un thread atteint ce point, il continue sa vie (non bloquant).

Le dernier thread du groupe à arriver réveille thread_barriere.

thread_barriere :
- Arrive à la barrière.
- Se bloque (attend).
- Se débloque uniquement quand tous les threads du groupe ont atteint leur barrière.

Un compteur global pour savoir combien de threads du groupe ont atteint leur point de rendez-vous.
Une variable conditionnelle (pthread_cond_t) pour bloquer et réveiller thread_barriere.
Un mutex (pthread_mutex_t) pour protéger l’accès au compteur.
Des sleep() random pour simuler du travail.