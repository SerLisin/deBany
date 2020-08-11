#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

#include "decode.h"

using namespace std;
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
//ifstream infile("101016.txt");
ofstream outfile("out.txt");
int cont[MAX],cont_pr[MAX]/*массив без служебных байт */,cont_pr_num,cont_num;
short stat_ind;// переменная для индикации работы программы
// массив данных для подсчёт количества и времени владения маркером ([0]-кол-во раз, [1]-время)
gr_mark gr_marker[13]={{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}}; 
int BST_count=0,MKS_count=0,AKS_count=0,OSPR_count=0,Y_count=0,byte_count=0; // счётчики телеграмм и байтов
int BST_max=0,MKS_max=0,AKS_max=0,OSPR_max=0, Y_max=0, bus_load_max=0; // максимальные значения по счётчикам

//функция определения типа телеграммы по первой строке
short telegr_type(char p[][MAX],int count){
	short i,num;
	char* str_p;
	
	//outfile<<endl<<endl;
	//outfile<<p[2]<<endl;
	str_p=&p[2][1];// нужно для того, чтобы убрать нулевой символ строки p[2], который есть CR
/*	if((p[2][2]=='U')&&(p[2][3]=='h')){
	  cout<<p[2]<<endl;	
	  for(i=0;i<34;i++){
	  	 num=str_p[i];
	  	 cout<<num<<" ";
	  };
	  cout<<endl;
      for(i=0;i<34;i++){
	  	 num=str_Uhr_zeit[i];
	  	 cout<<num<<" ";
	  };
	  cout<<endl;
    }*/
	//cout<<str_p<<endl;
	if(!strncmp(str_p,str_BST,25))return 1;
	if((!strncmp(str_p,str_AKS_DI0,55))||(!strncmp(str_p,str_AKS_DI1,49)))return 2;
	if(!strncmp(str_p,str_MKS_DI1,48))return 3;
	if(!strncmp(str_p,str_Y_PR,41))return 4;
	if(!strncmp(str_p,str_Y_AS,35))return 5;
	if(!strncmp(str_p,str_PL,26))return 6;
	if(!strncmp(str_p,str_PL_answ,73))return 7;
	if(!strncmp(str_p,str_PS,20))return 8;
	if(!strncmp(str_p,str_PS_answ,42))return 9;
	if(!strncmp(str_p,str_Uhr_zeit,32))return 10;
	if(!strncmp(str_p,str_UI_test,49))return 11;
	if(!strncmp(str_p,str_UI_check,45))return 12;
	if(!strncmp(str_p,str_Gruppen,13))return 13;
	if(!strncmp(str_p,str_BAC,26))return 14;
	if(!strncmp(str_p,str_BK,17))return 15;
	if(!strncmp(str_p,str_Master_Nah,30))return 16;
	if(!strncmp(str_p,str_Master_Fern,30))return 17;
	if(!strncmp(str_p,str_Master_Frage_Nah,28))return 18;
	if(!strncmp(str_p,str_Master_Frage_Fern,29))return 19;
	if(!strncmp(str_p,str_LUT,27))return 20;
	if(!strncmp(str_p,str_KQ,13))return 21;
	if(!strncmp(str_p,str_Anz,16))return 22;
	
	//закончить позже перечислять все типы телеграмм на шине !!!
	
	
	return 0;
}



