//TODO: error handling on cin and getline
#include <iostream>
#include <string>
#include <windows.h>
#include <fstream>
#include <vector>
#include <conio.h>
#include <iomanip>

using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

struct money {
  string name;
  int amount;
  int priority;
};

int maxNameLength_VAR;

void maxNameLength();
string toRoman(int value);
void formatAmount(int a);

//-------BUDGET----------
int budgetLine = 0;

vector<money> budget;

fstream fs_budget;

void loadBudget() {
  fs_budget.open("budget.txt", ios::in);
  while (!fs_budget.eof()) {
    budget.resize(budgetLine+1);
    fs_budget >> budget[budgetLine].name >> budget[budgetLine].amount;
    budgetLine++;
  }
  //because of empty line at the end
  budget.resize(budgetLine-1);
  budgetLine--;
  fs_budget.close();
}

void drawBudget() {
  for (int i = 0; i < budgetLine; i++) {
    SetConsoleTextAttribute(hConsole, 10);
    cout  << " " << i+1  << ".  " <<  budget[i].name << setw(10+maxNameLength_VAR-budget[i].name.length());
    formatAmount(budget[i].amount);
    SetConsoleTextAttribute(hConsole, 7);
  }
}

void addBudget(string name, int amount) {
  budget.push_back( {name, amount} );
  budgetLine++;
}

void changeBudget(string oldName, string newName, int amount) {
  for (int i = 0; i < budgetLine; i++) {
      if (!budget[i].name.empty()) {
        if (oldName == budget[i].name) {
          if (newName != "") {
            budget[i].name = newName;
          } else {
            budget[i].amount += amount;
          }
        }
      }
  }
}

void removeBudget(string name) {
  for (int i = 0; i < budgetLine; i++) {
    if (name == budget[i].name) {
      budget.erase(budget.begin()+i);
      budgetLine--;
    }
  }
}

void saveBudget() {
  fs_budget.open("budget.txt", ios::out);
  for (int i = 0; i < budgetLine; i++) {
    if (!budget[i].name.empty()) {
      fs_budget << budget[i].name << " " << budget[i].amount << endl;
    }
  }
  fs_budget.close();
}


//-------COST----------
int costLine = 0;

vector<money> cost;

fstream fs_cost;

void loadCost() {
  fs_cost.open("cost.txt", ios::in);
  while (!fs_cost.eof()) {
    cost.resize(costLine+1);
    fs_cost >> cost[costLine].name >> cost[costLine].amount >> cost[costLine].priority;
    costLine++;
  }
  //because of empty line at the end
  cost.resize(costLine-1);
  costLine--;
  fs_cost.close();
}

void drawCost() {
  //sorting by priority
  int minIndex;
  for (int i = 0; i < costLine-1; i++) {
    for (int j = i+1; j < costLine; j++) {
      minIndex = i;
      if (cost[j].priority < cost[minIndex].priority) {
        minIndex = j;
      }
      swap(cost[minIndex], cost[i]);
    }
  }

  //find largest priority number
  int pmaxValue = 0;
  for (int i = 0; i < costLine; i++) {
    if (pmaxValue < cost[i].priority) {
      pmaxValue = cost[i].priority;
    }
  }
  //draw cost
  int index = 0;
  for (int i = 0; i <= pmaxValue; i++) {
    for (int j = 0; j < costLine; j++) {
      if (cost[j].priority == i+1) {
        SetConsoleTextAttribute(hConsole, 6);
        cout << " PRIORITY " << toRoman(i+1) << endl;
        break;
      }
    }
    while (cost[index].priority == i+1 && index < costLine) {
      SetConsoleTextAttribute(hConsole, 4);
      cout << " " << index+1 << ".  " <<  cost[index].name << setw(10+maxNameLength_VAR-cost[index].name.length());
      formatAmount(cost[index].amount);
      SetConsoleTextAttribute(hConsole, 7);
      index++;
    }
  }
}

void addCost(string name, int amount, int priority) {
  cost.push_back( {name, amount, priority} );
  costLine++;
}

