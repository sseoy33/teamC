//----------------ingredient.h------------------

//원활한 코드 이해, 수정을 위한 각 재료의 유형 숫자 매크로
#define BREAD 0
#define MEAT 1
#define VEGETABLE1 2
#define VEGETABLE2 3
#define SAUCE1 4
#define SAUCE2 5
#define DRINK 6

#define MAX_IN 10 //각 유형별 재료의 최대치, 현재는 채소와 소스가 5개로 최대이지만 임의로 10으로 정함
#define NEED_MAX 7  // 손님이 주문하는 재료 가짓수의 최대(빵1,고기1,채소1~2,소스1~2,음료1로 최대 7개 주문함)

//게임 밸런스 조정을 위한 변수들의 초기값 숫자 매크로
#define TIME_LIMIT 120 //제한시간 = TIME_LIMIT(초) * 현재 스테이지
#define DEFAULT_CUSTOMER 1  //스테이지 별 손님 수 = 기본 손님 수 + (현재 스테이지 * 2)
#define INCREASE_SCORE 10   //요리 한 번에 획득하는 점수는 INCREASE_SCORE * 만족도5단계
#define INITIAL_GOLD 1000 //게임 시작 시 초기 금액 
#define DISH_DEFAULT_GOLD 500 //샌드위치 기본금액 = 고기번호 * DISH_DEFAULT_GOLD
#define ADDITIONAL_GOLD 1000    //추가 획득 금액 = ADDITIONAL_GOLD * 만족도5단계
#define UNLOCK_GOLD 1500    //재료 해금에 필요한 금액 = UNLOCK_GOLD * 재료번호 
#define UNLOCK_MAX 10 //해금할 수 있는 총 재료 수 (엔딩 시 모든 재료 해금했는지 확인 위함)
#define CLEAR_SCORE 650 //해피 엔딩을 보기 위한 최소 점수

#define COST_BREAD 10 //요리 중 빵 사용 시 소모되는 비용
#define COST_MEAT 10 //요리 중 고기 사용 시 소모되는 비용 
#define COST_VEGETABLE 10 //요리 중 채소 사용 시 소모되는 비용
#define COST_SAUCE 10 //요리 중 소스 사용 시 소모되는 비용
#define COST_DRINK 10 //요리 중 음료 사용 시 소모되는 비용

//재료 유형 (채소와 소스는 두 번 들어 갈 수 도 있음)  
const char ingredientType[MAX_IN][20] = { "빵","고기","채소","채소","소스","소스","음료" };

//유형 별 재료 목록
const char totalBread[MAX_IN][20] = { "위트","화이트" };
const char totalMeat[MAX_IN][20] = { "햄","치킨","새우" };
const char totalVegetable[MAX_IN][20] = { "양상추","토마토","양파","오이","피클" };
const char totalSauce[MAX_IN][20] = { "소금","후추","머스타드","스위트칠리","랜치" };
const char totalDrink[MAX_IN][20] = { "콜라","사이다" };

const char customerList[6][20] = { "손님a", "손님b", "손님c", "손님d", "손님e", "손님f" }; //손님 종류
const char customerFeeling[5][20] = { "Bad","Normal","Good","Great","Excellent" }; //만족도 종류


