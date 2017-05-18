//
//  LibClientFunctions.c
//  LabCMarketClient
//
//  Created by Guilherme Guia on 3/27/17.
//  Copyright © 2017 Guilherme Guia. All rights reserved.
//

#include "LibClientFunctions.h"


char* itoa(int val, int base){
    
    static char buf[32] = {0};
    
    int i = 30;
    
    for(; val && i ; --i, val /= base)
        
        buf[i] = "0123456789abcdef"[val % base];
    
    return &buf[i+1];
    
}

int connToServer(){
    int sock;
    struct sockaddr_in server;
    
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Erro ao criar o socket");
    }
    puts("Socket criado com sucesso!");
    
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET; //TCP
    server.sin_port = htons( 8888 );
    
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("Erro ao conectar.");
        return 1;
    }
    
    puts("Conectado!\n");
    return sock;
}

Carts *createCart(){
    return NULL;
}

//function to write the string to the server and get the answer.
void writeToServer(int sock, char message[], char server_reply[]){
    
    memset(server_reply,0,STR_MAX_SIZE);
    
    if( send(sock , message , strlen(message) , 0) < 0)
    {
        puts("Send failed");
        exit(1);
    }
    
    if( recv(sock , server_reply , STR_MAX_SIZE , 0) < 0)
    {
        puts("recv failed");
        exit(1);
    }
    
}

//Print the balance from the users account
void manegeBalance(char server_replay[]){
    int i=0,j=0;
    char name[STR_MAX_SIZE],balance[STR_MAX_SIZE];
    float bal=0;
    memset(name,0,STR_MAX_SIZE);
    memset(balance,0,STR_MAX_SIZE);
    
    while(server_replay[i] != ':'){
        name[i] = server_replay[i];
        i++;
    }
    i++;
    while(server_replay[i] != ':'){
        balance[j] = server_replay[i];
        i++;
        j++;
    }
    printf("\t%s","** Saldo **\n");
    printf("Nome: %s",name);
    printf("\n");
    bal = atof(balance);
    printf("Saldo: %.2f",bal);
    printf("\n");
    printf("\t%s","** Fim Saldo **\n\n");
    memset(server_replay,0,STR_MAX_SIZE);
}

//Display options to the user to add more money
void showManageOptions(int sock, char username[], char server_replay[]){
    int option=0;
    char deposit[STR_MAX_SIZE];
    float value=0;
    char msg[STR_MAX_SIZE];
    memset(deposit,0,STR_MAX_SIZE);
    memset(msg,0,STR_MAX_SIZE);
    
    printf("1) Adicionar mais dinheiro\n");
    printf("2) Voltar ao menu anterior\n");
    scanf("%d",&option);
    
    while(option != 2){
        if(option == 1){
            option = 2;
            system("clear");
            printf("Digite a quantidade que deseja depositar: \n");
            scanf("%f",&value);
            strcpy(msg,"4:");
            strcat(msg,username);
            strcat(msg,":");
            snprintf(deposit,STR_MAX_SIZE,"%f",value);
            strcat(msg,deposit);
            strcat(msg,":");
            writeToServer(sock,msg,server_replay);
        }else if(option == 2){
            return;
        }else{
            printf("Opcao nao reconhecida.\n");
            printf("Tente novamente: \n");
            scanf("%d",&option);
        }
    }
    
    memset(server_replay,0,STR_MAX_SIZE);
}

