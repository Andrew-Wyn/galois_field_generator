#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Monomy{
    int coefficient;
    int degree;
    struct Monomy *prev;
    struct Monomy *next;
} Monomy;

typedef struct Polinomy{
    struct Monomy *head;
    struct Monomy *tail;
} Polinomy;

int modulo;
int generator_prime_degree;

void        insert_element(Polinomy *polinomy, int coefficient, int degree);
void        print_polinomy(Monomy *monomy, int boo);
void        delete_last(Polinomy *polinomy);
int         moltiplicative_reverse(int num);
int         modularnegativitiator(int n);
void        sum_monomy_inpol(Polinomy *tosum, int coefficient, int degree);
Monomy      return_monomy_from_polinomy(Polinomy polinomy, int degree);
void        copy_polinomy(Polinomy *blank, Polinomy *tocopy);
void        destroy_polinomy(Monomy *head);
Polinomy    division_rest(Polinomy dividendo, Polinomy divisore);
Polinomy    sum_polinomy(Polinomy pol_1, Polinomy pol_2);
Polinomy    mult_polinomy_modulo(Polinomy pol_1, Polinomy pol_2, Polinomy generator_prime);
int         radix_pol(Polinomy pol);
void        field_generator(Polinomy field[]);
void        make_generator_polinomy(Polinomy *generator_prime);
int         get_degree_from_polinomy(Monomy *tail);
void        make_rest_n_degree(Polinomy *rest_n_degree, Polinomy *sub_modular_field, Polinomy generator_prime);
void        print_additive_matrix();
void        print_moltiplicative_matrix();
void        my_getchar(int get);
void        check_primality(Polinomy field[], Polinomy findirreduc, int *_is_prime);
void        print_from_file(char *filename);
int         is_null(Polinomy pol);
int         is_identity(Polinomy pol);
void        irreduxor(Polinomy field[]);
Polinomy    polinomy_mult_constant(Polinomy pol, int n);
void        degree_additive_calculator(Polinomy field[]);
void        degree_molteplicity_calculator(Polinomy field[], Polinomy generator_prime);

void insert_element(Polinomy *polinomy, int coefficient, int degree){
    Monomy *new = malloc(sizeof(struct Monomy));
    new -> coefficient = coefficient;
    new -> degree = degree;
    new -> prev = NULL;
    new -> next = NULL;
    
    if(polinomy->head == NULL){
        polinomy->head = new;
    }

    if(polinomy->tail != NULL){
        polinomy -> tail -> next = new;
        new -> prev = polinomy->tail;
    }
    
    polinomy->tail = new;
}

void print_polinomy(Monomy *monomy, int boo){
    if(monomy == NULL) return;
    if(monomy -> coefficient){
        if(monomy -> degree){
            if(boo){
                if(monomy -> coefficient > 1){
                    printf(" + %dX", monomy -> coefficient);
                } else {
                    printf(" + X");
                }
                if(monomy -> degree > 1){
                    printf("^%d", monomy -> degree);
                }
            } else {
                if(monomy -> coefficient > 1){
                    printf("%dX", monomy -> coefficient, monomy -> degree);
                } else {
                    printf("X");
                }
                if(monomy -> degree > 1){
                    printf("^%d", monomy -> degree);
                }
                boo = 1;
            }
        } else {
            if(boo){
                printf(" + %d", monomy -> coefficient);
            } else {
                printf("%d", monomy -> coefficient);
            }
        }
    } else {
        if(!boo && !(monomy -> degree)){
            printf("%d", monomy -> coefficient);
        }
    }
    print_polinomy(monomy->prev, boo);
    return;
}

void delete_last(Polinomy *polinomy){
    if(polinomy -> tail != NULL){
        Monomy *appo = polinomy -> tail -> prev;
        free(polinomy -> tail);
        if(appo != NULL){
            polinomy -> tail = appo;
            polinomy -> tail -> next = NULL;
        } else {
            polinomy -> head = NULL;
            polinomy -> tail = NULL;
        }
    }
}

