#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define CARDVALUES 13
#define LANDS 4
#define MAX 100

//pelaaja tietää nimensä, voittonsa ja korttinsa
struct player {
	char name[MAX];
	int wins;
	char card[5];
};

//tasapelit ja kierroksen voittaja
struct gameInfo {
	char roundInfo[MAX];
	int ties;
};

//kysytään pelaajien nimet ja tallennetaan tietueisiin
void askInfo(struct player *p1, struct player *p2) {
	printf("give Player1 name\n");
	scanf("%s", p1->name);
	printf("give Player2 name\n");
	scanf("%s", p2->name);
}

//alustetaan cards taulukko sisältämään kaikki kortit
//callocilla varataan tarpeeksi monta bittiä jokaisen kortin merkkijonolle
//callocilla varataan 4 bittiä, koska suurin kortti on 3 merkkiä ja nullcharacter
void setup(char *cards[LANDS][CARDVALUES]) {
	char *maat[4] = { "A","B","C","D" };
	for (int i = 0; i < CARDVALUES; i++) {
		for (int j = 0; j < LANDS; j++) {
			cards[j][i] = calloc(4, sizeof(char));
			sprintf(cards[j][i], "%s%d ", maat[j], i + 1);
		}
	}
}

//tulostetaan kortit ja verrataan kumpi on isompi, printataan voittajan nimi ja kortti, kasvatetaan voittajapelaajan voittolaskuria
//annetaan tulostettava merkkijono gameInfo tyypin tietueelle
//tulostetaan myös pelaajien voitot ja tasapelit
void cardComparison(int a, int b, struct player *p1, struct player *p2, struct gameInfo *info) {
	printf("%s got %sand %s got %s\n", p1->name, p1->card, p2->name, p2->card);
	if (a != 0 && b != 0) {
		if (a > b) {
			printf("%s won \n", p1->name);
			sprintf(info->roundInfo, "%s has won this round with %s", p1->name, p1->card);
			p1->wins++;
		}
		else if (b > a) {
			printf("%s won\n", p2->name);
			sprintf(info->roundInfo, "%s has won this round with %s", p2->name, p2->card);
			p2->wins++;
		}
		else if (a == b) {
			printf("tie\n");
			sprintf(info->roundInfo, "This round ended in tie");
			info->ties++;
		}
	}
	else if (a == 0 || b == 0) {
		if (a == 0 && b != 0) {
			printf("%s won \n", p1->name);
			sprintf(info->roundInfo, "%s has won this round with %s", p1->name, p1->card);
			p1->wins++;
		}
		else if (b == 0 && a != 0) {
			printf("%s won\n", p2->name);
			sprintf(info->roundInfo, "%s has won this round with %s", p2->name, p2->card);
			p2->wins++;
		}
		else if (a == 0 && b == 0) {
			printf("tie\n");
			sprintf(info->roundInfo, "This round ended in tie");
			info->ties++;
		}
	}
	printf("%s : %d wins\n%s : %d wins \nties: %d\n", p1->name, p1->wins, p2->name, p2->wins, info->ties);
}

//arvotaan kortti, verrataan ettei korttia ole jo käytetty strcmp funktiolla, arvotaan toisen pelaajan kortti, mikä ei saa olla sama
//1. kortin kanssa ja tarkistetaan ettei korttia ole käytetty aikaisemmilla kierroksilla, kutsutaan cardComparison funtiota
//arvotut kortit merkataan käytetyiksi "X " merkkijonolla
void chooseCards(char *cards[LANDS][CARDVALUES], struct player *p1, struct player *p2, struct gameInfo *info) {
	int value1, land1, value2, land2, equal, running = 1;
	while (running == 1) {
		value1 = rand() % CARDVALUES;
		land1 = rand() % LANDS;
		equal = strcmp(cards[land1][value1], "X ");
		if (equal != 0) {
			strcpy(p1->card, cards[land1][value1]);
			running = 0;
		}
	}
	running = 1;
	while (running == 1) {
		value2 = rand() % CARDVALUES;
		land2 = rand() % LANDS;
		equal = strcmp(cards[land2][value2], "X ");
		if (equal != 0 && (value2 != value1 || land2 != land1)) {
			strcpy(p2->card, cards[land2][value2]);
			running = 0;
		}
	}
	cardComparison(value1, value2, p1, p2, info);
	strcpy(cards[land1][value1], "X ");
	strcpy(cards[land2][value2], "X ");
}

