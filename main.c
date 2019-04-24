#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#define datafile "allrecords.data"
#define indexfile "index.data"

struct details{
    char name[50];
    int roll;
    int marks;
    int count;
};

int saveIndexInSortedOrder(int,int);
void saveIndexInFile(int,int);
int** loadIndexesFromFile();
void getAllIndexes();
void insertData();
int writeStructureIntoTextFile(struct details*);
int readAllStructureFromTextFile();
int numberOfRecords();
struct details searchByRoll(int);
struct details searchByName(char*);
struct details searchByMarks(int);
void getSearchedData(int);
int binarySearch(int*,int,int,int);
int isExist(int);
int updateName(int,char*);
int updateRoll(int,int);
int updateMarks(int,int);
void deleteRecords(int);

void deleteRecords(int roll){
    int devider=0;
    struct details specific_data=searchByRoll(roll);
    int size=numberOfRecords();
    if(roll==specific_data.roll){
        struct details head;
        struct details data_for_delete;
        FILE *file=fopen(datafile,"r+b");
        if(file!=NULL){
            fseek(file,-sizeof(struct details),SEEK_END);
            if(fread(&data_for_delete,sizeof(struct details),1,file)==1){
                strcpy(head.name,data_for_delete.name);
                head.roll=data_for_delete.roll;
                head.marks=data_for_delete.marks;
                head.count=specific_data.count;
            }
            else
                printf("Something went wrong\n");

            fseek(file,(specific_data.count-1)*sizeof(struct details),SEEK_SET);
            if(fwrite(&head,sizeof(struct details),1,file)==1)
                printf("Records deleted successfully\n");
            else
                printf("Something went wrong\n");

            fseek(file,-sizeof(struct details),SEEK_END);
            trunc(*((double*)file));
        }
        fclose(file);
        int mat1[100][2];
        int **index_mat=loadIndexesFromFile(indexfile);
        for(int i=0;i<size;i++){
            for(int j=0;j<2;j++){
                mat1[i][j]=index_mat[i][j];
            }
        }
        for(int i=0;i<size;i++){
            if(roll==mat1[i][0])
                devider=i;
        }
        for(int i=devider;i<size;i++){
            mat1[i][0]=mat1[i+1][0];
            mat1[i][1]=mat1[i+1][1];
        }
        FILE *file1=fopen(indexfile,"w");
        if(file!=NULL){
            fwrite(mat1,sizeof(int),2*size-2,file1);
            printf("Records deleted successfully\n");
        }
        fclose(file);
    }
    else
        printf("\nRecord does not exist.\n");
}

int updateName(int roll,char newname[]){
    struct details specific_data=searchByRoll(roll);
    if(roll==specific_data.roll){
        struct details *head=(struct details*)malloc(sizeof( struct details));
        strcpy(head->name,newname);
        head->roll=specific_data.roll;
        head->marks=specific_data.marks;
        head->count=specific_data.count;
        FILE *file=fopen(datafile,"r+");
        if(file!=NULL){
            fseek(file,(specific_data.count-1)*sizeof(struct details),SEEK_SET);
            fwrite(head,sizeof(struct details),1,file);
            fclose(file);
            return 1;
        }

    }
    return 0;
}

int updateMarks(int roll,int newmarks){
    struct details specific_data=searchByRoll(roll);
    if(roll==specific_data.roll){
        struct details *head=(struct details*)malloc(sizeof( struct details));
        strcpy(head->name,specific_data.name);
        head->roll=specific_data.roll;
        head->marks=newmarks;
        head->count=specific_data.count;
        FILE *file=fopen(datafile,"r+");
        if(file!=NULL){
            fseek(file,(specific_data.count-1)*sizeof(struct details),SEEK_SET);
            fwrite(head,sizeof(struct details),1,file);
            fclose(file);
            return 1;
        }

    }
    return 0;
}

int updateRoll(int roll,int newroll){
    int mat1[100][2];
    int upwarddevider=-1;
    int corespon_index;
    struct details specific_data=searchByRoll(roll);
    if(roll==specific_data.roll){
        int size=numberOfRecords(indexfile);
        struct details *head=(struct details*)malloc(sizeof( struct details));
        strcpy(head->name,specific_data.name);
        head->roll=newroll;
        head->marks=specific_data.marks;
        head->count=specific_data.count;
        FILE *file=fopen(datafile,"r+");
        if(file!=NULL){
            fseek(file,(specific_data.count-1)*sizeof(struct details),SEEK_SET);
            if(fwrite(head,sizeof(struct details),1,file)!=1)
                printf("\nData not updated in datafile...something went wrong.\n");
        }
        fclose(file);

        int **index_mat=loadIndexesFromFile(indexfile);
        for(int i=0;i<size;i++){
            for(int j=0;j<2;j++){
                mat1[i][j]=index_mat[i][j];
            }
        }
        for(int j=0; j<size; j++) {
            if(roll==mat1[j][0]){
                corespon_index=mat1[j][1];
                upwarddevider=j;
            }
        }
        if(upwarddevider!=-1){
            for(int i=upwarddevider;i<size;i++){
                    mat1[i][0]=mat1[i+1][0];
                    mat1[i][1]=mat1[i+1][1];
                }
            FILE *mat_writer;
            mat_writer=fopen(indexfile,"wb");
            if(mat_writer!=NULL){
                if(fwrite(mat1, sizeof(int),2*size,mat_writer)!=2*size){
                    printf("\nerror: file not written correctly..\n");
                }
                fclose(mat_writer);
            }else{
                printf("file not loaded correctly for writing..");
            }
            if(saveIndexInSortedOrder(newroll,corespon_index)==1)
                return 1;
        }
    }
    return 0;
}