//게임 진행을 위한 변수들
int currentStage = 1; //현재 스테이지
int score = 0; //총 얻은 점수
char currentBread[MAX_IN][20] = { "위트","화이트","\0" };    //현재 해금되어 있는 유형 별 재료 목록
char currentMeat[MAX_IN][20] = { "햄","치킨","새우","\0" };  //해금되어 있지 않은 부분은 "\0"로 초기화 됨을 직관적으로 표시
char currentVegetable[MAX_IN][20] = { "양상추","\0" };
char currentSauce[MAX_IN][20] = { "소금","\0" };
char currentDrink[MAX_IN][20] = { "\0" };
struct State {  // (손님의 랜덤 재료 또는 플레이어 선택 재료)를 저장하는 구조체
    char list[NEED_MAX];    //재료 번호를 저장
    char ingredient[MAX_IN][20];    //재료 이름을 저장 
    int total;  // 손님 선택 재료 총 개수 또는 플레이어가 맞춘 재료 개수 저장
};
//구조체 변수 선언
struct State Need;
struct State Choose;
//State 구조체 초기화 함수
void initialState(struct State* state) { //변수의 주소 값을 받아와 함수 외부의 변수 값을 수정 가능하게 함 

    for (int i = 0; i < NEED_MAX; i++) {    //재료 번호의 모든 요소를 9로 초기화
        state->list[i] = 9;            //현재 재료는 최대 5번까지 있음
    }                                   //때문에 아무것도 저장되지 않은 9번으로 초기화
    for (int i = 0; i < MAX_IN; i++) {
        strcpy(state->ingredient[i], "\0");  //재료 이름의 모든 요소를 널문자로 초기화
    }
    state->total = 0;   //total 0으로 초기화
}
// 요리 과정을 표현할 3차원 배열 (각 행의 각 열마다 도트의 a(투명?),r(빨),g(초),b(파)값 저장)
int dish[6][6][4] = { 0 };
//요리 배열 초기화 함수
void initialDish() {
    for (int i = 0; i < 6; i++) { //각 행 반복(위에서 아래로)
        for (int k = 0; k < 6; k++) { //각 열 반복(왼쪽에서 오른쪽으로)
            dish[i][k][0] = 0; //요리 과정 배열 초기화
            dish[i][k][1] = 0;
            dish[i][k][2] = 0;
            dish[i][k][3] = 0;
        }
    }
}


void clearDisplay() {   //화면을 지우는 함수
    
    printf("\033[1;1f\033[?25l");   // 깜빡이는 커서를 보이지 않게 하는 이스케이프 시퀀스( 출력 시 onlineGDB에서는 커서 움직이는게 다 보여서 지저분함 ) https://en.wikipedia.org/wiki/ANSI_escape_code#Control_Sequence_Introducer_commands
    for (int i = 0; i < 20; i++) {
        printf("\033[0m                                                                                                    \n"); //빈 공백 20줄 출력
    }
    printf("\033[1;1f\033[25h");    //커서를 맨 왼쪽 위로 옮겨주고 다시 커서 깜빡임을 보이게 함
}

//"ingredient.h" 함수 목록
void displayIngredient(int TYPE, const char totalIngredient[MAX_IN][20], char currentIngredient[MAX_IN][20], int prep);// 재료 순서대로 화면에 positionIngredient()하는 함수
void positionIngredient(int TYPE, int number, int* cusor, char currentIngredientN[20], int isPrep);// 재료 출력 위치와 방식을 정해 printIngredient()하는 함수
void printIngredient(int TYPE, int stratLine, char currentIngredientN[20]);// 재료 출력 함수 
void chooseIngredient(int TYPE, const char totalIngredient[MAX_IN][20], char currentIngredient[MAX_IN][20], int* currentGold);//요리 진행 시 재료를 선택하는 함수
int printCooking(int cookDone); //화면 왼쪽에 요리 과정 출력 함수 
void addIngredient(int TYPE, const char totalIngredient[MAX_IN][20], char currentIngredient[MAX_IN][20], int* currentGold, int* unlockCount);//해금 단계에서 재료를 해금하는 함수