//List all products in the stock
void listProducts(int sock, char server_replay[], int codigos[], int *qtdProduct){
    int i=0,j=0,k=0,qtd;
    char msg[STR_MAX_SIZE],tmp[STR_MAX_SIZE];
    memset(msg,0,STR_MAX_SIZE);
    memset(tmp,0,STR_MAX_SIZE);
    strcpy(msg,"5:");
    
    writeToServer(sock,msg,server_replay);
    
    printf("\t\t\t** Lista dos produtos **\n");
    printf("%6s","Produto");
    printf("%14s","Descricao");
    printf("%10s","Codigo");
    printf("%15s","Quantidade");
    printf("%10s","Preco");
    printf("\n----------------------------------------------------------------------\n");
    while(server_replay[i] != '\0'){
        while(server_replay[i] != ':'){//print the name
            tmp[j] = server_replay[i];
            i++;
            j++;
        }
        j=0;
        i++;
        printf("%6s",tmp);
        memset(tmp,0,STR_MAX_SIZE);
        while(server_replay[i] != ':'){//print the description
            tmp[j] = server_replay[i];
            i++;
            j++;
        }
        i++;
        j=0;
        printf("%14s",tmp);
        memset(tmp,0,STR_MAX_SIZE);
        while(server_replay[i] != ':'){// print the code
            tmp[j] = server_replay[i];
            i++;
            j++;
        }
        i++;
        j=0;
        codigos[k] = atoi(tmp);
        k++;
        printf("%10s",tmp);
        memset(tmp,0,STR_MAX_SIZE);
        while(server_replay[i] != ':'){//print the quantty
            tmp[j] = server_replay[i];
            i++;
            j++;
        }
        i++;
        j=0;
        qtd = atoi(tmp);
        printf("%13d",qtd);
        memset(tmp,0,STR_MAX_SIZE);
        while(server_replay[i] != ':'){//print the price
            tmp[j] = server_replay[i];
            i++;
            j++;
        }
        j=0;
        
        printf("\t   ");
        while(j <= 3){//Show something like 1.00 and not 1.000000
            printf("%c",tmp[j]);
            j++;
        }
        //printf("%14s",tmp);
        printf("\n----------------------------------------------------------------------\n");
        i++;
        j=0;
        memset(tmp,0,STR_MAX_SIZE);
    }
    printf("\n");
    printf("\t\t\t**Fim da lista de produtos**\n");
    *qtdProduct = k;
    memset(server_replay,0,STR_MAX_SIZE);
    return;
}

//Verify the number of the products in the cart.
int checkProductList(int qtdProduct, int lst[], int key){
    int i;
    
    for(i=0;i<qtdProduct;i++){
        if(lst[i] == key){
            return 1;
        }
    }
    return 0;
}

//Search for a product in the cart
Carts *searchCart(Carts *lst, int code){
    Carts *c;
    
    for(c = lst; c!= NULL; c = c->next){
        if(c->cod == code){
            return c;
        }
    }
    return NULL;
}

//Add a product to the cart
Carts *addProduct2Cart(int sock,char server_reply[] , Carts *lst, int code, int qtd){
    char msg[STR_MAX_SIZE];
    memset(msg,0,STR_MAX_SIZE);
    Carts *c;
    c = searchCart(lst,code);
    if(c != NULL){//Verify if the product already exist in the cart
        c->qtd += qtd;
        return c;
    }
    
    
    strcpy(msg,"6:");
    strcat(msg,itoa(code,10));
    strcat(msg,":");
    strcat(msg,itoa(qtd,10));
    strcat(msg,":");
    
    writeToServer(sock,msg,server_reply);
    if(strcmp(server_reply,"1") == 0){
        Carts *novo = (Carts*) malloc(sizeof(Carts));
        
        novo->cod = code;
        novo->qtd = qtd;
        novo->next = lst;
        printf("Produto adicionado com sucesso!\n");
        return novo;
    }else{
        printf("Nao pode adicionar mais do que esta a venda.\n");
    }
    memset(server_reply,0,STR_MAX_SIZE);
    return NULL;
}

