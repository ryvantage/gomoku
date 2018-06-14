// Name: Ryan Wilson
// UFID : 1839-3364
// Gatorlink : ryguy40
// CGS3460 - Programming in C. Final project 
// Teacher - Venkatakrishnan Ramaswamy

#include<stdio.h>

void printTable(char s[15][15]);
void startNewGame();
void loadGame();
void clrscr(void);
void play(char s[15][15], int player, int mode);
int strToCommand(char *command, int *intComm, int *charComm);
int gameover(char s[15][15], int row, int col);
void quitGame(char s[15][15], int player, int mode);
void saveGame(char s[15][15], int player, int mode);
int chartoint(char ch);

/* main() is only really seen once. It prompts the user for what
 * they would like to do. Start a new game, load a game, etc.
 * Once that is decided, main leaves and is never seen again
 * unless the user quits. */ 
int main()
{
  char yorn, tmp, c;
  char s[15][15];
  int choice, error = 0, playing = 0;

  while (playing == 0)
  {
    while (error == 0)
    {
      clrscr();
      printf("Would you like to \n"
        "1) Play a new game\n"
        "2) Load a game\n"
        "3) Exit\n"
        "4) Play the computer (Unavailable)\n");
      printf("Enter choice: ");
      fflush(stdin);
      c = getchar();
      choice = chartoint(c);
      if (choice == 1)
      {
        error = 1;
        clrscr();
        startNewGame();
      } else if (choice == 2)
      {
        error = 1;
        clrscr();
        loadGame();
      } else if (choice == 3)
        return;
      else
      {
        clrscr();
        printf("Please enter a valid input.\n");
        continue;
      }
    }
    printf("Press the enter key to continue.");
    scanf("%c", &tmp);
    error = 0;
  }
  printf("Have a nice day.\n");
}

/* startNewGame()
 * 
 * If the user decides to start a new game, this method will create 
 * a new table and send it to the play() method to play on.*/
void startNewGame()
{
  printf("Starting new game");
  int i, j;
  char s[15][15];
  for (i=0; i<15; i++)
    for (j=0; j<15; j++)
      s[i][j] = '~';

  int mode = 0;
  play(s, 0, mode);
}

/* quitGame()
 * 
 * Executes if the user presses "x" at any time during the play()
 * method. Basically, this method prompts them whether or not they
 * would like to save the game. I could have probably done away
 * with this method. Oh well. */
void quitGame(char s[15][15], int player, int mode)
{
  char yn, tmp;
  printf("Would you like to save? <y/n>: ");
  scanf("%c", &yn);
  if (yn == 'y')
  {
    saveGame(s, player, mode);
    exit(1);
  } else
    exit(1);
}

/* saveGame()
 * 
 * If the user quits (by pressing x), then they will be asked if they would
 * like to save the game. If they answer yes, then they are sent to this method.
 * This method prompts the user for a file name, opens that file, and writes
 * the status of the current game to that file. */
void saveGame(char s[15][15], int player, int mode)
{
  int i, j;
  fflush(stdin); //  Pesky newlines in the buffer go away with this fflush
  char fname[100]; // Assume user will not enter a filename > 100 chars
  printf("Enter the name of the file to be written: ");
  fgets(fname, 100, stdin);
  for (i=0; i<strlen(fname); i++)
    if (fname[i] == '\n' || fname[i] == '\t' || fname[i] == ' ')
      fname[i] = '\0';

  FILE* fp = fopen(fname, "w");
  fprintf(fp, "%d", mode);
  fprintf(fp, "\n");
  fprintf(fp, "%d", player);
  fprintf(fp, "\n\n");
  for (i=0; i<15; i++)
  {
    for (j=0; j<15; j++)
    {
      if (s[i][j] == '~')
        s[i][j] = '0';
      fprintf(fp, "%c", s[i][j]);
    }
    fprintf(fp, "\n");
  }
}

