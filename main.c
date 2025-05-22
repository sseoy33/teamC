//-----------------–-main.c---------------------

#ifdef __linux__    //리눅스 환경에서는 unistd.h를 사용
#include <unistd.h> //출력 버퍼 비우기, sleep으로 화면 대기를 위해 사용 
#define sleep(s) sleep(s)
//문자열 비교를 위해서 fgets시 입력된 개행문자를 지워야 함
#define deleteEnter()                  {ingredientName[len - 2] = '\0';\
                                            ingredientName[len - 1] = '\0'; }   //online gdb에서는 한글 뒤의 개행문자를 지우려면 2바이트를 지워야 함
#elif _WIN32  //윈도우 환경에서는 windows.h를 사용
#define _CRT_SECURE_NO_WARNINGS 
#include <windows.h>    
#define sleep(s) Sleep((s)*1000)    //Sleep은 ms단위로 대기하기 때문에 1000을 곱해줌
#define deleteEnter() 			   { ingredientName[len - 1] = '\0'; }   //문자열 비교를 위해서 fgets시 입력된 개행문자를 지워야 함
#endif

#include <stdio.h>  //입출력을 위해 사용
#include <time.h>   //랜덤 함수 초기화, 요리에 걸린 시간 계산을 위해서 사용 
#include <stdlib.h> //랜덤 함수로 손님의 무작위 주문을 위해서 사용
#include <string.h> //문자열인 손님 주문 목록 과 플레이어 선택 목록을 비교, 현재 해금한 재료 목록을 갱신하기 위해 사용
#include "ingredient.h" //게임 변수들과 재료 도트 배열, 재료 출력과 관련된 함수가 모여있는 헤더파일

// 각종 변수, 재료 출력과 선택에 관한 함수는 "ingredient.h"에 있음


//"main.c" 함수 목록
void intro();   //게임 시작시 스토리, 게임 설명 등을 출력하기 위한 함수
void printStage(int maxCustomer, int* currentGold, int* unlockCount);  // 현재 스테이지를 출력, 2스테이지 부터는 해금 단계도 출력/ 3스테이지 이후 재료 해금 한번 하고 게임 엔딩
void cpyIngredient(char ingredientName[MAX_IN][20], char list[NEED_MAX]);  //(손님의 랜덤 주문 또는 플레이어)가 선택한 재료를 (손님 또는 플레이어) 구조체 문자열 멤버에 복사하는 함수 
void displayCustomer(); //손님의 주문을 랜덤으로 정하고 화면에 출력
void cooking(int maxCustomer, int* currentGold); //요리 진행 함수 / 플레이어 재료 선택, 진행되는 요리 과정 출력 
int customerScore(); //손님 랜덤 주문 목록과 플레이어 재료 선택 목록을 비교해 맞춘 정도 따라 수치 반환
void ShowScore(int correct, int plusScore, int plusGold, int* currentGold);   //맞춘 정도에 따라 계산한 손님 만족도, 획득 점수, 획득 금액을 출력
void ShowEnding(int finalGold, int unlockCount);  //엔딩 출력

int main() {    //main

    int customerCount = (currentStage * DEFAULT_CUSTOMER);    //스테이지 별 손님 수 = 현재 스테이지 * 기본 손님 수 
    int gold = INITIAL_GOLD;    //뵤유 금액 = 초기 금액
    int unlockTotal = 0;   //해금된 재료 수 확인(엔딩 조건 검사를 위해서)

    intro();    //인트로 출력    

    while (currentStage <= 3) { // 최대 스테이지인 3스테이지까지만 반복

        customerCount = (currentStage * DEFAULT_CUSTOMER);    //스테이지 별 손님 수 = 현재 스테이지 * 기본 손님 수 

        printStage(customerCount, &gold, &unlockTotal);   //현재 스테이지, 재료해금 출력
        cooking(customerCount, &gold);  //요리 과정 진행, 출력
        currentStage++; //다음 스테이지로

    }
    printStage(customerCount, &gold, &unlockTotal);   //엔딩 전 두 번째 엔딩 조건 충족을 위한 해금단계 진입을 위해 한번 더 스테이지 출력
    ShowEnding(gold, unlockTotal);   //엔딩 출력


    return 0;   //게임 종료
}