//요리 과정 출력 함수
int printCooking(int cookDone) { //요리 중(0) 인지 요리 완성(1) 인지 판단하기 위한 변수

    printf("\033[?25l");    //깜빡이는 커서 숨기기(재료 출력 도중 움직이는 커서가 거슬려서)

    int a = 0, r = 0, g = 0, b = 0; // 현재 요리과정 배열(dish)의 각 도트 argb값을 저장할 변수

    for (int i = 0; i < 6; i++) { //각 행 반복(위에서 아래로)

        printf("\033[%d;%df", i + 5, 1);    //출력할 위치 -> 행 : 상단부 점수출력 부분과 거리를 두기 위해 i에 +5, 열 : 1열(맨 왼쪽에 출력)
        
        for (int k = 0; k < 6; k++) { //각 열 반복(왼쪽에서 오른쪽으로)
            r = dish[i][k][0];  //dish의 argb값을 각각 변수에 저장 
            g = dish[i][k][1];
            b = dish[i][k][2];
            a = dish[i][k][3];

            if (a != 0) {   //투명도가 0이 아니라면(도트를 보이게 한다면)
                printf("\033[48;2;%d;%d;%dm  \033[0m", r, g, b);  //도트(공백 2칸) 출력 / [48;2;R;G;B 는 배경색을 0~255 rgb값으로 설정하는 이스케이프 시퀀스
            }
            else        //투명도가 0이라면(도트를 보이지 않게 한다면)
                printf("\033[2C"); //도트 출력X(커서 두칸 이동) https://en.wikipedia.org/wiki/ANSI_escape_code#Control_Sequence_Introducer_commands
        }
    }

    if (cookDone == 0)     //아직 요리가 끝나지 않았다면
        printf("\033[0m\033[4;1f제작중...");
    else {               //요리가 끝났다면
        printf("\033[0m\033[4;1f완성!!!");
    }

    printf("\033[?25h");    //깜빡이는 커서 보기
}


//재료 출력 함수
void printIngredient(int TYPE, int stratLine, char currentIngredientN[20]) {//재료의 유형, 재료를 출력을 시작할 열 위치 값, 현재 출력하려는 재료의 이름을 전달

    char ingredientName[1024] = ""; //텍스트파일의 재료이름을 저장해 현재 출력하려는 재료이름과 비교하기 위한 변수
    
    unsigned int argb = 0, r = 0, g = 0, b = 0, a = 0; //각 재료의 a(투명도 : 보임 or 안보임),rgb값을 저장할 변수
                                                       //텍스트 파일의 16진수는 부호가 없는 정수 자료형의 최대치(10진수로 42억...) 에 가깝게 저장되어 있기 때문에그냥 int형(최대 21억...)을 사용하면 오버플로우가 일어남

    char fileName[10][100] = { "BREAD.txt", "MEAT.txt", "VEGETABLE.txt","VEGETABLE.txt", "SAUCE.txt","SAUCE.txt", "DRINK.txt" }; //각 재료 유형별 텍스트파일 이름 (실행 시 소스코드와 동일한 위치에 있으면 인식할 수 있음, onlineGDB에서도 가능)
    FILE* fp = NULL;    //텍스트 파일을 열기 위한 파일 포인터 선언
    fp = fopen(fileName[TYPE], "rt");   //유형별 재료 텍스트파일 열기

    while (fgets(ingredientName, 1024, fp) != NULL) {   //파일에서 한 줄씩 읽어와 저장('\n'까지 저장됨)

        int len = strlen(currentIngredientN);//정확한 문자열 비교를 위해서 재료이름의 길이를 구함 

        if (!strncmp(ingredientName, currentIngredientN,len)) {//텍스트파일의 현재 줄에 있는 재료이름과 현재 재료이름이 같다면 

            break;  //텍스트 파일 속의 재료이름 다음줄 부터 있는 도트 배열을 읽어오기 위해 while문 종료
        }
    }

    for (int i = 0; i < 6; i++) { //각 행 반복(위에서 아래로)

        printf("\033[%d;%df", i + 5, stratLine);    //출력할 위치 /행 : 상단 점수출력 부분과 거리를 두기 위해 i에 +5, 열 : 전달 받은 시작 열
                                                    
        for (int k = 0; k < 6; k++) { //각 열 반복(왼쪽에서 오른쪽으로)
            
            //텍스트 파일에 한 도트당 8자리 16진수 정수 0x??/??/??/??로 저장됨  https://www.piskelapp.com/ 여기서 도트 그려서 c언어 16진수 배열로 파일 다운해서 텍스트 파일 생성함
            //16진수임을 나타내는 0x 바로 오른쪽 ??이 a(00 or ff : 투명 or 보임), 그다음 ??은 b(0~ff), 다음 ??은 g(0~ff), 마지막 ??은 r(0~ff)값. 16진수 2자리(0~ff)는 10진수로(0~255)임.
            
            fscanf(fp, "%x", &argb);    //16진수 형식으로 읽어오기 위해서 %d 가 아닌 %x / 16진수로 저장한 것을 10진수로 표현하면 숫자는 바뀌지만 수치 자체는 동일하기 때문에 변수를 10진수로 연산 가능 ex) 0xf == 15
            a = argb / (256*256*256); //16진수 2자리(00~ff)는 10진수로 3자리(0~255)로 표현되기 때문에 256으로 나눗셈과 나머지 연산해서 16진수의 각 2자리마다 수치를 뽑아낼 수 있음.
            b = (argb % (256*256*256)) / (256*256);   //a 값은 제외하고 그다음 ??만 저장
            g = (argb % (256 * 256)) / 256;    //a,b값은 제외하고 그다음 ??만 저장
            r = argb % 256;   //a,b,g값 제외하고 마지막 ??만 저장

            if (a != 0) {   // a(투명도) 가 0이(투명) 아니면 
                printf("\033[48;2;%d;%d;%dm  \033[0m", r, g, b);  //도트(공백 2칸) 출력 / [48;2;R;G;B 는 배경색을 0~255단계의 rgb값으로 설정하는 이스케이프 시퀀스 https://en.wikipedia.org/wiki/ANSI_escape_code#24-bit
                if (stratLine == 1) {   //만약 출력위치가 1열이라면(요리출력부분)
                    dish[i][k][0] = r; //요리 과정 배열에 현재 재료의 rgb값을 저장 (재료가 쌓이는 모습을 출력하기 위해서)
                    dish[i][k][1] = g;
                    dish[i][k][2] = b;
                    dish[i][k][3] = a;
                }
            }
            else {   //투명도가 0이면
                printf("\033[2C"); //도트 출력X(커서 두칸 이동) https://en.wikipedia.org/wiki/ANSI_escape_code#Control_Sequence_Introducer_commands
            }
        }
    }
    fclose(fp); //파일 닫기

}