/* loadGame()
 * 
 * If user chooses to load a game, this method executes. It
 * prompts the user for a file name, asks them if they want
 * to delete the file, opens it, and returns to the play
 * method from the last point it was in. */
void loadGame()
{
  char s[15][15], yn, tmp;
  int player, mode;

  int i, j;
  fflush(stdin); //  Pesky newlines in the buffer go away with this fflush
  char fname[100]; // Assume user will not enter a filename > 100 chars
  printf("Enter the name of the file to be read: ");
  fgets(fname, 100, stdin);
  for (i=0; i<strlen(fname); i++)
    if (fname[i] == '\n' || fname[i] == '\t' || fname[i] == ' ')
      fname[i] = '\0';

  printf("Would you like to delete the save file? <y/n>");
  scanf("%c", &yn);

  FILE* fp = fopen(fname, "r");
  fscanf(fp, "%d", &mode);
  fscanf(fp, "%d", &player);
  fscanf(fp, "%c", &tmp);
  fscanf(fp, "%c", &tmp);
  for (i=0; i<15; i++)
  {
    for (j=0; j<15; j++)
    {
      s[i][j] = fgetc(fp);
      if (s[i][j] == '0')
        s[i][j] = '~';
    }
    tmp = fgetc(fp);
  }

  if (yn == 'y')
    unlink(fname);

  play(s, player, mode);

}

/* strToCommand()
 * 
 * This method converts the given string (char *command) and converts it
 * to a valid move on the grid, with *charComm and *intComm being the character
 * portion of the command and the integer portion of the command, respectively.
 * If the command is invalid, a 0 is returned, otherwise, a 1 is returned. An
 * invalid command would be anything that is not of the form "(char)(int)", with
 * char ranging from a to o, and int ranging from 1 to 15. */
int strToCommand(char *command, int *intComm, int *charComm)
{
  int boolean = 0;
  int bit1 = command[0];
  int bit2 = command[1];
  int bit3 = command[2];
  char bit4 = command[3];
  int upper = 0, lower = 0;
  if (bit1 > 64 && bit1 < 80)
    upper++;
  if (bit1 > 96 && bit1 < 112)
    lower++;
  if (upper == 0 && lower == 0)
    return 0;
  if (bit4 != '\0' && bit3 != '\0')
    return 0;
  if (upper == 1)
    *charComm = bit1 - 64;
  if (lower == 1)
    *charComm = bit1 - 96;

  bit2 = bit2 - 48;

  if (command[2] != '\0')
  {
    boolean = 1;
    bit3 = bit3 - 48;
  }

  if ((bit2 < 1) || (bit2 > 9))
    return 0;
  if (boolean == 1)
  {
    if (bit2 != 1)
      return 0;
    if ((bit3 < 0) || (bit3 > 5))
      return 0;
  }

  if (boolean == 1)
    *intComm = (bit2*10) + bit3;
  else
    *intComm = bit2;

  return 1;
}

/* play()
 * 
 * The play method is where most of the activity in this program occurs. Play
 * is responsible for calling the printTable method, determining which player
 * is next, prompting the player for input, ensuring the user enters proper
 * input, etc. */