// функция для помещения содержимого телеграммы в массив чисел
bool telegr_str_mas(char p[][MAX],int count){
	int num1,num2;
	short i,k,j=0;
	char str[]="  ",str_PR[]=" Y-Funktion Anforderungstelegramm vom PR.",
	str_AS[]=" Y-Funktion Ruecktelegramm der AS ";
	bool fl=false,fl1,fl2; //флаг, указывающий на то, что начался процесс формирования массива
	
	//outfile<<p[1]<<endl;
	// содержимое телеграммы помещаем в массив
	for(k=1;k<=count;k++){ // c 1-ой строки начинается тело телеграммы
	   //проверка, что 2-й и 3-й символы строки - это 16-ое число 
	   num1=p[k][2];
	   if(((num1>=48)&&(num1<=57))or(num1>=65)&&(num1<=70))fl1=true;
	   else fl1=false;
	   num2=p[k][3];;
	   if(((num2>=48)&&(num2<=57))or(num2>=65)&&(num2<=70))fl2=true;
	   else fl2=false;
	   //outfile<<str<<" num1="<<num1<<" num2="<<num2<<endl;
	   if(fl&&(!(fl1&&fl2)))break; //если не число, а запись в массив уже началась, значит, конец телеграммы и выход из цикла
	   //outfile<<"num1="<<num1<<" num2="<<num2<<endl;
	   if(fl1&&fl2){//если второй и третий символы образуют число
	     fl=true; 
	     for(i=0;i<strlen(p[k]);i++){
	     	num1=p[k][i];
	     	if((num1!=13)&&(num1!=32)){//символ не равен пробелу и символу переноса каретки
	            str[0]=p[k][i];
		        str[1]=p[k][i+1];
		        cont[j]=strtol(str,NULL,16);// преобразование строки в число
		        //cout <<"j="<<j<<"  "<<str<< endl;
		        j++;// в j будет количество элементов массива
		        i++;
		  //cout <<str[0]<<"   "<< str[1]<<"   "<< cont[j-1]<< endl;
		    }
	     }
       }
    }
	/* Вывод массива на экран
	for(i=0;i<j;i++){
	   //cout <<"i="<<i<<"  "<<cont[i];
	   if(cont[i]<10)cout <<"    "<<cont[i];
	   if(cont[i]>9)cout <<"   "<<cont[i];
	   if(cont[i]>99)cout <<"  "<<cont[i];
	   //cout <<"  "<<cont[i];
	   if(!((i+1)%20)) cout << endl;
	   //cout << endl;
	//  cout<<"i="<<i<<"  "<<cont[i]<<endl;
    }*/
    k=0;// инициализация массива без служебных байт, в k будет количество элементов массива
    for(i=0;i<j;i++)// данный массив может использоваться не для всех видов телеграмм, например, для коротких - не может
    	if((i%4)){
    		cont_pr[k]=cont[i];
    	//	cout<< cont_pr[k]<< endl;
    		k++;
    	}
    cont_pr_num=k-1;
    cont_num=j;
    //outfile<<"Full amount="<<j-1<<" Amount without="<<k-1<<endl;
    
    // далее вывод обоих массивов в файл
 /*   outfile << endl; 
    for(i=0;i<j;i++){
    	outfile <<hex<<cont[i]<<"  ";
    	if(!((i+1)%20)) outfile << endl;
    }
    outfile << endl;
    for(i=0;i<k;i++){
    	outfile <<hex<<cont_pr[i]<<"  ";
    	if(!((i+1)%15)) outfile << endl;
    }*/
    //Определить тип телеграммы
   // if(telegr_type(p,count)==19)cout<<" Master_Frage "<<endl;
    //if(telegr_type(p,count)==7)cout<<" PL_answ"<<endl;
    //if(telegr_type(p,count)==3)cout<<" Y-AS"<<endl;
    //if(telegr_type(p,count)==4)cout<<" MKS_DI1"<<endl;
    
    //if(!strncmp(p[3],str_PR,41)) telegr_PR_decode (cont_pr,k-1,outfile);//cout <<strlen(str_PR);
    //if(!strncmp(p[3],str_AS,34)) telegr_AS_decode (cont_pr,k-1,outfile);//cout <<strlen(str_AS);
    
	return 0;
	
}
bool telegr_start(char s[]){//функция определения начала телеграммы
    int i,k; //нужно учитывать два типа разделителей: для телеграмм синхронизации времени и остальные
    
   // cout<<strlen(s);
    for(i=4;i<=44;i++){// если первые 44 символов равны "-" - для телеграмм синхронизации времни
    	k=s[i];
    	if(k!=45) return 0;
    }
    for(i=53;i<=56;i++){// если первые 57 символов равны "-"
    	k=s[i];
    	if(k!=45) return 0;
    }
    k=s[57]; // если 57-й символ не равен "-"
    if(k==45) return 0;
    return 1;
}
// функция индикации работы программы
bool status_ind(int n_str){
	//cout<<endl<<stat_ind<<endl;
	if(!(n_str%3000)){
		stat_ind++;
		switch (stat_ind%8) {
			case 0:{
				cout<<"\r|   "<<n_str;
				break;
			}
			case 1:{
				cout<<"\r/   "<<n_str;
				break;
			}
			case 2:{
				cout<<"\r-   "<<n_str;
				break;
			}
			case 3:{
				cout<<"\r\\   "<<n_str;
				break;
			}
			case 4:{
				cout<<"\r|   "<<n_str;
				break;
			}
			case 5:{
				cout<<"\r/   "<<n_str;
				break;
			}
			case 6:{
				cout<<"\r-   "<<n_str;
				break;
			}
			case 7:{
				cout<<"\r\\   "<<n_str;
				stat_ind=-1;
				break;
			}
		}
	}
	return 0;
}
// функция извлечения из строки времени BANY
telegr_time time_extract(char s[]){
	telegr_time time_str;
	char ho[]="  ",mi[]="  ",sec[]="  ",mm[]="   ";
	ho[0]=s[57]; //часы
	ho[1]=s[58];
	mi[0]=s[60]; //минуты
	mi[1]=s[61];
	sec[0]=s[63]; //секунды
	sec[1]=s[64];
	mm[0]=s[71];
	mm[1]=s[72];
	mm[2]=s[73];
	time_str.hour=strtol(ho,NULL,10);
	time_str.minute=strtol(mi,NULL,10);
	time_str.second=strtol(sec,NULL,10);
	time_str.ms=strtol(mm,NULL,10);
	
	return time_str;
}

