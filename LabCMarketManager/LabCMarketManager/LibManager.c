//
//  LibServer.c
//  LabCMarketServer
//
//  Created by Guilherme Guia on 3/29/17.
//  Copyright © 2017 Guilherme Guia. All rights reserved.
//

#include "LibManager.h"

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

char* itoa(int val, int base){
    
    static char buf[32] = {0};
    
    int i = 30;
    
    for(; val && i ; --i, val /= base)
        
        buf[i] = "0123456789abcdef"[val % base];
    
    return &buf[i+1];
    
}

void validManager(int sock){
    char username[50], password[50],server_reply[STR_MAX_SIZE];
    char message[STR_MAX_SIZE];
    memset(message,0,STR_MAX_SIZE);
    memset(server_reply,0,STR_MAX_SIZE);
    
    system("clear");
    printf("Autentique-se:\n");
    printf("Username: ");
    scanf("%s",username);
    printf("Password: ");
    scanf("%s",password);
    strcpy(message,"1:");
    strcat(message,username);
    strcat(message,":");
    strcat(message,password);
    strcat(message,":");
    write2Server(sock,message,server_reply);
    while(strcmp(server_reply,"1") != 0){
        system("clear");
        printf("Usuario nao encontrado, tente novamente.\n");
        printf("Autentique-se:\n");
        printf("Username: ");
        scanf("%s",username);
        printf("Password: ");
        scanf("%s",password);
        strcpy(message,"1:");
        strcat(message,username);
        strcat(message,":");
        strcat(message,password);
        strcat(message,":");
        write2Server(sock,message,server_reply);
    }
    system("clear");
    showOptions(sock);
}

int addnewUser(int sock, char server_reply[]){
    int option=0;
    char msg[STR_MAX_SIZE];
    char nome[255],contato[10],username[50],password[50];
    memset(nome,0,255);
    memset(contato,0,10);
    memset(username,0,50);
    memset(password,0,50);
    memset(msg,0,STR_MAX_SIZE);
    
    
    printf("Digite o nome: \n");
    scanf("%s",nome);
    printf("Digite o contato (max 9 digitos): \n");
    scanf("%s",contato);
    printf("Digite o username: \n");
    scanf("%s",username);
    printf("Digite a senha: \n");
    scanf("%s",password);
    
    strcpy(msg,"10");
    strcat(msg,":");
    strcat(msg,nome);
    strcat(msg,":");
    strcat(msg,contato);
    strcat(msg,":");
    strcat(msg,username);
    strcat(msg,":");
    strcat(msg,password);
    strcat(msg,":");
    strcat(msg,"1000");
    strcat(msg,":");
    
    printf("Porfavor, confirme as informacoes: \n");
    printf("Nome: %s\nContato: %s\nUsername: %s\nPassword: %s\n",nome,contato,username,password);
    printf("\nDigite 1 para confirmar, 0 para digitar novamente ou -1 para cancelar\n");
    scanf("%d",&option);
    while(option != 1 && option != 0 && option != -1){
        printf("Opcao nao reconhecida, tente novamente: \n");
        scanf("%d",&option);;
    }
    if(option == 1){//Send the msg to the server, and see if it's ok.
    
        write2Server(sock,msg,server_reply);
        if(strcmp(server_reply,"1") == 0){
            system("clear");
            printf("Usuario criado com sucesso!\n");
            return 0;
        }else{
            printf("Erro ao criar o novo usuario.\n");
            return 0;
        }
    }else if(option == -1){//Go back.
        return 0;
    }else{
        while(option == 0){
            system("clear");
            //Clear all the mem that was written before(just to clear all garbage).
            memset(nome,0,255);
            memset(contato,0,10);
            memset(username,0,50);
            memset(password,0,50);
            memset(msg,0,STR_MAX_SIZE);
            printf("Digite o nome: \n");
            scanf("%s",nome);
            printf("Digite o contato (max 9 digitos): \n");
            scanf("%s",contato);
            printf("Digite o username: \n");
            scanf("%s",username);
            printf("Digite a senha: \n");
            scanf("%s",password);
            //Put it in one string to send to server
            strcpy(msg,"10");
            strcat(msg,":");
            strcat(msg,nome);
            strcat(msg,":");
            strcat(msg,contato);
            strcat(msg,":");
            strcat(msg,username);
            strcat(msg,":");
            strcat(msg,password);
            strcat(msg,":");
            strcat(msg,"1000");
            strcat(msg,":");
            printf("Porfavor, confirme as informacoes: \n");
            printf("Nome: %s\nContato: %s\nUsername: %s\nPassword: %s\n",nome,contato,username,password);
            printf("\nDigite 1 para confirmar, 0 para digitar novamente ou -1 para cancelar\n");
            scanf("%d",&option);
        }
        write2Server(sock,msg,server_reply);
        if(strcmp(server_reply,"1") == 0){
            printf("Usuario criado com sucesso!\n");
        }else{
            printf("Erro ao criar o novo usuario.\n");
        }
    }
    
    return 0;

}