//funcktio tulostaa kaikki kortit
void showAll(char *cards[LANDS][CARDVALUES]) {
	printf("remaining cards:\n");
	for (int i = 0; i < LANDS; i++) {
		for (int j = 0; j < CARDVALUES; j++) {
			printf("%s", cards[i][j]);
		}
		printf("\n");
	}
}

//funcktio kysyy haluaako pelaaja pelata uudestaan, pelaaja joko pelaa uudestaan tai lopettaa pelin
//palauttaa haluaako pelaaja pelata uudestaan
int playAgain() {
	char mark = 0;
	int keepPlaying = 0;
	int running = 1;
	printf("play again k/e \n");
	while (running == 1) {
		while ((mark = getchar()) != '\n') {
			if (mark == 'e') {
				printf("you didn't want to play again \n");
				keepPlaying = 0;
				running = 0;
			}
			else if (mark == 'k') {
				printf("you want to play again \n");
				keepPlaying = 1;
				running = 0;
			}
			else {
				printf("bad input \n");
			}
		}
	}
	return keepPlaying;
}

//funktio tarkistaa onko kortteja enää jäljellä pakassa
//palauttaa 0, jos ei ole kortteja ja 1, jos on kortteja
int cardsRemaining(char *cards[LANDS][CARDVALUES]) {
	int returnValue = 0;
	int equal;
	for (int i = 0; i < LANDS; i++) {
		for (int j = 0; j < CARDVALUES; j++) {
			equal = strcmp(cards[i][j], "X ");
			if (equal != 0) {
				returnValue = 1;
			}
		}
	}
	if (returnValue == 0) {
		system("cls");
		printf("deck ran out out cards\n");
	}
	return returnValue;
}

//funktio yhdistää tietuieden merkkijonot yhdeksi merkkijonoksi ja se tallennetaan fileen,
//viimeisellä kerralla kun funktiota kutsutaan, fileen tallennetaan kuka voitti koko pelin ja millä tuloksella
void save(FILE **fPointer, struct player p1, struct player p2, struct gameInfo info, int lastTime) {
	char text[MAX];
	if (lastTime == 0) {
		sprintf(text, "%s drew %s%s drew %s\n%s\n%s has %d wins, %s has %d wins and there has been %d ties\n\n", p1.name, p1.card, p2.name, p2.card, info.roundInfo, p1.name, p1.wins, p2.name, p2.wins, info.ties);
	}
	else {
		if (p1.wins > p2.wins) {
			sprintf(text, "Congratulations to %s for winning %s\nwith total score of %d wins %d loses and %d ties", p1.name, p2.name, p1.wins, p2.wins, info.ties);
		}
		else if (p2.wins > p1.wins) {
			sprintf(text, "Congratulations to %s for winning %s\nwith total score of %d wins %d loses and %d ties", p2.name, p1.name, p2.wins, p1.wins, info.ties);
		}
		else if (p1.wins == p2.wins) {
			sprintf(text, "This game has ended in tie between %s and %s\nwith both players winning %d times", p1.name, p2.name, p1.wins);
		}
	}
	fprintf(*fPointer, text);
}

//luodaan kaksi pelaaja tietuetta ja yksi gameInfo tietue, alustatee 0 voittoa ja 0 tasapeliä
//srand käynnistää satunnaisnumerogeneroinnin, riippuvainen ajasta
//luodaan filepointer ja merkkijono
//pelataan, kunnes kortit ovat loppu tai pelaaja ei enää halua pelata
int main() {
	struct player p1;
	struct player p2;
	struct gameInfo info;
	p1.wins = 0;
	p2.wins = 0;
	info.ties = 0;
	srand(time(NULL));
	FILE *fPointer;
	fPointer = fopen("logs.txt", "w");
	char *cards[LANDS][CARDVALUES];
	askInfo(&p1, &p2);
	setup(cards);
	do {
		system("cls");
		chooseCards(cards, &p1, &p2, &info);
		showAll(cards);
		save(&fPointer, p1, p2, info, 0);
	} while (playAgain() == 1 && cardsRemaining(cards) == 1);
	save(&fPointer, p1, p2, info, 1);
	fclose(fPointer);
	return 0;
}