void intro() {   //게임 시작시 한번 나오는 튜토리얼을 위한 함수

    printf("\033[?25l");    //깜빡이는 커서를 보이지 않게함

    int argb = 0, r = 0, g = 0, b = 0, a = 0; //각 재료의 a(투명도 : 보임 or 안보임),rgb값을 저장할 변수

    FILE* fp = NULL;    //파일 포인터 선언
    fp = fopen("INTRO.txt", "rt");   //유형별 재료 텍스트파일 열기

    for (int i = 0; i < 13; i++) { //각 행 반복(위에서 아래로)
        for (int k = 0; k < 50; k++) { //각 열 반복(왼쪽에서 오른쪽으로)
            fscanf(fp, "%d", &argb);    //한 도트당 10자리수 정수 0,000,000,000가 저장됨 맨앞자리 0이 a(0 or 1 : 투명 or 보임), 그다음 000은 b(0~255), 다음 000은 g(0~255), 마지막 000은 r(0~255)값
            a = argb / 1000000000; //나눠서 맨 앞자리 만 저장
            b = (argb % 1000000000) / 1000000;   //a 값은 제외하고 그다음 000만 저장
            g = (argb % 1000000) / 1000;    //a,b값ㅇ 제외하고 그다음 000만 저장
            r = argb % 1000;   //a,b,g값 제외하고 마지막 000만 저장

            if (a != 0) {   // a(투명도) 가 0이(투명) 아니면 
                printf("\033[48;2;%d;%d;%dm  \033[0m", r, g, b);  //도트(공백 2칸) 출력 / [48;2;R;G;B 는 배경색을 0~255 rgb값으로 설정하는 이스케이프 시퀀스
            }
            else {   //투명도가 0이면
                printf("\033[2C"); //도트 출력X(커서 두칸 이동)
            }
        }
        printf("\n"); //줄바꿈
    }
    fclose(fp); //파일 닫기


    printf("\033[17;40f게임 시작>[Enter]");  //게임 진행 안내 문구
    fflush(stdout); //printf 내용이 버퍼에 남아 화면에 안보이는 경우가 있기 때문에 출력버퍼를 비워줌
    getchar();  //문자 하나를 입력받는 함수로 Enter입력으로 넘어갈 수 있음

}