void payCost(string costName, string budgetName) {
  int costLoc;
  bool  costExists = false,
        budgetExists = false;
  for (int i = 0; i < costLine; i++) {
    if (!cost[i].name.empty()) {
      if (costName == cost[i].name) {
        costLoc = i;
        costExists = true;
      }
    }
  }
  if (costExists == true) {
    for (int i = 0; i < budgetLine; i++) {
      if (!budget[i].name.empty()) {
        if (budgetName == budget[i].name) {
          budget[i].amount -= cost[costLoc].amount;
          if (budget[i].amount < 0) {
            budget[i].amount += cost[costLoc].amount;
            cout << " Not enough budget";
          } else {
            budgetExists = true;
          }
        }
      }
    }
  }

  if (costExists == true && budgetExists == true) {
    cost.erase(cost.begin()+costLoc);
    costLine--;
  }
}

void changeCost(string oldName, string newName, int amount, int newPriority) {
  for (int i = 0; i < costLine; i++) {
    if (!cost[i].name.empty()) {
      if (oldName == cost[i].name) {
        if (newName != "") {
          cost[i].name = newName;
        } else if (amount != 0) {
          cost[i].amount += amount;
        } else {
          cost[i].priority = newPriority;
        }
      }
    }
  }
}

void removeCost(string name) {
  for (int i = 0; i < costLine; i++) {
    if (name == cost[i].name) {
      cost.erase(cost.begin()+i);
      costLine--;
    }
  }
}

void saveCost() {
  fs_cost.open("cost.txt", ios::out);
  for (int i = 0; i < costLine; i++) {
    fs_cost << cost[i].name << " " << cost[i].amount << " "<< cost[i].priority << endl;
  }
  fs_cost.close();
}



int totalBudget() {
  int sumBudget = 0;
  for (int i = 0; i < budgetLine; i++) {
    sumBudget += budget[i].amount;
  }
  return sumBudget;
}

int totalCost() {
  int sumCost = 0;
  for (int i = 0; i < costLine; i++) {
    sumCost += cost[i].amount;
  }
  return sumCost;
}

int balance() {
  return totalBudget() - totalCost();;
}




void maxNameLength() {
  maxNameLength_VAR = 0;
  for (int i = 0; i < budgetLine; i++) {
    if (maxNameLength_VAR < budget[i].name.length()) {
      maxNameLength_VAR = budget[i].name.length();
    }
  }
  for (int i = 0; i < costLine; i++) {
    if (maxNameLength_VAR < cost[i].name.length()) {
      maxNameLength_VAR = cost[i].name.length();
    }
  }
}

void formatAmount(int a) {
  int c = 1;

  if (a < 0) {
    a *= -1;
    cout << "-";
  }
  while ((c *= 1000) < a);
  while (c > 1) {
    int t = (a % c) / (c / 1000);
    cout << (((c > a) || (t > 99)) ? "" : ((t > 9) ? "0" : "00")) << t;
    cout << (((c /= 1000) == 1) ? " HUF\n" : ",");
  }
}

string toRoman(int value) {
    struct romandata_t {
      int value;
      char const* numeral;
    };

    const struct romandata_t romandata[] = {
        {1000, "M"}, {900, "CM"},
        {500, "D"}, {400, "CD"},
        {100, "C"}, { 90, "XC"},
        { 50, "L"}, { 40, "XL"},
        { 10, "X"}, { 9, "IX"},
        { 5, "V"}, { 4, "IV"},
        { 1, "I"},
        { 0, NULL} // end marker
    };

    string result;
    for (const romandata_t* current = romandata; current->value > 0; ++current) {
      while (value >= current->value) {
        result += current->numeral;
        value -= current->value;
      }
    }
    return result;
}



void drawData() {
  cout << "My Budget, Total: ";
  formatAmount(totalBudget());
  cout <<" No. Name" << setw(13+maxNameLength_VAR) << "Amount\n";
  drawBudget();
  cout << "\nMy Cost, Total: ";
  formatAmount(totalCost());
  drawCost();
  cout << "\nBalance: ";
  if (balance() < 0) {
    SetConsoleTextAttribute(hConsole, 4);
  } else if (balance() > 0) {
   SetConsoleTextAttribute(hConsole, 10);
   cout << "+";
  }
  formatAmount(balance());
  SetConsoleTextAttribute(hConsole, 7);

}


