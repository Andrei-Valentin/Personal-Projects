#include <stdio.h>
#include <stdlib.h>

unsigned int InaltimeLungime(char* nume_fisier_sursa,char* nume_fisier_destinatie)
{
   FILE *fin, *fout;
   unsigned int latime_img, inaltime_img;

   printf("nume_fisier_sursa = %s \n",nume_fisier_sursa);

   fin = fopen(nume_fisier_sursa, "rb");
   if(fin == NULL)
   	{
   		printf("eroare");
   		return;
   	}

   fout = fopen(nume_fisier_destinatie, "wb+");



   fseek(fin, 18, SEEK_SET);
   fread(&latime_img, sizeof(unsigned int), 1, fin);
   fread(&inaltime_img, sizeof(unsigned int), 1, fin);
   unsigned int n=latime_img*inaltime_img*2-1;
  // unsigned int n=latime_img*inaltime_img*2;
   return n;

}
void grayscale_image(char* nume_fisier_sursa,char* nume_fisier_destinatie)
{
   FILE *fin, *fout;
   unsigned int dim_img, latime_img, inaltime_img;
   unsigned char pRGB[3], header[54], aux;

   printf("nume_fisier_sursa = %s \n",nume_fisier_sursa);

   fin = fopen(nume_fisier_sursa, "rb");
   if(fin == NULL)
   	{
   		printf("nu am gasit imaginea sursa din care citesc");
   		return;
   	}

   fout = fopen(nume_fisier_destinatie, "wb+");

   fseek(fin, 2, SEEK_SET);
   fread(&dim_img, sizeof(unsigned int), 1, fin);
   printf("Dimensiunea imaginii in octeti: %u\n", dim_img);

   fseek(fin, 18, SEEK_SET);
   fread(&latime_img, sizeof(unsigned int), 1, fin);
   fread(&inaltime_img, sizeof(unsigned int), 1, fin);
   printf("Dimensiunea imaginii in pixeli (latime x inaltime): %u x %u\n",latime_img, inaltime_img);

   //copiaza octet cu octet imaginea initiala in cea noua
	fseek(fin,0,SEEK_SET);
	unsigned char c;
	while(fread(&c,1,1,fin)==1)
	{
		fwrite(&c,1,1,fout);
		fflush(fout);
	}
	fclose(fin);

	//calculam padding-ul pentru o linie
	int padding;
    if(latime_img % 4 != 0)
        padding = 4 - (3 * latime_img) % 4;
    else
        padding = 0;

    printf("padding = %d \n",padding);

	fseek(fout, 54, SEEK_SET);
	int i,j;
	for(i = 0; i < inaltime_img; i++)
	{
		for(j = 0; j < latime_img; j++)
		{
			//citesc culorile pixelului
			fread(pRGB, 3, 1, fout);
			//fac conversia in pixel gri            AICI APLIC RELATIA
			aux = 0.299*pRGB[2] + 0.587*pRGB[1] + 0.114*pRGB[0];
			pRGB[0] = pRGB[1] = pRGB[2] = aux;
        	fseek(fout, -3, SEEK_CUR);
        	fwrite(pRGB, 3, 1, fout);
        	fflush(fout);
		}
		fseek(fout,padding,SEEK_CUR);
	}
	fclose(fout);
}
void PermutarePixeli(char* nume_fisier_sursa,char* nume_fisier_destinatie,unsigned int *PAPAvector)
{
   FILE *fin, *fout;
   unsigned int dim_img, latime_img, inaltime_img;
   unsigned char pRGB[3],pRGB2[3], header[54], aux;

   printf("nume_fisier_sursa = %s \n",nume_fisier_sursa);

   fin = fopen(nume_fisier_sursa, "rb");
   if(fin == NULL)
   	{
   		printf("nu am gasit imaginea sursa din care citesc");
   		return;
   	}

   fout = fopen(nume_fisier_destinatie, "wb+");

  fseek(fin, 2, SEEK_SET);
   fread(&dim_img, sizeof(unsigned int), 1, fin);
   printf("Dimensiunea imaginii in octeti: %u\n", dim_img);

   fseek(fin, 18, SEEK_SET);
   fread(&latime_img, sizeof(unsigned int), 1, fin);
   fread(&inaltime_img, sizeof(unsigned int), 1, fin);
   printf("Dimensiunea imaginii in pixeli (latime x inaltime): %u x %u\n",latime_img, inaltime_img);

unsigned int n=latime_img*inaltime_img;

   fseek(fin,0,SEEK_SET);
	unsigned char c;
	while(fread(&c,1,1,fin)==1)
	{
		fwrite(&c,1,1,fout);
		fflush(fout);
	}
	fclose(fin);


   int padding;
    if(latime_img % 4 != 0)
        padding = 4 - (3 * latime_img) % 4;
    else
        padding = 0;
    printf("padding = %d \n",padding);


	int i,j,k=1;

	for(i = 1; i <= inaltime_img; i++)
	{
		for(j = 1; j <= latime_img; j++)
		{
			fseek(fout, (k-1)*3+54, SEEK_SET);
			fread(pRGB, 3, 1, fout);
			fseek(fout, PAPAvector[k]*3+54, SEEK_SET);
			fwrite(pRGB, 3, 1, fout);
			fflush(fout);
			k++;
		}
		//fseek(fout,padding,SEEK_CUR);
	}



	  fseek(fout, 54, SEEK_SET);
			fread(pRGB, 3, 1, fout);
		pRGB[2] =	987654321 ^ pRGB[2] ^ PAPAvector[n];
		pRGB[1] =	987654321 ^ pRGB[1] ^ PAPAvector[n];
		pRGB[0] =	987654321 ^ pRGB[0] ^ PAPAvector[n];
			fseek(fout, -3, SEEK_CUR);
			fwrite(pRGB, 3, 1, fout);

	k=1;

	for(i = 1; i <= inaltime_img; i++)
	{
	   for(j = 1; j <= latime_img; j++)
		{

          if(k==n)
            break;

		 fread(pRGB2, 3, 1, fout);


      	pRGB[2] =	pRGB[2] ^ pRGB2[2] ^ PAPAvector[n+k];
		pRGB[1] =	pRGB[1] ^ pRGB2[1] ^ PAPAvector[n+k];
		pRGB[0] =	pRGB[0] ^ pRGB2[0] ^ PAPAvector[n+k];

			fseek(fout, -3, SEEK_CUR);
			fwrite(pRGB, 3, 1, fout);

      //  pRGB[2] = pRGB2[2];
       // pRGB[1] = pRGB2[1];
      //  pRGB[0] = pRGB2[0];

			fflush(fout);
			k++;
		}
	}


	fclose(fout);
}

