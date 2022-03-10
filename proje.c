#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <ctype.h> 
#include <locale.h> 

typedef struct student{

	char id[9];
	char name[20];
	char surname[20];
	int  credit;
	int  lectureNumber;
	struct student *next;
	struct student *prev;
	char **lecture_code;
}STUDENT;

typedef struct lecture{

	char code[8];
	char name[40];
	int credit ;
	int quota;
	int counter; // number of students who take this course
	char **enrolled_students;
	struct lecture *next;


}LECTURE;

void Student_travel(STUDENT *head);
void Lecture_travel(LECTURE *head);
void open_lecture_manuel(LECTURE  **head );
void close_lecture(LECTURE **head,STUDENT *std_head,int*,char* date);

void load_lectures(LECTURE  **head,char* buffer );
void load_student(STUDENT  **head,STUDENT **tail,char* buffer,int  );

void new_student(STUDENT **head,STUDENT **tail,int );
void delete_student(STUDENT **head,STUDENT **tail,LECTURE *lect_head);

void enroll(LECTURE *Lhead,STUDENT *Shead,int,int,char*,int,int*,char*);
void sort(char **list,int n);
void drop(LECTURE *Lhead,STUDENT *Shead,char *date);

void createLecturesFile(LECTURE *head,STUDENT *Shead);
void createStudentFile(STUDENT *head);

void Lecture_Write(LECTURE *head,FILE *fp,char delimiter );
void Student_Write(STUDENT *head,FILE *fp,char delimiter );

void log_update(char* code,char* id,int mode,int *log_counter,char* date);
void logChangeClosed(char* code);

int main(){
	setlocale(LC_ALL, "Turkish");
	int log_counter=10000;
	char date[11];
	int option,lecture_limit,credit_limit;
	STUDENT *student_head=NULL;
	STUDENT *student_tail=NULL;
	LECTURE *lecture_head=NULL;
	char buffer[255];
	
	printf("enter the date(dd/mm/yyyy):");
	scanf("%s",date);

	printf("enter the lecture limit per student ");
	scanf("%d",&lecture_limit);
	printf("enter the credit limit per student ");
	scanf("%d",&credit_limit);
	

	FILE *fp_lecture=fopen("dersler.txt","r");
	
	while(fgets(buffer,255,fp_lecture)!=NULL){
		load_lectures(&lecture_head,buffer);
	}	
	
	fclose(fp_lecture);
	
	FILE *fp_student=fopen("ogrenciler.txt","r");

	while(fgets(buffer,255,fp_student)!=NULL){
		load_student(&student_head,&student_tail, buffer,lecture_limit);
	}
	fclose(fp_student);
	
	FILE *fp_log=fopen("OgrenciDersKayit.txt","r+");
	while(fgets(buffer,255,fp_log)!=NULL){
		enroll(lecture_head,student_head,lecture_limit,credit_limit,buffer,2,&log_counter,date); 
	}
		fclose(fp_log);


	do{

		printf("\n\nSelect operation \n 1-> Add new lecture\n 2-> Close a lecture\n 3-> Add new student\n 4-> Delete a student\n 5-> Enrollment\n 6-> Dropp lecture\n  7-> Create a lecture.txt file\n 8-> Create a student.txt file\n 9-> Lecture list \n 10-> Student list \n\n -1-> update input files and exit\n\n");
		scanf("%d",&option);


		switch (option){
			case 1 : 
				open_lecture_manuel(&lecture_head);
				Lecture_travel(lecture_head);
				
				break ;
			case 2 : 	

				close_lecture(&lecture_head,student_head,&log_counter,date);
				Lecture_travel(lecture_head);
		
				break ;
			case 3 :
				new_student(&student_head,&student_tail,lecture_limit);
				 Student_travel(student_head);
			
				break ;
			case 4 : 
				delete_student(&student_head,&student_tail,lecture_head); 
				Student_travel(student_head);
	
				break ;
			case 5 : 
				enroll(lecture_head,student_head,lecture_limit,credit_limit,buffer,1,&log_counter,date); 
				Student_travel(student_head);
			
				break ;
			case 6 :
				drop(lecture_head,student_head,date);
			
				break;
			case 7 :
				createLecturesFile(lecture_head,student_head);
			
				break;
			case 8 :
				createStudentFile(student_head);
			
				break;
			case 9 :
				Lecture_travel(lecture_head);
				
			break;	
			case 10 :
				 Student_travel(student_head);

			
			break;			
		}
	}while(option!=-1);
	
 	fp_lecture=fopen("dersler.txt","w");
	Lecture_Write(lecture_head,fp_lecture,',');
	fclose(fp_lecture);

	fp_student=fopen("ogrenciler.txt","w");
	Student_Write(student_head,fp_student,',');
	fclose(fp_lecture);

	return 0;
}

