# get_next_line

> Read a file (or standard input) line by line through a file descriptor, one
> `read()` buffer at a time, with zero memory leaks.
>
> Lire un fichier (ou l'entrée standard) ligne par ligne via un descripteur de
> fichier, bloc par bloc, sans aucune fuite mémoire.

A [42 School](https://42.fr) project. The goal is to write a function
`get_next_line(int fd)` that returns the next line read from a file descriptor,
including the terminating `\n` (when present), and `NULL` once there is nothing
left to read.

Projet de l'école 42. L'objectif est d'écrire une fonction
`get_next_line(int fd)` qui renvoie la ligne suivante lue depuis un descripteur
de fichier, terminateur `\n` inclus (s'il existe), puis `NULL` une fois la
lecture terminée.

---

## 🇬🇧 English

### Principle

`get_next_line` reads from a file descriptor and returns its content one line at
a time. Each call returns the next line — the text up to and including the next
`\n` (or up to end of file for the last line). When there is nothing left to
read, it returns `NULL`. The file is never loaded entirely into memory: it is
consumed progressively, which makes the function work the same whether the
source is a small file, a multi-gigabyte log, or `stdin`.

```c
char *get_next_line(int fd);
```

The caller is responsible for `free()`-ing every returned line.

### How it works

- **Static variable.** A `static char *left_c;` survives between calls. It holds
  whatever was read past the previous line but not yet returned. This is what
  lets the function "remember" where it stopped without re-reading the file.
- **Reading by blocks.** `fill_line_buffer()` calls `read(fd, buffer,
  BUFFER_SIZE)` in a loop, appending each chunk to the stash, until either a
  `\n` is found or `read()` returns `0` (EOF). Reading happens `BUFFER_SIZE`
  bytes at a time, never the whole file at once.
- **BUFFER_SIZE.** The size of each `read()` call is a compile-time constant.
  It is defined with `-D BUFFER_SIZE=n` at compilation; if it is not provided,
  the header falls back to a default (`42` here). The algorithm is independent
  of the value — it works with `1`, `42`, `9999`, etc.
- **Splitting.** Once a full line is available in the stash, `set_line()`
  extracts the part up to and including the `\n` (the line returned to the
  caller), and `set_left_c()` keeps the remainder in the static variable for the
  next call.
- **Memory hygiene.** Every intermediate allocation is freed: on error, on EOF,
  and when the stash is consumed, the static pointer is freed and reset to
  `NULL` so nothing leaks across calls.

The helper functions (`ft_strjoin`, `ft_strchr`, `ft_strdup`, `ft_substr`,
`ft_strlen`, `fill_str`) live in `get_next_line_utils.c`.

#### Bonus

The bonus version (`get_next_line_bonus.c` / `.h`) handles **multiple file
descriptors at once**. Instead of a single static pointer it uses an array,
`static char *left_c[MAX_FD];` (with `MAX_FD` defaulting to `1024`), indexed by
the file descriptor. This means you can interleave calls on several fds and each
keeps its own independent read position.

### Build & Use

There is no Makefile in this repository: compile the sources directly, passing
the buffer size with `-D BUFFER_SIZE=n`.

**Mandatory version** (a `main.c` is included; it opens the local `feur` file as
a demo):

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
   get_next_line.c get_next_line_utils.c main.c -o gnl
./gnl
```

**Bonus version** (multiple file descriptors):

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
   get_next_line_bonus.c get_next_line_utils_bonus.c your_main.c -o gnl_bonus
```

**Using it in your own code** — call it in a loop and free each line:

```c
#include "get_next_line.h"
#include <fcntl.h>

int main(void)
{
    int   fd = open("feur", O_RDONLY);
    char *line;

    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}
```

You can also read from standard input by passing `0` as the file descriptor.

### What I learned

- **File descriptors** — how `read()` works on an fd, what its return value
  means (bytes read, `0` at EOF, `-1` on error), and how to drive it in a loop.
- **Static variables** — using persistent state to carry the leftover bytes
  between successive calls, and the scope/lifetime rules that make it work.
- **Fine-grained memory management** — every `malloc` paired with a `free`, no
  leak on any path (error, EOF, normal flow), validated with tools like
  Valgrind.
- **Buffered reading** — building a complete line from arbitrary-sized chunks,
  and writing an algorithm that behaves correctly for any `BUFFER_SIZE`.

---

## 🇫🇷 Français

### Principe

`get_next_line` lit depuis un descripteur de fichier et en renvoie le contenu
ligne par ligne. Chaque appel renvoie la ligne suivante — le texte jusqu'au
prochain `\n` inclus (ou jusqu'à la fin du fichier pour la dernière ligne).
Quand il n'y a plus rien à lire, la fonction renvoie `NULL`. Le fichier n'est
jamais chargé entièrement en mémoire : il est consommé progressivement, ce qui
fait fonctionner la fonction de la même manière pour un petit fichier, un
journal de plusieurs gigaoctets, ou `stdin`.

```c
char *get_next_line(int fd);
```

C'est à l'appelant de `free()` chaque ligne renvoyée.

### Fonctionnement

- **Variable statique.** Une `static char *left_c;` survit entre les appels.
  Elle conserve ce qui a été lu au-delà de la ligne précédente mais pas encore
  renvoyé. C'est ce qui permet à la fonction de « se souvenir » de l'endroit où
  elle s'est arrêtée sans relire le fichier.
- **Lecture par blocs.** `fill_line_buffer()` appelle `read(fd, buffer,
  BUFFER_SIZE)` dans une boucle, en ajoutant chaque morceau au tampon
  accumulé, jusqu'à trouver un `\n` ou jusqu'à ce que `read()` renvoie `0`
  (fin de fichier). La lecture se fait `BUFFER_SIZE` octets à la fois, jamais
  tout le fichier d'un coup.
- **BUFFER_SIZE.** La taille de chaque `read()` est une constante définie à la
  compilation, via `-D BUFFER_SIZE=n` ; si elle n'est pas fournie, le header
  utilise une valeur par défaut (`42` ici). L'algorithme est indépendant de
  cette valeur — il fonctionne avec `1`, `42`, `9999`, etc.
- **Découpage.** Une fois une ligne complète disponible dans le tampon,
  `set_line()` en extrait la partie jusqu'au `\n` inclus (la ligne renvoyée à
  l'appelant), et `set_left_c()` garde le reste dans la variable statique pour
  l'appel suivant.
- **Hygiène mémoire.** Chaque allocation intermédiaire est libérée : en cas
  d'erreur, à la fin du fichier, et lorsque le tampon est consommé, le pointeur
  statique est libéré puis remis à `NULL` afin que rien ne fuie d'un appel à
  l'autre.

Les fonctions utilitaires (`ft_strjoin`, `ft_strchr`, `ft_strdup`, `ft_substr`,
`ft_strlen`, `fill_str`) se trouvent dans `get_next_line_utils.c`.

#### Bonus

La version bonus (`get_next_line_bonus.c` / `.h`) gère **plusieurs descripteurs
de fichiers à la fois**. Au lieu d'un seul pointeur statique, elle utilise un
tableau, `static char *left_c[MAX_FD];` (avec `MAX_FD` valant `1024` par
défaut), indexé par le descripteur de fichier. On peut ainsi alterner les
appels sur plusieurs fds, chacun conservant sa propre position de lecture
indépendante.

### Compilation & Utilisation

Il n'y a pas de Makefile dans ce dépôt : on compile les sources directement, en
passant la taille du tampon avec `-D BUFFER_SIZE=n`.

**Version obligatoire** (un `main.c` est fourni ; il ouvre le fichier local
`feur` à titre de démonstration) :

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
   get_next_line.c get_next_line_utils.c main.c -o gnl
./gnl
```

**Version bonus** (plusieurs descripteurs de fichiers) :

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
   get_next_line_bonus.c get_next_line_utils_bonus.c ton_main.c -o gnl_bonus
```

**Utilisation dans ton propre code** — appelle la fonction dans une boucle et
libère chaque ligne :

```c
#include "get_next_line.h"
#include <fcntl.h>

int main(void)
{
    int   fd = open("feur", O_RDONLY);
    char *line;

    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}
```

On peut aussi lire depuis l'entrée standard en passant `0` comme descripteur.

### Ce que ça m'a apporté

- **Les descripteurs de fichiers** — comprendre comment `read()` opère sur un
  fd, ce que signifie sa valeur de retour (octets lus, `0` à la fin, `-1` en
  cas d'erreur), et comment le piloter dans une boucle.
- **Les variables statiques** — utiliser un état persistant pour transporter le
  reste des octets entre les appels successifs, et les règles de portée et de
  durée de vie qui rendent cela possible.
- **La gestion fine de la mémoire** — chaque `malloc` associé à un `free`,
  aucune fuite sur aucun chemin (erreur, fin de fichier, flux normal), validé
  avec des outils comme Valgrind.
- **La lecture bufferisée** — reconstruire une ligne complète à partir de
  morceaux de taille arbitraire, et écrire un algorithme correct quelle que
  soit la valeur de `BUFFER_SIZE`.
