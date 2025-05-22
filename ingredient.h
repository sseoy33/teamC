
//----------------ingredient.h------------------

#define MAX_IN 10 //�� ������ ����� �ִ�ġ, ����� ä�ҿ� �ҽ��� 5���� �ִ������� ���Ƿ� 10���� ����
#define NEED_MAX 7  // �մ��� �ֹ��ϴ� ��� �������� �ִ�(��1,���1,ä��1~2,�ҽ�1~2,����1�� �ִ� 7�� �ֹ���)

//��Ȱ�� �ڵ� ����, ������ ���� �� ����� ���� ���� ��ũ��
#define BREAD 0
#define MEAT 1
#define VEGETABLE1 2
#define VEGETABLE2 3
#define SAUCE1 4
#define SAUCE2 5
#define DRINK 6

//���� �뷱�� ������ ���� �������� �ʱⰪ ���� ��ũ��
#define TIME_LIMIT 120 //���ѽð� = timeLimit(��) * ���� ��������
#define DEFAULT_CUSTOMER 3  //�������� �� �մ� �� = �⺻ �մ� �� * ���� ��������
#define INCREASE_SCORE 10   //�丮 �� ������ ȹ���ϴ� ������ increaseScore * ������5�ܰ�
#define INITIAL_GOLD 1000 //�ʱ� �ݾ� 
#define DISH_DEFAULT_GOLD 500 //������ġ �⺻�ݾ� = ����ȣ * dishDefaultGold
#define ADDITIONAL_GOLD 1000    //�߰� ȹ�� �ݾ� = increaseGold * ������5�ܰ�
#define UNLOCK_GOLD 1500    //��� �رݿ� �ʿ��� �ݾ� = unlockGold * ����ȣ 
#define UNLOCK_MAX 10 //�� �ر��� ��� �ִ�ġ(���� �� ��� ��� �ر��ߴ��� Ȯ�� ����)
#define CLEAR_SCORE 800 // ������ ���� ���� �ּ� ����

#define COST_BREAD 10 //�丮 �� �� ��� �� �Ҹ�Ǵ� ���
#define COST_MEAT 10 //�丮 �� ��� ��� �� �Ҹ�Ǵ� ��� 
#define COST_VEGETABLE 10 //�丮 �� ä�� ��� �� �Ҹ�Ǵ� ���
#define COST_SAUCE 10 //�丮 �� �ҽ� ��� �� �Ҹ�Ǵ� ���
#define COST_DRINK 10 //�丮 �� ���� ��� �� �Ҹ�Ǵ� ���

//��� ���� (ä�ҿ� �ҽ��� �� �� ��� �� �� �� ����)  
const char ingredientType[MAX_IN][20] = { "��","���","ä��","ä��","�ҽ�","�ҽ�","����" };

//���� �� ��� ���
const char totalBread[MAX_IN][20] = { "��Ʈ","ȭ��Ʈ" };
const char totalMeat[MAX_IN][20] = { "��","ġŲ","����" };
const char totalVegetable[MAX_IN][20] = { "�����","�丶��","����","����","��Ŭ" };
const char totalSauce[MAX_IN][20] = { "�ұ�","����","�ӽ�Ÿ��","����Ʈĥ��","��ġ" };
const char totalDrink[MAX_IN][20] = { "�ݶ�","���̴�" };

const char customerList[6][20] = { "�մ�a", "�մ�b", "�մ�c", "�մ�d", "�մ�e", "�մ�f" }; //�մ� ����
const char customerFeeling[5][20] = { "Bad","Normal","Good","Great","Excellent" }; //������ ����

//���� ������ ���� ������
int currentStage = 1; //���� ��������
int score = 0; //�� ���� ����
char currentBread[MAX_IN][20] = { "��Ʈ","ȭ��Ʈ","\0" };    //���� �رݵǾ� �ִ� ���� �� ��� ���
char currentMeat[MAX_IN][20] = { "��","ġŲ","����","\0" };  // "\0"��� �迭�� ������ �κ��� �ι��ڷ� �ʱ�ȭ������ �׳� ��
char currentVegetable[MAX_IN][20] = { "�����","\0" };
char currentSauce[MAX_IN][20] = { "�ұ�","\0" };
char currentDrink[MAX_IN][20] = { "\0" };
struct State {  //�մ��� ������ ���� ��ᳪ �÷��̾ ������ ��Ḧ �����ϴ� ����ü
    char list[NEED_MAX];    //��� ��ȣ�� ����
    char ingredient[MAX_IN][20];    //��� �̸��� ���� 
    int total;  //�մ��� �� ��� ��Ḧ �����ߴ��� �Ǵ� �÷��̾ ��� ��Ḧ ������� ����
};
//����ü ���� ����
struct State Need;
struct State Choose;
//State ����ü �ʱ�ȭ �Լ�
void initialState(struct State* state) { //������ �ּ� ���� �޾ƿ� �Լ� �ܺ��� ���� ���� ���� �����ϰ� �� 

    for (int i = 0; i < NEED_MAX; i++) {    //��� ��ȣ�� ��� ��Ҹ� 10�� �ʱ�ȭ
        state->list[i] = -1;            //���� ���� �ִ� 5������ ����
    }                                   //������ �ƹ��͵� ������� ���� 10������ �ʱ�ȭ
    for (int i = 0; i < MAX_IN; i++) {
        strcpy(state->ingredient[i], "\0");  //��� �̸��� ��� ��Ҹ� �ι��ڷ� �ʱ�ȭ
    }
    state->total = 0;   //total 0���� �ʱ�ȭ
}
// �丮 ������ ǥ���� 3���� �迭
int dish[6][6][4] = { 0 };
//�丮 �迭 �ʱ�ȭ �Լ�
void initialDish() {
    for (int i = 0; i < 6; i++) { //�� �� �ݺ�(������ �Ʒ���)
        for (int k = 0; k < 6; k++) { //�� �� �ݺ�(���ʿ��� ����������)
            dish[i][k][0] = 0; //�丮 ���� �迭 �ʱ�ȭ
            dish[i][k][1] = 0;
            dish[i][k][2] = 0;
            dish[i][k][3] = 0;
        }
    }
}


void clearDisplay() {   //ȭ���� ����� �Լ�
    printf("\033[1;1f\033[?25l");
    for (int i = 0; i < 20; i++) {
        printf("\033[0m                                                                                                    \n");
    }
    printf("\033[1;1f\033[25h");
}

//"ingredient.h" �Լ� ���
void displayIngredient(int TYPE, const char totalIngredient[MAX_IN][20], char currentIngredient[MAX_IN][20], int prep);//��Ḧ ������� ȭ�鿡 ����ϴ� �Լ�
void positionIngredient(int TYPE, int number, int* cusor, char currentIngredientN[20], int isPrep);//��� ��� ��ġ�� ����� ���ϴ� �Լ�
void printIngredient(int TYPE, int stratLine, char currentIngredientN[20]);//��� ��� �Լ� 
void chooseIngredient(int TYPE, const char totalIngredient[MAX_IN][20], char currentIngredient[MAX_IN][20], int* currentGold);//�丮 ���� �� ��Ḧ �����ϴ� �Լ�
int printCooking(int cookDone); //�丮 ���� ��� �Լ� 
void addIngredient(int TYPE, const char totalIngredient[MAX_IN][20], char currentIngredient[MAX_IN][20], int* currentGold, int* unlockCount);//�ر� �ܰ迡�� ��Ḧ �ر��ϴ� �Լ�



//�丮 ���� ��� �Լ�
int printCooking(int cookDone) { //�丮 ��(0) ���� �丮 �ϼ�(1) ���� �Ǵ��ϱ� ���� ����

    printf("\033[?25l");    //�����̴� Ŀ�� �����(��� ��� ���� �����̴� Ŀ���� �Ž�����)

    int r = 0, g = 0, b = 0, a = 0; //�� ����� rgb���� ������ ����

    for (int i = 0; i < 6; i++) { //�� �� �ݺ�(������ �Ʒ���)

        printf("\033[%d;%df", i + 5, 1);    //����� ��ġ /�� : ��� ������� �κа� �Ÿ��� �α� ���� i�� +5,
        //�� : ���� ���� ���� ��
        for (int k = 0; k < 6; k++) { //�� �� �ݺ�(���ʿ��� ����������)
            r = dish[i][k][0];  //rgb, a(����:���� or ����)��
            g = dish[i][k][1];
            b = dish[i][k][2];
            a = dish[i][k][3];

            if (a != 0) {   //������ 0�� �ƴ϶��(��Ʈ�� ���δٸ�)
                printf("\033[48;2;%d;%d;%dm  \033[0m", r, g, b);  //��Ʈ(���� 2ĭ) ��� / [48;2;R;G;B �� ������ 0~255 rgb������ �����ϴ� �̽������� ������
            }
            else
                printf("\033[2C"); //��Ʈ ���X(Ŀ�� ��ĭ �̵�)
        }
    }

    if (cookDone == 0)     //���� �丮�� ������ �ʾҴٸ�
        printf("\033[0m\033[4;1f������...");
    else {               //�丮�� �����ٸ�
        printf("\033[0m\033[4;1f�ϼ�!!!");
    }

    printf("\033[?25h");    //�����̴� Ŀ�� ����
}


//��� ��� �Լ�
void printIngredient(int TYPE, int stratLine, char currentIngredientN[20]) {//����� ����, ��Ḧ ����� ������ �� ��ġ ��, ����� �̸��� ����

    char ingredientName[1024] = ""; //�ؽ�Ʈ������ ����̸��� ������ ���� ����̸��� ���ϱ� ���� ����
    int argb = 0, r = 0, g = 0, b = 0, a = 0; //�� ����� a(���� : ���� or �Ⱥ���),rgb���� ������ ����

    char fileName[10][100] = { "BREAD.txt", "MEAT.txt", "VEGETABLE.txt","VEGETABLE.txt", "SAUCE.txt","SAUCE.txt", "DRINK.txt" }; //�� ��� ������ �ؽ�Ʈ���� �̸�
    FILE* fp = NULL;    //���� ������ ����
    fp = fopen(fileName[TYPE], "rt");   //������ ��� �ؽ�Ʈ���� ����

    while (fgets(ingredientName, 1024, fp) != NULL) {   //���Ͽ��� �� �پ� �о�� ����('\n'���� �����)

        int len = strlen(ingredientName);//������ '\n'���๮�� ���Ÿ� ���� ���ڿ� ���� 

        deleteEnter(); //���� ��� �̸��� �񱳸� ���ؼ� ���ʿ��� '\n'���๮�� ����

        if (!strcmp(ingredientName, currentIngredientN)) {//�ؽ�Ʈ������ ���� �ٿ� �ִ� ����̸��� ���� ����̸��� ���ٸ� 

            break;  //�� ������ ���� �ִ� ��Ʈ �迭�� �о���� ���� �ݺ��� ����
        }
    }

    for (int i = 0; i < 6; i++) { //�� �� �ݺ�(������ �Ʒ���)

        printf("\033[%d;%df", i + 5, stratLine);    //����� ��ġ /�� : ��� ������� �κа� �Ÿ��� �α� ���� i�� +5,
        //�� : ���� ���� ���� ��
        for (int k = 0; k < 6; k++) { //�� �� �ݺ�(���ʿ��� ����������)
            fscanf(fp, "%d", &argb);    //�� ��Ʈ�� 10�ڸ��� ���� 0,000,000,000�� ����� �Ǿ��ڸ� 0�� a(0 or 1 : ���� or ����), �״��� 000�� b(0~255), ���� 000�� g(0~255), ������ 000�� r(0~255)��
            a = argb / 1000000000; //������ �� ���ڸ� �� ����
            b = (argb % 1000000000) / 1000000;   //a ���� �����ϰ� �״��� 000�� ����
            g = (argb % 1000000) / 1000;    //a,b���� �����ϰ� �״��� 000�� ����
            r = argb % 1000;   //a,b,g�� �����ϰ� ������ 000�� ����

            if (a != 0) {   // a(����) �� 0��(����) �ƴϸ� 
                printf("\033[48;2;%d;%d;%dm  \033[0m", r, g, b);  //��Ʈ(���� 2ĭ) ��� / [48;2;R;G;B �� ������ 0~255 rgb������ �����ϴ� �̽������� ������
                if (stratLine == 1) {   //���� �����ġ�� 1���̶��(�丮��ºκ�)
                    dish[i][k][0] = r; //�丮 ���� �迭�� ���� ����� rgb���� ����
                    dish[i][k][1] = g;
                    dish[i][k][2] = b;
                    dish[i][k][3] = a;
                }
            }
            else {   //������ 0�̸�
                printf("\033[2C"); //��Ʈ ���X(Ŀ�� ��ĭ �̵�)
            }
        }
    }
    fclose(fp); //���� �ݱ�

}