void getSearchedData(int roll){
    struct details specific_data=searchByRoll(roll);
    if(roll==specific_data.roll){
        printf("\n Roll\t\tName\t\tMarks\n");
        printf("========================================\n");
        printf(" %d\t\t%s\t\t%d\n",specific_data.roll,specific_data.name,specific_data.marks);
    }
    else{
        printf("\nData not Found.\n");
    }
}

int binarySearch(int arr[], int l, int r, int x){
    while (l <= r) {
        int m = l + (r-l)/2;
        if (arr[m] == x)
            return m;
        if (arr[m] < x)
            l = m + 1;
        else
            r = m - 1;
      }
    return -1;
}

int isExist(int roll){
    int temp[100];
    int **mat1=loadIndexesFromFile(indexfile);
    int size=numberOfRecords(datafile);
    for(int j=0;j<size; j++) {
        temp[j]=mat1[j][0];
    }
    if(binarySearch(temp,0,size-1,roll)!=-1)
        return 1;
    else
        return 0;
}

struct details searchByRoll(int roll){
    int temp[100];
    int **mat1=loadIndexesFromFile(indexfile);
    int size=numberOfRecords(datafile);
    struct details specific_data;
    for(int j=0;j<size; j++) {
        temp[j]=mat1[j][0];
    }
    int result=binarySearch(temp,0,size-1,roll);
    int search_index=mat1[result][1];
    if(result!=-1){
        FILE *file=fopen(datafile,"rb");
        if(file!=NULL){
            fseek(file,(search_index-1)*sizeof(struct details),SEEK_SET);
            fread(&specific_data,sizeof(struct details),1,file);
        }
        fclose(file);
    }
    else{
        printf("\nData does not exist\n");
    }
    return  specific_data;
}

int saveIndexInSortedOrder(int roll,int index){
    int downwarddevider=0;
    int mat[100][2];
    int size=numberOfRecords(datafile);
    if(size==0){
        saveIndexInFile(roll,index);
        return 0;
    }else{
        int **index_mat=loadIndexesFromFile(indexfile);
        for(int i=0;i<size;i++){
            for(int j=0;j<2;j++){
                mat[i][j]=index_mat[i][j];
            }
        }
        while(roll>=mat[downwarddevider][0] && downwarddevider<=size){
             downwarddevider++;
        }
        for(int i=size;i>=downwarddevider;i--){
            mat[i+1][0]=mat[i][0];
            mat[i+1][1]=mat[i][1];
        }
        mat[downwarddevider][0]=roll;
        mat[downwarddevider][1]=index;

        FILE *mat_writer;
        mat_writer=fopen(indexfile,"wb");
        if(mat_writer!=NULL){
            if(fwrite(mat, sizeof(int),2*size+2,mat_writer)!=2*size+2){
                printf("\nerror: file not written correctly..\n");
            }
            fclose(mat_writer);
        }else{
            printf("file not loaded correctly for writing..");
        }
    }
    return 1;

}

void saveIndexInFile(int roll,int index){
    int mat[1][2];
    mat[0][0]=roll;
    mat[0][1]=index;
    FILE *mat_writer;
    mat_writer=fopen(indexfile,"ab");
    if(mat_writer!=NULL){
        if(fwrite(mat, sizeof(int),2,mat_writer)!=2){
            printf("\nerror: file not written correctly..\n");
        }
        fclose(mat_writer);
    }else{
        printf("file not loaded correctly for writing..");
    }

}

int** loadIndexesFromFile(){
    int mat2[100][2];
    int size=numberOfRecords(datafile);
    int **mat1=(int**)malloc(10*sizeof(int*));
    for(int i=0;i<10;i++){
        mat1[i]=(int*)malloc(2*sizeof(int));
    }
    FILE *mat_reader=fopen(indexfile,"r+b");
    if(mat_reader!=NULL){
        if(fread(mat2, sizeof(int),2*size,mat_reader)!=2*size){
            printf("\nerror: file not read properly..\n");
        }
        fclose(mat_reader);
        for(int i=0;i<size;i++){
            for(int j=0;j<2;j++){
                mat1[i][j]=mat2[i][j];
            }
        }
    }

    return mat1;
}

void getAllIndexes(){
    int **mat1=loadIndexesFromFile(indexfile);
    printf("\nRoll\tIndex\n");
    printf("================\n");
    for(int j=0; j<numberOfRecords(datafile); j++) {
            for(int i=0; i<2; i++)
                printf("%d\t ", mat1[j][i]);
            printf("\n");
        }
}



