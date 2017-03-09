# Exercice Termbox

L'idée de cette exercice est de se familliariser avec Termbox. Pour cela, vous écrirez un petit programme dans un fichier suivant les indications.

Avant de commencer, on pensera à installer la bibliothèque termbox. Pour cela, les instructions sont sur leur [dépôt git](https://github.com/nsf/termbox). Pour Archlinux, un simple `yaourt -S termbox-git` suffit.

Pour compiler un programme avec termbox, il faut utiliser le paramètre `-ltermbox` avec gcc:

```
> gcc -ltermbox fichier.c
```

Dans le programme final, on veut que l'utilisateur puisse rentrer les coordonnées de déplacement des chèvres ou tigres. Il nous faut donc gérer un champ.

Ce champ fonctionne comme décrit ci-dessous.

Avant que l'on commencer à taper, est précédé du mot `Move`

```
Move _
```

Ensuite, on tape la première coordonnée dans le tableau (une lettre):

```
Move a_
```

Et une fois la seconde coordonnée tapée, on ajoute `to` au champ:

```
Move a1 to _
```

Une fois qu'on a fini de taper la seconde coordonnée, on quitte termbox pour ensuite afficher les deux coordonnées tapées dans la sortie standard (`printf`).

```
a1
b2
```

En pensera à gérer le *backspace* pour la correction.

En amélioration, on pourra:

- mettre les coordonnées en couleur dans le champs
- restraindre les coordonnés que l'on peut taper (avec une liste donnée)
