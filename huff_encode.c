
#include "arbrebin.h"
#include "bfile.h"
#include "fap.h"
#include "huffman_code.h"
#include <assert.h>
#include <stdio.h>

typedef struct {
    int tab[256];
} TableOcc_t;

struct code_char HuffmanCode[256];

void ConstruireTableOcc(FILE *fichier, TableOcc_t *TableOcc) {
    int c;

    c = fgetc(fichier);
    while (c != EOF) {
        TableOcc->tab[c] += 1;
        c = fgetc(fichier);
    };


    int i;
    for (i = 0; i < 256; i++) {
        if (TableOcc->tab[i] != 0)
            printf("Occurences du caractere %c (code %d) : %d\n", i, i,
                   TableOcc->tab[i]);
    }
}

fap InitHuffman(TableOcc_t *TableOcc) {
    fap huff = creer_fap_vide();
    for (int i = 0; i < 256; i++) {
        Arbre arbre = NouveauNoeud(ArbreVide(), i, ArbreVide());
        inserer(huff, arbre, TableOcc->tab[i]);
    }
    return huff;
}

Arbre ConstruireArbre(fap file) {
    Arbre dernier;
    while (!est_fap_vide(file)) {
        Arbre g, d;
        int pg, pd;
        extraire(file, &g, &pg);
        extraire(file, &d, &pd);
        Arbre noeud = NouveauNoeud(g, '\0', d);
        inserer(file, noeud, pd + pg);
        dernier = noeud;
    }
    return dernier;
}

void explorer(Arbre huff) {
    
}

void ConstruireCode(Arbre huff) {
    explorer(huff);
}

void Encoder(FILE *fic_in, FILE *fic_out, Arbre ArbreHuffman) {
    /* A COMPLETER */
    printf("Programme non realise (Encoder)\n");
}

int main(int argc, char *argv[]) {

    TableOcc_t TableOcc;
    FILE *fichier;
    FILE *fichier_encode;

    fichier = fopen(argv[1], "r");
    /* Construire la table d'occurences */
    ConstruireTableOcc(fichier, &TableOcc);
    fclose(fichier);

    /* Initialiser la FAP */
    fap file = InitHuffman(&TableOcc);

    /* Construire l'arbre d'Huffman */
    Arbre ArbreHuffman = ConstruireArbre(file);

        AfficherArbre(ArbreHuffman);

    /* Construire la table de codage */
    ConstruireCode(ArbreHuffman);

    /* Encodage */
    fichier = fopen(argv[1], "r");
    fichier_encode = fopen(argv[2], "w");

    Encoder(fichier, fichier_encode, ArbreHuffman);

    fclose(fichier_encode);
    fclose(fichier);

    return 0;
}