void printStage(int maxCustomer, int* currentGold, int* unlockCount) {  //현재 스테이지를 출력하고 2스테이지 부터는 재료 해금 단계도 출력하는 함수

    clearDisplay();  //화면 전체를 지움
    printf("\033[?25l");    //깜빡이는 커서를 보이지 않게함
    if (currentStage <= 3) {
        printf("\033[1m\033[6;16fSTAGE %d  \033[0m", currentStage); //굵은 글씨로 현재 스테이지 출력
        fflush(stdout); //printf 내용이 버퍼에 남아 화면에 안보이는 경우가 있기 때문에 출력버퍼를 비워줌
        sleep(2);   //현재 스테이지를 화면에 표시하기 위한 2초 대기
    }
    if ((currentStage > 1) && (currentStage <= 4)) {  //2스테이지 부터 재료 해금 단계 출력
        printf("\033[2K\033[1m\033[6;16f재료 해금\033[0m");    //굵은 글씨로 게임 진행 안내 문구
        fflush(stdout); //printf 내용이 버퍼에 남아 화면에 안보이는 경우가 있기 때문에 출력버퍼를 비워줌
        sleep(2);       //재료 해금 화면에 표시하기 위한 2초 대기
        addIngredient(VEGETABLE1, totalVegetable, currentVegetable, currentGold, unlockCount);    //채소 해금 단계 "ingredient.h"에 있는 함수
        addIngredient(SAUCE1, totalSauce, currentSauce, currentGold, unlockCount);    //소스 해금 단계 "ingredient.h"에 있는 함수
        addIngredient(DRINK, totalDrink, currentDrink, currentGold, unlockCount);    //음료 해금 단계 "ingredient.h"에 있는 함수
    }

    if (currentStage <= 3) {
        clearDisplay();  //화면 전체를 지움  
        printf("\033[?25l");    //깜빡이는 커서를 보이지 않게함
        printf("\033[1m\033[6;14f제한시간 %d분  \033[0m", currentStage * 2); //굵은 글씨로 현재 스테이지의 제한시간 출력
        fflush(stdout); //printf 내용이 버퍼에 남아 화면에 안보이는 경우가 있기 때문에 출력버퍼를 비워줌
        sleep(2);   //제한시간을 화면에 표시하기 위한 2초 대기 
        printf("\033[2K\033[1m\033[6;16f손님 %d명  \033[0m", maxCustomer); //굵은 글씨로 현재 스테이지의 최대 손님 수 출력
        fflush(stdout); //printf 내용이 버퍼에 남아 화면에 안보이는 경우가 있기 때문에 출력버퍼를 비워줌
        sleep(2);   //제한시간을 화면에 표시하기 위한 2초 대기 
    }

}
// 손님의 랜덤 재료 또는 플레이어가 선택한 재료의 이름을 저장하기 위한 함수
void cpyIngredient(char ingredientName[MAX_IN][20], char list[NEED_MAX]) {   //함수의 전달인자로 배열을 받을 땐 그 주소값이 들어 오기 때문에 변경 값이 배열에도 적용 됨

    strcpy(ingredientName[BREAD], currentBread[list[BREAD]]);                //(손님 랜덤 이나 플레이어 선택)의 ingredient[]문자열은 재료 이름("양상추"~"피클"), list[]배열은 인덱스[재료 유형]마다 재료 번호 - 1 (0~6)이 저장됨
    strcpy(ingredientName[MEAT], currentMeat[list[MEAT]]);                   //이 함수에서는 list[]에 따라 current재료[]문자열(현재 해금 되어 있는 재료 이름들)에서 "재료 이름"을 ingredient에 복사함
    strcpy(ingredientName[VEGETABLE1], currentVegetable[list[VEGETABLE1]]);  //BREAD(0)~DRINK(6) 인덱스로 list[]의 각 재료 유형(ex채소)에 접근해 
    strcpy(ingredientName[VEGETABLE2], currentVegetable[list[VEGETABLE2]]);  //저장되어 있는 재료의 번호 ex)list[VEGETABLE2] = 0 (재료 번호-1)인 0을 받아
    strcpy(ingredientName[SAUCE1], currentSauce[list[SAUCE1]]);              //current재료([0]="양상추")를 state의 ingredient([0]="양상추")로 복사
    strcpy(ingredientName[SAUCE2], currentSauce[list[SAUCE2]]);              //해금하지 않은 재료는 current재료[]에 "\0"로 저장되어 있기 때문에 복사해도 의미 없음
    strcpy(ingredientName[DRINK], currentDrink[list[DRINK]]);
}

