# OCR

[![Build Status](https://magnum.travis-ci.com/dethi/ocr.svg?token=x9iDKPK5fQNGphzep8jS&branch=develop)](https://magnum.travis-ci.com/dethi/ocr)

## Compilation

Pour initialiser la première fois le repos (nécessite le package autoconf) :

````bash
autoreconf --install
./configure
```

Pour compiler :

```bash
make
```

Pour nettoyer les binaires :

```bash
make clean
```

Pour lancer les tests rapidement :

```bash
./run_test.sh
```

Pour réinitialiser le repos (en cas de bug de compilation/makefile par exemple) :

```bash
git clean -dxf
```

## Tâches

- Pré-traitement :
    * Niveau de gris
    * Binarisation (Otsu par exemple)
    * Flou gaussien

- Découpage :
    * Découper paragraphes
    * Découper les lignes
    * Découper les caractères

- Analyse :
    * Réseau de neurones
    * Apprentissage

- Interface graphique :
    * GTK
    * Glade pour concevoir l'interface

- Site web
