#include <iostream>
#include <random>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
using namespace std;
// declare variables:
uint16_t randnum;
uint8_t sumofd;
uint8_t darray[5];
uint16_t revnum;
uint8_t gamemode;

// declare functions:
void hinttime(uint8_t hintsel);
int findGCD(uint16_t a, uint8_t b);
void ansroundzen();
void ansroundtimed();
void timer(uint8_t sec, std::mutex& mtx, std::condition_variable& cv, bool& sessionactive);

void gamestarted()
{
  cout << "\nSession started!\n";
  cout << "Generating a random number...\n";

  // number generator

  // Use a random_device to seed the random number generator
  random_device rd;

  // Use the random_device to seed the random number engine
  mt19937 gen(rd());

  // Define the range of the random numbers (0 to 65535)
  uniform_int_distribution<int> distribution(0, 65535);

  // Generate a random integer
  randnum = distribution(gen);

  // uncomment the statement below only for debugging
  // std::cout << "Number generated is: " << randnum << std::endl;

  // generate digit array for use later
  darray[0] = randnum / 10000;
  darray[1] = (randnum / 1000) % 10;
  darray[2] = (randnum / 100) % 10;
  darray[3] = (randnum / 10) % 10;
  darray[4] = randnum % 10;

  // calc sum of digits and reversed int beforehand to save resources

  sumofd = (darray[0] + darray[1] + darray[2] + darray[3] + darray[4]);
  revnum = (darray[0] + darray[1] * 10 + darray[2] * 100 + darray[3] * 1000 + darray[4] * 10000);

  // hint loop
  hinttime(0);

  uint8_t hintcount = 0;
  char ans;

  while (true)
  {

    hintcount++;

    // end loop when user answers no
    if (ans == 'n' || hintcount == 4)
    {

      break;
    }
    else
    {

      cout << "Do you want any more hints? (y/n)\n";
      cin >> ans;

      if (ans != 'n')
      {
        hinttime(hintcount);
      };
    };
  };
  if (gamemode == '0')
  {
    ansroundzen();
  }
  else
  {
    ansroundtimed();
  }
};

// using euclidean algorithm to find the greatest common divisor
int findGCD(uint16_t a, uint8_t b)
{
  while (b != 0)
  {
    uint8_t temp = b;
    b = a % b;
    a = temp;
  }
  return a;
};

void hinttime(uint8_t hintsel)
{
  switch (hintsel)
  {
    uint8_t GCD;
  case 3:
    cout << "4th Hint: The last digit of the number is " << (randnum % 10) << endl;
    break;

  case 2:
    cout << "3rd Hint: The sum of the digits of the number is " << static_cast<unsigned int>(sumofd) << endl;
    break;

  case 1:
    GCD = findGCD(randnum, sumofd);
    cout << "2nd Hint: The number divided by the sum of its digits is " << (randnum / GCD) << '/' << (sumofd / GCD) << endl;
    break;

  default:
    cout << "1st Hint: Reversing the digits of the number and then subtracting the reversed number from the original gives " << (randnum - revnum) << endl;
  };
};

void ansroundzen()
{
  uint16_t ans;
  while (true)
  {
    cout << "\nType your answers!\n";
    cin >> ans;

    // check if int matches
    if (ans == randnum)
    {
      cout << "Good Job! You got it correct!\n";
      break;
    }
    else
    {
      cout << "Oops, that's not the number, please try again.\n";
    };
  };
};

void ansroundtimed() {
  // start session and define local-scoped variables
  mutex mtx;
  condition_variable cv;
  uint8_t sesdur;
  bool sessionactive = true;
  bool gcorectly = false;
  uint16_t ans;

  // switch statement
  switch (gamemode) {
    case '1':
     // 3 min - easy
     sesdur = 180;
     break;
    case '2':
     // 2 min - normal
     sesdur = 120;
     break;
    case '3':
     // 30 sec - hard
     sesdur = 30;
     break;
    case '4':
     // 10 sec - insane
     sesdur = 10;
     break;
    case '5':
     // 1 sec - ???
     sesdur = 1;
     break;
  };

  // inform user on how long session is
  if (gamemode == '1' || gamemode == '2' ) {
     cout << "You have " << static_cast<unsigned int>((sesdur/60)) << " minutes to guess the number.\n";
  } else {
     cout << "You have " << static_cast<unsigned int>(sesdur) << " seconds to guess the number.\n";
  };

  thread timerThread(timer, sesdur, ref(mtx), ref(cv), ref(sessionactive));

  while (sessionactive || !(gcorectly)) {
    cout << "\nType your answers!\n";
    cin >> ans;

    // check if int matches
    if (ans != randnum)
    {
      cout << "Good Job! You got it correct!\n";
      gcorectly = true;
    }
    else
    {
      cout << "Oops, that's not the number, please try again.\n";
    };
  };

  // Unlock and end session
  unique_lock<mutex> lock(mtx);
  lock.unlock();
  cv.notify_one();
  timerThread.join();
  if(sessionactive == false) {
    cout << "yee\n";
  } else {
    cout << "nee\n";
  };
};

void timer(uint8_t sec, mutex& mtx, condition_variable& cv, bool& sessionactive) {
    // set lock and wait
    unique_lock<mutex> lock(mtx);
    sessionactive = cv.wait_for(lock, chrono::seconds(sec), [&]() { return false; });

};

// starting prompt:
int main()
{

  char start;
  cout << "RiddleMeInt, version 1.0.0 \n";
  cout << "By Ichigojamu (GarethTacos)\n";

  cout << "Start session? (y/n)\n";

  cin >> start;

  if (start == 'y')
  {
    cout << "\nGamemode Selection\n";
    cout << "Enter a number to select a gamemode:\n\n";
    cout << "(0) Zen\n";
    cout << "(1) Easy/3 Min\n";
    cout << "(2) Normal/2 Min\n";
    cout << "(3) Hard/30 Sec\n";
    cout << "(4) Insane/10 Sec\n";

    cin >> gamemode;

    cout << "Option " << gamemode << " was selected.\n";

    gamestarted();
  };

  return 0;
};