void displayCustomer() {

    clearDisplay();  //화면 전체를 지움
    printf("\033[?25l");    //깜빡이는 커서를 보이지 않게함

    Need.total = 2; //손님 주문의 총 재료 수 초기화(빵과 고기는 기본으로 주문되기 때문에 2), 후에 플레이어가 맞춘 수와 비교해 점수 계산

    int randomCustomer = 0; // 랜덤한 손님의 이름 인덱스 초기화
    srand(time(NULL));  //현재 시간으로 랜덤을 초기화

    randomCustomer = rand() % 6;  //랜덤 손님이름 인덱스 (0~5)

    switch (currentStage) {   //스테이지에 따라 손님이 주문하는 재료 최대 개수 증가
    case 3:                         // 3스테이지 부터
        Need.list[SAUCE2] = rand() % 5;   // 2번째 소스 추가  
    case 2:                         // 2스테이지 부터
        Need.list[VEGETABLE2] = rand() % 5;   // 2번째 채소 추가
    case 1:                         // 1스테이지
        Need.list[BREAD] = rand() % 2;    // 랜덤 빵 번호 (0,1)->(위트,화이트)
        Need.list[MEAT] = rand() % 3;     // 랜덤 고기 번호 (0~2)->(햄,치킨,새우) 
        Need.list[VEGETABLE1] = rand() % 5; // 랜덤 채소 번호 (0~4)->(양상추,토마토,양파,오이,피클)
        Need.list[SAUCE1] = rand() % 5;   // 랜덤 소스 번호 (0~4)->(소금,후추,머스타드,스위트칠리,랜치)
        Need.list[DRINK] = rand() % 2;    // 랜덤 음료 번호 (0,1)->(콜라,사이다)
    }



    clearDisplay();  //화면 전체를 지움
    printf("\033[2;1f%s :", customerList[randomCustomer]);   //상단에 랜덤 손님이름 출력
    printf("\033[3;1f빵은 %s/ ", currentBread[Need.list[BREAD]]);// 이어서 빵 이름[ 랜덤 번호[빵] ] 출력
    printf("메인은 %s/ ", currentMeat[Need.list[MEAT]]); // 이어서 고기 이름[ 랜덤 번호[고기] ] 출력
    printf("채소는 ");  // 이어서 조건문에 따라 채소 이름[ 랜덤 번호[채소] ] 출력
    // 만약 채소가 해금 되어 있고 동일한 2가지 채소(2스테이지부터 나오는)라면 
    if ((currentVegetable[Need.list[VEGETABLE1]][0] != '\0') && (Need.list[VEGETABLE1] == Need.list[VEGETABLE2])) {
        printf("%s 2단/ ", currentVegetable[Need.list[VEGETABLE1]]); //"채소이름 2단, " 출력 
        Need.total += 2;    //주문 재료 수 +2
    }
    else {
        //동일한 채소가 아니면 각 랜덤 채소를 출력
        for (int i = VEGETABLE1; i <= VEGETABLE2; i++) {   //채소 최대 2개 만큼 반복
            if (currentVegetable[Need.list[i]][0] != '\0') {    //해금되어있는 채소라면
                printf("%s/ ", currentVegetable[Need.list[i]]);  //채소 출력
                Need.total++;   //주문 재료 수 +1
            }
        }
        //랜덤 채소들이 모두 해금되어있지 않다면 기본 채소로 양상추
        if (currentVegetable[Need.list[VEGETABLE1]][0] == '\0' && currentVegetable[Need.list[VEGETABLE2]][0] == '\0') {
            Need.list[VEGETABLE1] = 0;  // 손님 랜덤 채소 1개 양상추로 변경
            printf("%s/ ", currentVegetable[Need.list[VEGETABLE1]]); //양상추 출력
            Need.total++; //주문 재료 수 +1
        }
    }
    printf("소스는 ");// 이어서 조건문에 따라 소스 이름[ 랜덤 번호[소스] ] 출력
    // 만약 소스가 해금 되어 있고 동일한 2가지 소스(3스테이지부터 나오는)라면 
    if ((currentSauce[Need.list[SAUCE1]][0] != '\0') && (Need.list[SAUCE1] == Need.list[SAUCE2])) {
        printf("%s 2번/ ", currentSauce[Need.list[SAUCE1]]); //"소스이름 2단, " 출력
        Need.total += 2;	 //주문 재료 수 +2
    }
    else {
        //동일한 소스가 아니면 각 랜덤 소스를 출력
        for (int i = SAUCE1; i <= SAUCE2; i++) {   //소스 최대 2개 만큼 반복
            if (currentSauce[Need.list[i]][0] != '\0') {  //해금되어있는 소스라면
                printf("%s/ ", currentSauce[Need.list[i]]);    //소스 출력
                Need.total++;   //주문 재료 수 +1
            }
        }
        //랜덤 소스들이 모두 해금되어있지 않다면 기본 소스로 소금
        if (currentSauce[Need.list[SAUCE1]][0] == '\0' && currentSauce[Need.list[SAUCE2]][0] == '\0') {
            Need.list[SAUCE1] = 0;  //손님 랜덤 소스 1개 소금으로 변경
            printf("%s/ ", currentSauce[Need.list[SAUCE1]]); //소금 출력
            Need.total++;   //주문 재료 수 +1
        }
    }
    if (currentDrink[Need.list[DRINK]][0] != '\0') {  //랜덤 음료가 해금되어 있다면
        printf("음료는 %s로 해줘", currentDrink[Need.list[DRINK]]);    //음료 이름[ 랜덤 번호[음료] ] 출력
        Need.total++;   //주문 재료 수 +1
    }
    else {   //음료가 해금되어 있지 않다면
        printf("그리고... 음료는 필요없어");    //필요 없음 출력
    }

    cpyIngredient(Need.ingredient, Need.list);   //손님 재료 문자열과 번호 문자열 주소를 함수에 전달해 랜덤으로 정해진대로 문자열 변경
    //채소와 소스가 기본으로 정해지는 경우가 있기 때문에 주문 텍스트 다 출력 후 값을 저장

    for (int i = 3; i >= 1; i--) {    //3 2 1 출력하는 반복문
        printf("\033[1m\033[6;20f%d", i);    // 반복에 따라 3->2->1 출력
        fflush(stdout); //printf 내용이 버퍼에 남아 화면에 안보이는 경우가 있기 때문에 출력버퍼를 비워줌
        sleep(1);   // 3 2 1 화면에 표시하기 위한 1초 대기
    }
    printf("\033[6;18fSTART\033[0m");   //START 출력
    fflush(stdout); //printf 내용이 버퍼에 남아 화면에 안보이는 경우가 있기 때문에 출력버퍼를 비워줌
    sleep(1);   //START 화면 표시를 위한 1초 대기
    clearDisplay();  //화면 전체를 지움
    fflush(stdout); //printf 내용이 버퍼에 남아 화면에 안보이는 경우가 있기 때문에 출력버퍼를 비워줌
}


