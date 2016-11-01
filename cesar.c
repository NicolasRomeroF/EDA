#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
// Lista ciclica
typedef struct nodoLista
{
    char letra;
    struct nodoLista *siguiente;
} Nodo;

typedef struct
{
    Nodo *cabeza;
    int size;
} Lista;

Lista crearLista()
{
    Lista nueva;
    nueva.cabeza = NULL;
    nueva.size = 0;
    return nueva;
}

Nodo *crearNodo(char elemento)
{
    Nodo *nuevo = malloc(sizeof(Nodo));
    if(nuevo==NULL)
    {
        printf("No se encontro memora suficiente para crear el nodo\n");
        return NULL;
    }
    nuevo -> letra = elemento;
    nuevo -> siguiente = NULL;
    return nuevo;
}

Lista insertar(Lista actual, char elemento, int posicion)
{
    Nodo *nuevo = crearNodo(elemento);
    Nodo *indice = NULL;
    int contador;
    if(posicion > actual.size || posicion < 0)
    {
        printf("La posicion no existe\n");
        return actual;
    }
    if(nuevo==NULL)
    {
        return actual;
    }
    if(posicion==actual.size && actual.size == 0)
    {
        actual.cabeza = nuevo;
    }
    else if(posicion==actual.size)
    {
        nuevo->siguiente=actual.cabeza;
    }
    if(posicion == 0)
    {
        nuevo -> siguiente = actual.cabeza;
        actual.cabeza = nuevo;
        actual.size++;
        return actual;
    }
    indice = actual.cabeza;
    for(contador = 0; contador < posicion - 1; contador++)
    {
        indice = indice -> siguiente;
    }
    nuevo -> siguiente = indice -> siguiente;
    indice -> siguiente = nuevo;
    actual.size++;
    return actual;
}

char obtener(Lista actual, int posicion)
{
    Nodo *indice = NULL;
    int contador;
    if(posicion < 0)
    {
        return -1;
    }
    indice = actual.cabeza;
    for(contador = 0; contador < posicion; contador++)
    {
        indice = indice -> siguiente;
    }
    return indice -> letra;
}

Lista borrar(Lista actual, int posicion)
{
    Nodo *indice = actual.cabeza;
    Nodo *auxiliar = NULL;
    int cuenta = 0;
    if(posicion < 0 || posicion >= actual.size)
    {
        return actual;
    }
    if(posicion == 0)
    {
        actual.cabeza = indice -> siguiente;
        free(indice);
        indice = NULL;
        actual.size--;
        return actual;
    }
    while(cuenta < posicion - 1)
    {
        indice = indice -> siguiente;
        cuenta++;
    }
    auxiliar = indice -> siguiente;
    indice -> siguiente = auxiliar -> siguiente;
    free(auxiliar);
    auxiliar = NULL;
    actual.size--;
    return actual;
}

int buscar(Lista actual, char elemento)
{
    Nodo *indice = actual.cabeza;
    int i;
    for(i=0; i<actual.size; i++)
    {
        if(indice -> letra == elemento)
        {
            return i;
        }
        indice = indice -> siguiente;
    }
    return -1;
}

Lista anular(Lista actual)
{
    while(actual.size > 0)
    {
        actual = borrar(actual, 0);
    }
    return actual;
}

Lista moverIzq(Lista actual)
{
    actual.cabeza=actual.cabeza->siguiente;
    return actual;
}

Lista moverDer(Lista actual)
{
    Nodo *indice=actual.cabeza;
    int i;
    for(i=0; i<actual.size-1; i++)
    {
        indice=indice->siguiente;
    }
    actual.cabeza=indice;
    return actual;
}

Lista ordenarLista(Lista lista)
{
    Nodo *p=NULL;
    Nodo *t=NULL;
    int i;
    int temp;
    int band=1;
    while(band!=0)
    {
        band=0;
        p=lista.cabeza;
        for(i=0; i<lista.size; i++)
        {
            t=p;
            p=p->siguiente;
            if(!(i==lista.size-1))
            {
                if(t->letra>p->letra)
                {
                    temp=t->letra;
                    t->letra=p->letra;
                    p->letra=temp;
                    band++;
                }
            }
        }

    }
    return lista;
}

Lista copiarLista(Lista actual)
{
    Lista copia = crearLista();
    char elemento;
    int i;
    for(i=0; i<actual.size; i++)
    {
        elemento=obtener(actual,i);
        copia=insertar(copia,elemento,i);
    }
    return copia;
}

Lista borrarRepetidos(Lista lista)
{
    int i;
    int j=0;
    char elemento;
    Lista nueva = crearLista();
    for (i=0; i<lista.size; i++)
    {
        elemento=obtener(lista,i);

        if(buscar(nueva,elemento)==-1)
        {
            nueva=insertar(nueva,elemento,j);
            j++;
        }
    }
    return nueva;
}


void mostrar(Lista actual)
{
    Nodo *indice = actual.cabeza;
    int i = 0;
    while(i<actual.size)
    {
        printf("%c ", indice -> letra);
        indice = indice -> siguiente;
        i++;
    }
    printf("\n");
}

Lista crearListaCodificacion(Lista lista)
{
    lista=ordenarLista(lista);
    lista=borrarRepetidos(lista);
    lista=insertar(lista,' ',lista.size);
    return lista;
}

