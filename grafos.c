#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int nVisita = 1;

typedef struct
{
    int camino;
    int distancia;
} Dijkstra;

int **crearMatriz(int size)
{
    int **matriz=(int**)malloc((sizeof(int*)*size));
    if(matriz!=NULL)
    {
        int i;
        for (i=0; i<size; i++)
        {
            matriz[i]=(int*)malloc(sizeof(int)*size);
            if (matriz[i]==NULL)
            {
                printf("ERROR: No se encontro suficiente memoria para crear matriz\n");
                return NULL;
            }
        }
        int j;
        for(i=0; i<size; i++)
        {
            for(j=0; j<size; j++)
            {
                matriz[i][j]=0;
            }
        }
        return matriz;
    }
    else
    {
        printf("ERROR: No se encontro suficiente memoria para crear matriz\n");
        return NULL;
    }

}

void printMatriz(int **matriz,int size)
{
    int i,j;
    for(i=0; i<size; i++)
    {
        for(j=0; j<size; j++)
        {
            printf("%d ",matriz[i][j]);
        }
        printf("\n");
    }
}

void printArreglo(int *arreglo,int size)
{
    int i;
    for(i=0; i<size; i++)
    {
        printf("%d ",arreglo[i]);
    }
    printf("\n");
}

void printArregloArchivo(int *arreglo,int size,FILE *archivo)
{
    int i;
    for(i=0; i<size; i++)
    {
        fprintf(archivo, "%d ",arreglo[i]);
    }
    fprintf(archivo,"\n");
}

int **crearMatrizAdy(FILE *archivo,int size)
{
    if(fgetc(archivo)=='\r')
    {
        fgetc(archivo);
    }
    int **matrizAdyacencia=crearMatriz(size);
    if (matrizAdyacencia==NULL)
    {
        return NULL;
    }
    int columna;
    int fila;
    int i,indiceFila,indiceColumna;
    char aux;
    for(i=0; i<size; i++)
    {
        fscanf(archivo,"%d%c",&indiceFila,&aux);
        while(feof(archivo)==0)
        {
            fscanf(archivo,"%d%c",&indiceColumna,&aux);
            matrizAdyacencia[indiceFila-1][indiceColumna-1]=1;
            if(aux==EOF)
            {
                break;
            }
            else if(aux=='\n')
            {
                break;
            }
            else if(aux=='\r')
            {
                aux = fgetc(archivo);
                break;
            }
        }
        aux=0;

    }
    return matrizAdyacencia;
}

void recorrer(int nodoBuscado,int *nodos,int **matriz,int size)
{
    int adyacente;
    nodos[nodoBuscado]=nVisita++;
    for(adyacente=0; adyacente<size; adyacente++)
    {
        if(matriz[nodoBuscado][adyacente] && nodos[adyacente]==0)
        {
            recorrer(adyacente,nodos,matriz,size);
        }
    }
}

int esConexo(int **matriz,int size)
{
    int nodoBuscado;
    int *nodos=(int*)malloc(sizeof(int)*size);
    if(nodos==NULL)
    {
        printf("ERROR: No se pudo asignar memoria para la verificar si el graof es conexo.");
        return -1;
    }
    int i;
    for(i=0; i<size; i++)
    {
        nodos[i]=0;
    }
    int flag=0;
    for(nodoBuscado=0; nodoBuscado<size; nodoBuscado++)
    {
        if(nodos[nodoBuscado]==0)
        {
            recorrer(nodoBuscado,nodos,matriz,size);

            flag++;
        }
    }
    nVisita = 1;
    free(nodos);
    if(flag==1)
    {
        return 1;
    }
    else
    {
        return 0;
    }

}

void freeMatriz(int **matriz,int size)
{
    int i;
    for(i=0; i<size; i++)
    {
        free(matriz[i]);
    }
    free(matriz);
}

int buscar(int *arreglo,int size,int valor)
{
    int i,j;
    for(i=0; i<size; i++)
    {
        if(arreglo[i] == valor)
        {
            return i;
        }
    }
    return -1;
}

int *centralidad(int **matrizAdyacencia,int size)
{
    int *nodos=(int*)malloc(sizeof(int)*size);
    int *orden=(int*)malloc(sizeof(int)*size);
    if(nodos==NULL || orden==NULL)
    {
        printf("ERROR: No se pudo asignar memoria para centralidad.");
        return NULL;
    }
    int i,j,aux;
    for(i=0; i<size; i++)
    {
        aux = 0;
        for(j=0; j<size; j++)
        {
            if(matrizAdyacencia[i][j])
            {
                aux++;
            }
        }
        nodos[i]=aux;
    }

    int may,indice;
    for(i=0; i<size; i++)
    {
        may=-1;
        for(j=0; j<size; j++)
        {
            if(nodos[j]>may)
            {
                may=nodos[j];
            }
        }
        indice=buscar(nodos,size,may);
        orden[i]=indice+1;
        nodos[indice]=-1;
    }
    free(nodos);
    return orden;
}