//Show products in the cart
float listProductInCart(Carts *lst, int sock, char server_reply[]){
    int i=0,j=0,k=0,qtdC[200];
    Carts *c;
    float precos[200],gasto=0;
    char msg[STR_MAX_SIZE];
    char nome[100],preco[10];
    memset(msg,0,STR_MAX_SIZE);
    memset(nome,0,100);
    memset(preco,0,10);
    
    printf("\t**Lista dos produtos no carrinho**\n");
    printf("%6s","Produto");
    printf("%10s","Codigo");
    printf("%15s","Quantidade");
    printf("%10s","Preco");
    printf("\n----------------------------------------------------------------------\n");
    for(c = lst;c!=NULL;c = c->next){
        memset(msg,0,STR_MAX_SIZE);
        memset(preco,0,10);
        memset(nome,0,100);
        memset(server_reply,0,STR_MAX_SIZE);
        strcpy(msg,"7:");
        strcat(msg,itoa(c->cod,10));
        strcat(msg,":");
        writeToServer(sock,msg,server_reply);//Msg to get the name and the price of the product.
        i=0;
        j=0;
        while(server_reply[i] != ':'){
            nome[i] = server_reply[i];
            i++;
        }
        i++;
        while(server_reply[i] != ':'){
            preco[j] = server_reply[i];
            i++;
            j++;
        }
        precos[k] = atof(preco);
        qtdC[k] = c->qtd;
        k++;
        printf("%6s",nome);
        printf("%10d",c->cod);
        printf("%10d",c->qtd);
        printf("%15s",preco);
        printf("\n----------------------------------------------------------------------\n");
    }
    
    for(i=0;i<k;i++){//Calculate all the money that will expand
        gasto = (precos[i] * qtdC[i]) + gasto;
    }
    
    printf("\n** Total gasto ate o momento: %.2f **\n",gasto);
    
    printf("\n\t\t\t**Fim da lista do carrinho**\n");
    memset(server_reply,0,STR_MAX_SIZE);
    return gasto;
}

//Verify if the user has the money, and subtract it.
Carts *checkout(Carts *lst, char username[], char server_reply[], int sock, float total, int cod){
    char msg[STR_MAX_SIZE];
    memset(msg,0,STR_MAX_SIZE);
    Carts *c = lst;   //Cart
    Carts *a = NULL;   //Previus cart
    
    strcpy(msg,"8:");
    strcat(msg,username);
    strcat(msg,":");
    strcat(msg,itoa(total,10));
    strcat(msg,":");
    strcat(msg,itoa(cod,10));
    strcat(msg,":");
    
    writeToServer(sock,msg,server_reply);
    
    if(strcmp(server_reply,"1") == 0){
        printf("Compra finalizada com sucesso!\n");
    }else{
        printf("Dinheiro insuficiente.\n");
        return lst;
    }
    
    while(c != NULL && c->cod != cod){
        a = c;
        c = c->next;
    }
    
    if(a == NULL){
        lst = c->next;
    }else{
        a->next=c->next;
    }
    free(c);
    
    memset(server_reply,0,STR_MAX_SIZE);
    
    return lst;
}

//Remove a product from the cart.
Carts *removeProductFromCart(Carts *lst, int code, int sock, char server_reply[]){
    char msg[STR_MAX_SIZE];
    
    if(lst == NULL){
        printf("Carrinho ja esta vazio, tente adicionar algo antes de remover.\n");
        return lst;
    }
    
    Carts *a = NULL; //Element before
    Carts *c = lst; //Go through the list
    
    //Search for the product ant keep saving the previous
    while(c!=NULL && c->cod != code){
        a = c;
        c = c->next;
    }
    
    //Verify if got the product
    if (c == NULL){
        return lst;
    }
    if(a == NULL){//Remove the first element
        lst = c->next;
    }else{
        a->next = c->next;
    }
    
    strcpy(msg,"20:");
    strcat(msg,itoa(code,10));
    strcat(msg,":");
    strcat(msg,itoa(c->qtd,10));
    strcat(msg,":");
    
    writeToServer(sock, msg, server_reply);
    
    if(strcmp(server_reply,"1") == 0)
        printf("Produto removido com sucesso!\n");
    
    free(c);
    return lst;
}

