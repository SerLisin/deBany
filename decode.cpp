#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <iomanip> 
#include "decode.h"

using namespace std;


// ������� ����������� ����������, ���������� Y-������, �� AS
bool telegr_AS_decode (int cont[],int count,ofstream &fileOut){ 
    char c;
    short i,j;
    
    fileOut<<"Sender:"<<dec<<cont[5]<<" "<<cont[4]<<endl;
    fileOut<<"Receiver:"<<dec<<cont[2]<<" "<<cont[1]<<endl;
    fileOut<<"��� ������� ��������: 0x"<<hex<<cont[3]<<endl;
    fileOut<<"LG="<<dec<<cont[7]<<endl;
    fileOut<<"OPC="<<hex<<"0x"<<cont[8]<<endl;
    fileOut<<"PBLNR="<<hex<<"0x"<<cont[10]<<cont[9]<<endl;
    fileOut<<"PBLNA="<<hex<<"0x"<<cont[12]<<cont[11]<<endl;
    fileOut<<"X="<<hex<<"0x"<<cont[14]<<cont[13]<<endl;
    fileOut<<"Code="<<dec<<cont[16]<<cont[15]<<endl;
	fileOut<<"LA="<<dec<<cont[17]<<endl;
    i=1;
    //outfile<<count<<endl;
    //����� Y-�������
      while(i<=cont[17]){
    	j=19+(i-1)*4;
    	if(cont[j+1]<16)fileOut<<hex<<"0"<<cont[j+1];
    	else fileOut<<hex<<cont[j+1];
    	if(cont[j]<16)fileOut<<hex<<"0"<<cont[j];
    	else fileOut<<hex<<cont[j];
    	if(cont[j+3]<16)fileOut<<hex<<":0"<<cont[j+3];
    	else fileOut<<hex<<":"<<cont[j+3];
    	if(cont[j+2]<16)fileOut<<hex<<"0"<<cont[j+2]<<endl;
    	else fileOut<<hex<<cont[j+2]<<endl;
    	i++;
    }
    fileOut<<hex<<"ABLNR=0x"<<cont[count-5]<<cont[count-6]<<"  ";
    fileOut<<hex<<"ABLNA=0x"<<cont[count-3]<<cont[count-4]<<endl;
    fileOut<<hex<<"B=0x"<<cont[count-1]<<cont[count-2]<<endl;
    fileOut<<hex<<"P=0x"<<cont[count]<<endl;
}
// ������� ����������� ���������� ������� Y-������� �� PU
bool telegr_PR_decode (int cont[],int count,ofstream &fileOut){ //count - ���������� ����� � �������
    char c;
    short i;
    
    fileOut<<"Sender:"<<dec<<cont[5]<<" "<<cont[4]<<endl;
    fileOut<<"Receiver:"<<dec<<cont[2]<<" "<<cont[1]<<endl;
    fileOut<<"��� ������� ��������: 0x"<<hex<<cont[3]<<endl;
    fileOut<<"LG="<<dec<<cont[7]<<endl;
    fileOut<<"OPC="<<hex<<"0x"<<cont[8]<<endl;
    fileOut<<"PBLNR="<<hex<<"0x"<<cont[10]<<cont[9]<<endl;
    fileOut<<"PBLNA="<<hex<<"0x"<<cont[12]<<cont[11]<<endl;
    fileOut<<"Y="<<hex<<"0x"<<cont[14]<<cont[13]<<endl;
    
    //������� ��� � ����� ����� 
    c=cont[16]; fileOut<<c;c=cont[15]; fileOut<<c;c=cont[18]; fileOut<<c;c=cont[17]; fileOut<<c<<",";
    c=cont[20]; fileOut<<c;c=cont[19]; fileOut<<c;c=cont[22]; fileOut<<c;c=cont[21]; fileOut<<c<<endl;
    i=23;
    while(i<=count){
    	switch (cont[i+1])
    	{
    		case 0:
    			{
    				fileOut<<dec<<"AA,"<<cont[i]<<endl;
    				i=i+2;
    				break;
    			}
    		case 1:
    			{
    				fileOut<<dec<<"EA,"<<cont[i]<<endl;
    				i=i+2;
    				break;
    			}
    		case 2:
    			{
    				fileOut<<dec<<"AB,"<<cont[i]<<endl;
    				i=i+2;
    				break;
    			}
    		case 3:
    			{
    				fileOut<<dec<<"EB,"<<cont[i]<<endl;
    				i=i+2;
    				break;
    			}
    			
			case 7:
    			{
    				fileOut<<dec<<"VB,"<<cont[i]<<endl;
    				i=i+2;
    				break;
    			}
    		default:
    			{
    				fileOut<<"Unknown type"<<endl;
    				i=i+2;
    			}
      	}       
    	if(cont[i]==254)break; // ���� ����� ��� ����� ����� = "FE"
    }
    fileOut<<hex<<"ABLNR=0x"<<cont[count-5]<<cont[count-6]<<"  ";
    fileOut<<hex<<"ABLNA=0x"<<cont[count-3]<<cont[count-4]<<endl;
    fileOut<<hex<<"B=0x"<<cont[count-1]<<cont[count-2]<<endl;
    fileOut<<hex<<"P=0x"<<cont[count]<<endl;
	return 0;
}
// ����������� ������� Zeit A
char* zeitA(int zeit1,int zeit2){//zeit2 - ������� �����
	int num;
	char str[10],str1[5];
	char* p;
	char* p1;
	
	num=zeit2;
	num=(zeit2&0x7)<<2;
			
	num=num|((zeit1&0xC0)>>6);//����
	str[0]=0;
	p=&str[0];
	str1[0]=0;
	itoa(num,str1,10);
	strcat(p,str1);
	strcat(p,":");
		
	num=zeit1&0x3F;//������
	itoa(num,str1,10);
	strcat(p,str1);
		
	return p;
		
}
// ����������� ������� Zeit B
char* zeitB(int zeit1,int zeit2){//zeit2 - ������� �����
    char* p;
    char str[20],str1[5];
    int num;
    
    num=zeit2;
    num=(zeit2&0xFC)>>2;//�������
    str[0]=0;
    p=&str[0];
    itoa(num,str1,10);
    strcat(p,str1);
    
    num=zeit1;
    num=num>>1;
    num=num&0xFF;
    num=num*2; //������������
    strcat(p,",");
    itoa(num,str1,10);
    strcat(p,str1);
    
    num=zeit1&0x1;
    if(num)strcat(p,", 1-������.");
    else strcat(p,", 0-����.");
    
    
    //fileOut<<str; 
	
	return p;
}
// ����������� ������� Zeit C
char* zeitC(int zeit1,int zeit2){//zeit2 - ������� �����
	int num;
	char str[10],str1[5];
	char* p;
	char* p1;
	
	num=zeit2;
	num=(zeit2&0xF)<<2;
			
	num=num|((zeit1&0xC0)>>6);//������
	
	str[0]=0;
	p=&str[0];
	str1[0]=0;
	itoa(num,str1,10);
	strcat(p,str1);
	strcat(p,":");
		
	num=zeit1&0x3F;//�������
	itoa(num,str1,10);
	strcat(p,str1);
		
	return p;
		
}
//������� ��� ������ � ������ ����� � �������� �����
char* int_str2(int data){
	short i,j,num,num1;
	char* p;
	char str[9],str1[9];
	
	num=data;
	str1[0]=0;
	str[0]=0;
	p=&str1[0];
	if(num&1)strcat(p,"1");
	else strcat(p,"0");
	
	for(i=1;i<8;i++){
		num1=1;
		for(j=0;j<i;j++)num1=num1*2;
		if((num&num1)>>i)strcat(p,"1");
		else strcat(p,"0");
		
	}
	
	for(i=1;i<=8;i++)str[i-1]=str1[8-i];//����������� ������
	p=&str[0]	;
	
	return p;
}
// ��������� ����� � ���������� AKS
double AKS_num(int b1,int b2,int b3, int b4,ofstream &fileOut){//������� ������, ��� � ����������
	int num,i;
	short mant,code,expon;
	double mant_r,expon_r;
	double res;
	bool m_sign=false,e_sign=false;//false- �������������
	
	//fileOut<<hex<<b1<<" "<<b2<<" "<<b3<<" "<<b4<<endl;
	expon=b1;
	if(expon&0x80){//���� ����� �������������, �������� ������������� ����� � ������ ����
		expon=~b1;
		expon=expon+1;   // �������� ����� � ������ ����
		expon=expon&0xF; //�������� ������ ������� � ���� short
		e_sign=true;
	} 
	
	
	code=b2;// ������������!!!
	mant=b3;
	num=b4<<8;
	mant=mant|num;
	if(mant&0x8000){//���� ����� �������������, �������� ������������� ����� � ������ ����
		mant=~mant;
		mant=mant+1;   // �������� ����� � ������ ����
		m_sign=true;
	} 
	
	//�������� �������� � ���������������� ���� - ����� ����� ����� ����
	
	/*for(i=0;i<7;i++){
		if(mant<pow(10,i+1)){
			mant_r=mant/pow(10,i+1);
			break;
		}
	}*/
	
	mant_r=1.0*mant;
	mant_r=mant_r/32768;// ���������� �������� � �������������� ���� � ����� ���������, ������ ����
		
	if(m_sign)mant_r=-mant_r;
	if(e_sign)expon_r=-1.0*expon;
	else expon_r=1.0*expon;
	res=mant_r*pow(2,expon_r);
		
	//fileOut<<dec<<res<<endl;
	//fileOut<<dec<<expon<<"  "<<expon_r<<endl;
	//fileOut<<dec<<mant<<"  "<<mant_r<<endl;
	
	return res;
}
// ������� ����������� ���������� ����� FC
bool FC_decode(int FC,ofstream &fileOut){
	fileOut<<"FT|MT|ET|BD|A1|A0|ZY|AE ";
	
	if(FC&fc_FT)fileOut<<"1";
	else fileOut<<"0";
	if(FC&fc_MT)fileOut<<"1";
	else fileOut<<"0";
	if(FC&fc_ET)fileOut<<"1";
	else fileOut<<"0";
	if(FC&fc_BD)fileOut<<"1";
	else fileOut<<"0";
	if(FC&fc_A1)fileOut<<"1";
	else fileOut<<"0";
	if(FC&fc_A0)fileOut<<"1";
	else fileOut<<"0";
	if(FC&fc_ZY)fileOut<<"1";
	else fileOut<<"0";
	if(FC&fc_AE)fileOut<<"1";
	else fileOut<<"0";
	
	return 0;
}
// ������� ��� ���������� �������� ���������� (�����, �����������, ����������) ���������� MKS
telegr_param telegr_MKS_param(int cont[],int count,ofstream &fileOut){
	int i,j,m;
	telegr_param telegr_tmp;
	
	
    i=0;
    j=0;//���������� �����������
    m=0;//���������� ������ � F0=F1
   
    while(i<12){ //6+6 - �������� ������,������������ � F0 � F1
       if(cont[3*i]==0xF1){ //���� ������ ���� ����� F1, ��������������, ��� ��� ������� � ���������
            if(cont[3*i+2]==0xFC);
            m++;
        }
    	else if(cont[3*i]==0xF0){
    		j++;
    		telegr_tmp.rec[j-1]=cont[3*i+2]*100+cont[3*i+1];
    	}
    	else if(cont[3*i]==0xEF)break;// ����� ����� �������
    	     else{ // ���� � ������������
    	        telegr_tmp.sen=cont[3*i+2]*100+cont[3*i+1];
			 }
    	i++;
    }	
    telegr_tmp.rec_num=j;
    telegr_tmp.num=cont[3*i+2*j+21]; // ����������� ������ ����������
            
    return telegr_tmp;
    
}
// ������� ����������� ���������� MKS
bool telegr_MKS_decode (int cont[],int count,ofstream &fileOut){ //count - ���������� ����� � �������
    int rec_num;//���������� ����������� ����������
    int i,j,k,cur_pos,m,i_T;
    long num1,num2;
    char str_Sender[50],str_Receiver[70];
    bool fl=true;// ���� ���������, ��� ��������� � ����� �������
    char* p_sen;
    char* p_rec;
    char str[20];
    char* p;
        
    // �������� FC_decode ��� ��� ��������� FC: ������, ������ � ��������� ����� (�������� �� ������� ������������)
    // ���������� ����������
    // ����������� �������� �����
    
      
    i=0;
    j=0;//���������� �����������
    m=0;//���������� ������ � F0=F1
    str_Sender[0]=0;str_Receiver[0]=0;//������� �����
    p_sen=&str_Sender[0]; // ��������� � ������ �����, ������ ��� ��������� ����� � ������������ ����� ����������
    p_rec=&str_Receiver[0];
    strcat(p_rec,"Receivers:");
    strcat(p_sen,"Sender:");
    while(i<12){ //6+6 - �������� ������,������������ � F0 � F1
        //fileOut<<hex<<cont[3*i]<<endl;      
        if(cont[3*i]==0xF1){ //���� ������ ���� ����� F1, ��������������, ��� ��� ������� � ���������
            if(cont[3*i+2]==0xFC){
            	itoa(cont[3*i+1],str,10);
    		    strcat(p_rec,"  (BK ");
    		    strcat(p_rec,str);
    		    strcat(p_rec,")->");
          }
        	m++;
        }
    	else if(cont[3*i]==0xF0){
    		j++;
    		/*if(j==1)fileOut<<"Receivers:"<<dec<<cont[3*i+2]<<" "<<cont[3*i+1];
    		else fileOut<<", "<<dec<<cont[3*i+2]<<" "<<cont[3*i+1];*/
    		if(j==1){ 
    			itoa(cont[3*i+2],str,10);
    		    strcat(p_rec,str);
    		    strcat(p_rec," ");
    		    itoa(cont[3*i+1],str,10);
    		    strcat(p_rec,str);
    		}
    		else{
    			strcat(p_rec,",  ");
    			itoa(cont[3*i+2],str,10);
    		    strcat(p_rec,str);
    		    strcat(p_rec," ");
    		    itoa(cont[3*i+1],str,10);
    		    strcat(p_rec,str);
    		}
      	}
    	else if(cont[3*i]==0xEF)break;// ����� ����� �������
    	     else{ // ���� � ������������
    	        i_T=3*i;// ���������� ����� ����� � ��������� ������� ��������
    	    	itoa(cont[3*i+2],str,10);
    		    strcat(p_sen,str);
    		    strcat(p_sen," ");
    		    itoa(cont[3*i+1],str,10);
    		    strcat(p_sen,str);
          	}
    	i++;
    }
    strcat(p_rec,"\n");
    strcat(p_rec,"��� ������� ��������: ");
    
    		    
    fileOut<<str_Sender<<endl;
    fileOut<<str_Receiver<<hex<<"0x"<<cont[i_T]<<endl;
    
    fileOut<<endl;
    fileOut<<"LG="<<dec<<cont[3*i+1]<<endl;
    fileOut<<"OPC="<<hex<<cont[3*i+2]<<endl;
    fileOut<<"PBLNR="<<hex<<cont[3*i+4]<<cont[3*i+3]<<endl;
    fileOut<<"PBLNA="<<hex<<cont[3*i+6]<<cont[3*i+5]<<endl;
    for(k=0;k<j;k++){
    	
    	num1=cont[(3*i+7)+2*k];
    	num2=cont[(3*i+7)+2*k+1];
    	if(((num2&0xC0)>>6)==3)fileOut<<"Block type=MKE"<<"  ";
    	
    	num2=num2&0xF;
    	num2=num2<<8;
    	num2=num2|num1;
    	fileOut<<"MKE-block nr:"<<dec<<num2<<" (���� ����� ������������ ������ �� �������� �����):"<<num1<<endl;
    }
    cur_pos=3*i+7+2*k;
    p=zeitA(cont[cur_pos],cont[cur_pos+1]);
    for(i=0;i<10;i++)str[i]=*(p+i);
    fileOut<<str;//����� ������� zeitA
    p=zeitB(cont[cur_pos+2],cont[cur_pos+3]);
    for(i=0;i<15;i++)str[i]=*(p+i);
    fileOut<<":"<<str<<endl;//����� ������� zeitB
    
    fileOut<<"Ddata1: ";
    p=int_str2(cont[cur_pos+5]);
    str[8]=0;
    for(i=0;i<8;i++)str[i]=*(p+i);
    fileOut<<str<<" ";//����� ������� Ddata1
    p=int_str2(cont[cur_pos+4]);
    str[8]=0;
    for(i=0;i<8;i++)str[i]=*(p+i);
    fileOut<<str<<endl;//����� ������� Ddata1
    
    fileOut<<"Ddata2: ";
    p=int_str2(cont[cur_pos+7]);
    str[8]=0;
    for(i=0;i<8;i++)str[i]=*(p+i);
    fileOut<<str<<" ";//����� ������� Ddata2
    p=int_str2(cont[cur_pos+6]);
    str[8]=0;
    for(i=0;i<8;i++)str[i]=*(p+i);
    fileOut<<str<<endl;//����� ������� Ddata2
    
    fileOut<<"Qdata1: ";
    p=int_str2(cont[cur_pos+9]);
    str[8]=0;
    for(i=0;i<8;i++)str[i]=*(p+i);
    fileOut<<str<<" ";//����� ������� Qdata1
    p=int_str2(cont[cur_pos+8]);
    str[8]=0;
    for(i=0;i<8;i++)str[i]=*(p+i);
    fileOut<<str<<endl;//����� ������� Qdata1
    
    fileOut<<"Qdata2: ";
    p=int_str2(cont[cur_pos+11]);
    str[8]=0;
    for(i=0;i<8;i++)str[i]=*(p+i);
    fileOut<<str<<" ";//����� ������� Qdata2
    p=int_str2(cont[cur_pos+10]);
    str[8]=0;
    for(i=0;i<8;i++)str[i]=*(p+i);
    fileOut<<str<<endl;//����� ������� Qdata2
    fileOut<<"ABLNR:"<<hex<<"0x"<<cont[cur_pos+13]<<cont[cur_pos+12]<<endl;
    fileOut<<"ABLNA:"<<hex<<"0x"<<cont[cur_pos+15]<<cont[cur_pos+14];
    num1=cont[cur_pos+14];
    num2=cont[cur_pos+15];
    num2=num2&0xF;
    num2=num2<<8;
    num1=num1|num2;
    fileOut<<"   ����� ���������� MKS:"<<dec<<num1<<endl<<"(���� ����� ������������ ������ �� �������� �����):"<<cont[cur_pos+14]<<endl;
    fileOut<<"B:"<<hex<<"0x"<<cont[cur_pos+17]<<cont[cur_pos+16]<<endl;
    fileOut<<"Pv:"<<hex<<"0x"<<cont[cur_pos+18]<<endl;
    
	        
    return 0;
}
// ������� ��� ���������� �������� ���������� (�����, �����������, ����������) ���������� AKS
telegr_param telegr_AKS_param(int cont[],int count,ofstream &fileOut){
	int i,j,m;
	telegr_param telegr_tmp;
	
	
    i=0;
    j=0;//���������� �����������
    m=0;//���������� ������ � F0=F1
   
    while(i<12){ //6+6 - �������� ������,������������ � F0 � F1
       if(cont[3*i]==0xF1){ //���� ������ ���� ����� F1, ��������������, ��� ��� ������� � ���������
            if(cont[3*i+2]==0xFC);
            m++;
        }
    	else if(cont[3*i]==0xF0){
    		j++;
    		telegr_tmp.rec[j-1]=cont[3*i+2]*100+cont[3*i+1];
    	}
    	else if(cont[3*i]==0xEF)break;// ����� ����� �������
    	     else{ // ���� � ������������
    	        telegr_tmp.sen=cont[3*i+2]*100+cont[3*i+1];
			 }
    	i++;
    }	
    telegr_tmp.rec_num=j;
    m=(int)((cont[3*i+1]-2*j-17)/4)+1;//���������� ���������� ���������� �������� � ���������� �� ��������� LG
    
    telegr_tmp.num=cont[3*i+2*j+4*m+11]; // ����������� ������ ����������
    //fileOut<<3*i+2*j+4*m+11<<" tel_num:"<<cont[3*i+2*j+4*m+11]<<endl;
            
    return telegr_tmp;
    
}
// ������� ����������� ���������� AKS
bool telegr_AKS_decode (int cont[],int count,ofstream &fileOut){ //count - ���������� ����� � �������
    int rec_num;//���������� ����������� ����������
    int i,j,k,cur_pos,m,i_T;
    long num1,num2;
    char str_Sender[50],str_Receiver[80],c;
    bool fl=true;// ���� ���������, ��� ��������� � ����� �������
    char* p_sen;
    char* p_rec;
    char str[15];
    char* p;
        
    
	
    // �������� FC_decode ��� ��� ��������� FC: ������, ������ � ��������� ����� (�������� �� ������� ������������)
    // ���������� ����������
    // ����������� �������� �����
    
      
    i=0;
    j=0;//���������� �����������
    m=0;//���������� ������ � F0=F1
    str_Sender[0]=0;str_Receiver[0]=0;//������� �����
    p_sen=&str_Sender[0]; // ��������� � ������ �����, ������ ��� ��������� ����� � ������������ ����� ����������
    p_rec=&str_Receiver[0];
    strcat(p_rec,"Receivers:");
    strcat(p_sen,"Sender:");
    while(i<12){ //6+6 - �������� ������,������������ � F0 � F1
        if(cont[3*i]==0xF1){ //���� ������ ���� ����� F1, ��������������, ��� ��� ������� � ���������
          if(cont[3*i+2]==0xFC){
            	itoa(cont[3*i+1],str,10);
    		    strcat(p_rec,"  (BK ");
    		    strcat(p_rec,str);
    		    strcat(p_rec,")->");
          }
        	m++;
        }
    	else if(cont[3*i]==0xF0){
    		j++;
    		/*if(j==1)fileOut<<"Receivers:"<<dec<<cont[3*i+2]<<" "<<cont[3*i+1];
    		else fileOut<<", "<<dec<<cont[3*i+2]<<" "<<cont[3*i+1];*/
    		if(j==1){ 
    			itoa(cont[3*i+2],str,10);
    		    strcat(p_rec,str);
    		    strcat(p_rec," ");
    		    itoa(cont[3*i+1],str,10);
    		    strcat(p_rec,str);
    		}
    		else{
    			strcat(p_rec,",  ");
    			itoa(cont[3*i+2],str,10);
    		    strcat(p_rec,str);
    		    strcat(p_rec," ");
    		    itoa(cont[3*i+1],str,10);
    		    strcat(p_rec,str);
    		}
      	}
    	else if(cont[3*i]==0xEF)break;// ����� ����� �������
    	     else{ // ���� � ������������
    	        i_T=3*i;// ���������� ����� ����� � ��������� ������� ��������
    	    	itoa(cont[3*i+2],str,10);
    		    strcat(p_sen,str);
    		    strcat(p_sen," ");
    		    itoa(cont[3*i+1],str,10);
    		    strcat(p_sen,str);
          	}
    	i++;
    }
    strcat(p_rec,"\n");
    strcat(p_rec,"��� ������� ��������: ");
    
    		    
    fileOut<<str_Sender<<endl;
    fileOut<<str_Receiver<<hex<<"0x"<<cont[i_T]<<endl;
    
    fileOut<<endl;
    fileOut<<"LG="<<dec<<cont[3*i+1]<<endl;
    fileOut<<"OPC="<<hex<<cont[3*i+2]<<endl;
    fileOut<<"PBLNR="<<hex<<cont[3*i+4]<<cont[3*i+3]<<endl;
    fileOut<<"PBLNA="<<hex<<cont[3*i+6]<<cont[3*i+5]<<" ('";
    c=cont[3*i+6]; //����� ������������ ����� ���������� � ���������� ����
	if(cont[3*i+6]>=0x20)fileOut<<c;
	c=cont[3*i+5];
	if(cont[3*i+5]>=0x20)fileOut<<c<<"')"<<endl;
	else fileOut<<"')"<<endl;
    
    for(k=0;k<j;k++){
    	
    	num1=cont[(3*i+7)+2*k];
    	num2=cont[(3*i+7)+2*k+1];
    	if(((num2&0xC0)>>6)==1)fileOut<<"Block type=AKE";
    	
    	num2=num2&0xF;
    	num2=num2<<8;
    	num2=num2|num1;
    	fileOut<<", AKE-block nr:"<<dec<<num2<<" (���� ������ �� �������� �����):"<<num1<<endl;
    }
    cur_pos=3*i+7+2*k;
    p=zeitC(cont[cur_pos],cont[cur_pos+1]);
    for(m=0;m<10;m++)str[m]=*(p+m);
    fileOut<<"ZeitC(min:sec): ";
    fileOut<<str<<endl;//����� ������� zeitC
    
    m=(int)((cont[3*i+1]-2*j-17)/4)+1;//���������� ���������� ���������� �������� � ���������� �� ��������� LG
    fileOut<<m<<endl;
    for(k=0;k<m;k++){
    	fileOut<<dec<<setw(2)<<k+1<<") "<<setw(20)<<setprecision(10)<<AKS_num(cont[cur_pos+2+4*k],cont[cur_pos+2+4*k+1],cont[cur_pos+2+4*k+2],cont[cur_pos+2+4*k+3],fileOut)<<",";
    	fileOut<<"  Quality code: 0x"<<hex<<cont[cur_pos+2+4*k+1];
    	if(cont[cur_pos+2+4*k+1]!=0xC0)fileOut<<" <<<<<<<"<<endl;
    	else fileOut<<endl;
    }
    cur_pos=cur_pos+4*m+2;
	
	//fileOut<<hex<<cont[cur_pos]<<"-"<<cont[cur_pos+1]<<"-"<<cont[cur_pos+2]<<"-"<<cont[cur_pos+3]<<"-"<<cont[cur_pos+4]<<"-"<<cont[cur_pos+5]<<"-"<<cont[cur_pos+6]<<endl;
    fileOut<<"ABLNR:"<<hex<<"0x"<<cont[cur_pos+1]<<cont[cur_pos]<<endl;
    fileOut<<"ABLNA:"<<hex<<"0x"<<cont[cur_pos+3]<<cont[cur_pos+2];
    num1=cont[cur_pos+2];
    num2=cont[cur_pos+3];
    num2=num2&0xF;
    num2=num2<<8;
    num1=num1|num2;
    fileOut<<"   ����� ���������� AKS:"<<dec<<num1<<endl<<"(���� ����� ������������ ������ �� �������� �����):"<<cont[cur_pos+2]<<endl;
    fileOut<<"B:"<<hex<<"0x"<<cont[cur_pos+5]<<cont[cur_pos+4]<<endl;
    fileOut<<"Pv:"<<hex<<"0x"<<cont[cur_pos+6]<<endl;
    
	        
    return 0;
}
//���������� ������� ������������ ��������� �� ���������� AKS
int telegr_AKS_param(int n, int cont[],int count, ofstream &fileOut){
    int rec_num;//���������� ����������� ����������
    int i,j,k,cur_pos,m,i_T;
    char* p_sen;
    char* p_rec;
    char str[15];
    char* p;
 
      
    i=0;
    j=0;//���������� �����������
    m=0;//���������� ������ � F0=F1
    while(i<12){ //6+6 - �������� ������,������������ � F0 � F1
        if(cont[3*i]==0xF1){ //���� ������ ���� ����� F1, ��������������, ��� ��� ������� � ���������
          m++;
        }
    	else if(cont[3*i]==0xF0){
    		j++;
      	}
    	else if(cont[3*i]==0xEF)break;// ����� ����� �������
    	i++;
    }
    cur_pos=3*i+7+2*j;
    p=zeitC(cont[cur_pos],cont[cur_pos+1]);
    for(m=0;m<10;m++)str[m]=*(p+m);
    fileOut<<"ZeitC(min:sec): ";
    fileOut<<setw(6)<<str;//����� ������� zeitC
       
    fileOut<<dec<<setw(20)<<setprecision(10)<<AKS_num(cont[cur_pos-2+4*n],cont[cur_pos+4*n-1],cont[cur_pos+4*n],cont[cur_pos+4*n+1],fileOut);
    fileOut<<"  Quality code: 0x"<<hex<<cont[cur_pos+4*n-1]<<endl;
  	        
    return 0;

}
// ������� ����������� ������ IB ���������� BST
char* BST_IB(int b1, int b2){//b2 - ������� ���� - ������� ����������, ��� � ����������
    char* p;
	char str[10],str1[4];
	int num1,num2;
	
	str[0]=0;
	p=&str[0];
	num2=b2; // ��� ��������� ��� ���������� ���������
	switch (num2){
		case bst_SRZ:{
			strcat(p,"SRZ,");
			break;
		}
		case bst_KRZ:{
			strcat(p,"KRZ,");
			break;
	    }
	    case bst_RB:{
			strcat(p,"RB,");
			break;
	    }
	    case bst_ASR:{
			strcat(p,"ASR,");
			break;
	    }
	    case bst_BSR:{
			strcat(p,"BSR,");
			break;
		}
		case bst_FGS:{
			strcat(p,"FGS,");
			break;
	    }
	    case bst_AET:{
			strcat(p,"AET,");
			break;
	    }
	    case bst_ITE:{
			strcat(p,"ITE,");
			break;
	    }
	    case bst_MU:{
			strcat(p,"MU,");
			break;
	    }
	    case bst_GWM:{
			strcat(p,"GWM,");
			break;
	    }
	    case bst_SR:{
			strcat(p,"SR,");
			break;
	    }
	    case bst_IBR:{
			strcat(p,"IBR,");
			break;
	    }
		case bst_IVL:{
			strcat(p,"IVL,");
			break;
		}
		case bst_KR:{
			strcat(p,"KR,");
			break;
	    }
	    case bst_ES:{
			strcat(p,"ES,");
			break;
	    }
	    case bst_EM:{
			strcat(p,"KRZ,");
			break;
	    }
	    case bst_AE:{
			strcat(p,"AE,");
			break;
	    }
	    case bst_KO:{
			strcat(p,"KO,");
			break;
	    }
	    case bst_GS:{
			strcat(p,"GS,");
			break;
	    }
	    case bst_ZIE:{
			strcat(p,"ZIE,");
			break;
	    }
	    case bst_AETE:{
			strcat(p,"AETE,");
			break;
	    }
	    case bst_FSM:{
			strcat(p,"FSM,");
			break;
	    }
	    case bst_FAW:{
			strcat(p,"FAW,");
			break;
	    }
	    case bst_SE:{
			strcat(p,"Sys.er,");
			break;
	    }
	    default:{
	    	itoa(num2,str1,16);
	    	strcat(p,"0x");
	    	strcat(p,str1);
	    	strcat(p,",");
	    }
	}
	itoa(b1,str1,10);
	strcat(p,str1);
	//fileOut<<hex<<num2<<endl;
	
	
	return p;
		
}