int menor(Dijkstra *caminoDistancia,int *visitados,int size)
{
    int i;
    int min=INT_MAX;
    int indice;
    for(i=0; i<size; i++)
    {
        if(caminoDistancia[i].distancia<min && visitados[i]==0)
        {
            min=caminoDistancia[i].distancia;
            indice = i;
        }
    }
    return indice;
}

Dijkstra *dijkstra(int **matrizAdy,int size,int origen)
{
    Dijkstra *caminoDistancia=(Dijkstra*)malloc(sizeof(Dijkstra)*size);
    if(caminoDistancia==NULL)
    {
        printf("ERROR: No se pudo asignar memoria para Dijkstra.");
        return NULL;
    }
    int visitados[size];
    int i,j;
    for(i=0; i<size; i++)
    {
        caminoDistancia[i].camino=-1;
        caminoDistancia[i].distancia=INT_MAX;
        visitados[i]=0;
    }
    caminoDistancia[origen-1].camino=0;
    caminoDistancia[origen-1].distancia=0;
    int menorIndice;
    for(i=0; i<size; i++)
    {
        menorIndice=menor(caminoDistancia,visitados,size);
        visitados[menorIndice]=1;


        for(j=0; j<size; j++)
        {
            if(matrizAdy[menorIndice][j]==1 && visitados[j]==0 && (caminoDistancia[menorIndice].distancia+1)<caminoDistancia[j].distancia)
            {
                caminoDistancia[j].camino=menorIndice;
                caminoDistancia[j].distancia=caminoDistancia[menorIndice].distancia+1;
            }
        }
    }

    return caminoDistancia;
}

/*void betweeness(int **matrizAdy,int size)
{
    int i,j,aux;
    Dijkstra **caminos;
    int *betweeness=(int*)malloc(sizeof(int)*size);
    for(i=0; i<size; i++)
    {
        caminos[i]=dijkstra(matrizAdy,size,i);
    }
    for(i=0;i<size;i++)
    {
        for(j=0;j<size;j++)
        {
            while(aux!=0)
            {

            }
        }
    }
}*/

int main()
{
    printf("///////INICIO DEL PROGRAMA///////\n");
    char nombre[30];
    printf("Ingrese el nombre del archivo con su extension (no mas de 30 caracteres): ");
    scanf("%s",nombre);
    printf("\n");
    FILE *archivo = fopen(nombre,"r");

    if(archivo==NULL)
    {
        printf("ERROR: El archivo no se pudo abrir, escriba el nombre correctamente con su extension.");
        return 0;
    }
    int size;
    fscanf(archivo,"%d",&size);
    int **matrizAdyacencia=crearMatrizAdy(archivo,size);
    fclose(archivo);

    if(matrizAdyacencia==NULL)
    {
        return 0;
    }

    FILE *salida=fopen("Salida.txt","w");
    if(salida==NULL)
    {
        printf("ERROR: El archivo de salida no se pudo crear");
        freeMatriz(matrizAdyacencia,size);
        return 0;
    }

    int conexo=esConexo(matrizAdyacencia,size);
    if(conexo==1)
    {
        fprintf(salida,"El grafo es conexo\n");
    }
    else if(conexo==0)
    {
        fprintf(salida,"El grafo no es conexo\n");
    }

    int *ordenCentralidad=centralidad(matrizAdyacencia,size);
    if(ordenCentralidad==NULL)
    {
    	freeMatriz(matrizAdyacencia,size);
    	fclose(salida);
        return 0;
    }
    fprintf(salida,"\n");
    fprintf(salida,"Centralidad de grado de mayor a menor: \n");
    printArregloArchivo(ordenCentralidad,size,salida);
    fprintf(salida,"\n");
    free(ordenCentralidad);
    Dijkstra *caminoDistancia=dijkstra(matrizAdyacencia,size,1);
    if(caminoDistancia==NULL)
    {
    	freeMatriz(matrizAdyacencia,size);
    	fclose(salida);
        return 0;
    }

    int i;
    fprintf(salida,"Dijkstra desde primer vertice hacia los demas:\n");
    for(i=0; i<size; i++)
    {
        fprintf(salida,"Vertice %d. distancia: %d desde: %d\n",i+1,caminoDistancia[i].distancia,caminoDistancia[i].camino+1);
    }
    free(caminoDistancia);

    fclose(salida);
    printf("El archivo de salida fue creado con exito\n");
    freeMatriz(matrizAdyacencia,size);
    printf("///////FIN DEL PROGRAMA///////\n");
    return 0;
}