//Show options from the cart, like: chekout, remove product...
Carts *manageCart(Carts *lst, int sock, char server_reply[], char username[]){
    int option=0,codigo,TotalCart;
    Carts* c;
    TotalCart = listProductInCart(lst,sock,server_reply);
    printf("\n1) Finalizar compra.\n");
    printf("2) Remover Produto.\n");
    printf("3) Voltar.\n");
    scanf("%d",&option);
    while(option != 3){
        if(option == 1){//Checkout
            if(lst == NULL){
                printf("Carrinho vazio, coloque alguns produtos no carrinho antes. \n");
            }else{
                for(c=lst;c!=NULL;c = c->next){
                    lst = checkout(lst,username,server_reply,sock,TotalCart,lst->cod);
                }
            }
            return lst;
        }else if(option == 2){//Delete product from the cart
            if(lst == NULL){
                printf("Carrinho ja esta vazio, tente adicionar algo antes de remover.\n");
                return lst;
            }
            printf("Digite o cod do produto que deseja remover: \n");
            scanf("%d",&codigo);
            while(searchCart(lst,codigo) == NULL){
                printf("Produto nao esta no carrinho, tente novamente: \n");
                scanf("%d",&codigo);
            }
            lst = removeProductFromCart(lst,codigo,sock,server_reply);
            return lst;
        }else if(option == 3){//Go back
            return lst;
        }else if(option == -2){
            system("clear");
            printf("\n1) Finalizar compra.\n");
            printf("2) Remover Produto.\n");
            printf("3) Voltar.\n");
            scanf("%d",&option);
        }else{
            printf("Opcao nao encontrada, tente novamente: \n");
            scanf("%d",&option);
        }
    }
    
    memset(server_reply,0,STR_MAX_SIZE);
    return lst;
}

//Function to manage Products to put it in the cart, or see remove...
Carts *manageProducts(int sock, char server_reply[], Carts *c, char username[]){
    int option=0;
    int qtdProcut,qtdComprar=0,productExist=0;
    int codigos[200],code;
    printf("**Comprando Produtos**\n");
    printf("1) Adicionar produto ao carrinho.\n");
    printf("2) Listar produtos do carrinho (Verificar, remover, finalizar compra).\n");
    printf("3) Voltar.\n");
    scanf("%d",&option);
    system("clear");
    while(option != 3){
        if(option == 1){
            option = -2;
            system("clear");
            listProducts(sock,server_reply,codigos,&qtdProcut);
            printf("Digite o codigo do produto que deseja comprar: (-1 para cancelar)\n");
            scanf("%d",&code);
            if(code == -1){
                return c;
            }
            productExist = checkProductList(qtdProcut, codigos, code);
            while(productExist == 0){
                system("clear");
                listProducts(sock,server_reply,codigos,&qtdProcut);
                printf("Produto nao encontrado, porfavor digite novamente o codigo: (-1 para cancelar)\n");
                scanf("%d",&code);
                if(code == -1){
                    return c;
                }
                productExist = checkProductList(qtdProcut, codigos, code);
            }
            printf("Digite a quantidade desejada: \n");
            scanf("%d",&qtdComprar);
            if(qtdComprar == 0){
                return c;
            }
            c = addProduct2Cart(sock,server_reply,c,code,qtdComprar);
        }else if(option == 2){
            option = -2;
            system("clear");
            c = manageCart(c,sock,server_reply,username);
        }else if(option == 3){
            return c;
        }else if(option == -2){
            printf("\n**Comprando Produtos**\n");
            printf("1) Adicionar produto ao carrinho.\n");
            printf("2) Listar produtos do carrinho.\n");
            printf("3) Voltar.\n");
            scanf("%d",&option);
        }else{
            printf("Opcao inesistente, tente novamente: \n");
            scanf("%d",&option);
        }
    }
    return c;
}

