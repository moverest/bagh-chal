# Bagh Chal

## Membres du groupe

- Samer Abou-Jaoude
- Julien Eyzat
- Clément Martinez (chef de projet)

## Compiler le projet

```
> make
```

Pour compiler tout le projet, il faut avoir `termbox`, `SDL2` et `SDL2_ttf` installés. Sinon, voici les cibles pour seulement compiler seulement avec la SDL2 ou Termbox:

- `build/main_minimalist_sdl`
- `build/main_tb`

Pour compiler le code avec les flags de débuggage:

```
> make debug
```

## Dépendances

- [Termbox](https://github.com/nsf/termbox) ([AUR](https://aur.archlinux.org/packages/termbox-git))