//��� ��� ��ġ�� ���(�ر� �ܰ��� �̹� �ر� �� ���� �̸�����, �ƴ� ���� ???�� �ʿ� �ݾ�)�� ���ϰ� printIngredient�� ȣ���ϴ� �Լ�
void positionIngredient(int TYPE, int number, int* cusor, char currentIngredientN[20], int isPrep) {//��� ����, ��� ��ȣ, ��� ��ġ, ����� ����� �̸� , �ر� �ܰ����� �Ǵ��ϴ� ����(0�Ǵ�1) ����

    int stratLine = 5 + (*cusor - isPrep) * 13;        //��� ��� ���� ��ġ(��)�� �丮���� ���� ������ �ֱ� ���� �⺻ 5 + ���� Ŀ��(��Ḧ �� ĭ ��� �ߴ���) - 0�Ǵ�1(�رݴܰ�� �丮 ���x �������� ��ĭ �̵�)
    // * 13 -> ((�� ĭ�� ��� ��Ʈ6��(���� 2ĭ) �̱� ����) 12 + 1 (��� ���� ������ ���ؼ�)) 
    if (currentIngredientN[0] == '\0') {    //���� ����Ϸ��� ��ᰡ ���� �رݵ��� �ʾҴٸ�
        if (isPrep == 1) {                                  //��� �ر� �ܰ��� ???�� �ر��� ���� �ʿ� �ݾ� ���
            printf("\033[4;%df%d%s\033[0m", stratLine, number, ".???"); //��� ���� ��ġ�� ����ȣ.??? ���
            printf("\033[5;%df��%d��\033[0m", stratLine, UNLOCK_GOLD * number);  //�� ���� �ٿ� �ر��� ���� �ݾ�(�رݱ⺻�ݾ�*����ȣ) ���
        }
        else {                                           //���� ��ᰡ �رݴܰ谡 �ƴ϶��(�ر� �Ǿ� ������ �ʴٸ�)                                         
            return;                                     //�ƹ��͵� ������� �ʰ� ���� ��ȣ�� ����
        }
    }
    else {                                               //��ᰡ �رݵǾ� �ִٸ�
        printf("\033[5;%df            \033[0m", stratLine);    //��� �رݴܰ迡�� �ر� ���� ���� �����Ƿ� ȭ�鿡 ǥ��Ǿ� ���� �ݾ� �κ��� �����ֱ� ���� ���

        printIngredient(TYPE, stratLine, currentIngredientN);   // ��� ��� 

        printf("\033[0m\033[4;%df%d.%s", stratLine, number, currentIngredientN);    //���� ��ġ�� ����ȣ.����̸��� ���,�ε����� 0���� �����̱� ������ (����ȣ(1���� ����) - 1)�� �ش��ϴ� ����̸�
    }
    *cusor += 1; //��Ḧ ��� �߱� ������ ���� ��ġ����       
    return;     //���� ��ȣ�� ����

}

