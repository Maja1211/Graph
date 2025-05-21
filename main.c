#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct cvor {
    double x, y, r;
} cvor;

typedef struct graf {
    int **Grane, BrojCvorova;
    cvor *Cvorovi;
} graf;

typedef struct red {
    int Velicina, Pocetak, Kraj;
    int *Niz;
} red;

red* kreiraj_red(int Velicina) {
    red *Red = malloc(sizeof(red));
    Red->Velicina = Velicina;
    Red->Pocetak = -1;
    Red->Kraj = -1;
    Red->Niz = malloc(Velicina * sizeof(int));
    return Red;
}

int je_prazan_red(red *Red) {
    return Red->Pocetak == -1;
}

int je_pun_red(red *Red) {
    return (Red->Kraj + 1) % Red->Velicina == Red->Pocetak;
}

void dodaj_u_red(red *Red, int Indeks) {
    if (je_pun_red(Red)) {
        printf("Red je pun!\n");
        exit(0);
    }

    Red->Kraj = (Red->Kraj + 1) % Red->Velicina;
    Red->Niz[Red->Kraj] = Indeks;

    if (Red->Pocetak == -1) {
        Red->Pocetak = 0;
    }
}

int ukloni_iz_reda(red *Red) {
    if (je_prazan_red(Red)) {
        printf("Red je prazan!\n");
        exit(0);
    }

    int Indeks = Red->Niz[Red->Pocetak];

    if (Red->Pocetak == Red->Kraj) {
        Red->Kraj = -1;
        Red->Pocetak = -1;
    } else {
        Red->Pocetak = (Red->Pocetak + 1) % Red->Velicina;
    }

    return Indeks;
}

void brisanje_reda(red *Red) {
    free(Red->Niz);
    free(Red);
}

graf* kreiranje_praznog_grafa(int BrojCvorova) {
    graf *Graf = malloc(sizeof(graf));
    Graf->BrojCvorova = BrojCvorova;

    Graf->Grane = malloc(BrojCvorova*sizeof(int*));
    for (int i = 0; i < BrojCvorova; i++) {
        Graf->Grane[i] = calloc(BrojCvorova, sizeof(int));
    }

    Graf->Cvorovi = calloc(BrojCvorova, sizeof(cvor));
    return Graf;
}

cvor* dodavanje_cvora(graf *Graf) {
    Graf->BrojCvorova++;

    Graf->Grane = realloc(Graf->Grane, Graf->BrojCvorova * sizeof(int*));
    for (int i = 0; i < Graf->BrojCvorova-1; i++) {
        Graf->Grane[i] = realloc(Graf->Grane[i], Graf->BrojCvorova * sizeof(int));
        Graf->Grane[i][Graf->BrojCvorova-1] = 0;
    }
    Graf->Grane[Graf->BrojCvorova-1] = calloc(Graf->BrojCvorova, sizeof(int));

    Graf->Cvorovi = realloc(Graf->Cvorovi, Graf->BrojCvorova * sizeof(cvor));
    return &Graf->Cvorovi[Graf->BrojCvorova-1];
}

void brisanje_cvora(graf *Graf, int Indeks) {
    if (Indeks >= Graf->BrojCvorova) {
        printf("Uneti indeks je van opsega!\n");
        return;
    }

    Graf->BrojCvorova--;

    free(Graf->Grane[Indeks]);
    for (int i = Indeks; i < Graf->BrojCvorova; i++) {
        Graf->Grane[i] = Graf->Grane[i+1];
    }
    Graf->Grane = realloc(Graf->Grane, Graf->BrojCvorova);

    for (int i = 0; i < Graf->BrojCvorova; i++) {
        for (int j = Indeks; j < Graf->BrojCvorova; j++) {
            Graf->Grane[i][j] = Graf->Grane[i][j+1];
        }
        Graf->Grane[i] = realloc(Graf->Grane[i], Graf->BrojCvorova);
    }

    for (int i = Indeks; i < Graf->BrojCvorova; i++) {
        Graf->Cvorovi[i] = Graf->Cvorovi[i+1];
    }
    Graf->Cvorovi = realloc(Graf->Cvorovi, Graf->BrojCvorova);
}

void dodavanje_grane(graf *Graf, int Indeks1, int Indeks2) {
    if (Indeks1 >= Graf->BrojCvorova || Indeks2 >= Graf->BrojCvorova) {
        printf("Uneti indeks je van opsega!\n");
        return;
    }

    Graf->Grane[Indeks1][Indeks2] = 1;
}