void listStock(int sock, char server_reply[]){
    char msg[STR_MAX_SIZE],tmp[STR_MAX_SIZE];
    int i=0,j=0;
    memset(msg,0,STR_MAX_SIZE);
    memset(tmp,0,STR_MAX_SIZE);
    strcpy(msg,"11:");
    
    write2Server(sock,msg,server_reply);
    
    printf("\t\t\t** Lista dos produtos **\n");
    printf("%6s","Produto");
    printf("%14s","Descricao");
    printf("%10s","Codigo");
    printf("%15s","Quantidade");
    printf("%10s","Custo");
    printf("%10s","Preco");
    printf("\n----------------------------------------------------------------------\n");
    while(server_reply[i] != '\0'){
        j=0;
        while(server_reply[i] != ':'){//print the name
            tmp[j] = server_reply[i];
            i++;
            j++;
        }
        j=0;
        i++;
        printf("%6s",tmp);
        memset(tmp,0,STR_MAX_SIZE);
        while(server_reply[i] != ':'){//print the description
            tmp[j] = server_reply[i];
            i++;
            j++;
        }
        i++;
        j=0;
        printf("%14s",tmp);
        memset(tmp,0,STR_MAX_SIZE);
        while(server_reply[i] != ':'){// print the code
            tmp[j] = server_reply[i];
            i++;
            j++;
        }
        i++;
        j=0;
        printf("%10s",tmp);
        memset(tmp,0,STR_MAX_SIZE);
        while(server_reply[i] != ':'){//print the quantty
            tmp[j] = server_reply[i];
            i++;
            j++;
        }
        i++;
        j=0;
        printf("%13s",tmp);
        memset(tmp,0,STR_MAX_SIZE);
        while(server_reply[i] != ':'){//print the cost
            tmp[j] = server_reply[i];
            i++;
            j++;
        }
        i++;
        j=0;
        printf("%10.2f",atof(tmp));
        memset(tmp,0,STR_MAX_SIZE);
        while(server_reply[i] != ':'){//print the price
            tmp[j] = server_reply[i];
            i++;
            j++;
        }
        printf("%14.2f",atof(tmp));
        printf("\n----------------------------------------------------------------------\n");
        i++;
        memset(tmp,0,STR_MAX_SIZE);
    }
    printf("\n");
    printf("\t\t\t**Fim da lista de produtos**\n");
    memset(server_reply,0,STR_MAX_SIZE);

}