void Lecture_travel(LECTURE *head){
	LECTURE *tmp=head;
	printf("\n\nCODE\t\tNAME\t\t\t\t\tCREDIT   \tQUOTA\n");
	while(tmp!=NULL){
		printf("\n%-12s%-50s%-12d%-12d",tmp->code,tmp->name,tmp->credit,tmp->quota);
		tmp = tmp->next;
	}
	printf("\n");
}

int isLecture_already_exist(LECTURE *head,char* code){
	LECTURE *tmp=head;
	while(tmp!=NULL && strcmp((tmp->code),(code))!=0){
		tmp = tmp->next;
	}
	if(tmp==NULL){
		return 0;
	}
	else{
		return 1;
	}
}


void load_lectures(LECTURE  **head,char* buffer ){
	char* token;
	LECTURE *new_lect;
	LECTURE *tmp;
	int res,i;
	new_lect = (LECTURE*) malloc(sizeof(LECTURE));
	new_lect->next=NULL;
	new_lect->counter=0;
	
	token=strtok(buffer,",\0");
	strcpy(new_lect->code,token);
	token=strtok(NULL,",\0");
	strcpy(new_lect->name,(token));
	token=strtok(NULL,",\0");
	new_lect->credit=atoi(token);
	token=strtok(NULL,",\0");
	new_lect->quota=atoi(token);	
	
	new_lect->enrolled_students=(char**)malloc(sizeof(char*)*new_lect->quota);	// dersi alan öðrencilerin id'lerini kayetmek için gerekli alan ayrýldý
	for(i=0;i<new_lect->quota;i++){
		new_lect->enrolled_students[i]=(char*)malloc(10*sizeof(char));
	}
		
	tmp = *head;
	if(*head==NULL ){ // liste boþ ise
		new_lect->next=*head;
		*head=new_lect;
	}
	else if((strcmp(tmp->code,new_lect->code)>0)){ // eklenecek eleman 2. eleman olacaksa
		new_lect->next=*head;
		*head=new_lect;
	}
	else{	// eleman code'una göre uygun yere yerleþtirilecekse
		while(tmp->next!= NULL && strcmp(tmp->next->code,new_lect->code)<0){
			tmp= tmp->next;
		}
		if(tmp->next==NULL){
			tmp->next=new_lect;
			new_lect->next=NULL;
		}
		else{
			new_lect->next = tmp->next;
			tmp->next=new_lect;
		}
	}
	

}


void open_lecture_manuel(LECTURE  **head ){
	LECTURE *new_lect;
	LECTURE *tmp;
	int res,i;
	new_lect = (LECTURE*) malloc(sizeof(LECTURE));
	new_lect->next=NULL;

	new_lect->counter=0;
	printf("Enter lectures code:");
	scanf("%s",new_lect->code );
	printf("Enter lecture credit: ");
	scanf("%d",&new_lect->credit );
	printf("Enter lecture quota: ");
	scanf("%d",&new_lect->quota);
	printf("Enter the lecture name: ");
	fflush(stdin);
	scanf("%[^\n]",new_lect->name);
	
	
	

	if(isLecture_already_exist(*head,new_lect->code)==1){
		printf("\n\n !!! This lecture is already exist !!!\n");
		return ;
	}
	
	
	new_lect->enrolled_students=(char**)malloc(sizeof(char*)*new_lect->quota);	// dersi alan öðrencilerin id'lerini kayetmek için gerekli alan ayrýldý
	for(i=0;i<new_lect->quota;i++){
		new_lect->enrolled_students[i]=(char*)malloc(10*sizeof(char));
	}
	
	
	
	tmp = *head;
	if(*head==NULL ){
		new_lect->next=*head;
		*head=new_lect;
	}
	else if((strcmp(tmp->code,new_lect->code)>0)){
		new_lect->next=*head;
		*head=new_lect;
	}
	else{
		while(tmp->next!= NULL && strcmp(tmp->next->code,new_lect->code)<0){
			tmp= tmp->next;
		}
		if(tmp->next==NULL){
			tmp->next=new_lect;
			new_lect->next=NULL;
		}
		else{
			new_lect->next = tmp->next;
			tmp->next=new_lect;
		}
	}
	return ;
}