void ReversePermutarePixeli(char* nume_fisier_sursa,char* nume_fisier_destinatie,unsigned int *PAPAvector)
{
   FILE *fin, *fout;
   unsigned int dim_img, latime_img, inaltime_img;
   unsigned char pRGB[3],pRGB2[3], header[54], aux;

   printf("nume_fisier_sursa = %s \n",nume_fisier_sursa);

   fin = fopen(nume_fisier_sursa, "rb");
   if(fin == NULL)
   	{
   		printf("nu am gasit imaginea sursa din care citesc");
   		return;
   	}

   fout = fopen(nume_fisier_destinatie, "wb+");

  fseek(fin, 2, SEEK_SET);
   fread(&dim_img, sizeof(unsigned int), 1, fin);
   printf("Dimensiunea imaginii in octeti: %u\n", dim_img);

   fseek(fin, 18, SEEK_SET);
   fread(&latime_img, sizeof(unsigned int), 1, fin);
   fread(&inaltime_img, sizeof(unsigned int), 1, fin);
   printf("Dimensiunea imaginii in pixeli (latime x inaltime): %u x %u\n",latime_img, inaltime_img);

 unsigned int n=latime_img*inaltime_img;

   fseek(fin,0,SEEK_SET);
	unsigned char c;
	while(fread(&c,1,1,fin)==1)
	{
		fwrite(&c,1,1,fout);
		fflush(fout);
	}
	fclose(fin);


   int padding;
    if(latime_img % 4 != 0)
        padding = 4 - (3 * latime_img) % 4;
    else
        padding = 0;
    printf("padding = %d \n",padding);


	int i,j,k=1;
/*
	for(i = 1; i <= inaltime_img; i++)
	{
		for(j = 1; j <= latime_img; j++)
		{
			fseek(fout, (k-1)*3+54, SEEK_SET);
			fread(pRGB, 3, 1, fout);
			fseek(fout, PAPAvector[k]*3+54, SEEK_SET);                  TREBUIE CALCULATA PERMUTAREA INVERSA
			fwrite(pRGB, 3, 1, fout);
			fflush(fout);
			k++;
		}
		//fseek(fout,padding,SEEK_CUR);
	}
*/


	  fseek(fout, 54, SEEK_SET);
			fread(pRGB, 3, 1, fout);
		pRGB[2] =	987654321 ^ pRGB[2] ^ PAPAvector[n];
		pRGB[1] =	987654321 ^ pRGB[1] ^ PAPAvector[n];
		pRGB[0] =	987654321 ^ pRGB[0] ^ PAPAvector[n];
			fseek(fout, -3, SEEK_CUR);
			fwrite(pRGB, 3, 1, fout);

	k=1;

	for(i = 1; i <= inaltime_img; i++)
	{
	   for(j = 1; j <= latime_img; j++)
		{

          if(k==n)
            break;

		 fread(pRGB2, 3, 1, fout);


      	pRGB[2] =	pRGB[2] ^ pRGB2[2] ^ PAPAvector[n+k];
		pRGB[1] =	pRGB[1] ^ pRGB2[1] ^ PAPAvector[n+k];
		pRGB[0] =	pRGB[0] ^ pRGB2[0] ^ PAPAvector[n+k];

			fseek(fout, -3, SEEK_CUR);
			fwrite(pRGB, 3, 1, fout);

      //  pRGB[2] = pRGB2[2];
       // pRGB[1] = pRGB2[1];
      //  pRGB[0] = pRGB2[0];

			fflush(fout);
			k++;
		}
	}


	fclose(fout);
}




