/* Generator ODE from chemistry */
#include<stdio.h>
#include<string.h>

struct specie { char name[8]; };

specie *listspec;
int num_species;

int is_species(char *spec)
{
 for(int i=0 ; i<num_species ; i++)
  if(strcmp(spec,listspec[i].name) == 0)
   return i;
 return -1;
}

int main()
{
 int i, num_reactions, akf, bkf, ykf, zkf, numA, numB, numY, numZ;
 char fileline[128], rtype[4], specA[8], specB[8], specY[8], specZ[8];
 FILE *fin  = fopen("chemistry","r");
 FILE *foutrhs = fopen("oderhs.cpp","w");
 FILE *foutjcb = fopen("odejcb.cpp","w");

 fprintf(foutrhs,"void rhs_ccode(int *n, double *x, double *y, double *f, void *upar)\n{\n");
 fprintf(foutjcb,"void jacmat_ccode(int *n, double *x, double *y, double *dfy, void *upar)\n{\n");

 fgets(fileline,128,fin);
 sscanf(fileline,"SPECIES %d\n",&num_species);

 listspec = new specie[num_species];
 for(i=0 ; i<num_species ; i++)
 {
   fgets(fileline,128,fin);
   sscanf(fileline,"%s\n",listspec[i].name);
 }

 fgets(fileline,128,fin);
 sscanf(fileline,"REACTIONS %d\n",&num_reactions);

 for(i=0 ; i<num_reactions ; i++)
 {
   fgets(fileline,128,fin);
   sscanf(fileline,"%s",rtype);

   if(strcmp(rtype,"2a1") == 0)  // a*A + b*B => y*Y
   {            
      sscanf(fileline,"%s %d*%s + %d*%s => %d*%s",rtype,&akf,specA,&bkf,specB,&ykf,specY);
      numA = is_species(specA);
      numB = is_species(specB); 
      numY = is_species(specY);
      // dA/dt = -a*k*A^a*B^b
      fprintf(foutrhs,"  f[%d] = f[%d] - %.1lf * k_%d * pow(y[%d],%d) * pow(y[%d],%d);\n",numA,numA,1.0*akf,i,numA,akf,numB,bkf);
      fprintf(foutjcb,"  dfy[%d*%d+%d] = dfy[%d*%d+%d] - %.1lf * k_%d * pow(y[%d],%d) * pow(y[%d],%d);\n",numA,num_species,numA,numA,num_species,numA,1.0*akf*akf,i,numA,akf-1,numB,bkf);
      fprintf(foutjcb,"  dfy[%d*%d+%d] = dfy[%d*%d+%d] - %.1lf * k_%d * pow(y[%d],%d) * pow(y[%d],%d);\n",numA,num_species,numB,numA,num_species,numB,1.0*akf*bkf,i,numA,akf,numB,bkf-1);
      // dB/dt = -b*k*A^a*B^b
      fprintf(foutrhs,"  f[%d] = f[%d] - %.1lf * k_%d * pow(y[%d],%d) * pow(y[%d],%d);\n",numB,numB,1.0*bkf,i,numA,akf,numB,bkf);
      fprintf(foutjcb,"  dfy[%d*%d+%d] = dfy[%d*%d+%d] - %.1lf * k_%d * pow(y[%d],%d) * pow(y[%d],%d);\n",numB,num_species,numA,numB,num_species,numA,1.0*bkf*akf,i,numA,akf-1,numB,bkf);
      fprintf(foutjcb,"  dfy[%d*%d+%d] = dfy[%d*%d+%d] - %.1lf * k_%d * pow(y[%d],%d) * pow(y[%d],%d);\n",numB,num_species,numB,numB,num_species,numB,1.0*bkf*bkf,i,numA,akf,numB,bkf-1);
      // dY/dt =  y*k*A^a*B^b
      fprintf(foutrhs,"  f[%d] = f[%d] + %.1lf * k_%d * pow(y[%d],%d) * pow(y[%d],%d);\n",numY,numY,1.0*ykf,i,numA,akf,numB,bkf);
      fprintf(foutjcb,"  dfy[%d*%d+%d] = dfy[%d*%d+%d] + %.1lf * k_%d * pow(y[%d],%d) * pow(y[%d],%d);\n",numY,num_species,numA,numY,num_species,numA,1.0*ykf*akf,i,numA,akf-1,numB,bkf);
      fprintf(foutjcb,"  dfy[%d*%d+%d] = dfy[%d*%d+%d] + %.1lf * k_%d * pow(y[%d],%d) * pow(y[%d],%d);\n",numY,num_species,numB,numY,num_species,numB,1.0*ykf*bkf,i,numA,akf,numB,bkf-1);
   }

   if(strcmp(rtype,"1a2") == 0) // a*A => y*Y + z*Z
   {
      sscanf(fileline,"%s %d*%s => %d*%s + %d*%s",rtype,&akf,specA,&ykf,specY,&zkf,specZ);
      numA = is_species(specA);
      numY = is_species(specY);
      numZ = is_species(specZ);
      // dA/dt = -a*k*A^a
      fprintf(foutrhs,"  f[%d] = f[%d] - %.1lf * k_%d * pow(y[%d],%d);\n",numA,numA,1.0*akf,i,numA,akf);
      fprintf(foutjcb,"  dfy[%d*%d+%d] = dfy[%d*%d+%d] - %.1lf * k_%d * pow(y[%d],%d);\n",numA,num_species,numA,numA,num_species,numA,1.0*akf*akf,i,numA,akf-1);
      // dY/dt =  y*k*A^a
      fprintf(foutrhs,"  f[%d] = f[%d] + %.1lf * k_%d * pow(y[%d],%d);\n",numY,numY,1.0*ykf,i,numA,akf);
      fprintf(foutjcb,"  dfy[%d*%d+%d] = dfy[%d*%d+%d] + %.1lf * k_%d * pow(y[%d],%d) ;\n",numY,num_species,numA,numY,num_species,numA,1.0*ykf*akf,i,numA,akf-1);
      // dZ/dt =  z*k*A^a
      fprintf(foutrhs,"  f[%d] = f[%d] + %.1lf * k_%d * pow(y[%d],%d);\n",numZ,numZ,1.0*zkf,i,numA,akf);
      fprintf(foutjcb,"  dfy[%d*%d+%d] = dfy[%d*%d+%d] + %.1lf * k_%d * pow(y[%d],%d) ;\n",numZ,num_species,numA,numZ,num_species,numA,1.0*zkf*akf,i,numA,akf-1);
   }

   if(strcmp(rtype,"2a2") == 0) // a*A + b*B => y*Y + z*Z
   {
      sscanf(fileline,"%s %d*%s + %d*%s => %d*%s + %d*%s",rtype,&akf,specA,&bkf,specB,&ykf,specY,&zkf,specZ);
      numA = is_species(specA);
      numB = is_species(specB); 
      numY = is_species(specY);
      numZ = is_species(specZ);
      // dA/dt = -a*k*A^a*B^b
      fprintf(foutrhs,"  f[%d] = f[%d] - %.1lf * k_%d * pow(y[%d],%d) * pow(y[%d],%d);\n",numA,numA,1.0*akf,i,numA,akf,numB,bkf);
      fprintf(foutjcb,"  dfy[%d*%d+%d] = dfy[%d*%d+%d] - %.1lf * k_%d * pow(y[%d],%d) * pow(y[%d],%d);\n",numA,num_species,numA,numA,num_species,numA,1.0*akf*akf,i,numA,akf-1,numB,bkf);
      fprintf(foutjcb,"  dfy[%d*%d+%d] = dfy[%d*%d+%d] - %.1lf * k_%d * pow(y[%d],%d) * pow(y[%d],%d);\n",numA,num_species,numB,numA,num_species,numB,1.0*akf*bkf,i,numA,akf,numB,bkf-1);
      // dB/dt = -b*k*A^a*B^b
      fprintf(foutrhs,"  f[%d] = f[%d] - %.1lf * k_%d * pow(y[%d],%d) * pow(y[%d],%d);\n",numB,numB,1.0*bkf,i,numA,akf,numB,bkf);
      fprintf(foutjcb,"  dfy[%d*%d+%d] = dfy[%d*%d+%d] - %.1lf * k_%d * pow(y[%d],%d) * pow(y[%d],%d);\n",numB,num_species,numA,numB,num_species,numA,1.0*bkf*akf,i,numA,akf-1,numB,bkf);
      fprintf(foutjcb,"  dfy[%d*%d+%d] = dfy[%d*%d+%d] - %.1lf * k_%d * pow(y[%d],%d) * pow(y[%d],%d);\n",numB,num_species,numB,numB,num_species,numB,1.0*bkf*bkf,i,numA,akf,numB,bkf-1);
      // dY/dt =  y*k*A^a*B^b
      fprintf(foutrhs,"  f[%d] = f[%d] + %.1lf * k_%d * pow(y[%d],%d) * pow(y[%d],%d);\n",numY,numY,1.0*ykf,i,numA,akf,numB,bkf);
      fprintf(foutjcb,"  dfy[%d*%d+%d] = dfy[%d*%d+%d] + %.1lf * k_%d * pow(y[%d],%d) * pow(y[%d],%d);\n",numY,num_species,numA,numY,num_species,numA,1.0*ykf*akf,i,numA,akf-1,numB,bkf);
      fprintf(foutjcb,"  dfy[%d*%d+%d] = dfy[%d*%d+%d] + %.1lf * k_%d * pow(y[%d],%d) * pow(y[%d],%d);\n",numY,num_species,numB,numY,num_species,numB,1.0*ykf*bkf,i,numA,akf,numB,bkf-1);
      // dZ/dt =  z*k*A^a*B^b
      fprintf(foutrhs,"  f[%d] = f[%d] + %.1lf * k_%d * pow(y[%d],%d) * pow(y[%d],%d);\n",numZ,numZ,1.0*zkf,i,numA,akf,numB,bkf);
      fprintf(foutjcb,"  dfy[%d*%d+%d] = dfy[%d*%d+%d] + %.1lf * k_%d * pow(y[%d],%d) * pow(y[%d],%d);\n",numZ,num_species,numA,numZ,num_species,numA,1.0*zkf*akf,i,numA,akf-1,numB,bkf);
      fprintf(foutjcb,"  dfy[%d*%d+%d] = dfy[%d*%d+%d] + %.1lf * k_%d * pow(y[%d],%d) * pow(y[%d],%d);\n",numZ,num_species,numB,numZ,num_species,numB,1.0*zkf*bkf,i,numA,akf,numB,bkf-1);
   }

 }

 fprintf(foutrhs,"}\n");
 fprintf(foutjcb,"}\n");

 delete listspec;
 fclose(fin);
 fclose(foutrhs);
 fclose(foutjcb);
}