void close_lecture(LECTURE **head,STUDENT *std_head,int* log_counter,char* date){
	LECTURE *tmp,*tmp2;
	char code[8];

	int i,j;
	STUDENT *tmpS;
	tmpS=std_head;
	printf("Enter the code of lecture you want to close :");
	scanf("%s",code);
	if((*head)==NULL){
		printf("\nThere is no lecture on list");
		return ;
	}
	if(strcmp((*head)->code,code)==0){
		tmp=(*head)->next;
		printf("%s closed\n",(*head)->code);
		free(*head);
		(*head)->next=NULL;
		*head = tmp;
	//	log_update(code,"",3,log_counter,date);
		logChangeClosed(code);
		// head nodundaki dersi alan öðrencilerin aldýðý dersler listesinden bu ders silinir
		for(i=0;i< (*head)->counter ;i++){
				
			while(strcmp((*head)->enrolled_students[i],tmpS->id)!=0 && tmpS->next!=NULL){ //  öðrenciler listesinden aranan nod bulunur
				tmpS=tmpS->next;
					
			}
			if(tmpS->next!=NULL){
				j=0;
				(tmpS->credit)-=(*head)->credit ;    // ogrenciye kredi hakký geri verilir (aldýðý kredi düþürülür)
				tmpS->lectureNumber--;         // ogrenciye alabileceði ders sayýsý 1 arttýrýlýr (yani aldýðý ders sayýsý düþürülür)
				while(strcmp(tmpS->lecture_code[j],code)!=0 && j<tmpS->lectureNumber){
					j++;
				}
				if(j!=tmpS->lectureNumber){
					strcpy(tmpS->lecture_code[j],"");
				}
			}
			
		}
			
	}
	else{
		tmp = *head;
		while(tmp->next!=NULL && strcmp(tmp->next->code,code)!=0){
			tmp = tmp->next;
		}
		if( tmp->next==NULL){
			printf("This lecture does not exist!\n");
		}
		else{
			tmp2=tmp->next;
			//ogrenciler listesinde bu dersi alan öðrenciler bulup aldýðý dersler listesinden bu dersi çýkarýr
			for(i=0;i< tmp2->counter ;i++){
				
				while(strcmp(tmp2->enrolled_students[i],tmpS->id)!=0 && tmpS->next!=NULL){ //  öðrenciler listesinden aranan nod bulunur
					tmpS=tmpS->next;
					
				}
				if(tmpS->next!=NULL){
					j=0;
						(tmpS->credit)-=tmp2->credit ;    // ogrenciye kredi hakký geri verilir (aldýðý kredi düþürülür)
						tmpS->lectureNumber--;          // ogrenciye alabileceði ders sayýsý 1 arttýrýlýr
					while(strcmp(tmpS->lecture_code[j],code)!=0 && j<tmpS->lectureNumber){
						j++;
					}
					if(j!=tmpS->lectureNumber){
						strcpy(tmpS->lecture_code[j],"");
					}
				}
			
			}
			logChangeClosed(code);
			tmp->next=tmp->next->next;
			printf("\n%s lecture closed \n",tmp2->code);
			free(tmp2);
			
			tmp2=NULL;
		}
	}
}


void Student_travel(STUDENT *head){
	STUDENT *tmp=head;
	printf("\n\nID\t    NAME\t\tSURNAME\t\tCREDIT\t\tLECTURES\n");
	while(tmp!=NULL){
		printf("\n%-12s%-15s\t%-15s\t\t%-12d\t%-12d",tmp->id,tmp->name,tmp->surname,tmp->credit,tmp->lectureNumber);
		tmp = tmp->next;
	}
	printf("\n");
}




int isStudent_already_exist(STUDENT *head,char* id){
	STUDENT *tmp=head;
	while(tmp!=NULL && strcmp((tmp->id),(id))!=0){
		tmp = tmp->next;
	}
	if(tmp==NULL){
		return 0;
	}
	else{
		return 1;
	}
}