int moltiplicative_reverse(int num){ //brutal force for dummies
    if(!num){ return 0;}
    for(int i=0; i<modulo; i++){
        if((num * i)%modulo == 1){
            return i;
        }
    }
}

int modularnegativitiator(int n){
    if(n < 0){
        while(0 > n){
            n = modulo + n;
        }
    } else {
        n = (n % modulo);
    }
    return n;
}

void sum_monomy_inpol(Polinomy *tosum, int coefficient, int degree){
    Monomy *appo = tosum -> head;
    while(tosum -> head -> degree != degree){
        tosum -> head = tosum -> head -> next;
    }

    tosum -> head -> coefficient = modularnegativitiator(tosum -> head -> coefficient + coefficient);
    tosum -> head = appo;
}

Monomy return_monomy_from_polinomy(Polinomy polinomy, int degree){
    Monomy ret;
    ret.coefficient = 0;
    while(polinomy.head != NULL){
        if(degree == polinomy.head->degree){
            ret = *(polinomy.head);
        }
        polinomy.head = polinomy.head -> next;
    }
    return ret;
}

void copy_polinomy(Polinomy *blank, Polinomy *tocopy){
    Monomy *appohead = tocopy -> head;
    blank->head = NULL;
    blank->tail = NULL;
    while(appohead != NULL){
        insert_element(blank, appohead->coefficient, appohead->degree);
        appohead = appohead -> next;
    }
    
}

void destroy_polinomy(Monomy *head){
    if(head == NULL) return;
    destroy_polinomy(head -> next);
    free(head);
}

Polinomy division_rest(Polinomy dividendo, Polinomy divisore){
    Polinomy appo_divisore;
    appo_divisore.head = NULL;
    appo_divisore.tail = NULL;
    copy_polinomy(&appo_divisore, &divisore);
    while(appo_divisore.tail->coefficient == 0){
        delete_last(&appo_divisore);
    }
    int degreedenom = appo_divisore.tail->degree;
    Polinomy *p_ret, ret;
    p_ret = &ret;
    copy_polinomy(p_ret, &dividendo);
    Monomy appo;
    appo.degree = 0;
    appo.coefficient = 0;
    while(p_ret -> tail -> degree >= appo_divisore.tail -> degree){
        appo.coefficient = p_ret -> tail -> coefficient * moltiplicative_reverse(appo_divisore.tail -> coefficient);
        appo.degree = p_ret -> tail -> degree - appo_divisore.tail -> degree;
        for(int i=degreedenom; i>=0; i--){
            Monomy appo2;
            appo2.coefficient = modularnegativitiator(-((return_monomy_from_polinomy(appo_divisore, i).coefficient * appo.coefficient)%modulo));
            appo2.degree = return_monomy_from_polinomy(appo_divisore, i).degree + appo.degree;
            sum_monomy_inpol(p_ret,appo2.coefficient, appo2.degree);
        }
        delete_last(p_ret);
    }
    destroy_polinomy(appo_divisore.head);
    return ret;
}

Polinomy sum_polinomy(Polinomy pol_1, Polinomy pol_2){
    Polinomy sum;
    sum.head = NULL;
    sum.tail = NULL;

    int max_degree;
    if(pol_1.tail -> degree >= pol_2.tail -> degree){
        max_degree = pol_1.tail -> degree;
    } else {
        max_degree = pol_2.tail -> degree;
    }

    for(int i=0; i<=max_degree; i++){
        int sum_coefficient = modularnegativitiator(return_monomy_from_polinomy(pol_1, i).coefficient + return_monomy_from_polinomy(pol_2, i).coefficient);
        insert_element(&sum, sum_coefficient, i);
    }

    return sum;
}