void play(char s[15][15], int player, int mode)
{
  int playing = 1;
  int intComm, error = 0, i;
  int charComm;
  char command[5];

  while (playing == 1)
  {
    clrscr();
    printf("\n");
    printTable(s);
    if (player == 0)
      printf("Black player (x), ");
    if (player == 1)
      printf("White player (o), ");
    printf("Please enter your move: ");
    while (error == 0)
    {
      fflush(stdin);
      fgets(command, 5, stdin);
      if (command[0] == 'x')
        quitGame(s, player, mode);
      for (i=0; i<4; i++)
        if (command[i] == '\n' || command[i] == '\t' || command[i] == ' ')
          command[i] = '\0';
      error = strToCommand(command, &intComm, &charComm);

      if (error == 0)
      {
        clrscr();
        printf("\n");
        printTable(s);

        printf("Valid commands are in the format row,column. For example, if \n"
          "you want row a, column 15, the correct input would be \" a15 \".\n"
          "Ranges -- Row [a-o], Column [1-15]\n"
          "Please enter a valid input: ");
        continue;
      }
      if (s[charComm-1][intComm-1] != '~')
      {
        clrscr();
        printf("\n");
        printTable(s);

        printf("That space is already taken. Make another choice: ");
        error = 0;
        continue;
      }
    }
    if (player == 0)
      s[charComm-1][intComm-1] = 'x';
    if (player == 1)
      s[charComm-1][intComm-1] = 'o';

    playing = gameover(s, charComm, intComm);
    if (playing == 0)
      continue;

    if (player == 1)
      player--;
    else
      player++;
    error = 0;
  }
  clrscr();
  printf("\n");
  printTable(s);

  if (player == 0)
    printf("Black player wins!\n");
  if (player == 1)
    printf("White player wins!\n");

}

/* printTable()
 * 
 * For the game table, I used the most basic data structure
 * ever. Matricies are very easy to work with */
void printTable(char s[15][15])
{
  // The world's biggest printf() statement. =)
  printf("|=================================|\n"
    "| |                   1 1 1 1 1 1 |\n"
    "| | 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 |\n"
    "|-+-------------------------------|\n"
    "|a| %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c |\n"
    "|b| %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c |\n"
    "|c| %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c |\n"
    "|d| %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c |\n"
    "|e| %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c |\n"
    "|f| %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c |\n"
    "|g| %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c |\n"
    "|h| %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c |\n"
    "|i| %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c |\n"
    "|j| %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c |\n"
    "|k| %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c |\n"
    "|l| %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c |\n"
    "|m| %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c |\n"
    "|n| %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c |\n"
    "|o| %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c |\n"
    "|=================================|\n",

  s[0][0], s[0][1], s[0][2], s[0][3], s[0][4], s[0][5], s[0][6], s[0][7],
      s[0][8], s[0][9], s[0][10], s[0][11], s[0][12], s[0][13], s[0][14],

      s[1][0], s[1][1], s[1][2], s[1][3], s[1][4], s[1][5], s[1][6], s[1][7],
      s[1][8], s[1][9], s[1][10], s[1][11], s[1][12], s[1][13], s[1][14],

      s[2][0], s[2][1], s[2][2], s[2][3], s[2][4], s[2][5], s[2][6], s[2][7],
      s[2][8], s[2][9], s[2][10], s[2][11], s[2][12], s[2][13], s[2][14],

      s[3][0], s[3][1], s[3][2], s[3][3], s[3][4], s[3][5], s[3][6], s[3][7],
      s[3][8], s[3][9], s[3][10], s[3][11], s[3][12], s[3][13], s[3][14],

      s[4][0], s[4][1], s[4][2], s[4][3], s[4][4], s[4][5], s[4][6], s[4][7],
      s[4][8], s[4][9], s[4][10], s[4][11], s[4][12], s[4][13], s[4][14],

      s[5][0], s[5][1], s[5][2], s[5][3], s[5][4], s[5][5], s[5][6], s[5][7],
      s[5][8], s[5][9], s[5][10], s[5][11], s[5][12], s[5][13], s[5][14],

      s[6][0], s[6][1], s[6][2], s[6][3], s[6][4], s[6][5], s[6][6], s[6][7],
      s[6][8], s[6][9], s[6][10], s[6][11], s[6][12], s[6][13], s[6][14],

      s[7][0], s[7][1], s[7][2], s[7][3], s[7][4], s[7][5], s[7][6], s[7][7],
      s[7][8], s[7][9], s[7][10], s[7][11], s[7][12], s[7][13], s[7][14],

      s[8][0], s[8][1], s[8][2], s[8][3], s[8][4], s[8][5], s[8][6], s[8][7],
      s[8][8], s[8][9], s[8][10], s[8][11], s[8][12], s[8][13], s[8][14],

      s[9][0], s[9][1], s[9][2], s[9][3], s[9][4], s[9][5], s[9][6], s[9][7],
      s[9][8], s[9][9], s[9][10], s[9][11], s[9][12], s[9][13], s[9][14],

      s[10][0], s[10][1], s[10][2], s[10][3], s[10][4], s[10][5], s[10][6],
      s[10][7], s[10][8], s[10][9], s[10][10], s[10][11], s[10][12], s[10][13],
      s[10][14],

      s[11][0], s[11][1], s[11][2], s[11][3], s[11][4], s[11][5], s[11][6],
      s[11][7], s[11][8], s[11][9], s[11][10], s[11][11], s[11][12], s[11][13],
      s[11][14],

      s[12][0], s[12][1], s[12][2], s[12][3], s[12][4], s[12][5], s[12][6],
      s[12][7], s[12][8], s[12][9], s[12][10], s[12][11], s[12][12], s[12][13],
      s[12][14],

      s[13][0], s[13][1], s[13][2], s[13][3], s[13][4], s[13][5], s[13][6],
      s[13][7], s[13][8], s[13][9], s[13][10], s[13][11], s[13][12], s[13][13],
      s[13][14],

      s[14][0], s[14][1], s[14][2], s[14][3], s[14][4], s[14][5], s[14][6],
      s[14][7], s[14][8], s[14][9], s[14][10], s[14][11], s[14][12], s[14][13],
      s[14][14]);
}