void uklanjanje_grane(graf *Graf, int Indeks1, int Indeks2) {
    if (Indeks1 >= Graf->BrojCvorova || Indeks2 >= Graf->BrojCvorova) {
        printf("Uneti indeks je van opsega!\n");
        return;
    }

    Graf->Grane[Indeks1][Indeks2] = 0;
}

void ispis_grafa(graf *Graf) {
    printf("    ");
    for (int i = 0; i < Graf->BrojCvorova; i++) {
        printf("%3d ", i);
    }
    printf("\n");

    for (int i = 0; i < Graf->BrojCvorova; i++) {
        printf("%3d ", i);
        for (int j = 0; j < Graf->BrojCvorova; j++) {
            printf("%3d ", Graf->Grane[i][j]);
        }
        printf("\n");
    }
}

void brisanje_grafa(graf *Graf) {
    for (int i = 0; i < Graf->BrojCvorova; i++) {
        free(Graf->Grane[i]);
    }
    free(Graf->Grane);
    free(Graf->Cvorovi);
    free(Graf);
}

int radijus_eksplozije(cvor Mina1, cvor Mina2) {
    return sqrt(pow(Mina1.x - Mina2.x, 2) + pow(Mina1.y - Mina2.y, 2)) <= Mina1.r;
}

int in(int *Niz, int DuzinaNiza, int Indeks) {
    for (int i = 0; i < DuzinaNiza; i++) {
        if (Niz[i] == Indeks) return 1;
    }
    return 0;
}

void efikasnost_mine(graf *Graf) {
    int Indeks;
    printf("Unesite indeks mine ciju efikasnost zelite da ispitate:\n");
    if (scanf("%d", &Indeks) == 0) {
        printf("Unesena vrednost nije validan broj!");
        return;
    }
    if (Indeks >= Graf->BrojCvorova) {
        printf("Uneti indeks je van opsega!\n");
        return;
    }

    int Obidjeni[Graf->BrojCvorova], Brojac = 0;
    red *Red = kreiraj_red(Graf->BrojCvorova-1);
    Obidjeni[Brojac++] = Indeks;
    dodaj_u_red(Red, Indeks);

    while (!je_prazan_red(Red)) {
        Indeks = ukloni_iz_reda(Red);

        for (int i = 0; i < Graf->BrojCvorova; i++) {
            if (Graf->Grane[Indeks][i] == 1 && !in(Obidjeni, Brojac, i)) {
                Obidjeni[Brojac++] = i;
                dodaj_u_red(Red, i);
            }
        }
    }
    brisanje_reda(Red);

    printf("Efikasnost mine je %d!\n", Brojac);
    printf("Detonirane mine su:\n");
    for (int i = 0; i < Brojac; i++) {
        printf("%d ", Obidjeni[i]);
    }
    printf("\n");
}

void najefikasnija_mina(graf *Graf) {
    int MaksEfikasnost = 0, Mina;

    for (int i = 0; i < Graf->BrojCvorova; i++) {
        int Obidjeni[Graf->BrojCvorova], Brojac = 0;
        red *Red = kreiraj_red(Graf->BrojCvorova-1);
        Obidjeni[Brojac++] = i;
        dodaj_u_red(Red, i);

        while (!je_prazan_red(Red)) {
            int Indeks = ukloni_iz_reda(Red);

            for (int j = 0; j < Graf->BrojCvorova; j++) {
                if (Graf->Grane[Indeks][j] == 1 && !in(Obidjeni, Brojac, j)) {
                    Obidjeni[Brojac++] = j;
                    dodaj_u_red(Red, j);
                }
            }
        }

        brisanje_reda(Red);

        if (Brojac > MaksEfikasnost) {
            Mina = i;
            MaksEfikasnost = Brojac;
        }
    }

    printf("Najefikasnija mina je %d. i ima efikasnost %d!\n", Mina, MaksEfikasnost);
}