void load_student(STUDENT **head ,STUDENT **tail,char* buffer,int lecture_limit){
	STUDENT *new_stud;
	STUDENT *tmp;
	char* token;
	int res,i;
	new_stud = (STUDENT*) malloc(sizeof(STUDENT));
	new_stud->next=NULL;
	new_stud->prev=NULL;
	
	token=strtok(buffer,",");
	strcpy(new_stud->id,token);
	token=strtok(NULL,",");
	strcpy(new_stud->name,(token));
	token=strtok(NULL,",");
	strcpy(new_stud->surname,(token));
	token=strtok(NULL,",");
	new_stud->credit=0;
	token=strtok(NULL,",");
	new_stud->lectureNumber=0;
	
	
	
		
	new_stud->lecture_code=(char**)malloc(lecture_limit *sizeof(char*));	// öðrencinin aldýðý dersleri saklar
	for(i=0;i<lecture_limit;i++){
		new_stud->lecture_code[i]=(char*)malloc(10*sizeof(char));
	}
	
		tmp = *head;
	if(*head==NULL   ){
		new_stud->next=*head;
		*tail=new_stud;
		*head=new_stud;
		(*head)->prev=NULL;
	}
	else if((strcmp(tmp->id,new_stud->id)>0)){
		new_stud->next=*head;
		(*head)->prev=new_stud;
		*head=new_stud;

	}
	else{
		while(tmp->next!= NULL && strcmp(tmp->next->id,new_stud->id)<0){
			tmp= tmp->next;
		}
		if(tmp->next==NULL){
			tmp->next=new_stud;
			new_stud->prev=tmp;
			new_stud->next=NULL;
			(*tail)=tmp->next;
		}
		else{
			new_stud->next = tmp->next;
			new_stud->prev=tmp;
			tmp->next->prev=new_stud;
			tmp->next=new_stud;
		}
	}
}

void new_student(STUDENT **head ,STUDENT **tail,int lecture_limit){
	STUDENT *new_stud;
	STUDENT *tmp;
	int res,i;
	new_stud = (STUDENT*) malloc(sizeof(STUDENT));
	new_stud->next=NULL;
	new_stud->prev=NULL;

	printf("Enter student's id:");
	scanf("%s",new_stud->id );
	fflush(stdin);
	printf("Enter student's name:");
	scanf("%[^\n]",new_stud->name);
	fflush(stdin);
	printf("Enter student's surname:");
	scanf("%[^\n]",new_stud->surname);
	new_stud->credit=0;
	new_stud->lectureNumber=0;
	
	new_stud->lecture_code=(char**)malloc(lecture_limit *sizeof(char*));	// öðrencinin aldýðý dersleri saklar
	for(i=0;i<lecture_limit;i++){
		new_stud->lecture_code[i]=(char*)malloc(10*sizeof(char));
	}

	if(isStudent_already_exist(*head,new_stud->id)==1){
		printf("\n\n !!! This student is already exist !!!\n");
		return ;
	}
	tmp = *head;
	if(*head==NULL   ){
		new_stud->next=*head;
		*tail=new_stud;
		*head=new_stud;
		(*head)->prev=NULL;
	}
	else if((strcmp(tmp->id,new_stud->id)>0)){
		new_stud->next=*head;
		(*head)->prev=new_stud;
		*head=new_stud;

	}
	else{
		while(tmp->next!= NULL && strcmp(tmp->next->id,new_stud->id)<0){
			tmp= tmp->next;
		}
		if(tmp->next==NULL){
			tmp->next=new_stud;
			new_stud->prev=tmp;
			new_stud->next=NULL;
			(*tail)=tmp->next;
		}
		else{
			new_stud->next = tmp->next;
			new_stud->prev=tmp;
			tmp->next->prev=new_stud;
			tmp->next=new_stud;
		}
	}
	return ;
}