void includeNewProduct(int sock, char server_reply[]){
    char msg[STR_MAX_SIZE],tmp[STR_MAX_SIZE];
    char nome[50],descricao[50];
    int codigo,qtd,option;
    float custo,preco;
    
    memset(nome,0,50);
    memset(descricao,0,50);
    memset(msg,0,STR_MAX_SIZE);
    memset(tmp,0,STR_MAX_SIZE);
    
    
    
    printf("Digite as seguintes informacoes do produto: \n");
    printf("Nome: ");
    scanf("%s",nome);
    printf("Descricao: ");
    scanf("%s",descricao);
    printf("Codigo: ");
    scanf("%d",&codigo);
    printf("Custo: ");
    scanf("%f",&custo);
    printf("Preco: ");
    scanf("%f",&preco);
    printf("Quantidade: ");
    scanf("%d",&qtd);
    
    
    system("clear");
    printf("Confirme as informacoes: \n");
    printf("Nome: %s\nDescricao: %s\nCodigo: %d\nCusto: %.2f\nPreco: %.2f\nQuantidade: %d\n",nome,descricao,codigo,custo,preco,qtd);
    printf("\nDigite 1 para confirmar ou 0 para digitar novamente.\n");
    scanf("%d",&option);
    
    
    while(option != 1 && option != 0){
        printf("Comando nao reconhecido, tente novamente: \n");
        scanf("%d",&option);
    }
    if(option == 1){
        strcpy(msg,"12:");
        strcat(msg,nome);
        strcat(msg,":");
        strcat(msg,descricao);
        strcat(msg,":");
        strcat(msg,itoa(codigo,10));
        strcat(msg,":");
        snprintf(tmp,STR_MAX_SIZE,"%f",custo);
        strcat(msg,tmp);
        memset(tmp,0,STR_MAX_SIZE);
        strcat(msg,":");
        snprintf(tmp,STR_MAX_SIZE,"%f",preco);
        strcat(msg,tmp);
        strcat(msg,":");
        strcat(msg,itoa(qtd,10));
        strcat(msg,":");
        
        write2Server(sock, msg, server_reply);
        
        if(strcmp(server_reply,"1") == 0){
            printf("Produto criado com sucesso!\n");
            return;
        }else{
            printf("Erro ao criar o produto.\n");
        }
    }else{
        return;
    }
    
}

void editStock(int sock, char server_reply[]){
    listStock(sock, server_reply);
    char msg[STR_MAX_SIZE],tmp[STR_MAX_SIZE];
    memset(msg,0,STR_MAX_SIZE);
    int cod = 0, flag = 1,option=0, changed;
    float changedf;
    printf("\n");
    while(flag == 1){
        printf("Digite o codigo do produto que deseja editar.\n");
        scanf("%d",&cod);
        memset(msg,0,STR_MAX_SIZE);
        strcpy(msg,"13:");
        strcat(msg,itoa(cod,10));
        write2Server(sock,msg,server_reply);
        if(strcmp(server_reply,"0") == 0){
            printf("Codigo invalido, digite 1 para tentar novamente ou digite 0 para cancelar.\n");
            scanf("%d",&flag);
        }else{
            flag = 2;
        }
    }
    
    if(flag == 0){
        system("clear");
        return;
    }
    
    printf("1) Alterar quantidade\n");
    printf("2) Alterar custo\n");
    printf("3) Alterar preco\n");
    printf("4) Remover produto\n");
    scanf("%d",&option);
    while(option != -3){
        if(option == 1){
            printf("Digite a nova quantidade: \n");
            scanf("%d",&changed);
            memset(msg,0,STR_MAX_SIZE);
            strcpy(msg,"14:");
            strcat(msg,itoa(changed,10));
            strcat(msg,":");
            strcat(msg,itoa(cod,10));
            strcat(msg,":");
            write2Server(sock, msg, server_reply);
            option = -3;
        }else if(option == 2){
            printf("Digite o novo custo: \n");
            scanf("%f",&changedf);
            memset(msg,0,STR_MAX_SIZE);
            memset(tmp,0,STR_MAX_SIZE);
            strcpy(msg,"15:");
            snprintf(tmp, STR_MAX_SIZE, "%f",changedf);
            strcat(msg,tmp);
            strcat(msg,":");
            strcat(msg,itoa(cod,10));
            strcat(msg,":");
            write2Server(sock,msg, server_reply);
            option = -3;
        }else if(option == 3){
            printf("Digite o novo preco: \n");
            scanf("%f",&changedf);
            memset(msg,0,STR_MAX_SIZE);
            memset(tmp,0,STR_MAX_SIZE);
            snprintf(tmp,STR_MAX_SIZE, "%f",changedf);
            strcpy(msg,"16:");
            strcat(msg,tmp);
            strcat(msg,":");
            strcat(msg,itoa(cod,10));
            strcat(msg,":");
            write2Server(sock,msg, server_reply);
            option = -3;
        }else if(option == 4){
            memset(msg,0,STR_MAX_SIZE);
            strcpy(msg,"17:");
            strcat(msg,itoa(cod,10));
            strcat(msg,":");
            write2Server(sock,msg, server_reply);
            option = -3;
        }else{
            printf("Verifique a alterantiva selecionada.\n");
        }
    }
    
}

