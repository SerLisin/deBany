#ifndef _decode_H_ // РµСЃР»Рё РЅРµ РѕРїСЂРµРґРµР»РµРЅРѕ
#define _decode_H_ // РѕРїСЂРµРґРµР»РёС‚СЊ


const int MAX = 250;
// Типы телеграмм из справочника для идентификации сравнением строк
const char str_BST[]=" Status/Bausteintelegramm   DI-Kopplung/Quittierung(Kanal 1)";
const char str_Y_PR[]=" Y-Funktion Anforderungstelegramm vom PR.";
const char str_Y_AS[]=" Y-Funktion Ruecktelegramm der AS ";
const char str_AKS_DI0[]=" AKS-Telegramm   DI-Kopplung/keine Quittierung(Kanal 0)  ";
const char str_AKS_DI1[]=" AKS-Telegramm   DI-Kopplung/Quittierung(Kanal 1)";
const char str_MKS_DI1[]=" MKS-Telegramm   DI-Kopplung/Quittierung(Kanal 1)"; 
const char str_PL[]=" Parameter-Lesen-Telegramm"; 
const char str_PL_answ[]=" Ruecktelegramm auf Parameter lesen/lesen eines absolut adr. Speicherber.";
const char str_PS[]=" Parameter-Schreiben ";
const char str_PS_answ[]=" Rueckantwort auf Parameter-Lesen/Schreiben";
const char str_Uhr_zeit[]=" Uhrzeitsynchronisationstelegramm";
const char str_UI_test[]=" Aufruf zum Anstoss UI-Selbsttest / Gruppenabbild";
const char str_UI_check[]=" UI-Abfrage fuer Gruppenabbild Gruppenadresse";
const char str_Gruppen[]=" Gruppenabbild";
const char str_BAC[]=" Bus-Aktiv-Kontroll-Element";
const char str_BK[]=" Buskopplerabbild";
const char str_Master_Nah[]=" Masterschaftsuebergabe Nahbus";
const char str_Master_Fern[]=" Masterschaftsuebergabe Fernbus";
const char str_Master_Frage_Nah[]=" Masterschaftsabfrage Nahbus";
const char str_Master_Frage_Fern[]=" Masterschaftsabfrage Fernbus";
const char str_LUT[]=" Telegramm mit OPCODE C2/82";
const char str_KQ[]=" Kurzquittung";
const char str_Anz[]=" Anzeigenauftrag";

// Типы головных и канальных операторов модуля EAS (для расшифровки телеграмм BST
//Головные операторы
const short bst_SRZ=0x78; // SRZ или SRZS
const short bst_KRZ=0x7B; // KRZ или KRZS, или MPA
const short bst_RB=0x72;  // RB или RBS
const short bst_ASR=0x82;
const short bst_BSR=0x83;
const short bst_FGS=0x81;
const short bst_AET=0x74;
//Канальные операторы
const short bst_ITE=0x6E;
const short bst_MU=0x12;
const short bst_GWM=0x04;
const short bst_SR=0x79;
const short bst_IBR=0x45;
const short bst_IVL=0x70;
const short bst_KR=0x7C;
const short bst_ES=0x6B;
const short bst_EM=0x69;
const short bst_AE=0x6D;
const short bst_KO=0x03;
const short bst_GS=0x2E;
const short bst_ZIE=0x8D;
const short bst_AETE=0x75;
const short bst_FSM=0x46;
const short bst_FAW=0x00; //FAW или FBW
const short bst_SE=0x47; //System error

struct telegr_time{
	int hour,minute,second,ms;
};
struct gr_mark{
	int num;
	double sec;
};
struct telegr_param{
	int num/*номер телеграммы*/,sen/*отправитель*/,rec_num/*кол-во получателей*/;
	int rec[6];/*получатели*/
};




//константы для служебного байта FC
const int fc_AE=0x01; // Признак элемента адреса
const int fc_ZY=0x02; // Признак цикла передачи
const int fc_A0=0x04; // Признак запроса 0
const int fc_A1=0x08; // Признак запроса 1
const int fc_BD=0x10; // Признак блочного режима передачи
const int fc_ET=0x20; // Признак последнего кадра телеграммы
const int fc_MT=0x40; // Признак ведущего шины
const int fc_FT=0x80; // Признак служебного кадра



bool telegr_AS_decode (int cont[],int count,std::ofstream &fileOut);
bool telegr_PR_decode (int cont[],int count,std::ofstream &fileOut);
bool telegr_MKS_decode (int cont[],int count,std::ofstream &fileOut);
bool telegr_AKS_decode (int cont[],int count,std::ofstream &fileOut);
bool telegr_BST_decode (int cont[],int count,std::ofstream &fileOut);
bool telegr_Uhr_decode (int cont[],int count,std::ofstream &fileOut);
bool telegr_LUT_decode(int cont[],int count,std::ofstream &fileOut);
bool telegr_PL_decode (int cont[],int count,std::ofstream &fileOut); 
telegr_param telegr_MKS_param(int cont[],int count,std::ofstream &fileOut);
telegr_param telegr_AKS_param(int cont[],int count,std::ofstream &fileOut);
telegr_param telegr_BST_param(int cont[],int count,std::ofstream &fileOut);
int telegr_AKS_param(int n, int cont[],int count, std::ofstream &fileOut);
bool FC_decode(int FC,std::ofstream &fileOut);
 
#endif // РєРѕРЅРµС† РѕРїСЂРµРґРµР»РµРЅРёСЏ 
 