int main() {
  char option;
  string  name,
          newName,
          budgetName;
  int amount,
      priority,
      newPriority;


  loadBudget();
  loadCost();
  maxNameLength();
  do {
MAIN_MENU:
//LAYER 0
    system("cls");
    drawData();
    cout << "\nMAIN MENU\n 1 - Budget\n 2 - Cost\n 3 - Log\n Esc - Escape\n\n";
    fflush(stdin);
    option = _getch();
    if (option == 49) {
      do {
    BUDGET_MENU:
    //LAYER 1
        system("cls");
        drawData();
        cout << "\nBUDGET:\n 1 - Add new budget\n 2 - Change budget\n 3 - Remove budget\n Esc - Back\n\n";
        fflush(stdin);
        option = _getch();
        if (option == 49) {
          cout << " Name: ";
          getline(cin, name);
          cout << " Amount: ";
          cin >> amount;
          addBudget(name, amount);
        }else if (option == 50) {
          do {
            system("cls");
            drawData();
            cout << "\nCHANGE BUDGET\n 1 - Change name\n 2 - Add amount\n Esc - Back\n\n";
            fflush(stdin);
            option = _getch();
            //LAYER 2
            if (option == 49) {
              cout << " Name: ";
              getline(cin, name);
              cout << " New name: ";
              getline(cin, newName);
              changeBudget(name, newName, 0);
            } else if (option == 50) {
              cout << " Name: ";
              getline(cin, name);
              cout << " Amount: ";
              cin >> amount;
              changeBudget(name, "", amount);
            } else if (option == 27) {
              goto BUDGET_MENU;
            }
          } while(true);
        } else if (option == 51) {
          cout << " Name: ";
          getline(cin, name);
          removeBudget(name);
        } else if (option == 27) {
          goto MAIN_MENU;
        }
      } while(true);
    } else if (option == 50) {
      do {
      COST_MENU:
      //LAYER 1
        system("cls");
        drawData();
        cout << "\nCOST:\n 1 - Add new cost\n 2 - Pay cost\n 3 - Change cost\n 4 - Remove cost\n Esc - Back\n\n";
        fflush(stdin);
        option = _getch();
        if (option == 49) {
          cout << " Name: ";
          getline(cin, name);
          cout << " Amount: ";
          cin >> amount;
          cout << " Priority: ";
          cin >> priority;
          addCost(name, amount, priority);
        } else if (option == 50) {
          cout << " Cost's name: ";
          getline(cin, name);
          cout << " Budget's name: ";
          cin >> budgetName;
          payCost(name, budgetName);
        } else if (option == 51) {
          do {
            system("cls");
            drawData();
            cout << "\nCHANGE COST\n 1 - Change name\n 2 - Add amount\n 3 - Change priority\n Esc - Back\n\n";
            fflush(stdin);
            option = _getch();
            if (option == 49) {
              cout << " Name: ";
              getline(cin, name);
              cout << " New name: ";
              getline(cin, newName);
              changeCost(name, newName, 0, 0);
            } else if (option == 50) {
              cout << " Name: ";
              getline(cin, name);
              cout << " Amount: ";
              cin >> amount;
              changeCost(name, "", amount, 0);
            } else if (option == 51) {
              cout << " Name: ";
              getline(cin, name);
              cout << " New priority: ";
              cin >> newPriority;
              changeCost(name, "", 0, newPriority);
            } else if (option == 27) {
              goto COST_MENU;
            }
          } while(true);
        } else if (option == 52) {
          cout << " Name: ";
          getline(cin, name);
          removeCost(name);
        } else if (option == 27) {
          goto MAIN_MENU;
        }
      } while(true);
    } else if (option == 51) {
      /* code */
    }
  } while(option != 27);
  saveBudget();
  saveCost();
  return 0;
}
