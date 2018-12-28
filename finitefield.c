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

Polinomy division_rest(Polinomy *dividendo, Polinomy divisore){
    while(divisore.tail->coefficient == 0){
        delete_last(&divisore);
    }
    int degreedenom = divisore.tail->degree;
    Polinomy *p_ret, ret;
    p_ret = &ret;
    copy_polinomy(p_ret, dividendo);
    Monomy appo;
    appo.degree = 0;
    appo.coefficient = 0;
    while(p_ret -> tail -> degree >= divisore.tail -> degree){
        appo.coefficient = p_ret -> tail -> coefficient * moltiplicative_reverse(divisore.tail -> coefficient);
        appo.degree = p_ret -> tail -> degree - divisore.tail -> degree;
        for(int i=degreedenom; i>=0; i--){
            Monomy appo2;
            appo2.coefficient = modularnegativitiator(-((return_monomy_from_polinomy(divisore, i).coefficient * appo.coefficient)%modulo));
            appo2.degree = return_monomy_from_polinomy(divisore, i).degree + appo.degree;
            sum_monomy_inpol(p_ret,appo2.coefficient, appo2.degree);
        }
        delete_last(p_ret);
    }
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

Polinomy mult_polinomy_modulo(Polinomy pol_1, Polinomy pol_2, Polinomy generator_prime){ //optimize
    Polinomy mul;
    mul.head = NULL;
    mul.tail = NULL;

    int max_degree, min_degree;
    Polinomy maxdegreepol;
    maxdegreepol.head = NULL;
    maxdegreepol.tail = NULL;
    Polinomy mindegreepol;
    mindegreepol.head = NULL;
    mindegreepol.tail = NULL;

    if(pol_1.tail -> degree >= pol_2.tail -> degree){
        max_degree = pol_1.tail -> degree;
        maxdegreepol = pol_1;
        mindegreepol = pol_2;
        min_degree = pol_2.tail -> degree;
    } else {
        max_degree = pol_2.tail -> degree;
        maxdegreepol = pol_2;
        mindegreepol = pol_1;
        min_degree = pol_1.tail -> degree;
    }

    int appoMax = 0;



    for(int i=0; i<=max_degree; i++){
        Polinomy appopol;
        appopol.head = NULL;
        appopol.tail = NULL;
        
        for(int j=0; j<=min_degree; j++){
            Monomy appo;
            appo = return_monomy_from_polinomy(maxdegreepol, i);
            appo.coefficient = return_monomy_from_polinomy(mindegreepol, j).coefficient * return_monomy_from_polinomy(maxdegreepol, i).coefficient;
            appo.coefficient = modularnegativitiator(appo.coefficient);
            appo.degree = return_monomy_from_polinomy(mindegreepol, j).degree + return_monomy_from_polinomy(maxdegreepol, i).degree;
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
        } else {
            mul = appopol;
        }
        
    }
    return division_rest(&mul, generator_prime);
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

void field_generator(Polinomy field[], int generator_prime_degree){
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

    printf("\n\n\t\tCAMPO DI CARDINALITA' (%d)\n", (int)(pow((double)modulo, (double)generator_prime_degree)));
    for(int i=0; i<(pow((double)modulo, (double)generator_prime_degree)); i++){
        printf("\n");
        printf("\t\t");
        print_polinomy(field[i].tail, 0);
        printf("\n");
    }
}

int main(void){

    int generator_prime_degree = 0;

    Polinomy generator_prime;
    generator_prime.head = NULL;
    generator_prime.tail = NULL;

    Polinomy second;
    second.head = NULL;
    second.tail = NULL;

    printf("Inserisci la cardinalita' del campo (primo)\nsul quale costruire l'anello dei polinomi: \n");
    fflush(stdout);
    scanf("%d", &modulo);

    //take irreducible polinomy, (generator polinomy)
    printf("Inserisci il grado del polinomio generatore\n");
    fflush(stdout);
    scanf("%d", &generator_prime_degree);

    for(int i=0; i<=generator_prime_degree; i++){
        int coefficient;
        printf("Inserisci il coefficiente del termine a grado %d\n", i);
        fflush(stdout);
        scanf("%d", &coefficient);
        coefficient = modularnegativitiator(coefficient);
        insert_element(&generator_prime, coefficient, i);
    }

    printf("\n");
    print_polinomy(generator_prime.tail, 0);
    printf("\n");

    if(radix_pol(generator_prime)){
        printf("polinomio Riducibile !! radice (%d)\n", radix_pol(generator_prime)-1);
        exit(EXIT_SUCCESS);
    }

    //make the x^n reducible in modulo prime;
    for(int i=0; i<=generator_prime_degree; i++){
        if(i == generator_prime_degree){
            insert_element(&second, generator_prime.tail->coefficient, generator_prime.tail->degree);
        } else {
            insert_element(&second, 0, i);
        }
    }

    Polinomy sub_modular_field= division_rest(&second, generator_prime);

    printf("\n");
    printf("\ngeneratore: ");
    print_polinomy(generator_prime.tail, 0);
    printf("\ngrado massimo: ");
    print_polinomy(second.tail, 0);
    printf(" -> ");
    print_polinomy(sub_modular_field.tail, 0);
    printf("\n");

    //generare campo di polnomi
    Polinomy field[(int)(pow((double)modulo, (double)generator_prime_degree))];
    field_generator(field, generator_prime_degree);

    //generare tabella addittiva
    printf("\n\n\t\tTABELLA ADDITTIVA DI CAMPO:\n\n");
    for(int i=0; i<(pow((double)modulo, (double)generator_prime_degree)); i++){
        printf("\t\t");
        for(int j=0; j<(pow((double)modulo, (double)generator_prime_degree)); j++){
                print_polinomy(sum_polinomy(field[i], field[j]).tail, 0);
                if(j != (pow((double)modulo, (double)generator_prime_degree))-1) printf(" | ");
            }    
        printf("\n\n");
    }

    //generare tabella moltiplicativa
    printf("\n\n\t\tTABELLA MOLTIPLICATIVA DI CAMPO:\n\n");   
    for(int i=0; i<(pow((double)modulo, (double)generator_prime_degree)); i++){
        printf("\t\t");
        for(int j=0; j<(pow((double)modulo, (double)generator_prime_degree)); j++){
                print_polinomy(mult_polinomy_modulo(field[i],field[j], generator_prime).tail, 0);
                if(j != (pow((double)modulo, (double)generator_prime_degree))-1) printf(" | ");
            }    
        printf("\n\n");
    }

    printf("Press enter to close ...\n");
    fflush(stdout);
    int c;
    while((c = getchar()) != '\n' && c != EOF);
    getchar();


    return 0;
}