int cantidadEspacios(Lista actual)
{
    int i;
    int espacios=0;
    for(i=0; i<actual.size; i++)
    {
        if(obtener(actual,i)==' ')
        {
            espacios++;
        }
    }
    return espacios-1;
}

FILE *abrirEntrada()
{
    FILE *archivo=fopen("Entrada.in","r");
    if(archivo==NULL)
    {
        printf("No se pudo abrir el archivo");
        return NULL;
    }
    return archivo;
}

Lista leerLetras(FILE *archivo)
{
    Lista letras = crearLista();
    int posicion = 0;
    char letra;
    while ((letra!='\n' || letra != '\r') && !feof(archivo))
    {
        letra = fgetc(archivo);
        letras = insertar(letras,letra,posicion);
        posicion++;

    }
    letras = borrar(letras,letras.size - 1);
    return letras;
}

Lista leerLetras2(FILE *archivo)
{
    Lista letras = crearLista();
    int posicion = 0;
    char letra;
    while ((letra!='\n' || letra != '\r') && !feof(archivo))
    {

        letra = fgetc(archivo);
        if(letra=='\n')
        {
            break;
        }
        if(letra == '\r')
        {
            fgetc(archivo);
            break;
        }
        letras = insertar(letras,letra,posicion);
        posicion++;


    }
    return letras;
}


void cifrar(Lista letras, Lista original)
{
    FILE *salida = fopen("Salida.out","w");
    if (salida==NULL)
    {
        printf("ERROR: No se pudo crear el archivo de salida");
        return;
    }
    Lista codificacion = copiarLista(letras);
    int i,j;
    char actual;
    char letraCod;
    int indice;
    for (i=0; i<original.size; i++)
    {
        actual = obtener(original,i);
        indice = buscar(letras,actual);
        letraCod=obtener(codificacion,indice);
        fputc(letraCod,salida);
        if(letraCod=='A'||letraCod=='E'||letraCod=='I'||letraCod=='O'||letraCod=='U'||letraCod==' '||letraCod=='a'||letraCod=='e'||letraCod=='i'||letraCod=='o'||letraCod=='u')
        {
            for(j=0; j<3; j++)
            {
                codificacion = moverIzq(codificacion);
            }
        }
        else
        {
            codificacion = moverDer(codificacion);
        }
    }
    char letraDes = obtener(letras,0);
    int desfase = buscar(codificacion,letraDes) + cantidadEspacios(codificacion);

    fprintf(salida,"\n%d",desfase);
    fclose(salida);
    anular(codificacion);
}

void descifrar(Lista letras,Lista cifrada,int desfase)
{
    FILE *salida= fopen("Salida.out","w");
    if (salida==NULL)
    {
        printf("ERROR: No se pudo crear el archivo de salida");
        return;
    }
    Lista decodificacion = copiarLista(letras);
    Lista original = crearLista();
    int i,j,k;
    char actual;
    int indice;
    for(j=0; j<desfase; j++)
    {
        decodificacion=moverDer(decodificacion);
    }
    for(i=0; i<cifrada.size; i++)
    {
        actual=obtener(cifrada,cifrada.size-1-i);
        if(actual=='A'||actual=='E'||actual=='I'||actual=='O'||actual=='U'||actual==' '||actual=='a'||actual=='e'||actual=='i'||actual=='o'||actual=='u')
        {
            for(j=0; j<3; j++)
            {
                decodificacion = moverDer(decodificacion);
            }
        }
        else
        {
            decodificacion = moverIzq(decodificacion);
        }
        indice=buscar(decodificacion,actual);
        original=insertar(original,obtener(letras,indice),0);
    }
    char letraDes = obtener(letras,0);
    int desfaseC = buscar(decodificacion,letraDes) ;
    for(j=0; j<original.size; j++)
    {
        fputc(obtener(original,j),salida);
    }
    fprintf(salida,"\n%d",desfaseC);
    fclose(salida);
    anular(decodificacion);
    anular(original);
}

int main()
{
    printf("////////INICIO DEL PROGRAMA////////\n");
    FILE *entrada=abrirEntrada();
    if (entrada==NULL)
    {
        return 0;
    }
    char string[11];
    char string2[11];
    fscanf(entrada,"%s",string);
    Lista letras;
    if(strcmp(string,"Cifrado")==0)
    {
        if(fgetc(entrada)=='\r')
        {
            fgetc(entrada);
        }
        Lista original = leerLetras(entrada);
        letras = copiarLista(original);
        letras = crearListaCodificacion(letras);
        cifrar(letras,original);
        anular(original);
        anular(letras);

    }
    else if(strcmp(string,"Descifrado")==0)
    {
        if(fgetc(entrada)=='\r')
        {
            fgetc(entrada);
        }
        printf("descicifrar\n");
        int desfase;
        fscanf(entrada,"%d",&desfase);
        if(fgetc(entrada)=='\r')
        {
            fgetc(entrada);
        }
        Lista cifrada=leerLetras2(entrada);
        letras = leerLetras(entrada);
        letras = crearListaCodificacion(letras);
        descifrar(letras,cifrada,desfase);
        anular(cifrada);
        anular(letras);
    }
    fclose(entrada);
    printf("////////FIN DEL PROGRAMA////////\n");
    return 1;
}