void insertData(){
    struct details *head=(struct details*)malloc(sizeof( struct details));
    char name[50];
    int roll,marks,count;
    printf("Enter name:");
    scanf("%s",name);
    printf("Enter roll number: ");
    scanf("%d",&roll);
    printf("Enter marks: ");
    scanf("%d",&marks);
    strcpy(head->name,name);
    head->roll=roll;
    head->marks=marks;
    if(numberOfRecords(datafile)==0)
         count=1;
    else
        count=numberOfRecords(datafile)+1;
    head->count=count;
    writeStructureIntoTextFile(head);
    saveIndexInSortedOrder(roll,count);

}

int writeStructureIntoTextFile(struct details *stu_details){
    FILE *outfile;
    outfile = fopen (datafile, "a");
    if (outfile == NULL)
    {
        fprintf(stderr, "\nError opend file\n");
        exit (1);
    }
     fwrite (stu_details, sizeof(struct details), 1,outfile);
     fclose(outfile);
     printf("\nData inserted in file..\n");
     return 0;
}

int numberOfRecords(){
    struct details row_counter;
    FILE * file= fopen(datafile, "r+b");
    if (file!= NULL) {
        fseek(file,-1*sizeof(struct details),SEEK_END);
        fread(&row_counter, sizeof(struct details), 1, file);
        fclose(file);
        return row_counter.count;
    }
    return 0;
}

int readAllStructureFromTextFile(){
    struct details all;
    FILE * file= fopen(datafile, "r+b");
    if (file!= NULL) {
        printf("\nIndex\tName\t\tRoll\t\tMarks\n");
        printf("====================================================\n");
        for(int i=0;i<numberOfRecords(datafile);i++){
            fread(&all, sizeof(struct details), 1, file);
            printf("  %d\t%s\t\t%d\t\t%d\n",all.count,all.name,all.roll,all.marks);
        }
        fclose(file);
    }else{
        printf("Error: file does not exist...");
    }

    return 0;
}

int main() {
        int choice,subchoiceForUpdate;
        int roll,newroll,newmarks;
        char newname[50];
        while(1){
            printf("Select Operation:-\n");
            printf(" 1. Add"
                   "\n 2. View all"
                   "\n 3. Search"
                   "\n 4. Update"
                   "\n 5. Delete"
                   "\n 6. Exit"
                   "\nEnter choice: ");
            scanf("%d",&choice);
            system("cls");
            switch(choice){
            case 1:
                insertData();
                break;
            case 2:
                readAllStructureFromTextFile(datafile);
                break;
            case 3:
                printf("\nEnter Roll Number:");
                scanf("%d",&roll);
                if(isExist(roll)==1)
                    getSearchedData(roll);
                else
                    printf("\nInvalid roll number\n\n");
                break;
            case 4:
                printf("\nEnter roll Number:");
                scanf("%d",&roll);
                if(isExist(roll)==1){
                    printf("What do you want to update?select choices.\n");
                    printf("\n\t1.Name"
                           "\n\t2.Roll number"
                           "\n\t3.Marks"
                           "\n\t4.Update all"
                           "\n\tEnter choice: ");
                    scanf("%d",&subchoiceForUpdate);
                    switch(subchoiceForUpdate){
                    case 1:
                        printf("Enter new Name:");
                        scanf("%s",newname);
                        if(updateName(roll,newname)==1)
                            printf("\nName updated successfully.\n\n");
                        break;
                    case 2:
                        printf("Enter new Roll number: ");
                        scanf("%d",&newroll);
                        if(updateRoll(roll,newroll)==1)
                            printf("\nRoll number updated successfully.\n\n");
                        break;
                    case 3:
                        printf("Enter new Marks: ");
                        scanf("%d",&newmarks);
                        if(updateMarks(roll,newmarks)==1)
                            printf("\nMarks updated successfully.\n\n");
                        break;
                    case 4:
                        printf("Enter new Roll number: ");
                        scanf("%d",&newroll);
                        printf("Enter new Name:");
                        scanf("%s",newname);
                        printf("Enter new Marks: ");
                        scanf("%d",&newmarks);
                        if(updateName(roll,newname)==1 &&
                           updateMarks(roll,newmarks)==1 &&
                           updateRoll(roll,newroll)){
                            printf("\nData updated successfully.\n\n");
                           }
                        break;
                    default:
                        printf("\nInvalid selection\n\n");
                    }
                }
                else
                    printf("\nInvalid Roll number\n\n");
            break;
            case 5:
                printf("\nEnter roll Number:");
                scanf("%d",&roll);
                if(isExist(roll)==1)
                    deleteRecords(roll);
                else
                    printf("\nInvalid Roll number\n\n");
                break;
            case 6:
                exit(0);
            case 7:
                getAllIndexes(indexfile);
                break;
            default:
                    printf("\nInvalid selection\n\n");
            }
            printf("\n");
        }
        return 0;

}