//재료 출력 위치와 방식(해금 단계라면 이미 해금 된 재료는 이름으로, 아닌 재료는 ???와 필요 금액)을 정하고 printIngredient()를 호출하는 함수
void positionIngredient(int TYPE, int number, int* cusor, char currentIngredientN[20], int isPrep) {//재료 유형, 재료 번호, 출력 위치 포인터(값 변경 적용을 위해서), 출력할 재료의 이름 , 해금 단계인지 판단하는 변수(0또는1) 전달

    int stratLine = 5 + (*cusor - isPrep) * 13;        //재료 출력 시작 위치(열)는 요리에서 조금 간격을 주기 위한 기본 5 + ( 현재 출력해야 할 위치 - 0또는1(해금단계는 요리 출력x -> 왼쪽으로 한칸 이동) )
                                                        // * 13 -> ((한 칸에 재료 도트6개(공백 2칸) 이기 때문) 12 + 1 (재료 사이 간격을 위해서)) 
    
    if (currentIngredientN[0] == '\0') {    //만약 출력하려는 재료가 현재 해금되지 않았다면
        if (isPrep == 1) {                                  //재료 해금 단계라면 ???와 해금을 위한 필요 금액 출력
            printf("\033[4;%df%d%s\033[0m", stratLine, number, ".???"); //출력 시작 위치에 재료번호.??? 출력
            printf("\033[5;%dfㄴ%d원\033[0m", stratLine, UNLOCK_GOLD * number);  //그 밑의 줄에 해금을 위한 금액(해금기본금액*재료번호) 출력
        }
        else {                                           //만약 재료가 해금단계가 아니라면(해금 되어 있지도 않다면)                                         
            return;                                     //아무것도 출력하지 않고 다음 번호의 재료로
        }
    }
    else {                                               //재료가 해금되어 있다면
        printf("\033[5;%df            \033[0m", stratLine);    //재료 해금단계에서 해금한 직후일 수도 있으므로 화면에 표기되어 있을 금액 부분을 지워주기 위해 출력

        printIngredient(TYPE, stratLine, currentIngredientN);   // 재료 출력 (출력할 재료 유형, 출력 시작 위치(열), 출력할 재료 이름)

        printf("\033[0m\033[4;%df%d.%s", stratLine, number, currentIngredientN);    //출력 시작 위치에 "재료번호.재료이름"을 출력
    }
    *cusor += 1; //재료를 출력 했기 때문에 다음 위치으로       

}