/* gameover()
 * 
 * This method checks to see if the game is over. It returns
 * an int acting as a boolean. It takes in the coordinates of
 * the last character command (row & col) and checks the
 * spaces around it for 5 consecutive pieces. First, it checks
 * vertical (up -> down), then horizontal (left -> right),
 * then diagonal (down right -> up left), then diagonal again,
 * (down right -> up left). If at any time there are 5 consecutive
 * pieces, it returns 0 (for true). Perhaps 1 should have been true.
 * Oh well. */
int gameover(char s[15][15], int row, int col)
{
  int i=0, count = 0, j = 0;
  row--;
  col--;
  char player = s[row][col];

  // Check Vertical
  j=0;
  while ((row+j) <= 14 && s[row+j][col] == player)
    j++;
  count = 0;
  for (i=1; i<=5; i++)
    if (row-i+j >= 0)
      if (s[row-i+j][col] == player)
        count++;
  if (count == 5)
    return 0;

  // Check Horizontal
  j=0;
  while ((col+j) <= 14 && s[row][col+j] == player)
    j++;
  count = 0;
  for (i=1; i<=5; i++)
    if (col-i+j >= 0)
      if (s[row][col-i+j] == player)
        count++;
  if (count == 5)
    return 0;

  // Check Diagonal - down right -> up left
  j=0;
  while ((col+j) <= 14 && (row+j) <= 14 && s[row+j][col+j] == player)
    j++;
  count = 0;
  for (i=1; i<=5; i++)
    if (col-i+j >= 0 && row-i+j >= 0)
      if (s[row-i+j][col-i+j] == player)
        count++;
  if (count == 5)
    return 0;

  // Check Diagonal - down left -> up right
  j=0;
  while ((col-j) >= 0 && (row+j) <= 14 && s[row+j][col-j] == player)
    j++;
  count = 0;
  for (i=1; i<=5; i++)
    if (col+i-j <= 14 && row-i+j >= 0)
      if (s[row-i+j][col+i-j] == player)
        count++;
  if (count == 5)
    return 0;
  return 1;
}

// Clear Screen
void clrscr(void)
{
  printf("\033[2J");/* Clear the entire screen.*/
  printf("\033[0;0f");/* Move cursor to the top left hand corner */
}


// Converts a given char to an int
int chartoint(char ch)
{
  if ((ch > 48) && (ch < 57))
    return (int)(ch-48);
  else
    return -1;
}