Polinomy mult_polinomy_modulo(Polinomy pol_1, Polinomy pol_2, Polinomy generator_prime){
    Polinomy mul;
    mul.head = NULL;
    mul.tail = NULL;

    int degree_mul;
    degree_mul = (pol_1.tail -> degree == pol_2.tail -> degree) ? pol_1.tail -> degree : 0;

    if(!degree_mul){
        printf("Errore! pol1 degree -> %d != pol2 degree -> %d", pol_1.tail -> degree, pol_2.tail -> degree);
        exit(EXIT_SUCCESS);
    }
    
    int appoMax = 0;

    for(int i=0; i<=degree_mul; i++){
        Polinomy appopol;
        appopol.head = NULL;
        appopol.tail = NULL;
        
        for(int j=0; j<=degree_mul; j++){
            Monomy appo;
            appo = return_monomy_from_polinomy(pol_1, i);
            appo.coefficient = return_monomy_from_polinomy(pol_2, j).coefficient * return_monomy_from_polinomy(pol_1, i).coefficient;
            appo.coefficient = modularnegativitiator(appo.coefficient);
            appo.degree = return_monomy_from_polinomy(pol_2, j).degree + return_monomy_from_polinomy(pol_1, i).degree;
            if(appo.degree > appoMax) appoMax = appo.degree;
            if(appopol.tail == NULL || appoMax == 0){    
                insert_element(&appopol,0, 0);
            }
            while(appopol.tail -> degree < appoMax){
                insert_element(&appopol,0, (appopol.tail -> degree)+1);
            }
            insert_element(&appopol,appo.coefficient, appo.degree);
        }
        if(mul.tail != NULL){
            mul = sum_polinomy(mul, appopol);
            destroy_polinomy(appopol.head); //avoid memory leak
        } else {
            mul = appopol;
        }
    }
    return division_rest(mul, generator_prime);
}

int radix_pol(Polinomy pol){
    for(int i=0; i<modulo; i++){
        int appo_radix = 0;
        Monomy *tocycle;
        tocycle = pol.tail;
        while(tocycle != NULL){
            appo_radix += tocycle -> coefficient * (pow((double)i, (double)tocycle -> degree));
            tocycle = tocycle->prev;
        }
        
        appo_radix = modularnegativitiator(appo_radix);

        if(!appo_radix){
            return i+1;
        }
    }
    return 0;
}

void print_field(Polinomy field[]){
    printf("\n\n\t\tCAMPO DI CARDINALITA' (%d)\n", (int)(pow((double)modulo, (double)generator_prime_degree)));
    for(int i=0; i<(pow((double)modulo, (double)generator_prime_degree)); i++){
        printf("\n");
        printf("\t\t");
        print_polinomy(field[i].tail, 0);
        printf("\n");
    }
}

void field_generator(Polinomy field[]){
    for(int i=0; i<(pow((double)modulo, (double)generator_prime_degree)); i++){
        field[i].head = NULL;
            field[i].tail = NULL;
        int coefficients[generator_prime_degree];  
        //initialize coefficients  
        int appo_coeff = i;
        for(int j=0; j<generator_prime_degree; j++){
            coefficients[j] = modularnegativitiator(appo_coeff);
            appo_coeff /= modulo;
        }
        for(int j=0; j<generator_prime_degree; j++){
            insert_element(field + i, coefficients[j], j);
        }
    }
    print_field(field);
}

void make_generator_polinomy(Polinomy *generator_prime){
        do{
            printf("Inserisci il grado del polinomio generatore n > 1\n");
            fflush(stdout);
            scanf("%d", &generator_prime_degree);
        } while(generator_prime_degree <= 1);

        for(int i=0; i<generator_prime_degree; i++){
            int coefficient;
            printf("Inserisci il coefficiente del termine a grado %d\n", i);
            fflush(stdout);
            scanf("%d", &coefficient);
            coefficient = modularnegativitiator(coefficient);
            insert_element(generator_prime, coefficient, i);
        }

        //avoid santo's mistere
        int coefficient;
        do{
            printf("Inserisci il coefficiente del termine a grado %d\n", generator_prime_degree);
            fflush(stdout);
            scanf("%d", &coefficient);
            coefficient = modularnegativitiator(coefficient);
        } while(!coefficient);
        insert_element(generator_prime, coefficient, generator_prime_degree);
}

int get_degree_from_polinomy(Monomy *tail){
    while(tail != NULL){
        if(tail->coefficient != 0){
            return tail->degree;
        } else {
            tail = tail->prev;
        }
    }
    return 0;
}

