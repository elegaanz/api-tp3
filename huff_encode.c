

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
	for (int i = 0; i < 256; i++) TableOcc->tab[i] = 0;

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
      huff = inserer(huff, arbre, TableOcc->tab[i]);
  }
  return huff;
}

Arbre ConstruireArbre(fap file) {
  Arbre dernier;
  for (int i = 0; i < 255; i++) {
      Arbre g, d;
      int pg, pd;
      file = extraire(file, &g, &pg);
      file = extraire(file, &d, &pd);
      Arbre noeud = NouveauNoeud(g, '\0', d);
      file = inserer(file, noeud, pd + pg);
  }
	int x;
  extraire(file, &dernier, &x);
  return dernier;
}

void explorer(Arbre huff, int lg, int *code) {
  if (EstVide(FilsGauche(huff)) || EstVide(FilsDroit(huff))) {
		unsigned char ch = Etiq(huff);
		HuffmanCode[(int)ch].lg = lg;
		for (int i = 0; i < lg; i++) {
			HuffmanCode[(unsigned int)ch].code[i] = code[i];
		}
	} else {
		code[lg] = 0;
		explorer(FilsGauche(huff), lg + 1, code);
		code[lg] = 1;
		explorer(FilsDroit(huff), lg + 1, code);
	}
}

void ConstruireCode(Arbre huff) {
	int code[32];
  explorer(huff, 0, &*code);
}

void Encoder(FILE *fic_in, FILE *fic_out, Arbre ArbreHuffman) {
  EcrireArbre(fic_out, ArbreHuffman);
	BFILE *bfic_out = bstart(fic_out, "w");
	while (!feof(fic_in)) {
		unsigned char c;
		fscanf(fic_in, "%c", &c);
		struct code_char *code = &HuffmanCode[(unsigned int)c];
		for (int i = 0; i < code->lg; i++) {
			bitwrite(bfic_out, code->code[i]);
		}
	} 
	bstop(bfic_out);
	LibererArbre(ArbreHuffman);
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