void eksplozija_rakete(graf *Graf) {
    double x, y, r;
    printf("Unesite koordinate x y i radijus rakete:\n");
    if (scanf("%lf %lf %lf", &x, &y, &r) < 3) {
        printf("Unesena vrednost nije validan broj!\n");
        return;
    }
    if (r < 0) {
        printf("Uneta vrednost poluprecnika nije validna!\n");
        return;
    }

    cvor Raketa = {x, y, r};
    int Obidjeni[Graf->BrojCvorova], Brojac = 0;
    red *Red = kreiraj_red(Graf->BrojCvorova-1);

    for (int i = 0; i < Graf->BrojCvorova; i++) {
        if (radijus_eksplozije(Raketa, Graf->Cvorovi[i])) {
            Obidjeni[Brojac++] = i;
            dodaj_u_red(Red, i);
        }
    }

    while (!je_prazan_red(Red)) {
        int Indeks = ukloni_iz_reda(Red);

        for (int i = 0; i < Graf->BrojCvorova; i++) {
            if (Graf->Grane[Indeks][i] == 1 && !in(Obidjeni, Brojac, i)) {
                Obidjeni[Brojac++] = i;
                dodaj_u_red(Red, i);
            }
        }
    }
    brisanje_reda(Red);

    printf("Raketa je detonirala sledece mine:\n");
    for (int i = 0; i < Brojac; i++) {
        printf("%d ", Obidjeni[i]);
    }
    printf("\n");
}

graf* datoteka(){
    char ime[100];
    int brCvorova,i,j;
    printf("Unesite ime datoteke:");
    scanf("%s",ime);
    FILE *in = fopen(ime,"r");
    fscanf(in,"%d",&brCvorova);
    graf* Graf = kreiranje_praznog_grafa(brCvorova);
    cvor *cvorovi = Graf->Cvorovi;
    for (i=0;i<brCvorova;i++){
        fscanf(in,"%lf",&cvorovi[i].x);
        fscanf(in,"%lf",&cvorovi[i].y);
        fscanf(in,"%lf",&cvorovi[i].r);
    }
    for (i=0;i<brCvorova;i++){
        for (j=0;j<brCvorova;j++){
            if (radijus_eksplozije(cvorovi[i],cvorovi[j])){
                dodavanje_grane(Graf,i,j);
            }
        }
    }
    Graf->Cvorovi = cvorovi; Graf->BrojCvorova = brCvorova;
    ispis_grafa(Graf);
    return Graf;
}

int main() {
    int n, unos, a, b;
    graf* graph;
    while(1){
        printf("1) Napravite graf\n2) Dodajte cvor\n3) Dodajte granu\n4) Obrisite granu\n5) Obrisite cvor\n6) Ispisati graf\n7) Ucitajte mine iz datoteke i napravite graf\n8) Pronadjite efikasnost mine\n9) Pronadjite minu sa maksimalnom efikasnoscu\n10) Raketa\n11) Pronadjite povrsinu mine sa maksimalnom efikasnoscu\n12) Kraj\n");
        printf("Unesite vas izbor:");
        scanf("%d",&unos);
        if(unos<=0 || unos>=13) break;
        if(unos==1) {
            printf("Unesite broj cvorova:");
            scanf("%d",&n);
            graph = kreiranje_praznog_grafa(n);
            printf("Kreiran je prazan graf\n");
        }
        if(unos==2) {
            dodavanje_cvora(graph);
            printf("Dodat je prazan cvor\n");
        }
        if(unos==3) {
            printf("Od cvora:");
            scanf("%d",&a);
            printf("Do cvora:");
            scanf("%d",&b);
            dodavanje_grane(graph,a,b);
            printf("Dodata je grana izmedju %d. i %d. cvora\n",a,b);
        }
        if(unos==4){
            printf("Od cvora:");
            scanf("%d",&a);
            printf("Do cvora:");
            scanf("%d",&b);
            uklanjanje_grane(graph,a,b);
            printf("Uklonjena je grana izmedju %d. i %d. cvora\n",a,b);
        }
        if(unos==5) {
            int index;
            printf("Unesite index cvora koji zelite da brisete:");
            scanf("%d",&index);
            brisanje_cvora(graph,index);
            printf("Cvor %d je izbrisan\n",index);
        }
        if(unos==6) {
            ispis_grafa(graph);
        }
        if(unos==7) {
            graph = datoteka();
        }
        if(unos==8) {
            efikasnost_mine(graph);
        }
        if(unos==9) {
            najefikasnija_mina(graph);
        }
        if(unos==10) {
            eksplozija_rakete(graph);
        }
        if(unos==11) {
            printf("Stavka nije dostupna!\n");
        }
        if(unos==12) {
            brisanje_grafa(graph);
            exit(0);
        }
    }
    return 0;
}