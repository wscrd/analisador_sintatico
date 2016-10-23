/*
  Integrantes do grupo:
  Wesley Scardua          41380665
  Leonardo Akihiro Hirata 41340086

  Entrada: aquivo "input.java" no formato do minijava 
  no mesmo diretorio contendo o executavel.
  Saida: arquivo "output.txt" com a lista de tokens reconhecidos
  no formato <nome(, valor)?> em cada linha.

  O arquivo de entrada pode conter ou nao espacos entre
  os diferentes lexemas: if(a) e if ( a ), por exemplo.

  a quantidade m·xima de id's aceitos È de 1024
  o maior tamanho possÌvel para um id È de 1024
  o maior tamanho possÌvel para um inteiro È de 20 dÌgitos

  Em caso de sucesso ir· apararecer: Terminado com sucesso!

  Em caso de erro, a ˙ltimas letras lidas e o ˙ltimo caracater lido
  s„o impressos na tela:
  entrad: System.out.pRintln
  "ERRO LEXICO. falha na linha 18: System.out.p
   ⁄ltimo caracter lido -> R". 
  O arquivo de saÌda È gerado atÈ o ˙ltimo token v·lido.

  Caso o ˙ltimo caracter seja \n, \s, \f, \r ou EOF, na saÌda ir· aparecer
  o cÛdigo do caracter, exemplos:
  ⁄ltimo caracter -> \(13)
  (e caso seja o final do arquivo)
  ⁄ltimo caracter lido -> EOF
  
  Observacoes:
  Systemout.println È reconhecido como <id,1><pontuacao, ponto><id,2>
  System.ouT.println n„o È reconhecido.
  System.outprintln tambÈm n„o È.
  O critÈrio estabelecido foi que uma vez que aparece "System.",
  necessariamente deve ser System.out.println. Dessa forma n„o h·°
  problema em nomear um id como System'qualquer caracter'.
  
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define ID_MAX_LENGHT 1024 // limite de tamanho para um id qualquer
#define NUM_MAX_SIZE 20 // limite do tamanho de um numero inteiro
#define ID_TABLE_MAX_SIZE 1024 //limite do numero de id possiveis em um programa

#define IF "if"
#define INTEGER "int"
#define FALSO "false"
#define WHILE "while"
#define CLASS "class"
#define LENGTH "lenght"
#define ELSE "else"
#define EXTENDS "extends"
#define ID "id"
#define NUM "num"
#define PONTUACAO "pontuacao"
#define OPERADOR "operador"
#define NULO "null"
#define NEW "new"
#define VERDADEIRO "true"
#define THIS "this"
#define MAIN "main"
#define VOID "void"
#define PUBLIC "public"
#define STATIC "static"
#define STRING "String"
#define SYSTEMOUTPRINTLN "System.out.println"
#define RETURN "return"
#define BOOLEAN "boolean"

#define ATRB "atrb"
#define SOMA "soma"
#define SUB "sub"
#define MULT "mult"
#define E "e"
#define COMPLEMENTAR "compl"
#define MENORQ "menorq"
#define IGUAL "igual"
#define DIF "dif"

#define ABREPAR "abrepar"
#define FECHAPAR "fechapar"
#define ABRECOL "abrecol"
#define FECHACOL "fechacol"
#define ABRECH "abrech"
#define FECHACH "fechach"
#define PV "pontovirgula"
#define PNT "ponto"
#define VL "virgula"

#define BRKLINE '\n'
#define SPACE ' '
#define TAB '\t'
#define CARRIAGERETURN '\r'
#define FORMFEED '\f'

typedef struct {
  char *name;
  char *value;
}token_t;

//state_t guarda referencias ao estado do parse tais como:
//tabela de id's, buffer e indices para guardar os id's.
typedef struct{
  int breakLines; //numero de quebras de linha
  int bufferBegin;//a posicao inicial do id
  int bufferEnd;  //a posicao final + 1 do id 
  int lookNext;   //flag usada para evitar que o programa olha para proxima letra do arquivo 
  int commentSection; //flag usada para desligar o buffer no caso de comentarios
  char buffer[ID_MAX_LENGHT]; //inicializa o buffer
  char *idTable[ID_TABLE_MAX_SIZE];//initializa a tabela de vari·veis
  int idTableSize;//tamanho atual da tabela de id's
  FILE *input;//referencia do arquivo de entrada
  FILE *output;//referencia do arquivo de saida
}state_t;

//faz a leitura de caracteres do arquivo e salva no buffer se È caracter de id
char readChar(state_t *s);

//retorna 1 se first e second sao iguais
//retorna 0, caso contrario
int areStringsEqual(char *first, char *second);
//retorna 1 se o caracter for de 0 a 9,
//retorna 0 caso contr·°rio
int isNumeral(char c);
//retorna 1 se o caracter for alfanumerico ou underscore,
//retorna 0 caso contr·°rio
int isIDChar(char c);
//retorna 1 se o caracter for \r \s \f \t,
//retorna 0 caso contr·°rio
int isBlank(char c);
//retorna 1 se o caracter for +. -. *,
//retorna 0 caso contr·°rio
int isArithmeticOperator(char c);
//retorna 1 se o caracter for de o primeiro caracter nao for de uma palavra reservada,
//retorna 0 caso contr·°rio
int isNonReservedFirstLetter(char c);
//retorna 1 se o caracter for de .,;{}()[],
//retorna 0 caso contr·°rio
int isPunctuation(char c);
//retorna 1 s e o caracter for letra maiuscula,
//retorna 0 caso contr·°rio
int isUpperCaseChar(char c);


//busca id na tabela de id's e retorna indice positivo se for encotrado,
//ou um valor negativo caso n·£o seja encontradao
int searchIdTable(char *id, state_t *s);

//alloca novo token
token_t *createToken(char *name, char *value);

//adiciona novo id a tabela de ids
void addNewId(char *id, state_t *s);
//metodo auxiliar para colocar os caracteres no buffer
void enqueueChar(char c, state_t *s);
//limpa o buffer de id's
void cleanBuffer(state_t *s);
//fecha o arquivo
void closeFile(FILE *file);
//metodo auxiliar para mostra o buffer
void showBuffer(state_t *s);
//metodo auxilar para mostrar a tabela de id's
void showTable(state_t *s);
//faz os processos de convers·£o lexema token
void tokenizer(state_t *s);
//metodo auxiliar para salvar a quantidade de quebras de linhas e 
//caracteres lidos
void updateState(state_t *s, char c);
//metodo auxiliar para leitura de comentarios
void turnOnCommentSection(state_t *s);
//metodo auxiliar para leitura de comentarios
void turnOffCommentSection(state_t *s);
//salva token no arquivo, usando o buffer se for um id,
void saveToken(token_t *t, state_t *s);
//salva token diretamente no arquivo, ignorando o buffer
void saveTokenToFile(token_t *t, state_t *s);
//salva token de pontuacao
void savePunctuation(char c, state_t *s);
//salva token para operadores de um caracter so: + - * <
void saveSimpleOperator(char c, state_t *s);
//salva token para operadores com um ou dois caracateres: != == = !
void saveOperator(char *name, state_t *s);
//salva palavra reservada
void saveReservedWord(char *name, state_t *s);
//salva id do buffer no arquivo
void saveId(state_t *s);
//salva token de numeral
void saveNum(state_t *s);
//limpa memorial do estado alocado
void freeState(state_t *s);

int main(int argc, char *argv[]) {
  char *inputName = "input.java";
  char *outputName = "output.txt";
  FILE *input = fopen(inputName, "r");
  FILE *output = fopen(outputName, "w");

  if(input == NULL) {
    printf("Error: %s could not be found\n", inputName);
    exit(1);
  }
  if(output == NULL) {
    printf("Error: %s could not be created\n", outputName);
    exit(1);
  }

  state_t *s = calloc(1, sizeof(state_t));
  s -> breakLines = 1;
  s -> bufferBegin = 0;
  s -> bufferEnd = 0;
  s -> lookNext = 1;
  s -> commentSection = 0;
  s -> idTableSize = 0;
  s -> input = input;
  s -> output = output;
  tokenizer(s);
  freeState(s);
  system("pause");
  return 0;
}

void tokenizer(state_t *s) {
  char c;
  char *context;

  q0:
      if(s -> lookNext) {
        c = readChar(s);
      }

      if(c == 'i'){
        goto q55;
      }
      else if(c == 'c'){
        goto q10;
      }
      else if(c == 'f') {
        goto q87;
      }
      else if(c == 'w') {
        goto q73;
      }
      else if(c == 'l') {
        goto q77;
      }
      else if(c == 'e') {
        goto q57;
      }
      else if(c == 'n') {
        goto q103;
      }
      else if(c == 't') {
        goto q92;
      }
      else if(c == 'm') {
        goto q28;
      }
      else if(c == 'v') {
        goto q32;
      }
      else if(c == 'p') {
        goto q33;
      }
      else if(c == 's') {
        goto q49;
      }
      else if(c == 'S') {
        goto q107;
      }
      else if(c == 'r') {
        goto q40;
      }
      else if(c == 'b') {
        goto q2;
      }
      else if(c == '/') {
        goto q23;
      }
      else if(isNonReservedFirstLetter(c)) {
        goto q60;
      }
      else if(isPunctuation(c)) {
        goto q62;
      }
      else if(isNumeral(c)) {
        goto q64;
      }
      else if(isArithmeticOperator(c)) {
        goto q66;
      }
      else if(c == '=') {
        goto q67;
      }
      else if(c == '<') {
        goto q65;
      }
      else if(c == '!') {
        goto q70;
      }
      else if(c == '&') {
        goto q68;
      }
      else if(isBlank(c)) {
        s -> lookNext = 1;
        goto q0;
      }
      else if(c < 0) {
        goto qfinal;
      }
      else {
        context = s -> buffer;
        goto qerro;
      }
  q70:
      c = readChar(s);

      if(c == '=') {
        goto q59;
      }
      else {
        saveOperator(COMPLEMENTAR, s);
        updateState(s, c);
        s -> lookNext = 0;
        goto q0;
      }
  q59:
      saveOperator(DIF, s);
      goto q0;
  q66:
      saveSimpleOperator(c, s);
      goto q0;
  q67:
      c = readChar(s);

      if(c == '=') {
        goto q15;
      }
      else {
        saveOperator(ATRB, s);
        updateState(s, c);
        s -> lookNext = 0;
        goto q0;  
      }
  q68:
      c = readChar(s);

      if(c == '&') {

        goto q69;
      }
      else {
        context = "&";
        goto qerro;
      }
  q69:
      saveOperator(E, s);
      goto q0;
  q15:
      saveOperator(IGUAL, s);
      goto q0;
  q65:
      saveSimpleOperator(c, s);
      goto q0;
  q73:
      c = readChar(s);

      if(c == 'h') {
        goto q74;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q74:
      c = readChar(s);

      if(c == 'i') {
        goto q75;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q75:
      c = readChar(s);

      if(c == 'l') {
        goto q76;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q76:
      c = readChar(s);

      if(c == 'e') {
        goto q96;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q96:
      c = readChar(s);

      if(isIDChar(c)) {
        goto q60;
      }
      else {
        saveReservedWord(WHILE, s);
        goto q0;
      }
  q77:
      c = readChar(s);

      if(c == 'e') {
        goto q78;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q78:
      c = readChar(s);

      if(c == 'n') {
        goto q79;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q79:
      c = readChar(s);

      if(c == 'g') {
        goto q80;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q80:
      c = readChar(s);

      if(c == 't') {
        goto q81;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q81:
      c = readChar(s);
      if(c == 'h') {
        goto q82;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q82:
      c = readChar(s);

      if(isIDChar(c)) {
        goto q60;
      }
      else {
        saveReservedWord(LENGTH, s);
        goto q0;
      }  
  q87:
      c = readChar(s);

      if(c == 'a') {
        goto q88;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q88:
      c = readChar(s);

      if(c == 'l') {
        goto q89;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q89:
      c = readChar(s);

      if(c == 's') {
        goto q90;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q90:
      c = readChar(s);

      if(c == 'e') {
        goto q91;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q91:
      c = readChar(s);

      if(isIDChar(c)) {
        goto q60;
      }
      else {
        saveReservedWord(FALSO, s);
        goto q0;
      }
  q57:
      c = readChar(s);

      if(c == 'l') {
        goto q58;
      }
      else if(c == 'x') {
        goto q17;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q58:
      c = readChar(s);

      if(c == 's') {
        goto q71;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q71:
      c = readChar(s);

      if(c == 'e') {
        goto q72;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q72:
      c = readChar(s);

      if(isIDChar(c)) {
        goto q60;
      }
      else {
        saveReservedWord(ELSE, s);
        goto q0;
      }
  q17:
      c = readChar(s);

      if(c == 't') {
        goto q18;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q18:
      c = readChar(s);

      if(c == 'e') {
        goto q19;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q19:
      c = readChar(s);

      if(c == 'n') {
        goto q20;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q20:
      c = readChar(s);

      if(c == 'd') {
        goto q21;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q21:
      c = readChar(s);

      if(c == 's') {
        goto q22;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q22:
      c = readChar(s);

      if(isIDChar(c)) {
        goto q60;
      }
      else {
        saveReservedWord(EXTENDS, s);
        goto q0;
      }
  q103:
      c = readChar(s);

      if(c == 'u') {
        goto q104;
      }
      else if(c == 'e') {
        goto q101;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q104:
      c = readChar(s);

      if(c == 'l') {
        goto q105;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q105:
      c = readChar(s);

      if(c == 'l') {
        goto q106;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q106:
      c = readChar(s);

      if(isIDChar(c)) {
        goto q60;
      }
      else {
        saveReservedWord(NULO, s);
        goto q0;
      }
  q101:
      c = readChar(s);

      if(c == 'w') {
        goto q102;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q102:
      c = readChar(s);

      if(isIDChar(c)) {
        goto q60;
      }
      else {
        saveReservedWord(NEW, s);
        goto q0;
      }
  q92:
      c = readChar(s);

      if(c == 'r') {
        goto q84;
      }
      else if(c == 'h'){
        goto q93;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q84:
      c = readChar(s);

      if(c == 'u') {
        goto q85;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q85:
      c = readChar(s);

      if(c == 'e') {
        goto q86;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q86:
      c = readChar(s);

      if(isIDChar(c)) {
        goto q60;
      }
      else {
        saveReservedWord(VERDADEIRO, s);
        goto q0;
      }
  q93:
      c = readChar(s);

      if(c == 'i') {
        goto q94;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q94:
      c = readChar(s);

      if(c == 's') {
        goto q95;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q95:
      c = readChar(s);

      if(isIDChar(c)) {
        goto q60;
      }
      else {
        saveReservedWord(THIS, s);
        goto q0;
      }
  q28:
      c = readChar(s);

      if(c == 'a') {
        goto q29;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q29:
      c = readChar(s);

      if(c == 'i') {
        goto q30;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q30:
      c = readChar(s);

      if(c == 'n') {
        goto q31;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q31:
      c = readChar(s);

      if(isIDChar(c)) {
        goto q60;
      }
      else {
        saveReservedWord(MAIN, s);
        goto q0;
      }
  q32:
      c = readChar(s);

      if(c == 'o') {
        goto q46;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q46:
      c = readChar(s);

      if(c == 'i') {
        goto q47;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q47:
      c = readChar(s);

      if(c == 'd') {
        goto q61;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q61:
      c = readChar(s);

      if(isIDChar(c)) {
        goto q60;
      }
      else {
        saveReservedWord(VOID, s);
        goto q0;
      }
  q33:
      c = readChar(s);

      if(c == 'u') {
        goto q34;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q34:
      c = readChar(s);

      if(c == 'b') {
        goto q35;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q35:
      c = readChar(s);

      if(c == 'l') {
        goto q36;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q36:
      c = readChar(s);

      if(c == 'i') {
        goto q37;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q37:
      c = readChar(s);

      if(c == 'c') {
        goto q38;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q38:
      c = readChar(s);

      if(isIDChar(c)) {
        goto q60;
      }
      else {
        saveReservedWord(PUBLIC, s);
        goto q0;
      }
  q49:
      c = readChar(s);

      if(c == 't') {
        goto q50;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q50:
      c = readChar(s);

      if(c == 'a') {
        goto q51;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q51:
      c = readChar(s);

      if(c == 't') {
        goto q52;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q52:
      c = readChar(s);

      if(c == 'i') {
        goto q53;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q53:
      c = readChar(s);

      if(c == 'c') {
        goto q54;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q54:
      c = readChar(s);

      if(isIDChar(c)) {
        goto q60;
      }
      else {
        saveReservedWord(STATIC, s);
        goto q0;
      }
  q107:
      c = readChar(s);

      if(c == 't') {
        goto q48;
      }
      else if(c == 'y') {
        goto q108;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q108:
      c = readChar(s);

      if(c == 's') {
        goto q109;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q109:
      c = readChar(s);

      if(c == 't') {
        goto q110;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q110:
      c = readChar(s);

      if(c == 'e') {
        goto q111;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q111:
      c = readChar(s);

      if(c == 'm') {
        goto q112;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q112:
      c = readChar(s);

      if(c == '.') {
        goto q113;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q113:
      c = readChar(s);

      if(c == 'o') {
        goto q114;
      }
      else {
        context = "System.";
        goto qerro;
      }
  q114:
      c = readChar(s);

      if(c == 'u') {
        goto q115;
      }
      else {
        context = "System.o";
        goto qerro;
      }
  q115:
      c = readChar(s);

      if(c == 't') {
        goto q116;
      }
      else {
        context = "System.ou";
        goto qerro;
      }
  q116:
      c = readChar(s);

      if(c == '.') {
        goto q117;
      }
      else {
        context = "System.out";
        goto qerro;
      }
  q117:
      c = readChar(s);

      if(c == 'p') {
        goto q118;
      }
      else {
        context = "System.out.";
        goto qerro;
      }
  q118:
      c = readChar(s);

      if(c == 'r') {
        goto q119;
      }
      else {
        context = "System.out.p";
        goto qerro;
      }
  q119:
      c = readChar(s);

      if(c == 'i') {
        goto q120;
      }
      else {
        context = "System.out.pr";
        goto qerro;
      }
  q120:
      c = readChar(s);

      if(c == 'n') {
        goto q121;
      }
      else {
        context = "System.out.pri";
        goto qerro;
      }
  q121:
      c = readChar(s);

      if(c == 't') {
        goto q122;
      }
      else {
        context = "System.out.prin";
        goto qerro;
      }
  q122:
      c = readChar(s);

      if(c == 'l') {
        goto q123;
      }
      else {
        context = "System.out.print";
        goto qerro;
      }
  q123:
      c = readChar(s);

      if(c == 'n') {
        goto q124;
      }
      else {
        context = "System.out.printl";
        goto qerro;
      }       
  q124:
      c = readChar(s);

      if(isIDChar(c)) {
        context = "System.out.println";
        goto qerro;
      }
      else {
        saveReservedWord(SYSTEMOUTPRINTLN, s);
        goto q0;
      }
  q48:
      c = readChar(s);

      if(c == 'r') {
        goto q1;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q1:
      c = readChar(s);

      if(c == 'i') {
        goto q16;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q16:
      c = readChar(s);

      if(c == 'n') {
        goto q24;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q24:
      c = readChar(s);

      if(c == 'g') {
        goto q27;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q27:
      c = readChar(s);

      if(isIDChar(c)) {
        goto q60;
      }
      else {
        saveReservedWord(STRING, s);
        goto q0;
      }
  q40:
      c = readChar(s);

      if(c == 'e') {
        goto q41;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q41:
      c = readChar(s);

      if(c == 't') {
        goto q42;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q42:
      c = readChar(s);

      if(c == 'u') {
        goto q43;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q43:
      c = readChar(s);

      if(c == 'r') {
        goto q44;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q44:
      c = readChar(s);

      if(c == 'n') {
        goto q45;
      }
      else {
        saveId(s);
        goto q0;
      }      
  q45:
      c = readChar(s);

      if(isIDChar(c)) {
        goto q60;
      }
      else {
        saveReservedWord(RETURN, s);
        goto q0;
      }
  q2:
      c = readChar(s);

      if(c == 'o') {
        goto q3;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q3:
      c = readChar(s);

      if(c == 'o') {
        goto q4;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q4:
      c = readChar(s);

      if(c == 'l') {
        goto q5;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q5:
      c = readChar(s);

      if(c == 'e') {
        goto q6;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q6:
      c = readChar(s);

      if(c == 'a') {
        goto q7;
      }
      else {
        saveId(s);
        goto q0;
      }
  q7:
      c = readChar(s);

      if(c == 'n') {
        goto q8;
      }
      else {
        saveId(s);
        goto q0;
      }    
  q8:
      c = readChar(s);

      if(isIDChar(c)) {
        goto q60;
      }
      else {
        saveReservedWord(BOOLEAN, s);
        goto q0;
      }
  q10:
      c = readChar(s);

      if(c == 'l') {
        goto q11;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q11:
      c = readChar(s);

      if(c == 'a') {
        goto q12;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q12:
      c = readChar(s);

      if(c == 's') {
        goto q13;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q13:
      c = readChar(s);

      if(c == 's') {
        goto q14;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q14:
      c = readChar(s);

      if(isIDChar(c)) {
        goto q60;
      }
      else {
        saveReservedWord(CLASS, s);
        goto q0;
      }
  q23:
      c = readChar(s);
      if(c == '*') {
        turnOnCommentSection(s);
        goto q83;
      }
      else if(c == '/') {
        turnOnCommentSection(s);
        goto q39;
      }
      else {
        context = "/";
        goto qerro;
      }
  
  q25:
      c = readChar(s);

      if(c == 't') {
        goto q26;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q26:
      c = readChar(s);

      if(isIDChar(c)) {
        goto q60;
      }
      else {
        saveReservedWord(INTEGER, s);
        goto q0;
      }
  q39:
      c = readChar(s);

      if(c == BRKLINE || c < 0) {
        turnOffCommentSection(s);
        goto q0;
      }
      else {
        goto q39;
      }
  q55:
      c = readChar(s);

      if(c == 'f') {
        goto q56;
      }
      else if(c == 'n') {
        goto q25;
      }
      else if(isIDChar(c)){
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q56:
      c = readChar(s);

      if(isIDChar(c)) {
        goto q60;
      }
      else {
        saveReservedWord(IF, s);
        goto q0;
      }
  q60:
      c = readChar(s);

      if(isIDChar(c)) {
        goto q60;
      }
      else {
        saveId(s);
        goto q0;
      }
  q62:
      savePunctuation(c, s);
      goto q0;
  q64:
      c = readChar(s);

      if(isNumeral(c)) {
        goto q64;
      }
      else if(isIDChar(c)) {
        context = s -> buffer;
        goto qerro;
      }
      else {
        saveNum(s);
        goto q0;
      }
  q83:
      c = readChar(s);
      if(c == '*') {
        goto q97;
      }
      else if(c < 0) {
        context = "EOF: Comentario nao foi fechado";
        goto qerro;
      }
      else {
        goto q83;
      }
  q97:
      c = readChar(s);
      if(c == '/') {
        turnOffCommentSection(s);
        goto q0;
      }
      else if(c == '*'){
        goto q97;
      }
      else if(c < 0) {
        context = "EOF: Comentario nao foi fechado";
        goto qerro;
      }
      else {
        goto q83;
      }
  qfinal:
      printf("Terminado com sucesso!\n");
      return;
  qerro:
      printf("ERRO LEXICO. falha na linha %d: %s \nUltimo caracter lido -> ", s -> breakLines, context);
      if(c < 0) {
        printf("EOF\n");
      }
      else if(isBlank(c)) {
        printf("\\(%d)\n", c);
      }
      else {
        printf("%c\n", c);
      }
}

void closeFile(FILE *file) {
  fclose(file);
}

char readChar(state_t *s) {
  char c;
  FILE *input = s -> input;
  if(fscanf(input, "%c", &c) != EOF) {
    updateState(s, c);
    return c;
  }
  return -1;
}

void updateState(state_t *s, char c) {
  if(c == BRKLINE) {
    s -> breakLines += 1;
  }
  if (isIDChar(c)  && !(s -> commentSection)){
    enqueueChar(c, s);
  }
}
int isBlank(char c) {
  if(c == SPACE || c == BRKLINE ||  c == TAB || c == CARRIAGERETURN || c == FORMFEED) {
    return 1;
  }
  return 0;
}

void saveToken(token_t *t, state_t *s) {
  cleanBuffer(s);
  saveTokenToFile(t, s);
  free(t);
}

void saveTokenToFile(token_t *t, state_t *s) {
  FILE *output = s -> output;

  fprintf(output, "<%s", t -> name);
  if(t -> value != NULL) {
    fprintf(output, ",%s", t -> value);
  }
  fprintf(output, ">\n");
}

void enqueueChar(char c, state_t *s) {
  s -> buffer[s -> bufferEnd] = c;
  s ->buffer[s->bufferEnd + 1] = '\0';
  s -> bufferEnd += 1;
}

void showBuffer(state_t *s) {
  char *buffer = s -> buffer;
  int i, begin, end;
  begin = s -> bufferBegin;
  end = s -> bufferEnd;
  printf("#");
  for (i = begin; i < end; ++i){
    printf("%c", buffer[i]);
  }
  printf("#");
  printf("\n");
}

void showTable(state_t *s) {
  int i;
  char **table = s -> idTable;
  int size = s -> idTableSize;
  for (i = 0; i < size; ++i) {
    printf("%d: %s|\n", (i + 1), table[i]);
  }
}

void saveId(state_t *s) {
  token_t *t;
  char value[10];
  char *id = s -> buffer;
  int index = searchIdTable(id, s);
  if(index < 0) {
    index = s -> idTableSize;
    addNewId(id, s);
  }
  sprintf(value, "%d", (index + 1));
  t = createToken(ID, value);
  saveToken(t, s);
}

void saveNum(state_t *s) {
  token_t *t;
  char num[(NUM_MAX_SIZE + 1)];
  strcpy(num, s -> buffer);
  t = createToken(NUM, num);
  saveToken(t, s);
}

void saveReservedWord(char *name, state_t *s) {
  token_t *t = createToken(name, NULL);
  saveToken(t, s);
}

int areStringsEqual(char *first, char *second) {
  return strcmp(first, second) == 0;
}

void saveOperator(char *name, state_t *s) {
  char *value;
  token_t *t;
  if(areStringsEqual(name, IGUAL)) {
    value = IGUAL;
  }
  else if(areStringsEqual(name, DIF)) {
    value = DIF;
  }
  else if(areStringsEqual(name, E)) {
    value = E;
  }
  else if(areStringsEqual(name, COMPLEMENTAR)) {
    value = COMPLEMENTAR;
  }
  else if(areStringsEqual(name, ATRB)) {
    value = ATRB;
  }
  else {
    value = NULL;
  }
  t = createToken(OPERADOR, value);
  saveToken(t, s);
  s -> lookNext = 1;
}

void saveSimpleOperator(char c, state_t *s) {
  char *value;
  token_t *t;
  switch(c) {
    case '+':
      value = SOMA;
      break;
    case '-':
      value = SUB;
      break;
    case '*':
      value = MULT;
      break;
    case '<':
      value = MENORQ;
      break;
    default:
      value = NULL;
  }
  t = createToken(OPERADOR, value);
  saveToken(t, s);
  s -> lookNext = 1;
}

void savePunctuation(char c, state_t *s) {
  char *value;
  token_t *t;
  switch(c) {
    case '(':
      value = ABREPAR;
      break;
    case ')':
      value = FECHAPAR;
      break;
    case '[':
      value = ABRECOL;
      break;
    case ']':
      value = FECHACOL;
      break;
    case '{':
      value = ABRECH;
      break;
    case '}':
      value = FECHACH;
      break;
    case ';':
      value = PV;
      break;
    case '.':
      value = PNT;
      break;
    case ',':
      value = VL;
      break;
    default:
      value = NULL;
  }
  t = createToken(PONTUACAO, value);
  saveToken(t, s);
  s -> lookNext = 1;
}

token_t *createToken(char *name, char *value) {
  token_t *t;
  t = malloc(sizeof(token_t));
  t -> name = name;
  t -> value = value;
  return t;
}

int isPunctuation(char c) {
  return (c == '(') || (c == ')') || 
         (c == '[') || (c == ']') ||
         (c == '{') || (c == '}') ||
         (c == '.') || (c == ',') ||
         (c == ';'); 
}

int isArithmeticOperator(char c) {
  return (c == '+') || (c == '-') ||
         (c == '*');
}

int isIDChar(char c) {
  return ('a' <= c && c <= 'z') ||
         ('A' <= c && c <= 'Z') ||
         isNumeral(c) || (c == '_');
}

int isUpperCaseChar(char c) {
  return ('A' <= c && c <= 'Z');
}

int isNonReservedFirstLetter(char c) {
  int i;
  char list[12] = {'q', 'u', 'z', 'y', 'x', 'a', 'g', 'd', 'k', 'j', 'h', 'o'};
  if(isUpperCaseChar(c) && c != 'S'){
    return 1;
  }
  for (i = 0; i < 12; ++i){
     if(c == list[i]) {
        return 1;
     }
  }
  return 0;
}

int searchIdTable(char *id, state_t *s) {
  int i;
  char **table = s -> idTable;
  int size = s -> idTableSize;
  for (i = 0; i < size; ++i) {
    if(areStringsEqual(id, table[i])) {
      return i;
    }
  }
  return -1;
}

void addNewId(char *id, state_t *s) {
  char **table = s -> idTable;
  int size = s -> idTableSize;
  table[size] = calloc(1, sizeof(char));
  strcpy(table[size], id);
  s -> idTableSize ++;
}

int isNumeral(char c) {
  return ('0' <= c && c <= '9');
}

int isNotEqual(char c, char d) {
  return c != d;
}

void freeState(state_t *s) {
  int i;
  closeFile(s -> input);
  closeFile(s -> output);
  for (i = 0; i < s -> idTableSize; ++i) {
    free((s -> idTable)[i]);
  }
  free(s);
}

void turnOnCommentSection(state_t *s) {
  s -> commentSection = 1;
}
void turnOffCommentSection(state_t *s) {
  s -> commentSection = 0;
}

void cleanBuffer(state_t *s) {
  s -> lookNext = 0;
  s -> bufferEnd = 0;
  s->bufferBegin = s -> bufferEnd;
}