unsigned int randomXOR(unsigned int seed,unsigned int n)
{unsigned int r,k,r2;

unsigned int *PAPAvector = malloc(n * sizeof(unsigned int));

r=seed;
for(k = 1; k <=(n+1)/2 ; k++)
{
    r = r ^ r << 13;
    r = r ^ r >> 17;
    r = r ^ r << 5;

        r2=r%k;

   // printf("%d\n",r2);  //TEST
    PAPAvector[k]=r2;
}

for(k = (n+1)/2+1; k <=n ; k++)
{
    r = r ^ r << 13;
    r = r ^ r >> 17;
    r = r ^ r << 5;


    //printf("%u\n",r);
    PAPAvector[k]=r;
}
//PAPAvector[0]=seed;
return PAPAvector;
}

unsigned int permutari(unsigned int n,unsigned int *PAPAvector2)
{unsigned int r,k,aux;
int *p;
p = malloc(n * sizeof(unsigned int));
    for(k = 1; k <=(n+1)/2 ; k++)
    p[k]=k;

    for(k = (n+1)/2+1 ; k <= n; k++)
    p[k]=PAPAvector2[k];



for(k = (n+1)/2; k >= 1; k--)
{
    r=PAPAvector2[k];
    aux=p[r];
    p[r]=p[k];
    p[k]=aux;
}
return p;
}



 int main()
{unsigned int n;
char nume_img_sursa[] = "peppers.bmp";
char nume_img_grayscale[] = "test_grayscale.bmp";
char nume_img_grayscale2[] = "test_grayscale2.bmp";
n=InaltimeLungime(nume_img_sursa, nume_img_grayscale);           // lungima*inaltime*2-1
printf("%u\n",n);

//unsigned int p=(n+1)/2;
//printf("%d\n",p);
unsigned int *PAPAvector2 = malloc(n * sizeof(unsigned int));
unsigned int *rotcevAPAP = malloc(n * sizeof(unsigned int));
PAPAvector2=randomXOR(123456789,n);

PAPAvector2=permutari(n,PAPAvector2);
//for(int i=p-1; i>=0; i--)
  //  printf("%d\n",PAPAvector2[i]);


PermutarePixeli(nume_img_sursa, nume_img_grayscale,PAPAvector2);
//grayscale_image("test_grayscale.bmp", "IMAGINE_FINALA.bmp");

//rotcevAPAP=PAPAvector2;

//for(int i=(n+1)/2;i>=1;i--)
    //rotcevAPAP[(n+1)/2-i+1]=PAPAvector2[i];

ReversePermutarePixeli(nume_img_grayscale, nume_img_grayscale2,rotcevAPAP);
    return 0;
}

