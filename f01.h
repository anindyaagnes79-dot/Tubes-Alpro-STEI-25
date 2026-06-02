#ifndef F01
#define F01

#define IDX_UNDEF -1
#include "f00.h"

unsigned int AcakLCG(){
    static unsigned int angka = 16525;
    unsigned long long A=67676767;
    unsigned long long C=1322518325;
    unsigned long long M=414141414;
    angka=(A*angka+C)%M;
    return angka;
}
void Discover(ListHalaman_Web *l){
    if(isEmpty(*l)==true){
        printf ("ERROR, TIDAK ADA WEB DI DATABASE");
        return;
    }
    else if ((l->nEff)<5){
    	int i;
        for (i=0; i<l->nEff; i++){
            printf ("- %s", l->HW[i].web_url);
            if (i < l->nEff - 1) {
                printf(", ");
            }
        }
    }
    else{
        int index_acak[MAX_WEB_PAGES];
        int i;
        for (i=0; i<l->nEff; i++){
            index_acak[i]=i;
        }
        for (i=l->nEff-1; i > 0; i--) {
            int j=AcakLCG() % (i + 1);
            int temp=index_acak[i];
            index_acak[i]=index_acak[j];
            index_acak[j]=temp;
        }
        for (i = 0; i < 5; i++) {
            printf("- %s", l->HW[index_acak[i]].web_url);
            if (i < 4) {
                printf(", ");
            }
        }
    }
    printf ("\n");
}

#endif