void delete_student(STUDENT **head,STUDENT **tail,LECTURE *lect_head){
	STUDENT *tmp,*tmp2;
	char id[9];
	int i,j;
	LECTURE *tmpL=lect_head;
	printf("Enter the id of student you want to delete");
	scanf("%s",id);
	if((*head)==NULL){
		printf("\nThere is no student on list");
		return ;
	}
	if(strcmp((*head)->id,id)==0){ // silinen ders ilk node'tayse
		tmp=(*head)->next;
		printf("%s deleted\n",(*head)->id);
		free(*head);
		(*head)->next=NULL;
		*head = tmp;
		// head nodundaki öðrencinin kayýt olduðu ders nodu bulunup dersi alan öðrenciler listesinden öðrencinin numarasý silinir
		for(i=0;i< (*head)->lectureNumber ;i++){
					
			while(strcmp((*head)->lecture_code[i],tmpL->code)!=0 && tmpL->next!=NULL){ //  öðrenciler listesinden aranan nod bulunur
				tmpL=tmpL->next;
				
			}
			if(tmpL->next!=NULL){
				j=0;
				while(strcmp(tmpL->enrolled_students[j],id)!=0 && j<tmpL->counter){
					j++;
				}
				if(j!=tmpL->counter){
					strcpy(tmpL->enrolled_students[j],"");
				}
			}
			
		}
		(tmpL->counter)--;	// kontenjan arttýrýldý
	}
	else{
		tmp = *head;
		while(tmp->next!=NULL && strcmp(tmp->next->id,id)!=0){
			tmp = tmp->next;
		}
		if( tmp->next==NULL){
			printf("This student does not exist!\n");
		}
		else{
			tmp2=tmp->next;
		// head nodundaki öðrencinin kayýt olduðu ders nodu bulunup dersi alan öðrenciler listesinden öðrencinin numarasý silinir

			for(i=0;i< (tmp2)->lectureNumber ;i++){
						
				while(strcmp((tmp2)->lecture_code[i],tmpL->code)!=0 && tmpL->next!=NULL){ //  öðrenciler listesinden aranan nod bulunur
					tmpL=tmpL->next;
					
				}
				if(tmpL->next!=NULL){
					j=0;
					while(strcmp(tmpL->enrolled_students[j],id)!=0 && j<tmpL->counter){
						j++;
					}
					if(j!=tmpL->counter){
						strcpy(tmpL->enrolled_students[j],"");
					}
				}
					
			}
			
			(tmpL->counter)--;	// kontenjan arttýrýldý
			tmp->next=tmp->next->next;
			tmp2->next->prev=tmp;
			printf("\n%s student deleted \n",tmp2->id);
			free(tmp2);
			tmp2=NULL;
		}
	}


	return ;
}

void enroll(LECTURE *Lhead,STUDENT *Shead,int lecture_limit,int credit_limit,char*buffer,int mode,int* log_counter,char* date){
	char* garbage;
		//char state[10];
		char* state=(char*)malloc(10*sizeof(char));
		char* code=(char*)malloc(10*sizeof(char));
		char* id=(char*)malloc(10*sizeof(char));
	int i;
	LECTURE *tmpL;
	STUDENT *tmpS;
	if(mode==1){
		printf("\n ENROLL -> student id :");
		scanf("%s",id);
		
		printf("\nENROLL -> lecture code :");
		scanf("%s",code);
	}
	else{
		(*log_counter)++;
		if(buffer=="\0") return ;
		STUDENT* tmpS;
		LECTURE* tmpL;
		garbage=strtok(buffer,",");
		code=strtok(NULL,",");
		id=strtok(NULL,",");
		garbage=strtok(NULL,",");
		state=strtok(NULL,",");
		if(state==NULL) return ;
		if(strcmp(state,"kayitli\n")!=0){
			//free(state);
		//	free(id);
			//free(code);
			return;
			
		}
		//free(state);
		//free(id);
		//free(code);
	}
	tmpL=Lhead;
	tmpS=Shead;
	
	
	while( tmpL->next!=NULL && strcmp(tmpL->code,code)!=0 ){ // dersler listesinden aranan ders nodu bulunur 
		tmpL=tmpL->next;
	}
	if(strcmp(tmpL->code,code)==0){ // dersi  bulduysan diðer koþullara bak
		if(is_enrolled(tmpL,id)){ // bu öðrenc daha önce kaydolmuþ mu?
			printf("this student has already enrolled to this lecture");
			return ;
		}
		
		while(strcmp(tmpS->id,id)!=0 && tmpS->next!=NULL){ //  öðrenciler listesinden aranan nod bulunur
			tmpS=tmpS->next;
		}
		if( strcmp(tmpS->id,id)==0){  // öðrenciyi bulduysan diðer koþullara bak
			if(tmpL->counter+1 <= tmpL->quota){	// dersin kontenjanýnda yer var mý? 
			
				if(tmpS->credit + tmpL->credit <= credit_limit && tmpS->lectureNumber + 1 <= lecture_limit){ // öðrencinin kredi veya ders sayýsý uygun mu?
					strcpy(tmpL->enrolled_students[tmpL->counter],id); //dersler listesindeki ders düðümünde dersi alanlara öðrenci eklendi
					strcpy(tmpS->lecture_code[tmpS->lectureNumber],code); // öðrenciler listesindeki öðrenci düðümünde aldýðý derslere bu ders eklendi
					
				//	sort()
					
					
					
					(tmpS->credit)+=tmpL->credit;	//öðrencinin kredisini güncelle
					tmpS->lectureNumber++;		// öðrencinin aldýðý ders sayýsýný güncelle
					(tmpL->counter)++;			// dersi alan öðrenci sayýsýný güncelle
					
					sort(tmpL->enrolled_students,tmpL->counter);
					sort(tmpS->lecture_code,tmpS->lectureNumber);
					
					printf("\n Ders ekleme basarili");
					printf("\n\n %s 'yi alan ogrenciler:",tmpL->code);
					for(i=0;i<tmpL->counter;i++){
						printf("  %s  ",tmpL->enrolled_students[i]);
					}
					
					printf("\n\n %s ogrencisinin aldigi dersler: ",tmpS->id);
					for(i=0;i<tmpS->lectureNumber;i++){
						printf("  %s  ",tmpS->lecture_code[i]);
					}
				
					if(mode==1){
						(*log_counter)++;
						log_update(code,id,1,log_counter,date);// derskayitogrenci dosyasýna kayýt ekle;

					}
		
				}
	  	else{
					printf("Can not enroll :exceeded the limit of number of lectures or credits for a student");
				}
			}
			else{
				printf("Can not enroll : lecture quota is full !!!");
			}
		}
		else{
			printf("There is no such student");
		}
		
	
		
	}
	else{
		printf("There is no such lecture");
	}
	
}
	