// ������� �������������� ����� �� 12 ��� � ������ � ������ � �������� ����
char* BST_int_str2_12(int data){
    char* p;
	char str[13],str1[13];
	int num,num1;
	short i,j;
	
	num=data;
	str1[0]=0;
	str[0]=0;
	p=&str1[0];
	if(num&1)strcat(p,"1");
	else strcat(p,"0");
	
	for(i=1;i<12;i++){
		num1=1;
		for(j=0;j<i;j++)num1=num1*2;
		if((num&num1)>>i)strcat(p,"1");
		else strcat(p,"0");
	}
	for(i=1;i<=12;i++)str[i-1]=str1[12-i];//����������� ������
    p=&str[0];
	
	return p;
}
// ������� ��� ���������� �������� ���������� (�����, �����������, ����������) ���������� BST
telegr_param telegr_BST_param(int cont[],int count,ofstream &fileOut){
	int i,j,m;
	telegr_param telegr_tmp;
	
	
    i=0;
    j=0;//���������� �����������
    m=0;//���������� ������ � F0=F1
   
    while(i<12){ //6+6 - �������� ������,������������ � F0 � F1
       if(cont[3*i]==0xF1){ //���� ������ ���� ����� F1, ��������������, ��� ��� ������� � ���������
            if(cont[3*i+2]==0xFC);
            m++;
        }
    	else if(cont[3*i]==0xF0){
    		j++;
    		telegr_tmp.rec[j-1]=cont[3*i+2]*100+cont[3*i+1];
    	}
    	else if(cont[3*i]==0xEF)break;// ����� ����� �������
    	     else{ // ���� � ������������
    	        telegr_tmp.sen=cont[3*i+2]*100+cont[3*i+1];
			 }
    	i++;
    }	
    telegr_tmp.rec_num=j;
           
    telegr_tmp.num=0; // ��� ������ ��� BST ����������
    //fileOut<<3*i+2*j+4*m+11<<" tel_num:"<<cont[3*i+2*j+4*m+11]<<endl;
            
    return telegr_tmp;
    
}
// ������� ����������� ���������� BST
bool telegr_BST_decode (int cont[],int count,ofstream &fileOut){ //count - ���������� ����� � �������
    int rec_num;//���������� ����������� ����������
    int i,j,k,cur_pos,m,i_T;
    long num1,num2;
    char str_Sender[50],str_Receiver[50];
    bool fl=true;// ���� ���������, ��� ��������� � ����� �������
    char* p_sen;
    char* p_rec;
    char str[20];
    char* p;
        
    // �������� FC_decode ��� ��� ��������� FC: ������, ������ � ��������� ����� (�������� �� ������� ������������)
    // ���������� ����������
    // ����������� �������� �����
    
      
    i=0;
    j=0;//���������� �����������
    m=0;//���������� ������ � F0=F1
    str_Sender[0]=0;str_Receiver[0]=0;//������� �����
    p_sen=&str_Sender[0]; // ��������� � ������ �����, ������ ��� ��������� ����� � ������������ ����� ����������
    p_rec=&str_Receiver[0];
    strcat(p_rec,"Receivers:");
    strcat(p_sen,"Sender:");
    while(i<12){ //6+6 - �������� ������,������������ � F0 � F1
        //fileOut<<hex<<cont[3*i]<<endl;      
        if(cont[3*i]==0xF1){ //���� ������ ���� ����� F1, ��������������, ��� ��� ������� � ���������
            if(cont[3*i+2]==0xFC){
            	itoa(cont[3*i+1],str,10);
    		    strcat(p_rec,"  (BK ");
    		    strcat(p_rec,str);
    		    strcat(p_rec,")->");
          }
        	m++;
        }
    	else if(cont[3*i]==0xF0){
    		j++;
    		/*if(j==1)fileOut<<"Receivers:"<<dec<<cont[3*i+2]<<" "<<cont[3*i+1];
    		else fileOut<<", "<<dec<<cont[3*i+2]<<" "<<cont[3*i+1];*/
    		if(j==1){ 
    			itoa(cont[3*i+2],str,10);
    		    strcat(p_rec,str);
    		    strcat(p_rec," ");
    		    itoa(cont[3*i+1],str,10);
    		    strcat(p_rec,str);
    		}
    		else{
    			strcat(p_rec,",  ");
    			itoa(cont[3*i+2],str,10);
    		    strcat(p_rec,str);
    		    strcat(p_rec," ");
    		    itoa(cont[3*i+1],str,10);
    		    strcat(p_rec,str);
    		}
      	}
    	else if(cont[3*i]==0xEF)break;// ����� ����� �������
    	     else{ // ���� � ������������
    	        i_T=3*i;// ���������� ����� ����� � ��������� ������� ��������
    	    	itoa(cont[3*i+2],str,10);
    		    strcat(p_sen,str);
    		    strcat(p_sen," ");
    		    itoa(cont[3*i+1],str,10);
    		    strcat(p_sen,str);
          	}
    	i++;
    }
    strcat(p_rec,"\n");
    strcat(p_rec,"��� ������� ��������: ");
    
    		    
    fileOut<<str_Sender<<endl;
    fileOut<<str_Receiver<<hex<<"0x"<<cont[i_T]<<endl;
    
    fileOut<<endl;
    fileOut<<"LG="<<dec<<cont[3*i+1]<<endl;
    fileOut<<"OPC="<<hex<<cont[3*i+2]<<endl;
    fileOut<<"PBLNR="<<hex<<cont[3*i+4]<<cont[3*i+3]<<endl;
    fileOut<<"PBLNA="<<hex<<cont[3*i+6]<<cont[3*i+5]<<endl;
    cur_pos=3*i+7;
    p=zeitA(cont[cur_pos],cont[cur_pos+1]);
    for(i=0;i<10;i++)str[i]=*(p+i);
    fileOut<<"ZeitA(h:min):"<<str<<endl;//����� ������� zeitA
    fileOut<<dec<<"K(���������� ����� ������ � ����������):"<<cont[cur_pos+2]<<endl;//����� ������� zeitA
    m=cont[cur_pos+2];//���������� ����� ������ � ����������
    cur_pos=cur_pos+4;
    for(i=0;i<m;i++){ //����������� � ����� ����� ������
        fileOut<<dec<<i+1<<") ";
    	p=BST_IB(cont[cur_pos+8*i],cont[cur_pos+8*i+1]);
    	for(k=0;k<10;k++)str[k]=*(p+k);
		fileOut<<str<<endl;
		p=zeitB(cont[cur_pos+8*i+2],cont[cur_pos+8*i+3]);
        for(k=0;k<20;k++)str[k]=*(p+k);
        fileOut<<"ZeitB(sec,ms):"<<str<<endl;//����� ������� zeitB
        num1=cont[cur_pos+8*i+4];
        num2=cont[cur_pos+8*i+5];
        num1=num1|((num2&0x0F)<<8);
        fileOut<<"��� ������:";
        switch((num2&0xF0)>>4){
        	case 0:{
        		fileOut<<"F(0x00) - failure"<<endl;
        		break;
        	}
        	case 1:{
        		fileOut<<"B(0x01) - signals of status/position"<<endl;
        		break;
        	}
        	case 2:{
        		fileOut<<"Z(0x02) - signals of equipment"<<endl;
        		break;
        	}
        	case 3:{
        		fileOut<<"P(0x03) - engineering error"<<endl;
        		break;
        	}
        	case 4:{
        		fileOut<<"S(0x04) - system error"<<endl;
        		break;
        	}
        }
        p=BST_int_str2_12(num1);
        for(k=0;k<20;k++)str[k]=*(p+k);
        fileOut<<"�������� ���������:"<<str<<endl;//����� ������� zeitB
        
        num1=cont[cur_pos+8*i+6];
        num2=cont[cur_pos+8*i+7];
        num1=num1|((num2&0x0F)<<8);
        fileOut<<"����� ������:";
        num2=(num2&0xF0)>>4;
        fileOut<<dec<<num2<<endl;
        p=BST_int_str2_12(num1);
        for(k=0;k<20;k++)str[k]=*(p+k);
        fileOut<<"����� ���������:   "<<str<<endl;
    	//fileOut<<endl;
    }
    cur_pos=cur_pos+8*m;
    fileOut<<"ABLNR:"<<hex<<"0x"<<cont[cur_pos+1]<<cont[cur_pos]<<endl;
    fileOut<<"ABLNA:"<<hex<<"0x"<<cont[cur_pos+3]<<cont[cur_pos+2]<<endl;
    fileOut<<"B:"<<hex<<"0x"<<cont[cur_pos+5]<<cont[cur_pos+4]<<endl;
    fileOut<<"Pv:"<<hex<<"0x"<<cont[cur_pos+6]<<endl;
        
    return 0;
}
// ������� ����������� ���������� ������������� �������
bool telegr_Uhr_decode (int cont[],int count,ofstream &fileOut){ //count - ���������� ����� � �������
   short num1,num2,num3;
   fileOut<<"Receivers:"<<dec<<cont[2]<<" "<<cont[1]<<endl;
   fileOut<<"Sender:"<<cont[5]<<" "<<cont[4]<<endl;
   fileOut<<"��� ������� ��������: 0x"<<hex<<cont[3]<<endl;
   fileOut<<"LG="<<dec<<cont[7]<<endl;
   fileOut<<"OPC=0x"<<hex<<cont[8]<<endl;
   fileOut<<"PBLNR=0x"<<hex<<cont[10]<<cont[9]<<endl;
   fileOut<<"PBLNA=0x"<<hex<<cont[12]<<cont[11]<<endl;
   num1=cont[14]<<8;//����������� ����, ������ � ���
   num1=num1|cont[13];
   num2=cont[16]<<8;
   num2=num2|cont[15];
   num3=num2&0xF800;
   num3=(num3>>11)&0x1F;
   fileOut<<dec<<num3;//����
   num3=num1&0xF;
   fileOut<<dec<<"."<<num3;//�����
   num3=num1&0x7F0;
   num3=num3>>4;
   fileOut<<dec<<"."<<num3;//���
   num3=num2&0x7C0;
   num3=(num3>>6)&0x1F;
   fileOut<<dec<<"  "<<num3;//���
   num3=num2&0x3F;
   fileOut<<dec<<":"<<num3;//������
   num1=cont[19]<<8; //������-�� ���� ���� (18) ������������
   num1=num1|cont[17];
   num3=(num1&0xFC00)>>10;
   num3=num3&0x3F;
   fileOut<<dec<<":"<<num3;//�������
   num3=num1&0x3FF;
   fileOut<<dec<<","<<num3<<"ms"<<endl;//������������
   fileOut<<hex<<"Pv=0x"<<cont[20]<<endl;
      	
   return 0;
}
// ������� ����������� ���������� LUT
bool telegr_LUT_decode(int cont[],int count,ofstream &fileOut){
	int rec_num;//���������� ����������� ����������
    int i,j,k,cur_pos,m,i_T;
    long num1,num2;
    char str_Sender[50],str_Receiver[50];
    bool fl=true;// ���� ���������, ��� ��������� � ����� �������
    char* p_sen;
    char* p_rec;
    char str[20];
    char* p;
        
    // �������� FC_decode ��� ��� ��������� FC: ������, ������ � ��������� ����� (�������� �� ������� ������������)
    // ���������� ����������
    // ����������� �������� �����
    
      
    i=0;
    j=0;//���������� �����������
    m=0;//���������� ������ � F0=F1
    str_Sender[0]=0;str_Receiver[0]=0;//������� �����
    p_sen=&str_Sender[0]; // ��������� � ������ �����, ������ ��� ��������� ����� � ������������ ����� ����������
    p_rec=&str_Receiver[0];
    strcat(p_rec,"Receivers:");
    strcat(p_sen,"Sender:");
    while(i<16){ //8+8 - �������� ������,������������ � F0 � F1
        //fileOut<<hex<<cont[3*i]<<endl;      
        if(cont[3*i]==0xF1){ //���� ������ ���� ����� F1, ��������������, ��� ��� ������� � ���������
            if(cont[3*i+2]==0xFC){
            	itoa(cont[3*i+1],str,10);
    		    strcat(p_rec,"  (BK ");
    		    strcat(p_rec,str);
    		    strcat(p_rec,")->");
          }
        	m++;
        }
    	else if(cont[3*i]==0xF0){
    		j++;
    		/*if(j==1)fileOut<<"Receivers:"<<dec<<cont[3*i+2]<<" "<<cont[3*i+1];
    		else fileOut<<", "<<dec<<cont[3*i+2]<<" "<<cont[3*i+1];*/
    		if(j==1){ 
    			itoa(cont[3*i+2],str,10);
    		    strcat(p_rec,str);
    		    strcat(p_rec," ");
    		    itoa(cont[3*i+1],str,10);
    		    strcat(p_rec,str);
    		}
    		else{
    			strcat(p_rec,",  ");
    			itoa(cont[3*i+2],str,10);
    		    strcat(p_rec,str);
    		    strcat(p_rec," ");
    		    itoa(cont[3*i+1],str,10);
    		    strcat(p_rec,str);
    		}
      	}
    	else if(cont[3*i]==0xEF)break;// ����� ����� �������
    	     else{ // ���� � ������������
    	        i_T=3*i;// ���������� ����� ����� � ��������� ������� ��������
    	    	itoa(cont[3*i+2],str,10);
    		    strcat(p_sen,str);
    		    strcat(p_sen," ");
    		    itoa(cont[3*i+1],str,10);
    		    strcat(p_sen,str);
          	}
    	i++;
    }
    strcat(p_rec,"\n");
    strcat(p_rec,"��� ������� ��������: ");
    
    		    
    fileOut<<str_Sender<<endl;
    fileOut<<str_Receiver<<hex<<"0x"<<cont[i_T]<<endl;
    
    fileOut<<endl;
    fileOut<<"LG="<<dec<<cont[3*i+1]<<endl;
    fileOut<<"OPC="<<hex<<cont[3*i+2]<<endl;
    fileOut<<"PBLNR="<<hex<<cont[3*i+4]<<cont[3*i+3]<<endl;
    fileOut<<"PBLNA="<<hex<<cont[3*i+6]<<cont[3*i+5]<<endl;
    cur_pos=3*i+7;
    p=zeitA(cont[cur_pos],cont[cur_pos+1]);
    for(i=0;i<10;i++)str[i]=*(p+i);
    fileOut<<"Zeit(h:min:sec,ms):  "<<str;//����� ������� zeitA
    p=zeitB(cont[cur_pos+2],cont[cur_pos+3]);
    for(i=0;i<20;i++)str[i]=*(p+i);
    fileOut<<":"<<str<<endl;//����� ������� zeitB
    fileOut<<"CS:0x"<<hex<<cont[cur_pos+5]<<cont[cur_pos+4]<<endl;
    fileOut<<"ABLNR:0x"<<hex<<cont[cur_pos+7]<<cont[cur_pos+6]<<endl;
    fileOut<<"ABLNA:0x"<<hex<<cont[cur_pos+9]<<cont[cur_pos+8]<<endl;
    fileOut<<"Pv:0x"<<hex<<cont[cur_pos+10]<<endl;
    
    
	//fileOut<<"YES!!!"<<endl;
	return 0;
}
// ������� ����������� PL-����������
bool telegr_PL_decode (int cont[],int count,ofstream &fileOut){ 
    char c;
    int i,j,num;
    
    //���� ����� BUS-COUPLER!!! ������.
    fileOut<<"Sender:"<<dec<<cont[5]<<" "<<cont[4]<<endl;
    fileOut<<"Receiver:"<<dec<<cont[2]<<" "<<cont[1]<<endl;
    fileOut<<"��� ������� ��������: 0x"<<hex<<cont[3]<<endl;
    fileOut<<"LG="<<dec<<cont[7]<<endl;
    fileOut<<"OPC="<<hex<<"0x"<<cont[8]<<endl;
    fileOut<<"KINFO="<<hex<<"0x"<<cont[10]<<cont[9]<<endl;
    fileOut<<"KINFO="<<hex<<"0x"<<cont[12]<<cont[11]<<endl;
    
    num=(cont[7]-11)/4;//���������� ������������� ���������� - ������ �� 4 �����
    fileOut<<dec<<num<<endl;
    i=1;
    //outfile<<count<<endl;
    //����� Y-�������
      while(i<=num){
    	j=13+(i-1)*4;
    	if(cont[j+1]<16)fileOut<<hex<<"0"<<cont[j+1];
    	else fileOut<<hex<<cont[j+1];
    	if(cont[j]<16)fileOut<<hex<<"0"<<cont[j];
    	else fileOut<<hex<<cont[j];
    	if(cont[j+3]<16)fileOut<<hex<<":0"<<cont[j+3];
    	else fileOut<<hex<<":"<<cont[j+3];
    	if(cont[j+2]<16)fileOut<<hex<<"0"<<cont[j+2]<<endl;
    	else fileOut<<hex<<cont[j+2]<<endl;
    	i++;
    }
    fileOut<<hex<<"ABLNR=0x"<<cont[count-5]<<cont[count-6]<<"  ";
    fileOut<<hex<<"ABLNA=0x"<<cont[count-3]<<cont[count-4]<<endl;
    fileOut<<hex<<"B=0x"<<cont[count-1]<<cont[count-2]<<endl;
    fileOut<<hex<<"P=0x"<<cont[count]<<endl;
}

