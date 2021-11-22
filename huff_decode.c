#include <stdio.h>

#include "arbrebin.h"
#include "assert.h"
#include "bfile.h"

void Decoder(FILE *fichier_encode, Arbre ArbreHuffman) {
    //AfficherArbre(ArbreHuffman);

	BFILE *bfic = bstart(fichier_encode, "r");
	Arbre actuel = ArbreHuffman;
	while (!beof(bfic)) {
		int code = bitread(bfic);
		if (code == 0) {
			actuel = FilsGauche(actuel);
		} else {
			actuel = FilsDroit(actuel);
		}

		if (EstVide(FilsGauche(actuel)) || EstVide(FilsDroit(actuel))) {
			printf("%c", Etiq(actuel));
			actuel = ArbreHuffman;
		}
	} 
	printf("\n");
	bstop(bfic);
  LibererArbre(ArbreHuffman);
}

int main(int argc, char *argv[]) {
    FILE *fichier_encode = fopen(argv[1], "r");

    Arbre ArbreHuffman = LireArbre(fichier_encode);

    Decoder(fichier_encode, ArbreHuffman);

    fclose(fichier_encode);

    return 0;
}