void drop(LECTURE *Lhead,STUDENT *Shead,char *date){
	char id[9];
	int *log_counter; //önemsiz
	char code[20];
	int i;
	LECTURE *tmpL;
	STUDENT *tmpS;
	printf("\nDROP --> student id :");
	scanf("%s",id);
	
	printf("\nDROP --> lecture code :");
	scanf("%s",code);
	//dersin düðümüne öðrenci ekleme
	tmpL=Lhead;
	tmpS=Shead;
	
	while( tmpL->next!=NULL && strcmp(tmpL->code,code)!=0 ){ // dersler listesinden aranan ders nodu bulunur 
		tmpL=tmpL->next;
	}
	if(strcmp(tmpL->code,code)==0){ // dersi  bulduysan diðer koþullara bak
		if(!is_enrolled(tmpL,id)){ // bu öðrenci dersi alýyor mu
			printf("the student is already not on the lecture list");
			return ;
		}
		while(strcmp(tmpS->id,id)!=0 && tmpS->next!=NULL){ // öðrenciler listesinden aranan nod bulunur
			tmpS=tmpS->next;
		}
		if( strcmp(tmpS->id,id)==0){  // öðrenciyi bulduysan diðer koþullara bak
				i=0;
				while(strcmp(tmpL->enrolled_students[i],id)){
					i++;
				}
				strcpy(tmpL->enrolled_students[i],""); //dersler listesindeki ders düðümünde dersi alanlara öðrenci eklendi
			
				i=0;
				while(strcmp(tmpS->lecture_code[i],code)){
					i++;
				}
			
				strcpy(tmpS->lecture_code[i],""); // öðrenciler listesindeki öðrenci düðümünde aldýðý derslere bu ders eklendi
					
				(tmpS->credit)-=tmpL->credit;	//öðrencinin kredisini güncelle
				tmpS->lectureNumber--;		// öðrencinin aldýðý ders sayýsýný güncelle
				(tmpL->counter)--;			// dersi alan öðrenci sayýsýný güncelle
					
				sort(tmpL->enrolled_students,tmpL->counter);
				sort(tmpS->lecture_code,tmpS->lectureNumber);
					
					
				printf("\n %s 'yi alan ogrenciler",tmpL->code);
				for(i=0;i<tmpL->counter;i++){
					printf("\n%s",tmpL->enrolled_students[i]);
				}
				
				printf("\n %s ogrencisinin aldigi dersler",tmpS->id);
				for(i=0;i<tmpS->lectureNumber;i++){
					printf("\n%s",tmpS->lecture_code[i]);
				}
				log_update(code,id,2,log_counter,date);
		}
		else{
			printf("There is no such student");
		}

	}
	else{
		printf("There is no such lecture");
	}
		
}		