void make_rest_n_degree(Polinomy *rest_n_degree, Polinomy *sub_modular_field, Polinomy generator_prime){
    for(int i=0; i<=generator_prime_degree; i++){
        if(i == generator_prime_degree){
            insert_element(rest_n_degree, generator_prime.tail->coefficient, generator_prime.tail->degree);
        } else {
            insert_element(rest_n_degree, 0, i);
        }
    }

    *sub_modular_field = division_rest(*rest_n_degree, generator_prime);
}

void print_additive_matrix(Polinomy field[]){
    printf("\n\n\t\tTABELLA ADDITIVA DI CAMPO:\n\n");
    for(int i=0; i<(pow((double)modulo, (double)generator_prime_degree)); i++){
        printf("\t\t");
        for(int j=0; j<(pow((double)modulo, (double)generator_prime_degree)); j++){
                Polinomy appo;
                appo.head = NULL;
                appo.tail = NULL;
                appo = sum_polinomy(field[i], field[j]);
                print_polinomy(appo.tail, 0);
                if(j != (pow((double)modulo, (double)generator_prime_degree))-1) printf(" | ");
                destroy_polinomy(appo.head);
            }    
        printf("\n\n");
    }
}

void print_moltiplicative_matrix(Polinomy field[], Polinomy generator_prime){
    printf("\n\n\t\tTABELLA MOLTIPLICATIVA DI CAMPO:\n\n");   
    for(int i=0; i<(pow((double)modulo, (double)generator_prime_degree)); i++){
        printf("\t\t");
        for(int j=0; j<(pow((double)modulo, (double)generator_prime_degree)); j++){
                Polinomy appo;
                appo.head = NULL;
                appo.tail = NULL;
                appo = mult_polinomy_modulo(field[i],field[j], generator_prime);
                print_polinomy(appo.tail, 0);
                if(j != (pow((double)modulo, (double)generator_prime_degree))-1) printf(" | ");
                destroy_polinomy(appo.head);
            }    
        printf("\n\n");
    }
}

void my_getchar(int get){
    int c;
    while((c = getchar()) != '\n' && c != EOF);
    if(get) printf("\nPress enter to continue ...\n"), fflush(stdout), getchar();
}

void check_primality(Polinomy field[], Polinomy findirreduc, int *_is_prime){
    for(int i=modulo; i<(pow((double)modulo, (double)generator_prime_degree)); i++){
        if(get_degree_from_polinomy(field[i].tail) < get_degree_from_polinomy(findirreduc.tail)){
            Polinomy resto = division_rest(findirreduc, field[i]);
            if(!(return_monomy_from_polinomy(resto, get_degree_from_polinomy(resto.tail)).coefficient) && !get_degree_from_polinomy(resto.tail)){
                *_is_prime = 0;
                printf("polinomio riducibile over gf -> radice: ");
                print_polinomy(field[i].tail, 0);
                printf("\n");
            }
            destroy_polinomy(resto.head);
        }
    }
}

void print_from_file(char *filename){
    FILE *file = fopen(filename, "r");

    if(file == NULL){
        printf("Errore nell'apertura del file '%s'\n", filename);
        return;
    }

    char c;

    while((c=getc(file))!=EOF){
        printf("%c", c);
    }

    printf("\n\n");

    fclose(file);
}

int is_null(Polinomy pol){
    int sums;
    Polinomy appo_is_null;
    appo_is_null.head = NULL;
    appo_is_null.tail = NULL;

    copy_polinomy(&appo_is_null, &pol);

    while(appo_is_null.tail->coefficient == 0 && appo_is_null.tail->degree != 0){
        delete_last(&appo_is_null);
    }

    if(appo_is_null.tail->degree == 0 && appo_is_null.tail->coefficient == 0){
        destroy_polinomy(appo_is_null.head);
        return 1; 
    } else {
        destroy_polinomy(appo_is_null.head);
        return 0;
    }
}

