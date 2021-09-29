#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>


int bills[] = {100, 200, 500, 1000, 2000, 5000};
int cash[1000];
int counts[6] = {0, 0, 0, 0, 0, 0};
const int INF=1001;

void initReplenishment(){
    std::srand(std::time(nullptr));
    for(int i = 0; i < 1000; i++){
        cash[i] =  bills[rand() % 6];
    }
}

void replenishment(){

    std::ofstream atm("ATM.bin",std::ios::binary);

    for(int i = 0; i < 1000; i++){
        atm.write((char*) &cash[i],sizeof(bills[i]));
    }
    atm.close();
};

void calcCount(){
    for(int i = 0; i < 6; i++){
        counts[i] = 0;
    }
    for(int i = 0; i < 1000; i++) {
        switch (cash[i]) {
            case 100:
                counts[0]++;
                break;
            case 200:
                counts[1]++;
                break;
            case 500:
                counts[2]++;
                break;
            case 1000:
                counts[3]++;
                break;
            case 2000:
                counts[4]++;
                break;
            case 5000:
                counts[5]++;
                break;
        }
    }
}

void readFileContent(std::string filePath){
    std::ifstream rf(filePath,std::ios::binary);
    if(rf) {
        int i = 0;
        while((!rf.eof()&&(i < 1000))){
            int bill;
            rf.read((char*) &bill,sizeof(bill));
           // std::cout << bill << std::endl;
            cash[i] = bill;
            i++;
            }

        calcCount();

    } else {
        std::cerr << "File is not found!"<< std::endl;
    }
    rf.close();
}

void showCash(){
    for(int i = 0; i < 1000; i++){
        std::cout << cash[i] << std::endl;
    }

}


std::vector<int> alignment(int* sum){
    std::vector<int> a(6,0);
    int maxSum = 0;
    for(int i = 0; i < 6; i++){
        maxSum+=counts[i]*bills[i];

    }
    if((*sum % 100 == 0) &&(*sum <= maxSum)) {
        for (int i = 5; (i >= 0) || (*sum > 0); i--) {
            if (*sum  <= counts[i] * bills[i]) {
                a[i] = *sum / bills[i];
                *sum %= bills[i];
            } else {
                a[i] = counts[i];
                *sum -= counts[i] * bills[i];
            }

        }
    }
    return a;

}

int main() {
    std::srand(std::time(nullptr));
    char command;
    std::cout << "Input '+' if you want to replenish atm or input '-' if you want to withdraw a money" << std::endl;
    std::cin >> command;
    if (command == '+'){
        initReplenishment();
        replenishment();
        std::cout << "ATM is replenished!" << std::endl;
    } else {

        readFileContent("ATM.bin");
       // showCash();
        int maxSum = 0;
        std::cout << "ATM have" << std::endl;
        for(int i = 0; i < 6; i++){
            maxSum+=counts[i]*bills[i];
            if(counts[i]>0) std::cout << counts[i] <<" bills "<< bills[i]<<" RUB" <<std::endl;
        }
        std::cout << std::endl;
        std::cout <<"You account " << maxSum << " RUB"<< std::endl;
        std::vector<int> a(6);
        std::cout << std::endl;
        std::cout << "Input withdraw sum" << std::endl;
        int sum;
        std::cin >> sum;

       a = alignment(&sum);
        std::cout << "Bills for withdrawing:" << std::endl;
       for(int i = 0; i < a.size() ;i++){
           if(a[i]>0) std::cout << a[i] <<" bills "<< bills[i]<<" RUB" <<std::endl;
       }
        std::cout << std::endl;
       if(sum == 0){
           int k = 0;
           for(int i = 0; i < 6;i++){
               counts[i]-=a[i];
               for(int j = 0; j < counts[i]; j++){
                   cash[k] = bills[i];
                   k++;
               }
           }
           for(;k < 1000 ; k++){
               cash[k] = 0;
           }
           replenishment();
           readFileContent("ATM.bin");
           //showCash();
           maxSum = 0;
           std::cout << "ATM have" << std::endl;
           for(int i = 0; i < 6; i++){
               maxSum+=counts[i]*bills[i];
               if(counts[i]>0) std::cout << counts[i] <<" bills "<< bills[i]<<" RUB" <<std::endl;
           }
           std::cout << std::endl;
           std::cout << "You account " << maxSum << " RUB" << std::endl;
       } else {
           std::cout<< "You can't withdraw this sum!" << std::endl;
       }

      // showCash();

    }
    return 0;
}
