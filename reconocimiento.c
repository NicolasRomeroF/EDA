#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int R;
    int G;
    int B;
} Pixel;

typedef struct
{
    Pixel **pixeles;
    int filas;
    int columnas;
} Imagen;



void leerSize(FILE *archivo, Imagen *imagen)
{

    fscanf(archivo,"%d %d",&imagen->filas,&imagen->columnas);
    if(fgetc(archivo)=='\r')
        {
            fgetc(archivo);
        }

}
Pixel **crearMatriz(int N, int M)
{
    Pixel **matriz=(Pixel**)malloc((sizeof(Pixel*)*N));

    if(matriz!=NULL)
    {
        int i;
        for (i=0; i<N; i++)
        {
            matriz[i]=(Pixel*)malloc(sizeof(Pixel)*M);
            if (matriz[i]==NULL)
            {
                printf("ERROR: No se encontro suficiente memoria para crear matriz\n");
                return NULL;
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

Imagen *leerImagen(FILE *archivo)
{
    Imagen *imagenPrincipal= (Imagen*)malloc(sizeof(Imagen));
    if (imagenPrincipal!=NULL)
    {
        leerSize(archivo, imagenPrincipal);

        printf("Tamano de imagen: %dx%d\n",imagenPrincipal->filas,imagenPrincipal->columnas);
        imagenPrincipal->pixeles=crearMatriz(imagenPrincipal->filas,imagenPrincipal->columnas);
        int i;
        int j;
        if(imagenPrincipal->pixeles!=NULL)
        {
            for (i=0; i<imagenPrincipal->filas; i++)
            {
                for (j=0; j<imagenPrincipal->columnas; j++)
                {
                    fscanf(archivo,"%d,%d,%d",&imagenPrincipal->pixeles[i][j].R,&imagenPrincipal->pixeles[i][j].G,&imagenPrincipal->pixeles[i][j].B);
                    if(fgetc(archivo)=='\r')
        {
            fgetc(archivo);
        }
                }
            }
            return imagenPrincipal;
        }
        else
        {
            printf("ERROR: No se encontro suficiente memoria para guardar la imagen\n");
            return NULL;
        }
    }
    else
    {
        printf("ERROR: No se encontro suficiente memoria para guardar la imagen\n");
        return NULL;
    }
}

void printMatriz(Imagen *matriz)
{
    int i,j;
    printf("\n");
    for (i=0; i<matriz->filas; i++)
    {
        for (j=0; j<matriz->columnas; j++)
        {
            printf("%d,",matriz->pixeles[i][j].R);
            printf("%d,",matriz->pixeles[i][j].G);
            printf("%d ",matriz->pixeles[i][j].B);
        }
        printf("\n");
    }
}
int cantidadImagenes(FILE *buscar)
{
    int contador = 0;
    while(feof(buscar)==0)
    {
        if(fgetc(buscar)=='-')
        {
            contador+=1;
        }
    }
    rewind(buscar);
    return (contador/5)+1;
}

void liberarMemoria(Imagen *imagen)
{
    int i;
    int j;
    for (i=0; i<imagen->columnas; i++)
    {
        free(imagen->pixeles[i]);
    }
    free(imagen->pixeles);
    free(imagen);
}

Imagen *rotarMatriz(Imagen *matriz)
{
    int i;
    int j;
    Imagen *matrizRotada=(Imagen*)malloc(sizeof(Imagen));
    matrizRotada->pixeles=crearMatriz(matriz->columnas,matriz->filas);
    if(matrizRotada->pixeles!=NULL)
    {
        matrizRotada->filas=matriz->columnas;
        matrizRotada->columnas=matriz->filas;
        for(i=0; i<matriz->filas; i++)
        {

            for(j=0; j<matriz->columnas; j++)
            {
                matrizRotada->pixeles[j][(matriz->filas)-i-1]=matriz->pixeles[i][j];
            }
        }
        matriz=matrizRotada;

    }
    else
    {
        printf("ERROR: No se encontro suficiente memoria para rotar matriz\n");
    }
}
void compararMatriz(Imagen *principal, FILE *imagesBuscar)
{
    int i;
    int j;
    int k;
    int l;
    int flag ;
    int comprobacion;
    int cantidad = cantidadImagenes(imagesBuscar);
    int n;
    FILE *fSalida=fopen("resultado.txt","w");
    Imagen *buscar;
    int m;
    Imagen rotada;
    for (n=0; n<cantidad; n++)
    {
        buscar=leerImagen(imagesBuscar);
        fscanf(imagesBuscar,"-----");
        if(fgetc(imagesBuscar)=='\r')
        {
            fgetc(imagesBuscar);
        }
        comprobacion=0;
        flag = 1;
        i=0;
        j=0;
        k=0;
        l=0;
        rotada=*principal;
        for (m=0; m<4; m++)
        {

            for(i=0; i<rotada.filas && comprobacion == 0; i++)
            {
                for(j=0; j<rotada.columnas && comprobacion == 0; j++)
                {
                    comprobacion=0;
                    if(rotada.pixeles[i][j].R == buscar->pixeles[0][0].R && rotada.pixeles[i][j].G == buscar->pixeles[0][0].G && rotada.pixeles[i][j].B == buscar->pixeles[0][0].B)
                    {
                        flag=1;
                        for(k=0; k<buscar->filas && i+k<rotada.filas && flag==1; k++)
                        {
                            for(l=0; l<buscar->columnas && j+l<rotada.columnas && flag==1; l++)
                            {
                                if(rotada.pixeles[i+k][j+l].R == buscar->pixeles[k][l].R && rotada.pixeles[i+k][j+l].G == buscar->pixeles[k][l].G && rotada.pixeles[i+k][j+l].B == buscar->pixeles[k][l].B)
                                {
                                    comprobacion = 1;
                                }
                                else
                                {
                                    comprobacion = 0;
                                    flag = 0;
                                }
                            }
                        }
                    }

                }
            }
            rotarMatriz(&rotada);
        }
        if (comprobacion == 1 && k==buscar->filas && l==buscar->columnas)
        {
            fprintf(fSalida,"Imagen %d: Encontrada\n",n+1);

        }
        else
        {
            fprintf(fSalida,"Imagen %d: No encontrada\n",n+1);

        }
    }
    fclose(imagesBuscar);
}


int main()
{
    printf("//////INICIO DEL PROGRAMA//////\n");
    //Imagen Principal
    FILE *imagenPrincipal = fopen("imagenPrincipal.txt","r");

    if (imagenPrincipal==NULL)
    {
        printf("No se pudo abrir el archivo principal");
        printf("\n//////FIN DEL PROGRAMA//////\n");
        return 1;
    }

    Imagen *matrizPrincipal=leerImagen(imagenPrincipal);
    if (matrizPrincipal==NULL)
    {
        printf("\n//////FIN DEL PROGRAMA//////\n");
        return 1;
    }
    fclose(imagenPrincipal);
    //fin
    FILE *imagesBuscar = fopen("imagesBuscar.txt","r");
    if (imagenPrincipal==NULL)
    {
        printf("No se pudo abrir el archivo a comparar");
        printf("\n//////FIN DEL PROGRAMA//////\n");
        return 1;
    }
    compararMatriz(matrizPrincipal,imagesBuscar);
    fclose(imagesBuscar);
    liberarMemoria(matrizPrincipal);
    printf("\n//////FIN DEL PROGRAMA//////\n");

    return 0;
}