//손님 재료와 플레이어 선택 재료를 비교해 맞춘 정도를 반환하는 함수
int customerScore() {

    Choose.total = 0;   //플레이어가 맞춘 개수 초기화 

    for (int i = 0; i < NEED_MAX; i++)   //손님 주문 재료 하나하나 플레이어 선택 재료 전체와 비교, 주문 최대치 만큼 반복
        for (int k = 0; k < NEED_MAX; k++)
            //선택한 재료가 있고 주문 재료와 선택 재료 이름이 같다면 
            if ((Choose.ingredient[k][0] != '\0') && (!strcmp(Need.ingredient[i], Choose.ingredient[k]))) {

                Choose.total++; //맞춘 개수 +1
                strcpy(Need.ingredient[i], "\0");// 점수 출력 시 플레이어가 틀린 재료만 확인시켜주기 위해서 문자열 초기화
                strcpy(Choose.ingredient[k], "\0");//중복 카운트를 막기 위해서 문자열 초기화
                //(안지우면 손님요구 양배추 2개, 플레이어 선택 양배추 1개일 때 카운트 두 번 됨)
            }

    switch (Need.total - Choose.total) {//최대 5(만족도 BAD~EXCELLENT)에서 틀린 개수 마다 1빼서 반환
    case 0:
        return 5;
    case 1:
        return 4;
    case 2:
        return 3;
    case 3:
        return 2;
    default:
        return 1;
    }
}



