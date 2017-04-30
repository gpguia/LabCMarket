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
        printf("Could not create socket");
    }
    puts("Socket created");
    
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );
    
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
    
    puts("Connected\n");
    return sock;
}

Carts *createCart(){
    return NULL;
}

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

void manegeBalance(char server_replay[]){
    int i=0,j=0;
    char name[STR_MAX_SIZE],balance[STR_MAX_SIZE];
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
    printf("Saldo: %s",balance);
    printf("\n");
    printf("\t%s","** Fim Saldo **\n\n");
    memset(server_replay,0,STR_MAX_SIZE);
}

void showManageOptions(int sock, char username[], char server_replay[]){
    int option=0;
    char deposit[STR_MAX_SIZE];
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
            scanf("%s",deposit);
            strcpy(msg,"4:");
            strcat(msg,username);
            strcat(msg,":");
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

void listProducts(int sock, char server_replay[], int codigos[], int *qtdProduct){
    int i=0,j=0,k=0;
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
        printf("%13s",tmp);
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

int checkProductList(int qtdProduct, int lst[], int key){
    int i;
    
    for(i=0;i<qtdProduct;i++){
        if(lst[i] == key){
            return 1;
        }
    }
    return 0;
}

Carts *searchCart(Carts *lst, int code){
    Carts *c;
    
    for(c = lst; c!= NULL; c = c->next){
        if(c->cod == code){
            return c;
        }
    }
    return NULL;
}

Carts *addProduct2Cart(int sock,char server_reply[] , Carts *lst, int code, int qtd){
    char msg[STR_MAX_SIZE];
    memset(msg,0,STR_MAX_SIZE);
    Carts *c;
    c = searchCart(lst,code);
    if(c != NULL){//Verify if the product already exist in the cart
        c->qtd += qtd;
        return c;
    }
    
    Carts *novo = (Carts*) malloc(sizeof(Carts));
    
    novo->cod = code;
    novo->qtd = qtd;
    novo->next = lst;
    
    
    strcpy(msg,"6:");
    strcat(msg,itoa(code,10));
    strcat(msg,":");
    strcat(msg,itoa(qtd,10));
    strcat(msg,":");
    
    writeToServer(sock,msg,server_reply);
    if(strcmp(server_reply,"1") == 0){
        printf("Produto adicionado com sucesso!\n");
    }
    memset(server_reply,0,STR_MAX_SIZE);
    return novo;
}

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
Carts *checkout(Carts *lst, char username[], char server_reply[], int sock, float total){
    char msg[STR_MAX_SIZE];
    memset(msg,0,STR_MAX_SIZE);
    
    strcpy(msg,"8:");
    strcat(msg,username);
    strcat(msg,":");
    strcat(msg,itoa(total,10));
    strcat(msg,":");
    
    
    writeToServer(sock,msg,server_reply);
    
    if(strcmp(server_reply,"1") == 0){
        printf("Compra finalizada com sucesso!\n");
        sleep(2);
    }else{
        printf("Dinheiro insuficiente.\n");
        sleep(2);
        return lst;
    }
    
    while(lst != NULL){
        Carts *temp = lst;
        free(temp);
        lst = lst->next;
    }
    
    memset(server_reply,0,STR_MAX_SIZE);
    
    return lst;
}

Carts *removeProductFromCart(Carts *lst, int code){
    Carts *a = NULL; //Elemento anterior
    Carts *c = lst; //Percorrer a lista
    
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
    free(c);
    return lst;
}

Carts *manageCart(Carts *lst, int sock, char server_reply[], char username[]){
    int option=0,codigo,TotalCart;
    TotalCart = listProductInCart(lst,sock,server_reply);
    printf("\n1) Finalizar compra.\n");
    printf("2) Remover Produto.\n");
    printf("3) Voltar.\n");
    scanf("%d",&option);
    while(option != 3){
        if(option == 1){//Checkout
            lst = checkout(lst,username,server_reply,sock,TotalCart);
            return lst;
        }else if(option == 2){//Delete product from the cart
            printf("Digite o cod do produto que deseja remover: \n");
            scanf("%d",&codigo);
            while(searchCart(lst,codigo) == NULL){
                printf("Produto nao esta no carrinho, tente novamente: \n");
                scanf("%d",&codigo);
            }
            lst = removeProductFromCart(lst,codigo);
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

Carts *manageProducts(int sock, char server_reply[], Carts *c, char username[]){
    int option=0;
    int qtdProcut,qtdComprar=0,productExist=0;
    int codigos[200],code;
    printf("**Comprando Produtos**\n");
    printf("1) Adicionar produto ao carrinho.\n");
    printf("2) Listar produtos do carrinho (Verificar, remover, finalizar compra).\n");
    printf("3) Voltar.\n");
    scanf("%d",&option);
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
            system("clear");
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