//유형별 재료를 순서대로 화면에 positionIngredient()하는 함수
void displayIngredient(int TYPE, const char totalIngredient[MAX_IN][20], char currentIngredient[MAX_IN][20], int prep) {//재료 유형, 유형별 모든 재료 목록, 현재 해금한 재료 목록, 해금 단계인지 판단하는 변수(0또는1) 전달

    printf("\033[?25l");    //깜빡이는 커서 숨기기(재료 출력 도중 움직이는 커서가 거슬려서)
    int cusor = 1;          //재료의 출력 위치를 구하기 위한 변수

    for (int number = 1; number <= MAX_IN; number++) {   //재료 최대치 만큼 반복
        if (totalIngredient[number - 1][0] != '\0') {  //해당 번호의 재료가 모든 재료 목록에 있다면( 해금단계에서 해금할 재료가 없음에도 불구하고 ??? 출력하는 경우를 방지)
            positionIngredient(TYPE, number, &cusor, currentIngredient[number - 1], prep);   //출력 방식과 위치를 정해 재료를 화면에 출력, [number - 1] 로 현재 출력하려는 재료 이름만 전달 (재료 번호는 1~, 인덱스는 0~)
        }
    }
    printf("\033[4;%df6.선택 안함", 5 + (cusor - prep) * 13);  //모든 재료 출력 후 마지막은 "6.선택 안함" 선택지 출력(요리와 간격을 위한 기본 5 + 현재 칸 - 0또는1(해금단계에서는 요리 출력x -> 왼쪽으로 한칸 이동))
                                                                        // * 13 -> ((한 칸에 재료 도트6개(공백 2칸) 이기 때문) 12 + 1 (재료 사이 간격을 위해서) 

    printf("\033[?25h");    //깜빡이는 커서 보기

}