int is_identity(Polinomy pol){
    int sums;
    Polinomy appo_is_null;
    appo_is_null.head = NULL;
    appo_is_null.tail = NULL;

    copy_polinomy(&appo_is_null, &pol);

    while(appo_is_null.tail->coefficient == 0 && appo_is_null.tail->degree != 0){
        delete_last(&appo_is_null);
    }

    if(appo_is_null.tail->degree == 0 && appo_is_null.tail->coefficient == 1){
        destroy_polinomy(appo_is_null.head);
        return 1; 
    } else {
        destroy_polinomy(appo_is_null.head);
        return 0;
    }
}

void irreduxor(Polinomy field[]){ //macro funzione per calcolare l'irriducibilità di un polinomio a coefficienti in Zp su GF(p^n)
    int _is_prime = 1;
    Polinomy findirreduc;
    findirreduc.head = NULL;
    findirreduc.tail = NULL;

    int findirreduc_degree = 0;

    do{
        printf("\nInserisci il grado del polinomio n >= 1\n");
        fflush(stdout);
        scanf("%d", &findirreduc_degree);
    } while(findirreduc_degree < 1);

    for(int i=0; i<findirreduc_degree; i++){
        int coefficient;
        printf("Inserisci il coefficiente del termine a grado %d\n", i);
        fflush(stdout);
        scanf("%d", &coefficient);
        coefficient = modularnegativitiator(coefficient);
        insert_element(&findirreduc, coefficient, i);
    }

    //avoid santo's mistere
    int coefficient;
    do{
        printf("Inserisci il coefficiente del termine a grado %d\n", findirreduc_degree);
        fflush(stdout);
        scanf("%d", &coefficient);
        coefficient = modularnegativitiator(coefficient);
    } while(!coefficient);
    insert_element(&findirreduc, coefficient, findirreduc_degree);

    printf("\n");
    print_polinomy(findirreduc.tail, 0);
    printf("\n");

    check_primality(field, findirreduc, &_is_prime);
    
    if(_is_prime){
        printf("\npolinomio irriducibile over gf\n");
    }

    destroy_polinomy(findirreduc.head);
}

Polinomy polinomy_mult_constant(Polinomy pol, int n){
    Polinomy mul;
    mul.head = NULL;
    mul.tail = NULL;

    Monomy *mon = pol.head;

    while(mon != NULL){
        insert_element(&mul, modularnegativitiator((mon->coefficient)*n), mon->degree);
        mon = mon->next;
    }
    return mul;
}

void degree_additive_calculator(Polinomy field[]){
    Polinomy pol_to_calc;
    pol_to_calc.head = NULL;
    pol_to_calc.tail = NULL;
    int to_calc;
    int field_card = pow((double)modulo, (double)generator_prime_degree); 
    do{
        print_field(field);
        printf("\nInserisci l'elemento del quale calcolare il grado additivo (1 - %d):\n", field_card-1);
        fflush(stdout);
        scanf("%d", &to_calc);
    } while(to_calc < 1 || to_calc >= field_card);
        
    pol_to_calc = field[to_calc];

    printf("\ncalcolare il grado additivo del polinomio: ");
    print_polinomy(pol_to_calc.tail, 0);
    printf(" ...\n\n");

    for(int i=1; 1; i++){
        Polinomy appo_to_calc = polinomy_mult_constant(pol_to_calc, i);
        if(is_null(appo_to_calc)){
            printf("Il grado additivo del polinomio è -> %d\n\n", i);
            printf("ciò implica che ("), print_polinomy(pol_to_calc.tail, 0), printf(") * %d è uguale a 0 sopra a GF(%d^%d)\n\n", i, modulo, generator_prime_degree);
            destroy_polinomy(appo_to_calc.head);
            return;
        }
        destroy_polinomy(appo_to_calc.head);
    }
}