int main(int argc, char** argv) {
	int i=0,j=0,str_count=0,k=0, sec_num=-1,telegr_t;
	char buffer[MAX],str1[]=" Y-Funktion Anforderungstelegramm vom PR. ", str2[20];
	char telegr[50][MAX], key[10],key2[4],key3[4],key4[4],key5[4];
	bool fl,fl_mr=false,sen_fl,rec_fl,num_fl;
	double bus_load,sec;
	telegr_time time,time_mr;
	telegr_param telegr_fil;
	
	
	if((argc<2)){ //справка или отсутствие аргументов для приложения
	  cout<<"Use like this: "<<endl;
	  cout<<"deBany_v1 <file-name> <type> -k2"<<endl;
	  cout<<"deBany_v1 /? - help"<<endl;
	  exit(0);
	}	
	if(argc==2)
	  if(!strncmp(*(argv+1),"/?",2)){
	  	cout<<"Use like this: deBany_v1 <file-name> <type> -k2"<<endl;
	  	cout<<"deBany_v1 <file-name> <type> -k2"<<endl;
	  	cout<<"deBany_v1 /? - help"<<endl;
	  	cout<<"deBany_v1 <file-name> BST - Status/Bausteintelegramm."<<endl;
	  	cout<<"deBany_v1 <file-name> BST flt <sender> <receiver> - Status/Bausteintelegramm with filter."<<endl;
	  	cout<<"deBany_v1 <file-name> Y-PR - Y-Funktion Anforderungstelegramm vom PR."<<endl;
	  	cout<<"deBany_v1 <file-name> Y-AS - Y-Funktion Ruecktelegramm der AS."<<endl;
	  	cout<<"deBany_v1 <file-name> Y   - Y-PR and Y-AS."<<endl;
	  	cout<<"deBany_v1 <file-name> MKS - MKS-Telegramm DI1."<<endl;
	  	cout<<"deBany_v1 <file-name> MKS flt <sender> <receiver> <number> - MKS-Telegramm DI1 with filter."<<endl;
	  	cout<<"deBany_v1 <file-name> AKS - AKS-Telegramm DI0/DI1."<<endl;
	  	cout<<"deBany_v1 <file-name> AKS flt <sender> <receiver> <number> - AKS-Telegramm DI1 with filter."<<endl;
	  	cout<<"deBany_v1 <file-name> ANA <sender> <telegr_num> <param_num> - Analog parameter selection from AKS"<<endl;
	  	cout<<"deBany_v1 <file-name> UHR - Uhrzeitsynchronisationstelegramm."<<endl;
	  	cout<<"deBany_v1 <file-name> LUT - LUT-Telegramme."<<endl;
	  	cout<<"deBany_v1 <file-name> PL - PL-Telegramme."<<endl;
	  	cout<<"deBany_v1 <file-name> STAT - Statistic."<<endl;
	  	cout<<"deBany_v1 <file-name> STAT -k2 - Statistic <-k2> times per second (-k2=2,..)."<<endl;
	  	cout<<"deBany_v1 <file-name> MARK - Marker Statistic."<<endl;
	  	cout<<"deBany_v1 <file-name> <type> -h - Output only header of not decoded telegramm"<<endl;
	  	cout<<"deBany_v1 <file-name> <type> -b - Output all body of not decoded telegramm"<<endl;
	  	cout<<"deBany_v1 <file-name> <type> -f - Output FC-bytes for decoded telegramm"<<endl;
	  	cout<<"Examples---------------------------------------------------------------------"<<endl;
	  	cout<<"deBany_v1 <file-name> MKS flt 10 * 110 - MKS-Telegramm, sender-No 10, No 110"<<endl;	  	
	  	cout<<"deBany_v1 <file-name> AKS flt * * 110 - AKS-Telegramm, No-110"<<endl;	  	
	  	cout<<"deBany_v1 <file-name> BST flt 216 * - Status/Bausteintelegramm, sender - No 216 "<<endl;
	  	cout<<"deBany_v1 <file-name> ANA 15 60 10 - Parameter No 10 from AWE015 AKS60 "<<endl;	  	
	  	
	    exit(0);
	  }
	//cout<<"argc="<<argc<<endl;  
	if(argc>2){
	   strcpy(key,*(argv+2));//обработка ключей запуска программы
	   cout<<"Key="<< key;
    } 
    if(argc>3)strcpy(key2,*(argv+3));//обработка ключей запуска программы
	if(argc>4)strcpy(key3,*(argv+4));//обработка ключей запуска программы
	if(argc>5)strcpy(key4,*(argv+5));//обработка ключей запуска программы
	if(argc>6)strcpy(key5,*(argv+6));//обработка ключей запуска программы
	
    ifstream infile(*(argv+1));//обрабатываемый файл указывается первым аргументом
    stat_ind=0;
	for(;infile.getline(buffer,MAX);){
		i++;   
		str_count++;
		//cout << i <<buffer << "\n";
		//cout << str_count<<endl;
		if(telegr_start(buffer)){ // Начало телеграммы
		   telegr_str_mas(telegr,str_count-1);// расшифровка телеграммы, запись в файл
		   // вывод расшифровки телеграмм в файл в зависимости от ключей вызова программы
		   status_ind(i);
		   fl=true;
		   telegr_t=telegr_type(telegr,str_count-1);
		   if((!strncmp(key,"BST",4))&&(telegr_t==1)){
		   	 if((key2[1]=='f')||(key2[2]=='f')){ //вывести расшифровку байтов FC
	            outfile<<"VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV"<<endl;
	            outfile<<"FC of the first frame        ";FC_decode(cont[0],outfile); outfile<<endl;
	            outfile<<"FC of the intermediate frame ";FC_decode(cont[4],outfile);outfile<<endl;
	            outfile<<"FC of the last frame         ";FC_decode(cont[cont_num-4],outfile);outfile<<endl;
	         }
	         fl=false;
	         if(!strncmp(key2,"flt",4)){// фильтрация по номеру, отправителю и получателю телеграммы MKS
	         	//outfile<<telegr[3]<<endl;
	         	//outfile<<telegr[4]<<endl;
	         	telegr_fil=telegr_BST_param(cont_pr,cont_pr_num,outfile);// извлечение параметров телеграммы
	         	sen_fl=false;
	         	rec_fl=false;
	         	num_fl=false;
	         	if((telegr_fil.sen==(strtol(key3,NULL,10)))||(key3[0]=='*'))sen_fl=true; // проверка отправителя
	         	for(j=0;j<telegr_fil.rec_num;j++)if(telegr_fil.rec[j]==(strtol(key4,NULL,10)))rec_fl=true;// проверка получателей
	         	if(key4[0]=='*')rec_fl=true;// если все получатели
	         	if(sen_fl&&rec_fl){// проверка, что выполняются все условия
	         	  outfile<<telegr[1]<<endl;
		   	      outfile<<endl;
	              outfile<<telegr[2]<<endl;
	              telegr_BST_decode(cont_pr,cont_pr_num,outfile);
	             }
	         }
	         else {// если без фильтра
		   	 	outfile<<telegr[1]<<endl;
		   	    outfile<<endl;
	            outfile<<telegr[2]<<endl;
		   	 	telegr_BST_decode(cont_pr,cont_pr_num,outfile);
		   	 }
		   }
		   if((!strncmp(key,"AKS",4))&&(telegr_t==2)){
		   	 if((key2[1]=='f')||(key2[2]=='f')){ 
	           outfile<<"VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV"<<endl;
	           outfile<<"FC of the first frame        ";FC_decode(cont[0],outfile); outfile<<endl;
	           outfile<<"FC of the intermediate frame ";FC_decode(cont[4],outfile);outfile<<endl;
	           outfile<<"FC of the last frame         ";FC_decode(cont[cont_num-4],outfile);outfile<<endl;
	         }
	         fl=false;
	         if(!strncmp(key2,"flt",4)){// фильтрация по номеру, отправителю и получателю телеграммы MKS
	         	//outfile<<telegr[3]<<endl;
	         	//outfile<<telegr[4]<<endl;
	         	telegr_fil=telegr_AKS_param(cont_pr,cont_pr_num,outfile);// извлечение параметров телеграммы
	         	sen_fl=false;
	         	rec_fl=false;
	         	num_fl=false;
	         	if((telegr_fil.sen==(strtol(key3,NULL,10)))||(key3[0]=='*'))sen_fl=true; // проверка отправителя
	         	for(j=0;j<telegr_fil.rec_num;j++)if(telegr_fil.rec[j]==(strtol(key4,NULL,10)))rec_fl=true;// проверка получателей
	         	if(key4[0]=='*')rec_fl=true;// если все получатели
	         	if((telegr_fil.num==(strtol(key5,NULL,10)))||(key5[0]=='*'))num_fl=true; // проверка номера телеграммы
	         	if(sen_fl&&rec_fl&&num_fl){// проверка, что выполняются все условия
	         	  outfile<<telegr[1]<<endl;
		   	      outfile<<endl;
	              outfile<<telegr[2]<<endl;
	              telegr_AKS_decode(cont_pr,cont_pr_num,outfile);
	             }
	         }
	         else {// если без фильтра
		   	 	outfile<<telegr[1]<<endl;
		   	    outfile<<endl;
	            outfile<<telegr[2]<<endl;
		   	 	telegr_AKS_decode(cont_pr,cont_pr_num,outfile);
		   	 }
		   }
		   if((!strncmp(key,"MKS",4))&&(telegr_t==3)){
		   	 if((key2[1]=='f')||(key2[2]=='f')){ 
		   	 outfile<<"VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV"<<endl;
	         outfile<<"FC of the first frame        ";FC_decode(cont[0],outfile); outfile<<endl;
	         outfile<<"FC of the intermediate frame ";FC_decode(cont[4],outfile);outfile<<endl;
	         outfile<<"FC of the last frame         ";FC_decode(cont[cont_num-4],outfile);outfile<<endl;
	         }
	         fl=false;
	         if(!strncmp(key2,"flt",4)){// фильтрация по номеру, отправителю и получателю телеграммы MKS
	         	//outfile<<telegr[3]<<endl;
	         	//outfile<<telegr[4]<<endl;
	         	telegr_fil=telegr_MKS_param(cont_pr,cont_pr_num,outfile);// извлечение параметров телеграммы
	         	sen_fl=false;
	         	rec_fl=false;
	         	num_fl=false;
	         	if((telegr_fil.sen==(strtol(key3,NULL,10)))||(key3[0]=='*'))sen_fl=true; // проверка отправителя
	         	for(j=0;j<telegr_fil.rec_num;j++)if(telegr_fil.rec[j]==(strtol(key4,NULL,10)))rec_fl=true;// проверка получателей
	         	if(key4[0]=='*')rec_fl=true;// если все получатели
	         	if((telegr_fil.num==(strtol(key5,NULL,10)))||(key5[0]=='*'))num_fl=true; // проверка номера телеграммы
	         	if(sen_fl&&rec_fl&&num_fl){// проверка, что выполняются все условия
	         	  outfile<<telegr[1]<<endl;
		   	      outfile<<endl;
	              outfile<<telegr[2]<<endl;
	              telegr_MKS_decode(cont_pr,cont_pr_num,outfile);
	         	}
	         }
		   	 else {// если без фильтра
		   	 	outfile<<telegr[1]<<endl;
		   	    outfile<<endl;
	            outfile<<telegr[2]<<endl;
		   	 	telegr_MKS_decode(cont_pr,cont_pr_num,outfile);
		   	 }
		   }
		   if((!strncmp(key,"Y_PR",4))&&(telegr_t==4)){
		   	 outfile<<telegr[1]<<endl;
		   	 outfile<<endl;
	         outfile<<telegr[2]<<endl;
	         if((key2[1]=='f')||(key2[2]=='f')){ 
	         outfile<<"FC of the first frame        ";FC_decode(cont[0],outfile); outfile<<endl;
	         outfile<<"FC of the intermediate frame ";FC_decode(cont[4],outfile);outfile<<endl;
	         outfile<<"FC of the last frame         ";FC_decode(cont[cont_num-4],outfile);outfile<<endl;
	         }
	         fl=false;
		   	 telegr_PR_decode(cont_pr,cont_pr_num,outfile);
		   }
		   if((!strncmp(key,"Y_AS",4))&&(telegr_t==5)){
		   	 outfile<<telegr[1]<<endl;
		   	 outfile<<endl;
	         outfile<<telegr[2]<<endl;
	         if((key2[1]=='f')||(key2[2]=='f')){ 
	         outfile<<"FC of the first frame        ";FC_decode(cont[0],outfile); outfile<<endl;
	         outfile<<"FC of the intermediate frame ";FC_decode(cont[4],outfile);outfile<<endl;
	         outfile<<"FC of the last frame         ";FC_decode(cont[cont_num-4],outfile);outfile<<endl;
	         }
	         fl=false;
		   	 telegr_AS_decode(cont_pr,cont_pr_num,outfile);
		   }
		   if((!strncmp(key,"Y",4))&&((telegr_t==4)||(telegr_t==5))){
		   	 outfile<<telegr[1]<<endl;
		   	 outfile<<endl;
	         outfile<<telegr[2]<<endl;
	         if((key2[1]=='f')||(key2[2]=='f')){ 
	         outfile<<"FC of the first frame        ";FC_decode(cont[0],outfile); outfile<<endl;
	         outfile<<"FC of the intermediate frame ";FC_decode(cont[4],outfile);outfile<<endl;
	         outfile<<"FC of the last frame         ";FC_decode(cont[cont_num-4],outfile);outfile<<endl;
	         }
	         fl=false;
	         if(telegr_t==4)telegr_PR_decode(cont_pr,cont_pr_num,outfile);
	         if(telegr_t==5)telegr_AS_decode(cont_pr,cont_pr_num,outfile);
		   }
		   if((!strncmp(key,"UHR",3))&&(telegr_t==10)){
		   	 outfile<<telegr[1]<<endl;
		   	 outfile<<endl;
	         outfile<<telegr[2]<<endl;
	         if((key2[1]=='f')||(key2[2]=='f')){ 
	         outfile<<"FC of the first frame        ";FC_decode(cont[0],outfile); outfile<<endl;
	         outfile<<"FC of the intermediate frame ";FC_decode(cont[4],outfile);outfile<<endl;
	         outfile<<"FC of the last frame         ";FC_decode(cont[cont_num-4],outfile);outfile<<endl;
	         }
	         fl=false;
		   	 telegr_Uhr_decode(cont_pr,cont_pr_num,outfile);
		   }
		   if((!strncmp(key,"LUT",3))&&(telegr_t==20)){
		   	 outfile<<telegr[1]<<endl;
		   	 outfile<<endl;
	         outfile<<telegr[2]<<endl;
	         if((key2[1]=='f')||(key2[2]=='f')){ 
	         outfile<<"FC of the first frame        ";FC_decode(cont[0],outfile); outfile<<endl;
	         outfile<<"FC of the intermediate frame ";FC_decode(cont[4],outfile);outfile<<endl;
	         outfile<<"FC of the last frame         ";FC_decode(cont[cont_num-4],outfile);outfile<<endl;
	         }
	         fl=false;
		   	 telegr_LUT_decode(cont_pr,cont_pr_num,outfile);
		   }
		   if((!strncmp(key,"PL",2))&&(telegr_t==6)){
		   	 outfile<<telegr[1]<<endl;
		   	 outfile<<endl;
	         outfile<<telegr[2]<<endl;
	         if((key2[1]=='f')||(key2[2]=='f')){ 
	         outfile<<"FC of the first frame        ";FC_decode(cont[0],outfile); outfile<<endl;
	         outfile<<"FC of the intermediate frame ";FC_decode(cont[4],outfile);outfile<<endl;
	         outfile<<"FC of the last frame         ";FC_decode(cont[cont_num-4],outfile);outfile<<endl;
	         }
	         fl=false;
		   	 telegr_PL_decode(cont_pr,cont_pr_num,outfile);
		   }
		   if((!strncmp(key,"ANA",3))&&(telegr_t==2)){//выборка определённого параметра из определённой телеграммы AKS
		   	 telegr_fil=telegr_AKS_param(cont_pr,cont_pr_num,outfile);// извлечение параметров телеграммы
		   	 if(telegr_fil.sen==(strtol(key2,NULL,10)))
		   	   if(telegr_fil.num==(strtol(key3,NULL,10))){
		   	   	  telegr_AKS_param(strtol(key4,NULL,10),cont_pr,cont_pr_num,outfile);
		   	   	  //outfile<<dec<<telegr_fil.sen<<"  "<<telegr_fil.num<<endl;
		   	   }
		   	   
	         
		   	 //telegr_PL_decode(cont_pr,cont_pr_num,outfile);
		   }
		   if(!strncmp(key,"STAT",4)){ //вывод статистики
		     if(telegr_t==1)BST_count++; // подсчёт BST в секунду
			 if(telegr_t==3)MKS_count++; // подсчёт MKS в секунду
		     if(telegr_t==2)AKS_count++;//  подсчёт AKS в секунду
		     if((telegr_t==6)||(telegr_t==7))OSPR_count++;//  подсчёт OSPR в секунду
		     if((telegr_t==4)||(telegr_t==5))Y_count++;//  подсчёт телеграмм запроса Y-адресов и ответа на них в секунду
		     byte_count=byte_count+cont_num;                   // подсчёт количества байт в секунду
		     time=time_extract(telegr[1]); // получение времени из разделителя телеграмм (строки)
		     if((telegr_t==21)&&(cont_pr[2]!=0))outfile<<" Transmission prolem (KQ)!!! "<<time.second<<","<<time.ms<<endl;// Если ненулевое значение в телеграмме короткого квитирования
		     //if(telegr_t==22)outfile<<" Anzeigenauftrag!!! "<<time.second<<","<<time.ms<<endl;// Телеграмма-уведомления на шине
		     if(sec_num!=(-1)){
		     	if((sec_num!=time.second)&&(argc==3)){ // условие, определяющее с какой частотой выводить статистику
		     	    bus_load=100*9*(byte_count/8)/(31250); //вычисление нагрузки на шину в % от 250кбит/сек с учётом того, что кадры состоят из 4*9 бит
		     		//масштабируем вывод - чтобы не было сдвига
		     		if(time.hour<10)outfile<<"0"<<time.hour;
		     		else outfile<<time.hour;
		     		if(time.minute<10)outfile<<":0"<<time.minute;
		     		else outfile<<":"<<time.minute;
		     		if(time.second<10)outfile<<":0"<<time.second;
		     		else outfile<<":"<<time.second;
		     		if(MKS_count<10)outfile<<" MKS: "<<MKS_count;  else outfile<<" MKS:"<<MKS_count;
		     		if(AKS_count<10)outfile<<" AKS: "<<AKS_count;  else outfile<<" AKS:"<<AKS_count;
		     		if(BST_count<10)outfile<<" BST: "<<BST_count;  else outfile<<" BST:"<<BST_count;
		     		if(OSPR_count<10)outfile<<" PL: "<<OSPR_count; else outfile<<" PL:"<<OSPR_count;
		     		if(Y_count<10)outfile<<" Y: "<<Y_count; else outfile<<" Y:"<<Y_count;
		     	    outfile<<" Bus_load: "<<bus_load<</*"%"<<*/endl;
		     	    //вычисление максимальных значений
		     	    if(MKS_count>MKS_max)MKS_max=MKS_count;
		     	    if(AKS_count>AKS_max)AKS_max=AKS_count;
		     	    if(BST_count>BST_max)BST_max=BST_count;
		     	    if(OSPR_count>OSPR_max)OSPR_max=OSPR_count;
		     	    if(Y_count>Y_max)Y_max=Y_count;
		     	    if(bus_load>bus_load_max)bus_load_max=bus_load;
		     		MKS_count=0;
		     		AKS_count=0;
		     		BST_count=0;
		     		OSPR_count=0;
		     		Y_count=0;
		     		byte_count=0;
		     		sec_num=time.second;		     		
				 }
				 if(argc>3)k=strtol(key2,NULL,10);else k=1;
				 if(((sec_num!=time.second)||(time.ms>=(j+1)*1000/k))&&(argc==4)){ // условие, определяющее с какой частотой выводить статистику
		     		//outfile<<k<<endl;
		     		bus_load=100*9*(byte_count/8)/(31250/k); //вычисление нагрузки на шину в % от 250кбит/сек с учётом того, что кадры состоят из 4*9 бит
		     		//масштабируем вывод - чтобы не было сдвига
		     		if(time.hour<10)outfile<<"0"<<time.hour;
		     		else outfile<<time.hour;
		     		if(time.minute<10)outfile<<":0"<<time.minute;
		     		else outfile<<":"<<time.minute;
		     		if(time.second<10)outfile<<":0"<<time.second;
		     		else outfile<<":"<<time.second;
		     		if(time.ms<10)outfile<<",00"<<time.ms;
		     		   else if(time.ms<100)outfile<<",0"<<time.ms;
		     		        else outfile<<","<<time.ms;
		     		if(MKS_count<10)outfile<<" MKS: "<<MKS_count;  else outfile<<" MKS:"<<MKS_count;
		     		if(AKS_count<10)outfile<<" AKS: "<<AKS_count;  else outfile<<" AKS:"<<AKS_count;
		     		if(BST_count<10)outfile<<" BST: "<<BST_count;  else outfile<<" BST:"<<BST_count;
		     		if(OSPR_count<10)outfile<<" PL: "<<OSPR_count; else outfile<<" PL:"<<OSPR_count;
		     		if(Y_count<10)outfile<<" Y: "<<Y_count; else outfile<<" Y:"<<Y_count;
		     	    outfile<<" Bus_load: "<<bus_load<</*"%"<<*/endl;
		     	    //вычисление максимальных значений
		     	    if(MKS_count>MKS_max)MKS_max=MKS_count;
		     	    if(AKS_count>AKS_max)AKS_max=AKS_count;
		     	    if(BST_count>BST_max)BST_max=BST_count;
		     	    if(OSPR_count>OSPR_max)OSPR_max=OSPR_count;
		     	    if(Y_count>Y_max)Y_max=Y_count;
		     	    if(bus_load>bus_load_max)bus_load_max=bus_load;
		     		MKS_count=0;
		     		AKS_count=0;
		     		BST_count=0;
		     		OSPR_count=0;
		     		Y_count=0;
		     		byte_count=0;
		     		j++;
		     		if(sec_num!=time.second)j=0;
		     		sec_num=time.second;
		     	 }
		     }
		     else sec_num=time.second;
		     //outfile<<time.hour<<":"<<time.minute<<":"<<time.second<<"."<<time.ms<<endl;
		   }
		   if(!strncmp(key,"MARK",4)){// подсчёт количества раз и времени владения маркером каждой группой
		   	if(telegr_t==17){
               str2[0]=telegr[2][45];
               str2[1]=telegr[2][46];
               str2[2]=0;
               k=strtol(str2,NULL,10);
               gr_marker[k-1].num++;
               time=time_extract(telegr[1]); // получение времени из разделителя телеграмм (строки)
               if(!fl_mr)time_mr=time;
               else{//подсчёт количества секунд
               	    if(time_mr.hour>time.hour)sec=time.hour*3600+time.minute*60+time.second+0.001*time.ms+(86400-(time_mr.hour*3600+time_mr.minute*60+time_mr.second+0.001*time_mr.ms));
               	    else sec=time.hour*3600+time.minute*60+time.second+0.001*time.ms-(time_mr.hour*3600+time_mr.minute*60+time_mr.second+0.001*time_mr.ms);
               	    gr_marker[k-1].sec=gr_marker[k-1].sec+sec;
               	    time_mr=time;
               }
               fl_mr=true;
               
		   	}
	       }
		   if(fl){//если обработанная телеграмма не должна быть декодирована
		     if((key2[1]=='h')||(key2[2]=='h')){ //вывести только заголовок нераскодированной телеграммы
		       	outfile<<telegr[1]<<endl;
		   	    outfile<<endl;
	            outfile<<telegr[2]<<endl;
		     }
		     if((key2[1]=='b')||(key2[2]=='b')){ //вывести всю нераскодированную телеграмму
		     	for(k=1;k<str_count;k++)outfile<<telegr[k]<<endl;
		     }
		   }
		   str_count=1; // счётчик строк телеграммы
		   //exit(0); // временно, для проверки на одной телеграмме
		}
		//if(i>100)exit(0);
		strcpy(telegr[str_count],buffer);
		//if(!strncmp(buffer,str2,70)) j++;// сравнение первых символов строк
	}
	// окончание цикла считывания данных из файла
	if(!strncmp(key,"STAT",4)){ //вывод максимальных значений счётчиков для статистики
	   outfile<<"--------------------------------------------------------"<<endl;
	   outfile<<dec<<"max MKS:"<<MKS_max<<" max AKS:"<<AKS_max<<" max BST:"<<BST_max<<" max PL:"<<OSPR_max
	   <<" max Y:"<<Y_max<<" max busload:"<<bus_load_max<<"%"<<endl;
    }
    if(!strncmp(key,"MARK",4)){// вывод количества раз и времени владения маркером каждой группой
      outfile<<"Количество раз и время владения маркером по группам."<<endl;
      for(j=0;j<13;j++){
	      outfile<<"Группа №"<<j+1<<":"<<gr_marker[j].num<<"  "<<gr_marker[j].sec<<" sec"<<endl;
      }
	}
	cout << endl<<"Line amount:"<<dec<< i << "\n";
	outfile << "Количество строк:"<<dec<<i<< "\n";
    //cout<<argv<<endl;
    //cout<<argv+1<<endl;
    
}
