# Bagh Chal

## Membres du groupe

- Samer Abou-Jaoude
- Julien Eyzat
- Clément Martinez (chef de projet)

## Compiler le projet

```
> make
```

Pour compiler le code avec les flags de débuggage:

```
make debug
```
## Guides

- [Miroir git](guides/git_mirror.md)
- [Exercice Termbox](guides/train_termbox.md)

## Comment approcher le code?

Le jeu est divisé en plusieurs parties. Pour le moment, nous avons:

- `game` (avec `game.h` et `game.h`) qui est responsable de la logique du jeu. C'est lui qui gère le plateau et les intéractions avec celui-ci.
- `graphics` (avec `grahics.h`) qui est une interface pour afficher le jeu à l'écran. Ici, on ne veut pas dépendre d'un système d'affichage. `graphics` ne fait pas de rendu par lui-même. Pour le faire nous avons:
    - `graphics_tb` (avec `graphics_tb.h` et `grahics_tb.c`) qui se charge de faire le rendu du jeu dans un terminal.
    - (bientôt) `graphics_sdl` pour un rendu totalement graphique avec la SDL.
- `models` (avec `models.h` et `models.c`) qui défini les éléments du jeu (le plateau par exemple).
- `test` (avec `test.c` et `test.h`) pour les créer des tests.

`game` et `graphics` dépendent tout les deux de `models` mais ne se connaissent pas.

Pour commencer à étudier le code, rien de mieux que de regarder les tests. Il y a deux executables indépendants qui fond cela:
- `test_game` codé dans `test_game.c` qui teste la logique du jeu.
- `test_graphics_tb` codé avec `test_graphics_tb.c` qui teste le module graphique pour le rendu dans le terminal.

## Dépendances

- [Termbox](https://github.com/nsf/termbox) ([AUR](https://aur.archlinux.org/packages/termbox-git))