//맞춘 정도에 따라 손님 만족도, 획득 점수, 획득 금액을 출력하는 함수 
void ShowScore(int correct, int plusScore, int plusGold, int* currentGold) {

    clearDisplay();  //화면 전체를 지움

    printCooking(1); //완성된 요리 출력 0:제작중 1:완성

    printf("\033[?25l");    //깜빡이는 커서를 보이지 않게함
    printf("\033[2;1f\033[40m%d단계 >> 점수 : %d  돈 : %d  \033[0m", currentStage, score, *currentGold); //현재 스테이지, 점수, 보유 금액 출력

    printf("\033[4;15f손님 요구 재료 : %d개", Need.total);  //손님 주문 재료 개수 
    fflush(stdout); //printf 내용이 버퍼에 남아 화면에 안보이는 경우가 있기 때문에 출력버퍼를 비워줌
    sleep(1);   //화면에 일정 시간 간격을 두고 표시하기 위한 대기
    printf("\033[5;15f맞은 재료 수   : %d개", Choose.total); //플레이어가 맞춘 개수
    fflush(stdout); //출력 버퍼 비워줌
    sleep(1);   //화면에 일정 시간 간격을 두고 표시하기 위한 대기
    printf("\033[6;15f");   //틀린 재료가 있을 경우 출력하기 위해 커서 이동
    for (int i = 0; i < NEED_MAX; i++) {  //틀린 재료는 점수 계산시 지워지지 않기 때문에 남아있는 재료를 반복문으로 확인해 출력
        if (Choose.ingredient[i][0] == '\0') {
            continue;   //재료 목록에 아무것도 없으면 출력 X
        }
        else {
            printf("\033[40m%s -> \033[31m%s \033[0m", Need.ingredient[i], Choose.ingredient[i]); //재료가 목록에 있으면 틀렸다고 출력
            fflush(stdout); //출력 버퍼 비워줌
            sleep(1);   //화면에 일정 시간 간격을 두고 표시하기 위한 대기
        }
    }
    printf("\033[7;15f\033[1m         %s\033[0m", customerFeeling[correct - 1]);   //손님 만족도 (맞춘정도 = 1~5(점수,금액과 곱셈 계산을 위함) 따라서 -1)  
    fflush(stdout); //출력 버퍼 비워줌
    sleep(1);   //화면에 일정 시간 간격을 두고 표시하기 위한 대기
    printf("\033[9;15f점수 : +%d", plusScore);   //획득 점수(외부에서 계산해서 전달)
    score += plusScore;     //현재 점수에 획득 점수 추가
    fflush(stdout); //출력 버퍼 비워줌
    sleep(1);   //화면에 일정 시간 간격을 두고 표시하기 위한 대기
    printf("\033[10;15f돈 : +%d", plusGold);     //획득 금액(외부에서 계산해서 전달)
    *currentGold += plusGold;   //보유 금액에 획득 금액 추가
    fflush(stdout); //출력 버퍼 비워줌
    sleep(1);   //화면에 일정 시간 간격을 두고 표시하기 위한 대기


}
// 요리 과정 출력, 플레이어 재료 선택하는 함수
// 최대 손님수, 현재 보유 금액 포인터(변경값 적용을 위해서) 전달
void cooking(int maxCustomer, int* currentGold) {

    int correct = 0;    //맞춘 정도(만족도 5단계) 초기화
    int timeLimit = TIME_LIMIT; //제한시간 = timeLimit(초) * 현재 스테이지
    int increaseScore = 0; //요리 한 번에에 획득하는 점수는 만족도5단계 * increaseScore 
    int increaseGold = 0; //요리 한번마다 획득하게 되는 금액

    int durationOfPlayer = timeLimit * currentStage;  //요리 제한 시간 (기본제한시간 * 현재 스테이지)
    int customerCount = maxCustomer;    //나오는 손님 수 (기본손님 * 현재 스테이지)

    do {
        initialDish();          //요리 배열 초기화      "ingredient.h"에 있는 함수
        initialState(&Need);    //손님 주문 재료 초기화     "ingredient.h"에 있는 함수
        initialState(&Choose);  //플레이어 선택 재료 초기화 "ingredient.h"에 있는 함수

        displayCustomer(); //손님 랜덤 재료 주문

        time_t duration = time(NULL);    //소요 시간 측정을 위해 현재 시간 저장

        chooseIngredient(BREAD, totalBread, currentBread, currentGold);   //재료 유형, 총 재료 목록, 해금된 재료 목록, 보유금액을 전달해
        chooseIngredient(MEAT, totalMeat, currentMeat, currentGold);       //각 재료 유형 별로 재료 선택, 선택한 재료로 변화하는 요리 출력, 금액 변화 
        chooseIngredient(VEGETABLE1, totalVegetable, currentVegetable, currentGold);    //"ingredient.h"에 있는 함수  
        chooseIngredient(VEGETABLE2, totalVegetable, currentVegetable, currentGold);
        chooseIngredient(SAUCE1, totalSauce, currentSauce, currentGold);
        chooseIngredient(SAUCE2, totalSauce, currentSauce, currentGold);
        chooseIngredient(DRINK, totalDrink, currentDrink, currentGold);

        cpyIngredient(Choose.ingredient, Choose.list); //플레이어 선택 재료 문자열과 번호 문자열 주소를 함수에 전달해 선택한 대로 문자열 변경

        correct = customerScore();  //점수 계산 함수로 맞춘 정도를 저장

        increaseGold = (DISH_DEFAULT_GOLD * (Need.list[MEAT] + 1)) + (correct * ADDITIONAL_GOLD); //획득 금액 = 요리기본금액 * (손님이 선택한 고기(0~2) +1) + (맞춘정도 * 추가금액)
        increaseScore = (correct * INCREASE_SCORE); //획득 점수 = 맞춘 정도 * 기본획득점수

        //점수 출력 - 만족도(BAD~EXCELLENT), 획득 점수(맞춘 정도 * 기본획득점수), 획득 금액, 보유 금액 포인터
        ShowScore(correct, increaseScore, increaseGold, currentGold);

        durationOfPlayer -= (time(NULL) - duration);    //남은시간 = 제한시간 - 걸린 시간(현재 시간 - 소요 시간 측정 당시 시간)
        (durationOfPlayer < 0) ? durationOfPlayer = 0 : durationOfPlayer;   //남은시간이 0이하면 0으로 (남은시간 -20 이런 식이면 이상하기 때문)
        printf("\033[11;15f남은 시간      : %d초", durationOfPlayer);    //남은시간 출력
        fflush(stdout); //printf 내용이 버퍼에 남아 화면에 안보이는 경우가 있기 때문에 출력버퍼를 비워줌
        sleep(1);   //남은 시간을 화면에 표시하기 위한 1초 대기

        customerCount--; //남은 손님 - 1

    } while (durationOfPlayer > 0 && customerCount > 0); //남은 시간이 0이하거나 남은 손님이 없을 때까지
}
// 엔딩 출력 함수
void ShowEnding(int finalGold, int unlockCount) {

    clearDisplay();  //화면 전체를 지우기
    printf("\033[2;1f\033[40m%d단계 >> 점수 : %d  돈 : %d  \033[0m", currentStage, score, finalGold);   //현재 스테이지, 현재 점수, 보유 금액 출력

    if (score < CLEAR_SCORE) { //클리어 점수보다 낮으면
        printf("\033[7;25f망했어요...");    //BadEnding
    }
    else {   //클리어 점수를 넘었다면
        if (unlockCount == UNLOCK_MAX) {  //모든 재료를 해금 했다면
            printf("\033[7;25f꿈꾸던 요리 완성!!!");
            fflush(stdout);
            return;
        }
        else {   //모든 재료 해금까지는 못했다면
            printf("\033[7;25f음식 장사 성공!!!");
            fflush(stdout);
        }
    }

}