//해금 단계에 유형별로 재료를 해금하는 함수
void addIngredient(int TYPE, const char totalIngredient[MAX_IN][20], char currentIngredient[MAX_IN][20], int* currentGold, int* unlockCount) {
                    //재료 유형, 유형별 모든 재료 목록, 현재 해금한 재료 목록, 현재 보유 금액 포인터(변경값 적용을 위해서), 현재 해금한 재료 수 포인터(변경값 적용을 위해서) 전달

    char unlock = '0';   //해금할 재료 번호가 저장되는 변수(플레이어의 입력 값이 저장 됨)
    int unlockCost = 0; //해금에 필요한 금액(밑에서 입력받게되는 재료 번호 * 해금기본금액으로 계산)

    clearDisplay();  //화면 전체 지우기    

    while (1) {  //해금은 여러번 가능해야 하기 때문에 "6.선택안함"을 통해 함수를 종료하기 전까지 무한반복

        displayIngredient(TYPE, totalIngredient, currentIngredient, 1);//재료 유형, 모든 재료 목록, 현재 해금 목록, (1 = 현재 해금단계임)을 전달해 
                                                                       //해금 할 수 있거나 해금 된 재료를 화면에 출력(해금할 때마다 화면 갱신을 위해 while문 안쪽에 있음)

        printf("\033[2;1f\033[40m%d단계 >> 점수 : %d/%d  돈 : %d  \033[0m", currentStage, score,CLEAR_SCORE, *currentGold);   //현재 스테이지,점수,돈을 상단에 출력
        printf("\033[13;3f\033[2K해금할 %s의 번호 한 자리 입력[ex) 12 입력시 1로 판단] : ", ingredientType[TYPE]);  //재료 유형이름 ex)빵,채소... 안내 문구 하단부에 출력

        scanf(" %c", &unlock);   //플레이어 해금할 재료 번호 입력(정수형으로 입력 받으면 문자 입력시 버그가 발생하면서 코드 진행이 이상해짐)
        while (getchar() != '\n');//입력 버퍼 비우기(버그 최소화) ex) 위 scanf에 1111\n 입력하면 111\n이 남아 다음 선택에 입력됨 따라서 \n이 나올 때 까지 입력 받아 지워버림 

        unlock -= ('0' + 1); //char은 ASCII 정수로 저장('0'은 48 ~ '9'는 57)되기 때문에 정수 연산 가능
                       //'0'을 빼서 1~6의 수치로 변경 , 배열에 0부터 접근하기 위해서 추가로 1 더 빼줌
        
        unlockCost = (unlock + 1) * UNLOCK_GOLD; //해금에 필요한 금액 = 입력한 재료 번호(0~5 +1)  * 해금기본금액 

        if (unlock == 5) {    //6(선택안함) 을 입력했다면 
            return; //함수 종료
        }
        else if (totalIngredient[unlock][0] == '\0') {  //번호에 해당하는 재료가 전체 재료 목록에도 없다면(음료는 2개밖에 없어서 3~5번이 비어있음) 
            continue;   //번호 다시 입력 받기
        }
        else if (currentIngredient[unlock][0] == '\0') { //선택한 번호의 재료가 현재 해금 되어 있지 않다면
            
            if (*currentGold >= unlockCost) {   //보유한 금액이 해금에 필요한 금액보다 많거나 같다면   
                
                strcpy(currentIngredient[unlock], totalIngredient[unlock]); //해금 목록에 재료를 추가
                *currentGold -= unlockCost; //해금 금액 소모
                *unlockCount += 1;  //해금 재료 수 +1 (해피 엔딩시 모든 재료를 해금 했는지 확인하기 위해서)
            }
            else if (*currentGold < unlockCost) {    //보유한 금액이 해금에 필요한 금액보다 적다면
                
                printf("\033[13;3f\033[2K돈이 부족합니다.");    //돈 부족 안내 출력
                fflush(stdout); //printf 내용이 버퍼에 남아 화면에 안보이는 경우가 있기 때문에 출력버퍼를 비워줌
                sleep(1);   //안내 문구를 화면에 표시하기 위한 1초 대기
            }
        }
               //재료 하나를 해금 완료했거나, 선택한 재료가 이미 해금 되었거나,돈이 부족한 경우 
               //변수 초기화 하고 반복 처음부터 다시
    }
}

