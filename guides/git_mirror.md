# Miroir Git sur la Forge

Étant donné que la forge ne correspond pas à nos besoins mais que nous sommes obligés de pousser nos changements dessus, nous l'utilisons comme miroir.

Pour se rappeler de mettre à jour le miroir, un bot Slack rappelera les membres tous les lundis.

## Méthode automatisée

Il faudra penser à paramétrer une clef d'authentification SSH pour se connecter au dépot GitHub.

```
./update_mirror.sh
```

## Méthode manuelle
### Ajout du remote

```
> git remote add forge 'https://forge.telecomnancy.univ-lorraine.fr/git/martine96u_c'
```

### Mise à jour du miroir

```
> git push --mirror forge
```