//������ ��Ḧ ������� ȭ�鿡 ����ϴ� �Լ�
//��� ����, ������ ��� ��� ���, ���� �ر��� ��� ���, �ر� �ܰ����� �Ǵ��ϴ� ����(0�Ǵ�1) ����
void displayIngredient(int TYPE, const char totalIngredient[MAX_IN][20], char currentIngredient[MAX_IN][20], int prep) {

    printf("\033[?25l");    //�����̴� Ŀ�� �����(��� ��� ���� �����̴� Ŀ���� �Ž�����)
    int cusor = 1;          //����� ��� ��ġ�� ���ϱ� ���� ����

    for (int number = 1; number <= MAX_IN; number++) {   //��� �ִ�ġ ��ŭ �ݺ�
        if (totalIngredient[number - 1][0] != '\0') {  //�ش� ��ȣ�� ��ᰡ ��� ��� ��Ͽ� �ִٸ�( �������� �ұ��ϰ� ??? ����ϴ� ��츦 ����)
            positionIngredient(TYPE, number, &cusor, currentIngredient[number - 1], prep);   //��� ��İ� ��ġ�� ���� ��Ḧ ȭ�鿡 ���, [number - 1] �� ���� ����Ϸ��� ��� �̸��� ����
        }
    }
    printf("\033[4;%df6.���� ����", 5 + (cusor - prep) * 13);  //��� ��� ��� �� �������� "6.���� ����" ������ ���(�丮�� ������ ���� �⺻ 5 + ���� ĭ - 0�Ǵ�1(�رݴܰ�-�丮 ���x �޿������� ��ĭ �̵�))
    // * 13 -> ((�� ĭ�� ��� ��Ʈ6��(���� 2ĭ) �̱� ����) 12 + 1 (��� ���� ������ ���ؼ�) 

    printf("\033[?25h");    //�����̴� Ŀ�� ����

}


