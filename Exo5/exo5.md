# Exercice 5
## 5.1

producer consumer: 
- producer produces and puts inside buffer
- consumer consumes the buffer, one piece at a time
- producer can't add to buffer if the latest is full
- consumer can't take from the buffer if this one is empty
- accessing memory buffer should not be allowed to both producer and consumer on the same time

producer consumer : 2 semaphores. Ici il y a 2 producteurs et 2 consommateurs -> 4 semaphores
mutual exclusion : 1 mutex pour protéger la piste d'atterrissage/décollage

-----------------
- gestion de la file de décollage (file d'attente buffer)
- gestion de la file d'atterrissage (file d'attente buffer)
- gestion de l'unique piste

4 threads
- SortirAvion -> met dans le buffer (producteur)
- ApprocheAvion -> met dans le buffer
- Decollage -> consomme
- Atterrissage -> consomme

pour sécuriser la piste, on utilise un mutex sur la piste
Air et Sol sont des buffers

if (sol libre > 0) ? P(sol libre -> ajout sol) : wait

if (piste libre && ajout sol > 0) ? décollage : wait

## 5.2

P(sol occupé)
lock(mutex piste)
decoller()
unlock(mutex piste)
V(sol libre)


deadlock (interblocage) : tout le monde s'attend
livelock (famine) : ressource jamais libérée