int is_enrolled(LECTURE *Lhead,char* id){
	int i=0;
	while( i < Lhead->counter && strcmp(Lhead->enrolled_students[i],id) != 0  ){
			i++;
	}
	if(i == Lhead->counter){
		return 0;	// öðrenci o derse kayýtlý deðil
	}
	else{
		return 1; // öðrenci zaten kayýtlý
	}

}

void sort(char **list,int n){
	int i,j,min;
	char temp[10];
	//strcpy(temp,list[1]);
	for(i=0;i<n-1;i++){
		min=i;
		for(j=i+1;j<n;j++){
			if(strcmp(list[j],list[min])<0){
				min=j;
			}
		}
		strcpy(temp,list[i]);
		strcpy(list[i],list[min]);
		strcpy(list[min],temp);
	
	}
}


void Lecture_Write(LECTURE *head,FILE *fp,char delimiter ){
	
	char tmp[4];
	while(head!=NULL){
		// itoa () function in C language converts int data type to string data type. 
		// 10 means decimal, 2 means binary, 16 means hexadecimal
	
		fputs(head->code,fp);
		fputc(delimiter,fp);
		fputs(head->name,fp);
		fputc(delimiter,fp);
		itoa(head->credit,tmp,10);
		fputs(tmp,fp);
		fputc(delimiter,fp);	
		itoa(head->quota,tmp,10);
		fputs(tmp,fp);
		fputc('\n',fp);	
		head=head->next;	
	}
	fclose(fp);
	return ;
}

void Student_Write(STUDENT *head,FILE *fp,char delimiter ){
	
	char tmp[4];
	while(head!=NULL){
	
		fputs(head->id,fp);
		fputc(delimiter,fp);
		fputs(head->name,fp);
		fputc(delimiter,fp);
		fputs(head->surname,fp);
		fputc(delimiter,fp);
		itoa(head->credit,tmp,10);
		fputs(tmp,fp);
		fputc(delimiter,fp);	
		itoa(head->lectureNumber,tmp,10);
		fputs(tmp,fp);
		fputc('\n',fp);	
		head=head->next;	
	}
	fclose(fp);
	return ;
}