void manageStock(int sock,char server_reply[]){
    int option;
    printf("**Menu Gerir Stock**\n");
    printf("1) Listar os produtos.\n");
    printf("2) Incluir novo produto.\n");
    printf("3) Editar produto existente.\n");
    printf("4) Retornar ao menu principal.\n");
    scanf("%d",&option);
    
    while(option != 4){
        if(option == 1){
            option = -2;
            system("clear");
            listStock(sock,server_reply);
        }else if(option == 2){
            option = -2;
            includeNewProduct(sock,server_reply);
        }else if(option == 3){
            option = -2;
            system("clear");
            editStock(sock,server_reply);
        }else if(option == 4){
            return;
        }else if(option == -2){
            printf("**Menu Gerir Stock**\n");
            printf("1) Listar os produtos.\n");
            printf("2) Incluir novo produto.\n");
            printf("3) Editar produto existente.\n");
            printf("4) Retornar ao menu principal.\n");
            scanf("%d",&option);
        }else{
            printf("Opcao nao reconhecida, tente novamente: \n");
            scanf("%d",&option);
        }
    }
}

void listAllUsers(int sock,char server_reply[]){
    //Eg msg reciving: jorge:jorge:912345678:guilherme:gui:123456789:
    // Name:username:contact
    int i=0,j=0,contact;
    char tmp[STR_MAX_SIZE];
    write2Server(sock,"18:",server_reply);
    memset(tmp,0,STR_MAX_SIZE);
    
    printf("%12s","Nome");
    printf("%15s","Username");
    printf("%15s","Contato");
    printf("\n----------------------------------------------------------------------\n");
    while(server_reply[i] != '\0'){//run untl the server_reply ends.
        j=0;
        while(server_reply[i] != ':'){//get the name and print it.
            tmp[j] = server_reply[i];
            i++;
            j++;
        }
        i++;
        j=0;
        printf("%13s",tmp); // print the name
        memset(tmp,0,STR_MAX_SIZE);//clear memory
        while(server_reply[i] != ':'){//get username.
            tmp[j] = server_reply[i];
            i++;
            j++;
        }
        i++;
        j=0;
        printf("%15s",tmp);//print username
        memset(tmp,0,STR_MAX_SIZE);
        while(server_reply[i] != ':'){//Get contact number
            tmp[j] = server_reply[i];
            i++;
            j++;
        }
        i++;
        j=0;
        contact = atoi(tmp);//change char to int.
        printf("%15d",contact);
        printf("\n----------------------------------------------------------------------\n");
    }
    
}