void degree_molteplicity_calculator(Polinomy field[], Polinomy generator_prime){
    Polinomy pol_to_calc;
    pol_to_calc.head = NULL;
    pol_to_calc.tail = NULL;
    int to_calc;
    int field_card = pow((double)modulo, (double)generator_prime_degree); 
    do{
        print_field(field);
        printf("\nInserisci l'elemento del quale calcolare il grado moltiplicativo (2 - %d):\n", field_card-1);
        fflush(stdout);
        scanf("%d", &to_calc);
    } while(to_calc < 2 || to_calc >= field_card);
        
    pol_to_calc = field[to_calc];

    printf("\ncalcolare il grado moltiplicativo del polinomio: ");
    print_polinomy(pol_to_calc.tail, 0);
    printf(" ...\n\n");

    for(int i=1; 1; i++){
        fflush(stdout);
        Polinomy appo_to_calc;
        appo_to_calc.head = NULL;
        appo_to_calc.tail = NULL;

        copy_polinomy(&appo_to_calc, &pol_to_calc);

        for(int j=1; j<i; j++){
            appo_to_calc = mult_polinomy_modulo(pol_to_calc, appo_to_calc, generator_prime);
        }

        if(is_identity(appo_to_calc)){
            printf("Il grado moltiplicativo del polinomio è -> %d\n\n", i);
            printf("ciò implica che ("), print_polinomy(pol_to_calc.tail, 0), printf(")^%d è uguale a 1 sopra a GF(%d^%d)\n\n", i, modulo, generator_prime_degree);
            destroy_polinomy(appo_to_calc.head);
            return;
        }
        destroy_polinomy(appo_to_calc.head);
    }
}

int main(void){
    generator_prime_degree = 0;

    Polinomy generator_prime;
    generator_prime.head = NULL;
    generator_prime.tail = NULL;

    Polinomy rest_n_degree;
    rest_n_degree.head = NULL;
    rest_n_degree.tail = NULL;

    Polinomy sub_modular_field;
    sub_modular_field.head = NULL;
    sub_modular_field.tail = NULL;

    char choose;

    system("clear");
    print_from_file("title.txt");

    printf("Inserisci la cardinalita' del campo (primo)\nsul quale costruire l'anello dei polinomi:\n");
    fflush(stdout);
    scanf("%d", &modulo);

    //take irreducible polinomy, (generator polinomy)
    make_generator_polinomy(&generator_prime);

    printf("\n");
    print_polinomy(generator_prime.tail, 0);
    printf("\n");

    if(radix_pol(generator_prime)){
        printf("polinomio Riducibile !! radice (%d)\n", radix_pol(generator_prime)-1);
        exit(EXIT_SUCCESS);
    }

    //make the x^n reducible in modulo prime;
    make_rest_n_degree(&rest_n_degree, &sub_modular_field,generator_prime);

    printf("\n");
    printf("\ngeneratore: ");
    print_polinomy(generator_prime.tail, 0);
    printf("\ngrado massimo: ");
    print_polinomy(rest_n_degree.tail, 0);
    printf(" -> ");
    print_polinomy(sub_modular_field.tail, 0);
    printf("\n");

    //generare campo di polnomi
    Polinomy field[(int)(pow((double)modulo, (double)generator_prime_degree))];
    field_generator(field);

    my_getchar(0);

    printf("\n- Stampare le tabelle di additività e moltiplicazione ? (Y/n):\n");
    fflush(stdout);
    scanf("%c", &choose);

    if(choose == 'Y'){
        //stampa tabella additiva
        print_additive_matrix(field);

        //generare tabella moltiplicativa
        print_moltiplicative_matrix(field, generator_prime);

        my_getchar(1);
    }

    int boo = 1;
    do{
        //my_getchar(0);
        choose = 0;
        printf("\n- Operazioni possibili:\n\t1)calcolare irriducibilità\n\t2)calcolare grado additivo\n\t3)calcolare grado moltiplicativo\n\t4)esci\n");
        fflush(stdout);
        scanf("%d", &choose);
        switch(choose){
            case 1:
                irreduxor(field);
                break;
            case 2:
                degree_additive_calculator(field);
                break;
            case 3:
                degree_molteplicity_calculator(field, generator_prime);
                break;
            default:
                boo = 0;
                break;
        }
    } while(boo);
    
    return 0;
}