void log_update(char* code,char* id,int mode,int *log_counter,char* date){
	char *tmp=malloc(20*sizeof(char));
	char *temp_date=malloc(20*sizeof(char));
	char *temp_code=malloc(20*sizeof(char));
	char *temp_id=malloc(20*sizeof(char));


	FILE* fp,fpc;
	FILE* fp2;
	char c;
	int n,i;
	
	char buffer[255]=" ",buffercpy[255]=" ";
//yeni kayýt
	if(mode==1){
		fp=fopen("OgrenciDersKayit.txt","a");
		itoa(*log_counter,tmp,10);
		fputs(tmp,fp);
		fputc(',',fp);
		fputs(code,fp);
		fputc(',',fp);
		fputs(id,fp);
		fputc(',',fp);
		fputs(date,fp);
		fputc(',',fp);
		fputs("kayitli",fp);
		fputs("\n",fp);
		fclose(fp);
	}
	// ders býrakma
	else {
	
		fp=fopen("OgrenciDersKayit.txt","r+");
		fp2=fopen("NOgrenciDersKayit.txt","w");
		

		fgets(buffer,255,fp);
		strcpy(buffercpy,buffer);
		tmp=strtok(buffer,",");
		temp_code=strtok(NULL,",");
		temp_id=strtok(NULL,",");
		temp_date=strtok(NULL,",");	
		while(!(strcmp(code,temp_code)==0 && strcmp(id,temp_id )==0)){
		
			
			fputs(buffercpy,fp2);
			fgets(buffer,255,fp);
			strcpy(buffercpy,buffer);
			tmp=strtok(buffer,",");
			temp_code=strtok(NULL,",");
			temp_id=strtok(NULL,",");
			temp_date=strtok(NULL,",");

		}
		
		fputs(tmp,fp2);
		fputc(',',fp2);
		fputs(temp_code,fp2);
		fputc(',',fp2);
		fputs(temp_id,fp2);
		fputc(',',fp2);
		fputs(temp_date,fp2);
		fputc(',',fp2);
		fputs("sildi",fp2);
		fputc('\n',fp2);	
		
		
		while(fgets(buffer,255,fp)!=NULL){
		
			strcpy(buffercpy,buffer);
			fputs(buffercpy,fp2);
		}
		
	fclose(fp);
	fclose(fp2);
	
	remove("OgrenciDersKayit.txt");
	rename("NOgrenciDersKayit.txt", "OgrenciDersKayit.txt");
	
	
	
	}

}
// log dosyasýna ders kapandý  yazan fonksiyon
void logChangeClosed(char* code){
	char *temp_log=malloc(20*sizeof(char));
	char *temp_date=malloc(20*sizeof(char));
	char *temp_code=malloc(20*sizeof(char));
	char *temp_id=malloc(20*sizeof(char));
	char *tmp=malloc(20*sizeof(char));
	FILE* fp;
	FILE* fp2;
	char buffer[255];
	fp=fopen("OgrenciDersKayit.txt","r+");
	fp2=fopen("NOgrenciDersKayit.txt","w");
	while(fgets(buffer,255,fp)!=NULL){
			temp_log=strtok(buffer,",\0");
			temp_code=strtok(NULL,",\0");
			temp_id=strtok(NULL,",\0");
			temp_date=strtok(NULL,",\0");
			tmp=strtok(NULL,"\n\0");
			
			fputs(temp_log,fp2);
			fputc(',',fp2);
			fputs(temp_code,fp2);
			fputc(',',fp2);
			fputs(temp_id,fp2);
			fputc(',',fp2);
			fputs(temp_date,fp2);
			fputc(',',fp2);
			
			
			
			if(!strcmp(temp_code,code) ){
				
				fputs("ders kapandi",fp2);
				fputc('\n',fp2);			
			}
			else{
				
				fputs(tmp,fp2);
				fputc('\n',fp2);
			}
		
	}
	fclose(fp);
	fclose(fp2);
	
	remove("OgrenciDersKayit.txt");
	rename("NOgrenciDersKayit.txt", "OgrenciDersKayit.txt");
	
}



// istenen dersin dosyaýný oluþturur ve içine derse kayýrlý olan öðrencileri yazar
void createLecturesFile(LECTURE *head,STUDENT *Shead){
	int i;
	char fileName[20];
	char code[20];
	char tmp_text[20];
	LECTURE* tmp=head;
	STUDENT* tmpS;
	printf("enter the code of lecture : ");
	scanf("%s",code);
	tmpS=Shead;
// bu code'a sahip bir ders var mý diye bak
	while(strcmp(tmp->code,code) && tmp->next!=NULL){
		tmp=tmp->next;
	}
	if(strcmp(tmp->code,code)==0){
		strcpy(fileName,code);
		strcat(fileName,".txt");
		FILE *fp=fopen(fileName,"w");
		for(i=0; i < tmp->counter; i++ ){
			fputs(tmp->enrolled_students[i],fp);
			fputc(',',fp);
			while(strcmp(tmpS->id,tmp->enrolled_students[i])){
				tmpS=tmpS->next;
			}
			fputs(tmpS->name,fp);
			fputc(',',fp);
			fputs(tmpS->surname,fp);
			fputc('\n',fp);

		}
		
		printf("%s.txt file has been created",code);
		fclose(fp);
	}
	else{
		printf("there is no such lecture");
	}
	
}


//istenen öðrencinin dosyasýný oluþturur ve içine kaydolduðu dersleri yazar


void createStudentFile(STUDENT *head){
	int i;
	char fileName[20];
	char id[20];
	char tmp_text[20];

	STUDENT* tmp=head;
	printf("enter the ID of student : ");
	scanf("%s",id);
	
// bu id'ye sahip bir öðrenci var mý diye bak
	while(strcmp(tmp->id,id) && tmp->next!=NULL){
		tmp=tmp->next;
	}
	if(strcmp(tmp->id,id)==0){
		strcpy(fileName,id);
		strcat(fileName,".txt");
		FILE *fp=fopen(fileName,"w");
		for(i=0;i < tmp->lectureNumber; i++ ){
		
			fputs(tmp->lecture_code[i],fp);
			fputc('\n',fp);

		}
		
		printf("%s.txt file has been created",id);
		fclose(fp);
	}
	else{
		printf("there is no such student");
	}
	
}