void listStatisticsSpecificUser(int sock, char server_reply[]){
    int i=0,j=0,codigo,qtdComprada,hora,min,dia;
    char username[50],aux[STR_MAX_SIZE],nomeProduto[50];
    float valorGasto,totalGastoMarket=0;
    memset(username,0,50);
    memset(aux,0,STR_MAX_SIZE);
    
    listAllUsers(sock,server_reply);
    
    printf("Digite o username: \n");
    scanf("%s",username);
    
    strcpy(aux,"19:");
    strcat(aux,username);
    
    write2Server(sock,aux,server_reply);
    while(strcmp(server_reply,"0") == 0){
        memset(username,0,50);
        memset(server_reply,0,STR_MAX_SIZE);
        memset(aux,0,STR_MAX_SIZE);
        printf("Usuario nao encontrado, tente novamente: \n");
        scanf("%s",username);
        strcpy(aux,"19:");
        strcat(aux,username);
        write2Server(sock,aux,server_reply);
    }
    memset(aux,0,STR_MAX_SIZE);
    strcpy(aux,"9:");
    strcat(aux,username);
    strcat(aux,":");
    write2Server(sock,aux,server_reply);
    
    if(strcmp(server_reply,"0") == 0){
        system("clear");
        printf("Usuario nao comprou nada ainda.\n");
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
    memset(aux,0,STR_MAX_SIZE);
    while(server_reply[i] != '\0'){
        j=0;
        memset(aux,0,STR_MAX_SIZE);
        while(server_reply[i] != ':'){//COdigo
            aux[j] = server_reply[i];
            i++;
            j++;
        }
        i++;
        codigo = atoi(aux);
        memset(nomeProduto,0,50);
        j=0;
        while(server_reply[i] != ':'){//Nome do produto
            nomeProduto[j] = server_reply[i];
            i++;
            j++;
        }
        i++;
        j=0;
        memset(aux,0,STR_MAX_SIZE);
        while(server_reply[i] != ':'){//Qtd comprada
            aux[j] = server_reply[i];
            i++;
            j++;
        }
        i++;
        j=0;
        qtdComprada = atoi(aux);
        memset(aux,0,STR_MAX_SIZE);
        while(server_reply[i] != ':'){//Valor Gasto
            aux[j] = server_reply[i];
            i++;
            j++;
        }
        i++;
        j=0;
        valorGasto = atof(aux);
        memset(aux,0,STR_MAX_SIZE);
        while(server_reply[i] != ':'){//Hora
            aux[j] = server_reply[i];
            i++;
            j++;
        }
        i++;
        j=0;
        hora = atoi(aux);
        memset(aux,0,STR_MAX_SIZE);
        while(server_reply[i] != ':'){//Min
            aux[j] = server_reply[i];
            i++;
            j++;
        }
        i++;
        j=0;
        min = atoi(aux);
        memset(aux,0,STR_MAX_SIZE);
        while(server_reply[i] != ':'){//mDay
            aux[j] = server_reply[i];
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
    
    printf("Valor total gasto no LabCMarket foi de: %.2f\n\n",totalGastoMarket);
    
}

void showStatisticsOptions(int sock,char server_reply[]){
    int option;
    
    printf("Vers estatisticas de: \n");
    printf("1) todos os usuarios.\n");
    printf("2) um usuario especifico.\n");
    printf("3) Voltar.\n");
    scanf("%d",&option);
    
    if(option == 3){
        return;
    }else if(option == 1){
        option = -2;
        system("clear");
    }else if(option == 2){
        option = -2;
        system("clear");
        listStatisticsSpecificUser(sock, server_reply);
    }else if(option == -2){
        printf("Vers estatisticas de: \n");
        printf("1) todos os usuarios.\n");
        printf("2) um usuario especifico.\n");
        printf("3) Voltar.\n");
        scanf("%d",&option);
    }else{
        printf("Opcao nao reconhecida, tente novamente: \n");
        scanf("%d",&option);
    }
    
}

void showOptions(int sock){
    int option,check=0;
    char server_reply[STR_MAX_SIZE];
    printf("**Menu Principal**\n");
    printf("1) Criar novo utilizador\n");
    printf("2) Gerir Stocks\n");
    printf("3) Ver Estatisticas\n");
    printf("4) Logout\n");
    scanf("%d",&option);
    while(option != 5){
        if(option == 1){
            system("clear");
            option = -2;
            check = addnewUser(sock,server_reply);
            while(check == 1){
                check = addnewUser(sock,server_reply);
            }
        }else if(option == 2){
            system("clear");
            option = -2;
            manageStock(sock,server_reply);
        }else if(option == 3){
            system("clear");
            option = -2;
            showStatisticsOptions(sock,server_reply);
        }else if(option == 4){
            return;
        }else if(option == -2){
            printf("**Menu Principal**\n");
            printf("1) Criar novo utilizador\n");
            printf("2) Gerir Stocks\n");
            printf("3) Ver Estatisticas\n");
            printf("4) Logout\n");
            scanf("%d",&option);
        }else{
            printf("Verifique o numero digitado.\n");
            scanf("%d",&option);
        }
    }
}

char *write2Server(int sock, char msg[], char server_reply[]){
    memset(server_reply,0,2000);
    
    if( send(sock , msg , strlen(msg) , 0) < 0)
    {
        puts("Send failed");
        return "-1";
    }

    if( recv(sock , server_reply , 2000 , 0) < 0)
    {
        puts("recv failed");
        return "-1";
    }
    return server_reply;
}