//�ر� �ܰ迡 �������� ��Ḧ �ر��ϴ� �Լ�
//��� ����, ������ ��� ��� ���, ���� �ر��� ��� ���, ���� ���� �ݾ� ������(���氪 ������ ���ؼ�) ����
void addIngredient(int TYPE, const char totalIngredient[MAX_IN][20], char currentIngredient[MAX_IN][20], int* currentGold, int* unlockCount) {

    char unlock = '0';   //�ر��� ��� ��ȣ�� ����Ǵ� ����(�÷��̾��� �Է� ���� ���� ��)
    int unlockCost = 0; //�رݿ� �ʿ��� �ݾ�(�ؿ��� �Է��� ��� ��ȣ * �رݱ⺻�ݾ����� ���)

    clearDisplay();  //ȭ�� ��ü �����    

    while (unlock < '1' || '6' < unlock) {  //�÷��̾� �Է� ���� 1~6�̿��� ��(��� ���� ���� ������ ä�ҿ� �ҽ����� 5��, 6.���� ���� ��������)

        displayIngredient(TYPE, totalIngredient, currentIngredient, 1);//��� ����, ��� ��� ���, ���� �ر� ���, (1=���� �رݴܰ�O)�� ������ 
        //�ر� �� �� �ְų� �ر� �� ��Ḧ ȭ�鿡 ���(�ر��� ������ ȭ�� ������ ���� while�� ���ʿ� ����)

        printf("\033[?25l"); //�����̴� Ŀ�� �����(��� ���� �����̴� Ŀ���� �Ž�����)
        printf("\033[2;1f\033[40m%d�ܰ� >> ���� : %d  �� : %d  \033[0m", currentStage, score, *currentGold);   //���� ��������,����,���� ��ܿ� ���
        printf("\033[13;3f\033[2K�ر��� %s�� ��ȣ �� �ڸ� �Է�[ex) 12 �Է½� 1�� �Ǵ�] : ", ingredientType[TYPE]);  //��� �����̸� ex)��,ä��... �ȳ� ���� �ϴܺο� ���

        printf("\033[?25h"); //�����̴� Ŀ�� ����(�Է� ���� ���� Ŀ���� ���̴� ���� ��������)

        scanf(" %c", &unlock);   //�÷��̾� �ر��� ��� ��ȣ �Է�(���������� �Է� ������ ���� �Է½� ���װ� �߻��ϸ鼭 �ڵ� ������ �̻�����)
        while (getchar() != '\n');//�Է� ���� ����(���� �ּ�ȭ) ������ 1111\n �Է��ϸ� 111\n�� ���� ���� ���ÿ� �Էµ� ���� \n�� ���� �� ���� �Է� �޾� �������� 

        unlock -= ('0' + 1); //char�� ASCII ������ ����('0'�� 48 ~ '9'�� 57)�Ǳ� ������ ���� ���� ����
        //'0'�� ���� 1~6�� ��ġ�� ����, �迭 �ε����� 0���� �����̱� ������ �߰��� 1 �� ����
        unlockCost = (unlock + 1) * UNLOCK_GOLD; //�رݿ� �ʿ��� �ݾ� = ( �Է��� ��� ��ȣ(0 ~ 5) + 1 )  * �رݱ⺻�ݾ� 

        if (unlock == 5) {    //6(���þ���) �� �Է��ߴٸ� �Լ� ����
            return;
        }
        else if (totalIngredient[unlock][0] == '\0') {  //��ȣ�� �ش��ϴ� ��ᰡ ��ü ��� ��Ͽ��� ���ٸ�(����� 2���ۿ� ��� 3~5���� �������) ��ȣ �ٽ� �Է� �ޱ�
            continue;
        }
        else if (*currentGold >= unlockCost) {   //������ �ݾ��� �رݿ� �ʿ��� �ݾ׺��� ���ų� ���ٸ�

            if (currentIngredient[unlock][0] == '\0') { //���� �ر� �Ǿ� ���� �ʴٸ�
                strcpy(currentIngredient[unlock], totalIngredient[unlock]); //�ر� ��Ͽ� ��Ḧ �߰�
                *currentGold -= unlockCost; //�ر� �ݾ� �Ҹ�
                *unlockCount += 1;
            }
        }
        else if (*currentGold < unlockCost) {    //������ �ݾ��� �رݿ� �ʿ��� �ݾ׺��� ���ٸ�
            printf("\033[13;3f\033[2K���� �����մϴ�.");    //�� ���� �ȳ� ���
            unlock = '0';    //�Է°� �ʱ�ȭ
            fflush(stdout); //printf ������ ���ۿ� ���� ȭ�鿡 �Ⱥ��̴� ��찡 �ֱ� ������ ��¹��۸� �����
            sleep(1);   //�ȳ� ������ ȭ�鿡 ǥ���ϱ� ���� 1�� ���

        }           //�Է°��� ���� ��, �ر��Ϸ��� ��ᰡ ��ü ��� ��Ͽ� ����,��� �ϳ� �ر� �Ϸ�, ���� ��� �̹� �ر� ��,���� ������ ��� 
                    //�ݺ� ó������ �ٽ�
    }
}