//Show all statistics, from a user.
void listStatistics(int sock, char username[], char server_repaly[]){
    int i=0,j=0,codigo,qtdComprada,hora,min,dia,option;
    char aux[STR_MAX_SIZE],nomeProduto[50];
    char msg[STR_MAX_SIZE];
    float valorGasto,totalGastoMarket=0;
    memset(msg,0,STR_MAX_SIZE);
    memset(aux,0,STR_MAX_SIZE);
    memset(nomeProduto,0,50);
    
    strcpy(msg,"9:");
    strcat(msg,username);
    strcat(msg,":");
    
    writeToServer(sock,msg,server_repaly);
    if(strcmp(server_repaly,"0") == 0){
        printf("Usuario nunca comprou nenhum produto.\n");
        printf("Digite 0 para voltar: ");
        scanf("%d",&option);
        while(option != 0){
            printf("Comando nao encontrado, tente novamente. (digite 0 para voltar): ");
            scanf("%d",&option);
        }
        memset(server_repaly,0,STR_MAX_SIZE);
        return;
    }
    printf("\t** Estatisticas **\n");
    printf("%6s","Nome");
    printf("%8s","Codigo");
    printf("%15s","Qtd comprada");
    printf("%15s","Valor Gasto");
    printf("%10s","Dia");
    printf("%10s","Hora");
    printf("%10s","Min");
    printf("\n--------------------------------------------------------------------------\n");
    while(server_repaly[i] != '\0'){
        memset(aux,0,STR_MAX_SIZE);
        j=0;
        while(server_repaly[i] != ':'){//COdigo
            aux[j] = server_repaly[i];
            i++;
            j++;
        }
        i++;
        codigo = atoi(aux);
        j=0;
        memset(nomeProduto,0,50);
        while(server_repaly[i] != ':'){//Nome do produto
            nomeProduto[j] = server_repaly[i];
            i++;
            j++;
        }
        i++;
        j=0;
        memset(aux,0,STR_MAX_SIZE);
        while(server_repaly[i] != ':'){//Qtd comprada
            aux[j] = server_repaly[i];
            i++;
            j++;
        }
        i++;
        j=0;
        qtdComprada = atoi(aux);
        memset(aux,0,STR_MAX_SIZE);
        while(server_repaly[i] != ':'){//Valor Gasto
            aux[j] = server_repaly[i];
            i++;
            j++;
        }
        i++;
        j=0;
        valorGasto = atof(aux);
        memset(aux,0,STR_MAX_SIZE);
        while(server_repaly[i] != ':'){//Hora
            aux[j] = server_repaly[i];
            i++;
            j++;
        }
        i++;
        j=0;
        hora = atoi(aux);
        memset(aux,0,STR_MAX_SIZE);
        while(server_repaly[i] != ':'){//Min
            aux[j] = server_repaly[i];
            i++;
            j++;
        }
        i++;
        j=0;
        min = atoi(aux);
        memset(aux,0,STR_MAX_SIZE);
        while(server_repaly[i] != ':'){//mDay
            aux[j] = server_repaly[i];
            i++;
            j++;
        }
        dia = atoi(aux);
        i++;    
        printf("%6s",nomeProduto);
        printf("%8d",codigo);
        printf("%10d",qtdComprada);
        printf("%15.2f",valorGasto);
        printf("%13d",dia);
        printf("%11d",hora);
        printf("%11d",min);
    
        printf("\n--------------------------------------------------------------------------\n");
        memset(aux,0,STR_MAX_SIZE);
        totalGastoMarket += valorGasto;
    }
    
    printf("Valor total gasto no LabCMarket foi de: %.2f",totalGastoMarket);
    
    printf("\n\nDigite 0 para voltar: ");
    scanf("%d",&option);
    while(option != 0){
        printf("Comando nao encontrado, tente novamente. (digite 0 para voltar): ");
        scanf("%d",&option);
    }
    
    memset(server_repaly,0,STR_MAX_SIZE);
    
}

//Show the principal menu
void showMenu(int sock, char *username){
    int option;
    char msg[STR_MAX_SIZE];
    char server_repaly[STR_MAX_SIZE];
    Carts* c;
    c = createCart();
    
    printf("**MENU**\n");
    printf("1) Gerir Saldo\n");
    printf("2) Gerir Lista de Compras\n");
    printf("3) Ver Estatisticas\n");
    printf("4) Logout\n");
    scanf("%d",&option);
    while(option != 4){
        if(option == 1){
            option = -2;
            system("clear");
            memset(msg,0,STR_MAX_SIZE);
            strcpy(msg,"3:");
            strcat(msg,username);
            strcat(msg,":");
            writeToServer(sock,msg,server_repaly);
            manegeBalance(server_repaly);
            showManageOptions(sock,username,server_repaly);
        }else if(option == 2){
            option = -2;
            system("clear");
            c = manageProducts(sock,server_repaly,c,username);
        }else if(option == 3){
            option = -2;
            listStatistics(sock,username,server_repaly);
        }else if(option == -2){
            system("clear");
            printf("**MENU**\n");
            printf("1) Gerir Saldo\n");
            printf("2) Gerir Lista de Compras\n");
            printf("3) Ver Estatisticas\n");
            printf("4) Logout\n");
            scanf("%d",&option);
        }
    }
}