//요리 진행 시 재료를 선택하는 함수
void chooseIngredient(int TYPE, const char totalIngredient[MAX_IN][20], char currentIngredient[MAX_IN][20], int* currentGold) {//재료 유형, 유형별 모든 재료 목록, 현재 해금한 재료 목록, 보유 금액 포인터(변경값 적용을 위해서) 전달

    int costList[MAX_IN] = { COST_BREAD, COST_MEAT, COST_VEGETABLE, COST_VEGETABLE, COST_SAUCE, COST_SAUCE, COST_DRINK };   //각 재료 사용 시 소모되는 비용 = costList[재료 유형]*재료번호

    int useGold = 0;    //재료 사용시 소모되는 금액 초기화                         

    if (currentIngredient[Need.list[TYPE]][0] == '\0') {    //손님 랜덤 재료가 현재 해금한 재료 목록에 없다면(다른 함수에서도 손님 주문 텍스트로 출력 되지도 않고 점수 계산에서도 빠짐) 
        return;                                             //함수 종료
    }

    clearDisplay();  //화면 전체 지우기
    printf("\033[2;1f\033[40m%d단계 >> 점수 : %d/%d  돈 : %d  \033[0m", currentStage, score,CLEAR_SCORE, *currentGold);   //현재 스테이지,점수,돈을 상단에 출력

    printCooking(0); //현재까지 진행된 요리과정 출력 0:요리중 1:요리완성

    displayIngredient(TYPE, totalIngredient, currentIngredient, 0);   //재료 유형, 모든 재료 목록, 현재 해금 목록, (0=현재 해금단계X)를 전달해 
                                                                      //선택할 재료를 화면에 출력(해금과 달리 한 번 선택하면 다음 함수로 넘어기기 때문에 while문 밖에 있음)

    while (1) {  //플레이어가 1~5 사이 값을 입력해 재료를 선택했거나 6을 선택해 함수를 종료할 때까지 무한반복

        printf("\033[13;3f\033[2K사용할 %s 번호 한 자리 입력[ex) 12 입력시 1로 판단] : ", ingredientType[TYPE]);    //재료 유형이름 ex)빵,채소... 안내 문구 하단부에 출력

        scanf(" %c", &Choose.list[TYPE]); //선택할 재료 번호 입력(정수형으로 입력 받으면 문자 입력시 버그가 발생하면서 코드 진행이 이상해짐)
        while (getchar() != '\n');    //입력 버퍼 비우기(버그 최소화) ex) 위 scanf에 1111\n 입력하면 111\n이 남아 다음 선택에 입력됨 따라서 \n이 나올 때 까지 입력 받아 지워버림 

        Choose.list[TYPE] -= ('0'+ 1);//char은 ASCII 정수로 저장('0'은 48 ~ '9'는 57)되기 때문에 정수 연산 가능
                                 //'0'을 빼서 1~6의 수치로 변경, 재료 배열에 접근할 때 재료 이름은 0부터 저장되기 때문에 추가로 1 더빼줌
        
        useGold = costList[TYPE] * (Choose.list[TYPE] + 1); //각 재료 사용 시 소모되는 비용 = costList[재료 유형] * ( 선택한 재료번호(0~5) + 1 )

        if (Choose.list[TYPE] == 5) { //6(선택안함) 을 입력했다면 
            return; //함수 종료
        }
        else if (totalIngredient[Choose.list[TYPE]][0] == '\0') { //번호에 해당하는 재료가 전체 재료 목록에도 없다면(음료는 2개밖에 없어서 3~5번이 비어있음) 
            continue; //번호 다시 입력 받기
        }
        else if (currentIngredient[Choose.list[TYPE]][0] != '\0') { //선택한 재료가 현재 해금되어 있다면
                
            if (*currentGold >= useGold) {  //보유한 금액이 재료 사용에 필요한 금액보다 많거나 같다면
                
                *currentGold -= useGold;    //재료 사용에 필요한 금액 소모
                printIngredient(TYPE, 1, currentIngredient[Choose.list[TYPE]]);  //재료를 요리과정 부분에 출력
                return;    //함수 종료
            }
            else if (*currentGold < useGold) {    //보유한 금액이 재료 사용에 필요한 금액보다 적다면
                
                printf("\033[13;3f\033[2K돈이 부족합니다.");    //돈 부족 안내 문구 출력
                fflush(stdout); //printf 내용이 버퍼에 남아 화면에 안보이는 경우가 있기 때문에 출력버퍼를 비워줌
                sleep(1);   //안내 문구 출력을 위한 1초 대기
            }
        }
        //선택한 재료가 (전체 또는 현재) 재료 목록에 없음,돈이 부족인 경우
        //반복 처음부터 다시
    }
}