//�丮 ���� �� ��Ḧ �����ϴ� �Լ�
//��� ����, ������ ��� ��� ���, ���� �ر��� ��� ���, ���� �ݾ� ������(���氪 ������ ���ؼ�) ����
void chooseIngredient(int TYPE, const char totalIngredient[MAX_IN][20], char currentIngredient[MAX_IN][20], int* currentGold) {

    int costList[MAX_IN] = { COST_BREAD, COST_MEAT, COST_VEGETABLE, COST_VEGETABLE, COST_SAUCE, COST_SAUCE, COST_DRINK };   //�� ��� ��� �� �Ҹ�Ǵ� ��� = costList[��� ����]*����ȣ

    int useGold = 0;    //��� ���� �Ҹ�Ǵ� �ݾ� �ʱ�ȭ                         

    if (currentIngredient[Need.list[TYPE]][0] == '\0') {    //�մ� ���� ��ᰡ ���� �ر��� ��� ��Ͽ� ���ٸ�(�մ� �ֹ� �ؽ�Ʈ�� ��� ������ �ʰ� ���� ��꿡���� ����) 
        return;                                             //�Լ� ����
    }

    clearDisplay();  //ȭ�� ��ü �����
    printf("\033[?25l");    //�����̴� Ŀ�� �����
    printf("\033[2;1f\033[40m%d�ܰ� >> ���� : %d  �� : %d  \033[0m", currentStage, score, *currentGold);   //���� ��������,����,���� ��ܿ� ���
    printf("\033[?25h"); //�����̴� Ŀ�� ����

    printCooking(0); //������� ����� �丮���� ��� 0:�丮�� 1:�丮�ϼ�

    displayIngredient(TYPE, totalIngredient, currentIngredient, 0);   //��� ����, ��� ��� ���, ���� �ر� ���, (0=���� �رݴܰ�X)�� ������ 
    //������ ��Ḧ ȭ�鿡 ���(�رݰ� �޸� �� �� �����ϸ� ���� �Լ��� �Ѿ��� ������ while�� �ۿ� ����)

    while (Choose.list[TYPE] < '1' || '6' < Choose.list[TYPE]) {  //�÷��̾� �Է� ���� 1~6�̿��� ��(��� ���� ���� ������ ä�ҿ� �ҽ����� 5��, 6.���� ���� ��������)

        printf("\033[13;3f\033[2K����� %s ��ȣ �� �ڸ� �Է�[ex) 12 �Է½� 1�� �Ǵ�] : ", ingredientType[TYPE]);    //��� �����̸� ex)��,ä��... �ȳ� ���� �ϴܺο� ���

        scanf(" %c", &Choose.list[TYPE]); //������ ��� ��ȣ �Է�(���������� �Է� ������ ���� �Է½� ���װ� �߻��ϸ鼭 �ڵ� ������ �̻�����)
        while (getchar() != '\n');    //�Է� ���� ����(���� �ּ�ȭ) ������ 1111\n �Է��ϸ� 111\n�� ���� ���� ���ÿ� �Էµ� ���� \n�� ���� �� ���� �Է� �޾� �������� 

        Choose.list[TYPE] -= ('0' + 1);//char�� ASCII ������ ����('0'�� 48 ~ '9'�� 57)�Ǳ� ������ ���� ���� ����
        //'0'�� ���� 1~6�� ��ġ�� ����, �迭 �ε����� 0���� �����̱� ������ �߰��� 1 �� ����
        useGold = costList[TYPE] * (Choose.list[TYPE] + 1); //�� ��� ��� �� �Ҹ�Ǵ� ��� = costList[��� ����] * ( ������ ����ȣ(0~5) + 1 )

        if (Choose.list[TYPE] == 5) { //6(���þ���) �� �Է��ߴٸ� �Լ� ����
            return;
        }
        else if (totalIngredient[Choose.list[TYPE]][0] == '\0') { //��ȣ�� �ش��ϴ� ��ᰡ ��ü ��� ��Ͽ��� ���ٸ�(����� 2���ۿ� ��� 3~5���� �������) ��ȣ �ٽ� �Է� �ޱ�
            continue;
        }
        else if (*currentGold >= useGold) {  //������ �ݾ��� ��� ��뿡 �ʿ��� �ݾ׺��� ���ų� ���ٸ�
            if (currentIngredient[Choose.list[TYPE]][0] != '\0') { //���� �رݵǾ� �ִٸ�
                *currentGold -= useGold;    //��� ��뿡 �ʿ��� �ݾ� �Ҹ�
                printIngredient(TYPE, 1, currentIngredient[Choose.list[TYPE]]);  //��Ḧ �丮���� �κп� ���
                return;                                                 //�Լ� ����
            }
        }
        else if (*currentGold < useGold) {    //������ �ݾ��� ��� ��뿡 �ʿ��� �ݾ׺��� ���ٸ�
            printf("\033[13;3f\033[2K���� �����մϴ�.");    //�� ���� �ȳ� ���� ���
            Choose.list[TYPE] = '0'; //�÷��̾� ���� �ʱ�ȭ
            fflush(stdout); //printf ������ ���ۿ� ���� ȭ�鿡 �Ⱥ��̴� ��찡 �ֱ� ������ ��¹��۸� �����
            sleep(1);   //�ȳ� ���� ����� ���� 1�� ���

        }
        //�Է°��� ���� ��, ������ ��ᰡ ��ü ��� ��Ͽ� ����,���� ������ ���
        //�ݺ� ó������ �ٽ�
    }
}

